#include <stdio.h>

int cryptograph(short number){
	short bit[8];
	for(short i = 1; i < 7; ++i){
		bit[i] = number & (1 << i);
	}
	if(bit[1] == 0) number &= ~(1 << 4);
	else number |= (1 << 4);

	if(bit[2] == 0) number &= ~(1 << 1);
	else number |= (1 << 1);

	if(bit[3] == 0) number &= ~(1 << 5);
	else number |= (1 << 5);

	if(bit[4] == 0) number &= ~(1 << 2);
	else number |= (1 << 2);

	if(bit[5] == 0) number &= ~(1 << 6);
	else number |= (1 << 6);

	if(bit[6] == 0) number &= ~(1 << 3);
	else number |= (1 << 3);

	return number;
}

short n;

int main(void){
	scanf("%d", &n);
	printf("Número criptografado: %d\n", cryptograph(n));
	return 0;
}
