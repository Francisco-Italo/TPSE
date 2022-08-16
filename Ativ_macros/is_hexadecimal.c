#include <stdio.h>

#define N 100000
#define HAS_X(d) (d == 'x' || d == 'X') ? 1 : 0
#define IS_HEX(d) (HAS_X(d) || (d > 64 && d < 71) || (d > 96 && d < 103)) ? 1 : 0

int main(void){
	char number[N];
	scanf("%[^\n]", number);

	for(register unsigned short it = 0; number[it]; it++){
		if(IS_HEX(number[it])){
			printf("O número %s é hexadecimal\n", number);
			goto fin;
		}
	}
	printf("O número %s não é hexadecimal\n", number);
	fin:
	return 0;
}