/* 
   Routines to deal with high score files for seatris.
   Copyright 1999 Jonathan McDowell for Project Purple
   21/05/99 
*/

#include <time.h>

/* Default high score file location */
#define HIGHSCOREFILE "/var/lib/games/seatris.score"
/* Max number of high scores we store */
#define MAXHIGHSCORES 20

#ifdef NOEXTERNS
char *highscoref;
#else
extern char *highscoref;
#endif

struct highscoreent {
	char name[16];
	unsigned int startlvl,endlvl,lines,score;
	time_t date;
};

int get_high_scores(struct highscoreent *highscores[], int *numscores);
int save_high_scores(struct highscoreent highscores[], int *numscores);
/* Checks if a score got into the high score table and if so add it to
   the high score file. Returns 0 if unable to do so, MAXHIGHSCORE+1
   if the score didn't get into the high score table, otherwise the
   position in the table. */
int add_high_score(const char name[], unsigned int startlvl,
                         unsigned int endlvl, unsigned int lines,
                        unsigned int score);
void print_high_scores(void);
