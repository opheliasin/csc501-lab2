/*Create a lock:  int lcreate (void) – Creates a lock and returns a lock descriptor that can be used in further calls to refer to this lock. This call should return SYSERR if there are no available entries in the lock table.  */
#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <lock.h>
#include <stdio.h>

LOCAL int newlock();

/*------------------------------------------------------------------------
 * screate  --  create and initialize a lock, returning its id
 *------------------------------------------------------------------------
 */
int lcreate() {
    STATWORD ps;    
	int	lock;

	disable(ps);
	if ( count<0 || (lock=newlock())==SYSERR ) {
		restore(ps);
		return(SYSERR);
	}

	restore(ps);
	return(lock);
}

/*------------------------------------------------------------------------
 * newlock  --  allocate an unused lock and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newlock()
{
	int	lock;
	int	i;

	for (i=0 ; i<NLOCKS ; i++) {
		lock=nextlock--;
		if (nextlock < 0)
			nextlock = NLOCKS-1;
		if (locks[lock].lstate==LFREE) {
			locks[lock].lstate = LUSED;
			return(lock);
		}
	}
	return(SYSERR);
}