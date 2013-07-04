#pragma once

#include <stdarg.h>
#include "Arduino.h"

/**
*	Log text format string
*	+---------------+-----------------------------------------------+---------------------------------------+
*	|       Format	|       Description                             |       Argument type                   |
*	+---------------+-----------------------------------------------+---------------------------------------+
*	|       %%      |       '%'                                     |        -                              |
*	|       %s      |       string                                  |        char*                          |
*	|       %S      |       string (stored within PGMSPACE)         |        const __FlashStringHelper*     |
*	|       %c      |       char                                    |        char                           |
*	|       %d      |       integer                                 |        int                            |
*	|       %l      |       long                                    |        long                           |
*	|       %x      |       integer (hex)                           |        int                            |
*	|       %X      |       integer (hex with '0x' prefix)          |        int                            |
*	|       %b      |       integer (binary)                        |        int                            |
*	|       %B      |       integer (binary with '0b' prefix)       |        int                            |
*	|       %t      |       boolean ('t' or 'f')                    |        bool                           |
*	|       %T      |       boolean ('true' or 'false')             |        bool                           |
*	|       %f      |       float point value                       |        float*                         |
*	|       %F      |       float point value                       |        double*                        |
*	+---------------+-----------------------------------------------+---------------------------------------+
**/

/**
*	Log levels enumeration
**/
enum log_level
{
	/**
	*	DBG log level
	**/
	LOG_DEBUG,

	/**
	*	INF log level
	**/
	LOG_INFO,

	/**
	*	ERR log level
	**/
	LOG_ERROR
};

/**
*	Log event writer
**/
class log_event
{
public:
	/**
	*	Constructor
	*	@param	enable	indicates whether current log writer instance is allowed to write any data into log
	**/
	explicit log_event(bool enable);

	/**
	*	Destructor
	**/
	~log_event();

	/**
	*	Appends a formatted text into log
	*	@param	msg	format string
	**/
	void printf(const char* msg, ...);

	/**
	*	Appends a formatted text into log
	*	@param	msg	format string
	**/
	void printf(const __FlashStringHelper* msg, ...);

private:
	/**
	*	Indicates whether current log writer instance is allowed to write any data into log
	**/
	bool _enable;
};

/**
*	Local time type forward declaration
**/
class systime;

/**
*	Logger class
**/
class logger
{
public:
	/**
	*	Initializes logging
	*	@param	baud		serial port baud rate
	*	@param	max_level	maximum logging level
	**/
	void init(const int16_t baud, const log_level max_level = LOG_DEBUG);

	/**
	*	Writes a formatted message with ERR log level into log
	*	@param	msg	format string
	**/
	void error(const char* msg, ...);

	/**
	*	Writes a formatted message with INF log level into log
	*	@param	msg	format string
	**/
	void info(const char* msg, ...);

	/**
	*	Writes a formatted message with DBG log level into log
	*	@param	msg	format string
	**/
	void debug(const char* msg, ...);


	/**
	*	Writes a formatted message with ERR log level into log
	*	@param	msg	format string
	**/
	void error(const __FlashStringHelper* msg, ...);

	/**
	*	Writes a formatted message with INF log level into log
	*	@param	msg	format string
	**/
	void info(const __FlashStringHelper* msg, ...);

	/**
	*	Writes a formatted message with DBG log level into log
	*	@param	msg	format string
	**/
	void debug(const __FlashStringHelper* msg, ...);


	/**
	*	Starts writing an event using log event writer.
	*	@param		level	log level
	*	@returns	log event writer
	**/
	log_event begin_event(log_level level);

private:
	log_level _max_level;

	friend class log_event;

	/**
	*	Writes a formatted message into log
	*	@param	level	log level
	*	@param	format	format string
	*	@params	args	format arguments list
	**/
	void print(log_level level, const char* format, va_list& args);

	/**
	*	Writes a formatted message into log
	*	@param	level	log level
	*	@param	format	format string
	*	@params	args	format arguments list
	**/
	void print(log_level level, const __FlashStringHelper* format, va_list& args);

	/**
	*	Writes a log message header
	*	@param	level	log level
	**/
	static void print_header(log_level level);

	/**
	*	Writes a formatted local time into log
	*	@param	time local time
	**/
	static void print_time(const systime& time);

	/**
	*	Writes a formatted decimal number into log
	*	@param	value		a number to write
	*	@param	precision	amount of decimal places
	*	@param	placeholder	a placeholder character
	**/
	static void print_formatted_number(int16_t value, int8_t precision, char placeholder = '0');

	/**
	*	Writes a formatted message into log
	*	@param	format	format string
	*	@params	args	format arguments list
	**/
	static void print_message(const char* format, va_list& args);

	/**
	*	Writes a formatted message into log
	*	@param	format	format string
	*	@params	args	format arguments list
	**/
	static void print_message(const __FlashStringHelper* format, va_list& args);

	/**
	*	Writes a format message placeholder into log
	*	@param	c		format placeholder code
	*	@params	args	format arguments list
	**/
	static void print_format_placeholder(const char c, va_list& args);
};

/**
*	Logger static instance
**/
extern logger syslog;
