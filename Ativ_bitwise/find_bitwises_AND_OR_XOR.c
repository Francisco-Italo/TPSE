#include <stdio.h>

int main(void){
	int n1, n2;
	scanf("%x %x", &n1, &n2);

	printf("Bitwise AND of the numbers %X and %X: %X\n", n1, n2, n1 & n2);
	printf("Bitwise OR of the numbers %X and %X: %X\n", n1, n2, n1 | n2);
	printf("Bitwise XOR of the numbers %X and %X: %d\n", n1, n2, n1 ^ n2);
	
	return 0;
}