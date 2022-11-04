/**
 * @file lcd_16x2.c
 * @author Embedded Laboratory
 * @date May 12, 2016
 * @brief LCD Initialization and Functions Definitions.
 */

#include "lcd_16x2.h"

static boolean lcd_initialized = FALSE;   /**< LCD Initializatin Status.*/
static 
char lcdCharLines[LCD_ROWS][LCD_BUFFER_LEN];   /**< LCD display message.*/

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
  // Set LCD Pins as Output Pins
  LPC_GPIO2->DIR |= ( (0xF0<<3)|0x0F );
  
  LPC_IOCON->R_PIO1_0 |= 0x1;
  LPC_IOCON->R_PIO1_1 |= 0x1;
  LPC_IOCON->R_PIO1_2 |= 0x1;
  
  LPC_GPIO1->DIR |= (1<<LCD_RS);
  LPC_GPIO1->DIR |= (1<<LCD_RW);
  LPC_GPIO1->DIR |= (1<<LCD_EN);
  
  LPC_GPIO3->DIR |= (1<<LCD_BACKLIT_PIN);
  
  // Clear Values
  LPC_GPIO2->DATA &= ~((0xF0<<3)|0x0F );
  LPC_GPIO1->DATA &= ~(0x01<<LCD_RS);
  LPC_GPIO1->DATA &= ~(0x01<<LCD_RW);
  LPC_GPIO1->DATA &= ~(0x01<<LCD_EN);
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
  LPC_GPIO2->DATA &= ~((0xF0<<3)|0x0F );  // Clear
  u32_t temp1 = (command & 0xF0);
  u32_t temp2 = (command & 0x0F);
  temp1 = temp1 << 3;
  LPC_GPIO2->DATA |= (temp1|temp2);
  LPC_GPIO1->DATA &= ~(0x01<<LCD_RS);
  LPC_GPIO1->DATA &= ~(0x01<<LCD_RW);
  LPC_GPIO1->DATA |= (0x01<<LCD_EN);
  __no_operation();
  __no_operation();
  LPC_GPIO1->DATA &= ~(0x01<<LCD_EN);
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
    LPC_GPIO2->DATA &= ~((0xF0<<3)|0x0F );  // Clear
    u32_t temp1 = (Data & 0xF0);
    u32_t temp2 = (Data & 0x0F);
    temp1 = temp1 << 3;
    LPC_GPIO2->DATA |= (temp1|temp2);
    LPC_GPIO1->DATA |= (0x01<<LCD_RS);
    LPC_GPIO1->DATA &= ~(0x01<<LCD_RW);
    LPC_GPIO1->DATA |= (0x01<<LCD_EN);
    __no_operation();
    __no_operation();
    LPC_GPIO1->DATA &= ~(0x01<<LCD_EN);
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
void LCD_Write_Text(char *msg)
{
  while(*msg)
  {
    LCD_Write(*msg);
    msg++;
  }
}

/**
 * @brief Turn On Back Light.
 *
 * Turn's On the Back Light of LCD.
 * @note Power Consumption will be increased.
 */
void LCD_BackLight_On( void )
{
  LPC_GPIO3->DATA |= (0x01<<LCD_BACKLIT_PIN);
}

/**
 * @brief Turn Off Back Light.
 *
 * Turn's Off the Back Light of LCD.
 */
void LCD_BackLight_Off( void )
{
  LPC_GPIO3->DATA &= ~(0x01<<LCD_BACKLIT_PIN);
}

/**
 * @brief Update LCD Display Buffer.
 *
 * This function will update the LCD display buffer.
 * @param lcd_line Row Number, in which data has to be written.
 * @param *p_lcd_msg Message which needs to be updated in Lcd Buffer.
 * @return TRUE if successfull otherwise FALSE.
 * @note Power Consumption will be increased.
 */
boolean LCD_Print_Line(u8_t lcd_line, char *p_lcd_msg)
{
  boolean updated = TRUE;
  /* 
  right-pad the message with spaces to erase any unwritten characters on the 
  display
  */
  // Padding Width must be changed if LCD_COLs
  s8_t n = snprintf(lcdCharLines[lcd_line], LCD_BUFFER_LEN, "%-16s", p_lcd_msg);
  if( n < 0 )
    updated = FALSE;
  else
  {
    if( n >= LCD_BUFFER_LEN )
      updated = FALSE;
  }
  return updated;     
}

/**
 * @brief Update LCD.
 *
 * Update the LCD with the data from LCD Buffer.
 */
void LCD_Update( void )
{
  for (u8_t i = 0; i < LCD_ROWS; i++)
  {
    switch(i)
    {
    case 0:
      LCD_Cmd(LCD_FIRST_ROW);
      break;
    case 1:
      LCD_Cmd(LCD_SECOND_ROW);
      break;
    }
    LCD_Write_Text(lcdCharLines[i]);
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
  LPC_GPIO2->DATA |= (0x01<<LCD_D7);
  LPC_GPIO2->DIR &= ~(0x01<<LCD_D7);    // Make as Input Pin
  
  LPC_GPIO1->DATA |= (1<<LCD_EN);
  LPC_GPIO1->DATA &= ~(1<<LCD_RS);
  LPC_GPIO1->DATA |= (1<<LCD_RW);
  while( (LPC_GPIO2->DATA >> LCD_D7)&0x01 )
  {
    LPC_GPIO1->DATA &= ~(1<<LCD_EN);
    __no_operation();
    __no_operation();
    LPC_GPIO1->DATA |= (1<<LCD_EN);
    timeout++;
    if( timeout > 2500u)
    {
      lcd_initialized = FALSE;
      break;
    }
  }
  LPC_GPIO2->DIR |= (0x01<<LCD_D7);    // Make Pin as Output Pin
  LPC_GPIO1->DATA &= ~(1<<LCD_RW);
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
    for(j=0;j<8000u;j++)
      ;
  }
}
#endif
