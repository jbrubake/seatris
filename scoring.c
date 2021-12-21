/* 
   Routines to deal with high score files for seatris.
   Copyright 1999 Jonathan McDowell for Project Purple
   21/05/99 

   27/06/99 - Added date field
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "scoring.h"

int get_high_scores(struct highscoreent *highscores[], int *numscores)
{
	FILE *scorefile;
	long filelen;

	if ((scorefile=fopen(highscoref,"r"))==NULL) {
		return 0;
	}

	fseek(scorefile, 0, SEEK_END);
	filelen=ftell(scorefile)+1;
	rewind(scorefile);

	if ((*highscores=malloc(filelen))==NULL) {
		return 0;
	}

	fread(*highscores, 1, filelen, scorefile);
	*numscores=filelen/sizeof(struct highscoreent);
	if (*numscores>MAXHIGHSCORES) *numscores=MAXHIGHSCORES;

	fclose(scorefile);
	return 1;
}

int save_high_scores(struct highscoreent highscores[], int *numscores)
{
	FILE *scorefile;

	if ((scorefile=fopen(highscoref,"w"))==NULL) {
		return 0;
	}

	if (*numscores>MAXHIGHSCORES) *numscores=MAXHIGHSCORES;
	fwrite(highscores, 1, *numscores * sizeof(struct highscoreent), scorefile);

	fclose(scorefile);
	return 1;
}

/* Checks if a score got into the high score table and if so add it to
   the high score file. Returns 0 if unable to do so, MAXHIGHSCORES+1
   if the score didn't get into the high score table, otherwise the
   position in the table. */
int add_high_score(const char name[], unsigned int startlvl,
			 unsigned int endlvl, unsigned int lines,
			unsigned int score)
{
	struct highscoreent highscore;
	struct highscoreent *scores, *newscores;
	int loop, numscores;

	strncpy(highscore.name, name, 15);
	highscore.name[15]=numscores=0;
	highscore.startlvl=startlvl;
	highscore.endlvl=endlvl;
	highscore.lines=lines;
	highscore.score=score;
	highscore.date=time(NULL);

	if (get_high_scores(&scores, &numscores)==0 || numscores==0) {
		numscores=1;
		if (save_high_scores(&highscore, &numscores))
			return 1;
		else
			return 0;
	}

	if (numscores==MAXHIGHSCORES) numscores--;

	if ((newscores=malloc(sizeof(struct highscoreent)*(numscores+1)))==NULL) {
		return 0;
	}

	memcpy(newscores, scores, sizeof(struct highscoreent)*numscores);

	loop=0;
	while (score<(scores[loop]).score && loop<numscores) loop++;

	if (score<(scores[loop]).score) {
		/* didn't get on score table */
		return (MAXHIGHSCORES+1);
	}

	if (loop<numscores) {
		memmove(&(newscores[loop+1]), &(newscores[loop]), sizeof(struct highscoreent)*(numscores-loop));
	}
	memcpy(&(newscores[loop]), &highscore, sizeof(highscore));
	++numscores;
	// SJH
	if (!save_high_scores(newscores, &numscores))
		return 0;

	return (loop+1);
}

void print_high_scores(void)
{
	struct highscoreent *scores;
	int numscores,loop;
	char tempdate[30];

	if (get_high_scores(&scores, &numscores)==1) {
		printf("+------------------+-------+-------+-------+---------+-----------------------+\n");
		printf("| Name             | Start |  End  | Lines |  Score  | Date                  |\n");
		printf("|                  | Level | Level |       |         |                       |\n");
		printf("+------------------+-------+-------+-------+---------+-----------------------+\n");
		for (loop=0; loop<numscores; loop++) {
			strncpy(tempdate, ctime(&scores[loop].date), 29);
			strncpy(tempdate+16,tempdate+19,5);
			tempdate[21]=0;
			printf("| %-16s | %5d | %5d | %5d | %7d | %21s |\n", scores[loop].name,
				scores[loop].startlvl, scores[loop].endlvl,
				scores[loop].lines, scores[loop].score,
				tempdate);
		}
		printf("+------------------+-------+-------+-------+---------+-----------------------+\n");
	} else {
		printf("Couldn't read high score file.\n");
	}
}
