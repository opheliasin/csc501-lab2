/* Acquisition of a lock for read/write: int lock (int ldes1, int type, int priority) –  
This call is explained below (“Wait on locks with Priority”).
 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

extern unsigned long ctr1000; 

int lock(int ldes1, int type, int priority) {
    // priority can be any integer 
    STATWORD ps; 
    disable(ps);
	struct lentry *lptr;
	struct pentry *pptr = &proctab[currpid];

    // if the process is trying to acquire has already been deleted, then return SYSERR
	// prevent process from acquiring a lock that's been recreated:
	//	solution: check if the lock was created before or after the lock was requested by the process

    if (isbadlock(ldes1) || (lptr = &locktab[ldes1])->lstate == LDELETED || lptr->lcreatetime > pptr->pwait_time) {
        restore(ps);
		pptr->plockret = SYSERR;
        return(pptr->plockret);
    }
	
	struct qent *qptr = &q[lptr->wqhead];
	int wmaxpprio; // TODO: check if this works

	if (lptr->lstate == LFREE) { //if current state of lock is free
		//then process can acquire it regardless of type 

		// add process to lock's queue // TODO change
		// enqueue(currpid, lptr->hqhead);
		kprintf("Initial lock mask: %lld\n", lptr->curr_mask);
    
		lptr->curr_mask |= (1LL << currpid);  // Set bit ldes1 to 1
		
		kprintf("Updated lock mask: %lld\n", lptr->curr_mask);
    
		// add lock to the process' queue //TODO change
		// enqueue(ldes1, pptr->lqtail); 
		kprintf("Initial lock mask: %lld\n", pptr->locks);
    
		// Add (set) the 3rd lock (bit position 3)
		pptr->locks |= (1LL << ldes1);  // Set bit ldes1 to 1
		
		kprintf("Updated lock mask: %lld\n", pptr->locks);
	
		lptr->lstate = type; // set type of locking to the default type
		restore(ps);
		return(OK);
	}
	else if (lptr->lstate == LREAD) { 
		if (type == READ) {
			// if there's a higher or equal priority writer already waiting for the lock
			if (wmaxpprio = q[qptr->qnext].qkey >= priority) {
				pptr->pstate = PRWAIT;
				pptr->lockid = ldes1;
				insert(currpid, lptr->rqhead, priority); 
				pptr->pwait_time = ctr1000; // set start time in wait queue 
				pptr->plockret = OK;
				resched(); // switch to another process 

				if (lptr->lstate == LDELETED) {  
					pptr->pwaitret = DELETED;
				}

				restore(ps);
				return pptr->plockret;
			} else {
				
				// all readers with higher priority than the highest priority writer should also be admitted 
				// TODO: change to bitmask implementation
				int rptr = q[lptr->rqhead].qnext; 

				while (q[rptr].qkey > wmaxpprio) { 
					ready(getfirst(lptr->rqhead), RESCHYES); 
					rptr = q[rptr].qnext; 
				}
				
				restore(ps);
				return(OK);
			}

		} else if (type == WRITE) {
			pptr->pstate = PRWAIT;
			pptr->lockid = ldes1;
			insert(currpid, lptr->wqhead, priority);
			pptr->pwait_time = ctr1000;
			pptr->plockret = OK;
			resched(); // switch to another process 

			if (lptr->lstate == LDELETED) {  
				pptr->pwaitret = DELETED;
			}

			restore(ps);
			return pptr->plockret;
		} else {
			restore(ps);
			return(SYSERR);
		}
	} 
	else if (lptr->lstate == LWRITE) {
		pptr->pstate = PRWAIT;
		pptr->lockid = ldes1;
		if (type == READ) {
			insert(currpid, lptr->rqhead, priority);  
		} else if (type == WRITE) {
			insert(currpid, lptr->wqhead, priority);
		} else {
			restore(ps);
			return(SYSERR);
		}
		pptr->plockret = OK;
		pptr->pwait_time = ctr1000;
		resched(); // switch to another process 

		if (lptr->lstate == LDELETED) {  
			pptr->pwaitret = DELETED;
		}
		restore(ps);
		return pptr->plockret;	
	} 
	else {
		restore(ps);
		return(SYSERR);
	}
}