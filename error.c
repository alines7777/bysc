/****************************************
  *	cenu <cenu00@yahoo.com>		*
  *	error.c				*
  *	BYSC Error Source		*
  *	BYte Sequence Composer		*
  ****************************************/

/* ** PREPROCESSOR INSTRUCTIONS ******************************************* */

#include <stdio.h>
#include <stdlib.h>
#include "bysc.h"



/* ** FUNCTION PROTOTYPES ************************************************* */

int f_read_error(const char *filename);

int m_alloc_error(lineQ linebuffer);

int m_dealloc_error(void);

int arg_mode_error(const char *argument);

int arg_out_error(const char *argument);

int command_error(const char *argument);

extern int clear_linebuffer(lineQ lines);

extern int clear_command(argQ args);



/* ** FUNCTION DEFINITIONS ************************************************ */

int f_read_error(const char *filename){
	fprintf(stderr,"FILE :: %s :: READ ERROR !!!!\n",filename);
	fprintf(stderr,"FILE DOES NOT EXIST & CANNOT BE CREATED\n");

	return BYSC_FILE_ERROR;
}

int m_l_alloc_error(lineQ pointer){
	fprintf(stderr,"MEMORY :: ALLOCATION ERROR !!!!\n");
	fprintf(stderr,"ATTEMPTING TO DEALLOCATE ALLOCATED MEMORY\n");

	clear_linebuffer(pointer);

	fprintf(stderr,"memory successfully cleared after runtime error\n");
	fprintf(stderr,"closing program\n");

	return BYSC_MEMORY_ERROR;
}

int m_a_alloc_error(argQ pointer){
	fprintf(stderr,"MEMORY :: ALLOCATION ERROR !!!!\n");
	fprintf(stderr,"ATTEMPTING TO DEALLOCATE ALLOCATED MEMORY\n");

	clear_command(pointer);

	fprintf(stderr,"memory successfully cleared after runtime error\n");
	fprintf(stderr,"closing program\n");

	return BYSC_MEMORY_ERROR;
}

int m_dealloc_error(void){
	fprintf(stderr,"MEMORY :: MEMORY DEALLOCATION ERROR !!!!\n");
	fprintf(stderr,"!!!! PANIC !!!!\n QUITTING IMMEDIATELY\n");

	return BYSC_MEMORY_ERROR;
}

int arg_mode_error(const char *argument){
	fprintf(stderr,"ARGUMENT :: %s :: NO VALID MODE FOR FORMAT SWITCH\n",argument);
	fprintf(stderr,"\t PLEASE REFER TO THE\'ARGUMENTS\' SECTION OF THIS PROGRAM\'S DOCUMENTATION\n");

	return BYSC_ARGUMENT_ERROR;
}

int arg_out_error(const char *argument){
	fprintf(stderr,"ARGUMENT :: %s :: NO FILE NAME GIVEN\n",argument);
	fprintf(stderr,"\tPLEASE REFER TO THE \'ARGUMENTS\' SECTION OF THIS PROGRAM\'S DOCUMENTATION\n");

	return BYSC_ARGUMENT_ERROR;
}

int command_error(const char *argument){
	fprintf(stderr,"COMMAND :: %s :: INVALID COMMAND OR ARGUMENT\n",argument);
	fprintf(stderr,"\tPLEASE REFER TO THE \'COMMANDS\' SECTION OF THIS PROGRAM\'S DOCUMENTATION\n");

	return BYSC_COMMAND_ERROR;
}
