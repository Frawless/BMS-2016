/************************************************
*	 Projekt: 	Projekt č. 1 do předmětu BMS   * 
*	 Autor:		Jakub Stejskal <xstejs24>	   *
*	 Nazev souboru: 	bms1A				   *
************************************************/
 
#include "func.h"

/**
 * Main
 * @param argc počet vstupních argumentů
 * @param argv pole vstupních argumentů
 * @return návratová hodnota programu
 */
int main (int argc, char *argv[])
{
	/* Buffer pro vstup */
	unsigned char *source = NULL;
	/* Kontrola argumentů */
	checkArgs(argc);
	
	/* Kontrola vstupního souboru a jeho otevření */
	FILE *fp = fopen (argv[1], "r");
	checkInputFile(fp);
	
	/* Kontrola existence výstupního souboru a jeho otevření */
	FILE *outputFile = fopen(concat(argv[1],".out"),"wb");
	checkOutputFile(outputFile);	
		 
	/* Naplnění bufferu vstupním souborem a zjištění velikosti vstupu*/
	source = fillBuffer(fp);
	long unsigned int len = getInputSize(fp);
	fclose(fp);
	
	/* Inicializace knihovny RScode */
	initialize_ecc();

	/* Začátek zpracování vstupu */
	unsigned char *aux = malloc(sizeof(char) * (len+1));
	/* Alokace potřebného místa na základě velikosti souboru a délce kódového slova */
	if(len < KLENGTH)		
		memset(aux,0,sizeof(char) * (len+1));
	else
		memset(aux,0,sizeof(char) * (KLENGTH+1));
	
	long unsigned int newSize = getOutputSize(len);
	/* Vytvoření polí pro zakódovaný a proložený vstup */
	unsigned char encodedMsg[newSize+1];
	unsigned char shufledEncodedMsg[newSize+1];
	//memset(aux,0,sizeof(char) * (NLENGTH+1));
	
	int move = 0;	/* Krok pro posun v paměti */
	unsigned int byteCnt = 0;	/* Čítač projdutých bajtů */
	for(int x = 0; x < len; x++)
	{
		aux[byteCnt] = source[x];
		if(byteCnt == KLENGTH-1)
		{
			encode_data(aux, KLENGTH, codeword);
//			shuffle(codeword);
			memmove(encodedMsg+move,codeword,NLENGTH);
			/* Vynulování čítače bajtů a zvýšení kroku */
			byteCnt = 0;
			move += NLENGTH;
			/* Vymazání pomocných polí */
			memset(aux,0,sizeof(char) * (KLENGTH+1));
			memset(codeword,0,sizeof(char) * NLENGTH);			
		}
		else
			byteCnt++;		
	}
	if(byteCnt != 0)
	{
		/* Zakódování konce souboru v případě, že velikost vstupu není násobek velikosti kódového slova */
		encode_data(aux, byteCnt, codeword);
		memmove(encodedMsg+move,codeword,byteCnt+NPAR);
	}

	/* Prokládání a zápis do souboru*/
	interleaving(encodedMsg,shufledEncodedMsg,newSize);
	fwrite(shufledEncodedMsg,sizeof(char), newSize, outputFile);
	
	fclose(outputFile);
	free(aux);
	free(source);	

	exit(EXIT_SUCCESS);
}

