/**
 * @file keypad.h
 * @author Embedded Laboratory
 * @date June 16, 2016
 * @brief Matrix Keypad Related Macros and Function Prototypes.
 *
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "config.h"

#define MAX_ROW         4                 /**< Maximum Row.*/
#define MAX_COL         4                 /**< Maximum Column.*/

#define ROW_1_PIN       PORTBbits.RB0     /**< Row 1 Pin Number.*/
#define ROW_1_DIR       TRISBbits.TRISB0  /**< Row 1 Direction.*/
#define ROW_2_PIN       PORTBbits.RB1     /**< Row 2 Pin Number.*/
#define ROW_2_DIR       TRISBbits.TRISB1  /**< Row 2 Direction.*/
#define ROW_3_PIN       PORTBbits.RB2     /**< Row 3 Pin Number.*/
#define ROW_3_DIR       TRISBbits.TRISB2  /**< Row 3 Direction.*/
#define ROW_4_PIN       PORTBbits.RB3     /**< Row 4 Pin Number.*/
#define ROW_4_DIR       TRISBbits.TRISB3  /**< Row 5 Direction.*/

#define COL_1_PIN       PORTBbits.RB4     /**< Col 1 Pin Number.*/
#define COL_1_DIR       TRISBbits.TRISB4  /**< Col 1 Direction.*/
#define COL_2_PIN       PORTBbits.RB5     /**< Col 2 Pin Number.*/
#define COL_2_DIR       TRISBbits.TRISB5  /**< Col 2 Direction.*/
#define COL_3_PIN       PORTBbits.RB6     /**< Col 3 Pin Number.*/
#define COL_3_DIR       TRISBbits.TRISB6  /**< Col 3 Direction.*/
#define COL_4_PIN       PORTBbits.RB7     /**< Col 4 Pin Number.*/
#define COL_4_DIR       TRISBbits.TRISB7  /**< Col 4 Direction.*/

#define KEYPAD_DEBOUNCE_TIME    20u       /**< Debounce Time in msec.*/
#define KEYPAD_HOLD_TIME        2000u     /**< Keypad Hold Time before Repeat.*/
#define KEYPAD_REPEAT_TIME      100u      /**< Keypad Repeat Time.*/

#define NO_KEYs                 255u      /**< No Key Pressed.*/
#define NO_KEY                  0u        /**< No Key Pressed.*/

/**
 * @brief Keypad States
 *
 * Different States of Matrix Keypad used by State Machine.
 */
typedef enum _Keypad_State_e
{
  KEYPAD_UP = 0,      /**< Key Up State, Initial State.*/
  KEYPAD_PRESSED,     /**< Key Pressed Detected State.*/
  KEYPAD_DOWN,        /**< Key Still Pressed State.*/
  KEYPAD_HELD,        /**< Key Hold Down for Repetetion State.*/
  KEYPAD_RELEASED,    /**< Key Release State.*/
  KEYPAD_DEBOUNCE     /**< Key Debouncing State.*/
} Keypad_State_e;

/**
 * @brief Keypad Structure
 *
 * Handle Matrix Keypad Data.
 */
typedef struct _Keypad_s
{
  u8_t keyPressed;             /**< Key Pressed Detected.*/
  u8_t keySensed;              /**< Key Sensed based on algorithm.*/
  u32_t keyStatus_timeStamp;    /**< Key State Change Timestamp.*/
  Keypad_State_e keypad_state;  /**< Keypad Current State.*/
} Keypad_s;

/* Public Function Prototypes*/
void Initialize_Keypad( void );
u8_t getKey( void );

#endif /* KEYPAD_H_ */
