#pragma once

#include <stdio.h>

void print_dashes(int dash_count){
	for(int i = 0; i < dash_count; i++){
		printf("-");
	}
	printf("\n");
}
