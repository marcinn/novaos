#ifndef NOVA_8253_H
#define NOVA_8253_H

#include <exec/types.h>

#define	_8253_COUNTER0_PORT		0x40
#define _8253_COUNTER1_PORT		0x41
#define _8253_COUNTER2_PORT		0x42
#define _8253_CONTROL_PORT		0x43

// generators 

#define _8253_G0		0
#define _8253_G1		64
#define _8253_G2		128

// operations

#define _8253_COPY_CE_2_CRHCRL	0
#define _8253_CRL_RW			16
#define _8253_CRH_RW			32
#define _8253_CE_RW				_8253_CRL_RW|_8253_CRH_RW

// mode

#define _8253_MODE0		0
#define _8253_MODE1		2
#define _8253_MODE2		4
#define _8253_MODE3		6
#define _8253_MODE4		8
#define _8253_MODE5		10

// counter

#define _8253_COUNTER_BINARY	0
#define _8253_COUNTER_BCD		1

#define	_8253_CLK		119318L

#endif
