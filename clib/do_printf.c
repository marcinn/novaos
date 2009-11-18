#include <exec/types.h>
#include <stdarg.h>
#include <string.h>

void do_printf (char *string, va_list args, void *fn())
{
    ULONG i,j, l, count;
    char * p=string, *s, buf[30], podstawa;
    BYTE dir,lz;
    ULONG flags, leadspaces;
    
    count=0;
    while(*p) {
	flags=leadspaces=0;
	switch(*p) {
	    case '%':
TCASE:
		switch(*++p) {
		    case '1':
		    case '2':
		    case '3':
		    case '4':
		    case '5':
		    case '6':
		    case '7':
		    case '8':
		    case '9':
			leadspaces=leadspaces*10+(*p-'0');
			goto TCASE;
			break;
		    case '-':
			flags|=DPF_LEFTALIGN;
			goto TCASE;
			break;
		    case 'c':
			fn(va_arg(args, UBYTE));
			count++;
			break;
		    case 's':
			s=va_arg(args, char *);
			goto WRITE;
		    case 'i':
			podstawa=10;
			goto DO_NUMBER;
		    case 'o':
			podstawa=8;
		    case 'X':
			flags|=DPF_UPPERCASE;
		    case 'x':
			podstawa=16;
DO_NUMBER:
			{
			    ULONG n,t;
			    n=va_arg(args, ULONG);
			    s=buf;
			    				
			    do {
				t=n % podstawa;
				if(t<10) *s++=t+'0';
				    else if (flags&DPF_UPPERCASE)
					*s++=t-10+'A';
					    else *s++=t-10+'a';
				n=n/podstawa;
			    } while(n!=0);
			    if(podstawa==0x10) lz=8;
			    else lz=10;

			    if(flags&DPF_LEADINGZERO)
				for(i=lz-(s-buf);i>0;i--)
				    *s++='0';
			    if(flags&DPF_HEXPREFIX)
				if(podstawa==0x10) {
				    *s++='x';
				    *s++='0';
				}
			    *s='\0';
			    s=buf;
			    flags|=DPF_REVERSE;
			}
WRITE:
			l=strlen(s);
			if(flags&DPF_REVERSE) {
			    s+=l-1;
			    dir=-1;
			} else dir=1;

			count+=l;
			if(leadspaces>0) {
			    if(l>leadspaces) {    
				l=leadspaces;
	    			leadspaces=0;
			    }
			}
			if(!(flags&DPF_LEFTALIGN))
			    if(leadspaces>0)
				for(i=0,j=leadspaces-l;i<j;i++) {
				    fn(0x20);
				    count++;
				}
				
			for(i=0;i<l;i++) {
			    fn(*s);
			    s+=dir;
			}
			
			if(flags&DPF_LEFTALIGN)
			    if(leadspaces>0)
				for(i=0,j=leadspaces-l;i<j;i++) {
				    fn(0x20);
				    count++;
				}
			
			break;
		    case '0':
			if(leadspaces>0)
			    leadspaces=leadspaces*10;
			else
			    flags|=DPF_LEADINGZERO;
			goto TCASE;
			break;
		    case '#':
			flags|=DPF_HEXPREFIX;
			goto TCASE;
			break;
		    default:
			fn(*p);
			break;
		}
		break;

/* --- "[*ep?" "[*eb?" sequence check

	    case '[':
		switch(*++p) {
		    case '*':
			switch(*++p) {
			    case 'e':
				switch(*++p) {
				    case 'p':
					p++;
					if( (*p>= '0') && (*p <= '9'))
					    vm.v_attrs = SetColor( *p - '0', (vm.v_attrs & 0xF0) >>4);
					else if ((*p>= 'a') && (*p <= 'f'))
					    vm.v_attrs = SetColor( *p - 'a' + 10, (vm.v_attrs & 0xF0) >>4);
					break;
				    case 'b':
					p++;
					if ((*p>='0')&&(*p<='9'))
					    vm.v_attrs=SetColor((vm.v_attrs&0xF),*p-'0');
					else if ((*p>='a')&&(*p<='f'))
					    vm.v_attrs=SetColor((vm.v_attrs&0xF),*p-'a');
					break;
				    default:
					p-=3;
					fn(*p++);
					count++;
					continue;
					break;
				}
				break;
	    		    default:
				p-=2;
				fn(*p++);
				count++;
				continue;
				break;
			}
			break;
		    default:
			p--;
			fn(*p++);
			count++;
			continue;
			break;	
		}
		break;
*/

	    default:
		fn(*p);
		count++;
		break;
	}
    p++;
    } 
    va_end();
}


