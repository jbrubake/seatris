/*
	disp_ncurses.c - Display module for seatris using ncurses
	Copyright 1999 Jonathan McDowell for Project Purple

	06/05/99
*/

#include "autoconf.h"

#ifdef HAVE_NCURSES_H
	#include <ncurses.h>
#else
	#include <curses.h>
#endif
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "disp_ncurses.h"
#include "scoring.h"

#define FIELD_TOP 2
#define NEXT_TOP 15 

int maxx,maxy;
int status_col,xscale,yscale;
int field_left;
int usecolour, dodots=1, dobold=1, dosquiggles=1;

/* Initialise the display */
int disp_init(int colour, int bold, int dots, int squiggles)
{
	char infostr[80];

	initscr();
	usecolour=0;
	dobold=bold;
	dodots=dots;
	dosquiggles=squiggles;
#ifdef COLOUR_CURSES
	if (colour!=1 && (has_colors() || colour==2)) {
		usecolour=1;
		/* Argh. WTF can't everyone spell colour correctly? It's
		   doing my head in. */
		start_color();
		init_pair(1, COLOR_WHITE, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
		init_pair(3, COLOR_GREEN, COLOR_BLACK);
		init_pair(4, COLOR_YELLOW, COLOR_BLACK);
		init_pair(5, COLOR_BLUE, COLOR_BLACK);
		init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
		init_pair(7, COLOR_CYAN, COLOR_BLACK);
	}
#endif

	cbreak();
	noecho();
	curs_set(0);
	clear();
#ifdef FANCY_CURSES
	if (dobold) attrset(A_BOLD);
#endif
	getmaxyx(stdscr, maxy, maxx);

	status_col=maxx-40;

	xscale=(maxx-44)/FIELD_COLS;
	yscale=(maxy-3)/FIELD_ROWS;

	/* Check if screen seems too small */
	if (xscale==0 || yscale==0) return 0;

	xscale=yscale=(xscale>yscale) ? yscale : xscale;

	field_left=(status_col-xscale*FIELD_COLS-2)/2;

	snprintf(infostr, 80, "Seatris v%s by Jonathan McDowell", VERSION);
	mvaddstr(2,status_col,infostr);

	return 1;
}

/* Close the display */
void disp_close(void)
{
	endwin();
}

/* Display the field */
void disp_field(char field[FIELD_COLS][FIELD_ROWS])
{
	int xloop,yloop;
	int xscaleloop,yscaleloop;

	for (yloop=FIELD_TOP; yloop<(FIELD_TOP+FIELD_ROWS*yscale); yloop++) {
#ifdef COLOUR_CURSES
		if (usecolour) {
			mvaddch(yloop,field_left-1,'*' | COLOR_PAIR(5));
			mvaddch(yloop,field_left+FIELD_COLS*xscale,'*' | 
							COLOR_PAIR(5));
		} else {
#endif
			mvaddstr(yloop,field_left-1,"*");
			mvaddstr(yloop,field_left+FIELD_COLS*xscale,"*");
#ifdef COLOUR_CURSES
		}
#endif
	}

	for (xloop=field_left-1; xloop<(field_left+FIELD_COLS*xscale+1); 
								xloop++) {
#ifdef COLOUR_CURSES
		if (usecolour) {
			mvaddch(FIELD_TOP+FIELD_ROWS*yscale, xloop, '*' |
								COLOR_PAIR(5));
		} else {
#endif
			mvaddstr(FIELD_TOP+FIELD_ROWS*yscale, xloop, "*");
#ifdef COLOUR_CURSES
		}
#endif
	}

	/* This little lot indents seriously. Might wanna look at reorganizing
	   it so it all fits in 80 columns. */
	for (yloop=0; yloop<FIELD_ROWS; yloop++)
		for (xloop=0; xloop<FIELD_COLS; xloop++) {
			for (xscaleloop=0; xscaleloop < xscale; xscaleloop++) {
				for (yscaleloop=0; yscaleloop < yscale; yscaleloop++) {
					if (field[xloop][yloop]!=0)
#ifdef COLOUR_CURSES
						if (usecolour) {
							mvaddch(FIELD_TOP+(FIELD_ROWS-yloop-1)*yscale+yscaleloop,xloop*xscale+field_left+xscaleloop, '#' | COLOR_PAIR(field[xloop][yloop]));
						} else {
#endif
							mvaddstr(FIELD_TOP+(FIELD_ROWS-yloop-1)*yscale+yscaleloop,xloop*xscale+field_left+xscaleloop, "#");
#ifdef COLOUR_CURSES
						}
#endif
					else {
						if (dodots && xloop % 2 == 1)
						mvaddstr(FIELD_TOP+(FIELD_ROWS-yloop-1)*yscale+yscaleloop,xloop*xscale+field_left+xscaleloop, ".");
						else
						mvaddstr(FIELD_TOP+(FIELD_ROWS-yloop-1)*yscale+yscaleloop,xloop*xscale+field_left+xscaleloop, " ");
					}
				}
			}
		}

	refresh();
}

/* Display the field with a block overlayed */
void disp_field_with_block(char field[FIELD_COLS][FIELD_ROWS], int block, int rotation, int x, int y)
{
	int xloop, yloop;
	int xscaleloop,yscaleloop;

	disp_field(field);

	for (yloop=0; yloop<4; yloop++)
		for (xloop=0; xloop<4; xloop++)
			if ((blocks[block][rotation][3-yloop][xloop]!=0) &&
			    ((FIELD_ROWS-y-yloop)>0))
				for (xscaleloop=0; xscaleloop < xscale; xscaleloop++) {
					for (yscaleloop=0; yscaleloop < yscale; yscaleloop++) {
#ifdef COLOUR_CURSES
						if (usecolour) {
							mvaddch(FIELD_TOP+(FIELD_ROWS-y-yloop-1)*yscale+yscaleloop,(x+xloop)*xscale+field_left+xscaleloop, '#' | COLOR_PAIR(block+1));
							if (dosquiggles) mvaddch(FIELD_TOP+(FIELD_ROWS*yscale),(x+xloop)*xscale+field_left+xscaleloop, '$' | COLOR_PAIR(block+1));
						} else {
#endif
							mvaddstr(FIELD_TOP+(FIELD_ROWS-y-yloop-1)*yscale+yscaleloop,(x+xloop)*xscale+field_left+xscaleloop, "#");
							if (dosquiggles) mvaddstr(FIELD_TOP+(FIELD_ROWS*yscale),(x+xloop)*xscale+field_left+xscaleloop, "$");
#ifdef COLOUR_CURSES
						}
#endif
					}
				}

	refresh();
}

/* Display the next block */
void disp_next_block(int block, int rotation)
{
	int xloop, yloop, xscaleloop, yscaleloop;

	for (yloop=0; yloop<4; yloop++)
		for (xloop=0; xloop<4; xloop++)
				for (xscaleloop=0; xscaleloop < xscale; xscaleloop++) {
					for (yscaleloop=0; yscaleloop < yscale; yscaleloop++) {
						if (blocks[block][rotation][yloop][xloop]!=0)
#ifdef COLOUR_CURSES
							if (usecolour) {
								mvaddch(NEXT_TOP+yloop*yscale+yscaleloop,xloop*xscale+status_col+xscaleloop, '#' | COLOR_PAIR(block+1));
							} else {
#endif
								mvaddstr(NEXT_TOP+yloop*yscale+yscaleloop,xloop*xscale+status_col+xscaleloop, "#");
#ifdef COLOUR_CURSES
							}
#endif
						else
#ifdef COLOUR_CURSES
							if (usecolour) {
								mvaddch(NEXT_TOP+yloop*yscale+yscaleloop,xloop*xscale+status_col+xscaleloop, ' ' | COLOR_PAIR(block+1));
							} else {
#endif
								mvaddstr(NEXT_TOP+yloop*yscale+yscaleloop,xloop*xscale+status_col+xscaleloop, " ");
#ifdef COLOUR_CURSES
							}
#endif
					}
				}

}

/* Display block statistics */
void disp_blockstats(int blockstats[])
{
	int loop;
	char statstr[40];

	for (loop=0; loop<7; loop++) {
		snprintf(statstr, 40, "%d    ", blockstats[loop]);
#ifdef COLOUR_CURSES
		if (usecolour) {
			mvaddch(NEXT_TOP+loop, status_col+20, '#' | COLOR_PAIR(loop+1));
		} else {
#endif
			mvaddstr(NEXT_TOP+loop, status_col+20, "#");
#ifdef COLOUR_CURSES
		}
#endif

		mvaddstr(NEXT_TOP+loop, status_col+22, statstr);
	}
}

/* Get a character.
   Returns -1 if no character retrieved in usec usecs or char otherwise */

int disp_getch(unsigned int usec)
{
	int flags=0;
	fd_set rfds;
	struct timeval tv;
	int r=-1;

	tv.tv_sec = 0;
	tv.tv_usec = usec;

	FD_ZERO(&rfds);
	FD_SET(0,&rfds);
	flags = select(0+1,&rfds,NULL,NULL,&tv);
	if (flags) r=getch();

/*	if (r==KEY_RESIZE) disp_resize(); */

	return r;
}

/* Display score info etc */

void disp_show_info(int lines, int level, int score)
{
	char infostr[80];

	snprintf(infostr, 80, "Lines: %d    ", lines);
	mvaddstr(4,status_col,infostr);

	snprintf(infostr, 80, "Level: %d    ", level);
	mvaddstr(5,status_col,infostr);

	snprintf(infostr, 80, "Score: %d    ", score);
	mvaddstr(6,status_col,infostr);
}

#ifdef DEBUG
/* Show debug info */
void disp_show_debug(int x, int y, int block, int rotation, int hitbottom)
{
	char debugstr[80];
	snprintf(debugstr, 80, "x=%d, y=%d, block=%d, rotation=%d   ", x, y, block,rotation);
	mvaddstr(12, status_col, debugstr);

	snprintf(debugstr, 80, "hitbottom=%d   ", hitbottom);
	mvaddstr(13, status_col, debugstr);

	refresh();
}
#endif

/* Print paused message and wait for a key */
void disp_do_pause(void)
{
	mvaddstr(FIELD_TOP, field_left, "**PAUSED**");

	while (getch()==12)
		disp_force_redraw();
}

/* Force ncurses to redraw all of the screen. Useful if it gets
   corrupted over a serial link or by a ytalk request. ;) */
void disp_force_redraw(void)
{
	redrawwin(stdscr);
}

/* Resize the display */
void disp_resize(int sig)
{
	char infostr[80];

	curs_set(0);
	clear();
	getmaxyx(stdscr, maxy, maxx);

	status_col=maxx-40;

	xscale=(maxx-44)/FIELD_COLS;
	yscale=(maxy-3)/FIELD_ROWS;

	/* If either xscale or yscale = 0 the screen is too small and we
           should throw up an error. FIXME */

	xscale=yscale= (xscale>yscale) ? yscale : xscale;

	field_left=(status_col-xscale*FIELD_COLS-2)/2;

	snprintf(infostr, 80, "Seatris v%s by Jonathan McDowell", VERSION);
	mvaddstr(2,status_col,infostr);
	refresh();
}

/* Display a Play Again (Y/N)? message and get response. Returns 0 for
   no, 1 for yes. */
int disp_playagain(unsigned int score, int pos)
{
	char tmpstr[80];
	int r,loop;

	if (pos==0) {
		snprintf(tmpstr, 80, " Unable to write high score.  ");
	} else if (pos<=MAXHIGHSCORES) {
		snprintf(tmpstr, 80, "        Score ranks #%-2d       ", pos);
	} else {
		snprintf(tmpstr, 80, " You didn't get a high score. ");
	}

#ifdef COLOUR_CURSES
	if (usecolour) {
		attrset(A_REVERSE | COLOR_PAIR(5));
	} else {
#endif
#ifdef FANCY_CURSES
	attrset(A_REVERSE);
#endif
#ifdef COLOUR_CURSES
	}
#endif
        mvaddstr((maxy/2)-2,(maxx-28)/2, "                              ");
	mvaddstr((maxy/2)-1,(maxx-28)/2, tmpstr);
        mvaddstr((maxy/2),(maxx-28)/2, "       Play Again (Y/N)?      ");
        mvaddstr((maxy/2)+1,(maxx-28)/2, "                              ");
	refresh();
	if (dobold) {
		attrset(A_BOLD);
	} else {
		attrset(A_NORMAL);
	}

	r=1;
	while (strchr("YyNn", r)==NULL || r==ERR) {
		r=getch();
		if (r==12) disp_force_redraw();
	}

	for (loop=(maxy/2)-2; loop<=(maxy/2)+1; loop++)
	        mvaddstr(loop,(maxx-28)/2, "                              ");

	return (r=='Y' || r=='y');
}

/* Display keys */
void disp_show_keys(const char *keys)
{
	char infostr[80];

	snprintf(infostr, 80, "Left: '%c'  Right: '%c'  Rotate: '%c'",
			keys[0], keys[1], keys[2]);
	mvaddstr(8, status_col, infostr);
	snprintf(infostr, 80, "Drop: '%c'  Drop one line: '%c'",
			keys[3], keys[4]);
	mvaddstr(9, status_col, infostr);
	snprintf(infostr, 80, "Inc Lvl: '%c'  Pause: '%c'  Quit: '%c'",
			keys[5], keys[6], keys[7]);
	mvaddstr(10, status_col, infostr);

	refresh();
}
