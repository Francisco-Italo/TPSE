#include <stdio.h>

#define N 100000
#define HAS_BEGINNING_ZERO(d) d == '0' ? 1 : 0
#define HAS_B_OR_H_OR_SPACE(d) (d == 'h' || d == 'b' || d == 'H' || d == 'B' || d == ' ') ? 1 : 0
#define HAS_LETTERS(d) ((d > 64 && d < 71) || (d > 96 && d < 103)) ? 1 : 0

int main(void){
	char number[N];
	scanf("%[^\n]", number);
	
	register unsigned short it;
	for(it = 0; number[it]; ++it){
		if((it == 0 && HAS_BEGINNING_ZERO(number[it])) || HAS_B_OR_H_OR_SPACE(number[it]) || HAS_LETTERS(number[it])){
			printf("O número %s não é um número decimal\n", number);
			goto fin;
		}
	}
	printf("O número %s é um número decimal\n", number);
	
	fin:
	return 0;
}