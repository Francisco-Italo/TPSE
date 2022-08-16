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

/*
 * DMTIMER_TCLR					T.R.M 20.2.4.7
 * DMTIMER_TCRR					T.R.M 20.2.4.8
 * DMTIMER_IRQENABLE_SET		T.R.M 20.1.5.7
 * DMTIMER_IRQENABLE_CLR		T.R.M 20.1.5.7
 * DMTIMER_TSICR				T.R.M 20.2.4.14
 * DMTIMER_TWPS					T.R.M 20.2.4.11
*/

#include "bbb_regs.h"
#include "hw_types.h"

typedef enum _pinNum{
	PIN1=1,
	PIN2,
	PIN3
}pinNum;

typedef enum _butNum{
	RIGHT=1,
	LEFT
}butNum;


unsigned int flag_timer, seq_num;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  disableWdt
 *  Description:  
 * =====================================================================================
 */
void disableWdt(void){
	HWREG(WDT_WSPR) = 0xAAAA;
	while((HWREG(WDT_WWPS) & (1<<4)));
	
	HWREG(WDT_WSPR) = 0x5555;
	while((HWREG(WDT_WWPS) & (1<<4)));
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  putCh
 *  Description:  
 * =====================================================================================
 */
void putCh(char c){
	while(!(HWREG(UART0_LSR) & (1<<5)));

	HWREG(UART0_THR) = c;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getCh
 *  Description:  
 * =====================================================================================
 */
char getCh(){
	while(!(HWREG(UART0_LSR) & (1<<0)));

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
 *         Name:  timerEnable
 *  Description:  
 * =====================================================================================
 */
void timerEnable(){
    /* Wait for previous write to complete in TCLR */
	DMTimerWaitForWrite(0x1);

    /* Start the timer */
	/* T.R.M 20.2.4.7 */
    HWREG(DMTIMER_TCLR) |= 0x1;
}/* -----  end of function timerEnable  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timerDisable
 *  Description:  
 * =====================================================================================
 */
void timerDisable(){
    /* Wait for previous write to complete in TCLR */
	DMTimerWaitForWrite(0x1);

    /* Stop the timer */
    HWREG(DMTIMER_TCLR) &= ~(0x1);
}/* -----  end of function timerEnable  ----- */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  delay
 *  Description:  
 * =====================================================================================
 */
void delay(unsigned int mSec){
#ifdef DELAY_USE_INTERRUPT
    unsigned int countVal = TIMER_OVERFLOW - (mSec * TIMER_1MS_COUNT);

   	/* Wait for previous write to complete */
	DMTimerWaitForWrite(0x2);

    /* Load the register with the re-load value */
	/* T.R.M 20.2.4.8 */
	HWREG(DMTIMER_TCRR) = countVal;
	
	flag_timer = false;

    /* Enable the DMTimer interrupts */
	HWREG(DMTIMER_IRQENABLE_SET) = 0x2;

    /* Start the DMTimer */
	timerEnable();

    while(flag_timer == false);

    /* Disable the DMTimer interrupts */
	HWREG(DMTIMER_IRQENABLE_CLR) = 0x2; 
#else
    while(mSec != 0){
        /* Wait for previous write to complete */
        DMTimerWaitForWrite(0x2);

        /* Set the counter value. */
        HWREG(DMTIMER_TCRR) = 0x0;

        timerEnable();

        while(HWREG(DMTIMER_TCRR) < TIMER_1MS_COUNT);

        /* Stop the timer */
        HWREG(DMTIMER_TCLR) &= ~(0x1);

        mSec--;
    }
#endif
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timerSetup
 *  Description:  
 * =====================================================================================
 */
void timerSetup(void){
	/*  Clock enable for DMTIMER7 TRM 8.1.12.1.25 */
	HWREG(CM_PER_TIMER7_CLKCTRL) |= 0x2;

	/*  Check clock enable for DMTIMER7 TRM 8.1.12.1.25 */
	while((HWREG(CM_PER_TIMER7_CLKCTRL) & 0x3) != 0x2);

	flag_timer = false;

#ifdef DELAY_USE_INTERRUPT
    /* Interrupt mask */
    HWREG(INTC_MIR_CLEAR2) |= INT_CONFIG_BIT(TINT7);//(95 --> Bit 31 do 3º registrador (MIR CLEAR2))
#endif
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  gpioSetup
 *  Description:  
 * =====================================================================================
 */
void gpioSetup(){
	/* Set clock for GPIO1, TRM 8.1.12.1.31 */
	HWREG(CM_PER_GPIO1_CLKCTRL) = 0x40002;

	/* Interrupt mask */
	/* 98 -> 1100010 -> MIR_CLEAR3 (11) bit 2 (00010) */
	/* 99 -> 1100011 -> MIR_CLEAR3 (11) bit 3 (00011) */
	HWREG(INTC_MIR_CLEAR3) |= INT_CONFIG_BIT(GPIOINT1A) | INT_CONFIG_BIT(GPIOINT1B);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  butConfig
 *  Description:  
 * =====================================================================================
 */

void butConfig(){
	HWREG(CONF_GPMC_A0)   |= 0x2F;
	HWREG(CONF_GPMC_BEN1) |= 0x2F;

	HWREG(GPIO1_OE) |= (1<<16) | (1<<28);

	/* Enabling IRQ generation on these GPIO pins. */
	HWREG(GPIO1_IRQSTATUS_SET_0) |= 1<<16;
	HWREG(GPIO1_IRQSTATUS_SET_1) |= 1<<28;

	/* Debounce enabling */
	HWREG(GPIO1_DEBOUNCENABLE) |= (1<<16) | (1<<28);

	/* Enable interrupt generation on detection of a rising edge.*/
	HWREG(GPIO1_RISINGDETECT) |= (1<<16) | (1<<28);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledConfig
 *  Description:  
 * =====================================================================================
 */

void ledConfig(){
	/* Configure pin mux for output GPIO */
	HWREG(CONF_GPMC_AD2) |= 0x7;
	HWREG(CONF_GPMC_AD3) |= 0x7;
	HWREG(CONF_GPMC_AD7) |= 0x7;

	/* clear pins 2, 3 and 7 for output */
	HWREG(GPIO1_OE) &= ~(1<<2) & ~(1<<3) & ~(1<<7);
	/* clear bit field that will be used */
	HWREG(GPIO1_SETDATAOUT) &= ~(1<<2) & ~(1<<3) & ~(1<<7);
	/* clear bit field that will be used */
	HWREG(GPIO1_CLEARDATAOUT) &= ~(1<<2) & ~(1<<3) & ~(1<<7);

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledOff
 *  Description:  
 * =====================================================================================
 */
void ledOff(pinNum pin){
	switch (pin) {
		case PIN1:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<2);
		break;
		case PIN2:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<3);
		break;
		case PIN3:
			HWREG(GPIO1_CLEARDATAOUT) |= (1<<7);
		break;
		default: break;
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledOn
 *  Description:  
 * =====================================================================================
 */
void ledOn(pinNum pin){
	switch (pin) {
		case PIN1:
			HWREG(GPIO1_SETDATAOUT) |= (1<<2);
		break;
		case PIN2:
			HWREG(GPIO1_SETDATAOUT) |= (1<<3);
		break;
		case PIN3:
			HWREG(GPIO1_SETDATAOUT) |= (1<<7);
		break;
		default: break;
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  timer_irq_handler
 *  Description:  
 * =====================================================================================
 */
void timer_irq_handler(void){

    /* Clear the status of the interrupt flags */
	HWREG(DMTIMER_IRQSTATUS) = 0x2; 

	flag_timer = true;

    /* Stop the DMTimer */
	timerDisable();

}

void gpio_irq_handler(butNum button){
	switch(button){
		case RIGHT:
			/* Clear the interrupt detection signal */
			HWREG(GPIO1_IRQSTATUS_0) |= 1<<16;

			if(seq_num == 0 || seq_num == 2) seq_num = 1;
			else seq_num = 0;
		break;
		case LEFT:
			/* Clear the interrupt detection signal */
			HWREG(GPIO1_IRQSTATUS_1) |= 1<<28;

			if(seq_num == 0 || seq_num == 1) seq_num = 2;
			else seq_num = 0;
		break;
		default: break;
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ISR_Handler
 *  Description:  
 * =====================================================================================
 */
void ISR_Handler(void){
	/* Verifica se é interrupção do DMTIMER7 */
	unsigned int irq_number = HWREG(INTC_SIR_IRQ) & 0x7f;

	switch(irq_number){
		case TINT7:		timer_irq_handler();		break;
		case GPIOINT1A:	gpio_irq_handler(RIGHT);	break;
		case GPIOINT1B:	gpio_irq_handler(LEFT);		break;
		default:									break;
	}

	/* Reconhece a IRQ */
	HWREG(INTC_CONTROL) = 0x1;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(void){
	/* Hardware setup */
	gpioSetup();
	timerSetup();
	butConfig();
	ledConfig();
	disableWdt();

#ifdef DELAY_USE_INTERRUPT 
	putString("Timer Interrupt program\n\r",25);
#else
	putString("Timer: ",7);
#endif

	seq_num = 0;
	while(true){
		switch(seq_num){
			case 1:
				putString("sequence 1\n\r", 12);
				ledOn(PIN1);
				delay(delay_opt_3);
				ledOn(PIN2);
				delay(delay_opt_3);
				ledOn(PIN3);
				delay(delay_opt_2);
				ledOff(PIN1);
				delay(delay_opt_3);
				ledOff(PIN2);
				delay(delay_opt_3);
				ledOff(PIN3);
				delay(delay_opt_1);
			break;
			case 2:
				putString("sequence 2\n\r", 12);
				ledOn(PIN2);
				delay(delay_opt_1);
				ledOn(PIN1);
				delay(delay_opt_1);
				ledOn(PIN3);
				delay(delay_opt_1);
				ledOff(PIN1);
				ledOff(PIN2);
				ledOff(PIN3);
				delay(delay_opt_1);
			break;
			default:
				putString("sequence 0\n\r", 12);
				ledOn(PIN1);
				delay(delay_opt_0);
				ledOn(PIN2);
				delay(delay_opt_0);
				ledOn(PIN3);
				delay(delay_opt_0);
				ledOff(PIN1);
				delay(delay_opt_0);
				ledOff(PIN2);
				delay(delay_opt_0);
				ledOff(PIN3);
				delay(delay_opt_2);
			break;
		}
	}
	return(0);
}
