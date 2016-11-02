/************************************************
*	 Projekt: 	Projekt č. 1 do předmětu BMS   * 
*	 Autor:		Jakub Stejskal <xstejs24>	   *
*	 Nazev souboru: 	bms1b				   *
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
	FILE *outputFile = fopen(concat(argv[1],".ok"),"wb");
	checkOutputFile(outputFile);	
		 
	/* Naplnění bufferu vstupním souborem a zjištění velikosti vstupu*/
	source = fillBuffer(fp);
	long unsigned int len = getInputSize(fp);
	fclose(fp);

	/* Reverzace prokládání vstupního souboru */
	unsigned char encodedMsg[len+1];
	deinterleaving(source, encodedMsg, len);
	
	/* Inicializace knihovny RScode */
	initialize_ecc();
	
	/* Začátek zpracování vstupu */
	unsigned char *aux = malloc(sizeof(char) * (NLENGTH+1));
	memset(aux,0,sizeof(char) * (NLENGTH+1));	
	int blocks = 0;	/* Počet zpracovaných bloků */
	int move = 0;	/* Krok pro posun v paměti */
	for(int x = 0; x < len; x++)
	{
		if (x%NLENGTH == 0)
		{
			/* Výpočet délky pro zápis do souboru */
			int writeLen = (len / NLENGTH <= blocks ? ((len-x)%NLENGTH-NPAR) : KLENGTH);
			blocks++;
			memset(aux,0,sizeof(char) * (NLENGTH+1));
			/* Výpočet délky slova pro děkódování */
			int decodeLenght = (writeLen >= KLENGTH) ? NLENGTH : writeLen + NPAR;
			memmove(aux,encodedMsg+move,decodeLenght);
			/* Zvýšení kroku pro posun v paměti */
			move += decodeLenght;
			/* Dekódování vstupních bloků */
			decode_data(aux, decodeLenght);

			/* Kontrola syndromů pro opravu chyb */
			if (check_syndrome () != 0) 
			{
				/* Oprava chyb */
				correct_errors_erasures (aux, decodeLenght, 0, NULL);
			}	
			fwrite(aux,sizeof(char),writeLen, outputFile);
		}
	}
	fclose(outputFile);
	free(aux);
	free(source);
	exit(EXIT_SUCCESS);
}

