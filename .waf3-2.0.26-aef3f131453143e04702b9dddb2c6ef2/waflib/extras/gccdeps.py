#! /usr/bin/env python
# encoding: utf-8
# WARNING! Do not edit! https://waf.io/book/index.html#_obtaining_the_waf_file

import os,re,threading
from waflib import Task,Logs,Utils,Errors
from waflib.Tools import asm,c,c_preproc,cxx
from waflib.TaskGen import before_method,feature
lock=threading.Lock()
gccdeps_flags=['-MD']
if not c_preproc.go_absolute:
	gccdeps_flags=['-MMD']
supported_compilers=['gas','gcc','icc','clang']
re_o=re.compile(r"\.o$")
re_splitter=re.compile(r'(?<!\\)\s+')
def remove_makefile_rule_lhs(line):
	rulesep=': '
	sep_idx=line.find(rulesep)
	if sep_idx>=0:
		return line[sep_idx+2:]
	else:
		return line
def path_to_node(base_node,path,cached_nodes):
	if getattr(path,'__hash__'):
		node_lookup_key=(base_node,path)
	else:
		node_lookup_key=(base_node,os.path.sep.join(path))
	try:
		node=cached_nodes[node_lookup_key]
	except KeyError:
		with lock:
			try:
				node=cached_nodes[node_lookup_key]
			except KeyError:
				node=cached_nodes[node_lookup_key]=base_node.find_resource(path)
	return node
def post_run(self):
	if not self.__class__.__name__ in self.env.ENABLE_GCCDEPS:
		return super(self.derived_gccdeps,self).post_run()
	deps_filename=self.outputs[0].abspath()
	deps_filename=re_o.sub('.d',deps_filename)
	try:
		deps_txt=Utils.readf(deps_filename)
	except EnvironmentError:
		Logs.error('Could not find a .d dependency file, are cflags/cxxflags overwritten?')
		raise
	deps_txt='\n'.join([remove_makefile_rule_lhs(line)for line in deps_txt.splitlines()])
	deps_txt=deps_txt.replace('\\\n','')
	dep_paths=deps_txt.strip()
	dep_paths=[x.replace('\\ ',' ')for x in re_splitter.split(dep_paths)if x]
	resolved_nodes=[]
	unresolved_names=[]
	bld=self.generator.bld
	try:
		cached_nodes=bld.cached_nodes
	except AttributeError:
		cached_nodes=bld.cached_nodes={}
	for path in dep_paths:
		node=None
		if os.path.isabs(path):
			node=path_to_node(bld.root,path,cached_nodes)
		else:
			base_node=getattr(bld,'cwdx',bld.bldnode)
			path=[k for k in Utils.split_path(path)if k and k!='.']
			while'..'in path:
				idx=path.index('..')
				if idx==0:
					path=path[1:]
					base_node=base_node.parent
				else:
					del path[idx]
					del path[idx-1]
			node=path_to_node(base_node,path,cached_nodes)
		if not node:
			raise ValueError('could not find %r for %r'%(path,self))
		if id(node)==id(self.inputs[0]):
			continue
		resolved_nodes.append(node)
	Logs.debug('deps: gccdeps for %s returned %s',self,resolved_nodes)
	bld.node_deps[self.uid()]=resolved_nodes
	bld.raw_deps[self.uid()]=unresolved_names
	try:
		del self.cache_sig
	except AttributeError:
		pass
	Task.Task.post_run(self)
def scan(self):
	if not self.__class__.__name__ in self.env.ENABLE_GCCDEPS:
		return super(self.derived_gccdeps,self).scan()
	resolved_nodes=self.generator.bld.node_deps.get(self.uid(),[])
	unresolved_names=[]
	return(resolved_nodes,unresolved_names)
def sig_implicit_deps(self):
	if not self.__class__.__name__ in self.env.ENABLE_GCCDEPS:
		return super(self.derived_gccdeps,self).sig_implicit_deps()
	bld=self.generator.bld
	try:
		return self.compute_sig_implicit_deps()
	except Errors.TaskNotReady:
		raise ValueError("Please specify the build order precisely with gccdeps (asm/c/c++ tasks)")
	except EnvironmentError:
		for x in bld.node_deps.get(self.uid(),[]):
			if not x.is_bld()and not x.exists():
				try:
					del x.parent.children[x.name]
				except KeyError:
					pass
	key=self.uid()
	bld.node_deps[key]=[]
	bld.raw_deps[key]=[]
	return Utils.SIG_NIL
def wrap_compiled_task(classname):
	derived_class=type(classname,(Task.classes[classname],),{})
	derived_class.derived_gccdeps=derived_class
	derived_class.post_run=post_run
	derived_class.scan=scan
	derived_class.sig_implicit_deps=sig_implicit_deps
for k in('asm','c','cxx'):
	if k in Task.classes:
		wrap_compiled_task(k)
@before_method('process_source')
@feature('force_gccdeps')
def force_gccdeps(self):
	self.env.ENABLE_GCCDEPS=['asm','c','cxx']
def configure(conf):
	if not getattr(conf.options,'enable_gccdeps',True):
		return
	global gccdeps_flags
	flags=conf.env.GCCDEPS_FLAGS or gccdeps_flags
	if conf.env.ASM_NAME in supported_compilers:
		try:
			conf.check(fragment='',features='asm force_gccdeps',asflags=flags,compile_filename='test.S',msg='Checking for asm flags %r'%''.join(flags))
		except Errors.ConfigurationError:
			pass
		else:
			conf.env.append_value('ASFLAGS',flags)
			conf.env.append_unique('ENABLE_GCCDEPS','asm')
	if conf.env.CC_NAME in supported_compilers:
		try:
			conf.check(fragment='int main() { return 0; }',features='c force_gccdeps',cflags=flags,msg='Checking for c flags %r'%''.join(flags))
		except Errors.ConfigurationError:
			pass
		else:
			conf.env.append_value('CFLAGS',flags)
			conf.env.append_unique('ENABLE_GCCDEPS','c')
	if conf.env.CXX_NAME in supported_compilers:
		try:
			conf.check(fragment='int main() { return 0; }',features='cxx force_gccdeps',cxxflags=flags,msg='Checking for cxx flags %r'%''.join(flags))
		except Errors.ConfigurationError:
			pass
		else:
			conf.env.append_value('CXXFLAGS',flags)
			conf.env.append_unique('ENABLE_GCCDEPS','cxx')
def options(opt):
	raise ValueError('Do not load gccdeps options')
