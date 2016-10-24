#include "func.h"

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
	
	for(int x = 0; x < len; x++)
	{
		//printf("len: %ld\n",len);
		#define ML KLENGHT + NPAR)
		if (x%NLENGTH == 0)
		{
//			printf("x: %d\n",x);
//			printf("NLENGTH+1: %d\n",NLENGTH+1);
			memset(aux,0,sizeof(char) * (NLENGTH+1));
			memcpy(aux,&source[x],NLENGTH);

//			printf("Source: %c%c%c%c%c%c%c%c%c\n",source[173],source[174],source[175],source[176],source[177],source[178],source[179],source[180],source[181]);
			int tmp = (int)(strchr((char*)aux,'\0')-(char*)aux);	// Výpočet pozice '\0'
			int writeLen = ((tmp >= KLENGHT) ? KLENGHT : tmp);			// Výpočet počtu bitů pro zápis
//			printf("writeLen: %d/%d/%d\n",tmp, writeLen, 15-NPAR);			
//			printf("index: %d\n",(int)(strchr((char*)aux,'\0')-(char*)aux));
			printf("(%d-%d): %s\n",x,x+15-1,aux);
			
			int decodeLenght = (tmp >= KLENGHT) ? NLENGTH : tmp + NPAR;
//			printf("tmp: %d\n",tmp);
//			printf("decodeLength: %d\n",decodeLenght);
//			printf("aux: |%s|\n",aux);
			decode_data(aux, decodeLenght);
//			printf("aux: %s\n",aux);
			//###################################
			/* check if syndrome is all zeros */
			if (check_syndrome () != 0) {
			  correct_errors_erasures (aux, 
						   NLENGTH,
						   0, 
						   NULL);

			  printf("Corrected codeword: |%s|\n", aux);
			}	
			printf("D(%d-%d): |%s|\n",x,x+NLENGTH-1,aux);
			//###################################			
//			fprintf(outputFile, "%c",aux[i]);
//			fprintf(outputFile, "%c",aux);
			fwrite(aux,sizeof(char),writeLen, outputFile);
			printf("Zápis -> |%s|\n",aux);
		}
	}
	
	fclose(outputFile);
	free(aux);
	free(source);
	exit(EXIT_SUCCESS);
}