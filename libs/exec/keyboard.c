#include <exec/keyboard.h>
#include <exec/types.h>
#include <system.h>

extern void _printf(STRPTR,...);

kbuf_t	keyb[KEYB_BUFFER_SIZE];
__volatile__ ULONG keyb_state=0;
__volatile__ UBYTE kb_next=0, kb_actual=0;
__volatile__ UBYTE __rawkey;
__volatile__ UBYTE __leds=1;

// __volatile__ UBYTE __lastkey=0;


UBYTE ascii_n[KEYB_MAX_SCANCODE]=
{0,A_ESC,'1','2','3','4','5','6','7','8','9','0','-','=',A_BACKSPACE,
A_TAB,'q','w','e','r','t','y','u','i','o','p','[',']',A_ENTER,
A_CTRL,'a','s','d','f','g','h','j','k','l',';',0x27,'`',A_SHIFT,'\\',
'z','x','c','v','b','n','m',',','.','/',0,A_PRTSCR,A_ALT,' ',A_CAPSLOCK,
A_F1,0,0,0,0,0,0,0,0,0,A_PAUSE,A_SCRLOCK,0,0,0,0,A_LEFTARROW,0,A_RIGHTARROW};

UBYTE ascii_s[KEYB_MAX_SCANCODE]=
{0,A_ESC,'!','@','#','$','%','^','&','*','(',')','_','+',A_BACKSPACE,
A_TAB,'Q','W','E','R','T','Y','U','I','O','P','{','}',A_ENTER,
A_CTRL,'A','S','D','F','G','H','J','K','L',':','"','~',A_SHIFT,'|',
'Z','X','C','V','B','N','M','<','>','?',0,A_PRTSCR,A_ALT,' ',A_CAPSLOCK,
A_F1,0,0,0,0,0,0,0,0,0,A_PAUSE,A_SCRLOCK,0,0,0,0,A_LEFTARROW,0,A_RIGHTARROW};


void kbflush()
{
    kb_next=kb_actual=0;
}

void exec_defkeybhandler()
{
    UBYTE c,cx,cy;

    if(inportb(0x64)&1) {
	c=inportb(0x60);
	if(c==__rawkey+RK_KEYUP)
	    __rawkey=0;
	else
    	    if(c<89) __rawkey=c;
	switch(c) {
	    case RK_SHIFTL:
	    case RK_SHIFTR:
		keyb_state|=SK_SHIFT;
		break;
	    case RK_SHIFTL+RK_KEYUP:
	    case RK_SHIFTR+RK_KEYUP:
		keyb_state&=~SK_SHIFT;
		break;
	    default:
		if(c<109) {
		    if(keyb_state&SK_SHIFT)
			c=ascii_s[c];
		    else
			c=ascii_n[c];
		    if(c) {
		    	keyb[kb_next].ascii=c;
				kb_next++;
				if(kb_next>KEYB_BUFFER_SIZE-1)
			    	kb_next=0;
				if(kb_next==kb_actual) {
				    kb_actual++;
					if(kb_actual>KEYB_BUFFER_SIZE-1)
						kb_actual=0;
				}
		    }
		}
		break;
	}
    }
//    outportb(0x20,0x20);
}

void _waitkbdw()
{
    while(inportb(0x64)&0x02);
}
void _waitkbdr()
{
    while(inportb(0x64)&0x01);
}
void kbd_wait_ack()
{
    _waitkbdr();
    if(inportb(0x60) != 0xFA)
	#if DEBUG
		_printf("kbdaw: ack failed\n");
	#endif
}

void exec_InitKeyboard()
{
    UBYTE i;
    
    for(i=0;i<KEYB_BUFFER_SIZE;i++){
	keyb[i].ascii=0;
	keyb[i].rawkey=0;
    }

/*
    forbid();
    kcSetInt(33,(ULONG) &irq33, IGF_TYPICAL, KERNEL_CS);
    permit();
*/
    outportb(0x60,0xF3);
    _waitkbdw();
    outportb(0x60,0);

}

UBYTE kgetc()
{
    UBYTE c;

    for(;;) {
	if(kb_actual!=kb_next) {
	    c=keyb[kb_actual].ascii;
    	    kb_actual++;
	    if(kb_actual>KEYB_BUFFER_SIZE-1) {
		kb_actual=0;
	    }
	    if(c)
		return(c);
	}

    }
}

UBYTE kgetcnw()
{
    UBYTE c;
    if(kb_actual!=kb_next) {
	c=keyb[kb_actual].ascii;
    	kb_actual++;
	if(kb_actual>KEYB_BUFFER_SIZE-1) {
	    kb_actual=0;
	}
        return(c);
    }
    
    return(0);
}

UBYTE krawkey()
{
    UBYTE c;
    c=__rawkey;
    kbflush();
    return(c);
}

ULONG kgetline(UBYTE *str)
{
    UBYTE c;
    ULONG p=0;
    
    for(;;)
    {
	c=kgetc();
	switch(c) {
	    case '\n':
	    case '\0':
		str[p]=c;
		return(p);
	    case '\r':
		if(p>0)
		    p--;
		break;
	    default:
		str[p++]=c;
		break;
	}
    }
}

