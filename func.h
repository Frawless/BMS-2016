/************************************************
*	 Projekt: 	Projekt č. 1 do předmětu BMS   * 
*	 Autor:		Jakub Stejskal <xstejs24>	   *
*	 Nazev souboru: 	func.h				   *
************************************************/

#ifndef FUNC_H
#define FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lib/ecc.h"
 
/* Potřebné délky */
#define NLENGTH	(KLENGTH+NPAR)		// Délka zakódovaného slova (kódové slovo + parita))
#define KLENGTH 137					// Délka kódového slova
//#define ROW		15
//#define COL		(888/9)
/* Codeword */
unsigned char codeword[NLENGTH+1];

/* Enumerátor pro chybové stavy */
enum code
{
	ERR_OK = 0,
	ERR_INCORECT_INPUT,
	ERR_CANT_OPEN_INPUT,
	ERR_CANT_OPEN_OUTPUT,
	ERR_ELSE
};

/* Funkce pro vypsání chybového hlášení */
void errorMsg(int err_code);
/* Funkce pro test vstupních argumentů */
void checkArgs(int argc);
/* Funkce pro test existence vstupního souboru */
void checkInputFile(FILE *fp);
/* Funkce pro test existence výstupního souboru */
void checkOutputFile(FILE *fp);
/* Funkce pro konkatenaci dvou řetězců - převzato:  http://stackoverflow.com/questions/8465006/how-to-concatenate-2-strings-in-c */
char* concat(const char *s1, const char *s2);
/* Funkce pro získání velikosti vstupního souboru */
long unsigned int getInputSize(FILE *fp);
/* Funkce pro naplnění bufferu vstupem - převzato:  http://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c */
unsigned char *fillBuffer(FILE *fp);
/* Funkce pro zjištění velikosti souboru s redundancí */
long unsigned int getOutputSize(int size);
/* Funkce pro prokládání kódových slov */
void interleaving(unsigned char L1[], unsigned char L2[],long int size);
/* Funkce pro zvrácení prokládání kódových slov */
void deinterleaving(unsigned char L1[], unsigned char L2[],long int size);
/* Funkce pro experimentování s prokládáním uvnitř matice */
void shuffle(unsigned char codeword[]);

/* Funkce pro opravu chyb -  Knihovna RScode */
/* Introduce a byte error at LOC */
void byte_err (int err, int loc, unsigned char *dst);
/* Pass in location of error (first byte position is
   labeled starting at 1, not 0), and the codeword. */
void byte_erasure (int loc, unsigned char dst[], int cwsize, int erasures[]);






#ifdef __cplusplus
}
#endif

#endif /* FUNC_H */

