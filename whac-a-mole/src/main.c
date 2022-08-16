#include "auxiliarFunctions.h"

bool stop_game, hit_once;
unsigned int score, pos, delay_ms;
static int actualPositionMole;
pinNum ledPins[] = {PIN2,PIN3,PIN6,PIN7};
pinNum butPins[] = {BTN_PIN16,BTN_PIN28,BTN_PIN22,BTN_PIN24}
const unsigned int vetSeq[] =  {1,3,2,4,1,3,2,4,2,
								3,1,3,2,4,1,3,2,4,
								1,3,2,4,1,3,2,1,3,
								2,4,1,3,2,4,2,3,1,
								3,2,4,1,3,2,4,1,3,
								2,4,1,3,2,4,1,3,2,
								2,3,3,1,4,4,2,1,3};

void timerSetup(void){
	HWREG(SOC_CM_PER_REGS + CM_PER_TIMER7_CLKCTRL) |= MODULEMODE_ENABLE;

	while((HWREG(SOC_CM_PER_REGS + CM_PER_TIMER7_CLKCTRL) & 0x3) != MODULEMODE_ENABLE);

#if DELAY_USE_INTERRUPT
	/* Interrupt mask */
	HWREG(INTC_BASE + INTC_MIR_CLEAR2) |= INT_CONFIG_BIT(TINT7);
#endif
}

//Configurado com os novos registadores
void gpioSetup(){
	/* Set clock for GPIO1 and GPIO2 */
	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO1) |= OPTFCLKEN_GPIO_GDBCLK_FCLK_EN + MODULEMODE_ENABLE;
	HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2) |= OPTFCLKEN_GPIO_GDBCLK_FCLK_EN + MODULEMODE_ENABLE;

	/* Interrupt mask */
	HWREG(INTC_BASE + INTC_MIR_CLEAR3) |= INT_CONFIG_BIT(GPIO_INT_1_A) | INT_CONFIG_BIT(GPIO_INT_1_B);
	HWREG(INTC_BASE + INTC_MIR_CLEAR1) |= INT_CONFIG_BIT(GPIO_INT_2_A) | INT_CONFIG_BIT(GPIO_INT_2_B);
}


void butConfig ( ){
	HWREG(SOC_CONTROL_REGS + CONF_GPMC_BEN_1) |= GPIO_FUNC + PAD_PULLUP_PULLDOWN_EN + PAD_INPUT_VALUE_EN;
	HWREG(SOC_CONTROL_REGS + CONF_GPMC_A0)   |= GPIO_FUNC + PAD_PULLUP_PULLDOWN_EN + PAD_INPUT_VALUE_EN;
	HWREG(SOC_CONTROL_REGS + CONF_LCD_VSYNC) |= GPIO_FUNC + PAD_PULLUP_PULLDOWN_EN + PAD_INPUT_VALUE_EN;
	HWREG(SOC_CONTROL_REGS + CONF_LCD_PCLK)  |= GPIO_FUNC + PAD_PULLUP_PULLDOWN_EN + PAD_INPUT_VALUE_EN;

	HWREG(SOC_GPIO_1_REGS + GPIO_OE) |= (1<<16) | (1<<28);
	HWREG(SOC_GPIO_2_REGS + GPIO_OE) |= (1<<22) | (1<<24);

	/* Enabling IRQ generation on these GPIO pins. */
	HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_SET_0) |= 1<<16;
	HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_SET_1) |= 1<<28;
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET_0) |= 1<<22;
	HWREG(SOC_GPIO_2_REGS + GPIO_IRQSTATUS_SET_1) |= 1<<24;

	/* Debounce enabling */
	HWREG(SOC_GPIO_1_REGS + GPIO_DEBOUNCENABLE) |= (1<<16) | (1<<28);
	HWREG(SOC_GPIO_2_REGS + GPIO_DEBOUNCENABLE) |= (1<<22) | (1<<24);

	/* Enable interrupt generation on detection of a rising edge.*/
	HWREG(SOC_GPIO_1_REGS + GPIO_RISINGDETECT) |= (1<<16) | (1<<28);
	HWREG(SOC_GPIO_2_REGS + GPIO_RISINGDETECT) |= (1<<22) | (1<<24);
}/* -----  end of function butConfig  ----- */


void ledConfig ( ){
   /* Configure pin mux for output GPIO */
	HWREG(SOC_CONTROL_REGS + CONF_GPMC_AD2)  |= GPIO_FUNC;
	HWREG(SOC_CONTROL_REGS + CONF_GPMC_AD3)  |= GPIO_FUNC;
	HWREG(SOC_CONTROL_REGS + CONF_GPMC_AD6)  |= GPIO_FUNC;
	HWREG(SOC_CONTROL_REGS + CONF_GPMC_AD7)  |= GPIO_FUNC;
	HWREG(SOC_CONTROL_REGS + CONF_GPMC_AD14) |= GPIO_FUNC;

	/* clear pins 2, 3, 6 and 7 for output */
	HWREG(SOC_GPIO_1_REGS + GPIO_OE) &= ~(1<<2) & ~(1<<3) & ~(1<<6) & ~(1<<7) & ~(1<<14);
	/* clear bit field that will be used */
	HWREG(SOC_GPIO_1_REGS + GPIO_DATAOUT) &= ~(1<<2) & ~(1<<3) & ~(1<<6) & ~(1<<7) & ~(1<<14);

}/* -----  end of function ledConfig  ----- */

void strikeChecker(unsigned int gpio, unsigned int irqStatus, pinNum pin, molePosition hole){
	if(stop_game && hole == HOLE1){									//restart game
		gameStartPrint();
		score = 0;
		stop_game = false;
	}else if(actualPositionMole == hole && !hit_once){				//success strike
		score++;
		hit_once = true;
		if(score == 50){											//win the game
			winStrikePrint(SOC_GPIO_1_REGS,ledPins,4,PIN14,500,score);
			delay_ms = 1000;
			stop_game = true;
			return;
		}
		successStrikePrint(SOC_GPIO_1_REGS,PIN14,score);
		if(delay_ms > 500) delay_ms -= 25;
		else if(delay_ms > 250) delay_ms -= 20;
		else if (delay_ms > 200) delay_ms -= 10;
	}else{															//game over
		failStrikePrint(SOC_GPIO_1_REGS,ledPins,2,score, PIN14);
		delay_ms = 1000;
		stop_game = true;
	}

	HWREG(gpio+irqStatus) |= 1<<pin;
}

void gpioIrqHandler(int btn){
    /* Clear the status of the interrupt flags */
	switch(btn){
		case HOLE1: strikeChecker(SOC_GPIO_1_REGS,GPIO_IRQSTATUS_0,butPins[0],HOLE1); break;
		case HOLE2: strikeChecker(SOC_GPIO_1_REGS,GPIO_IRQSTATUS_1,butPins[1],HOLE2); break;
		case HOLE3: strikeChecker(SOC_GPIO_2_REGS,GPIO_IRQSTATUS_0,butPins[2],HOLE3); break;
		case HOLE4: strikeChecker(SOC_GPIO_2_REGS,GPIO_IRQSTATUS_1,butPins[3],HOLE4); break;
	}
}

void ISR_Handler(void){
	/* Verifica a interrupção disparada */
	unsigned int irq_number = HWREG(INTC_BASE+INTC_SIR_IRQ) & 0x7f;

	switch(irq_number){
		case TINT7:       timerIrqHandler();      break;
		case GPIO_INT_1_A: gpioIrqHandler(HOLE1); break;
		case GPIO_INT_1_B: gpioIrqHandler(HOLE2); break;
		case GPIO_INT_2_A: gpioIrqHandler(HOLE3); break;
		case GPIO_INT_2_B: gpioIrqHandler(HOLE4); break;
		default:                                  break;
	}

	/* Sinaliza que a interrupção foi tratada */
	HWREG(INTC_BASE+INTC_CONTROL) = 0x1;
}

int main(void){
	/* Hardware setup */
	gpioSetup();
	timerSetup();
	butConfig();
	ledConfig();

	internBlink(SOC_GPIO_1_REGS,ledPins,4,125);
	internBlink(SOC_GPIO_1_REGS,ledPins,4,125);

	/* setup environment variables */
	delay_ms = 1000;
	pos = 0;
	actualPositionMole = vetSeq[pos];
	stop_game = false;

	gameStartPrint();

	while(true){
		hit_once = false;
		setLedsOFF(SOC_GPIO_1_REGS,ledPins,4);

		while(stop_game);

		actualPositionMole = vetSeq[pos];

		switch (actualPositionMole){
			case HOLE1: pinON(SOC_GPIO_1_REGS,ledPins[0]); break;
			case HOLE2: pinON(SOC_GPIO_1_REGS,ledPins[1]); break;
			case HOLE3: pinON(SOC_GPIO_1_REGS,ledPins[2]); break;
			case HOLE4: pinON(SOC_GPIO_1_REGS,ledPins[3]); break;
		}

		delay(TIME);
		pos = (pos+1)%63;
	}

	return(0);
}
