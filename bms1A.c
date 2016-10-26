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
	memset(aux,0,sizeof(char) * (KLENGTH+1));
	/* Pole pro celý soubor */
	long unsigned int newSize = get_new_size(len);
	printf("NewSIze: %ld\n",newSize);
	unsigned char encodedMsg[newSize+1];
	unsigned char shufledEncodedMsg[newSize+1];
	memset(aux,0,sizeof(char) * (NLENGTH+1));
	
	int move = 0;
		
	unsigned int byteCnt = 0;
	for(int x = 0; x < len; x++)
	{
//		printf("byteCnt: %d\n", byteCnt);
		aux[byteCnt] = source[x];
		if(byteCnt == KLENGTH-1)
		{
			//printf("|%.*s|\n", 3, "123456789");
			
//			printf("auxC: |%s|\n",aux);
//			printf("auxLEN: %ld",sizeof(aux));
			encode_data(aux, KLENGTH, codeword);
//			fwrite(codeword,sizeof(char),NLENGTH,outputFile);
			for(int x = 0; x < NLENGTH; x++)
			{
				printf("x:%d:%c",x,codeword[x]);
			}
			printf("|\n");
			printf("codeword: |%s|\n",codeword);
//			shuffle(codeword);
			printf("shufword: |%s|\n",codeword);
			printf("byteCnt+par: %d\n",byteCnt+NPAR);
			for(int x = 0; x < byteCnt+NPAR+1; x++)
			{
				printf("%c",codeword[x]);
			}
			printf("\n");			
//			shuffle(codeword);
//			printf("reshword: |%s|\n",codeword);
			memmove(encodedMsg+move,codeword,NLENGTH);
//
			byteCnt = 0;
			move += NLENGTH;
			
			memset(aux,0,sizeof(char) * (KLENGTH+1));
			memset(codeword,0,sizeof(char) * NLENGTH);			
		}
		else
		{
			byteCnt++;		
		}
	}
	// Zbytek v aux je také třeba zakódovat (poslední rámec)
	if(byteCnt != 0)
	{
		printf("codeword: |%s|\n",aux);
		encode_data(aux, byteCnt, codeword);  //NEKODUHE SE
		//shuffle(codeword);
		for(int x = 0; x < byteCnt+NPAR; x++)
		{
			printf("%c",codeword[x]);
		}		
		memmove(encodedMsg+move,codeword,byteCnt+NPAR);
		
//		fwrite(codeword,sizeof(char),byteCnt+NPAR,outputFile);
	}
	

	/** INTERELAVING testy# */
//	unsigned char L1[] = "abcdefghijklmnopqrstuvwxyz";
//	deinterleaving(interleaving(L1,26),26);
	printf("v prdeli\n");
	interleaving(encodedMsg,shufledEncodedMsg,newSize+1);
	printf("test\n");
	
	fwrite(shufledEncodedMsg,sizeof(char), newSize, outputFile);
	
	printf("\nZapsáno!\n");
	fclose(outputFile);
//	free(aux);
	free(source);	

	exit(EXIT_SUCCESS);
}

