/**
 * @file keypad.c
 * @author Embedded Laboratory
 * @date June 16, 2016
 * @brief Keypad Sense related functions and Algorithm.
 * 
 */

#include "keypad.h"

static Keypad_s s_keypad;             /**< Keypad Structure.*/
static u8_t KeyPressTable[MAX_ROW][MAX_COL] = {
  '1','2','3','A',
  '4','5','6','B',
  '7','8','9','C',
  '*','0','#','D'
};  /**< Key Look-Up Table.*/

/* Private Functions */
static u8_t _Process_Keypress( void );
static u8_t _Sense_Keypress( void );

/**
 * @brief Initialize Matrix Keyboard.
 *
 * Initialize 4x4 Matrix Keypad.
 * 
 */
void Initialize_Keypad( void )
{
  // Initialize Rows as Output
  ROW_1_DIR = 0;
  ROW_2_DIR = 0;
  ROW_3_DIR = 0;
  ROW_4_DIR = 0;
  ROW_1_PIN = 0;
  ROW_2_PIN = 0;
  ROW_3_PIN = 0;
  ROW_4_PIN = 0;
  
  // Enable Pull-Up of Columns or place hardware pull-ups
  
  // Initialize Columns as Input
  COL_1_DIR = 1;
  COL_2_DIR = 1;
  COL_3_DIR = 1;
  COL_4_DIR = 1;
}

/**
 * @brief Get Pressed Key Value.
 *
 * This function returns the key pressed on the Matrix Keypad.
 * return Pressed Key Value Stored in Config.
 * @note This function returns 0u/NO_KEY if no key press is detected.
 */
u8_t getKey( void )
{
  u8_t key;
  key = _Process_Keypress();
  if(key == NO_KEYs)
  {
    key = NO_KEY;
  }
  return key;
}

/**
 * @brief Scan Key Press.
 *
 * This is a private function, this returns the pressed key, but it don't care
 * about anything else, like debouncing and other things.
 * return Pressed Key Value Stored in Config.
 * @note This function returns 0xff/NO_KEYs if no key press is detected.
 */
static u8_t _Sense_Keypress( void )
{
  u8_t row = 0u, col = 0u;
  u8_t keypress = NO_KEYs;
  if( !COL_1_PIN )
    col = 1u;
  if( !COL_2_PIN )
    col = 2u;
  if( !COL_3_PIN )
    col = 3u;
  if( !COL_4_PIN )
    col = 4u;
  
  // If Some Key is Pressed
  if( col )
  {
    for( row=1u; row<=MAX_ROW; row++ )
    {
      switch(row)
      {
      case 1:
        // Ground Row-1
        ROW_1_PIN = 0;
        ROW_2_PIN = 1;
        ROW_3_PIN = 1;
        ROW_4_PIN = 1;
        break;
      case 2:
        // Ground Row-2
        ROW_1_PIN = 1;
        ROW_2_PIN = 0;
        ROW_3_PIN = 1;
        ROW_4_PIN = 1;
        break;
      case 3:
        // Ground Row-3
        ROW_1_PIN = 1;
        ROW_2_PIN = 1;
        ROW_3_PIN = 0;
        ROW_4_PIN = 1;
        break;
      case 4:
        // Ground Row-4
        ROW_1_PIN = 1;
        ROW_2_PIN = 1;
        ROW_3_PIN = 1;
        ROW_4_PIN = 0;
        break;
      }
      // Scan Column
      if( !COL_1_PIN )
        break;
      if( !COL_2_PIN )
        break;
      if( !COL_3_PIN )
        break;
      if( !COL_4_PIN )
        break;
    }  
    // Reset Row Values
    ROW_1_PIN = 0;
    ROW_2_PIN = 0;
    ROW_3_PIN = 0;
    ROW_4_PIN = 0;

    if( row && col )
    {
      if( row <= MAX_ROW && col <= MAX_COL )
        keypress = KeyPressTable[row-1][col-1];
    }
  }
  return keypress;
}

/**
 * @brief Process Detected Key Press.
 *
 * This is a private function, it process the detected key press, anc checks its
 * validity.
 * return Pressed Key Value Stored in Config.
 * @note This function returns 0xff/NO_KEYs if no key press is detected.
 */
static u8_t _Process_Keypress( void )
{
  s_keypad.keyPressed = _Sense_Keypress();
  switch( s_keypad.keypad_state )
  {
  case KEYPAD_UP:
    if( s_keypad.keyPressed != NO_KEYs )
    {
      s_keypad.keySensed = s_keypad.keyPressed;
      s_keypad.keyStatus_timeStamp = millis();
      s_keypad.keypad_state = KEYPAD_DEBOUNCE;
    }
    else
    {
      s_keypad.keySensed = NO_KEYs;
    }
    break;
  case KEYPAD_DEBOUNCE:
    if(s_keypad.keyPressed != NO_KEYs)
    {
      if(s_keypad.keyPressed == s_keypad.keySensed )
      {
        if( (millis() - s_keypad.keyStatus_timeStamp ) > KEYPAD_DEBOUNCE_TIME )
        {
          s_keypad.keypad_state = KEYPAD_PRESSED;
          s_keypad.keyStatus_timeStamp = millis();
          return s_keypad.keySensed;
        }
      }
      else
      {
        s_keypad.keyStatus_timeStamp = millis();
        s_keypad.keySensed = s_keypad.keyPressed;
      }
    }
    else
    {
      s_keypad.keypad_state = KEYPAD_UP;
      s_keypad.keySensed = NO_KEYs;
    }
    break;
  case KEYPAD_PRESSED:
    if( s_keypad.keyPressed != NO_KEYs )
    {
      if( s_keypad.keySensed == s_keypad.keyPressed )
      {
        s_keypad.keypad_state= KEYPAD_DOWN;
      }
      else
      {
        s_keypad.keypad_state= KEYPAD_DEBOUNCE;
      }
    }
    else
    {
      s_keypad.keypad_state= KEYPAD_RELEASED;
    }
    s_keypad.keyStatus_timeStamp = millis();
    break;
  case KEYPAD_DOWN:
    if(s_keypad.keySensed == s_keypad.keyPressed )
    {
      if((millis() - s_keypad.keyStatus_timeStamp) > KEYPAD_HOLD_TIME )
      {
        s_keypad.keypad_state = KEYPAD_HELD;
        s_keypad.keyStatus_timeStamp = millis();
      }
    }
    else
    {
      s_keypad.keypad_state = KEYPAD_RELEASED;
      s_keypad.keyStatus_timeStamp = millis();
    }
    break;
  case KEYPAD_HELD:
    if( s_keypad.keySensed != s_keypad.keyPressed )
    {
      s_keypad.keypad_state = KEYPAD_RELEASED;
      s_keypad.keyStatus_timeStamp = millis();
      return s_keypad.keySensed;
    }
    else if( millis() - s_keypad.keyStatus_timeStamp > KEYPAD_REPEAT_TIME )
    {
      s_keypad.keyStatus_timeStamp = millis();
      return s_keypad.keySensed;
    }
    break;
  case KEYPAD_RELEASED:
    if( s_keypad.keyPressed == NO_KEYs )
    {
      s_keypad.keypad_state = KEYPAD_UP;
    }
    else
    {
      s_keypad.keypad_state = KEYPAD_DEBOUNCE;
      s_keypad.keySensed = s_keypad.keyPressed;
      s_keypad.keyStatus_timeStamp = millis();
    }
    break;
  default:
    s_keypad.keypad_state = KEYPAD_UP;
    s_keypad.keyPressed = 0u;
    s_keypad.keySensed = 0u;
    s_keypad.keyStatus_timeStamp = 0u;
    break;
  }
  return NO_KEYs;
}
