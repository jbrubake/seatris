/* 
   seatris.h - global headers for seatris
   Copyright 1999 Jonathan McDowell for Project Purple
   06/05/99
*/

#ifndef __SEATRIS_H_
#define __SEATRIS_H_

#define VERSION "0.0.14"

#define CONFIGFILE "/etc/seatrisrc"

#define FIELD_ROWS 20
#define FIELD_COLS 10

/* Max moves before the block drops a line */
#define MAXMOVE 4

/* Only want to define these once, but reference lots of times */
#ifndef NOEXTERNS
extern char blocks[7][4][4][4];
#endif

#endif /* __SEATRIS_H_ */
