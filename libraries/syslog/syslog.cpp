#include "Arduino.h"
#include <avr/pgmspace.h>
#include "syslog.h"

/**
*	Logger static instance
**/
logger syslog;

/*
************************************************************************
*	Helper functions
************************************************************************
*/

/**
*	Computes a power of 10
*	@param		pow	power
*	@returns	a value of 10^pow
**/
int powerOf10(int pow)
{
	int16_t x = 1;
	for (int i = 0; i < pow; i++)
	{
		x *= 10;
	}

	return x;
}

/**
*	Gets a decimal value from a number
*	@param		value	an input value
*	@param		index	decimal place index
*	@returns	a decimal value from a number's decimal place
**/
int get_number(int value, int index)
{
	int16_t d1 = powerOf10(index + 1);
	int16_t d2 = powerOf10(index);
	int16_t x = (value % d1 - value % d2) / d2;
	return x;
}

/**
*	Prints a formatted decimal number
*	@param	print		printer
*	@param	precision	decimal places count
*	@param	placeholder	decimal placeholder
**/
void print_number(Print& print, int value, int precision, char placeholder)
{
	for (int i = precision - 1; i >= 0; i--)
	{
		int16_t x = get_number(value, i);

		if(x == 0 && i != 0)
		{
			print.print(placeholder);
		}
		else
		{
			print.print(x);
		}
	}
}

/*
************************************************************************
*	systime
*	Local time type
************************************************************************
*/

/**
*	Local time type
**/
struct systime
{
	/**
	*	Hours
	**/
	uint16_t h;

	/**
	*	Minutes
	**/
	uint8_t  min;

	/**
	*	Seconds
	**/
	uint8_t  sec;

	/**
	*	Milliseconds
	**/
	uint16_t ms;

	/**
	* Gets current local time
	* @returns current local time
	**/
	static const systime now()
	{
		systime time;

		unsigned long ms = millis();
		unsigned long sec = ms / 1000;
		unsigned long min = sec / 60;
		unsigned long h = min / 60;
		sec = sec % 60;
		min = min % 60;
		ms = ms % 1000;

		time.h = h;
		time.min = min;
		time.sec = sec;
		time.ms = ms;

		return time;
	}
};

/*
************************************************************************
*	log_event
*	Log event writer
************************************************************************
*/

/**
*	Constructor
*	@param	enable	indicates whether current log writer instance is allowed to write any data into log
**/
log_event::log_event(bool enable)
	: _enable(enable)
{ }

/**
*	Destructor
**/
log_event::~log_event()
{
	if(_enable)
	{
		Serial.println();
		Serial.flush();
	}
}

/**
*	Appends a formatted text into log
*	@param	msg	format string
**/
void log_event::printf(const char* msg, ...)
{
	if(_enable)
	{
		va_list args;
		va_start(args, msg);
		syslog.print_message(msg, args);
	}
}

/**
*	Appends a formatted text into log
*	@param	msg	format string
**/
void log_event::printf(const __FlashStringHelper* msg, ...)
{
	if(_enable)
	{
		va_list args;
		va_start(args, msg);
		syslog.print_message(msg, args);
	}
}

/*
************************************************************************
*	log_t
*	Logger class
************************************************************************
*/

/** public members **/

/**
*	Initializes logging
*	@param	baud	serial port baud rate
**/
/**
*	Initializes logging
*	@param	baud		serial port baud rate
*	@param	max_level	maximum logging level
**/
void logger::init(const long baud, const log_level max_level)
{
	_max_level = max_level;
	Serial.begin(baud);
}

/**
*	Writes a formatted message with ERR log level into log
*	@param	msg	format string
**/
void logger::error(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_ERROR, msg, args);
}

/**
*	Writes a formatted message with INF log level into log
*	@param	msg	format string
**/
void logger::info(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_INFO, msg, args);
}

/**
*	Writes a formatted message with DBG log level into log
*	@param	msg	format string
**/
void logger::debug(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_DEBUG, msg, args);
}

/**
*	Writes a formatted message with ERR log level into log
*	@param	msg	format string
**/
void logger::error(const __FlashStringHelper* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_ERROR, msg, args);
}

/**
*	Writes a formatted message with INF log level into log
*	@param	msg	format string
**/
void logger::info(const __FlashStringHelper* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_INFO, msg, args);
}

/**
*	Writes a formatted message with DBG log level into log
*	@param	msg	format string
**/
void logger::debug(const __FlashStringHelper* msg, ...)
{
	va_list args;
	va_start(args, msg);
	print(LOG_DEBUG, msg ,args);
}

/**
*	Starts writing an event using log event writer.
*	@param		level	log level
*	@returns	log event writer
**/
log_event logger::begin_event(log_level level)
{
	if(level < _max_level)
	{
		return log_event(false);
	}

	print_header(level);
	return log_event(true);
}

/**	private members	**/

/**
*	Writes a formatted message into log
*	@param	level	log level
*	@param	format	format string
*	@params	args	format arguments list
**/
void logger::print(log_level level, const char* format, va_list args)
{
	if(level < _max_level)
	{
		return;
	}

	print_header(level);

	print_message(format, args);
	Serial.println();
	Serial.flush();
}

/**
*	Writes a formatted message into log
*	@param	level	log level
*	@param	format	format string
*	@params	args	format arguments list
**/
void logger::print(log_level level, const __FlashStringHelper* format, va_list args)
{
	if(level < _max_level)
	{
		return;
	}

	print_header(level);

	print_message(format, args);
	Serial.println();
	Serial.flush();
}

/**
*	Writes a log message header
*	@param	level	log level
*/
void logger::print_header(log_level level)
{
	const __FlashStringHelper* header;
	switch (level)
	{
	case LOG_ERROR:
		header = F("ERROR");
		break;
	case LOG_INFO:
		header = F("INFO");
		break;
	case LOG_DEBUG:
		header = F("DEBUG");
		break;
	default:
		break;
	}

	// TIME
	print_time(systime::now());
	Serial.print('\t');

	// LOG LEVEL
	Serial.print(header);
	Serial.print('\t');
}

/**
*	Writes a formatted local time into log
*	@param	time local time
**/
void logger::print_time(const systime& time)
{
	print_formatted_number(time.h, 4, ' ');
	Serial.print(':');
	print_formatted_number(time.min, 2);
	Serial.print(':');
	print_formatted_number(time.sec, 2);
	Serial.print('.');
	print_formatted_number(time.ms, 3);
}

/**
*	Writes a formatted decimal number into log
*	@param	value		a number to write
*	@param	precision	amount of decimal places
*	@param	placeholder	a placeholder character
**/
void logger::print_formatted_number(int value, int precision, char placeholder)
{
	print_number(Serial, value, precision, placeholder);
	return;
}

/**
*	Writes a formatted message into log
*	@param	format	format string
*	@params	args	format arguments list
**/
void logger::print_message(const __FlashStringHelper* format_ptr, va_list args) 
{
	uint16_t address = reinterpret_cast<uint16_t>(format_ptr);
	char c;
	size_t i = -1;
	while(true)
	{
		i++;
		c = pgm_read_byte(address + i);
		if(c == '\0')
		{
			break;
		}

		if (c == '%') 
		{
			++i;
			c = pgm_read_byte(address + i);
			print_format_placeholder(c, args);
		}
		else
		{
			Serial.print(c);
		}
	}
}

/**
*	Writes a formatted message into log
*	@param	format	format string
*	@params	args	format arguments list
**/
void logger::print_message(const char *format, va_list args) 
{
	// loop through format string
	for (; *format != 0; ++format)
	{
		if (*format == '%') 
		{
			++format;
			print_format_placeholder(*format, args);
		}
		else
		{
			Serial.print(*format);
		}
	}
}

/**
*	Writes a format message placeholder into log
*	@param	c		format placeholder code
*	@params	args	format arguments list
**/
void logger::print_format_placeholder(const char c, va_list& args) 
{
	if (c == '\0') 
	{
		return;
	}

	// '%' literal
	if (c == '%') 
	{
		Serial.print(c);
		return;
	}

	// String
	if(c == 's')
	{
		const char* s = (const char*)va_arg(args, int);
		Serial.print(s);
		return;
	}

	// PGMSPACE string
	if(c == 'S') 
	{
		const __FlashStringHelper* s = (const __FlashStringHelper*)va_arg(args, int);
		Serial.print(s);
		return;
	}

	// decimal integer
	if( c == 'd')
	{
		const int x = va_arg(args, int);
		Serial.print(x, DEC);
		return;
	}

	// hexadecimal integer
	if(c == 'x')
	{
		const int x = va_arg(args, int);
		Serial.print(x, HEX);
		return;
	}

	// hexadecimal integer with "0x" prefix
	if(c == 'X')
	{
		const int x = va_arg(args, int);
		Serial.print("0x");
		Serial.print(x, HEX);
		return;
	}

	// binary integer
	if(c == 'b') 
	{
		const int x = va_arg(args, int);
		Serial.print(x, BIN);
		return;
	}

	// binary integer with "0b" prefix
	if(c == 'B') 
	{
		const int x = va_arg(args, int);
		Serial.print("0b");
		Serial.print(x, BIN);
		return;
	}

	// decimal long integer
	if(c == 'l')
	{
		const long x = va_arg(args, long);
		Serial.print(x, DEC);
		return;
	}

	// a char
	if(c == 'c')
	{
		const char c = va_arg(args, int);
		Serial.print(c);
		return;
	}

	// a "T/F" boolean
	if(c == 't')
	{
		const bool b = va_arg(args, int);
		if (b)
		{
			Serial.print("T");
		}
		else 
		{
			Serial.print("F");
		}
		return;
	}

	// a "true/false" boolean
	if(c == 'T')
	{
		const bool b = va_arg(args, int);
		if (b)
		{
			Serial.print("true");
		}
		else 
		{
			Serial.print("false");
		}
		return;
	}

	// a float value
	if(c == 'f') 
	{
		float x = *(float*)va_arg(args, int);
		Serial.print(x);
		return;
	}

	// a double value
	if(c == 'F') 
	{
		double x = *(double*)va_arg(args, int);
		Serial.print(x);
		return;
	}

	Serial.print(c);
}
