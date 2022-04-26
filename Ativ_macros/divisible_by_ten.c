#include <stdio.h>

#define IS_DIV_TEN(var) (!(var % 10) ? 1:0)

int main(void){
	int n;
	scanf("%d", &n);

	IS_DIV_TEN(n) ? printf("%d é divisível por 10\n", n) : printf("%d não é divisível por 10\n", n);
	return 0;
}