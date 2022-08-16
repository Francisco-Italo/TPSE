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
//external leds
#define CM_conf_gpmc_ben1										0x0878
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
#define BUTTON_PIN												(1<<28) //gpio1_28

//unsigned int flagBlink;
unsigned int high_level;

/*****************************************************************************
**                INTERNAL FUNCTION PROTOTYPES
*****************************************************************************/
static void delay();
static void ledInit();
//static void ledToggle();
static void led_blink_seq_1();
static void led_blink_seq_2();

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int _main(void){

	//flagBlink=0;	//init flag
  	
	/* Configure the green LED control pin. */
  	ledInit();
  
  	while(1){
		if((HWREG(SOC_GPIO_1_REGS+GPIO_DATAIN) & (1 << 28)) && !high_level) high_level = HIGH;
		else if((HWREG(SOC_GPIO_1_REGS+GPIO_DATAIN) & (1 << 28)) && high_level) high_level = LOW;

		if(high_level) led_blink_seq_1();
		else led_blink_seq_2();
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
	
	//unsigned int val_temp; 	
	/*-----------------------------------------------------------------------------
	 *  configure clock GPIO in clock module
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_CM_PER_REGS+CM_PER_GPIO1) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;

	/*-----------------------------------------------------------------------------
	 * configure mux pin in control module
	 *-----------------------------------------------------------------------------*/
	
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_ben1) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a5) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a6) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a7) |= GPIO_PIN_MODE;
	HWREG(SOC_CONTROL_REGS+CM_conf_gpmc_a8) |= GPIO_PIN_MODE;

	/*-----------------------------------------------------------------------------
	 *  set pin direction 
	 *-----------------------------------------------------------------------------*/
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) &= ~(LED_USR0) & ~(LED_USR1) & ~(LED_USR2) & ~(LED_USR3);
	HWREG(SOC_GPIO_1_REGS+GPIO_OE) |= (BUTTON_PIN);
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
