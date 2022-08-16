#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define INT_SIZE 32

int var;
int main(void){
	register uint8_t i = 0; 
	for(;i < INT_SIZE; i += 2) var |= (1 << i);

	while(1){
		for(i = 0;i < INT_SIZE; ++i){
			(var & (1 << i)) ? printf("1") : printf("0");
		}
		printf("\n");

		var = ~var;
		sleep(2);									//2 seconds
	}
}
