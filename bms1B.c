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
	
	/* Parse input file to 15/9b */
	unsigned char *aux = malloc(sizeof(char) * (NLENGTH+1));
	printf("size len: %ld\n",sizeof(aux));
	memset(aux,0,sizeof(char) * (NLENGTH+1));	
	int blocks = 0;
	
	for(int x = 0; x < len; x++)
	{
		//printf("len: %ld\n",len);
		#define ML KLENGHT + NPAR)
		if (x%NLENGTH == 0)
		{
			int writeLen = (len / NLENGTH <= blocks ? ((len-x)%NLENGTH-NPAR) : KLENGTH);
			blocks++;

			memset(aux,0,sizeof(char) * (NLENGTH+1));
			memcpy(aux,&source[x],NLENGTH);
			printf("(%d-%d): %s\n",x,x+15-1,aux);
			
			int decodeLenght = (writeLen >= KLENGTH) ? NLENGTH : writeLen + NPAR;
			printf("WL: %d| DL: %d| 9: %d|\n", writeLen, decodeLenght,15-NPAR);

			decode_data(aux, decodeLenght);
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
			fwrite(aux,sizeof(char),writeLen, outputFile);
			printf("Zápis -> |%s|\n",aux);
		}
	}
	
	fclose(outputFile);
	free(aux);
	free(source);
	exit(EXIT_SUCCESS);
}