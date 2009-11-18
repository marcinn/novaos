#include <exec/exec_private.h>
#include <string.h>
#include <system.h>
#include <exec/memory.h>
#include <exec/execbase.h>


UWORD _cursx, _cursy, _tcols, _trows;

ULONG _peekl(UWORD sel, ULONG addr)
{
    ULONG ret;
    
    __asm__ __volatile__ ("\tpushw %%es\n\t"
			    "movw %w1,%%es\n"
			    "es\n\t"
			    "movl (%2),%0\n\t"
			    "popw %%es\n\t"
			    : "=a" (ret)
			    : "m" (sel),
			      "r" (addr));
    return(ret);
}

UWORD _peekw(UWORD sel, ULONG addr)
{
    UWORD ret;
    
    __asm__ __volatile__ ("\tpushw %%es\n\t"
			    "movw %w1,%%es\n"
			    "es\n\t"
			    "movw (%2),%w0\n\t"
			    "popw %%es\n\t"
			    : "=a" (ret)
			    : "m" (sel),
			      "r" (addr));
    return(ret);
}

UBYTE _peekb(UWORD sel, ULONG addr)
{
    UBYTE ret;
    
    __asm__ __volatile__ ("\tpushw %%es\n\t"
			    "movw %w1,%%es\n"
			    "es\n\t"
			    "movb (%2),%b0\n\t"
			    "popw %%es\n\t"
			    : "=a" (ret)
			    : "m" (sel),
			      "r" (addr));
    return(ret);
}

void _pokel(UWORD sel, ULONG addr, ULONG dana)
{
    __asm__ __volatile__ ("\tpushw %%es\n\t"
			    "movw %w0,%%es\n"
			    "es\n\t"
			    "movl %2,(%1)\n\t"
			    "popw %%es\n\t"
			    :
			    : "m" (sel),
			      "r" (addr),
			      "r" (dana));
}

void _pokeb(UWORD sel, ULONG addr, UBYTE dana)
{
    __asm__ __volatile__ ("\tpushw %%es\n\t"
			    "movw %w0,%%es\n"
			    "es\n\t"
			    "movb %b2,(%1)\n\t"
			    "popw %%es\n\t"
			    :
			    : "m" (sel),
			      "r" (addr),
			      "r" (dana));
}

void _pokew(UWORD sel, ULONG addr, UWORD dana)
{
    __asm__ __volatile__ ("\tpushw %%es\n\t"
			    "movw %w0,%%es\n"
			    "es\n\t"
			    "movw %w2,(%1)\n\t"
			    "popw %%es\n\t"
			    :
			    : "m" (sel),
			      "r" (addr),
			      "r" (dana));
}

void _disable()
{
    __asm__ __volatile__ ("\n\tcli"::);
}

void _enable()
{
    __asm__ __volatile__ ("\n\tsti"::);
}

void _die()
{
	while(1);
}

void exec_dbug_clrscr() {
	ULONG size = _tcols * _trows;
	
	for (size;size>0;size--)
		_pokew(LINEAR_DS,EXEC_DEBUGVIDEOADDR+(size<<1),EXEC_DEBUGCON_EMPTYCHAR);
}

void exec_dbug_setcursor(UBYTE x, UBYTE y) {
    UWORD off=y*_tcols+x;
    _cursx=x;
    _cursy=y;
    outportb(CRT_ADDR_REG,CRT_CURSOR_LOC_HIGH);
    outportb(CRT_DATA_REG,off>>8);
    outportb(CRT_ADDR_REG,CRT_CURSOR_LOC_LOW);
    outportb(CRT_DATA_REG,off&0xFF);
}    

void _memcpy(ULONG src, ULONG dst, ULONG size)
{
	for(size;size>0;size--)
		_pokeb(LINEAR_DS, dst++, _peekb(LINEAR_DS, src++));
}

UBYTE exec_dbug_movcsr(BYTE xdir, BYTE ydir)
{
    UBYTE ret=0;

    if(xdir<0) {
	xdir=-xdir;
	if(_cursx<xdir) {
	    if(_cursy>0) {
		xdir-=_cursx;
		_cursx=_tcols-xdir;
		_cursy--;
	    }
	    else
		_cursx=0;
	}
	else {
	    _cursx-=xdir;
	}
    }
    else {
	if(_cursx+xdir>=_tcols) {
   	    xdir-=_tcols-_cursx;
	    _cursx=xdir;
	    if(_cursy<_trows-1) {
		_cursy++;
	    } else
		ret=1;
	} else
	    _cursx+=xdir;
    }
    
    if(ydir) {
	if(ydir<0) {
	    ydir=-ydir;
	    if(_cursy<ydir)
		_cursy=0;
	    else
		_cursy-=ydir;
	}
	else {
	    if(_cursy+ydir>=_trows) {
		_cursy=_trows-1;
		ret=1;
	    }
	    else
		_cursy+=ydir;
	}
    }	

    exec_dbug_setcursor(_cursx,_cursy);
    return(ret);
}	

void exec_dbug_putc(char c)
{
    ULONG i,j;
    BYTE dx,dy;
    
    switch(c) {
	case '\n':
	    _cursx=0;
	    dx=0;
	    dy=1;
	    break;
	case '\b':
	    exec_dbug_movcsr(-1,0);
	    _pokeb(LINEAR_DS,EXEC_DEBUGVIDEOADDR+((_tcols*_cursy+_cursx)<<1),0x20);
	    return;
	case '\r':
	    exec_dbug_setcursor(0,_cursy);
	    return;
	default:
	    dx=1;
	    dy=0;	
	    _pokeb(LINEAR_DS,EXEC_DEBUGVIDEOADDR+((_tcols*_cursy+_cursx)<<1), c);
	    break;
    }

    if(exec_dbug_movcsr(dx,dy)) {
	_memcpy(EXEC_DEBUGVIDEOADDR + (_tcols<<1),
			    EXEC_DEBUGVIDEOADDR,
			    _tcols*(_trows-1)*2);
	    for(i=EXEC_DEBUGVIDEOADDR+(_tcols*_cursy*2),j=i+(_tcols<<1); i<j; i+=2)
		_pokew(LINEAR_DS,i,EXEC_DEBUGCON_EMPTYCHAR);
    }
    
//    kputc(EXEC_DEBUGVIDEOADDR+((_tcols*_cursy+vm.c_cx)<<1), 0x20, vm.v_attrs);
}


void _printf(char *string, ...)
{
    va_list args;
    va_start(args,string);
    do_printf(string, args, (void *) &exec_dbug_putc );
    va_end(args);
}

void exec_mh_addblock(ULONG addr, ULONG size, struct ExecBase *SysBase)
{
	struct MemBlock *newmb,*mb;
	
	newmb = (struct MemBlock *) addr;
	newmb->mb_Node.ln_Next = (struct Node *) NULL;
	newmb->mb_Node.ln_Prev = (struct Node *) NULL;
	newmb->mb_Magic = MEMF_MAGIC;
	newmb->mb_Size = size - sizeof(struct MemBlock);

	AddTail(&SysBase->FreeMemList, newmb);
/*
	if(mb=(struct MemBlock *) GetTail(&SysBase->FreeMemList))
		AddTail(&SysBase->FreeMemList, newmb);

	{
		mb->mb_Node.ln_Next = (struct Node *) newmb;
		newmb->mb_Node.ln_Prev = (struct Node *) mb;
		SysBase->FreeMemList.lh_Tail = (struct Node *) newmb;
	}

	else {
		// brak ogona lub glowy swiadczy o pustej liscie
		SysBase->FreeMemList.lh_Head = (struct Node *) newmb;
		SysBase->FreeMemList.lh_Tail = SysBase->FreeMemList.lh_Head;
	}
*/

}	

void exec_dbug_dumpmemblocks(struct ExecBase *SysBase)
{
	struct MemBlock *mb;
	char buf[80];
	UWORD i=0;
	ULONG sum=0;
	
	_printf("\n%4s%-11s%-11s\n","Lp","Start","Size");
	_printf("%s\n",repc('-',40,buf));
	
	mb=(struct MemBlock *) GetHead(&SysBase->FreeMemList);
	while(mb) {
		i++;
		_printf("%4i%11#X%11i\n",i,(ULONG) mb, mb->mb_Size);
		sum+=mb->mb_Size;
		mb=(struct MemBlock *) GetNext(mb);
	}
//	_printf("%s\n",repc('-',40,buf));
	if(!i)
		_printf("No memory blocks available.\n");
	else {
		_printf("%26i Free Memory Available\n", sum);
	}
}

void exec_dbug_showlibs(struct ExecBase *SysBase)
{
	struct Library *lib;
	UWORD i;
	char buf[80];
	
	_printf("\nLibraries list:\n");
	_printf("%4s%-20s%8s%8s%12s%-6s\n",
			"Lp.","Lib name","Ver","Rev","Base","Pri");
	_printf("%s\n",repc('-',79,buf));
	
	lib=(struct Library *) GetHead(&SysBase->LibList);
	i=0;
	while(lib) {
		i++;
		_printf("%3i %-20s%8i%8i%12#X%6i\n",
				i,
				lib->lib_IdString,
				lib->lib_Version,
				lib->lib_Revision,
				(ULONG) lib,
				lib->lib_Node.ln_Pri
				);
		lib=(struct Library *) GetNext(lib);
	}
	
	if(!i)
		_printf("No libraries loaded.\n");
}

UBYTE exec_GetCMOSByte(UBYTE adr)
{
	UBYTE ret;
	
	__asm__ ("movb	%1,%%bl\n\t"
			"inb	$0x70,%%al\n\t"
			"andb	$192,%%al\n\t"
			"orb	%%bl,%%al\n\t"
			"outb	%%al,$0x70\n\t"
			"inb	$0x71,%%al\n\t"
			"movb	%%al,%0\n\t"
			: "=r" (ret)
			: "r" (adr));
	return(ret);
}

UWORD exec_GetCMOSWord(UBYTE adr)
{
	UWORD ret;
	
	ret=exec_GetCMOSByte(adr);
	ret|=(exec_GetCMOSByte(adr+1)<<8);
	
	return(ret);
}
