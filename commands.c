/*
 * commands.c
 *
 *  Created on: Oct 18, 2016
 *      Author: epenate
 */
#define __COMMANDS_GLOBAL
#include "includes.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "utils/ustdlib.h"

//extern volatile bool g_bStart;
//extern volatile unsigned int g_ui32counter;
//extern volatile unsigned int g_ui32button;
//extern volatile unsigned int g_ui32errorLow;
//extern volatile unsigned int g_ui32errorHigh;

//*****************************************************************************
//
// Command: help
//
// Print the help strings for all commands.
//
//*****************************************************************************
int
CMD_help(UART_Handle uart, int argc, char **argv)
{
    int32_t i32Index;
    int n;
    char buff[128];
    tCmdLineEntry *psCmdEntry;
    //
    // Keep the compiler happy.
    //
    (void)argc;
    (void)argv;
    (void)uart;

    //
    // Check the argument count and return errors for too many or too few.
    //
    if(argc == 1)
    {
    	//
    	// Start at the beginning of the command table
    	//
    	i32Index = 0;

    	//
    	// Get to the start of a clean line on the serial output.
    	//
    	n = sprintf(buff, "\n\rAvailable Commands\n\r------------------\n\n");
    	UART_write(uart, buff, n);
    	//
    	// Display strings until we run out of them.
    	//
    	while(g_psCmdTable[i32Index].pcCmd)
    	{
    		n = sprintf(buff, "\r\x1b[32;1m%17s\x1b[0m %s\n", g_psCmdTable[i32Index].pcCmd,
    				g_psCmdTable[i32Index].pcHelp);
    		UART_write(uart, buff, n);
    		i32Index++;
    	}
    }
    if(argc == 2)
    {
    	// Start at the beginning of the command table, to look for a matching
    	// command.
    	//
    	psCmdEntry = &g_psCmdTable[0];

    	//
    	// Search through the command table until a null command string is
    	// found, which marks the end of the table.
    	//
    	while (psCmdEntry->pcCmd) {
    		//
    		// If this command entry command string matches argv[0], then call
    		// the function for this command, passing the command line
    		// arguments.
    		// ustrncmp(argv[1], psCmdEntry->pcCmd, 5)
    		if (!ustrncmp(argv[1], psCmdEntry->pcCmd, 10)) {
    			n = sprintf(buff, "\n\rCommand: \x1b[32;1m%s\x1b[0m\n\r------------------\n\r  %s\n", argv[1], psCmdEntry->pcHelp + 3);
    			UART_write(uart, buff, n);
    		}

    		//
    		// Not found, so advance to the next entry.
    		//
    		psCmdEntry++;
    	}
    }

    if(argc > 2)
    {
    	return(CMDLINE_TOO_MANY_ARGS);
    }
    //
    // Leave a blank line after the help strings.
    //
    n = sprintf(buff, "\n\r");
    UART_write(uart, buff, n);

    return (0);
}

//*****************************************************************************
//
// Command: cls
//
// Clear the terminal screen.
//
//*****************************************************************************
int
CMD_cls(UART_Handle uart, int argc, char **argv)
{
	int n;
	char buff[32];
	(void)uart;

    n = sprintf(buff, "\033[2J\033[H");
    UART_write(uart, buff, n);
//    g_bFirstUpdate = true;
//    g_ui8FirstLine = 1;
    return (0);
}

//*****************************************************************************
//
// Command: start
//
// Clear the terminal screen.
//
//*****************************************************************************
int
CMD_start(UART_Handle uart, int argc, char **argv)
{
	int n;
	char buff[32];
	(void)uart;

	n = sprintf(buff, "\r Button tester sequence started\n\r");
	UART_write(uart, buff, n);
//	g_bStart = true;
	DOSet(0, true);
	return (0);
}

//*****************************************************************************
//
// Command: stop
//
// Clear the terminal screen.
//
//*****************************************************************************
int
CMD_stop(UART_Handle uart, int argc, char **argv)
{
	int n;
	char buff[32];
	(void)uart;

	n = sprintf(buff, "\r Button tester sequence stopped\n\r");
	UART_write(uart, buff, n);
//	g_bStart = false;
	DOSet(0, false);
	return (0);
}

//*****************************************************************************
//
// Command: stop
//
// Clear the terminal screen.
//
//*****************************************************************************
int
CMD_stat(UART_Handle uart, int argc, char **argv)
{
	int n;
	char buff[128];
	bool exit = false, dataAvailable;
	uint32_t in0, in1, led0, led1;
	(void)uart;


	n = sprintf(buff, "\r Press \"\x1b[32;1mq\x1b[0m\" to exit\n\r");
	UART_write(uart, buff, n);

	while(!exit){
		UART_control(uart, UART_CMD_ISAVAILABLE, &dataAvailable);
		if (dataAvailable){
			UART_read(uart, &buff, 1);
			if (buff[0] == 'q') {
				n = sprintf(buff, "\n\r");
				UART_write(uart, buff, n);
				exit = true;
				return (0);
			}
		}
		in0 = DIGet(0);
		in1 = DIGet(1);
		led0 = DOGet(0);
		led1 = DOGet(1);
		n = sprintf(buff, "\r In0: \x1b[36m%d\x1b[0m, In1: \x1b[32m%d\x1b[0m, LED0: \x1b[36m%d\x1b[0m, LED1: \x1b[32m%d\x1b[0m   ", in0, in1, led0, led1);
		UART_write(uart, buff, n);
		Task_sleep(100);
	}

	return (0);
}

//*****************************************************************************
//
// Command: stop
//
// Clear the terminal screen.
//
//*****************************************************************************
int
CMD_eeprom(UART_Handle uart, int argc, char **argv)
{
	int n;
	char buff[128];
	uint32_t pui32Data[4];

    //
    // Keep the compiler happy.
    //
    (void)argc;
    (void)argv;
    (void)uart;

    //
    // Check the argument count and return errors for too many or too few.
    //
    if(argc == 1)
    {
    	return(CMDLINE_TOO_FEW_ARGS);
    }
    if(argc > 2)
    {
    	return(CMDLINE_TOO_MANY_ARGS);
    }

    if(ustrncmp(argv[1], "read", 4) == 0)
    {
    	EEPROMRead(pui32Data, 0x000, sizeof(pui32Data));
    	n = sprintf(buff, "\r Counter: \x1b[36m%d\x1b[0m, buttons: \x1b[32m%d\x1b[0m, errorLow: \x1b[31;1m%d\x1b[0m, errorHigh: \x1b[31;1m%d\x1b[0m ", pui32Data[0], pui32Data[1], pui32Data[2], pui32Data[3]);
    	UART_write(uart, buff, n);
    }else if(ustrncmp(argv[1], "reset", 5) == 0){
    	pui32Data[0] = 0;
    	pui32Data[1] = 0;
    	pui32Data[2] = 0;
    	pui32Data[3] = 0;
//    	g_ui32counter = 0;
//    	g_ui32button = 0;
//    	g_ui32errorLow = 0;
//    	g_ui32errorHigh = 0;
    	EEPROMProgram(pui32Data, 0x000, sizeof(pui32Data));
    	EEPROMRead(pui32Data, 0x000, sizeof(pui32Data));
    	n = sprintf(buff, "\r Counter: \x1b[36m%d\x1b[0m, buttons: \x1b[32m%d\x1b[0m, errorLow: \x1b[31;1m%d\x1b[0m, errorHigh: \x1b[31;1m%d\x1b[0m ", pui32Data[0], pui32Data[1], pui32Data[2], pui32Data[3]);
    	UART_write(uart, buff, n);
    }else{
    	n = sprintf(buff, " Command not recognized. Please enter eeprom read or eeprom reset");
    	UART_write(uart, buff, n);
    }

    n = sprintf(buff, "\n\r");
    UART_write(uart, buff, n);
	return (0);
}


//*****************************************************************************
//
// Command: DIO
//
// Clear the terminal screen.
//
//*****************************************************************************


int
CMD_DICfgMode(UART_Handle uart, int argc, char **argv)
{
    int n;
    unsigned long index, mode;
    char buff[64];
    (void)uart;
    const char *ptr;

    if((argc == 1)||(argc == 2))
    {
        return(CMDLINE_TOO_FEW_ARGS);
    }
    if(argc > 3)
    {
        return(CMDLINE_TOO_MANY_ARGS);
    }

    index = ustrtoul(argv[1], NULL, 0);
    mode = ustrtoul(argv[2], NULL, 0);

    switch(mode){
    case DI_MODE_LOW:
        ptr = "DI_MODE_LOW";
        break;
    case DI_MODE_HIGH:
        ptr = "DI_MODE_HIGH";
        break;
    case DI_MODE_DIRECT:
        ptr = "DI_MODE_DIRECT";
        break;
    case DI_MODE_INV:
        ptr = "DI_MODE_INV";
        break;
    case DI_MODE_EDGE_LOW_GOING:
        ptr = "DI_MODE_EDGE_LOW_GOING";
        break;
    case DI_MODE_EDGE_HIGH_GOING:
        ptr = "DI_MODE_EDGE_HIGH_GOING";
        break;
    case DI_MODE_EDGE_BOTH:
        ptr = "DI_MODE_EDGE_BOTH";
        break;
    case DI_MODE_TOGGLE_LOW_GOING:
        ptr = "DI_MODE_TOGGLE_LOW_GOING";
        break;
    case DI_MODE_TOGGLE_HIGH_GOING:
        ptr = "DI_MODE_TOGGLE_HIGH_GOING";
        break;
    default:
        ptr = "error";
        break;
    }

    DICfgMode (index, mode);
    n = sprintf(buff, "\r Input %d configured as %s\n\r", index, ptr);
    UART_write(uart, buff, n);
    return (0);
}


int
CMD_cnfgOut(UART_Handle uart, int argc, char **argv)
{
    int n;
    char buff[64];
    (void)uart;


//  g_bStart = false;
    DOCfgMode (0, DO_MODE_BLINK_ASYNC, false);
    DOCfgBlink (0, DO_BLINK_EN_NORMAL, 500, 1000);
    n = sprintf(buff, "\r Output 0 is configured to blink D=500ms T=1000\n\r");
    UART_write(uart, buff, n);
    return (0);
}

int
CMD_DOCfgMode(UART_Handle uart, int argc, char **argv)
{
    int n;
    unsigned long index, mode,ulinv;
    bool binv;
    char buff[64];
    (void)uart;
    const char *ptr;

    if((argc == 1)||(argc == 2))
    {
        return(CMDLINE_TOO_FEW_ARGS);
    }
    if(argc > 4)
    {
        return(CMDLINE_TOO_MANY_ARGS);
    }

    index = ustrtoul(argv[1], NULL, 0);
    mode = ustrtoul(argv[2], NULL, 0);
    ulinv = ustrtoul(argv[3], NULL, 0);
    binv = ulinv ? true:false;

    switch(mode){
    case DO_MODE_LOW:
        ptr = "DO_MODE_LOW";
        break;
    case DO_MODE_HIGH:
        ptr = "DO_MODE_HIGH";
        break;
    case DO_MODE_DIRECT:
        ptr = "DO_MODE_DIRECT";
        break;
    case DO_MODE_BLINK_SYNC:
        ptr = "DO_MODE_BLINK_SYNC";
        break;
    case DO_MODE_BLINK_ASYNC:
        ptr = "DO_MODE_BLINK_SYNC";
        break;
    default:
        ptr = "error";
        break;
    }

    DOCfgMode (index, mode, binv);
    n = sprintf(buff, "\r Output %d configured as %s %s\n\r", index, ptr, binv ? "inverted":"not inverted");
    UART_write(uart, buff, n);
    return (0);
}

int
CMD_DOGet(UART_Handle uart, int argc, char **argv)
{
    int n;
    unsigned long ulindex;
    bool bval;
    char buff[32];
    (void)uart;

    if(argc == 1)
    {
        return(CMDLINE_TOO_FEW_ARGS);
    }
    if(argc > 2)
    {
        return(CMDLINE_TOO_MANY_ARGS);
    }

    ulindex = ustrtoul(argv[1], NULL, 0);
    bval = DOGet(ulindex);

    n = sprintf(buff, "\r Output %d value: %s\n\r", ulindex, bval ? "true":"false");
    UART_write(uart, buff, n);
    return (0);
}

//*****************************************************************************
//
// Table of valid command strings, callback functions and help messages.  This
// is used by the cmdline module.
//
//*****************************************************************************
tCmdLineEntry g_psCmdTable[] =
{
    {"help",   CMD_help,   " : Display list of commands." },
    {"?",      CMD_help,   " : Display list of commands." },
//    {"date",   CMD_date,   " : Set Date \"DD/MM/YYYY\"."},
//    {"time12", CMD_time12, " : Set Time 12HR style \"HH:MM:XX\" "
//                           "XX = AM or PM"},
//    {"time24", CMD_time24, " : Set Time 24HR style \"HH:MM\"."},
    {"cls",    CMD_cls,    " : Clear the terminal screen"},
	{"start",  CMD_start,  " : Start test"},
	{"stop",   CMD_stop,   " : Stop test"},
	{"status",   CMD_stat,   " : Show status"},
	{"eeprom",   CMD_eeprom, " : eeprom read, eeprom reset"},

	{"DICfgMode",  CMD_DICfgMode, " : configure LED0 to blink"},

	{"config",  CMD_cnfgOut, " : configure LED0 to blink"},
    {"DOCfgMode",  CMD_DOCfgMode, " : Configure output mode selector"},
    {"DOGet",  CMD_DOGet, " : Read output value .DOOut for output n"},
    { 0, 0, 0 }
};


