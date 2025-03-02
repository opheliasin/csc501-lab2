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
		pptr->plockret = SYSERR;
        return(pptr->plockret);
    }

	// prevent process from acquiring a lock that's been recreated (same lock but different version)
	
	struct pentry *pptr = &proctab[currpid];
	struct lentry *lptr = &locktab[ldes1];

	// check to see which process is the first reader in waiting queue 
	int first_writer = lptr->wqhead;

	// if it's a reader and a writer has a lock or there's a higher or equal priority writer already waiting for the lock
	if (type == READ && (lptr->lstate == LWRITE || (&proctab[(lptr->wqhead).qnext]->pwait_prio >= priority))) {
			pptr->pstate = PRWAIT;
			pptr->lockid = ldes1;
			insert(currpid, int head, priority);  // TODO: put process in waiting queue
			pptr->plockret = OK;
			resched(); // switch to another process 
			restore(ps);
			return pptr->plockret;
	} else if ((lptr->lstate != LFREE && type == WRITE)) {
			pptr->pstate = PRWAIT;
			pptr->lockid = ldes1;
			insert(currpid, int head, priority); //TODO: put process in waiting queue (need to determine which one)
			pptr->plockret = OK;
			resched();
			restore(ps);
			return pptr->plockret;
	} 

	// TODO: all readers with higher priority than the highest priority writer should also be admitted 

	struct qent *rptr = &q[lptr->rqhead];
	struct qent *wptr = &q[lptr->wqhead].qnext;
	if (type == READ) {
		// loop through lock's reader's waiting queue 
		while (qptr->qkey > wptr->qkey) {
			ready(getfirst())
		}
	}

	enqueue(ldes1, pptr->lqtail); // TODO: add lock to the process' queue
	lptr->lstate = type; // set type of locking to READ
	restore(ps);
	return(OK);
    
}