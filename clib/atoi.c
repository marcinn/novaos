/***********************************************

    ATOI.H
    21.12.2002 (c) NovaTeam
    
    ANSI C atoi() - Ascii to integer
    Author: Marcin Nowak
    
 **********************************************/

unsigned long atoi(char *buf)
{
    unsigned long n=0,i;
    for(i=0;buf[i] >= '0' && buf[i] <= '9'; ++i)
	n=10*n+(buf[i] - '0');
    return(n);
}

