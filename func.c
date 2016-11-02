/************************************************
*	 Projekt: 	Projekt č. 1 do předmětu BMS   * 
*	 Autor:		Jakub Stejskal <xstejs24>	   *
*	 Nazev souboru: 	func.h				   *
************************************************/

#include "func.h"

/**
 *  Funkce pro vypsání chybového hlášení
 * @param err_code číslo chybového stavu
 */
void errorMsg(int err_code)
{
	if(err_code == ERR_INCORECT_INPUT)
		fprintf(stderr, "Incorect input!\n");
	else if (err_code == ERR_CANT_OPEN_INPUT)
		fprintf(stderr, "Can't open input file!\n");
	else if (err_code == ERR_CANT_OPEN_OUTPUT)
		fprintf(stderr, "Can't create output file!\n");	
}

/**
 * Funkce pro test vstupních argumentů
 * @param argc počet argumentů
 */
void checkArgs(int argc)
{
	if(argc != 2)
	{
		errorMsg(ERR_INCORECT_INPUT);
		exit(EXIT_FAILURE);
	}
}

/**
 * Funkce pro test existence vstupního souboru
 * @param fp vstupní soubor
 */
void checkInputFile(FILE *fp)
{
	if(fp == NULL)
	{
		errorMsg(ERR_CANT_OPEN_INPUT);
		exit(EXIT_FAILURE);
	}	
}

/**
 * Funkce pro test existence výstupního souboru
 * @param fp výstupní soubor
 */
void checkOutputFile(FILE *fp)
{
	if(fp == NULL)
	{
		errorMsg(ERR_CANT_OPEN_OUTPUT);
		exit(EXIT_FAILURE);
	}	
}

/**
 * Funkce pro konkatenaci dvou stringů
 * Zdroj: http://stackoverflow.com/questions/8465006/how-to-concatenate-2-strings-in-c
 * @param s1 první řetězec
 * @param s2 druhý řetězec
 * @return výsledný řetězec
 */
char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}


/**
 * Funkce pro získání velikosti vstupního souboru
 * @param fp ukazatel na vstupní soubor
 * @return velikost načteného souboru
 */
long unsigned int getInputSize(FILE *fp)
{
	/* Posun na konec souboru */
	if (fseek(fp, 0L, SEEK_END) == 0) {
		/* Získání velikosti souboru */
		return ftell(fp);
	}
	else
		return 0;
}

/**
 * Funkce pro naplnění bufferu vstupem
 * Zdroj: http://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c
 * @param fp ukazatel na vstupní soubor
 * @return naplněný buffer
 */
unsigned char *fillBuffer(FILE *fp)
{
	/* Test pro existenci vstupního souboru */
	if(fp == NULL)
	{
		errorMsg(ERR_CANT_OPEN_INPUT);
		exit(EXIT_FAILURE);
	}
	else
	{
		unsigned char *source = NULL;
		/* Načítání souboru do bufferu */
		/* Posun na konec souboru */
		if (fseek(fp, 0L, SEEK_END) == 0) {
			/* Get the size of the file. */
			long bufsize = ftell(fp);
			if (bufsize == -1) { /* Error */ }

			/* Allokace bufferu */
			source = malloc(sizeof(char) * (bufsize + 1));

			/* Posun na začátek souboru */
			if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

			/* Načtení vstupního souboru do paměti */
			size_t newLen = fread(source, sizeof(char), bufsize, fp);
			if ( ferror( fp ) != 0 ) {
				fputs("Chyba při čtení souboru!", stderr);
			} else {
				source[newLen++] = '\0';
			}
		}
		return source;
	}
}

/**
 * Funkce pro zjištění velikosti souboru s redundancí
 * @param size velikost původního souboru
 * @return velikost souboru s redundancí
 */
long unsigned int getOutputSize(int size)
{
	int tmp = size%KLENGTH;
	int aux = size/KLENGTH;
	if(aux == 0)
		return (tmp+NPAR);
	else
		return(tmp > 0 ? (((size/KLENGTH)*NLENGTH)+tmp+NPAR) : ((size/KLENGTH)*NLENGTH));
}

/**
 * Funkce pro experimentování s prokládáním uvnitř kódového slova
 * @param codeword kódové slovo
 */
void shuffle(unsigned char codeword[])
{
	for(int x = 0, y = NLENGTH-1; x < KLENGTH; x+=2,y--)
	{
		/* Prohození znaků v kódovém slově */
		char tmp;
		tmp = codeword[x];
		codeword[x] = codeword[y];
		codeword[y] = tmp;
	}
}

/**
 * Funkce pro prokládání kódových slov
 * @param L1 pole obsahující neproložený vstupní soubor
 * @param L2 pole obsahující proložený vstupní soubor
 * @param size velikost souboru s redundancí
 */
void interleaving(unsigned char L1[], unsigned char L2[],long int size)
{
	/* Definice velikosti matice pro prokládání */
	#define ROW (249*2)
	#define COL (size/249/2)

	int z = 0;
	/* Plnění matic, počet je závislý na velikosti souboru, poslední rámec řeše individuálně */
	for(int k = 0; k < size/(ROW*COL); k++)
	{	
		/* Plnění jednotlivých matic proloženými daty, využity dvě 1D pole */
		for(int i = 0; i < ROW*COL; i++, z++)
		{
			L2[z] = L1[(z%COL)*ROW+(z/COL)+ (ROW*k)*(COL-1)];	
		}			
	}
	/* Zpracování posledního rámce souboru, navržená velikost matice je velká pro poslední rámec
	 * k poslednímu rámci jsou tedy připojeny již proložené bajty z předchozí matice, ty jsou promíchány
	 * a následně proběhně proložení posledního rámce s přidanými bity do navržené matice */
	if(size%(ROW*COL) != 0)
	{
		if (size > (ROW*COL))
		{
			/* Výpočet posunu pro naplnění výsledné matice */
			int offset = size-ROW*COL;
			/* Nakopírování potřebných bajtů pro doplnění matice pro proložení posledního rámce */
			for(int i = 0, k = offset; i < ROW*COL-size%(ROW*COL); i++, k++)
				L1[k] = L2[k];
			
			//################################################
			// Prohození pořádí bajtů v poslední matici
			for(int q = 0; q < ROW*COL; q++)
			{
				char tmp = L1[q+offset];
				L1[q+offset] = L1[size-q-1];
				L1[size-q-1] = tmp;
				
			}
			//################################################
			/* Proložení vzniklé matice */
			for(int k = 0; k < size/(ROW*COL); k++)
			{	
				for(int z = 0; z < ROW*COL; z++)
					L2[z+offset] = L1[offset+(z%COL)*ROW+(z/COL)];
			}
		}
		/* Případ, kdy velikost souboru je menší než velikost matice, nevyužito prokládání (nestává se, spíše pro testování) */
		else
		{
			for(int i = 0; i < size; i++)
				L2[i] = L1[i];
		}		
	}
}

/**
 * Funkce pro zvrácení prokládání kódových slov
 * @param L1 pole obsahující proložený vstupní soubor
 * @param L2 pole obsahující neproložený vstupní soubor
 * @param size velikost souboru s redundancí
 */
void deinterleaving(unsigned char L1[], unsigned char L2[],long int size)
{
	/* Definice velikosti matice pro prokládání */
	#define ROW (249*2)
	#define COL (size/249/2)
	
	/* Nejprve je zpracován poslední rámec souboru (nutno postupovat opačně než v případě prokládání) */
	if(size%(ROW*COL) != 0)
	{
		if (size > (ROW*COL))
		{
			int offset = size-ROW*COL;
			//################################################
			// Prohození pořádí bajtů v poslední matici
			for(int q = 0; q < ROW*COL; q++)
			{
				char tmp = L1[q+offset];
				L1[q+offset] = L1[size-q-1];
				L1[size-q-1] = tmp;
				
			}
			//################################################	
			/* Proložení proložené matice (reverzace proložení pomocí prohození řádků a sloupců) */
			for(int k = 0; k < size/(ROW*COL); k++)
			{	
				for(int z = 0; z < ROW*COL; z++)
					L2[z+offset] = L1[offset+(z%ROW)*COL+(z/ROW)];
			}
			for(int i = 0, k = offset; i < ROW*COL-size%(ROW*COL); i++, k++)
				L1[k] = L2[k];
		}
		/* Případ, kdy velikost souboru je menší než velikost matice, nevyužito prokládání (nestává se, spíše pro testování) */
		else
		{
			for(int i = 0; i < size; i++)
				L2[i] = L1[i];
		}		
	}	
	
	int z = 0;
	/* Zvrácení prokládání vstupu, stejný princip jako při prokládání pouze prohozené řádky a sloupce */ 
	for(int k = 0; k < size/(ROW*COL); k++)
	{	
		for(int i = 0; i < ROW*COL; i++, z++)
			L2[z] = L1[(z%ROW)*COL+(z/ROW)+ (COL*k)*(ROW-1)];	
	}
}

/* Funkce z původní knihovny RScode */
/* Introduce a byte error at LOC */
void
byte_err (int err, int loc, unsigned char *dst)
{
  printf("Adding Error at loc %d, data %#x\n", loc, dst[loc-1]);
  dst[loc-1] ^= err;
}

/* Pass in location of error (first byte position is
   labeled starting at 1, not 0), and the codeword.
*/
void
byte_erasure (int loc, unsigned char dst[], int cwsize, int erasures[]) 
{
  printf("Erasure at loc %d, data %#x\n", loc, dst[loc-1]);
  dst[loc-1] = 0;
}