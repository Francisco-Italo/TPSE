#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define INT_SIZE 32

char input[INT_SIZE];
int sum, num;
int main(void){
	scanf("%[^\n]", input);

	register int8_t buf, i, pot = 0;
	for(i = INT_SIZE-1; i > -1; --i){
		buf = input[i] - '0';
		if(buf >= 0){
			if(buf) num += pow(2, pot++);
			else pot++;
		}
	}

	for(i = 0; i < INT_SIZE; ++i){
		sum += (num & (1 << i));
	}

	printf("%d\n", sum);
	return 0;
}
