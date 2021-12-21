/*
	readcfg.c - Routines to load and parse config files.
	Copyright 1999 Jonathan McDowell for Project Purple

	19/09/1999 - Started writing.
*/

#include <stdio.h>
// #include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parse.h"

char *parsecfgopt(char *buf, char *opt)
{
	char tmp[1024];
	int loop, cpyloop;

	strncpy(tmp, buf, 1023);
	tmp[1023] = 0;
	/* Remove WS at string end */
	for (loop = strlen(tmp) - 1; isspace(tmp[loop]); --loop)
		tmp[loop] = 0;

	/* Remove WS at string start & uppercase */
	for (loop = 0; isspace(tmp[loop]) && tmp[loop] != 0; ++loop);
	for (cpyloop = 0; !isspace(tmp[loop]) && tmp[loop] != 0; ++loop)
		opt[cpyloop++] = toupper(tmp[loop]);
	opt[cpyloop] = 0;

	/* Put params at start of buf */
	for (; isspace(tmp[loop]) && tmp[loop] != 0; ++loop);
	for (cpyloop = 0; tmp[loop] != 0; ++loop)
		buf[cpyloop++] = tmp[loop];
	buf[cpyloop] = 0;

	return buf;
}

int readcfg(const char *filename, struct cfginf cfgopts[])
{
	FILE *fp;
	char buf[1024];
	int invalid=0;

	if ((fp=fopen(filename, "r"))==NULL) {
		return 0;
	}

	buf[1023]=0;
	while (fgets(buf, 1023, fp) != NULL) {
		// parsecfgopt(buf, option);
		if (buf[0] != '#' && buf[0] != 0) {
			if (buf[strlen(buf)-1]=='\n') {
				buf[strlen(buf)-1]=0;
			}
			if (parseline(cfgopts, buf)==-1) {
				printf("Unknown config file option: %s\n", buf);
				++invalid;
			}
		}
	}

	fclose(fp);
	return invalid;
}
