#include <conf.h>
#include <kernel.h>
#include <sem.h>
#include <lock.h>

extern int nextlock;

void linit() {
	nextlock = NLOCKS-1;

    for (i=0 ; i<NLOCKS ; i++) {	/* initialize locks */
		(lptr = &locks[i])->lstate = SFREE;
		lptr->lqtail = 1 + (lptr->lqhead = newqueue());
	}
}