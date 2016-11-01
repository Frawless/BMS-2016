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
	printf("source %s|:",source);
	for(int x = 0; x < len; x++)
	{
		printf("%c",source[x]);
	}
	printf("\n");
	unsigned char encodedMsg[len+1];
	printf("len: %ld\n",len);
	deinterleaving(source, encodedMsg, len);
	printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\n\n\n");
	for(int x = 0; x < len; x++)
	{
		printf("%c",encodedMsg[x]);
	}	
	
	/* RScode library init */
	initialize_ecc();
	
	/* Parse input file to 15/9b */
	unsigned char *aux = malloc(sizeof(char) * (NLENGTH+1));
	printf("size len: %ld\n",sizeof(aux));
	memset(aux,0,sizeof(char) * (NLENGTH+1));	
	int blocks = 0;
	
	for(int x = 0; x < len; x++)
	{
		if (x%NLENGTH == 0)
		{
			int writeLen = (len / NLENGTH <= blocks ? ((len-x)%NLENGTH-NPAR) : KLENGTH);
			blocks++;

			memset(aux,0,sizeof(char) * (NLENGTH+1));
			memcpy(aux,&encodedMsg[x],NLENGTH);
//			printf("(%d-%d): %s\n",x,x+15-1,aux);
//			printf("NL: %d| KL: %d\n",NLENGTH,KLENGTH);
			int decodeLenght = (writeLen >= KLENGTH) ? NLENGTH : writeLen + NPAR;
//			printf("WL: %d| DL: %d| 9: %d|\n", writeLen, decodeLenght,NLENGTH-NPAR);
//			printf("aux: %s|\n",aux);
//			for(int u = 0; u < NLENGTH; u++)
//			{
//				printf("%c",aux[u]);
//			}
//			printf("|\n");			
//			for(int u = 0; u < NLENGTH; u++)
//			{
//				printf("x:%d:%c",u,aux[u]);
//			}
//			printf("|\n");
//			aux[4] = '1';
//			printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
//			printf("sux: %s|\n",deshuffle(aux));
//			if(decodeLenght == NLENGTH)
//				shuffle(aux);
//			shuffle(aux);
//			printf("sux: %s|\n",aux);
			decode_data(aux, decodeLenght);
//			for(int u = 0; u < NLENGTH; u++)
//			{
//				printf("x:%d:%c",u,aux[u]);
//			}	
//			printf("|\n");
			//8hQRd1KkWB5nBbihg7GNUDv1OBtc
			//###################################
			/* check if syndrome is all zeros */
			if (check_syndrome () != 0) {
				printf("KOREKCE!!!!\n");
			  correct_errors_erasures (aux, 
						   NLENGTH,
						   0, 
						   NULL);

			  printf("Corrected codeword: |%s|\n", aux);
			}	
//			printf("D(%d-%d): |%s|\n",x,x+NLENGTH-1,aux);
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

