/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   func.h
 * Author: frawless
 *
 * Created on 24. října 2016, 15:24
 */

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
 
/* Lenght of codeword */
#define NLENGTH	15	
/* Codeword */
unsigned char codeword[16];

/* Enumerator for Err codes */
enum code
{
	ERR_OK = 0,
	ERR_INCORECT_INPUT,
	ERR_CANT_OPEN_INPUT,
	ERR_CANT_OPEN_OUTPUT,
	ERR_ELSE
};

/* Function for print error message */
void error_msg(int err_code);
/* Function for test input arguments*/
void check_args(int argc);
/* Function for check input file */
void check_inputFile(FILE *fp);
/* Function for check output file */
void check_outputFile(FILE *fp);
/* Function for concatenate two strings*/
/* http://stackoverflow.com/questions/8465006/how-to-concatenate-2-strings-in-c */
char* concat(const char *s1, const char *s2);
/* Introduce a byte error at LOC */
void byte_err (int err, int loc, unsigned char *dst);
/* Pass in location of error (first byte position is
   labeled starting at 1, not 0), and the codeword. */
void byte_erasure (int loc, unsigned char dst[], int cwsize, int erasures[]);
/* Function for get input file size */
long unsigned int get_file_size(FILE *fp);
/* Function for load bytes from file in buffer */
unsigned char *fillBuffer(FILE *fp);

#ifdef __cplusplus
}
#endif

#endif /* FUNC_H */

