/* Acquisition of a lock for read/write: int lock (int ldes1, int type, int priority) –  
This call is explained below (“Wait on locks with Priority”).
 */
#include <conf.h>
#include <kernel.h>
#include <lock.h>

int lock(int ldes1, int type, int priority) {
    // priority is any integer 
    STATWORD ps; 

    disable(ps);
    
    // if lock is deleted, then return SYSERR 
    if(isbadlock(ldes1) || lptr= &locks[ldes1])->lstate== DELETED) {
        restore(ps);
        return(SYSERR);
    }

    // if the lock has been deleted, return DELETE instead of OKAY
	if (sptr->sstate == SDELETED) {
		return(DELETED)
	}
	
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

    // lock can only be acquired if current is reader and process that wants to acquire is also a reader 
    // or there's no process currently using the lock then either a writer or a reader can acquire it

    
}