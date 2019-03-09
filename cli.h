/*******************************************************************************
\file cli.h

This is the header file for the command line interface 

$Header:$

$State: Experimental $ 

$Author:$
$Date: $
$Revision: $

********************************************************************************/

#ifndef __CLI_TASK_H__
#define __CLI_TASK_H__

/*******************************************************************************

DEFINES

*******************************************************************************/

//
// This is the total number of commands that the CLI knows. Increasing this value will make the
// cmd_cli_list array larger 
//
#define CLI_MAX_COMMANDS 32

#define RETURN_SUCCESS 0x00
#define RETURN_ERROR 0x01
#define RETURN_GET_COMMAND 0x03

#define CLI_SIZE 32   /*!< Maximum number of CLI commands */

//
// Taken from http://asciitable.com/
//
#define LINE_FEED       0x0A
#define CARRIAGE_RETURN 0x0D
#define SPACE_CHAR      0x20
#define NUMBER_0        0x30
#define NUMBER_9        0x39
#define LETTER_A        0x41
#define LETTER_Z        0x5A
#define LETTER_a        0x61
#define LETTER_f        0x66
#define LETTER_z        0x7a

#define  ASCII_NULL   (uint8_t)0

/*******************************************************************************

GLOBAL VARIABLES

*******************************************************************************/


/*******************************************************************************

\typedef CLICMDFuncPtr
Function pointer for CLI Task

\struct CLI_CMD_Type
Structure for a CLI Command.  

*******************************************************************************/

//
// Function pointer type for the CLI_Type
//
typedef void (* CLICMDFuncPtr)(const uint8_t *);

//
// Structure used to represent each command
//
typedef struct{
  uint8_t string[CLI_SIZE];         /*!< The lower case, no spaces, string for this command */
  uint8_t size;                     /*!< The number of bytes in the string*/
  uint8_t help[CLI_SIZE*2];         /*!< The help message for this command*/
  void (*handler)(const uint8_t *); /*!< The function pointer to the handler for this command*/
} CLI_CMD_Type;                       

/*******************************************************************************

PUBLIC FUNCTIONS

********************************************************************************/
void CLI_Init(void);
void CLI_ProcessCommand(void);
void CLI_AddByte(void);

#endif
