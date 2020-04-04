/*
 * lcd.h - LCD 20x4 library header file
 *
 * Device: LCD 20x4
 * Driver: 44780
 * Date: Dec 31, 2016
 * Rev: 0.1
 * Author: Ly Hon Quang
 */

#ifndef LCD2004_H_
#define LCD2004_H_

/*******************************Definitions***********************************/
// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

/****************************Function prototypes******************************/
void LCD_init(uint8_t Mode, uint8_t nLine, uint8_t Dotsize);

void LCD_clear(void);
void LCD_home(void);

void LCD_noDisplay(void);
void LCD_display(void);
void LCD_noBlink(void);
void LCD_blink(void);
void LCD_noCursor(void);
void LCD_cursor(void);
void LCD_scrollDisplayLeft(void);
void LCD_scrollDisplayRight(void);
void LCD_leftToRight(void);
void LCD_rightToLeft(void);
void LCD_autoscroll(void);
void LCD_noAutoscroll(void);

void LCD_createChar(uint8_t location, uint8_t charmap[]);
void LCD_setCursor(uint8_t ui8Col, uint8_t ui8Row);
void LCD_write(uint8_t value);
void LCD_command(uint8_t value);

void LCD_printf(uint8_t x, uint8_t y, const char *pcString, ...);

/*****************************************************************************/
#endif

/* End of LCD1602.h */
