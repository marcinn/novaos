#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <exec/types.h>

// STATEKEYS
#define	SK_ALT		1
#define	SK_CTRL		1<<1
#define	SK_SHIFT	1<<2
#define	SK_MENU		1<<3
#define	SK_WINLEFT	1<<4
#define	SK_WINRIGHT	1<<5
#define	SK_NUMLOCK	1<<6
#define	SK_CAPSLOCK	1<<7
#define	SK_SCROLLLOCK	1<<8
#define	SK_PAUSE	1<<9

// RAWKEYS

#define RK_ALT		56
#define	RK_CTRL		29
#define	RK_SHIFTL	42
#define	RK_SHIFTR	54
#define	RK_KEYUP	128

#define	KEYB_BUFFER_SIZE	16
#define	KEYB_REPEAT_SPEED	1
#define	KEYB_DELAY		1
#define KEYB_MAX_SCANCODE	200

typedef struct keyb_buf_s
{
    UBYTE ascii;
    UWORD rawkey;
} kbuf_t;

#define A_BACKSPACE	8
#define A_TAB		9
#define A_ENTER		10
#define A_ESC		1
#define	A_CTRL		0
#define	A_SHIFT		0
#define	A_PRTSCR	0
#define	A_ALT		0
#define A_CAPSLOCK	0
#define A_F1		0
#define A_PAUSE		0
#define A_SCRLOCK	0
#define A_LEFTARROW	0x2
#define A_RIGHTARROW	0x3

extern void exec_defkeybhandler();
extern void exec_InitKeyboard();
extern UBYTE kgetc();
extern UBYTE kgetcnw();
extern UBYTE krawkey();
extern void kbdTimerInt();

//#define KbdGetChar() kgetc()

// vars

extern kbuf_t keyb[];
extern __volatile__ UBYTE __lastkey;
extern __volatile__ UBYTE __rawkey;

#endif
