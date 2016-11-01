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

/* Function for get new file size */
long unsigned int get_new_size(int size)
{
	int tmp = size%KLENGTH;
	int aux = size/KLENGTH;
//	printf("tmp: %d\n",tmp);
//	printf("size: %d\n",size);
//	printf("size/klength: %d\n",size/KLENGTH);
//	printf("tmp+npar: %d\n",tmp+NPAR);
//	printf("velikost: %d\n",((size/KLENGTH)*NLENGTH)+tmp+NPAR);
	if(aux == 0)
		return (tmp+NPAR);
	else
		return(tmp > 0 ? (((size/KLENGTH)*NLENGTH)+tmp+NPAR) : ((size/KLENGTH)*NLENGTH));
}


void shuffle(unsigned char codeword[])
{
	for(int x = 0, y = NLENGTH-1; x < KLENGTH; x+=2,y--)
	{
//		printf("x: %d|y: %d\n",x,y);
		char tmp;
		tmp = codeword[x];
		codeword[x] = codeword[y];
		codeword[y] = tmp;
	}
}


void interleaving(unsigned char L1[], unsigned char L2[],long int size)
{
	unsigned char L_tmp[size];
	printf("size: %ld\n",size);
	printf("IN: %s|\n",L1);
	#define ROW 255
	#define COL 3
	unsigned char L_aux[ROW*COL];
	int z = 0;
	int h = 0;
	for(int k = 0; k < size/(ROW*COL); k++)
	{	
		for(int i = 0; i < ROW*COL; i++)
		{			
//			printf("%d%c ",z, L1[(z%COL)*ROW+(z/COL)+ (ROW*k)*(COL-1)]);
			L_tmp[z] = L1[(z%COL)*ROW+(z/COL)+ (ROW*k)*(COL-1)];
			z++;
			
//			if((i+1)%COL == 0)
//				printf("\n");
			 
		}	
	}
	for(int p = z; p < size; p++)
	{
//		L2[p] = L1[(p%COL)*ROW+(p/COL)+ (ROW*(k+1))*(COL-1)];
		L_tmp[p] = L1[p]; //původní
		z++;
	}
	z--;
	printf("Ltmp: %s|\n",L_tmp);
	printf("%d\n",z);
	z -= ROW*COL;
	for(int p = z, x =0; p < size; p++,x++)
	{
		L_aux[x] = L_tmp[p]; 
	}
	//z = 0;
	printf("Můžu se nat o vysrat|");
	for(int x = 0; x < 24; x++)
	{
		printf("%c",L_aux[x]);
	}
	printf("\n");	

	
	for(int x = 0; x < ROW*COL; x++)
	{
		printf("Na: %d vkládám: %c| z  %d:\n",x,L_aux[(x%COL)*ROW+(x/COL)+ (ROW*0*(COL-1))],(x%ROW)*COL+(x/ROW));
		L2[x] = L_aux[(x%COL)*ROW+(x/COL)+ (ROW*0)*(COL-1)];
		h++;
//		z++;
	}
	printf("L2: |%s|\n",L2);
	printf("|");
	for(int x = 0; x < 24; x++)
	{
		printf("%c",L2[x]);
	}
	printf("\n");
	
	for(int y = size-ROW*COL; y != 0; y--)
	{
		printf("Na: %d vkládám: %c| z  %d:\n",h, L_tmp[z],z);
		L2[h] = L_tmp[z];
		h++;
		z--;
	}
	printf("H: %d\n",h);
	printf("L2: |%s|\n",L2);
	printf("|");
	for(int x = 0; x < 44; x++)
	{
		printf("%c",L2[x]);
	}
	printf("\n");	
}

void deinterleaving(unsigned char L1[], unsigned char L2[],long int size)
{
	printf("IN: %s|\n",L1);
	printf("size: %ld\n",size);
	unsigned char L_tmp[size];
	#define ROW 255
	#define COL 3
	int z = size-1-ROW*COL;
	int h = 0;
	
	for(int x = 0; x < ROW*COL; x++)
	{
		printf("X: %d\n",x);
		printf("Na: %d vkládám: %c| z  %d:\n",z,L1[(x%ROW)*COL+(x/ROW)+ (COL*0)*(ROW-1)],(x%ROW)*COL+(x/ROW)+ (COL*0)*(ROW-1));
		L_tmp[z] = L1[(x%ROW)*COL+(x/ROW)+ (COL*0)*(ROW-1)];
		h++;
		z++;
	}
	printf("Z: %d\n",z);
	
	for(int y = size-1-h, j = 0; y >= 0; j++, y--)
	{
		printf("Na: %d vkládám: %c| z  %d:\n",z,L1[h],h);
		L_tmp[y] = L1[h];
		h++;
		z++;
	}
	printf("Poprask!\n");
	for(int x = 0; x < 44; x++)
	{
		printf("%c",L_tmp[x]);
	}
	printf("\n");	
	z = 0;
	printf("Z: %d\n",z);
	
	
	for(int k = 0; k < size/(ROW*COL); k++)
	{	
		for(int i = 0; i < ROW*COL; i++)
		{			
//			printf("%d%c ",z, L1[(z%COL)*ROW+(z/COL)+ (ROW*k)*(COL-1)]);
			L2[z] = L_tmp[(z%ROW)*COL+(z/ROW)+ (COL*k)*(ROW-1)];
			z++;
//			if((i+1)%COL == 0)
//				printf("\n");
		}	
//		printf("\n");
	}
	for(int p = z; p < size; p++)
	{
//		L2[p] = L1[(p%ROW)*COL+(p/ROW)+ (COL*(k+1))*(ROW-1)];
		L2[p] = L_tmp[p];		// původní
		z++;
	}
	printf("Ltmp: %s|\n",L_tmp);
	
	printf("Zadek!\n");
	for(int x = 0; x < 44; x++)
	{
		printf("%c",L2[x]);
	}
	printf("\n");	
	
	z--;

}