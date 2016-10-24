#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/ecc.h"

#define NLENGTH	15

unsigned char codeword[16];

enum code
{
	ERR_OK = 0,
	ERR_INCORECT_INPUT,
	ERR_CANT_OPEN_INPUT,
	ERR_CANT_OPEN_OUTPUT,
	ERR_ELSE
};

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

void check_inputFile(FILE *fp)
{
	if(fp == NULL)
	{
		error_msg(ERR_CANT_OPEN_INPUT);
		exit(EXIT_FAILURE);
	}	
}

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
    char *result = (char *)malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


long unsigned int get_file_size(FILE *fp)
{
	if (fseek(fp, 0L, SEEK_END) == 0) {
		/* Get the size of the file. */
		return ftell(fp);
	}
	else
		return 0;
}

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
		long bufsize;
		if (fseek(fp, 0L, SEEK_END) == 0) {
			/* Get the size of the file. */
			bufsize = ftell(fp);
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


int
main (int argc, char *argv[])
{
	unsigned char *source = NULL;		// buffer for file
	/* Argument exist test*/
	check_args(argc);
	
	FILE *fp = fopen (argv[1], "r");
	/* Fileopen test*/
	check_inputFile(fp);
	
	/* Open/create output file*/
	FILE *outputFile = fopen(concat(argv[1],".ok"),"wb");
	check_outputFile(outputFile);	
		 
	
	source = fillBuffer(fp);
	long unsigned int len = get_file_size(fp);
	fclose(fp);
	
	/* RScode library init */
	initialize_ecc();
	
	//#########################
	int erasures[16];
	int nerasures = 0;
	//#########################
	
	/* Parse input file to 15/9b */
	unsigned char *aux = malloc(sizeof(char) * (NLENGTH+1));
	printf("size len: %ld\n",sizeof(aux));
	memset(aux,0,sizeof(char) * (NLENGTH+1));	
	unsigned int byteCnt = 0;
	

	//#########################
	//erasures[nerasures++] = len - 6;
	//#########################
	
	for(int x = 0; x < len; x++)
	{
		//printf("len: %ld\n",len);
		#define ML 9 + NPAR)
		if (x%NLENGTH == 0)
		{

			memset(aux,0,sizeof(char) * (NLENGTH+1));
			memcpy(aux,&source[x],NLENGTH);
			int tmp = (int)(strchr((char*)aux,'\0')-(char*)aux);	// Výpočet pozice '\0'
			int writeLen = ((tmp >= NLENGTH) ? (NLENGTH-NPAR) : tmp);			// Výpočet počtu bitů pro zápis
//			printf("writeLen: %d/%d/%d\n",tmp, writeLen, 15-NPAR);			
//			printf("index: %d\n",(int)(strchr((char*)aux,'\0')-(char*)aux));
//			printf("(%d-%d): %s\n",x,x+15-1,aux);
			
			decode_data(aux, NLENGTH);
			//###################################
			/* check if syndrome is all zeros */
			if (check_syndrome () != 0) {
			  correct_errors_erasures (aux, 
						   NLENGTH,
						   nerasures, 
						   erasures);

			  printf("Corrected codeword: \"%s\"\n", aux);
			}	
			printf("D(%d-%d): %s\n",x,x+NLENGTH-1,aux);
			//###################################			
//			fprintf(outputFile, "%c",aux[i]);
//			fprintf(outputFile, "%c",aux);
			fwrite(aux,sizeof(char),writeLen, outputFile);
			printf("Zápis -> %s\n",aux);
		}
		
////		printf("byteCnt: %d\n",byteCnt);
//		aux[byteCnt] = source[x];
//		if(byteCnt == 14)
//		{
//			printf("test: |%c|",aux[11]);
//			printf("Uncorected: \"%s\"\n",aux);
////			printf("Encode!\n");
//			decode_data(aux, 15);
//			
//			//###################################
//			/* check if syndrome is all zeros */
//			if (check_syndrome () != 0 && 'N' == aux[0]) {
//				printf("%s\n",aux);
//			  correct_errors_erasures (aux, 
//						   15,
//						   nerasures, 
//						   erasures);
//
//			  printf("Corrected codeword: \"%s\"\n", aux);
//			}	
//			//###################################
//			
//			//encode_data(aux, sizeof(char) * (len+1), codeword);
//			for(int i = 0; i < 9; i++)
//			{
//				printf("%c",aux[i]);
////				printf("i: %d\n",i);
//				if(aux[i] == '\0')
//				{
////					printf("i: %d\n",i);
////					printf("break\n");
//					break;				
//				}
//				fprintf(outputFile, "%c",aux[i]);
//			}
//			printf("\n");
//			memset(aux,0,sizeof(char) * (15+1));
//			byteCnt = 0;
//		}
//		else if(byteCnt == 16)
//		{
//			byteCnt = 0;
//			printf("\n");
//		}
//		else
//			byteCnt++;		
	}
	// Zbytek v aux je také třeba zakódovat (poslední rámec)
//	if(byteCnt != 0)
//	{
//		decode_data(aux, 15);
//		for(int i = 0; i < 9; i++)
//		{
//			printf("x\n");
//			fprintf(outputFile, "%c",aux[i]);
//		}
//	}	
	
	/* Now decode -- encoded codeword size must be passed */
	//decode_data(codeword, ML);
	
	
	exit(EXIT_SUCCESS);
}