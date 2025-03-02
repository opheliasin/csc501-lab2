/* Simultaneous release of multiple locks: int releaseall (int numlocks, int ldes1,…, int ldesN) */

/* also need to wake up processes */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int releaseall(int numlocks, long ldes) 
{	
	STATWORD ps;    
	struct	lentry	*lptr;
	struct pentry *pptr;
	int return_state = SYSERR; // if lock in argument isn't held by calling process – return(SYSERR)

	disable(ps);
	unsigned long *a; /* points to list of args	*/
	int release_locks[NLOCKS] = {0}; /* list of locks to be releasead */

    a = (unsigned long *)(&ldes) + (numlocks-1); /* last argument*/

	int i;
	int *lock;

	for (i = 0; i < numlocks; i++)	{
		release_locks[*a--] = 1;	/* onto release_locks array */
		lock = *a--;

		// while the queue is not empty, we check to see if we can find the lock in the queue
		struct qent *hptr = &q[pptr->lqhead]; 
		if (nonempty(hptr)) {
			while (hptr != EMPTY) { 
				if (&hptr == lock) {
					return_state = OK; // return OK instead if found 
					break;
				}
				hptr = hptr->qnext;
			} 
		}
		// release one waiting process 
		if ((lptr->lcnt++) < 0) {
			// we check to see whether READ or WRITE has higher priority 
			
		}
	}
	restore(ps);
	return(return_state);
}