#include <stdio.h>
#include <stdint.h>

#define SIZE_INT 16

int num, count;
int main(void){
	scanf("%i", &num);

	register uint8_t i;
	for(i = 0; i < SIZE_INT; i++){
		if(num & (1 << i)) count++;
	}

	printf("%i\n", count);
	return 0;
}
