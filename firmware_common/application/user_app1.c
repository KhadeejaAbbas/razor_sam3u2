/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
static u8 title[] = "  Milo the monkey!";
static u16 u16WaitCount;
static u8 u16DontClickTooSoon;
static u8 start[] = "Can you handle Milo?";

//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */



/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

static void UserApp1SM_Title(void);
static void UserApp1SM_ChooseGame(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{


    u16WaitCount = 0;
    u16DontClickTooSoon = 0;
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, title);

          // LcdMessage(LINE2_START_ADDR, start);
          // LcdMessage(LINE2_START_ADDR, au8Message);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Title;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
static void UserApp1SM_Title(void){
  // LedOn(GREEN);
  u16WaitCount++;

  if (u16WaitCount > 1000){ //change this to 5000 instead of 1000
    // LedOff(GREEN);
    // LedOn(RED);
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, start);
    LcdMessage(LINE2_START_ADDR + 5, "Yes");
    LcdMessage(LINE2_START_ADDR + 13, "No");
      UserApp1_pfStateMachine = UserApp1SM_ChooseGame;
      }
}

static void UserApp1SM_ChooseGame(void){
    u16DontClickTooSoon++;
    if (u16DontClickTooSoon > 250){
        UserApp1_pfStateMachine = UserApp1SM_Idle;
    }

}
/* What does this state do? */
static void UserApp1SM_Idle(void)
{

    if((WasButtonPressed(BUTTON1) | WasButtonPressed(BUTTON0)))
    {
      int flag = 1;
      ButtonAcknowledge(BUTTON0);
      ButtonAcknowledge(BUTTON1);

      LedOn(PURPLE); // now what  u have to do is create a new .c and .h file for the game screen with MILO! i created a file called 'homeScreen'
      LcdCommand(LCD_CLEAR_CMD);
      while(flag==1){
              LcdMessage(LINE1_START_ADDR, "    Aww too bad...");

      }


    
    }
    else if((WasButtonPressed(BUTTON2) | WasButtonPressed(BUTTON3))){
      ButtonAcknowledge(BUTTON3);
      ButtonAcknowledge(BUTTON2);
      LedOn(WHITE);
      LedOn(RED);
      LcdCommand(LCD_CLEAR_CMD);

      LcdMessage(LINE1_START_ADDR, "    Aww too bad...");
      LcdMessage(LINE2_START_ADDR + 9, ":(");


    }


} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
