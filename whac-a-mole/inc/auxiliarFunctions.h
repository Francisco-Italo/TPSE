#ifndef __AUXILIAR_FUNCTIONS_H
	#define __AUXILIAR_FUNCTIONS_H
#include "timers.h"

//enum macros
typedef enum _pinNum{
    PIN0,  PIN1,  PIN2,  PIN3,  PIN4,  PIN5,  PIN6,  PIN7,
    PIN8,  PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15,
    PIN16, PIN17, PIN18, PIN19, PIN20, PIN21, PIN22, PIN23,
    PIN24, PIN25, PIN26, PIN27, PIN28, PIN29, PIN30, PIN31,
}pinNum;

typedef enum _molePosition{
    HOLE1=1, HOLE2, HOLE3, HOLE4
}molePosition;
/*                       Functionalities                            */

// operational intern functions
void disableWdt(void);
void putCh(char c);
char getCh();
int putString(char *str, unsigned int length);
int getString(char *buf, unsigned int length);
void pinON(unsigned int gpio,pinNum pin);
void pinOFF(unsigned int gpio,pinNum pin);
int readButton(unsigned int gpio,btnPinNum pin);
void clearScreen();

//blinks functions
void intercalatedBlink(unsigned int gpio,pinNum pin[], int n, unsigned int TIME);
void sequentialBlink(unsigned int gpio,pinNum pin[], int n, unsigned int TIME);
void allBlink(unsigned int gpio,pinNum pin[], int n, unsigned int TIME);
void farEndBlink(unsigned int gpio,pinNum pin[], int n, unsigned int TIME);
void internBlink(unsigned int gpio,pinNum pin[], int n, unsigned int TIME);
void goOnGoOutBlink(unsigned int gpio,pinNum pins[], int n,unsigned int TIME);
void setLedsOFF(unsigned int gpio,pinNum pins[], int n);

// timer auxiliar functions
void delay(unsigned int mSec);
void timerIrqHandler(void);

//interruption auxiliar functions
#define INT_CONFIG_BIT(irq_num)					(1<<(irq_num & 0x1F))
void failStrikePrint(unsigned int gpio,pinNum pin[],int n,int score, pinNum buzzer);
void gameStartPrint();
void successStrikePrint(unsigned int gpio,pinNum buzzer,int score);
void winStrikePrint(unsigned int gpio,pinNum pins[], int n, pinNum buzzer, unsigned int TIME,int score);

#endif // 	__AUXILIAR_FUNCTIONS_H
