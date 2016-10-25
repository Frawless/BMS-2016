/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "func.h"

/* Function for print error message */
void error_msg(int err_code)
{
	if(err_code == ERR_INCORECT_INPUT)
		fprintf(stderr, "Incorect input!\n");
	else if (err_code == ERR_CANT_OPEN_INPUT)
		fprintf(stderr, "Can't open input file!\n");
	else if (err_code == ERR_CANT_OPEN_OUTPUT)
		fprintf(stderr, "Can't create output file!\n");	
}

/* Argument exist test*/
void check_args(int argc)
{
	if(argc != 2)
	{
		error_msg(ERR_INCORECT_INPUT);
		exit(EXIT_FAILURE);
	}
}

/* Function for check input file */
void check_inputFile(FILE *fp)
{
	if(fp == NULL)
	{
		error_msg(ERR_CANT_OPEN_INPUT);
		exit(EXIT_FAILURE);
	}	
}

/* Function for check output file */
void check_outputFile(FILE *fp)
{
	if(fp == NULL)
	{
		error_msg(ERR_CANT_OPEN_OUTPUT);
		exit(EXIT_FAILURE);
	}	
}

/* Function for concatenate two strings*/
/* http://stackoverflow.com/questions/8465006/how-to-concatenate-2-strings-in-c */
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/* Some debugging routines to introduce errors or erasures
   into a codeword. 
   */

/* Introduce a byte error at LOC */
void
byte_err (int err, int loc, unsigned char *dst)
{
  printf("Adding Error at loc %d, data %#x\n", loc, dst[loc-1]);
  dst[loc-1] ^= err;
}

/* Pass in location of error (first byte position is
   labeled starting at 1, not 0), and the codeword.
*/
void
byte_erasure (int loc, unsigned char dst[], int cwsize, int erasures[]) 
{
  printf("Erasure at loc %d, data %#x\n", loc, dst[loc-1]);
  dst[loc-1] = 0;
}

/* Function for get input file size */
long unsigned int get_file_size(FILE *fp)
{
	if (fseek(fp, 0L, SEEK_END) == 0) {
		/* Get the size of the file. */
		return ftell(fp);
	}
	else
		return 0;
}

/* Function for load bytes from file in buffer */
unsigned char *fillBuffer(FILE *fp)
{
	/* Fileopen test*/
	if(fp == NULL)
	{
		error_msg(ERR_CANT_OPEN_INPUT);
		exit(EXIT_FAILURE);
	}
	else
	{
		unsigned char *source = NULL;
		/* Reading file in buffer */
		/* Go to the end of the file. */
		/* http://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c */
		if (fseek(fp, 0L, SEEK_END) == 0) {
			/* Get the size of the file. */
			long bufsize = ftell(fp);
			if (bufsize == -1) { /* Error */ }

			/* Allocate our buffer to that size. */
			source = malloc(sizeof(char) * (bufsize + 1));

			/* Go back to the start of the file. */
			if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

			/* Read the entire file into memory. */
			size_t newLen = fread(source, sizeof(char), bufsize, fp);
			if ( ferror( fp ) != 0 ) {
				fputs("Error reading file", stderr);
			} else {
				source[newLen++] = '\0'; /* Just to be safe. */
			}
		}
		return source;
	}
}

int get_new_size(int size)
{
	int tmp = size%KLENGTH;
	return(tmp > 0 ? (((size/KLENGTH)*NLENGTH)+tmp+NPAR+1) : ((size/KLENGTH)*NLENGTH));
}