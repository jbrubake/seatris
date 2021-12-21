/*
   blockstuff.h - Main routines for dealing with blocks and the play
                  field
   Copyright 1999 Jonathan McDowell for Project Purple
   06/05/99
*/

#include "seatris.h"

int check_block(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int x,  int y);
/* Checks if we can rotate the block and if so returns the new
   rotation, otherwise returns the current one.  May move the block if it
   is against the left wall. */
int rotate_block(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int *x, int y);
/* Checks if the block has hit the bottom so that it can no longer fall
   Returns 1 if we have, 0 otherwise */
int hit_bottom(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int x, int y);
/* Adds the block to the field. Doesn't do any checking that the block
   isn't overwriting another block */
void add_block_to_field(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int x, int y);
