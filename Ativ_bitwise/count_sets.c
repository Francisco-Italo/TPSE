#include <stdio.h>
#include <stdint.h>

long long int num;

int main(void){
	scanf("%lld", &num);

	register uint8_t count = 0;
	while(num){
		if(num & (1 << 0)) count++;
		num >>= 1;
	}

	printf("%d\n", count);
	return 0;
}
