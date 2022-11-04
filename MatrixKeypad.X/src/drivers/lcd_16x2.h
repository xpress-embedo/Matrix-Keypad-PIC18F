/**
 * @file lcd_16x2.h
 * @author Embedded Laboratory
 * @date May 12, 2016
 * @brief LCD Functions.
 */

#ifndef LCD_16x2_H
#define	LCD_16x2_H

#include "config.h"

#define USE_LCD_BUSY_FLAG             /**< Use Busy Bit instead of Delay.*/
#define LCD_ROWS              2u      /**< Total Number of Row in LCD.*/
#define LCD_COLS              16u     /**< Total Number of Column in LCD.*/
#define LCD_BUFFER_LEN (LCD_COLS + 1) /**< No of characters in a row buffer.*/

#ifdef	__cplusplus
extern "C"
{
#endif

#define LCD_D0                0       /**< LCD Data Line0.*/
#define LCD_D1                1       /**< LCD Data Line1.*/
#define LCD_D2                2       /**< LCD Data Line2.*/
#define LCD_D3                3       /**< LCD Data Line3.*/
#define LCD_D4                7       /**< LCD Data Line4.*/
#define LCD_D5                8       /**< LCD Data Line5.*/
#define LCD_D6                9       /**< LCD Data Line6.*/
#define LCD_D7                10      /**< LCD Data Line7.*/
#define LCD_DATA_PORT         PORT2   /**< LCD Data Lines Direction.*/
  
#define LCD_RS                2       /**< LCD RS Pin.*/
#define LCD_RW                1       /**< LCD RW Pin.*/
#define LCD_EN                0       /**< LCD EN Pin.*/
#define LCD_RS_PORT           PORT1   /**< LCD RS Pin Direction.*/
#define LCD_RW_PORT           PORT1   /**< LCD RW Pin Direction.*/
#define LCD_EN_PORT           PORT1   /**< LCD EN Pin Direction.*/
  
#define LCD_BACKLIT_PIN       1       /**< LCD Back Light Pin.*/
#define LCD_BACKLIT_PORT      PORT3   /**< LCD Back Light Pin Direction.*/

/* LCD Commands */
#define LCD_16x2_INIT         0x38    /**< Initialize 16x2 Lcd in 8-bit Mode.*/
#define LCD_DISP_ON_CUR_ON    0x0E    /**< LCD Display On Cursor On.*/
#define LCD_DISP_ON_CUR_OFF   0x0C    /**< LCD Display On Cursor Off.*/
#define LCD_DISP_ON_CUR_BLNK  0x0F    /**< LCD Display On Cursor Blink.*/
#define LCD_ENTRY_MODE        0x06    /**< LCD Entry Mode. */
#define LCD_FIRST_ROW         0x80    /**< Move Pointer to First Row.*/
#define LCD_SECOND_ROW        0xC0    /**< Move Pointer to Second Row.*/
#define LCD_CLEAR             0x01    /**< Clear LCD Display.*/

/* LCD Function Prototypes */
void LCD_Init(void);
void LCD_Cmd(u8_t command);
void LCD_Write(u8_t Data);
void LCD_Write_Text(char *msg);
void LCD_BackLight_On( void );
void LCD_BackLight_Off( void );
void LCD_Update( void );
boolean LCD_Print_Line(u8_t lcd_line, char *p_lcd_msg);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_16x2_H */

