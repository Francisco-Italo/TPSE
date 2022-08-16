#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define SIZE 32

#define PRINT_AND_WAIT(v, t) for(int i = 31; i > -1; --i) v & (1 << i) ? printf("1") : printf("0"); \
							 printf("\n"); \
							 sleep(t)

void blink_bits(int8_t time){
	int led = 0;
	register int8_t i;

	do{
		led |= (1 << 0);
		PRINT_AND_WAIT(led, time);

		for(i = 2;i < SIZE; i += 2){
			led |= (1 << i);
			led &= ~(1 << i-2);
			PRINT_AND_WAIT(led, time);
		}

		led ^= (11 << 30);
		PRINT_AND_WAIT(led, time);

		for(i = SIZE-3; i > -1; i -= 2){
			led |= (1 << i);
			led &= ~(1 << i+2);
			PRINT_AND_WAIT(led, time);
		}

		led &= ~(1 << 1);
	}while(1);
}

int8_t time;
int main(void){
	scanf("%d", &time);
	printf("Pressione CTRL + z para terminar a execução.\n");
	sleep(2);
	blink_bits(time);
}
