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

    a = (unsigned long *)(&ldes) + (numlocks-1); /* last argument*/

	int i;
	int lock;
	// struct qent *hptr = &q[pptr->lqhead]; 

	// if there aren't any locks stored in the process's linked list then we don't have to run more checks
	if ((&proctab[currpid])->locks == 0) {
		return(return_state);
	}

	int wptr;
	int rptr;
	int longestwait;
	int longestwaitproc;
	int wmaxpprio;

	// for each lock, check to see if lock is present in the process's queue
	for (i = 0; i < numlocks; i++)	{
		lock = *a--;

		// while the queue is not empty and (lock isn't in state LFREE or LDELETED)
		// we check to see if we can find the lock in the queue

		// Change to bitmask
		if ((pptr = &proctab[currpid])->locks & (1LL << lock)) {
			return_state = OK;
		} 
		// while (hptr != EMPTY && (locktab[lock]->lstate != LFREE || locktab[lock]->lstate != LDELETED)) { 
		// 	if (&hptr == lock) {
		// 		return_state = OK; // return OK instead if found 
		// 		break;
		// 	}
		// 	hptr = hptr->qnext;
		// }

		// TODO: just because one process is releasing the lock doesn't mean no other processes are holding it <- remember to check this 
		// default: highest priority process gets the lock

		// if it's SYSERR then we don't have to release it
		if (return_state == OK) {

			lptr = &locktab[lock];
			// remove process from the lock's linked list
			lptr->curr_mask &= ~(1LL << currpid);

			// remove lock from process's linked list
			pptr->locks &= ~(1LL << lock);

			// TODO: recalculate locktab[lock]->lprio 

			// TODO: change pinh (inherited priority) of 

			if (lptr->curr_mask == 0) { // if no process is holding the lock -> change state to LAVAIL
				lptr->lstate = LAVAIL;
			}
			// Check state of lock 
			if (lptr->lstate == LAVAIL) { 
				wptr = q[lptr->wqhead].qnext; // points to qnext of head of writer's queue
				rptr = q[lptr->rqhead].qnext; // points to qnext of head of reader's queue 
				int wpprio = q[wptr].qkey;
				int rpprio = q[rptr].qkey;
				// check to see whether READ or WRITE has higher priority 
				// if writer priority higher than reader priority 
				if (wpprio > rpprio) { //TODO: figure out syntax for this line
					longestwait = proctab[wptr].pwait_time; //get wait time for queue 
					longestwaitproc = wptr;
					while (q[wptr].qkey == wpprio) { // check if there are multiple writers with same priority - if so, run the one longest waiting time 
						if (proctab[wptr].pwait_time > longestwait) {
							longestwait = proctab[wptr].pwait_time; //not sure if proctab has the same index as q
							longestwaitproc = wptr;
						}
						wptr = q[wptr].qnext;
					}
					ready(dequeue(wptr), RESCHYES);
				}
				else if (wpprio < rpprio) { // if reader priority higher than writer priority 
					// check if there are multiple writers with same priority - if so, run the one longest waiting time 
					longestwait = proctab[rptr].pwait_time; //get wait time for queue
					longestwaitproc = rptr; 
					while (q[rptr].qkey == wpprio) { // check if there are multiple readers with same priority - if so, run the one longest waiting time 
						if (proctab[rptr].pwait_time > longestwait) {
							longestwait = proctab[rptr].pwait_time; //not sure if proctab has the same index as q
							longestwaitproc = rptr;
						}
						rptr = q[rptr].qnext;
					}
					ready(dequeue(rptr), RESCHYES);

				}  else { // else if wpprio = rpprio equal
					// if writer waiting time is at least 0.5 seconds longer than reader, then writer runs
					if (proctab[wptr].pwait_time - proctab[rptr].pwait_time >= 5000) {
						ready(getfirst(wptr), RESCHYES);
					} else {
						ready(getfirst(rptr), RESCHYES);
					}
				}				
			} 
			else if (lptr->lstate == LREAD) {
				// If other processes are still holding the lock for reading after the current process releases it, the lock remains in READ state. The lock will only be available when the last reader releases it. 

				// get writer with highest priority 
				wmaxpprio = q[q[(&locktab[lock])->wqhead].qnext].qkey;
				int rptr = q[(&locktab[lock])->rqhead].qnext; // change variable name and rewrite pointer logic

				while (q[rptr].qkey > wmaxpprio) { 
					ready(getfirst(lptr->rqhead), RESCHYES); 
					rptr = q[rptr].qnext; 
				} 

				restore(ps);
				return(OK);
			} 
		}
	}
	restore(ps);
	return(return_state);
}