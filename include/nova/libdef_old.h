#ifndef LIBDEFS_H
#define LIBDEFS_H

#include <exec/types.h>


#ifdef NOVA_REGPARAMS
#	define	REGPARMS(x)	__attribute__ ((regparm(x)))
#else
#	define	REGPARMS(x)
#endif

#ifdef NOVA_LIBCALL_PARAMS_BY_REGS

#define	ASMCALL(addr) 					\
		__asm__ __volatile__ (	"call *%0\n\t"	\
								: : "m" (addr))
#define GETRET(type)	({ type r; __asm__ __volatile__ ("movl %%eax,%0\n\t" \
															: "=m" (r));	 \
						   (type) r;										 \
						})
#define SETPARAMS2(___a, t1, ___b, t2)						\
{														\
	t1 __a; t2 __b; __a= (___a); __b = (___b);				\
		__asm__ __volatile__ (	"movl	%0,%%eax\n\t"	\
								"movl	%1,%%edx\n\t"	\
								:						\
								: "m" (__a),			\
								  "m" (__b)				\
							 );							\
}

#define LIBCALL(func,parms,rtype)	\
({									\
	rtype r;						\
	parms;							\
	ASMCALL(func);					\
	r=GETRET(rtype);				\
	(rtype) r;						\
})

#else

#define SETPATAMS1(__t1,__p1)			(__t1 __p1)
#define SETPARAMS2(__t1,__p1,__t2,__p2)	(__t1 __p1, __t2 __p2)
#define SETPARAMS3(__t1,__p1,__t2,__p2,__t3,__p3)	\
					(__t1 __p1, __t2 __p2, __t3 __p3)
#define SETPARAMS4(__t1,__p1,__t2,__p2,__t3,__p3,__t4,__p4) \
					(__t1 __p1, __t2 __p2, __t3 __p3, __t4 __p4)
#define	LIBCALL(func,parms,rtype)	rtype func parms


#endif /* NOVA_LIBCALL_PARAMS_BY_REGS

#define	__TEXT			__attribute__ ((section (".text")))
#define __NORETURN		__attribute__ ((noreturn))
#define	__REGPARM(x)	__attribute__ ((regparm(x)))
#define REGPARM0		__REGPARM(0)
#define	REGPARM1		__REGPARM(1)
#define	REGPARM2		__REGPARM(2)
#define	REGPARM3		__REGPARM(3)

#ifdef NOVA_LIBFUNC_PARAMS_BY_REGS
	#define	LIBFUNC0(return, name, b)				return REGPARM0 name(b)
	#define	LIBFUNC1(return, name, p1, b)			return REGPARM1 name(p1,b)
	#define LIBFUNC2(return, name, p1, p2, b) 		return REGPARM2 name(p1,p2,b)
	#define LIBFUNC3(return, name, p1, p2, p3, b)	return REGPARM3 name(p1,p2,p3,b)
#else
	#define	LIBFUNC0(return, name, b)				return name(b)
	#define	LIBFUNC1(return, name, p1, b)			return name(p1,b)
	#define LIBFUNC2(return, name, p1, p2, b) 		return name(p1,p2,b)
	#define LIBFUNC3(return, name, p1, p2, p3, b)	return name(p1,p2,p3,b)
#endif

typedef ULONG	FUNCVEC;			/* size of one entry in functable */

struct LibVec
{
	UBYTE	jmp_cmd;
	FUNCVEC	entry;
};

#define	LIBVEC_JMPCMD	0xE9

#define	FUNCTABLE_INIT(name)	const FUNCVEC name[]= {
#define	FUNCTABLE_ENTRY(func)	(FUNCVEC) &func,
#define	FUNCTABLE_END			};


#define	MAKE_ID_STR(ver,rev)	"#ver" ## ".#rev"

extern ULONG LIBEND;



#endif
