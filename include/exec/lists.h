#ifndef EXEC_LISTS_H
#define EXEC_LISTS_H

/***********************************************

    LISTS.H
    20.12.2002 (c) NovaTeam
    
    Definitions of Lists
    Author: Marcin Nowak
    
 **********************************************/


#include <exec/types.h>
#include <exec/nodes.h>

struct	List
{
    struct Node
		*lh_Head,
		*lh_Null,
		*lh_Tail;
    UBYTE	lh_Type;
    UBYTE	lh_pad;		// wyrownanie do parzystej wielkosci struktury
};

struct	MinList
{
    struct Node	*mlh_Head,
		*mlh_Tail,
		*mlh_TailPred;
};

#define	LH_MEMORYLIST			0
#define	LH_TASKLIST				1
#define	LH_LIBRARYLIST			2
#define	LH_DEVICELIST			3
		
#define	NewList(_l)												\
		do														\
		{														\
			struct List *list=(struct List *) (_l);				\
			list->lh_Head = (struct Node *) &list->lh_Null;		\
			list->lh_Null = (struct Node *) (0);				\
			list->lh_Tail = (struct Node *) list;				\
		} while(0)

#define	AddHead(list,head)				\
		do								\
		{								\
			struct List *_l=(struct List *) (list);		\
			struct Node *_n=(struct Node *) (head);		\
			_n->ln_Next = (struct Node *) _l->lh_Head;	\
			_n->ln_Prev	= (struct Node *) &_l->lh_Head;	\
			_l->lh_Head->ln_Prev 	= _n;				\
			_l->lh_Head				= _n;				\
		} while(0)

#define	AddTail(list,tail) 				\
		do								\
		{								\
			struct List *_l=(struct List *) (list);			\
			struct Node *_n=(struct Node *) (tail);			\
			_n->ln_Prev = (struct Node *) _l->lh_Tail;		\
			_n->ln_Next = (struct Node *) &_l->lh_Null;		\
			_l->lh_Tail->ln_Next = _n;						\
			_l->lh_Tail = (struct Node *) _n;				\
		} while(0)

#define Remove(_n)	                         \
    do                                        \
    {                                         \
	struct Node *n = (struct Node *)(_n); 	  \
        n->ln_Prev->ln_Next = n->ln_Next;     \
		n->ln_Next->ln_Prev = n->ln_Prev;     \
    } while (0)


#define	GetHead(_l)								\
		({										\
			struct List *l = (struct List *) (_l);	\
			l->lh_Head->ln_Next ? l->lh_Head : (struct Node *) 0; \
		})

#define	GetTail(_l)	\
		({	\
			struct List *l = (struct List *) (_l);	\
			l->lh_Tail->ln_Prev ? l->lh_Tail : (struct Node *) 0; \
		})

#define	GetNext(_n) \
		({	\
			struct Node *n = (struct Node *) (_n);	\
			n->ln_Next->ln_Next ? n->ln_Next : (struct Node *) 0;	\
		})

#define GetPrev(_n) \
		({	\
			struct Node *n = (struct Node *) (_n);	\
			n->ln_Prev->ln_Prev ? n->ln_Prev : (struct Node *) 0;	\
		})

#define ForeachNode(l,n)                        \
    for                                            \
    (                                              \
        n=(void *)(((struct List *)(l))->lh_Head); \
        ((struct Node *)(n))->ln_Next;             \
	n=(void *)(((struct Node *)(n))->ln_Next)  \
    )
			
#endif /* EXEC_LISTS_H */
