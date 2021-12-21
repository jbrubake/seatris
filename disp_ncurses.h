/*
   disp_ncurses.h - Display module for seatris using ncurses
   Copyright 1999 Jonathan McDowell for Project Purple
   06/05/99
*/

#include "seatris.h"

/* Initialise the display */
int disp_init(int colour, int bold, int dots, int squiggles);
/* Close the display */
void disp_close(void);
/* Display the field */
void disp_field(char field[FIELD_COLS][FIELD_ROWS]);
/* Display the field with a block overlayed */
void disp_field_with_block(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int x, int y);
/* Display the next block */
void disp_next_block(int block, int rotation);
/* Display block statistics */
void disp_blockstats(int blockstats[]);
/* Get a char from the keyboard */
int disp_getch(unsigned int usec);
/* Display score info etc */
void disp_show_info(int lines, int level, int score);
#ifdef DEBUG
/* Show debug info */
void disp_show_debug(int x, int y, int block, int rotation, int hitbottom);
#endif
/* Print paused message and wait for a key */
void disp_do_pause(void);
/* Force ncurses to redraw all of the screen. Useful if it gets
   corrupted over a serial link or by a ytalk request. ;) */
void disp_force_redraw(void);
/* Resize the display */
void disp_resize(int sig);
/* Display a Play Again (Y/N)? message and get response. Returns 0 for
   no, 1 for yes. */
int disp_playagain(unsigned int score, int pos);
/* Display keys */
void disp_show_keys(const char *keys);
