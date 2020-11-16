/************************************************
  *	alines7777 <alines7777@disroot.org>	*
  *	command.c				*
  *	BYSC Command Parser Source		*
  *	BYte Sequence Composer			*
  ************************************************/

/* ** PREPROCESSOR INSTRUCTIONS ******************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bysc.h"
#include "cans.h"



/* ** GLOBAL VARIABLES **************************************************** */

char com[BYSC_LINE_LIMIT + 1] = { '\0' }; /* a pointer attatched to this character will carry commands */

/*	this variable has been made global to avoid having long comands
	overwrite the return addresses of functions in which it may be used.
	making it local to a function would cause potential problems.
	i know what i'm doing */

char changes = BYSC_NOCHANGES;
/* '*' - no unwritten changes to file buffer */
/* '!' - unwritten changes to file buffer*/



/* ** FUNCTION PROTOTYPES ************************************************* */

/* local prototypes */
int file_mode(char **ifile,char **ofile,lineQ *linebuffer,char **iformat,char **oformat);
/* interface for working w/ the filesystem */
/* return values:
	0 - quit program
	2 - switch to pen mode */

char *set_format(const char *newline);

int pen_mode(char **filename,lineQ *linebuffer,char **newline);
/* interface for editing the buffer from the selected file */
/* return values:
 	0 - quit program
	1 - switch to file mode */

int file_parse(char *cmd,lineQ *linebuffer);
/* parse commands given by the user :: file mode */
int pen_parse(char *cmd,lineQ *linbuffer);
/* parse commands given by the user :: pen mode */

int quit(const int status);

argQ string__args(const char *cmd,int *status);

int is_number(const char *string);

void wipe_string(char *string);

void clean_string(char *string);

const size_t count_arguments(argQ args);

int clear_command(argQ args);

/* external prototypes */

/* file.c */
extern lineQ open_file(char *filename,size_t *filesize,char *newline);

extern int write_file(const char *filename,lineQ linebuffer);

extern int clear_linebuffer(lineQ linebuffer);

extern int command_error(const char *argument);

extern int m_a_alloc_error(argQ pointer);

extern void m_dealloc_error(void);

/* pen.c -- forthcoming */



/* ** FUNCTION DEFINITIONS ************************************************ */

int file_mode(char **ifile,char **ofile,lineQ *linebuffer,char **iformat,char **oformat){
	int sw = BYSC_FILEMODE;
	char *command = NULL;

	printf("[%c]: ",changes);
	command = fgets(com,(int)BYSC_LINE_LIMIT,stdin);
	clean_string(command);

	if(strlen(command) != strlen("")){
		sw = file_parse(command,linebuffer);
	}
		else{
			sw = BYSC_FILEMODE;
	}

	wipe_string(command);
	command = NULL;

	return sw;
}

char *set_format(const char *newline){
	char *format = NULL;

	format = (char *)malloc(sizeof(char) * (strlen(newline) + 1));
	strncpy(format,newline,strlen(newline));
	format[strlen(newline) + 1] = '\0';

	return format;
}

int pen_mode(char **filename,lineQ *linebuffer,char **newline){
	int sw = BYSC_PENMODE;
	char *command = NULL;

	printf("<%c>: ",changes);
	command = fgets(com,(int)BYSC_LINE_LIMIT,stdin);
	clean_string(command);

	if(strlen(command) != strlen("")){
		sw = pen_parse(command,linebuffer);
	}
	else{
		sw = BYSC_PENMODE;
	}

	wipe_string(command);
	command = NULL;

	return sw;
}

int file_parse(char *cmd,lineQ *linebuffer){
	int status = 0;

	argQ args = string__args((const char *)cmd,&status);
	argQ warg = NULL;

	if(args == NULL && status == BYSC_TRUE){
		return BYSC_FILEMODE;
	}
	else if(args == NULL && status == BYSC_FALSE){
		return BYSC_QUIT;
	}
	else if(args != NULL && status == BYSC_TRUE){
		warg = args;
	}
	else{
		fprintf(stderr,"ANOMALY DETECTED -- PUTTING EVERYTHING AWAY & QUITTING NOW\n");
		clear_command(args);
		return BYSC_QUIT;
	}

	if(strncmp(warg->argument,BYSC_COMMAND_QUIT,strlen(warg->argument)) == 0){
		clear_command(args);
		status = quit(status);

		return BYSC_QUIT;
	}
	else if(strncmp(warg->argument,BYSC_F_COMMAND_PEN,strlen(warg->argument)) == 0){
		clear_command(args);

		return BYSC_PENMODE;
	}
	else{
		command_error(warg->argument);
		clear_command(args);

		return BYSC_FILEMODE;
	}
}

int pen_parse(char *cmd,lineQ *linebuffer){
	int status = 0;

	unsigned long lineA = 0;
	unsigned long lineB = 0;

	unsigned long lineW = 0;
	unsigned long lineX = 0;

	argQ args = string__args((const char *)cmd,&status);
	argQ warg = NULL;

	lineQ wline = *linebuffer;

	if(args == NULL && status == BYSC_TRUE){
		return BYSC_PENMODE;
	}
	else if(args == NULL && status == BYSC_FALSE){
		return BYSC_QUIT;
	}
	else if(args != NULL && status == BYSC_TRUE){
		warg = args;
	}
	else{
		fprintf(stderr,"ANOMALY DETECTED -- PUTTING EVERYTHING AWAY & QUITTING NOW\n");
		clear_command(args);
		return BYSC_QUIT;
	}

	if(strncmp(warg->argument,BYSC_COMMAND_QUIT,strlen(warg->argument)) == 0){
		clear_command(args);
		status = quit(status);

		return status;
	}
	else if(strncmp(warg->argument,BYSC_COMMAND_CLOSE_QUIT,strlen(warg->argument)) == 0){
		clear_command(args);

		return BYSC_QUIT;
	}
	else if(strncmp(warg->argument,BYSC_P_COMMAND_FILE,strlen(warg->argument)) == 0){
		clear_command(args);

		return BYSC_FILEMODE;
	}

	else if(strncmp(warg->argument,BYSC_P_COMMAND_PRINT,strlen(warg->argument)) == 0){

		if(warg->next == NULL){
			while(wline != NULL){
				printf("%s%s%*lu :%s\t%s%s\n",BRT,CYN,BYSC_PADDING,++lineW,YLW,wline->text,DEF);
				wline = wline->next;
			}
			printf("\n");
		}
		else{
			warg = warg->next;

			while(warg != NULL){
				wline = *linebuffer;
				lineA = 0;
				lineB = 0;
				lineW = 0;
				lineX = 0;

				if(is_number(warg->argument) == BYSC_TRUE){
					if(warg->next != NULL){
						if(strncmp(warg->next->argument,BYSC_COMMAND_THROUGH,strlen(warg->next->argument)) == 0){
							lineA = atol(warg->argument);

							if(warg->next->next != NULL){
								warg = warg->next->next;
							}
							else{
								command_error(warg->next->argument);
								clear_command(args);

								return BYSC_PENMODE;
							}

							if(is_number(warg->argument) == BYSC_TRUE){
								lineB = atol(warg->argument);

								lineB > lineA ? (void)(0) : (lineA = ((lineA = lineA ^ lineB) ^ (lineB = lineA ^ lineB)));

								while(lineW++ < lineA-1 && wline != NULL){
									wline = wline->next;
								}
								lineW--;
								while(lineW++ < lineB && wline != NULL){
									/* uses macros from 'cans.h' */
									printf("%s%s%*lu :%s\t%s%s\n",BRT,CYN,BYSC_PADDING,lineW,YLW,wline->text,DEF);
									wline = wline->next;
								}
								printf("\n");
							}
							else{
								command_error(warg->argument);
								clear_command(args);

								return BYSC_PENMODE;
							}
						}
						else{
							lineX = atol(warg->argument);

							while(lineW++ < lineX-1 && wline != NULL){
								wline = wline->next;
							}

							if(wline != NULL){
								/* uses macros from 'cans.h' */
								printf("%s%s%*lu :%s\t%s%s\n\n",BRT,CYN,BYSC_PADDING,lineX,YLW,wline->text,DEF);
							}
						}
					}
					else{
						lineX = atol(warg->argument);

						while(lineW++ < lineX-1 && wline != NULL){
							wline = wline->next;
						}

						if(wline != NULL){
							/* uses macros from 'cans.h' */
							printf("%s%s%*lu :%s\t%s%s\n\n",BRT,CYN,BYSC_PADDING,lineX,YLW,wline->text,DEF);
						}
					}
				}
				else{
					command_error(warg->argument);
					clear_command(args);

					return BYSC_PENMODE;
				}

				warg = warg->next;
			}
		}

		clear_command(args);

		return BYSC_PENMODE;
	}

	else{
		command_error(warg->argument);
		clear_command(args);

		return BYSC_PENMODE;
	}
}

int quit(const int status){
	char *input = NULL;

	wipe_string(com);
	if(changes == BYSC_CHANGES){
		printf("UNSAVED CHANGES::OVERWRITE STREAM?\n~~ ");
		input = fgets(com,(int)BYSC_LINE_LIMIT,stdin);
		if(strncmp(input,BYSC_YES,strlen(BYSC_YES)) == 0){
			printf("::SAVE FUNCTION HERE::\n");
			wipe_string(input);
			return BYSC_QUIT;
		}
		else if(strncmp(input,BYSC_NO,strlen(BYSC_NO)) == 0){
			wipe_string(input);
			printf("QUIT ANYWAY?\n~~ ");
			input = fgets(com,(int)BYSC_LINE_LIMIT,stdin);

			if(strncmp(input,BYSC_YES,strlen(BYSC_YES)) == 0){
				wipe_string(input);
				return BYSC_QUIT;
			}
			else{
				wipe_string(input);
				return status;
			}
		}
		else{
			wipe_string(input);
			return status;
		}
	}
	else if(changes == BYSC_NOCHANGES){
		return BYSC_QUIT;
	}
	else{
		fprintf(stderr,"ANOMALY DETECTED::PUTTING EVERYTHING AWAY & QUITTING NOW\n");
		return BYSC_QUIT;
	}
}

argQ string__args(const char *cmd,int *status){
	size_t loop = 0;
	size_t check = 0;
	size_t back = 0;

	argQ args = NULL;
	argQ warg = NULL;

	if(strlen(cmd) != strlen("")){
		args = (argQ)malloc(sizeof(struct argN));
		if(args == NULL){
			m_a_alloc_error(args);
			*status = BYSC_FALSE;
			return NULL;
		}
		else{
			warg = args;
			warg->next = NULL;
		}

		while(loop < strlen(cmd) + 1){
			if(*(cmd + loop) == '\t' || *(cmd + loop) == ' '){
				check = loop;
				loop = back;

				if(loop == check){
					back = ++loop;
				}
				else if(loop < check){
					warg->argument = (char *)malloc(sizeof(char) * ((check - loop) + 1));
					if(warg->argument == NULL){
						m_a_alloc_error(args);
						*status = BYSC_FALSE;
						return NULL;
					}
					while(loop < check){
						warg->argument[loop - back] = *(cmd + (loop));
						loop++;
					}
					warg->argument[loop - back] = '\0';
					warg->next = (argQ)malloc(sizeof(struct argN));
					warg = warg->next;
					if(warg == NULL){
						m_a_alloc_error(args);
						*status = BYSC_FALSE;
						return NULL;
					}
					warg->argument = NULL;
					warg->next = NULL;
					back = ++loop;
				}
				else{
					fprintf(stderr,"ANOMALY DETECTED -- PUTTING EVERYTHING AWAY AND QUITTING NOW\n");
					clear_command(args);
					*status = BYSC_FALSE;
					return NULL;
				}
			}
			else if(*(cmd + loop) == '\0' || loop == strlen(cmd)){
				check = loop;
				loop = back;

				if(loop == check){
					back = ++loop;
				}
				else if(loop < check){
					warg->argument = (char *)malloc(sizeof(char) * ((check - loop) + 1));
					if(warg->argument == NULL){
						m_a_alloc_error(args);
						*status = BYSC_FALSE;
						return NULL;
					}
					while(loop < check){
						warg->argument[loop - back] = *(cmd + (loop));
						loop++;
					}
					warg->argument[loop - back] = '\0';
					warg->next = NULL;
					back = ++loop;
				}
			}
			else{
				loop++;
			}
		}
	}
	else{
		*status = BYSC_TRUE;
		return NULL;
	}

	*status = BYSC_TRUE;
	return args;
}

int is_number(const char *string){
	size_t n = 0;

	while(*(string + n) != '\0'){
		if(isdigit(*( string + n ))){
			n++;
		}
		else{
			return BYSC_FALSE;
		}
	}

	return BYSC_TRUE;
}

void wipe_string(char *string){
	size_t l = 0;

	for(l = 0;l <= strlen(string);l++){
		*(string + l) = '\0';
	}
	return;
}

void clean_string(char *string){
	int l = 0;

	while(*(string + l) != '\0'){
		if(strncmp(string + l,NEW_LINE_LF,strlen(NEW_LINE_LF)) == 0){
			wipe_string(string + l++);
		}
		else if(strncmp(string + l,NEW_LINE_CR,strlen(NEW_LINE_CR)) == 0){
			wipe_string(string + l++);
		}
		else if(strncmp(string + l,NEW_LINE_CRLF,strlen(NEW_LINE_CRLF)) == 0){
			wipe_string(string + l++);
		}
		else{
			l++;
		}
	}

	return;
}

int clear_command(argQ args){
	argQ warg = args;

	while(warg != NULL){
		if(warg->argument != NULL){
			free(warg->argument);
			warg->argument = NULL;
		}
		args = args->next;
		free(warg);
		warg = args;
	}

	return BYSC_TRUE;
}
