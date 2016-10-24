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
	FILE *outputFile = fopen(concat(argv[1],".out"),"wb");
	check_outputFile(outputFile);	
		 
	
	source = fillBuffer(fp);
	long unsigned int len = get_file_size(fp);
	fclose(fp);
	
	/* RScode library init */
	initialize_ecc();
	
	/* Parse input file to 15/9b */
	unsigned char *aux = malloc(sizeof(char) * (len+1));
	memset(aux,0,sizeof(char) * (9+1));	
	unsigned int byteCnt = 0;
	bool tmp = true;
	for(int x = 0; x < len; x++)
	{
		aux[byteCnt] = source[x];
		if(byteCnt == sizeof(aux))
		{
			printf("aux-> \'%s\'\n",aux);
			//printf("Encode!\n");
			encode_data(aux, sizeof(aux)+1, codeword);
			printf("cod-> \'%s\'\n",codeword);
			// Simulace chyby
			if(tmp)
			{
				byte_err(0x23, 6, codeword);
				byte_err(0x23, 8, codeword);
				byte_err(0x23, 1, codeword);
				printf("err-> %s\n",codeword);
			}
			tmp = false;			
			// Simulace chyby
			for(int i = 0; i < 15; i++)
			{	
				fprintf(outputFile, "%c",codeword[i]);
			}
			memset(aux,0,sizeof(char) * (9+1));
			byteCnt = 0;
		}
		else
			byteCnt++;		
	}
	// Zbytek v aux je také třeba zakódovat (poslední rámec)
	if(byteCnt != 0)
	{
		encode_data(aux, sizeof(aux)+1, codeword);
		printf("cod-> %s\n",codeword);
		fwrite(codeword,sizeof(char),15,outputFile);
//		for(int i = 0; i < 15; i++)
//		{
//			fprintf(outputFile, "%c",codeword[i]);
//		}
	}
	
	
//###################################################
	/* Shuffle prokládání */
//	int cols = 3;
//	int rows = len/cols;
//
//	for (int i = 1; i <= len; i++)
//	{
//		
//		printf("%c", source[i-1]);
//		if (i != 0 && i%cols == 0)
//			printf("\n");
//	}
//	
//	int i = 0;
//	for (int y = 0; y < cols; y++)
//	{
//		for (int x = 0; x < rows; x++)
//		{
//			/* Write encoded data into output file */
//			aux[i] = source[x*cols+y];
//			if(i == sizeof(aux))
//			{
//				printf("Encode!\n");
//				encode_data(aux, sizeof(char) * (len+1), codeword);
//				for(int i = 0; i < 15; i++)
//				{
//					fprintf(outputFile, "%c",codeword[i]);
//				}
//				memset(aux,0,sizeof(char) * (len+1));
//				i = 0;
//			}
//			else
//				i++;
//		}	
//	}
//	// If aux not clear
//	if(i != 0)
//	{
//		encode_data(aux, sizeof(aux), codeword);
//		for(int i = 0; i < 15; i++)
//		{
//			fprintf(outputFile, "%c",codeword[i]);
//		}
//	}
//###################################################	
	printf("Zapsáno!\n");
	fclose(outputFile);

	
//	int erasures[16];
//	int nerasures = 0;
//
//	/* Initialization the ECC library */
//
//	initialize_ecc ();
//
//	/* ************** */
//
//	/* Encode data into codeword, adding NPAR parity bytes */
//	encode_data(msg, sizeof(msg), codeword);
//
//	printf("Encoded data is: \"%s\"\n", codeword);
//
//  #define ML (sizeof (msg) + NPAR)
//
//
//	/* Add one error and two erasures */
//	byte_err(0x35, 3, codeword);
//
//	byte_err(0x23, 17, codeword);
//	byte_err(0x34, 19, codeword);
//
//
//	printf("with some errors: \"%s\"\n", codeword);
//
//	/* We need to indicate the position of the erasures.  Eraseure
//	   positions are indexed (1 based) from the end of the message... */
//
//	erasures[nerasures++] = ML-17;
//	erasures[nerasures++] = ML-19;
//
//
//	/* Now decode -- encoded codeword size must be passed */
//	decode_data(codeword, ML);
//
//	/* check if syndrome is all zeros */
//	if (check_syndrome () != 0) {
//	  correct_errors_erasures (codeword, 
//				   ML,
//				   nerasures, 
//				   erasures);
//
//	  printf("Corrected codeword: \"%s\"\n", codeword);
//	}

	exit(EXIT_SUCCESS);
}

