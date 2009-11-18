#include <exec/execbase.h>
#include <exec/memory.h>
#include <exec/types.h>
#include <exec/exec_private.h>
#include <exec/lists.h>
#include <exec/alerts.h>
#include <exec/resident.h>
#include <nova/libdef.h>
#include <nova/8253.h>
#include <nova/8259a.h>
#include <exec/keyboard.h>
#include <exec/interrupts.h>
#include <inline/exec.h>
#include <system.h>
#include "exec_init.h"

const __TEXT ULONG LIBSTART=1;		/* very very first byte of .text section */
// const __TEXT struct ExecBase LibBase={};


struct ExecBase *SysBase, *ExecBase;

extern void MakeFunctions(); /* for bootup only */
//extern ULONG Avail(ULONG, struct Library *);

#define	EXEC_VERSION	2
#define	EXEC_REVISION	5

#define	NAME_STRING		"exec.library"
#define	VERSION_STRING	"2.5"

const char 	EXEC_NAME[] = NAME_STRING;
const char	EXEC_VERID[] = VERSION_STRING;

/* PRIVATE FUNCTIONS */

static void 	exec_boot();
int 			_start();
static ULONG 	exec_CountMem();
static void 	exec_ZeroMem(ULONG start, ULONG size);
static void		exec_settimerfreq(ULONG);
static UBYTE	exec_init8259a();
static void		exec_set8259aIMR(UBYTE, UBYTE);
void exec_setup_dbug();
ULONG exec_countfuncs();

extern void exec_init_idt();
extern void exec_defaultintserver();

/* RESIDENT */

struct ExtResident Exec_Resident=
{
	RTC_MATCHWORD,
	(struct Resident *) &Exec_Resident,
	(APTR) &LIBEND,
	RTF_SINGLETASK,
	EXEC_VERSION,
	RTT_LIBRARY,
	105,
	(char *) EXEC_NAME,
	(char *) EXEC_VERID,
	&init,
	&exec_boot
};

// static const char 	EXEC_NAME[]=NAME_STRING;
// static const char	EXEC_VERID[]="002.05";

/*	Wartosc tej zmiennej zostanie ustalona przez exec_boot()
	Taka postac powinna znalezc sie w naglowku kazdego programu */
	
/* LIB ENTRY */

int _start() {
	return(-1);
}

/**************************************************************************
 ***                              EXEC BOOT                             ***
 **************************************************************************/

void exec_boot()
{
	ULONG numfunc,extmem;
	
	/* COUNTING EXEC FUNCTIONS */

	numfunc=exec_countfuncs();
	
	/*	ustalamy adres ExecBase/SysBase na 0x1000+wielkosc
		tablicy jmp_vec_table. w tym miejscu nie mozna
		uzyc AllocMem()	*/
		
	ExecBase=(struct ExecBase *) (0x1000+(numfunc*sizeof(struct LibVec)));
	*(struct ExecBase **) EXECBASE_PTR = ExecBase;
	SysBase = (struct ExecBase *) ExecBase;
	exec_ZeroMem((ULONG) ExecBase, sizeof(ExecBase));

	/* DBUG INIT */

	exec_setup_dbug();

	/* Call MakeFunctions() */

	PUSHPARAM(ExecBase);
	PUSHPARAM(numfunc);
	PUSHPARAM(exec_functable);
	PUSHPARAM(ExecBase);
	__asm__("call *%0\n\t"
			"addl $24,%%esp" :
			: "r" (&MakeFunctions));


/*	=======================================================================
		OD TEGO MOMENTU MOZNA UZYWAC FUNKCJI EXEC'OWSKICH
	======================================================================= */


/************************************
 *           INIT MEMORY            *
 ************************************/

	{
		_printf("Checking installed memory.\n");
		
		extmem=exec_CountMem();				// counting ext memory
		NewList(&SysBase->FreeMemList);
		SysBase->FreeMemList.lh_Type = LH_MEMORYLIST;
		exec_mh_addblock(EXEC_FREEMEM1_START, EXEC_FREEMEM1_END - EXEC_FREEMEM1_START, SysBase);
		exec_mh_addblock(EXEC_EXTMEM_START, extmem, SysBase);
		exec_ZeroMem(0, sizeof(ULONG));
	}

/************************************
 *       INIT INTERRUPTS            *
 ************************************/

	{
		ULONG i;
	
		_printf("Setting up interrupts.\n");
		
		for(i=0;i<MAX_HANDLED_INTERRUPTS;i++)
			NewList(&SysBase->Interrupts[i]);

		SysBase->IntServer=&exec_defaultintserver;

		exec_init_idt();
		exec_init8259a();
	}
	
/************************************
 *   INIT DEFAULT KEYBOARD HANDLER  *
 ************************************/

// PROPONUJE PRZERZUCIC HANDLER DO ODDZIELNEGO MODULU
// JEDNAKZE DO TEGO WYMAGANA JEST JUZ OBSLUGA FLOPPY/HDD
// ORAZ PROCEDURY RELOKACJI

	{
		struct Interrupt *KeybHandler;
		
		KeybHandler=AllocMem(sizeof(struct Interrupt), MEMF_ANY);
		if(!KeybHandler) Alert(AT_DEADEND|AG_OutOfMemory);
	
		KeybHandler->i_Node.ln_Next=(struct Node *) NULL;
		KeybHandler->i_Node.ln_Prev=(struct Node *) NULL;		
		KeybHandler->i_FuncPtr=&exec_defkeybhandler;
		
		AddTail(&SysBase->Interrupts[33],KeybHandler);	

		exec_set8259aIMR(0x0,0x0);			// ENABLE ALL INTERRUPTS CHANNELS
		__asm__ __volatile__ ("sti\n\t");	// INTs ON!!

		exec_InitKeyboard();

		_printf("Keyboard handler initialized.\n");
	}
	
/************************************
 *          INIT EXECBASE           *
 ************************************/

// POLA TE MA UZUPELNIAC FUNKCJA InitLibrary()

	ExecBase->LibNode.lib_NegSize	= (numfunc * sizeof(FUNCVEC));
	ExecBase->LibNode.lib_PosSize	= (ULONG) ((&LIBEND) - (&LIBSTART));
	ExecBase->LibNode.lib_Version	= EXEC_VERSION;
	ExecBase->LibNode.lib_Revision	= EXEC_REVISION;
	ExecBase->LibNode.lib_IdString	= (STRPTR) EXEC_NAME;

// ----------------------------------------------

	ExecBase->MemSize				= (0x100000 + extmem);
	ExecBase->LibNode.lib_Node.ln_Pri = 105;
	
	NewList(&ExecBase->LibList);
	NewList(&ExecBase->DeviceList);
	NewList(&ExecBase->PortList);
	NewList(&ExecBase->TaskReady);
	NewList(&ExecBase->TaskWait);
	NewList(&ExecBase->SemaphoreList);
	NewList(&ExecBase->ResidentList);
	
	/* dodanie exec'a do listy bibliotek */

	Enqueue(&ExecBase->LibList, (struct Node *) ExecBase);

/************************************
 *         BOOTUP INFO              *
 ************************************/
	
	{
		_printf("%40s: %i (%#X) kb\n",	"Physical memory size",
										SysBase->MemSize);

		_printf("%40s: %i (%#X) kb\n",	"Available memory",
										Avail(MEMF_ANY)/1024,
										Avail(MEMF_ANY)/1024);

		_printf("%40s: %i (%#X) kb\n",	"Largest free block",
										 Avail(MEMF_ANY|MEMF_LARGEST)/1024,
										 Avail(MEMF_ANY|MEMF_LARGEST)/1024);

	}
	{
		UBYTE c;
		_printf("\n\nTry to type some words:\n");
		while(1) {
			c=kgetc();
			_printf("%c",c);
		}
	}
	
	
	D(bug("Now we're going to sleep..."));
	_die();

/*
	_printf("TEST ODCZYTU PARAMETROW Z CMOS\n");
	_printf("------------------------------\n\n");
	{
		UBYTE type,cb;

		_printf("BaseMem (CMOS) kb: %X (%i)\n", exec_GetCMOSWord(0x15),
												exec_GetCMOSWord(0x15));
		_printf("ExtMem (CMOS) kb: %X (%i)\n", exec_GetCMOSWord(0x30),
											exec_GetCMOSWord(0x30));
											
		_printf("Checking HardDrives...\n");
		type=exec_GetCMOSByte(0x12) & 0xF0;
		type>>=4;
		if(type!=0xF)
			_printf("\tHD1: type %i\n",type);
		else {
			_printf("\tHD1: type %i\n",type=exec_GetCMOSByte(0x19));
			if(type==47) cb=0x20;
			else if(type==48) cb=0x35;
				else cb=0x0;
			
			if(cb) {
				_printf("\t\tCyls:\t%i\n"
						"\t\tHeads:\t%i\n"
						"\t\tSecPerTrack: %i\n",
						exec_GetCMOSWord(cb),
						exec_GetCMOSByte(cb+2),
						exec_GetCMOSByte(cb+7));
			}
		}
	}
*/

}



/*****************************************************************************
 **                            DEBUG                                        **
 *****************************************************************************

/*	debug ewentualnie przeniesc do innego modulu, dolaczac tylko w przypadku
	debugowania (na poczatku zawsze? :) */

void exec_setup_dbug()
{
	_cursx = 0;
	_cursy = 0;
	_tcols = EXEC_DEBUGCON_COLS;
	_trows = EXEC_DEBUGCON_ROWS;
	exec_dbug_clrscr();
}

/* boot() == preInit() */

ULONG exec_countfuncs()
{
	FUNCVEC *func;
	ULONG numfunc;
	func=exec_functable;
	numfunc=0;
		
	while((*func) != 0L) {
		numfunc++;
		func++;
	}
	return(numfunc);
}

	
ULONG exec_CountMem()
{
    ULONG i=1023;
    ULONG ba;

    _disable();
        
    ba = EXEC_EXTMEM_START-1024;
    
    do {
	ba+=1024;
	_pokel(LINEAR_DS,ba,MEMF_MAGICVAL);
	i++;
    } while(_peekl(LINEAR_DS,ba)==MEMF_MAGICVAL);

    _enable();
        
    return (i*1024);
}

void exec_ZeroMem(ULONG start, ULONG size)
{
	ULONG i,end=start+size;
	
	if(start%4==0 && size%4==0) {
		for(i=start;i<end;i+=4)
			_pokel(LINEAR_DS,i,0x0);
	} else {
		for(i=start;i<end;i++)
			_pokeb(LINEAR_DS,i,0x0);
	}
}

void exec_settimerfreq(ULONG hz)
{
    UBYTE crh,crl;
    UWORD ce;
    
    outportb(_8253_CONTROL_PORT, _8253_G0 | _8253_MODE3 | _8253_CE_RW | _8253_COUNTER_BINARY);
    ce=_8253_CLK/hz;
    outportb(_8253_COUNTER0_PORT, ce>>8);
    outportb(_8253_COUNTER0_PORT, (_u8) ce);
}

UBYTE exec_init8259a()
{
    outportb(_8259A_MASTERPORT1,ICW1_ICW4PRESENT|ICW1_WYZWALANIEPOZIOMEM|ICW1_SIGNATURE);
    outportb(_8259A_MASTERPORT2,0x20);
    outportb(_8259A_MASTERPORT2,0x04);
    outportb(_8259A_MASTERPORT2,ICW4_NORMALMODE);
    
    outportb(_8259A_SLAVEPORT1,ICW1_ICW4PRESENT|ICW1_WYZWALANIEPOZIOMEM|ICW1_SIGNATURE);
    outportb(_8259A_SLAVEPORT2,0x28);
    outportb(_8259A_SLAVEPORT2,0x02);
    outportb(_8259A_SLAVEPORT2,ICW4_NORMALMODE);

    exec_set8259aIMR(0xFF,0xFF); /* zadne z przerwan nie bedzie aktywne */
    
    return(1);
}

void exec_set8259aIMR(UBYTE master, UBYTE slave)
{
    outportb(_8259A_MASTERPORT2, master);
    outportb(_8259A_SLAVEPORT2, slave);
}

/*
void __NORETURN IRQ_Timer()
{
	SysBase->Elapsed++;
}
*/
