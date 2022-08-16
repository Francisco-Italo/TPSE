#include <stdio.h>
#include <stdint.h>

#define SIZE 8

uint8_t num;
int main(void){
	scanf("%d", &num);

	num &= ~(1 << 3), num &= ~(1 << 4);
	num ^= (1 << 7), num ^= 1;

	printf("%X\n", num);
	return 0;
}
