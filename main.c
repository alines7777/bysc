/************************************************
  *	BYte Sequence Composer			*
  *	main.c					*
  *	Dr. Cutie - CENU <cenu00@gmail.com>	*
  ************************************************/

/* ** PREPROCESSOR INSTRUCTIONS ******************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bysc.h"
#include "cans.h"



/* ** FUNCTION PROTOTYPES ************************************************* */

/* local prototypes */
int is_switch(const char *argument);
/* return values:
	RETURN_NS - not a switch
	RETURN_R - read switch
	RETURN_W - write swtich
	RETURN_RW - read & write switch
	RETURN_O - output file switch */

int is_mode(const char *argument); /* this function is only called in the event a switch is used */
/* return values:
	RETURN_NM - not a mode - user doesn't know how to enter arguments properly
	RETURN_LF - Line Feed (UNIX, Linux, BSD, OS X, Haiku/BeOS, Plan9, Amiga)
	RETURN_CRLF - Carriage Return & Line Feed (OS/2, DOS, NT, VMS)
	RETURN_CR - Carriage Return (Mac OS Classic) */

/* external prototypes */

/* file.c */
extern lineQ open_file(const char *filename,const size_t *filesize,char *newline);

extern int clear_linebuffer(lineQ lines);

/* command.c */
extern int file_mode(char **input_file,char **output_file,lineQ *linebuffer,char **iformat,char **oformat);

extern int pen_mode(char **filename,lineQ *linebuffer,char **newline);

extern void wipe_string(char *string);

extern char *set_format(const char *newline);

/* error.c */
extern int arg_mode_error(const char *argument);

extern int arg_out_error(const char *argument);



/* ** MAIN FUNCTION ******************************************************* */

int main(const int argc,const char **argv){
	size_t argloop = 1;
	size_t filesize = 0;

	lineQ linebuffer = NULL;
	lineQ wline = NULL;

	char *input_format = NULL;
	char *output_format = NULL;

	char *input_file = NULL;
	char *output_file = NULL; /* if the '-o' switch is used */

	/* these variables exist for working w/ multiple arguments */
	int rswitch_called = 0; /* determines whether or not the read switch has been called */
	int wswitch_called = 0; /* determines whether or not the write switch has been called */
	/* the read & write switch will set both variables */
	int oswitch_called = 0; /* determines whether or not the output switch has been called */
	int if_set = 0; /* determines whether or not the input format is already set */
	int of_set = 0; /* determines whether or not the output format is already set */
	int ifile_set = 0; /* determines whether or not a filename has been called */
	int ofile_set = 0; /* determines whether of not an output filename has been called */
	/* recurrences of these types of arguments should be ignored */

	int next_operation = BYSC_QUIT;

	/* count # of arguments for determining exactly what to do */
	if(argc < 2){
		input_format = set_format(NEW_LINE);
		output_format = input_format;
	}
	else if(argc > 2){
		while(argloop < argc){
			if(!rswitch_called && is_switch(*(argv + argloop)) == RETURN_R){
				if(argloop + 1 < argc){
					argloop++;
				}
				else{
					if(output_format != NULL){
						free(output_format);
					}
					if(input_file != NULL){
						free(input_file);
					}
					if(output_file != NULL){
						free(output_file);
					}

					return arg_mode_error(*(argv + argloop));
				}

				if(!if_set && is_mode(*(argv + argloop)) == RETURN_LF){
					input_format = set_format(NEW_LINE_LF);
					if_set = 1;
				}
				else if(!if_set && is_mode(*(argv + argloop)) == RETURN_CRLF){
					input_format = set_format(NEW_LINE_CRLF);
					if_set = 1;
				}
				else if(!if_set && is_mode(*(argv + argloop)) == RETURN_CR){
					input_format = set_format(NEW_LINE_CR);
					if_set = 1;
				}
				else{
					if(output_format != NULL){
						free(output_format);
					}
					if(input_file != NULL){
						free(input_file);
					}
					if(output_file != NULL){
						free(output_file);
					}

					return arg_mode_error(*(argv + argloop));
				}
				rswitch_called = 1;
			}
			else if(!wswitch_called && is_switch(*(argv + argloop)) == RETURN_W){
				if(argloop + 1 < argc){
					argloop++;
				}
				else{
					if(input_format != NULL){
						free(input_format);
					}
					if(input_file != NULL){
						free(input_file);
					}
					if(output_file != NULL){
						free(output_file);
					}

					return arg_mode_error(*(argv + argloop));
				}

				if(!of_set && is_mode(*(argv + argloop)) == RETURN_LF){
					output_format = set_format(NEW_LINE_LF);
					of_set = 1;
				}
				else if(!of_set && is_mode(*(argv + argloop)) == RETURN_CRLF){
					output_format = set_format(NEW_LINE_CRLF);
					of_set = 1;
				}
				else if(!of_set && is_mode(*(argv + argloop)) == RETURN_CR){
					output_format = set_format(NEW_LINE_CR);
					of_set = 1;
				}
				else{
					if(input_format != NULL){
						free(input_format);
					}
					if(input_file != NULL){
						free(input_file);
					}
					if(output_file != NULL){
						free(output_file);
					}

					return arg_mode_error(*(argv + argloop));
				}
				wswitch_called = 1;
			}
			else if((!rswitch_called && !wswitch_called) && is_switch(*(argv + argloop)) == RETURN_RW){
				if(argloop + 1 < argc){
					argloop++;
				}
				else{
					if(input_file != NULL){
						free(input_file);
					}
					if(output_file != NULL){
						free(output_file);
					}

					return arg_mode_error(*(argv + argloop));
				}

				if((!if_set && !of_set) && is_mode(*(argv + argloop)) == RETURN_LF){
					input_format = set_format(NEW_LINE_LF);
					output_format = input_format;
					if_set = 1;
					of_set = 1;
				}
				else if((!if_set && !of_set) && is_mode(*(argv + argloop)) == RETURN_CRLF){
					input_format = set_format(NEW_LINE_CRLF);
					output_format = input_format;
					if_set = 1;
					of_set = 1;
				}
				else if((!if_set && !of_set) && is_mode(*(argv + argloop)) == RETURN_CR){
					input_format = set_format(NEW_LINE_CR);
					output_format = input_format;
					if_set = 1;
					of_set = 1;
				}
				else{
					if(input_file != NULL){
						free(input_file);
					}
					if(output_file != NULL){
						free(output_file);
					}

					return arg_mode_error(*(argv + argloop));
				}
				rswitch_called = 1;
				wswitch_called = 1;
			}
			else if(!oswitch_called && is_switch(*(argv + argloop)) == RETURN_O){
				if(argloop + 1 < argc){
					argloop++;
				}
				else{
					if(input_format != NULL){
						free(input_format);
					}
					if(output_format != NULL){
						free(output_format);
					}

					return arg_out_error(*(argv + argloop));
				}
				output_file = (char *)malloc(sizeof(char) * (strlen(*(argv + argloop) + 1)));
				strncpy(output_file,*(argv + argloop),strlen(*(argv + argloop)));
				output_file[strlen(*(argv + argloop))] = '\0';
				ofile_set = 1;
				wswitch_called = 1;
			}
			else if(!ifile_set){
				input_file = (char *)malloc(sizeof(char) * (strlen(*(argv + argloop)) + 1));
				strncpy(input_file,*(argv + argloop),strlen(*(argv + argloop)));
				input_file[strlen(*(argv + argloop))] = '\0';
				ifile_set = 1;
			}
			else{
				if(output_format == input_format){
					if(input_format != NULL){
						free(input_format);
					}
				}
				else{
					if(input_format != NULL){
						free(input_format);
					}
					if(output_format != NULL){
						free(output_format);
					}
				}
				if(output_file == input_file){
					if(input_file != NULL){
						free(input_file);
					}
				}
				else{
					if(input_file != NULL){
						free(input_file);
					}
					if(output_file){
						free(output_file);
					}
				}
				fprintf(stderr,"EXTRANEOUS OR INVALID ARGUMENT(S)\n");
				fprintf(stderr,"\tPLEASE REFER TO THE \'ARGUMENTS\' SECTION OF THIS PROGRAM\'S DOCUMENTATION\n");
				return BYSC_ARGUMENT_ERROR;
			}
			argloop++;
		}
		if(input_format == NULL){
			input_format = set_format(NEW_LINE);
		}
		if(output_format == NULL){
			output_format = input_format;
		}
		if(output_file == NULL){
			output_file = input_file;
		}
	}
	else if(argc > 8){
		fprintf(stderr,"EXTRANEOUS OR INVALID ARGUMENT(S)\n");
		fprintf(stderr,"\tPLEASE REFER TO THE \'ARGUMENTS\' SECTION OF THIS PROGRAM\'S DOCUMENTATION\n");
		return BYSC_ARGUMENT_ERROR;
	}
	else{
		input_format = set_format(NEW_LINE);
		output_format = input_format;
		input_file = (char *)malloc(sizeof(char) * (strlen(*(argv + 1)) + 1));
		strncat(input_file,*(argv + 1),strlen(*(argv + 1)));
		input_file[strlen(*(argv + 1))] = '\0';
		output_file = input_file;
	}

	/* begin program proper */
	if(input_file == NULL){
		if(linebuffer != NULL){
			clear_linebuffer(linebuffer);
		}
		next_operation = file_mode(&input_file,&output_file,&linebuffer,&input_format,&output_format);
	}
	else{
		if(linebuffer != NULL){
			clear_linebuffer(linebuffer);
		}
		linebuffer = open_file(input_file,&filesize,input_format);
		if(output_file == NULL){
			output_file = input_file;
		}

		wline = linebuffer;
		while(wline != NULL){
			printf("%s\n",wline->text);
			wline = wline->next;
		}

		next_operation = pen_mode(&output_file,&linebuffer,&output_format);
	}
	if(next_operation != BYSC_QUIT){
		while(next_operation != BYSC_QUIT){
			if(next_operation == BYSC_FILEMODE){
				next_operation = file_mode(&input_file,&output_file,&linebuffer,&input_format,&output_format);
			}
			else if(next_operation == BYSC_PENMODE){
				next_operation = pen_mode(&output_file,&linebuffer,&output_format);
			}
		}
	}
	/* end of program cycle - free all allocated memory */
	/* the reason for checking whether or not memory has been allocated */
	/* is b/c it may turn out that it has remained unallocated, or freed */
	/* before the end of the program's cycle. & there's no use trying to */
	/* trying to free memory that hasn't been allocated, or memory that */
	/* that has already been freed */
	if(linebuffer != NULL){
		clear_linebuffer(linebuffer);
	}

	if(output_format == input_format){
		if(input_format != NULL){
			free(input_format);
		}
	}
	else{
		if(input_format != NULL){
			free(input_format);
		}
		if(output_format != NULL){
			free(output_format);
		}
	}

	if(output_file == input_file){
		if(input_file != NULL){
			free(input_file);
		}
	}
	else{
		if(input_file != NULL){
			free(input_file);
		}
		if(output_file != NULL){
			free(output_file);
		}
	}

	return 0;
}



/* ** FUCNTION DEFINITIONS ************************************************ */

int is_switch(const char *argument){
	int rv = RETURN_NS;

	if(strncmp(argument,SWITCH_R,strlen(SWITCH_R)) == 0){
		rv = RETURN_R;
	}
	else if(strncmp(argument,SWITCH_W,strlen(SWITCH_W)) == 0){
		rv = RETURN_W;
	}
	else if(strncmp(argument,SWITCH_RW,strlen(SWITCH_RW)) == 0){
		rv = RETURN_RW;
	}
	else if(strncmp(argument,SWITCH_WR,strlen(SWITCH_WR)) == 0){
		rv = RETURN_RW;
	}
	else if(strncmp(argument,SWITCH_O,strlen(SWITCH_O)) == 0){
		rv = RETURN_O;
	}

	return rv;
}

int is_mode(const char *argument){
	int rv = RETURN_NM;

	if(strncmp(argument,MODE_UNIX,strlen(MODE_UNIX)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_LINUX,strlen(MODE_LINUX)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_BSD,strlen(MODE_BSD)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_OSX,strlen(MODE_OSX)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_PLAN9,strlen(MODE_PLAN9)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_HAIKU,strlen(MODE_HAIKU)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_BEOS,strlen(MODE_BEOS)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_AMIGA,strlen(MODE_AMIGA)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_LF,strlen(MODE_LF)) == 0){
		rv = RETURN_LF;
	}
	else if(strncmp(argument,MODE_DOS,strlen(MODE_DOS)) == 0){
		rv = RETURN_CRLF;
	}
	else if(strncmp(argument,MODE_OS2,strlen(MODE_OS2)) == 0){
		rv = RETURN_CRLF;
	}
	else if(strncmp(argument,MODE_VMS,strlen(MODE_VMS)) == 0){
		rv = RETURN_CRLF;
	}
	else if(strncmp(argument,MODE_CRLF,strlen(MODE_CRLF)) == 0){
		rv = RETURN_CRLF;
	}
	else if(strncmp(argument,MODE_MAC,strlen(MODE_MAC)) == 0){
		rv = RETURN_CR;
	}
	else if(strncmp(argument,MODE_CR,strlen(MODE_CR)) == 0){
		rv = RETURN_CR;
	}

	return rv;
}
