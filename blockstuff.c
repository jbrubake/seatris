/*
   blockstuff.c - Main routines for dealing with blocks and the play
                  field
   Copyright 1999 Jonathan McDowell for Project Purple
   06/05/99
*/

#include "blockstuff.h"

/* Check that the given block can fit in the field at the given
   position, which is the lower left corner
   Returns 0 if the block can fit, 1 if there is an existing block in the
   way, and 2 if the left wall is in the way */
   
int check_block(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int x,  int y)
{
	int xloop, yloop;

	if (y<0 || x<0) return 1;

	for (xloop=0; xloop<4; xloop++) {
		for (yloop=0; yloop<4; yloop++) {
/* This logic is ick.  Why not test for blocks[][][][]!=0 at this point? */
			if (yloop+y<FIELD_ROWS) {
				if ((field[x+xloop][y+yloop]!=0) &&
				    (blocks[block][rotation][3-yloop][xloop]!=0))
					return 1;
				else if (((x+xloop)>=FIELD_COLS) &&
				         (blocks[block][rotation][3-yloop][xloop]!=0))
					return 2;
			} else if ((x+xloop)>=FIELD_COLS) {
				if (blocks[block][rotation][3-yloop][xloop]!=0)
					return 2;
			}
		}
	}

	return 0;
}

/* Checks if we can rotate the block and if so returns the new
   rotation, otherwise returns the current one */

int rotate_block(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int *x, int y)
{
	int answer;
	int oldx = *x;
	while ((answer = check_block(field, block, (rotation+1) % 4, *x, y)) == 2) {
		(*x)--;
	}
	if (answer == 1) {
		*x = oldx;
		return rotation;
	}
	else
		return (rotation+1) % 4;
}

/* Checks if the block has hit the bottom so that it can no longer fall
   Returns 1 if we have, 0 otherwise */

int hit_bottom(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int x, int y)
{
	if (y==0)
		return 1;
	else
		return (check_block(field, block, rotation, x, y-1));
}

/* Adds the block to the field. Doesn't do any checking that the block
   isn't overwriting another block */

void add_block_to_field(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int x, int y)
{
	int xloop, yloop;

	for (yloop=0; yloop<4; yloop++) {
		for (xloop=0; xloop<4; xloop++) {
			if (blocks[block][rotation][3-yloop][xloop]!=0 && (y+yloop)<FIELD_ROWS)
				field[x+xloop][y+yloop]=blocks[block][rotation][3-yloop][xloop];
		}
	}
}
