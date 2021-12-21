/*
	parse.h - General string parsing routines.
	Copyright 1999 Jonathan McDowell for Project Purple

	19/09/1999 - Started writing.
*/

#ifndef __PARSE_H_
#define __PARSE_H_

#define CMDLEN 16

struct cfginf {
	char command[CMDLEN];
	int type;	/* 0 : No option.
			   1 : String.
			   2 : Bool (in an int).
			   3 : Int. */
	void *var;	/* Variable to store option in */
};

int parseline(struct cfginf commands[], const char *commandline);
int str2bool(const char *buf);

#endif
