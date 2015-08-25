#include "params.h"


#ifndef UNIT_TEST

/* Just unit test */
int main(void){

	int exit_code;
	char* key;

	if((exit_code=params_init())){
		return exit_code;
	}

	/*
	 * use here...
	 */
	key = "prop1";
	printf("value for key %s: %s", key, params_get(key));

	return 0;
}


#endif
