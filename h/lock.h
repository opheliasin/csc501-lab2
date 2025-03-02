#ifndef _LOCK_H_
#define _LOCK_H_

#define	LFREE	'\01'		/* this lock is free		*/
#define	LREAD	'\02'		/* this lock is read access		*/
#define LWRITE   '\03'	    /* this lock has write access		*/
#define LDELETED '\04'		/* this lock is deleted    */

#ifndef	NLOCKS
#define NLOCKS  50 /* number of locks allowed */
#endif

struct lentry { /* lock table entry*/
    char lstate; /* the state LFREE, LREAD, LWRITE, or LDELETED */
    int lcnt;   /* the count */
    int version; /* version of lock – increments every time a lock is recreated after being deleted */
    int lprio; /* max priority among all processes waiting in the lock's wait queue */

    /* linked list of the process ids of the processes currently holding the lock */
    
    int rqhead; /* q index of head of readers linked list */
    int rqtail; /* q index of tail of readers linked list */

    int wqhead; /* q index of head of writers linked list */
    int wqtail; /* q index of tail of writers linked list */
}

extern struct lentry locktab[];
extern int nextlock;

#define	isbadlock(l)	(l<0 || l>=NLOCKS)

void linit();

#endif