/**
 * @file lcd.c
 * @author Embedded Laboratory
 * @date May 12, 2016
 * @brief LCD Initialization and Functions Definitions.
 * 23rd June 2016: Busy Check functions and other functions added.
 */

#include "lcd.h"

static boolean lcd_initialized = FALSE;   /**< LCD Initializatin Status.*/

/* Private Function Prototype*/
#ifdef USE_LCD_BUSY_FLAG
static void lcd_busy( void );
#else
static void lcd_delay_ms( u32_t ms );
#endif


/**
 * @brief Initialize 16x2 LCD Module.
 *
 * Initialize 16x2 LCD Module in 8-bit mode.
 * 
 */
void LCD_Init(void)
{
#ifndef USE_LCD_BUSY_FLAG
  lcd_initialized = TRUE;     // Set to True if using delay mode
#endif
  LCD_DATA_DIR = 0x00;
  LCD_RS_DIR = 0;
  LCD_RW_DIR = 0;
  LCD_EN_DIR = 0;
  #ifdef USE_LCD_BUSY_FLAG
  lcd_busy();
#else
  lcd_delay_ms(2);
#endif
  LCD_Cmd(LCD_16x2_INIT);
  LCD_Cmd(LCD_DISP_ON_CUR_ON);
  LCD_Cmd(LCD_DISP_ON_CUR_OFF);
  LCD_Cmd(LCD_CLEAR);
  LCD_Cmd(LCD_ENTRY_MODE);
  LCD_Cmd(LCD_FIRST_ROW);
}

/**
 * @brief Send Command to LCD.
 *
 * Send Command to LCD, use the following commands.
 * <b>LCD_16x2_INIT,LCD_DISP_ON_CUR_ON,LCD_DISP_ON_CUR_OFF,LCD_DISP_ON_CUR_BLNK,
 * LCD_FIRST_ROW,LCD_SECOND_ROW,LCD_CLEAR</b>.
 * @param command Command to Send to the LCD.
 */
void LCD_Cmd(u8_t command)
{
  LCD_DATA = command;
  LCD_RS = 0;
  LCD_RW = 0;
  LCD_EN = 1;
  Nop();
  Nop();
  Nop();
  LCD_EN = 0;
#ifdef USE_LCD_BUSY_FLAG
  lcd_busy();
#else
  lcd_delay_ms(2);
#endif
}

/**
 * @brief Write Data on LCD.
 *
 * Write Data on LCD, specified as arguments.
 * @param Data Data to Write on LCD.
 */
void LCD_Write(u8_t Data)
{
  if( !lcd_initialized )
  {
    LCD_Init();   // Re-Initialize LCD
  }
  
  if( lcd_initialized )
  {
    LCD_DATA = Data;
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 1;
    Nop();
    Nop();
    Nop();
    LCD_EN = 0;
#ifdef USE_LCD_BUSY_FLAG
    lcd_busy();
#else
    lcd_delay_ms(2);
#endif
  }
}

/**
 * @brief Write String on LCD.
 *
 * Write String on LCD, specified as arguments.
 * @param *msg First Character Address of the String.
 * @note String Must be terminated by NULL Character.
 */
void LCD_Write_Text(u8_t *msg)
{
  while(*msg)
  {
    LCD_Write(*msg);
    msg++;
  }
}

#ifdef USE_LCD_BUSY_FLAG
/**
 * @brief Lcd Busy.
 *
 * Wait for LCD Controller to get Ready.
 * @note Timeout Counter is added to protect the system from hanging state. In 
 * most of the cases timeout counter value is < 50 but as clear command takes 
 * time, and at that point counter value is around < 1800. Things will change 
 * with operating frequency of micro. Please re-evaluate the optimum value for
 * your setup.
 */
static void lcd_busy( void )
{
  u32_t timeout = 0u;
  lcd_initialized = TRUE;               // Become False if, initialization fails
  TRISDbits.TRISD7 = 1;    // Input Pin  
  LCD_EN = 1;
  LCD_RS = 0;
  LCD_RW = 1;
  while( PORTDbits.RD7 )
  {
    LCD_EN = 0;
    Nop();
    Nop();
    Nop();
    LCD_EN = 1;
    timeout++;
    if( timeout > 2500u)
    {
      lcd_initialized = FALSE;
      break;
    }
  }
  TRISDbits.TRISD7 = 0;    // Output Pin
  LCD_RW = 0;
}
#else
/**
 * @brief Delay For LCD.
 *
 * @warning Note Accurate, approximate.
 */
static void lcd_delay_ms( u32_t ms )
{
  u32_t i,j = 0;
  for(i=0;i<=ms;i++)
  {
    for(j=0;j<150u;j++)
      ;
  }
}
#endif