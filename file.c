/************************************************
  *	alines7777 <alines7777@disroot.org>	*
  *	file.c					*
  *	BYSC File Source			*
  *	BYte Sequence Composer			*
  ************************************************/

/* ** PREPROCESSOR INSTRUCTIONS ******************************************* */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bysc.h"
#include "cans.h"



/* ** FUNCTION PROTOTYPES ************************************************* */

/* local prototypes */
lineQ open_file(const char *filename,size_t *filesize,char *newline);
/* primary routine for reading existing files */
int check_file(const char *filename);

int file__tfbuffer(FILE *stream,const char *filename,size_t *filesize,char **tfilebuffer);

lineQ tfbuffer__linebuffer(const char *tfilebuffer,const size_t *filesize,char *newline);

int write_file(const char *filename,lineQ linebuffer);

int linebuffer__file(FILE *stream,const char* filename,lineQ linebuffer);

int clear_linebuffer(lineQ lines);

/* external prototypes */

/* error.c */
extern int f_read_error(const char *filename);

extern int m_l_alloc_error(lineQ pointer);

extern void m_dealloc_error(void);



/* ** FUNCTION DEFINITIONS ************************************************ */

lineQ open_file(const char *filename,size_t *filesize,char *newline){
	FILE *stream = NULL;
	char *tfbuffer = NULL;
	lineQ line = NULL;
	/* Temporary File Buffer string of bytes for temporarily storing file's contents before transferring to line buffer */

	if(file__tfbuffer(stream,filename,filesize,&tfbuffer) == BYSC_FILE_ERROR){
		stream = fopen(filename,READ_WRITE_N);
		if(stream == NULL){
			f_read_error(filename);
			return NULL;
		}
		else{
			line = (lineQ)malloc(sizeof(struct lineN));
			line->text = (char *)malloc(sizeof(char) * (strlen("") + 1));
			return line;
		}
	}

	line = tfbuffer__linebuffer(tfbuffer,filesize,newline);
	free(tfbuffer);
	/* return addresses from tfbuffer__linebuffer to line buffer in main */
	return line;
}

int file__tfbuffer(FILE *stream,const char *filename,size_t *filesize,char **tfilebuffer){
	size_t loop = 0;
	stream = fopen(filename,READ);
	/* open file stream */

	if(stream == NULL){
		return BYSC_FILE_ERROR;
	}
	/* check if file could be read */

	fseek(stream,0L,SEEK_END);
	*filesize = ftell(stream);
	fseek(stream,0L,SEEK_SET);
	/* get file size */
	*tfilebuffer = (char *)malloc((sizeof(char)) * (*filesize + 1));
	/* allocate file size to temporary file buffer */

	while(!feof(stream)){
		fread((char *)*tfilebuffer + loop,1,sizeof(char),stream);
		loop++;
	}

	fclose(stream);
	/* close read-only file stream */
	/* open again for writing later */
	return BYSC_TRUE;
}

lineQ tfbuffer__linebuffer(const char *tfilebuffer,const size_t *filesize,char *newline){
	size_t loop = 0;
	/* byte counter for the temporary file buffer */
	size_t back = 0;
	/* bookmark for loop */
	size_t check = 0;
	/* filebuffer checkpoint : set after each newline escape sequence */
	lineQ linebuffer = NULL;
	lineQ wline = NULL;
	lineQ lines = NULL;


	/* prepare line buffer nodes */
	linebuffer = (lineQ)malloc(sizeof(struct lineN));
	if(linebuffer == NULL){
		fprintf(stderr,"memory allocation failure -- quitting now\n");

		return NULL;
	}
	else{
		lines = linebuffer;
		wline = linebuffer;
		wline->next = NULL;
	}

	/* copy formatted text to a sequence of strings in a queue data structure */
	while(loop < *filesize + 1){
		/* check for a newline sequence (ascertained by operating system, or compatibility switch/command) */
		if(strncmp(tfilebuffer + loop,newline,strlen(newline)) == 0){
			check = loop;
			/* set checkpoint to cease copying & continue detecting newline sequences */
			loop = back;
			/* go back to current return point to begin copying */

			if(loop == check){ /* nth line contains only 'newline' */
				wline->text = (char *)malloc(sizeof(char));
				if(wline->text == NULL){
					m_l_alloc_error(linebuffer);
					return NULL;
				}
				/* allocate size of empty string (1 character) */
				wline->text[loop - back] = '\0';
				/* write empty string to line of text */
				wline->next = (lineQ)malloc(sizeof(struct lineN));
				if(wline->next == NULL){
					m_l_alloc_error(linebuffer);
					return NULL;
				}
				/* allocate next line in buffer */
				wline = wline->next;
				wline->text = NULL;
				wline->next = NULL;
				/* move to next line in buffer */
				loop += strlen(newline);
				/* skip the newline sequence in the file buffer */
				/* to write a new formatted file, the line buffer, by definition, ought not have newline sequences in its strings */
				/* this is to enable the program to newline sequences at the end of each line as directed by the operating system, or a compatibility switch/command */
				back = loop;
				/* set a new return point */
			}
			else if(loop < check){
				wline->text = (char *)malloc(sizeof(char) * ((check - loop) + 1));
				while(loop < check){
					wline->text[loop - back] = *(tfilebuffer + loop);
					loop++;
				}
				wline->text[loop - back] = '\0';
				wline->next = (lineQ)malloc(sizeof(struct lineN));
				wline = wline->next;
				wline->text = NULL;
				wline->next = NULL;
				loop += strlen(newline);
				back = loop;
			}
			else{	/* THE UNTHINKABLE HAS HAPPENED -- HACKERS! THIS IS THE TIME TO PANIC !!!! */
				fprintf(stderr,"ANOMALY DETECTED -- PUTTING EVERYTHING AWAY & QUITTING NOW\n");
				if(lines != NULL){
					clear_linebuffer(lines);
				}
				return NULL;
			}
		}
		else if(loop == *filesize){
			check = loop;
			loop = back;

			if(loop == check){ /* last line is empty */
				wline->text = (char *)malloc(sizeof(char) * 1);
				wline->text[loop - back] = '\0';
				wline->next = NULL;
				loop++;
			}
			else if(loop < check){
				wline->text = (char *)malloc(sizeof(char) * ((loop - back) + 1));
				while(loop < check){
					wline->text[loop - back] = *(tfilebuffer + loop);
					loop++;
				}
				wline->text[loop - back] = '\0';
				wline->next = NULL;
				loop++;
			}
			else{
				fprintf(stderr,"ANOMALY DETECTED -- PUTTING EVERYTHING AWAY & QUITTING NOW\n");
				if(lines != NULL){
					clear_linebuffer(lines);
				}
				return NULL;
			}
		}
		else{
			loop++;
		}
	}
	/* return addresses of allocated memory to return statement in open_file */
	return linebuffer;
}

int clear_linebuffer(lineQ lines){
	lineQ wline = lines;

	while(wline != NULL){
		if(wline->text != NULL){
			free(wline->text);
			wline->text = NULL;
		}
		lines = lines->next;
		free(wline);
		wline = lines;
	}

	return BYSC_TRUE;
}
