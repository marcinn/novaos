#ifndef LIBDEFS_H
#define LIBDEFS_H

#include <exec/types.h>
#include <exec/libraries.h>
#include <system.h>

#undef GENERAL_REGS
#define GENERAL_REGS	ALL_REGS

#define LIBGP_START	__asm__ __volatile__ (
#define	LIBGP_END(var) 	"movl %%eax,%0" : "=m" (var))

/*	LIB GET PARAM */
#define LGP0(var)							\
						LIBGP_START					\
						"movl 8(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP1(var)							\
						LIBGP_START					\
						"movl 12(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP2(var)							\
						LIBGP_START					\
						"movl 16(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP3(var)							\
						LIBGP_START					\
						"movl 20(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP4(var)							\
						LIBGP_START					\
						"movl 24(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP5(var)							\
						LIBGP_START					\
						"movl 28(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP6(var)							\
						LIBGP_START					\
						"movl 32(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP7(var)							\
						LIBGP_START					\
						"movl 36(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP8(var)							\
						LIBGP_START					\
						"movl 40(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)
#define LGP9(var)							\
						LIBGP_START					\
						"movl 44(%%ebp),%%eax\n\t"	\
						LIBGP_END(var)

#define PUSHA	__asm__ __volatile__ ("pushl %ebx\n\t"	\
						"pushl %ecx\n\t"	\
						"pushl %edx\n\t"	\
						"pushl %esi\n\t"	\
						"pushl %edi\n\t")
						
#define POPA	__asm__ __volatile__ ("popl %edi\n\t"		\
						"popl %esi\n\t"		\
						"popl %edx\n\t"		\
						"popl %ecx\n\t"		\
						"popl %ebx\n\t")


#define LIBFUNCEND		}

#define	LIBGETPARAMS1(__p1)	\
						LGP0(__p1)
#define	LIBGETPARAMS2(__p1,__p2) \
						LGP0(__p1); LGP1(__p2)
#define	LIBGETPARAMS3(__p1,__p2,__p3) \
						LGP0(__p1); LGP1(__p2); LGP2(__p3)
#define	LIBGETPARAMS4(__p1,__p2,__p3,__p4) \
						LGP0(__p1); LGP1(__p2); LGP2(__p3); LGP3(__p4)
#define	LIBGETPARAMS5(__p1,__p2,__p3,__p4,__p5) \
						LGP0(__p1); LGP1(__p2); LGP2(__p3); LGP3(__p4);	\
						LGP4(__p5)

/*
#define LIBFUNCHEAD(return, name, bt, base)		\
					__asm__ __volatile__ (".globl "#name"\n\t"	\
										".type\t"#name",@function\n\t");	\
						 PUSHA;	\
						 __asm__ __volatile__ ("pushl %ebp\n\t"	
						 					"movl %esp,%ebp\n\t");{	\
						 bt base
*/
#define LIBFUNCHEAD(return, name, bt, base)	\
					return name() {	\
					bt base
										
#define LIBFUNC0(return, name, bt, base)						\
						LIBFUNCHEAD(return, name, bt, base);	\
						LIBGETPARAMS1(base);
#define LIBFUNC1(return, name, t1,p1, bt,base)			\
						LIBFUNCHEAD(return, name, bt,base);	\
						t1 p1;	\
						LIBGETPARAMS2(base,p1);
#define LIBFUNC2(return, name, t1,p1, t2,p2, bt,base)			\
						LIBFUNCHEAD(return, name, bt,base);	\
						t1 p1;	t2 p2;						\
						LIBGETPARAMS3(base, p1, p2);
#define LIBFUNC3(return, name, t1,p1, t2,p2, t3,p3, bt,base)	\
						return name(t1 p1, t2 p2, t3 p3) {	\
						bt base;	\
						LIBGETPARAMS4(base,p1,p2,p3);
#define LIBFUNC4(return, name, t1,p1, t2,p2, t3,p3, t4,p4, bt,base)	\
						LIBFUNCHEAD(return, name, bt,base);	\
						t1 p1; t2 p2; t3 p3; t4 p4;			\
						LIBGETPARAMS5(base,p1,p2,p3,p4);

#define	PUSHPARAM(__p)	__asm__ __volatile__ ("pushl %0\n\t":: "m" (__p))


#define _CFH(___base, ___offs)	\
	register struct Library *__##___base##_b=(struct Library *) (___base)


#define _CFHNR(___base, ___offs)	\
	struct Library *__##___base##_b=(struct Library *) (___base)

#define _ASMCALL(___rel,___sc)	\
	__asm__ __volatile__ ("movl %0,%%eax\n\t"		\
			"call *%%eax\n\t"		\
			"addl $"#___sc",%%esp\n\t"	\
			:			\
			: "r" ((___rel)))

#define	_GETRET(__ret__)	__asm__ __volatile__ ("mov %%eax,%0": "=m" ((__ret__)))

#define	CF0(__rt,__offs, __base)	\
({	\
	_CFH(__base,__offs);			\
	APTR __fc=(APTR) (((ULONG) (__##__base##_b))-(__offs));	\
	__rt __ret;	\
	PUSHPARAM(__base);				\
	_ASMCALL(__fc,4);		\
	_GETRET(__ret);			\
	(__rt) __ret;					\
})
#define	CF0NR(__offs, __base)	\
({	\
	_CFHNR(__base,__offs);			\
	APTR __fc=(APTR) (((ULONG) (__##__base##_b))-(__offs));	\
	PUSHPARAM(__base);				\
	_ASMCALL(__fc,4);		\
})

#define	CF1(__rt,__offs, __t1,__p1, __base)	\
({	\
	{	\
	__t1 __pp1 = (__p1);		\
	struct Library *___base=(struct Library *) (__base);	\
	__rt __ret;	\
	{	\
	__asm__  (	\
						"subl $8,%%esp\n\t"	\
						"pushl %2\n\t"	\
						"pushl %3\n\t"	\
						"movl %1,%%eax\n\t"	\
						"call *%%eax\n\t"			\
						"addl $16,%%esp\n\t"	\
						"movl %%eax,%0\n\t"			\
						: "=g" (__ret)				\
						: "r" (((ULONG) ___base)-__offs),	\
						"g" ((ULONG) __pp1), "g" ((ULONG) ___base) \
						: "eax","ebx","ecx","edx");	\
	}\
	(__rt) __ret;					\
	}\
})

#define	CF1NR(__offs, __t1,__p1, __base)	\
({	\
	__t1 __pp1 = (__p1);		\
	struct Library *___base=(struct Library *) (__base);	\
	{	\
	__asm__  (	\
						"subl $8,%%esp\n\t"	\
						"pushl %1\n\t"	\
						"pushl %2\n\t"	\
						"movl %0,%%eax\n\t"	\
						"call *%%eax\n\t"			\
						"addl $16,%%esp\n\t"	\
						:	\
						: "r" (((ULONG) ___base)-__offs),	\
						"g" ((ULONG) __pp1), "g" ((ULONG) ___base) \
						: "eax","ebx","ecx","edx");	\
	}\
})

#define	CF2(__rt,__offs, __t1,__p1, __t2,__p2, __base)	\
({	\
	__t1 __pp1 = (__p1);		\
	__t2 __pp2 = (__p2);		\
	struct Library *___base=(struct Library *) (__base);	\
	__rt __ret;	\
	__asm__  (	\
						"subl $12,%%esp\n\t"	\
						"pushl %2\n\t"	\
						"pushl %3\n\t"	\
						"pushl %4\n\t" \
						"movl %1,%%eax\n\t"	\
						"call *%%eax\n\t"			\
						"addl $20,%%esp\n\t"	\
						"movl %%eax,%0\n\t"			\
						: "=g" (__ret)				\
						: "r" (((ULONG) ___base)-__offs),	\
						"g" (__pp2),	\
						"g" ((ULONG) __pp1), \
						"g" ((ULONG) ___base) \
						: "eax","ebx","ecx","edx");	\
	(__rt) __ret;					\
})

#define	CF2NR(__offs, __t1,__p1, __t2,__p2, __base)	\
({	\
	__t1 __pp1 = (__p1);		\
	__t2 __pp2 = (__p2);		\
	struct Library *___base=(struct Library *) (__base);	\
	__asm__  (	\
						"subl $12,%%esp\n\t"	\
						"pushl %1\n\t"	\
						"pushl %2\n\t"	\
						"pushl %3\n\t" \
						"movl %0,%%eax\n\t"	\
						"call *%%eax\n\t"			\
						"addl $20,%%esp\n\t"	\
						: 			\
						: "r" (((ULONG) ___base)-__offs),	\
						"g" (__pp2),	\
						"g" ((ULONG) __pp1), \
						"g" ((ULONG) ___base) \
						: "eax","ebx","ecx","edx");	\
})
	
#define	CALLFUNC(_rt,__offs,__base,__pn)	\
({ \
						struct Library *__libbase=(struct Library *) __base; \
						_rt __ret;	\
						APTR __func=(APTR) (((ULONG) (__libbase))-__offs);	\
						_printf("jumping to: %#X\n",__func);			\
						__asm__("movl %1,%%eax\n\t"	\
								"call *-"#__offs"(%%eax)\n\t"	\
								"addl "#__pn", %%esp\n\t"	\
								"movl %%eax,%0\n\t"			\
								: "=m" (__ret)				\
								: "m" (&__base));				\
						(_rt) __ret;						\
})

#define	CALLFUNCNR(__offs,__base,__pn)	\
({ \
						struct Library *__libbase=(struct Library *) __base; \
						APTR __func=(APTR) (((ULONG) (__libbase))-__offs);	\
						__asm__("call *%0":: "r" (__func));	\
						__asm__("addl %0, %%esp\n\t"		\
								:: "m" (__pn)); 				\
})

typedef ULONG	FUNCVEC;			/* size of one entry in functable */

struct LibVec
{
	UWORD	jmp_cmd __PACKED;
	FUNCVEC	entry __PACKED;
} __PACKED;

//#define	LIBVEC_JMPCMD	0xFF25		/* jmp near */
#define	LIBVEC_JMPCMD	0xE990

#define	FUNCTABLE_INIT(name)	const FUNCVEC name[]= {
#define	FUNCTABLE_ENTRY(func)	(FUNCVEC) &func,
#define	FUNCTABLE_END			};


#define	MAKE_ID_STR(ver,rev)	"#ver" ## ".#rev"

extern ULONG LIBEND;



#endif
