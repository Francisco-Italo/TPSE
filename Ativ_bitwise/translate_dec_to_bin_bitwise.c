#include <stdio.h>

#define INT_SIZE 32

int n, aux;
int main(void){
	scanf("%d", &n);

	for(int i = INT_SIZE-1; i > -1; --i){
		aux = n & (1 << i);
		aux >>= i;
		printf("%d", aux);
	}
	printf("\n");

	return 0;
}
