#include <stdio.h>

#define AND_OP(var) var & 0xF

struct{
	signed var : 4;
	unsigned it : 4;
	unsigned shifter : 5;
}tools;

long int n, aux;

int main(void){
	scanf("%li", &n);

	tools.shifter = 0;
	for(tools.it = 0; tools.it < 8; ++tools.it){
		aux = n;
		aux >>= tools.shifter;

		tools.var = AND_OP(aux);

		printf("Parte %d: %d\n", tools.it+1, tools.var);
		tools.shifter += 4;
	}

	return 0;
}
