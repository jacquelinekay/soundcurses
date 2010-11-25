/**************************************************************************
 * binfile.h - Convenience functions for writing binary values to a file. 
 */

#ifndef _BINFILE_H_
#define _BINFILE_H_

#include <stdio.h>

/* Write a 1, 2, or 4-byte value to a file.  You can pass signed types
   into these functions, they will get correctly represented in the
   file. */
void bin_write_1(FILE* fp, unsigned char d);
void bin_write_2(FILE* fp, unsigned short d);
void bin_write_4(FILE* fp, unsigned int d);

/* Read a 1, 2, or 4-byte value from a file.  You can pass signed
   types into these functions, they will get correctly represented in
   the file (but they may generate a compiler warning). */
void bin_read_1(FILE* fp, unsigned char* d);
void bin_read_2(FILE* fp, unsigned short* d);
void bin_read_4(FILE* fp, unsigned int* d);

/* Write the null-terminated string to the file byte-by-byte. */
void bin_write_tag(FILE* fp, const char* tag);

/* Generate an error if the null-terminated string does not
   match what is read from the file. */
void bin_match_tag(FILE* fp, const char* tag);

#endif
