/***********************************************

    STRLEN.C
    21.12.2002 (c) NovaTeam
    
    ANSI C stlen()
    Author: Marcin Nowak
    
 **********************************************/

unsigned long strlen(char *buf)
{
    register unsigned long i=0;
    register char *p=buf;

    while(*p++) i++;
    return(i);
}

