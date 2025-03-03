/* Simultaneous release of multiple locks: int releaseall (int numlocks, int ldes1,…, int ldesN) */

/* also need to wake up processes */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * releaseall  --  signal a lock, releasing waiting process(es)
 *------------------------------------------------------------------------
 */

extern unsigned long ctr1000; 

int releaseall(int numlocks, long ldes) 
{	
	STATWORD ps;    
	disable(ps);
	struct	lentry	*lptr;
	struct pentry *pptr;
	int return_state = SYSERR; // if lock in argument isn't held by calling process – return(SYSERR)

	unsigned long *a; /* points to list of args	*/
	int release_locks[NLOCKS] = {0}; /* list of locks to be releasead */

    a = (unsigned long *)(&ldes) + (numlocks-1); /* last argument*/

	int i;
	int *lock;
	struct qent *hptr = &q[pptr->lqhead]; 

	// if there aren't any locks stored in the process's linked list then we don't have to run more checks
	if isempty(hptr) {
		return(return_state);
	}

	struct qent *wptr;
	struct qent *rptr;
	int longestwaitproc;

	// for each lock, check to see if lock is present in the process's queue
	for (i = 0; i < numlocks; i++)	{
		lock = *a--;

		// while the queue is not empty, we check to see if we can find the lock in the queue
		while (hptr != EMPTY) { 
			if (&hptr == lock) {
				return_state = OK; // return OK instead if found 
				break;
			}
			hptr = hptr->qnext;
		}

		// TODO: check to see whether READ or WRITE has higher priority 

		// TODO: just because one process is releasing the lock doesn't mean no other processes are holding it <- remember to check this 
		// default: highest priority process gets the lock

		lptr = &locktab[lock];

		// if writer priority higher than reader priority 
		if ((wptr = lptr->wqhead)->qkey > (rptr = lptr->rqhead)->qkey) {

			while () { // check if there are multiple writers with same priority - if so, run the one longest waiting time 
				if () {
					longestwaitproc = ; 
				}
			}
		}
		else if (wptr->qkey < rptr->qkey) { // if reader priority higher than writer priority 
			// check if there are multiple writers with same priority - if so, run the one longest waiting time 

		}  else { // else if it's equal
			// if writer waiting time is at least 0.5 seconds longer than reader, then writer runs
			if () {
				ready(getfirst(lptr->wqhead));
			} else {
				ready(getfirst(lptr->rqhead));
			}
		}
					
	}
	restore(ps);
	return(return_state);
}