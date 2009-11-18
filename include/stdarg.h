#ifndef STDARG_H
#define	STDARG_H

#include <exec/types.h>

#define	STACKITEM	ULONG

typedef UBYTE *va_list;

#define	VA_SIZE(TYPE)					\
	((sizeof(TYPE) + sizeof(STACKITEM) - 1)	\
		& ~(sizeof(STACKITEM) - 1))

#define	va_start(AP, LASTARG)	\
	(AP=((va_list)&(LASTARG) + VA_SIZE(LASTARG)))

#define va_end(AP)	

#define va_arg(AP, TYPE)	\
	(AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))

#endif /* STDARG_H */

