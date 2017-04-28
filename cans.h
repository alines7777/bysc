/****************************************
  *	cenu <cenu00@yahoo.com>		*
  *	cans.h				*
  *	CANS Header			*
  *	BYte Sequence Composer		*
  ****************************************/



/* PREPROCESSOR INSTRUCTIONS ********************************************** */

#include <stdio.h>
#include <string.h>

/* for no settings.... */

#define NAUGHT ""

/* text formatting */
#define DEF	"\x1b[0m"	/* default */
#define BRT	"\x1b[1m"	/* bright */
#define UNDL	"\x1b[4m"	/* underline */
#define FLASH	"\x1b[5m"	/* flashing */
#define REV	"\x1b[5m"	/* reverse video */
#define HIDE	"\x1b[8m"	/* hide text */

/* text color */
#define BLK	"\x1b[30m"	/* black */
#define RED	"\x1b[31m"	/* red */
#define GRN	"\x1b[32m"	/* green */
#define YLW	"\x1b[33m"	/* yellow */
#define BLU	"\x1b[34m"	/* blue */
#define MAG	"\x1b[35m"	/* magenta */
#define CYN	"\x1b[36m"	/* cyan */
#define WHT	"\x1b[37m"	/* white */

/* background color */
#define XBLK	"\x1b[40m"
#define XRED	"\x1b[41m"
#define XGRN	"\x1b[42m"
#define XYLW	"\x1b[43m"
#define XBLU	"\x1b[44m"
#define XMAG	"\x1b[45m"
#define XCYN	"\x1b[46m"
#define XWHT	"\x1b[47m"



/* ** FUNCTION PROTOTYPES ************************************************* */

void sprint(const char *format,const char *color,const char *xcolor,const char *string);



/* ** FUNCTION DEFINITIONS ************************************************ */

void sprint(const char *format,const char *color,const char *xcolor,const char *string){
	/* formatting */
	printf("%s%s%s%s%s\n",format,color,xcolor,string,DEF);

	return;
}
