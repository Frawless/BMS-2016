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

	unsigned char encodedMsg[len+1];
	printf("len: %ld\n",len);
	deinterleaving(source, encodedMsg, len);
	
	/* RScode library init */
	initialize_ecc();
	
	/* Parse input file to 15/9b */
	unsigned char *aux = malloc(sizeof(char) * (NLENGTH+1));
//	printf("size len: %ld\n",sizeof(aux));
	memset(aux,0,sizeof(char) * (NLENGTH+1));	
	int blocks = 0;
	int move = 0;
	for(int x = 0; x < len; x++)
	{
		if (x%NLENGTH == 0)
		{
			int writeLen = (len / NLENGTH <= blocks ? ((len-x)%NLENGTH-NPAR) : KLENGTH);
			blocks++;

			memset(aux,0,sizeof(char) * (NLENGTH+1));
			
			int decodeLenght = (writeLen >= KLENGTH) ? NLENGTH : writeLen + NPAR;
			
//			memcpy(aux,&encodedMsg[x],decodeLenght);
			
			memmove(aux,encodedMsg+move,decodeLenght);
			move += decodeLenght;

			decode_data(aux, decodeLenght);

			/* check if syndrome is all zeros */
			if (check_syndrome () != 0) {
//				printf("KOREKCE!!!!\n");
			  correct_errors_erasures (aux, 
						   NLENGTH,
						   0, 
						   NULL);

//			  printf("Corrected codeword: |%s|\n", aux);
			}	
			fwrite(aux,sizeof(char),writeLen, outputFile);
		}
	}
	
	fclose(outputFile);
	free(aux);
	free(source);
	exit(EXIT_SUCCESS);
}

