/*
   fieldstuff.c - Main routines for dealing with the field
   Copyright 1999 Jonathan McDowell for Project Purple
   06/05/99
*/

#include "fieldstuff.h"

/* Empty the field */
void init_field(char field[FIELD_COLS][FIELD_ROWS])
{
	int xloop,yloop;

	for (yloop=0; yloop<FIELD_ROWS; yloop++)
		for (xloop=0; xloop<FIELD_COLS; xloop++)
			field[xloop][yloop]=0;
}

/* Delete a line and move all the lines above down 1 */

void delete_line(char field[FIELD_COLS][FIELD_ROWS], int linenum)
{
	int xloop, yloop, zerorow;

	zerorow=0;

	for (yloop=linenum; yloop<FIELD_ROWS && !zerorow; yloop++) {
		zerorow=1;
		for (xloop=0; xloop<FIELD_COLS; xloop++) {
			if (yloop==FIELD_ROWS-1)
				field[xloop][yloop]=0;
			else {
				field[xloop][yloop]=field[xloop][yloop+1];
				if (field[xloop][yloop]!=0) zerorow=0;
			}
		}
	}
}

/* Check for completed lines and removes them
   Returns the number of completed lines removed */

int clear_lines(char field[FIELD_COLS][FIELD_ROWS])
{
	int xloop, yloop, completed, count;
	
	count=0;

	for (yloop=FIELD_ROWS-1; yloop>=0; yloop--) {
		completed=1;
		for (xloop=0; xloop<FIELD_COLS && completed; xloop++)
			if (field[xloop][yloop]==0) completed=0;
		if (completed) {
			delete_line(field, yloop);
			++count;
		}
	}

	return count;
}
