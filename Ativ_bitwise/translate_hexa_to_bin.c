#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define N 100000

char* translate(char digit){
	if(digit > 96 && digit < 103) digit &= ~(1 << 5);
	switch(digit){
		case '0': return "0000";
		case '1': return "0001";
		case '2': return "0010";
		case '3': return "0011";
		case '4': return "0100";
		case '5': return "0101";
		case '6': return "0110";
		case '7': return "0111";
		case '8': return "1000";
		case '9': return "1001";
		case 'A': return "1010";
		case 'B': return "1011";
		case 'C': return "1100";
		case 'D': return "1101";
		case 'E': return "1110";
		case 'F': return "1111";
		default: return "";
	}
}

char value[N], bin_num[4*N];
int main(void){
	scanf("%[^\n]", value);

	register uint8_t i;
	
	if(value[1] == 'x' || value[1] == 'X') i = 2;
	else i = 0;
	
	for(; value[i]; ++i) strcat(bin_num, translate(value[i]));

	printf("%s\n", bin_num);
	return 0;
}