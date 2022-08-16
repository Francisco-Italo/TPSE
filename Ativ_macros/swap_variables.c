#include <stdio.h>

#define SWAP(a, b) a += b; b = a - b; a -= b

int main(void){
	int n1, n2;
	scanf("%d %d", &n1, &n2);

	printf("n1: %d n2: %d\n\n", n1, n2);

	SWAP(n1, n2);

	printf("After swap\n");
	printf("n1: %d n2: %d\n", n1, n2);
	return 0;
}