/*******************************************************************************

UART based command line interface (CLI)

$Header: $
$State:$ 

$Author:$
$Date:$
$Revision:$

$Log: cli.c.rca $


********************************************************************************/

/*******************************************************************************

INCLUDES

******************************************************************************/
#include "stdio.h"
#include "stm32f30x.h"
#include "stm32f3_EXEC.h"
#include "cli.h"
#include "stm32f3_Flash.h"
#include "stm32f3_Watchdog.h"
#include "stm32f3_DMA.h"
#include "stm32f3_discovery_board.h"

/*******************************************************************************

GLOBAL VARIABLES

******************************************************************************/
static uint8_t cli_string_buffer[CLI_SIZE];
static uint8_t cli_string[CLI_SIZE];
static uint32_t cli_index;

/*******************************************************************************

 LOCAL VARIABLES

******************************************************************************/
static CLI_CMD_Type cmd_cli_list[CLI_MAX_COMMANDS];     /*!< list of command function pointers */

/*******************************************************************************

 PRIVATE FUNCTIONS

******************************************************************************/
static void     cli_help(const uint8_t * cli_string);
static void     cli_set_memory(const uint8_t * cli_string);
static void     cli_get_memory(const uint8_t * cli_string);
static void     cli_erase_flash(const uint8_t * cli_string);
static void     cli_watchdog_trigger(const uint8_t * cli_string);
static void     cli_memset(const uint8_t * cli_string);
static void     cli_memcopy(const uint8_t * cli_string);
static void     cli_temperature(const uint8_t * cli_string);
static uint8_t  cli_list_init(uint8_t index, const uint8_t * string, uint8_t size, const uint8_t * help, CLICMDFuncPtr fptr);

static uint8_t  is_dec_num(uint8_t src);
static uint8_t  is_hex_num(uint8_t src);
static uint8_t  atoi(uint8_t src);
static void     memset(uint8_t * dst, uint16_t count, uint8_t value);
static void     strcpy(const uint8_t * src, uint8_t * dst, uint8_t count);
static uint8_t  strncmp(const uint8_t * src, const uint8_t * dst, uint8_t count);
static uint8_t  str_2_long(const uint8_t * src, uint32_t * dst);
static uint8_t  str_2_short(const uint8_t * src, uint16_t * dst);
static uint8_t  to_lower(const uint8_t src);

/*******************************************************************************

Sets the count number of bytes from the dst pointer to the value

\param dst The location to start setting the memory
\param count The number of bytes to set
\param value The value to write to the locations

\retval void

******************************************************************************/
void memset(uint8_t * dst, uint16_t count, uint8_t value){
	uint16_t i;
	
	i=0;
	while (i<count){
		dst[i] = value;
		i++;
	}
	
	return;
}

/*******************************************************************************

Compares the first count number of characters between the src and dst strings

\param src The first string to compare
\param dst The second string to compare
\param count The number of characters to compare

\retval return RETURN_SUCCESS if the strings match, else return RETURN_ERROR

******************************************************************************/
uint8_t strncmp(const uint8_t * src, const uint8_t * dst, uint8_t count){
	uint8_t i;
	
	for (i=0; i< count; i++){
		if (src[i] != dst[i]){
			return RETURN_ERROR;
		}
	}
	return RETURN_SUCCESS;
}


/*******************************************************************************

Copies the src string to the dst string for count number of characters
or until it detects the ASCII_NULL character in the src.

\param src The string to copy
\param dst The destination for copy operation
\param count The number of characters to copy

\retval void

******************************************************************************/
void strcpy(const uint8_t * src, uint8_t * dst, uint8_t count){
	while ((count >0) && (*src !=ASCII_NULL)){
		*dst ++ = *src ++;
		count --;
	}
	*dst = ASCII_NULL;
	return;
}


/*******************************************************************************

Converts an upper case ascii letter to a lower case one

\param src the letter to be converted to lower case

\retval The lower case value if the src was upper case, other wise it will return
 the src character.

******************************************************************************/
uint8_t to_lower(const uint8_t src){
	if ((src >= LETTER_A) && (src <= LETTER_Z))
		return src + 32;
	else
		return src;
}


/*******************************************************************************

Checks if the value passed in is a valid decimal number or not 

\param src the character to check if it is a decimal number 0-9

\retval RETURN_SUCCESS if this is a valid character to be dec number.  RETURN_ERROR
if it is not.

******************************************************************************/
uint8_t is_dec_num(uint8_t src){
	if ((src >= NUMBER_0) && (src <= NUMBER_9))
		return RETURN_SUCCESS;
	else
		return RETURN_ERROR;
}


/*******************************************************************************

Checks if the value passed in is a valid hexadecimal number or not

\param src the character to check if it is a hexadecimal number 0-9 or a-f

\retval RETURN_SUCCESS if this is a valid character to be a hex number.  RETURN_ERROR
if it is not.

******************************************************************************/
uint8_t is_hex_num(uint8_t src){
	
	uint8_t lower;
	
	lower = to_lower(src);
	
	if (is_dec_num(lower)==RETURN_SUCCESS){
		return RETURN_SUCCESS;
	}else if ((lower >= LETTER_a) && (lower <= LETTER_f)){
		return RETURN_SUCCESS;
	}else{
		return RETURN_ERROR;
	}
}


/*******************************************************************************
Converts an integer into an ascii character

\param src the character to turn into an ascii number

\retval the number that the character is

******************************************************************************/
uint8_t atoi(uint8_t src){
	if ((src >= NUMBER_0) && (src <= NUMBER_9))
		return  src - 0x30;
	else
		return src - 0x57;
}

/*******************************************************************************
Converts an ASCII string to an unsigned long (32 bits).

\param src The input string to convert to a long
\param dst The 32 bit unsigned long value 

\retval RETURN_SUCCESS if conversion works, returns a RETURN_ERROR if it did not

******************************************************************************/
uint8_t str_2_long(const uint8_t * src, uint32_t * dst){
	
	uint8_t i;
	uint8_t num;
	
	
	//
	// Since we are ORing in values, make sure we have all 0s to start
	//
	*dst =0x00000000;
	
	for (i=0; i<8; i++){
		if (is_hex_num(src[i])==RETURN_SUCCESS){
			num = atoi(src[i]) ;
			*dst|= num << ((7-i)*4);
		}else{
			return RETURN_ERROR;
		}
	}
	return RETURN_SUCCESS;
}

/*******************************************************************************

Converts an ASCII string to an unsigned short (16 bits).

\param src The input string to convert to a long
\param dst The 16 bit unsigned short value 

\retval RETURN_SUCCESS if conversion works, returns a RETURN_ERROR if it did not

******************************************************************************/
uint8_t str_2_short(const uint8_t * src, uint16_t * dst){
	
	uint8_t i;
	uint8_t num;
	
	*dst =0;
	for (i=0; i<4; i++){
		if (is_hex_num(src[i])==RETURN_SUCCESS){
			num = atoi(src[i]) ;
			*dst|= num << ((3-i)*4);
		}else{
			return RETURN_ERROR;
		}
	}
	return RETURN_SUCCESS;
}


/*******************************************************************************
This function will loop through the lsit of possible CLI commands and print to
the serial console the command and its help string

\param[in] cli_string the string to parse for command information

\retval None

******************************************************************************/
void cli_help(const uint8_t * cli_string){
	uint8_t i;
	CLI_CMD_Type * cmd_ptr;


	//
	// Start the pointer to the current command to the start of the list
	//
	cmd_ptr = &cmd_cli_list[0];

	//
	// Go through all the clis and print their help strings
	// 
	i=0;
	while (i<CLI_MAX_COMMANDS){
		//
		// only print the strings that are valid
		//
		if ((cmd_ptr[i].string[0] != ASCII_NULL) && (cmd_ptr[i].help[0] != ASCII_NULL)){
			printf("%s: ", cmd_ptr[i].string);
			printf("%s\r\n", cmd_ptr[i].help);
		}
		i++;
	}
	
	return;
}



/*******************************************************************************
This function parses the input string for a 32 bit pointer and 32 bit data and
then writes the data to the address of pointer

\param[in] cli_string the string to parse for command information

\retval None

\warning There is NO checking on the pointer field.  

******************************************************************************/
void cli_set_memory(const uint8_t * cli_string){
	uint32_t ptr=0;
	uint32_t data=0;
	uint32_t * p;
    uint32_t tmpPtr=0;
    uint32_t tmpData=0;
	
	str_2_long(&cli_string[9], &ptr);
	
	str_2_long(&cli_string[17], &data);
	
    //check if address is in the FLASH range
    if ((FLASH_BASE < ptr) && (ptr < (FLASH_BASE+0x00400000))){
      //write the first 16 bits
      if (FlashWrite16(ptr, (uint16_t)data) == 1) {
        //increment pointer
        tmpPtr = ptr + 2;
        //shift the data
        tmpData = data >> 16;
        //write the other 16 bits
        if (FlashWrite16(tmpPtr, (uint16_t)tmpData) == 1){
          printf("FLASH Write SUCCESSFUL");
        } else {
          printf("FLASH Write FAILED");
        }
      } else {
        printf("FLASH Write FAILED");
      }
    } else {
	  p = (uint32_t *) ptr;
      *p= data;
    }

	return;
}

/*******************************************************************************
This function parses the input string for a 32 bit pointer and then erases the
page in FLASH that contains the address

\param[in] cli_string the string to parse for command information

\retval None

\warning There is NO checking on the pointer field.  

******************************************************************************/
void cli_erase_flash(const uint8_t * cli_string){
	uint32_t ptr=0;
	
	str_2_long(&cli_string[10], &ptr);
	
    //check if address is in the FLASH range
    if ((FLASH_BASE < ptr) && (ptr < (FLASH_BASE+0x00400000))){
      //erase flash page
      if (FlashErasePage(ptr) == 1){
        printf("FLASH Erase SUCCESSFUL");
      } else {
        printf("FLASH Erase FAILED");
      }
    }

	return;
}

/*******************************************************************************
This function triggers the watchdog reset to occur

\param[in] cli_string the string to parse for command information

\retval None

\warning There is NO checking on the pointer field.  

******************************************************************************/
void cli_watchdog_trigger(const uint8_t * cli_string){
	Watchdog_Trigger();
	return;
}

/*******************************************************************************
This function parses the input string for a 1 byte dma channel, 4 bytes 
destination address, 4 bytes data to write and 2 bytes count of transfers

\param[in] cli_string the string to parse for command information

\retval None

\warning There is NO checking on the pointer field.  

******************************************************************************/
void cli_memset(const uint8_t * cli_string){
    uint8_t ch;
    uint32_t dst;
    uint32_t data;
    uint16_t cnt;
    DMA_Channel_TypeDef* dma;

    ch = atoi(cli_string[6]);
    str_2_long(&cli_string[7], &dst);
    str_2_long(&cli_string[15], &data);
    str_2_short(&cli_string[23], &cnt);
    //check if ch is between 1 and 7,
    //dst addr is in sram and cnt < 64K
    if (((ch>=1)&&(ch<=7)) &&
        ((SRAM_BASE <= dst)&&(dst <= (SRAM_BASE+0x9fff))) &&
        (cnt <= 65535)){
      switch(ch){
        case 1: dma = DMA1_Channel1;
                break;
        case 2: dma = DMA1_Channel2;
                break;
        case 3: dma = DMA1_Channel3;
                break;
        case 4: dma = DMA1_Channel4;
                break;
        case 5: dma = DMA1_Channel5;
                break;
        case 6: dma = DMA1_Channel6;
                break;
        case 7: dma = DMA1_Channel7;
                break;
      }
      DMA_MemSet(dma, dst, data, cnt);
    }
	

	return;
}

/*******************************************************************************
This function parses the input string for a 1 byte dma channel, 4 bytes 
source address, 4 bytes destination address and 2 bytes count of transfers

\param[in] cli_string the string to parse for command information

\retval None

\warning There is NO checking on the pointer field.  

******************************************************************************/
void cli_memcopy(const uint8_t * cli_string){
    uint8_t ch;
    uint32_t src;
    uint32_t dst;
    uint16_t cnt;
    DMA_Channel_TypeDef* dma;

    ch = atoi(cli_string[7]);
    str_2_long(&cli_string[8], &src);
    str_2_long(&cli_string[16], &dst);
    str_2_short(&cli_string[24], &cnt);
    //check if ch is between 1 and 7, src and
    //dst addr is in sram and cnt < 64K
    if (((ch>=1)&&(ch<=7)) &&
        ((SRAM_BASE <= src)&&(src <= (SRAM_BASE+0x9fff))) &&
        ((SRAM_BASE <= dst)&&(dst <= (SRAM_BASE+0x9fff))) &&
        (cnt <= 65535)){
      switch(ch){
        case 1: dma = DMA1_Channel1;
                break;
        case 2: dma = DMA1_Channel2;
                break;
        case 3: dma = DMA1_Channel3;
                break;
        case 4: dma = DMA1_Channel4;
                break;
        case 5: dma = DMA1_Channel5;
                break;
        case 6: dma = DMA1_Channel6;
                break;
        case 7: dma = DMA1_Channel7;
                break;
      }
      DMA_MemCopy(dma, src, dst, cnt);
    }

	return;
}

/*******************************************************************************
This function parses the input string for a 1 byte dma channel, 4 bytes 
source address, 4 bytes destination address and 2 bytes count of transfers

\param[in] cli_string the string to parse for command information

\retval None

\warning There is NO checking on the pointer field.  

******************************************************************************/
void cli_temperature(const uint8_t * cli_string){

    printf("Current Temperature: %d", Get_temperature());    
	return;
}


/*******************************************************************************
This function parses the input string for a 32 bit pointer and prints the data
from that address

\param[in] cli_string the string to parse for command information

\retval None

\warning There is NO checking on the pointer field.
******************************************************************************/
void cli_get_memory(const uint8_t * cli_string){
	uint32_t ptr=0;
	uint32_t data=0;
	uint32_t * p;
	uint32_t shcsr;

	str_2_long(&cli_string[9], &ptr);
  
	p = (uint32_t * )ptr;

	//
	// Turn off various faults during the reading of the data since we never know what kind of bad 
	// address will be asked for
	//
	shcsr =  SCB->SHCSR;
	__disable_irq();
	SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
	data = *p;
	SCB->SHCSR = shcsr;
	__enable_irq();

	printf("\r\nDATA = %8x @ 0x%lx", data, (uint32_t) p);
  
	return;
}


/*******************************************************************************
This function parses the input string for a 32 bit pointer and a 16 bit count.
It then loops starting from the pointer, count times and prints the data from
all of theses addresses

\param[in] cli_string the string to parse for command information

\retval None

\warning There is NO checking on the pointer field.

******************************************************************************/
void cli_dump_memory(const uint8_t * cli_string){
	uint16_t i=0;
	uint16_t count=0;
	uint32_t * p;
	uint32_t ptr;

	str_2_long(&cli_string[10], &ptr);
	p = (uint32_t *) ptr;

	str_2_short(&cli_string[18], &count);

	printf("\nADDRESS          Data\n");

	//
	// Turn off unaligned access traps since you never know when someone from the command
	// line will attempt to read from an unaligned address 
	//
	SCB->CCR &= ~SCB_CCR_UNALIGN_TRP_Msk;
	for (i=0; i< count; i++){   
		printf("%8x\t%8x\n", (uint32_t) p, *p);
		p++;
	}
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
	return;
}


/*******************************************************************************
This function will install a new cli command into the list of possible commands.
If the index is greater than the last spot, an error is returned.  Otherwise
the inputs are stored in the structure and the command becomes active

\param index The index in the table for this command
\param string The string to match for this command
\param size The size of the string for this command
\param help The help string for this command
\param fptr The handler function for this command

\retval RETURN_ERROR if index is past the last spot, else RETURN_SUCCESS

******************************************************************************/
uint8_t  cli_list_init(uint8_t index, const uint8_t * string, uint8_t size, const uint8_t * help, CLICMDFuncPtr fptr){

	//
	// Don't try to install if the command index is past the maximum number of commands, the array is 
	// a finite size
	// 
	if (index > CLI_MAX_COMMANDS){
		return RETURN_ERROR;
	}

	memset(&cmd_cli_list[index].string[0], size+1, ASCII_NULL);
	strcpy(string, &cmd_cli_list[index].string[0], size);
	cmd_cli_list[index].size = size;

	memset(&cmd_cli_list[index].help[0], CLI_SIZE*2, ASCII_NULL);
	strcpy(help, &cmd_cli_list[index].help[0], CLI_SIZE*2);

	cmd_cli_list[index].handler=fptr;
	return RETURN_SUCCESS;
}

/*******************************************************************************
This function will keep reading data from FPGA UART 0 until it is empty.  As 
each character is read in, it is checked to see if it is a carriage return. If it
is, echo it back and return RETURN_GET_COMMAND.  If it is not a carriage return 
and NOT a space put the lower case version of the character into the buffer.  
Update the pointer for the buffer and handle the wrap around condition.  When done
echo the character.

Once we run out of input characters, return RETURN_SUCCESS

\param[in, out] cli_index The index of the next byte
\param[in, out] cli_string The string that is getting the new byte

\retval RETURN_GET_COMMAND for a carriage return, else return RETURN_SUCCESS

******************************************************************************/
void CLI_AddByte(void){
	static uint8_t  new_char;
	uint32_t i;
	
			
	//
	// Get the new character from the UART and echo it
	//
	new_char = USART2->RDR & 0x00FF;

	//
	// User hit enter, got the CARRIAGE RETURN, alert the CLI Process task
	//
	if (new_char == CARRIAGE_RETURN){
		for (i=0; i<CLI_SIZE; i++){
			cli_string_buffer[i] = cli_string[i];			
		}
		printf("\r\n");
		
		Exec_SetTask(EXEC_TASK_CLI_PROCESS);		
		return;
	}
	
	//
	// Put non space characters into the global buffer
	//
	if (new_char != SPACE_CHAR){
		cli_string[(cli_index)] = to_lower(new_char);
		//
		// Wrap around the index to the global buffer.  We only do this when we put a character into the buffer
		//
		if (cli_index >= CLI_SIZE){
			(cli_index) = 0;
		}else{
			(cli_index) ++;
		}
	}

	//
    // Echo the character back
    //
	USART2->TDR=new_char;				
	return;
}

void CLI_Init(void){
	uint8_t i, j;

	//
	// Initialize the cli string that holds the characters from the UART
	//
	cli_index =0;
  
	//
	// Clear out the cmd table before populating it.
	//
	for (i=0; i<CLI_MAX_COMMANDS-1; i++){
		cmd_cli_list[i].size =0;
		cmd_cli_list[i].handler =NULL;
		for (j=0; j<CLI_SIZE; j++){
			cmd_cli_list[i].string[j] = ASCII_NULL;
		}
		for (j=0; j<CLI_SIZE*2; j++){
			cmd_cli_list[i].help[j] = ASCII_NULL;
		}
	}

	//
	// Clear out the cli string buffer
	//
	for (i=0; i<CLI_SIZE; i++){
		cli_string_buffer[i] =0;
	}

	//
	// Setup the function pointer handlers and commands this task will respond to
	//
	cli_list_init(0,  "help",           4, "Prints out a list of all commands", &cli_help);
	cli_list_init(1,  "getmemory",      9, "Gets data from address: get memory <address>", &cli_get_memory);
	cli_list_init(2,  "setmemory",      9, "Sets data to address: set memory <address> <data>", &cli_set_memory);
	cli_list_init(3,  "eraseflash",    10, "Erases FLASH page : erase flash <page>", &cli_erase_flash);
	cli_list_init(4,  "watchdog",       8, "Triggers watchdog reset : watchdog", &cli_watchdog_trigger);
	cli_list_init(5,  "memset",         6, "Write set value to memory range specified by count: memset", &cli_memset);
	cli_list_init(6,  "memcopy",        7, "Copies src data to dst specified by count : memset", &cli_memcopy);
	cli_list_init(7,  "temperature",   11, "Displays the current temperature : temperature", &cli_temperature);
	cli_list_init(CLI_MAX_COMMANDS-1, ASCII_NULL,  0, ASCII_NULL, NULL);  //ALWAYS THE LAST ONE!

	//
	// Go to the idle state at the start of the task so we Pend and wait for a 
	// message to act
	//
	printf("\n\rRaviThakur>");

	return;
  
}


/*******************************************************************************
This is the main loop of the cli task. We initialize the hardware we 
need to use and start the state machine.  

This task handles sending and receiving commands via the HOST UART.  This is the
primary control interface.  This task does not send the data about the monkey
watching the dot. 

\param[in]  ptr_args  Currently nothing

\retval None 

******************************************************************************/
void CLI_ProcessCommand(void){
	uint32_t i;
	CLI_CMD_Type * cmd_ptr;
	
	//
    // Start with the first command
    //
	i=0;

	//
	// Start the pointer to the current command to the start of the list
	//
	cmd_ptr = &cmd_cli_list[0];
	
	//
	// Loop through the clis looking for a match.  On a match call the handler for that string
	//
	while (cmd_ptr[i].string != ""){
		if (strncmp(&cli_string_buffer[0], cmd_ptr[i].string, cmd_ptr[i].size) == RETURN_SUCCESS){
			if (i <  CLI_MAX_COMMANDS){
				if (cmd_ptr[i].handler != NULL){
					cmd_ptr[i].handler(&cli_string_buffer[0]);
					
					//
					// Clear out the cli string buffer after the handler is done so we don't re-run the same command
					//
					for (i=0; i<CLI_SIZE; i++){
						cli_string_buffer[i] =0;
						cli_string[i]=0;
					}
				}
			}else{
				printf("CLI: INDEX ERROR %d\n",i);				
			}
			printf("\n\rRaviThakur>");
			cli_index = 0;
			break;
		}else{
			i++;
		}
	}
}
