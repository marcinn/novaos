/***********************************************

    REPC.H
    21.12.2002 (c) NovaTeam
    
    Repeat char 'count' times
    Author: Marcin Nowak
    
 **********************************************/


char *repc(char c, unsigned int count, char *buf)
{
    unsigned int i=0;
    while(count--)
	buf[i++]=c;
    buf[i]='\0';
    return(buf);
}

