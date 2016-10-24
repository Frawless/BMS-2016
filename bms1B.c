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