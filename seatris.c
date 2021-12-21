/* 
	seatris.c - Main program file for seatris.
	Copyright 1999 Jonathan McDowell for Project Purple

	06/05/99
*/

#include "autoconf.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#ifdef HAVE_GETOPT_H
	#include <getopt.h>
#endif
#include <string.h>

#define NOEXTERNS
#include "seatris.h"
#include "disp_ncurses.h"
#include "block.h"
#include "fieldstuff.h"
#include "blockstuff.h"
#include "scoring.h"
#include "parse.h"
#include "readcfg.h"

char field[FIELD_COLS][FIELD_ROWS];
#define NUM_KEYS 8
char *keys;

int game_loop(int startlvl)
{
	int block, rotation, x, y, lines, level, score, oldlines, quit;
	int newblock, newrotation;
	int maxmove;
	unsigned int speed, curspeed;
	int key,keyidx;
	struct passwd *userinfo;
	int loop, scorepos, curmove;
	int blockstats[7];

	init_field(field);

	y=lines=score=quit=0;
	maxmove=8-(startlvl/2);
	if (maxmove<4) maxmove=4;
	level=startlvl;
	for (loop=0; loop<7; loop++) blockstats[loop]=0;
	loop=0;
	speed=1000000;
	while (loop<startlvl) {
		speed *= 0.85;
		loop++;
	}
	newblock=rand()%7;
	newrotation=rand()%4;

	/* Main game loop starts here */

	while (y!=FIELD_ROWS && !quit) {

		/* New block. */
		block=newblock;
		rotation=newrotation;
		newblock=rand()%7;
		newrotation=rand()%4;
		blockstats[block]++;
		y=FIELD_ROWS;
		x=FIELD_COLS/2;

		disp_field(field);
		disp_next_block(newblock,newrotation);
		disp_show_info(lines, level, score);
		disp_blockstats(blockstats);

		/* Block loop */
		while (!hit_bottom(field,block,rotation,x,y) && !quit) {
#ifdef DEBUG
			disp_show_debug(x, y, block, rotation, hit_bottom(field, block, rotation, x, y));
#endif
			
			y--;
			curspeed=speed;
			curmove=0;
			while (curspeed>0 && curmove<maxmove && !quit) {
			disp_field_with_block(field, block, rotation, x, y);
			key=-1;
			while (key==-1 && curmove<maxmove && curspeed>0) {
				key=disp_getch(curspeed/maxmove);
				curspeed -= speed/maxmove;
				curmove++;
			}

			if (key==12) disp_force_redraw();
			for (keyidx=0; keyidx<NUM_KEYS && keys[keyidx]!=key; ++keyidx) ;
			switch (keyidx) {
				case 0: /* Left */
				if (!check_block(field, block, rotation, x-1, y)) x--;
				break;
				case 1: /* Right */
				if (!check_block(field, block, rotation, x+1, y)) x++;
				break;
				case 2: /* Rotate */
				rotation=rotate_block(field, block, rotation, &x, y);
				break;
				case 3: /* Drop */
				while(!hit_bottom(field,block,rotation,x,y)) {
					y--;
					disp_field_with_block(field, block, rotation, x, y);
				}
				break;
				case 4: /* Down */
					curspeed=0;
					break;
				case 5: /* Increase level */
					level++;
					speed *= 0.85;
					curspeed *= 0.85;
					maxmove=8-(level/2);
					if (maxmove<4) maxmove=4;
					disp_show_info(lines, level, score);
					break;
				case 6: /* Pause */
					disp_do_pause();
					break;
				case 7: /* Quit */
					quit=1;
					break;
				default:
					break;
			}
			}
		}

		if (!quit) {
			add_block_to_field(field, block, rotation, x, y);
			oldlines = lines;
			lines += clear_lines(field);
			if (level < lines/10) {
				speed *= 0.85;
				level=lines/10;
				maxmove=8-(level/2);
				if (maxmove<4) maxmove=4;
			}
			score += (FIELD_ROWS-y)*(level+1);
			if ((lines-oldlines)>0) score+=((lines-oldlines)*(lines-oldlines)*(lines-oldlines))*10*(level+1);
		}

#ifdef DEBUG
		disp_show_debug(x, y, block, rotation, hit_bottom(field, block, rotation, x, y));
#endif
	}
	
#ifdef DEBUG
	disp_show_debug(x, y, block, rotation, hit_bottom(field, block, rotation, x, y));
#endif
	userinfo=getpwuid(getuid());
	if (userinfo!=NULL) {
		scorepos=add_high_score(userinfo->pw_name, startlvl, level, lines, score);
	} else {
		scorepos=add_high_score("UNKNOWN", startlvl, level, lines, score);
	}

	return disp_playagain(score, scorepos);
}

void display_help()
{
	/* I want a print <<EOF :-) huggie@earth.li */
	printf("seatris v%s by Jonathan McDowell\n\n", VERSION);
	
	puts("-h            : Print out this help.");
	puts("-s            : Print out the high score table.");
	puts("-l x          : Start at level x. (max 20)");
	puts("-k \"abcdefgh\" : Use key a as left");
	puts("                        b as right");
	puts("                        c as rotate");
	puts("                        d as drop");
	puts("                        e as drop by one line");
	puts("                        f as increase level");
	puts("                        g as pause");
	puts("                        h as quit.");
	puts("                Default is \"jlk m+pq\"");
	puts("-f <filename> : Use <filename> as an alternative high score file. Does");
	puts("                not work if seatris is SUID or SGID.");
	puts("-b            : Force seatris not to use colour, even if ncurses reports");
	puts("                that the display is capable of it.");
	puts("-c            : Force seatris to use colour, even if ncurses reports that");
	puts("                the display isn't capable of it. May not work.");
}

int main(int argc, char *argv[])
{
	int option,startlvl,doscores,colour,die;
	int bold=1, dots=1, squiggles=1;
	char *tmpfile=NULL, rcfile[1024];
	struct cfginf configf[]={{"KEYS", 1, &keys},
					{"MONO", 2, NULL},
					{"SCOREFILE", 1, &highscoref},
					{"BOLD", 2, NULL},
					{"DOTS", 2, NULL},
					{"SQUIGGLES", 2, NULL},
					{"LEVEL", 3, NULL}};

	configf[1].var=&colour;
	configf[3].var=&bold;
	configf[4].var=&dots;
	configf[5].var=&squiggles;
	configf[6].var=&startlvl;
	srand(time(NULL));
	startlvl=option=doscores=colour=die=0;
	keys=highscoref=NULL;
	readcfg(CONFIGFILE, configf);
	if (highscoref==NULL) highscoref=strdup(HIGHSCOREFILE);

	configf[2].var=&tmpfile;
	rcfile[1023]=0;
	strncpy(rcfile, getenv("HOME"), 1023);
	if (strlen(rcfile)>1013) {
		puts("$HOME is too large!");
		exit(1);
	}
	strncat(rcfile, "/.seatrisrc", 1023-strlen(rcfile));
	readcfg(rcfile, configf);
	if (tmpfile!=NULL) {
		if (getuid()!=geteuid()) {
			printf("Seatris appears to be SUID - unable to accept SCOREFILE option in user rc file.\n");
			exit(1);
		}
		if (getgid()!=getegid()) {
			printf("Seatris appears to be SGID - unable to accept SCOREFILE option in user rc file.\n");
			exit(1);
		}
		free(highscoref);
		highscoref=tmpfile;
	} 

	while (option!=-1) {
		option=getopt(argc, argv, "bcf:hk:l:s");

		switch (option) {
		case '?':	die=1;
				break;
		case 'b':	colour=1;
				break;
		case 'c':	colour=2;
				break;
		case 'f':	if (strlen(optarg)>254) {
					printf("High score filename too long. Max 254 chars.\n");
					exit(1);
				}
				if (getuid()!=geteuid()) {
					printf("Seatris appears to be SUID - unable to accept -f option.\n");
					exit(1);
				}
				if (getgid()!=getegid()) {
					printf("Seatris appears to be SGID - unable to accept -f option.\n");
					exit(1);
				}
				highscoref=strdup(optarg);
				if (highscoref[strlen(highscoref) - 1] == '\n')
					highscoref[strlen(highscoref) - 1] = '\0';
				break;
		case 'h':	die=1;
				break;
		case 'k':	if (strlen(optarg)!=NUM_KEYS) {
					printf("Error! Must have %d chars in keydef string.\n", NUM_KEYS);
					exit(1);
				}
				keys=strdup(optarg);
				break;
		case 'l':	startlvl=atoi(optarg);
				if (startlvl>20) {
					printf("Hmmmm. A start level of %d? I think not.\n", startlvl);
					exit(1);
				}
				break;
		case 's':	doscores=1;
				break;
		default:	break;
		}
	}

	/* Give nice errors (not usumped by curses) huggie@earth.li */
	if ((optind<argc) || die) {
		if (optind<argc) {
			printf("Unrecognised command line parameters: ");
			while (optind < argc)
				printf("%s ", argv[optind++]);
			puts(" ");
		}
		display_help();
		exit(1);
	}

	if (doscores) {
		print_high_scores();
		exit(0);
	}

	if (keys==NULL) keys=strdup("jlk m+pq");
	if (!disp_init(colour, bold, dots, squiggles)) {
			printf("Error initializing screen - perhaps it is too small?\n");
			exit(1);
	}
	disp_show_keys(keys);
	while (game_loop(startlvl)) ;
	disp_close();
	return 0;
}
