/*Destroy a lock: int ldelete (int lockdescriptor) – 
Deletes the lock identified by the descriptor lockdescriptor. 
(see “Lock Deletion” below)*/

#include <conf.h>
#include <kernel.h>
#include <lock.h>

int ldelete(int lockdescriptor) {
    STATWORD ps; 

    disable(ps);
    struct lentry *lptr;

    lptr->lstate = LDELETED; // delete the lock 

    // for each process that's waiting on the lock that's being deleted, it should receive DELETED after waking up
    int i; 
    int curr_process = lptr->lqhead;  
    while ()
    restore(ps);
    return(OK);
}