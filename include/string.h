#ifndef STRING_H
#define STRING_H

/***********************************************

    STRING.H
    21.12.2002 (c) NovaTeam
    
    Header for ANSI-C String Operations
    Author: Marcin Nowak
    
 **********************************************/

#include <stdarg.h>

unsigned long	atoi(char *buf);
char			*repc(char, unsigned int, char *);
unsigned long	strcmp(char *, char *);
unsigned long	strlen(char *);
void 			do_printf (char *string, va_list args, void *fn());

/* do_printf() flags: */

#define	DPF_UPPERCASE		1
#define	DPF_LEADINGZERO		2
#define DPF_HEXPREFIX		4
#define DPF_LEADSPACES		8
#define DPF_REVERSE			16
#define DPF_LEFTALIGN		32
#define	CHAR_NL				0xA


#endif /* STRING_H */

