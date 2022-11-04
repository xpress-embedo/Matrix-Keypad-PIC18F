/**
 * @file main.c
 * @author Embedded Laboratory
 * @date July 23, 2016
 * @brief This is the main file of the project.
 * 
 * \mainpage Description
 * This project demonstrates how to use Matrix 4x4 Keypad input from user.
 * Keypad has 16 keys, whenever a key is pressed its counter increment by 1 and
 * keypress with counter value is then displayed on 16x2 LCD.
 * Keypad Hold feature is added in algorithm, which will increment counter 
 * speedily, when pressing a key for more than 2 seconds.
 * 
 * \section library_sec Libraries Used in this Project
 * \subsection library1 Matrix Keypad Library
 * This library handles decoding keypress on Matrix Keypad.
 * \subsection library2 16x2 LCD Library
 * This library controls and display characters on LCD. Function updated to 
 * reduce the update time, by using Busy Flag of 16x2 LCD. New functions are 
 * added.
*/
#include "config.h"
#include "lcd.h"
#include "keypad.h"

u8_t lcd_line[16] = {0};  /**< LCD Display Buffer.*/
u32_t count[16] = {0};
/**
 * Main Program.
 */
void main(void)
{
  enable_global_int();
  Timer0_Init();
  LCD_Init ();
  Initialize_Keypad();
  sprintf(lcd_line,"  Embedded Lab");
  LCD_Cmd (LCD_CLEAR);
  LCD_Write_Text(lcd_line);
  while(1)
  {
    u8_t keypress = 0;
    u32_t temp = 0;
    if( keypress = getKey() )
    {
      switch( keypress )
      {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '0':
        temp = ++(count[keypress - 48]);
        break;
      case 'A':
      case 'B':
      case 'C':
      case 'D':
        temp = ++(count[keypress - 55]);
        break;
      case '*':
        temp = ++(count[14]);
        break;
      case '#':
        temp = ++(count[15]);
        break;
      };
      sprintf(lcd_line,"%c -> %d",keypress, temp);
      LCD_Cmd(LCD_SECOND_ROW);
      LCD_Write_Text(lcd_line);
    }
  }
  return;
}
