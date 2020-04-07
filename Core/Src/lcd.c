/*
 * lcd.c - LCD 20x4 source code
 * This library is based on the Arduino's Liquid Crystal library
 *
 * Device: LCD 20x4
 * Driver: HD44780
 * Date: Dec 31, 2016
 * Rev: 0.1
 * Author: Ly Hon Quang
 */

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "lcd.h"
//include your GPIO library here
#include "main.h"
#include "support.h"

/****************************Private Definitions******************************/
//Define your pin controlling functions and delay function here to compatible
//with your hardware
#define LCD_RS_HIGH			HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)
#define LCD_RS_LOW			HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)
// #define LCD_RW_HIGH		NULL		//comment RW definitions if no use
// #define LCD_RW_LOW		NULL
#define LCD_E_HIGH			HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_SET)
#define LCD_E_LOW			HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET)
/* If hardware use shift register, function that need to rewrite are:
 - LCD_DataPinWrite
 - LCD_write4bits
 - LCD_write8bits
*/
#define LCD_D0_HIGH			NULL
#define LCD_D0_LOW			NULL
#define LCD_D1_HIGH			NULL
#define LCD_D1_LOW			NULL
#define LCD_D2_HIGH			NULL
#define LCD_D2_LOW			NULL
#define LCD_D3_HIGH			NULL
#define LCD_D3_LOW			NULL
#define LCD_D4_HIGH			HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_SET)
#define LCD_D4_LOW			HAL_GPIO_WritePin(LCD_DB4_GPIO_Port, LCD_DB4_Pin, GPIO_PIN_RESET)
#define LCD_D5_HIGH			HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_SET)
#define LCD_D5_LOW			HAL_GPIO_WritePin(LCD_DB5_GPIO_Port, LCD_DB5_Pin, GPIO_PIN_RESET)
#define LCD_D6_HIGH			HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_SET)
#define LCD_D6_LOW			HAL_GPIO_WritePin(LCD_DB6_GPIO_Port, LCD_DB6_Pin, GPIO_PIN_RESET)
#define LCD_D7_HIGH			HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_SET)
#define LCD_D7_LOW			HAL_GPIO_WritePin(LCD_DB7_GPIO_Port, LCD_DB7_Pin, GPIO_PIN_RESET)

#define delayus(t)			TIM4_delayus(t)	//unit: micro second

/*********************************Variables***********************************/
static uint8_t _displayfunction;
static uint8_t _displaycontrol;
static uint8_t _displaymode;
// static const uint8_t const _Row_Offsets[2] = {0x00, 0x40};	//LCD 16x2 or LCD 20x2
static const uint8_t _Row_Offsets[4] = {0x00, 0x40, 0x10, 0x50}; //LCD 16x4 or LCD 20x4

static const char * const g_pcHex = "0123456789abcdef";

/*************************Private function prototypes*************************/
static void LCD_DataPinWrite(uint8_t DataPin, uint8_t Value);
static void LCD_send(uint8_t value, uint8_t mode);
static void LCD_write4bits(uint8_t value);
static void LCD_write8bits(uint8_t value);
static void LCD_pulseEnable(void);
static void LCD_Putchar(char c);
static void LCD_Putstring(const char *pcBuf, uint8_t ui8Len);

/****************************Function definitions*****************************/

/******************************************************************************
 * LCD_init - Initialize the LCD before using
 *
 * Parameter:
 * 	- Mode: LCD data mode
 * 		+ LCD_8BITMODE: 8-bit data pin (D7..D0)
 * 		+ LCD_4BITMODE: 4-bit data pin (D7..D4)
 * 	- nLine: Number of line
 * 		+ LCD_2LINE: 2 lines
 * 		+ LCD_1LINE: 1 line
 * 	- Dotsize: character dot size
 * 		+ LCD_5x10DOTS: Dot size 5x10
 * 		+ LCD_5x8DOTS: Dot size 5x8
 *
 * Return: none
 *****************************************************************************/
void LCD_init(uint8_t Mode, uint8_t nLine, uint8_t Dotsize)
{
	_displayfunction = Mode | nLine | Dotsize;

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands.
	delayus(40000);

	// Now we pull both RS and R/W low to begin commands
	LCD_RS_LOW;
	LCD_E_LOW;
	#if defined(LCD_RW_LOW)
	LCD_RW_LOW;
	#endif

	//put the LCD into 4 bit or 8 bit mode
	if (!(_displayfunction & LCD_8BITMODE)) {
	  // this is according to the hitachi HD44780 datasheet
	  // figure 24, pg 46

	  // we start in 8bit mode, try to set 4 bit mode
	  LCD_write4bits(0x03);
	  delayus(4500); // wait min 4.1ms

	  // second try
	  LCD_write4bits(0x03);
	  delayus(4500); // wait min 4.1ms

	  // third go!
	  LCD_write4bits(0x03);
	  delayus(150);

	  // finally, set to 4-bit interface
	  LCD_write4bits(0x02);
	} else {
	  // this is according to the hitachi HD44780 datasheet
	  // page 45 figure 23

	  // Send function set command sequence
	  LCD_command(LCD_FUNCTIONSET | _displayfunction);
	  delayus(4500);  // wait more than 4.1ms

	  // second try
	  LCD_command(LCD_FUNCTIONSET | _displayfunction);
	  delayus(150);

	  // third go
	  LCD_command(LCD_FUNCTIONSET | _displayfunction);
	}

	// finally, set # lines, font size, etc.
	LCD_command(LCD_FUNCTIONSET | _displayfunction);

	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	LCD_display();

	// clear it off
	LCD_clear();

	// Initialize to default text direction (for romance languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	// set the entry mode
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

/******************************************************************************
 * LCD_clear - Clear the display and set cursor to home position
 * Note: this command takes long time to execute
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_clear(void)
{
	LCD_command(LCD_CLEARDISPLAY);	// clear display, set cursor position to zero
	delayus(2000);  // this command takes a long time!
}

/******************************************************************************
 * LCD_home - Set the LCD's cursor to home position
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_home(void)
{
	LCD_command(LCD_RETURNHOME);  // set cursor position to zero
	delayus(2000);  // this command takes a long time!
}

/******************************************************************************
 * LCD_noDisplay - Turn the LCD display off (quickly)
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_noDisplay(void)
{
	_displaycontrol &= ~LCD_DISPLAYON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/******************************************************************************
 * LCD_display - Turn the LCD display on (quickly)
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_display(void)
{
	_displaycontrol |= LCD_DISPLAYON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/******************************************************************************
 * LCD_noBlink - Turn off the blinking cursor
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_noBlink(void)
{
	_displaycontrol &= ~LCD_BLINKON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/******************************************************************************
 * LCD_blink - Turn on the blinking cursor
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_blink(void)
{
	_displaycontrol |= LCD_BLINKON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/******************************************************************************
 * LCD_noCursor - Turns the underline cursor off
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_noCursor(void)
{
	_displaycontrol &= ~LCD_CURSORON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/******************************************************************************
 * LCD_noCursor - Turns the underline cursor on
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_cursor(void)
{
	_displaycontrol |= LCD_CURSORON;
	LCD_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/******************************************************************************
 * LCD_scrollDisplayLeft - Scroll the display to the left without changing
 * the RAM.
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_scrollDisplayLeft(void)
{
	LCD_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

/******************************************************************************
 * LCD_scrollDisplayRight - Scroll the display to the right without changing
 * the RAM.
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_scrollDisplayRight(void)
{
	LCD_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/******************************************************************************
 * LCD_leftToRight - This function is for text that flows Left to Right
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_leftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

/******************************************************************************
 * LCD_rightToLeft - This function is for text that flows Right to Left
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_rightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

/******************************************************************************
 * LCD_autoscroll - This will 'right justify' text from the cursor
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_autoscroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

/******************************************************************************
 * LCD_noAutoscroll - This will 'left justify' text from the cursor
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
void LCD_noAutoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	LCD_command(LCD_ENTRYMODESET | _displaymode);
}

/******************************************************************************
 * LCD_createChar - Create your owm character into the CGRAM
 * Allows us to fill the first 8 CGRAM locations with custom characters
 *
 * Parameter:
 * 	- location: the location in CGRAM for new character to be put into (0 to 7)
 * 	- charmap : new character data (see datasheet for specification)
 *
 * Return: none
 *****************************************************************************/
void LCD_createChar(uint8_t location, uint8_t charmap[])
{
	uint8_t i;

	location &= 0x7; // we only have 8 locations 0-7
	LCD_command(LCD_SETCGRAMADDR | (location << 3));
	for (i=0; i<8; i++) {
	  LCD_write(charmap[i]);
	}
}

/******************************************************************************
 * LCD_setCursor - Move the cusor to the new location
 *
 * Parameter:
 * 	- ui8Col: new column location (0 to (number of column - 1))
 * 	- ui8Row: new row location (0 to (number of row - 1))
 *
 * Return: none
 *****************************************************************************/
void LCD_setCursor(uint8_t ui8Col, uint8_t ui8Row)
{
	const uint8_t max_lines = sizeof(_Row_Offsets) / sizeof(*_Row_Offsets);

	if ( ui8Row >= max_lines ) {
	  ui8Row = max_lines - 1;    // we count rows starting w/0
	}

	LCD_command(LCD_SETDDRAMADDR | (ui8Col + _Row_Offsets[ui8Row]));
}

/******************************************************************************
 * A simple LCD based printf function supporting \%c, \%d, \%p, \%s, \%u,
 * \%x, and \%X.
 *
 * Parameter:
 * 	(x,y): start coodinate for printing
 * 	pcString is the format string.
 * 	param ... are the optional arguments, which depend on the contents of the
 * format string.
 *
 * This function is very similar to the C library <tt>fprintf()</tt> function.
 * All of its output will be sent to the LCD.  Only the following formatting
 * characters are supported:
 *
 * - \%c to print a character
 * - \%d or \%i to print a decimal value
 * - \%s to print a string
 * - \%u to print an unsigned decimal value
 * - \%x to print a hexadecimal value using lower case letters
 * - \%X to print a hexadecimal value using lower case letters (not upper case
 * letters as would typically be used)
 * - \%p to print a pointer as a hexadecimal value
 * - \%\% to print out a \% character
 *
 * For \%s, \%d, \%i, \%u, \%p, \%x, and \%X, an optional number may reside
 * between the \% and the format character, which specifies the minimum number
 * of characters to use for that value; if preceded by a 0 then the extra
 * characters will be filled with zeros instead of spaces.  For example,
 * ``\%8d'' will use eight characters to print the decimal value with spaces
 * added to reach eight; ``\%08d'' will use eight characters as well but will
 * add zeroes instead of spaces.
 *
 * The type of the arguments after \e pcString must match the requirements of
 * the format string.  For example, if an integer was passed where a string
 * was expected, an error of some kind will most likely occur.
 *
 * Return: none.
 *****************************************************************************/
void LCD_printf(uint8_t x, uint8_t y, const char *pcString, ...)
{
	static unsigned int ulPos, ulCount, ulNeg;
	unsigned long ulValue, ulBase, ulIdx;
	char *pcStr, pcBuf[16], cFill;
	va_list vaArgP;

	LCD_setCursor(x,y);
	va_start(vaArgP, pcString);		// Start the varargs processing.

	// Loop while there are more characters in the string.
	while(*pcString)
	{
		for(ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0');
				ulIdx++);		// Find the first non-% character, or the end of the string.

		LCD_Putstring(pcString, ulIdx);		// Write this portion of the string.

		pcString += ulIdx;		// Skip the portion of the string that was written.

		if(*pcString == '%')		// See if the next character is a %.
		{
			pcString++;			// Skip the %.

			// Set the digit count to zero, and the fill character to space
			// (i.e. to the defaults).
			ulCount = 0;
			cFill = ' ';

			// It may be necessary to get back here to process more characters.
			// Goto's aren't pretty, but effective.  I feel extremely dirty for
			// using not one but two of the beasts.
			again:

			// Determine how to handle the next character.
			switch(*pcString++)
			{
			// Handle the digit characters.
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				// If this is a zero, and it is the first digit, then the
				// fill character is a zero instead of a space.
				if((pcString[-1] == '0') && (ulCount == 0))
					cFill = '0';

				// Update the digit count.
				ulCount *= 10;
				ulCount += pcString[-1] - '0';

				// Get the next character.
				goto again;
			}

			// Handle the %c command.
			case 'c':
			{
				ulValue = va_arg(vaArgP, unsigned long);	// Get the value from the varargs.
				LCD_Putstring((char *)&ulValue, 1);		// Print out the character.

				// This command has been handled.
				break;
			}

			// Handle the %d and %i commands.
			case 'd':
			case 'i':
			{
				ulValue = va_arg(vaArgP, unsigned long);		// Get the value from the varargs.
				ulPos = 0;		// Reset the buffer position.

				// If the value is negative, make it positive and indicate
				// that a minus sign is needed.
				if((long)ulValue < 0)
				{
					ulValue = -(long)ulValue;			// Make the value positive.
					ulNeg = 1;	// Indicate that the value is negative.
				}
				else
				{
					// Indicate that the value is positive so that a minus
					// sign isn't inserted.
					ulNeg = 0;
				}

				ulBase = 10;		// Set the base to 10.

				// Convert the value to ASCII.
				goto convert;
			}

			// Handle the %s command.
			case 's':
			{
				pcStr = va_arg(vaArgP, char *);		// Get the string pointer from the varargs.

				for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++);		// Determine the length of the string.
				LCD_Putstring(pcStr, ulIdx);		// Write the string.

				// Write any required padding spaces
				if(ulCount > ulIdx)
				{
					ulCount -= ulIdx;
					while(ulCount--)
						LCD_Putstring(" ", 1);
				}
				// This command has been handled.
				break;
			}

			// Handle the %u command.
			case 'u':
			{
				ulValue = va_arg(vaArgP, unsigned long);	// Get the value from the varargs.
				ulPos = 0;			// Reset the buffer position.
				ulBase = 10;		// Set the base to 10.

				// Indicate that the value is positive so that a minus sign
				// isn't inserted.
				ulNeg = 0;

				goto convert;		// Convert the value to ASCII.
			}

			// Handle the %x and %X commands.  Note that they are treated
			// identically; i.e. %X will use lower case letters for a-f
			// instead of the upper case letters is should use.  We also
			// alias %p to %x.
			case 'x':
			case 'X':
			case 'p':
			{
				ulValue = va_arg(vaArgP, unsigned long);		// Get the value from the varargs.
				ulPos = 0;		// Reset the buffer position.
				ulBase = 16;			// Set the base to 16.

				// Indicate that the value is positive so that a minus sign
				// isn't inserted.
				ulNeg = 0;

				// Determine the number of digits in the string version of
				// the value.
				convert:
				for(ulIdx = 1;
						(((ulIdx * ulBase) <= ulValue) &&
								(((ulIdx * ulBase) / ulBase) == ulIdx));
						ulIdx *= ulBase, ulCount--);

				// If the value is negative, reduce the count of padding
				// characters needed.
				if(ulNeg)
					ulCount--;

				// If the value is negative and the value is padded with
				// zeros, then place the minus sign before the padding.
				if(ulNeg && (cFill == '0'))
				{
					pcBuf[ulPos++] = '-';			// Place the minus sign in the output buffer.

					// The minus sign has been placed, so turn off the
					// negative flag.
					ulNeg = 0;
				}

				// Provide additional padding at the beginning of the
				// string conversion if needed.
				if((ulCount > 1) && (ulCount < 16))
				{
					for(ulCount--; ulCount; ulCount--)
						pcBuf[ulPos++] = cFill;
				}

				// If the value is negative, then place the minus sign
				// before the number.
				if(ulNeg)
					pcBuf[ulPos++] = '-';			// Place the minus sign in the output buffer.

				// Convert the value into a string.
				for(; ulIdx; ulIdx /= ulBase)
					pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase];

				LCD_Putstring(pcBuf, ulPos);		// Write the string.

				// This command has been handled.
				break;
			}

			// Handle the %% command.
			case '%':
			{
				LCD_Putstring(pcString - 1, 1);		// Simply write a single %.

				// This command has been handled.
				break;
			}

			// Handle all other commands.
			default:
			{
				LCD_Putstring("ERROR", 5);		// Indicate an error.

				// This command has been handled.
				break;
			}
			}
		}
	}
	va_end(vaArgP);	// End the varargs processing.
}

/******************************************************************************
 * LCD_write - Write display data to DDRAM
 *
 * Parameter:
 *  - value: data value
 *
 * Return: none
 *****************************************************************************/
void LCD_write(uint8_t value)
{
	LCD_send(value, 1);
}

/******************************************************************************
 * LCD_command - Send command/instruction to LCD
 *
 * Parameter:
 * 	- value: command/instruction data (instruction table can be found in datasheet)
 *
 * Return: none
 *****************************************************************************/
void LCD_command(uint8_t value)
{
	LCD_send(value, 0);
}

/*************************Private function definition*************************/

/******************************************************************************
 * LCD_DataPinWrite - Create a select case to write data pin depend on
 * input data.
 *
 * Parameter:
 * 	- DataPin: data pin (0 to 7)
 * 	- Value:
 * 		+ 0: reset the pin
 * 		+ 1: set the pin
 *
 * Return: none
 *****************************************************************************/
static void LCD_DataPinWrite(uint8_t DataPin, uint8_t Value)
{
	switch (DataPin)
	{
		case 0:
			Value ? LCD_D0_HIGH : LCD_D0_LOW;
			break;

		case 1:
			Value ? LCD_D1_HIGH : LCD_D1_LOW;
			break;

		case 2:
			Value ? LCD_D2_HIGH : LCD_D2_LOW;
			break;

		case 3:
			Value ? LCD_D3_HIGH : LCD_D3_LOW;
			break;

		case 4:
			Value ? LCD_D4_HIGH : LCD_D4_LOW;
			break;

		case 5:
			Value ? LCD_D5_HIGH : LCD_D5_LOW;
			break;

		case 6:
			Value ? LCD_D6_HIGH : LCD_D6_LOW;
			break;

		case 7:
			Value ? LCD_D7_HIGH : LCD_D7_LOW;
			break;

		default:
			return;
	}
}

/******************************************************************************
 * LCD_send - Write either command or data, with automatic 4/8-bit selection
 *
 * Parameter:
 * 	- value: instruction data in command mode or display data in display mode
 * 	- mode: define mode to put data to LCD
 * 		+ 1: Data mode
 * 		+ 0: Command mode
 *
 * Return: none
 *****************************************************************************/
static void LCD_send(uint8_t value, uint8_t mode)
{
	// write either command or data, with automatic 4/8-bit selection
	mode ? LCD_RS_HIGH : LCD_RS_LOW;

	// if there is a RW pin indicated, set it low to Write
	#if defined(LCD_RW_LOW)
	LCD_RW_LOW;
	#endif

	if (_displayfunction & LCD_8BITMODE)
		LCD_write8bits(value);
	else
	{
		LCD_write4bits(value >> 4);
		LCD_write4bits(value);
	}
}

/******************************************************************************
 * LCD_pulseEnable - put a pulse to E pin
 *
 * Parameter: none
 *
 * Return: none
 *****************************************************************************/
static void LCD_pulseEnable(void)
{
	LCD_E_LOW;
	delayus(1);
	LCD_E_HIGH;
	delayus(1);    // enable pulse must be >450ns
	LCD_E_LOW;
	delayus(100);   // commands need > 37us to settle
}

/******************************************************************************
 * LCD_write4bits - Write data to LCD in 4-bit mode
 *
 * Parameter:
 * 	- value: data code
 *
 * Return: none
 *****************************************************************************/
static void LCD_write4bits(uint8_t value)
{
	uint8_t i;
	for (i = 4; i < 8; i++)
		LCD_DataPinWrite(i, (value >> (i - 4)) & 0x01);

	LCD_pulseEnable();
}

/******************************************************************************
 * LCD_write8bits - Write data to LCD in 8-bit mode
 *
 * Parameter:
 * 	- value: data code
 *
 * Return: none
 *****************************************************************************/
static void LCD_write8bits(uint8_t value)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
		LCD_DataPinWrite(i, (value >> i) & 0x01);

	LCD_pulseEnable();
}

/******************************************************************************
 * LCD_Putchar - Put a single character to LCD
 * This function serves for LCD_Putstring function
 *
 * Parameter:
 * 	- c: character code in CGROM
 *
 * Return: none
 *****************************************************************************/
static void LCD_Putchar(char c)
{
	LCD_write(c);
}

/******************************************************************************
 * LCD_Putstring - Put a string to LCD
 * This function serves for LCD_printf function
 *
 * Parameter:
 * 	- *pcBuf: pointer to string. Note that this is a constant string.
 * 	- ui8Len: length of the string
 *
 * Return: none
 *****************************************************************************/
static void LCD_Putstring(const char *pcBuf, uint8_t ui8Len)
{
	uint8_t i;
	for(i = 0; i < ui8Len; i++)
	{
		LCD_Putchar(*pcBuf);
		pcBuf++;
	}
}

/* End of lcd.c */
