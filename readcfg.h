/*
	readcfg.h - Routines to load and parse config files.
	Copyright 1999 Jonathan McDowell for Project Purple

	19/09/1999 - Started writing.
*/

#ifndef __READCFG_H_
#define __READCFG_H_

#include "parse.h"

char *parsecfgopt(char *buf, char *opt);
int readcfg(const char *filename, struct cfginf cfgopts[]);

#endif
