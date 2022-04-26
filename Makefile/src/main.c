/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26-05-2015 17:31:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (), helderhdw@gmail.com 
 *   Organization:  Federal University of Ceara
 *
 * =====================================================================================
 */

#include "funcao.h"

int main (){
	int num;

	scanf("%d",&num);

	printf("nep:%f\n", logNep(num));
	return(0);
}
