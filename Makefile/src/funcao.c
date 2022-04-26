/*
 * =====================================================================================
 *
 *       Filename:  funcao.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07-06-2015 19:19:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (), helderhdw@gmail.com 
 *   Organization:  Federal University of Ceara
 *
 * =====================================================================================
 */

#include "funcao.h"

float logNep(int n){
	int i;
	float soma=1;	

	for(i=1; i<=n; i++)
		soma = soma + 1.0/fat(i);

	return(soma);
}

int fat(int n){
	int i, iFat=1;

	for(i=1; i<=n; i++)
		iFat = iFat*i;
	
	return(iFat);
}


