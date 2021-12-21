/*
   fieldstuff.h - Main routines for dealing with the field
   Copyright 1999 Jonathan McDowell for Project Purple
   06/05/99
*/

#include "seatris.h"

/* Empty the field */
void init_field(char field[FIELD_COLS][FIELD_ROWS]);

/* Delete a line and move all the lines above down 1 */
void delete_line(char field[FIELD_COLS][FIELD_ROWS], int linenum);

/* Check for completed lines and removes them
   Returns the number of completed lines removed */
int clear_lines(char field[FIELD_COLS][FIELD_ROWS]);
