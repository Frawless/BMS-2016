/* Example use of Reed-Solomon library 
 *
 * Copyright Henry Minsky (hqm@alum.mit.edu) 1991-2009
 *
 * This software library is licensed under terms of the GNU GENERAL
 * PUBLIC LICENSE
 *
 * RSCODE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RSCODE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Rscode.  If not, see <http://www.gnu.org/licenses/>.

 * Commercial licensing is available under a separate license, please
 * contact author for details.
 *
 * This same code demonstrates the use of the encodier and 
 * decoder/error-correction routines. 
 *
 * We are assuming we have at least four bytes of parity (NPAR >= 4).
 * 
 * This gives us the ability to correct up to two errors, or 
 * four erasures. 
 *
 * In general, with E errors, and K erasures, you will need
 * 2E + K bytes of parity to be able to correct the codeword
 * back to recover the original message data.
 *
 * You could say that each error 'consumes' two bytes of the parity,
 * whereas each erasure 'consumes' one byte.
 *
 * Thus, as demonstrated below, we can inject one error (location unknown)
 * and two erasures (with their locations specified) and the 
 * error-correction routine will be able to correct the codeword
 * back to the original message.
 * */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/ecc.h"
 
unsigned char msg[] = "Nervously I loaded the twin ducks aboard the revolving pl\
atform.";
unsigned char codeword[256];
 
/* Function for print error message */
void error_msg(int err_code)
{
	if(err_code == 1)
		fprintf(stderr, "Incorect input!\n");
	else if (err_code == 2)
		fprintf(stderr, "Can't open input file!\n");
	else if (err_code == 3)
		fprintf(stderr, "Can't create output file!\n");	
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


int
main (int argc, char *argv[])
{
	char *source = NULL;		// buffer for file
	/* Argument exist test*/
	if(argc != 2)
	{
		error_msg(1);
		exit(EXIT_FAILURE);
	}
	FILE *fp = fopen (argv[1], "r");
	/* Fileopen test*/
	if(fp == NULL)
	{
		error_msg(2);
		exit(EXIT_FAILURE);
	}
	else
	{	
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
		fclose(fp);
	}
	printf("%s",source);
		 
	/* Open/create output file*/
	FILE *outputFile = fopen(concat(argv[1],".out"),"w+");
	if(fp == NULL)
	{
		error_msg(3);
		exit(EXIT_FAILURE);
	}
	/* Write encoded data into output file */
	fputs(source,outputFile);
	printf("Zapsáno!\n");

	int erasures[16];
	int nerasures = 0;

	/* Initialization the ECC library */

	initialize_ecc ();

	/* ************** */

	/* Encode data into codeword, adding NPAR parity bytes */
	encode_data(msg, sizeof(msg), codeword);

	printf("Encoded data is: \"%s\"\n", codeword);

  #define ML (sizeof (msg) + NPAR)


	/* Add one error and two erasures */
	byte_err(0x35, 3, codeword);

	byte_err(0x23, 17, codeword);
	byte_err(0x34, 19, codeword);


	printf("with some errors: \"%s\"\n", codeword);

	/* We need to indicate the position of the erasures.  Eraseure
	   positions are indexed (1 based) from the end of the message... */

	erasures[nerasures++] = ML-17;
	erasures[nerasures++] = ML-19;


	/* Now decode -- encoded codeword size must be passed */
	decode_data(codeword, ML);

	/* check if syndrome is all zeros */
	if (check_syndrome () != 0) {
	  correct_errors_erasures (codeword, 
				   ML,
				   nerasures, 
				   erasures);

	  printf("Corrected codeword: \"%s\"\n", codeword);
	}

	exit(0);
}

