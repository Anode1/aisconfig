/**
 * Copyright (C) 2001 Vasili Gavrilov
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "common.h"
#include "params.h"

#define MAX_MAPPING_FILE_LINE 1024
#define MAX_KEY 1024

struct hash* params;

/* Forward declarations */
static int read_mapping(const char *filepath, struct hash *map);


int params_init(){
	int exit_code;
	params=hash_create(100);

	if((exit_code = read_mapping("system.properties", params)))
		return exit_code;

	return 0;
}

char* params_get(char *key){
	return (char*)hash_get(params, key);
}


static int read_mapping(const char *filepath, struct hash *map){

	FILE *fp = NULL;
	char line[MAX_MAPPING_FILE_LINE];
	char *delim;
	char *valStart;
	long lineNum = 0;
	long len;
	char key_buffer[MAX_KEY + 1];
	char val_buffer[MAX_MAPPING_FILE_LINE + 1];


	if ((fp = fopen(filepath, "r")) == NULL){
		printf("Cannot read %s\n", filepath);
		return -1;
	}

	if(FALSE)
		PRINT("%s loaded\n", filepath);

	errno=0;

	while(fgets(line, sizeof(line), fp) != NULL){

		if(errno){
			fprintf(stderr, "Can't read from file %s: %s\n", filepath, strerror(errno));
			exit(-1);
		}

		if(line[sizeof(line)-1] == '\n')
			line[sizeof(line)-1] = '\0';

		len = strlen(line);

		if(len == 0){
			PRINT("WARN: line %ld is empty -- ignored" ,lineNum);
			continue;
		}

		delim = strchr(line, '=');
		if(delim == NULL){
			continue;
		}

		*delim = '\0';
		valStart=delim+1;

		strcpy(key_buffer, line);
   	   	rtrim(key_buffer, ' '); /* remove '\n' (Glibc treats '\r' in fgets) */

   	   	strcpy(val_buffer, valStart);
		rtrim(val_buffer, '\n');
		rtrim(val_buffer, ' ');


     	hash_put(map, strdup(key_buffer), strdup(val_buffer));

		lineNum++;
	}/* fgets */

	errno=0;

	fclose(fp);

	if(errno)
		fprintf(stderr, "Can't close file descriptor %s: %s\n", filepath, strerror(errno));

	return 0;
}



