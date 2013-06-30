#include <syslog.h>

void plain_message_examples()
{
	/*******************************************/
	/*** Printing plain text messages        ***/
	/*******************************************/

	// Write an information message
	syslog.info("An example of information message");
	// Write an information message (storing message text in PROGMEM)
	syslog.info(F("An example of information message"));

	// Write an error message
	syslog.error("An example of error message");
	// Write an error message (storing message text in PROGMEM)
	syslog.error(F("An example of error message"));

	// Write an information message
	// This message will not be written due to LOG_INFO maximum logging level
	syslog.debug("An example of debug message");
	// Write an information message (storing message text in PROGMEM)
	// This message will not be written due to LOG_INFO maximum logging level
	syslog.debug(F("An example of debug message"));
}

void formatted_message_examples()
{
	/*******************************************/
	/*** Printing formatted messages         ***/
	/*******************************************/
	syslog.info(F("Printing formatted messages"));
	// %% format argument - prints out '%' symbol
	syslog.info(F("Print %%"));
	// %s format argument - prints out string literal
	syslog.info(F("Print %s"), "string literal");
	// %s format argument - prints out string literal stored in PGMSPACE
	syslog.info(F("Print %S"), F("string literal"));
	// %% format argument - prints out decimal integer
	syslog.info(F("Print %d"), 102);
	// %% format argument - prints out hexadecimal integer
	syslog.info(F("Print %x"), 102);
	// %% format argument - prints out hexadecimal integer with "0x" prefix
	syslog.info(F("Print %X"), 102);
	// %% format argument - prints out binary integer
	syslog.info(F("Print %b"), 102);
	// %% format argument - prints out binary integer with "0b" prefix
	syslog.info(F("Print %B"), 102);
	// %% format argument - prints out decimal long integer
	syslog.info(F("Print %l"), (long)102);
	// %% format argument - prints out a character
	syslog.info(F("Print %c"), '?');
	// %% format argument - prints out a "T"/"F" boolean
	syslog.info(F("Print %t"), true);
	syslog.info(F("Print %t"), false);
	// %% format argument - prints out a "true"/"false" boolean
	syslog.info(F("Print %T"), true);
	syslog.info(F("Print %T"), false);
	// %% format argument - prints out a float number
        float x = 1.02;
	syslog.info(F("Print %f"), &x);
	// %% format argument - prints out a double number
        double y = 1.02;
	syslog.info(F("Print %F"), &y);
}

void log_event_examples()
{
	/*******************************************/
	/*** Printing a log event                ***/
	/*******************************************/

	log_event e = syslog.begin_event(LOG_INFO);

	e.printf(F("Printing formatted messages\n"));
	// %% format argument - prints out '%' symbol
	e.printf(F("Print %%\n"));
	// %s format argument - prints out string literal
	e.printf(F("Print %s\n"), "string literal");
	// %s format argument - prints out string literal stored in PGMSPACE
	e.printf(F("Print %S\n"), F("string literal"));
	// %% format argument - prints out decimal integer
	e.printf(F("Print %d\n"), 102);
	// %% format argument - prints out hexadecimal integer
	e.printf(F("Print %x\n"), 102);
	// %% format argument - prints out hexadecimal integer with "0x" prefix
	e.printf(F("Print %X\n"), 102);
	// %% format argument - prints out binary integer
	e.printf(F("Print %b\n"), 102);
	// %% format argument - prints out binary integer with "0b" prefix
	e.printf(F("Print %B\n"), 102);
	// %% format argument - prints out decimal long integer
	e.printf(F("Print %l\n"), (long)102);
	// %% format argument - prints out a character
	e.printf(F("Print %c\n"), '?');
	// %% format argument - prints out a "T"/"F" boolean
	e.printf(F("Print %t\n"), true);
	e.printf(F("Print %t\n"), false);
	// %% format argument - prints out a "true"/"false" boolean
	e.printf(F("Print %T\n"), true);
	e.printf(F("Print %T\n"), false);
	// %% format argument - prints out a float number
        float x = 1.02;
	e.printf(F("Print %f\n"), &x);
	// %% format argument - prints out a double number
        double y = 1.02;
	e.printf(F("Print %F"), &y);
}

void setup()
{
	/*******************************************/
	/*** Initializing logger                 ***/
	/*******************************************/

	// Initialize logger
	syslog.init(/* baud rate */ 9600, /* maximum logging level */ LOG_INFO);

	/*******************************************/
	/*** Usage examples                      ***/
	/*******************************************/

	plain_message_examples();
	formatted_message_examples();
	log_event_examples();
}

void loop()
{
	delay(5000);
	syslog.info(F("Tick"));
}
