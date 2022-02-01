/************************************************
  *	alines7777 <alines7777@disroot.org>	*
  *	bysc.h					*
  *	BYSC Header				*
  *	BYte Sequence Composer			*
  ************************************************/



/* ** PREPROCESSOR INSTRUCTIONS ******************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYSC_TRUE 0
#define BYSC_FALSE -1
#define BYSC_FILE_ERROR -2	/* NOT SAFE TO IGNORE */
#define BYSC_FILE_NA -3		/* SAFE TO IGNORE */
#define BYSC_ARGUMENT_ERROR -4	/* SAFE TO IGNORE */
#define BYSC_COMMAND_ERROR -5	/* SAFE TO IGNORE */
#define BYSC_MEMORY_ERROR -6	/* NOT SAFE TO IGNORE */
#define BYSC_ANOMALY -7		/* NOT SAFE TO IGNORE */

#define BYSC_PADDING 4		/* pading for line numbers displayed */

#define BYSC_NOCHANGES '*'
#define BYSC_CHANGES '!'

#define BYSC_YES "y"
#define BYSC_NO "n"

/* file modes */

#define READ "r"		/* reading only */
#define READ_WRITE "r+"		/* reading & writing */
#define WRITE "w"		/* overwrite / write new */
#define READ_WRITE_N "w+"	/* create new file if file does not exist */
#define APPEND "a"		/* write only - stream starts @ end of file */
#define READ_APPEND "a+"	/* have i given you a clue? */

/* return procedures for pen & file modes */
#define BYSC_QUIT 0
#define BYSC_FILEMODE 1
#define BYSC_PENMODE 2

/* misc. */
#define BYSC_LINE_LIMIT (int)(((unsigned int)(~0) >> 2))
#define BYSC_SEARCH_LIMIT (int)((unsigned short)(~0))

#define BYSC_NOP (void)0

#define BYSC_COMMAND_QUIT "q"
#define BYSC_COMMAND_CLOSE_QUIT "q!"
#define BYSC_COMMAND_STATUS "s"
#define BYSC_COMMAND_STATUS_CLIP "s$"
#define BYSC_COMMAND_THROUGH ","
#define BYSC_COMMAND_FOREWARD "."
#define BYSC_COMMAND_NUMBER "#" /* treat numbers not as actual numbers, but as strings */
#define BYSC_COMMAND_QUOTE "\"" /* unify arguments within quotes into one string */

#define BYSC_F_COMMAND_READ "r"
#define BYSC_F_COMMAND_WRITE "w"
#define BYSC_F_COMMAND_ISTREAM "i"
#define BYSC_F_COMMAND_OSTREAM "o"
#define BYSC_F_COMMAND_FORMAT "f"
#define BYSC_F_COMMAND_CLOSE "!"
#define BYSC_F_COMMAND_WRITE_CLOSE "w!"

#define BYSC_F_COMMAND_PEN "p"

#define BYSC_P_COMMAND_SEARCH "/"			/* read */
#define BYSC_P_COMMAND_REPLACE "/*"			/* read & write */
#define BYSC_P_COMMAND_ERASE "/!"			/* read & write */
#define BYSC_P_COMMAND_DESTROY "\\"			/* read & write */
#define BYSC_P_COMMAND_WRITE "w"			/* write */
#define BYSC_P_COMMAND_WRITE_START "wi"	/* no args */	/* write */
#define BYSC_P_COMMAND_WRITE_END "wa"	/* no args */	/* write */
#define BYSC_P_COMMAND_STOP "*"		/* no args */	/* write */
#define BYSC_P_COMMAND_APPEND "a"			/* write */
#define BYSC_P_COMMAND_PREPEND "i"			/* write */
#define BYSC_P_COMMAND_REDACT "r"			/* write */

#define BYSC_P_COMMAND_C_TERMINATE "c;"			/* write */
#define BYSC_P_COMMAND_C_COMMENT_BLOCK "c/*"		/* write */
#define BYSC_P_COMMAND_C_COMMENT_LINE "c//"		/* write */
#define BYSC_P_COMMAND_C_CODE_BLOCK_A "c{"		/* wrtie */
#define BYSC_P_COMMAND_C_CODE_BLOCK_B "c}"		/* write */
#define BYSC_P_COMMAND_SHELL_COMMENT "$#"		/* write */
#define BYSC_P_COMMAND_ASM_COMMENT "s;"			/* write */

/************************************************************************
  *	i know c/c++ programmers would really appreciate these		*
  *	commands.							*
  *	these commands will instantly catch the most common but most	*
  *	easily corrected errors. hopefully more quickly and with	*
  *	greater ease than other editors might				*
  ************************************************************************/

#define BYSC_P_COMMAND_PRINT "p"			/* read */
#define BYSC_P_COMMAND_COPY "c"				/* read */
#define BYSC_P_COMMAND_CUT "x"				/* read & write */
#define BYSC_P_COMMAND_PASTE "v"			/* write */
#define BYSC_P_COMMAND_UNDO "z"		/* no args */	/* write */
#define BYSC_P_COMMAND_FORGET "?"	/* no args */	/* none */
#define BYSC_P_COMMAND_DELETE "d"			/* write */
#define BYSC_P_COMMAND_WIPE "!"		/* no args */	/* write */

/************************************************************************
  *	i think the commands below are for modifying the program's		*
  *	clipboard. i don't think they could have any other purpose,		*
  *	because the copy, cut, paste, and undo commands weren't		*
  *	specified for this.						*
  *************************************************************************/

#define BYSC_P_COMMAND_SEARCH_CLIP "/$"
#define BYSC_P_COMMAND_REPLACE_CLIP "/*$"
#define BYSC_P_COMMAND_ERASE_CLIP "/!$"
#define BYSC_P_COMMAND_DESTROY_CLIP "\\$"
#define BYSC_P_COMMAND_WRITE_CLIP "w$"
#define BYSC_P_COMMAND_WS_CLIP "wi$"	/* no args */
#define BYSC_P_COMMAND_WE_CLIP "wa$"	/* no args */
#define BYSC_P_COMMAND_APPEND_CLIP "a$"
#define BYSC_P_COMMAND_PREPEND_CLIP "i$"
#define BYSC_P_COMMAND_REDACT_CLIP "r$"
#define BYSC_P_COMMAND_PRINT_CLIP "p$"
#define BYSC_P_COMMAND_DELETE_CLIP "d$"
#define BYSC_P_COMMAND_WIPE_CLIP "!$"	/* no args */

#define BYSC_P_COMMAND_FILE "f"		/* no args */	/* switch */

/********************************
  *	UNIX-like	"\n"	*
  *	Classic Mac OS	"\r"	*
  *	CP/M, DOS, NT	"\r\n"	*
  *	RISC OS		"\n"	*
  *	BeOS / HAIKU	"\n"	*
  *	AMIGA		"\n"	*
  ********************************/

/* check for OS X */
#if defined(__APPLE__) && defined(__MACH__)
#define NEW_LINE "\n"
#endif

/* check for unix-like systems (linux, bsd, solaris, etc.) */
#if defined(__unix__)
#define NEW_LINE "\n"
#endif

/* check for Plan9 */
#if defined(EPLAN9)
#define NEW_LINE "\n"
#endif

/* check for HAIKU / BeOS */
#if defined(__BEOS__) || defined(__HAIKU__)
#define NEW_LINE "\n"
#endif

/* check for AmigaOS */
#if defined(AMIGA) || defined(__amigaos__)
#define NEW_LINE "\n"
#endif

/* check for Windows NT */
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#define NEW_LINE "\r\n"
#endif

/* check for DOS */
#if defined(__DOS__) || defined(__OS2__)
#define NEW_LINE "\r\n"
#endif

/* check for VMS */
#if defined(VMS) || defined(__VMS)
#define NEW_LINE "\r\n"
#endif

/* check for Classic Mac OS */
#if defined(Macintosh) || defined(macintosh)
#define NEW_LINE "\r"
#endif

#define NEW_LINE_LF "\n"
#define NEW_LINE_CR "\r"
#define NEW_LINE_CRLF "\r\n"

/* file i/o mode switches */
/* read */
#define SWITCH_R "-r"
/* write */
#define SWITCH_W "-w"
/* read & write */
#define SWITCH_RW "-rw"
#define SWITCH_WR "-wr"
/* file output */
#define SWITCH_O "-o"

/* file i/o modes */
#define MODE_DOS "dos"
#define MODE_UNIX "unix"
#define MODE_MAC "mac"

/* other file i/o modes */
#define MODE_OS2 "os2"
#define MODE_VMS "vms"
#define MODE_LINUX "linux"
#define MODE_BSD "bsd"
#define MODE_OSX "osx"
#define MODE_PLAN9 "plan9"
#define MODE_HAIKU "haiku"
#define MODE_BEOS "beos"
#define MODE_AMIGA "amiga"

/* explicit modalities */
#define MODE_LF "lf"
#define MODE_CRLF "crlf"
#define MODE_CR "cr"

/* return values for switches */
#define RETURN_NS 0
#define RETURN_R 1
#define RETURN_W 2
#define RETURN_RW 3
#define RETURN_O 4

/* return values for modes */
#define RETURN_NM 0
#define RETURN_LF 1
#define RETURN_CRLF 2
#define RETURN_CR 3



/* ** TYPE DEFINITIONS **************************************************** */

struct lineN{
	char *text;
	struct lineN *next;
};

typedef struct lineN *lineQ;

struct argN{
	char *argument;
	struct argN *next;
};

typedef struct argN *argQ;

/* TO DO */
/* implement clipbboard queue & undo stack */



/* ** FUNTION PROTOTYPE *************************************************** */

/* NONE */



/* ** FUNCTION DEFINITIONS ************************************************ */

/* NONE */
