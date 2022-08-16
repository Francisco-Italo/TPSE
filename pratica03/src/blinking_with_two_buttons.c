/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/02/2017 20:05:55
 *       Revision:  none
 *       Compiler:  arm-none-eabi-gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  UFC-Quixadá
 *
 * =====================================================================================
 */

#include	"hw_types.h"
#include	"soc_AM335x.h"

/*****************************************************************************
**                INTERNAL MACRO DEFINITIONS
*****************************************************************************/
#define TIME													1000000
#define HIGH													1
#define LOW														0
//GPIO declarations
#define CM_PER_GPIO1											0xAC
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE					0x02
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK			0x00040000
//external pins
#define CM_conf_gpmc_ben1										0x0878
#define CM_conf_gpmc_a0											0x0840
//Internal (user) leds
#define CM_conf_gpmc_a5											0x0854
#define CM_conf_gpmc_a6											0x0858
#define CM_conf_gpmc_a7											0x085C
#define CM_conf_gpmc_a8											0x0860

#define GPIO_OE													0x134 //output enable
#define GPIO_DATAIN												0x138
#define GPIO_CLEARDATAOUT										0x190
#define GPIO_SETDATAOUT											0x194
#define GPIO_PIN_MODE											0x07u
//leds used
#define LED_USR3												(1<<24)
#define LED_USR2												(1<<23)
#define LED_USR1												(1<<22)
#define LED_USR0												(1<<21)
//Button leds
#define BUTTON_PIN_1											(1<<16) //gpio1_16
#define BUTTON_PIN_2											(1<<28) //gpio1_28

unsigned int sw_1 = LOW, sw_2 = LOW;
unsigned int signal_1, signal_2;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay();
static void ledInit();
static void led_blink_seq_1();
static void led_blink_seq_2();
static void led_blink_seq_3();
static void led_blink_seq_4();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int _main(void){

  	ledInit();
  
  	while(1){
		//TODO - Função que faz a alternância entre as 4 sequências
		signal_1 = (HWREG(SOC_GPIO_1_REGS+GPIO_DATAIN) & (1 << 28)) ? (HIGH) : (LOW);
		signal_2 = (HWREG(SOC_GPIO_1_REGS+GPIO_DATAIN) & (1 << 16)) ? (HIGH) : (LOW);

		if(signal_1 && !sw_1) sw_1 = HIGH;
		else if(signal_1 && sw_1) sw_1 = LOW;

		if(signal_2 && !sw_2) sw_2 = HIGH;
		else if(signal_2 && sw_2) sw_2 = LOW;

		if(!sw_1 && !sw_2) led_blink_seq_4();
		else if(!sw_1 && sw_2) led_blink_seq_2();
		else if(sw_1 && !sw_2) led_blink_seq_3();
		else led_blink_seq_1();
		//else delay();
	}

	return(0);
} /* ----------  end of function main  ---------- */


/*FUNCTION*-------------------------------------------------------
*
* Function Name : Delay
* Comments      :
*END*-----------------------------------------------------------*/
static void delay(){
	volatile unsigned int ra;
	for(ra = 0; ra < TIME; ra ++);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ledInit
 *  Description:  Initiate the required devices to do the task
 * It uses the HWREG() macro, which function is access the register of the hw,
 * and it's arguments are the devices memory adresses
 * =====================================================================================
 */
void ledInit( ){
	/*-----------------------------------------------------------------------------
	 *  configure clock GPIO in clock module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CM_PER_REGS+CM_PER_GPIO1) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
	
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_ben1) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a0) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a5) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a6) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a7) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a8) |= GPIO_PIN_MODE;

	/*-----------------------------------------------------------------------------
	 *  set pin direction 
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) &= ~(LED_USR0) & ~(LED_USR1) & ~(LED_USR2) & ~(LED_USR3);
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) |= (BUTTON_PIN_1) | (BUTTON_PIN_2);
}/* -----  end of function ledInit  ----- */


void led_blink_seq_1(){
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR3);
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR0);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR0);
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR1);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR1);
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR2);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR2);
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR3);
	delay();
}

void led_blink_seq_2(){
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR0) | (LED_USR1);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR0) | (LED_USR1);
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR2) | (LED_USR3);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR2) | (LED_USR3);
}

void led_blink_seq_3(){
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR1) | (LED_USR3);
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR0) | (LED_USR2);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR0) | (LED_USR2);
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR1) | (LED_USR3);
	delay();
}

void led_blink_seq_4(){
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR0);
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR1) | (LED_USR2) | (LED_USR3);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR3);
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR0) | (LED_USR1) | (LED_USR2);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR1);
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR0) | (LED_USR2) | (LED_USR3);
	delay();
	HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (LED_USR2);
	HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) |= (LED_USR0) | (LED_USR1) | (LED_USR3);
	delay();
}
