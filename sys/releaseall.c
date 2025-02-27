/* Simultaneous release of multiple locks: int releaseall (int numlocks, int ldes1,…, int ldesN) */

int releaseall(int numlocks, long ldes) {
    a = (unsigned long *)(&ldes) + (numlocks-1); /* last argument	*/
	for ( ; numlocks > 0 ; numlocks--)	/* machine dependent; copy args	*/
		*--saddr = *a--;	/* onto created process' stack	*/
}