/***********************************************

    STRCMP.C
    21.12.2002 (c) NovaTeam
    
    ANSI C strcmp() - Compare strings
    Author: Marcin Nowak
    
    Returns: 0 - if strings are equal
	     or # of equal chars
	     
 **********************************************/

unsigned long strcmp(char *c1, char *c2)
{
    unsigned long i;

    for(i=0; c1[i]==c2[i];i++) {
	if(c1[i]=='\0')
	    return(0);
    }
    return c1[i]-c2[i];
}

