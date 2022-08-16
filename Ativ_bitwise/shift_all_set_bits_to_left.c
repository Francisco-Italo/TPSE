#include <stdio.h>
#include <stdint.h>

#define SIZE_INT 16

void print_binary(int var){
	register int8_t iter;
	uint8_t vec_bin_num[SIZE_INT] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	for(iter = 0; var != 0; ++iter){
		vec_bin_num[iter] = var%2;
		var /= 2;
	}
	for(iter = SIZE_INT-1; iter >= 0; --iter) printf("%i", vec_bin_num[iter]);
	printf("\n");

	return;
}

int num;

int main(void){
	scanf("%i", &num);

	register int8_t i = 0, set_bits = 0;

	printf("Número na forma binária: ");
	print_binary(num);

	while(i < SIZE_INT){
		if(num & (1 << i)) set_bits++;
		i++;
	}

	for(i = SIZE_INT-1; i >= 0; --i){
		if(set_bits != 0){
			num |= (1 << i);
			set_bits--;
		}else if(num & (1 << i)) num &= ~(1 << i);
	}

	printf("Após os shifts: ");
	print_binary(num);

	return 0;
}
