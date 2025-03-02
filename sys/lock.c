/* Acquisition of a lock for read/write: int lock (int ldes1, int type, int priority) –  
This call is explained below (“Wait on locks with Priority”).
 */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

int lock(int ldes1, int type, int priority) {
    // priority is any integer 
    STATWORD ps; 

    disable(ps);
    
    // if the process is trying to acquire has already been deleted, then return SYSERR 
    if(isbadlock(ldes1) || lptr= &locks[ldes1])->lstate== DELETED) {
        restore(ps);
        return(SYSERR);
    }

	// prevent process from acquiring a lock that's been recreated (same lock but different version)
	
	struct pentry *pptr;
	struct lentry *lptr = (&locktab[ldes1])

	// if it's a reader: 
	if (type == READ) {
		// if a writer has a lock or there's a higher or equal priority writer already waiting for the lock
		if ((lptr->lstate == LWRITE) || &proctab[lptr->wqhead]->pwait_prio >= priority) {
			// put in waiting queue
			(pptr = &proctab[currpid])->pstate = PRWAIT;
			lptr->lcnt = 

			resched();
			restore(ps);
			return ;
		}
	}
			// put in waiting queue
		// else:
			// the reader can acquire the lock
				// add process to the lock's priority queue if the process is currently holding the lock 
	// else: (it's a writer)
		// put in waiting queue 

	

	if (--(sptr->semcnt) < 0) {
		(pptr = &proctab[currpid])->pstate = PRWAIT;
		pptr->psem = sem;
		enqueue(currpid,sptr->sqtail);
		pptr->pwaitret = OK;
		resched();
		restore(ps);
		return pptr->pwaitret;
	}
	restore(ps);
	return(OK);
    
}