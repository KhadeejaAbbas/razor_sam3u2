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
static u8 title[] = "Milo the monkey!";
static u16 u16WaitCount;
static u16 u16WaitCount2;
static u16 u16WaitCount3;
static u16 u16WaitCount4;

static u16 u16Counter;
static u16 u16Blinking;
static u8 u8Awake;

static u8 u16DontClickTooSoon;
static u8 start[] = "Can you handle Milo?";
static u8 flag;
static u8 miloAwake;
static u8 eating;
static u8 fish1;
static u8 ate;
static u8 win;
static u8 wakeUpPart;


// static int randomBit;

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

  // PWMAudioSetFrequency(BUZZER1, 500);




    u16WaitCount = 0;
    u16WaitCount2 = 0;
    u16WaitCount3 = 0;
    u16WaitCount4 = 0;

    u16DontClickTooSoon = 0;
    u16Counter = 0;

    flag = 0;
    miloAwake = 0;
    eating = 0;
    fish1 = 0;
    ate = 0;
    win = 0;

    u16Blinking = 0;
    wakeUpPart = 0;

    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR+3, title);
    LedOff(LCD_RED);
    LedOff(LCD_BLUE);
    LedOff(LCD_GREEN);

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
  u16WaitCount++;

  if (u16WaitCount > 1000){ //change this to 5000 instead of 1000

    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR, start);
    LcdMessage(LINE2_START_ADDR + 6, "Yes");
    LcdMessage(LINE2_START_ADDR + 13, "No");
      UserApp1_pfStateMachine = UserApp1SM_Idle;
      }
}

static void UserApp1SM_WaitingScreenG(void){
  u16WaitCount3++;
  if (u16WaitCount3 > 2500){ //change this to 5000 instead of 1000
    LcdCommand(LCD_CLEAR_CMD);
    char mouth = 0xDB;
    char value2 = 0xF4;
    char value = 0xF3;
    eating = 1;
    if (win == 1 && (u16WaitCount4 >= 1)){
      LcdCommand(LCD_CLEAR_CMD);
      LedOn(GREEN);
      LcdMessage(LINE1_START_ADDR, "    Good Job!");
      LcdMessage(LINE2_START_ADDR + 7, "You did it!");
    }

    else if (ate == 1){
        LedOn(YELLOW);
        LcdMessage(LINE1_START_ADDR+7, "-   -   ( ");
        LcdMessage(LINE2_START_ADDR, "!     (@ _ @)  )"); 
        win = 1;
        u16WaitCount4++;

      
    }

    else if (ate == 0) {
      LcdMessage(LINE1_START_ADDR+7, &value);
      LcdMessage(LINE1_START_ADDR+11, &value2);
      LcdMessage(LINE1_START_ADDR+12, "   ( ");
      LcdMessage(LINE2_START_ADDR, "      (@ _ @)  )"); 
    }
    
    if (fish1 == 0){
      LcdMessage(LINE1_START_ADDR, "><>");
    }
    else if (fish1 == 1 && ate == 0){
      LedOn(ORANGE);
      LcdMessage(LINE2_START_ADDR, "?     (@ "); 
      LcdMessage(LINE2_START_ADDR+9, &mouth);

      LcdMessage(LINE2_START_ADDR+10, " @)  )"); 
      ate = 1;
      
    }
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }  
}



static void UserApp1SM_GameScreen(void){
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR+3, "Feed Angry Milo!");
    LcdMessage(LINE2_START_ADDR + 9, "><>");
    UserApp1_pfStateMachine = UserApp1SM_WaitingScreenG;
}

static void UserApp1SM_WaitingScreenP(void){
  u16WaitCount2++;
  if (u16WaitCount2 > 2500){ //change this to 5000 instead of 1000

    u16Counter++;
    u16Blinking++;
    LcdCommand(LCD_CLEAR_CMD);


    if (u8Awake < 3){
      LcdMessage(LINE1_START_ADDR+7, "-   -   ( ");
      LcdMessage(LINE2_START_ADDR+6, "(- _ -)  )");
      if ((u16Blinking >=5) & (u8Awake < 3)){
          u16Blinking = 0;
      }              

          if(u16Blinking > 3){  
                u8Awake++;
                LcdMessage(LINE2_START_ADDR+6, "(@ _ @)  )"); 
          }

    }
    else {
        char value2 = 0xF4;
        char value = 0xF3;

        LcdMessage(LINE1_START_ADDR+7, &value);
        LcdMessage(LINE1_START_ADDR+11, &value2);
        LcdMessage(LINE1_START_ADDR+12, "   ( ");
        LcdMessage(LINE2_START_ADDR, "!     (@ _ @)  )"); 
        miloAwake = 1;
        wakeUpPart = 2;

        // UserApp1_pfStateMachine = UserApp1SM_GameScreen;

    }
    UserApp1_pfStateMachine = UserApp1SM_Idle;


  }  
}


static void UserApp1SM_WakeUp(void){
    LcdCommand(LCD_CLEAR_CMD);
    LcdMessage(LINE1_START_ADDR+6, "Wake Milo");
    LcdMessage(LINE2_START_ADDR + 9, "Up!");
    wakeUpPart = 1;
    UserApp1_pfStateMachine = UserApp1SM_WaitingScreenP;
      
}



/* What does this state do? */
static void UserApp1SM_Idle(void)
{

  // if(IsButtonPressed(BUTTON0)){
  //   PWMAudioOn(BUZZER1);
  // }
  // else{
  //   PWMAudioOff(BUZZER1);
  // }





    if (eating == 1 && fish1 == 0){

        if (WasButtonPressed(BUTTON0)){
          ButtonAcknowledge(BUTTON0);
          fish1 = 1;
          LedOff(LCD_RED);
          LedOn(LCD_GREEN);
          UserApp1_pfStateMachine = UserApp1SM_WaitingScreenG;

        }
      }

    else if (WasButtonPressed(BUTTON0) && wakeUpPart == 2){
      ButtonAcknowledge(BUTTON0);
      if (miloAwake == 1 && fish1 == 0){
        LedOff(LCD_GREEN);
        LedOn(RED);
        LedOn(LCD_RED);
        UserApp1_pfStateMachine = UserApp1SM_GameScreen;
      }
      else if (fish1 == 1){
        LedOff(LCD_RED);
        LedOn(LCD_GREEN);
        UserApp1_pfStateMachine = UserApp1SM_GameScreen;
      }
    }
    else if((WasButtonPressed(BUTTON2) | WasButtonPressed(BUTTON3) | WasButtonPressed(BUTTON1)) && (wakeUpPart == 2) && (fish1 == 0)){
      ButtonAcknowledge(BUTTON1);
      ButtonAcknowledge(BUTTON2);
      ButtonAcknowledge(BUTTON3);
      LedOff(LCD_GREEN);
      LedOn(LCD_RED);
    }
    // else if((WasButtonPressed(BUTTON1) | WasButtonPressed(BUTTON0)))
    else if((WasButtonPressed(BUTTON1) && miloAwake == 0))
    {
      LedOff(LCD_RED);
      LedOn(LCD_GREEN);
      LedOn(WHITE);
      ButtonAcknowledge(BUTTON1);
      if (u8Awake == 1){
        LedOn(PURPLE);
      }
      if (u8Awake == 2){
        LedOn(BLUE);
      }
      if (u8Awake == 3){
        LedOn(CYAN);
      }
      UserApp1_pfStateMachine = UserApp1SM_WakeUp;     
      }
    
    
    else if((WasButtonPressed(BUTTON2))& (u16Counter == 0)){
      ButtonAcknowledge(BUTTON3);
      ButtonAcknowledge(BUTTON2);

      LcdCommand(LCD_CLEAR_CMD);

      LcdMessage(LINE1_START_ADDR, "    Aww too bad...");
      LcdMessage(LINE2_START_ADDR + 9, ":(");

      LedOn(LCD_RED);

    }
    else if((WasButtonPressed(BUTTON2) | WasButtonPressed(BUTTON3) | WasButtonPressed(BUTTON0)) && (wakeUpPart == 1)){
      ButtonAcknowledge(BUTTON3);
      ButtonAcknowledge(BUTTON2);
      ButtonAcknowledge(BUTTON0);

      LedOff(LCD_GREEN);
      LedOn(LCD_RED);
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
