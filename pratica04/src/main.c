/*TO-DO
*Mudar o CM_PER_GPMC_AD12_REGS para um q tenha a tabela de interrupções no MIR_CLEAR2
*Fazer algumas macros de constantes
*Fazer macro da função de setar o bit no MIR_CLEAR
*
*/
/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  15/05/2018 14:32:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

//12 -> direita / 29 -> esquerda

#include "bbb_regs.h"
#include "hw_types.h"

#define TIME 0x3FFFFFF

/* blinking control system flags */
bool flag_gpio_12, flag_gpio_29;

typedef enum _pinNum{
	PIN1=1,
	PIN2,
	PIN3,
	PIN4
}pinNum;

typedef enum _butNum{
	RIGHT=1,
	LEFT
}butNum;


/* 
 * ===  FUNCTION  ==========================================================================================
 *         Name:  disableWdt
 *  Description:  function that disable the bbb watchdog, and so the system won't intent to boot the OS
 * =========================================================================================================
 */
void disableWdt(void){
	//The Watchdog Start/Stop (WDT_WSPR) Register holds the start-stop value that controls the internal start-stop FSM.
	//See section 20.4.4.1.12 on AM335x TRM
	//The Watchdog Write Posting Bits (WDT_WWPS) Register contains the write posting bits for all writable functional registers.
	//See section 20.4.4.1.10 on AM335x TRM
	HWREG(WDT_WSPR) = 0xAAAA;
	while((HWREG(WDT_WWPS) & (1<<4)));

	HWREG(WDT_WSPR) = 0x5555;
	while((HWREG(WDT_WWPS) & (1<<4)));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delay
 *  Description:  
 * =====================================================================================
 */
void delay(unsigned int mSec){
	volatile unsigned int count;
	for(count=0; count<mSec; count++);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  putCh
 *  Description:  
 * =====================================================================================
 */
void putCh(char c){
	//Read: reflect the error bits (BI, FE, PE) of the character at the top of the RX FIFO (next character to be read).
	//pos 5 - TXFIFOE: 0h = Transmit hold register (TX FIFO) is not empty./1h = Transmit hold register (TX FIFO) is empty.
	//The transmission is not necessarily completed.
	//See section 19.5.1.19 on AM335x TRM
	while(!(HWREG(UART0_LSR) & (1<<5)));

	//The transmitter section consists of the transmit holding register and the transmit shift register
	//The data is placed in the transmit shift register where it is shifted out serially on the TX output.
	//If the FIFO is disabled, location zero of the FIFO is used to store the data.
	//See section 19.5.1.1 on AM335x TRM
	HWREG(UART0_THR) = c;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getCh
 *  Description:  
 * =====================================================================================
 */
char getCh(){
	//pos 0 - RXFIFOE: 0h = No data in the receive FIFO./1h = At least one data character in the RX FIFO.
	while(!(HWREG(UART0_LSR) & (1<<0)));

	//The receiver section consists of the receiver holding register and the receiver shift register.
	//The receiver shift register receives serial data from RX input.
	//The data is converted to parallel data and moved to the RHR.
	//If the FIFO is disabled, location zero of the FIFO is used to store the single data character.
	//See section 19.5.1.2 on AM335x TRM
	return(HWREG(UART0_RHR));
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  putString
 *  Description:  
 * =====================================================================================
 */
int putString(char *str, unsigned int length){
	for(int i = 0; i < length; i++){
		putCh(str[i]);
	}
	return(length);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getString
 *  Description:  
 * =====================================================================================
 */
int getString(char *buf, unsigned int length){
	for(int i = 0; i < length; i ++){
		buf[i] = getCh();
	}
	return(length);
}

/* 	
 * ===  FUNCTION  ======================================================================
 *         Name:  gpioSetup
 *  Description:  Configure GPIO module selection and the interrupt trigger selection
 * =====================================================================================
 */
void gpioSetup(){
	/* set clock for GPIO1, TRM 8.1.12.1.29 */
	HWREG(CM_PER_GPIO1_CLKCTRL) = 0x40002;

	/* Interrupt mask */
	/* 98 -> 1100010 -> MIR_CLEAR3 (11) bit 2 (00010) */
	/* 99 -> 1100011 -> MIR_CLEAR3 (11) bit 3 (00011) */
	HWREG(INTC_MIR_CLEAR3) |= (1<<2) | (1<<3);
}

/* 
 * ===  FUNCTION  =========================================================================
 *         Name:  butConfig
 *  Description:  Configure pin multiplexing, input direction, and interrupt configuration
 * ========================================================================================
 */
void butConfig ( ){
	/* configure pin mux for input GPIO and enable pull-up */
	HWREG(CM_PER_GPMC_AD12_REGS) |= 0x2F;
	HWREG(CM_PER_GPMC_CSN0_REGS) |= 0x2F;

	/* Configure output mode for external pins */
	HWREG(GPIO1_OE) |= (1<<12) | (1<<29);

	flag_gpio_12 = false;
	flag_gpio_29 = false;

	/* Enabling IRQ generation on these GPIO pins. */
	HWREG(GPIO1_IRQSTATUS_SET_0) |= 1<<12;
	HWREG(GPIO1_IRQSTATUS_SET_1) |= 1<<29;

	/* Debounce enabling */
	HWREG(GPIO1_DEBOUNCENABLE) |= (1<<12) | (1<<29);

	/* Enable interrupt generation on detection of a rising edge.*/
	HWREG(GPIO1_RISINGDETECT) |= (1<<12) | (1<<29);
}/* -----  end of function butConfig  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledConfig
 *  Description:  Configure GPIO mode and output direction
 * =====================================================================================
 */
void ledConfig ( ){
	/* Configure pin mux for output GPIO */
	HWREG(CM_PER_GPMC_A5_REGS) |= 0x7;
	HWREG(CM_PER_GPMC_A6_REGS) |= 0x7;
	HWREG(CM_PER_GPMC_A7_REGS) |= 0x7;
	HWREG(CM_PER_GPMC_A8_REGS) |= 0x7;

	/* clear pins 21, 22, 23 and 24 for output, leds USR0, USR1, USR2 and USR3, TRM 25.3.4.3 */
	HWREG(GPIO1_OE) &= ~(1<<21) & ~(1<<22) & ~(1<<23) & ~(1<<24);
}/* -----  end of function ledConfig  ----- */


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledOff
 *  Description:  Turn-off user leds
 * =====================================================================================
 */
void ledOff(pinNum pin){
	/* clear bit field that will be used */
	HWREG(GPIO1_CLEARDATAOUT) &= ~(0xF<<21);
	switch (pin) {
		case PIN1:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<21);
		break;
		case PIN2:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<22);
		break;
		case PIN3:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<23);
		break;
		case PIN4:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<24);
		break;
		default: break;
	}/* -----  end switch  ----- */
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledOn
 *  Description:  Turn-on user leds
 * =====================================================================================
 */
void ledOn(pinNum pin){
	/* clear bit field that will be used */
	HWREG(GPIO1_SETDATAOUT) &= ~(0xF<<21);
	switch (pin) {
		case PIN1:
			HWREG(GPIO1_SETDATAOUT) |= (1<<21);
		break;
		case PIN2:
			HWREG(GPIO1_SETDATAOUT) |= (1<<22);
		break;
		case PIN3:
			HWREG(GPIO1_SETDATAOUT) |= (1<<23);
		break;
		case PIN4:
			HWREG(GPIO1_SETDATAOUT) |= (1<<24);
		break;
		default: break;
	}/* -----  end switch  ----- */
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  gpio_irq_handler()
 *  Description:  Execute the steps defined by the programmer
 * =====================================================================================
 */

void gpio_isr_handler(butNum button){
	switch(button){
		case RIGHT:
			/* Clear the interrupt detection signal */
			HWREG(GPIO1_IRQSTATUS_0) |= 1<<12;

			flag_gpio_12 = true;
			flag_gpio_29 = false;
		break;
		case LEFT:
			/* Clear the interrupt detection signal */
			HWREG(GPIO1_IRQSTATUS_1) |= 1<<29;

			flag_gpio_29 = true;
			flag_gpio_12 = false;
		break;
		default: break;
	}
}

/* 
 * ===  FUNCTION  ====================================================================================================
 *         Name:  ISR_Handler
 *  Description:  This function identify the interrupt number and signal the processor that the interrupt was treated
 * ===================================================================================================================
 */
void ISR_Handler(void){
	/* Verify active IRQ number */
	unsigned int irq_number = HWREG(INTC_SIR_IRQ) & 0x7f;

	if(irq_number == 98){
		gpio_isr_handler(RIGHT);
	}else if(irq_number == 99){
		gpio_isr_handler(LEFT);
	}

	/* acknowledge IRQ */
	HWREG(INTC_CONTROL) = 0x1;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  Calls all of the other functions in an eternal loop
 * =====================================================================================
 */
int main(void){
	
	/* Hardware setup */
	gpioSetup();
	ledConfig();
	butConfig();
	disableWdt();

	putString("All set, program started!\n\r",27);
	ledOff(PIN1);
	ledOff(PIN2);
	ledOff(PIN3);
	ledOff(PIN4);
	delay(TIME);


	while(true){
		if(flag_gpio_12){
			putString("right button press!\n\r",21);

			ledOn(PIN1);
			delay(TIME);
			ledOn(PIN2);
			delay(TIME);
			ledOn(PIN3);
			delay(TIME);
			ledOn(PIN4);
			delay(TIME);

			ledOff(PIN1);
			ledOff(PIN2);
			ledOff(PIN3);
			ledOff(PIN4);
			delay(TIME);

			flag_gpio_12 = false;
		}else if(flag_gpio_29){
			putString("left button press!\n\r",20);

			ledOff(PIN3);
			ledOn(PIN1);
			delay(TIME);

			ledOff(PIN1);
			ledOn(PIN4);
			delay(TIME);

			ledOff(PIN4);
			ledOn(PIN2);
			delay(TIME);

			ledOff(PIN2);
			ledOn(PIN3);
			delay(TIME);

			flag_gpio_29 = false;
		}else{
			ledOn(PIN1);
			ledOn(PIN2);
			ledOn(PIN3);
			ledOn(PIN4);
			delay(TIME>>2);
			ledOff(PIN1);
			ledOff(PIN2);
			ledOff(PIN3);
			ledOff(PIN4);
			delay(TIME>>2);
		}
	}

	return(0);
}
