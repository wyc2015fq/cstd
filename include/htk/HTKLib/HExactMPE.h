

/*         File: HExactMPE.h   MPE implementation (exact)      */


/* !HVER!HExactMPE:   3.4.1 [CUED 12/03/09] */


/* A (rather long) routine called from HFBLat.c, relating to the 
   exact implementation of MPE.
*/
   

#define SUPPORT_EXACT_CORRECTNESS


void InitExactMPE(void); /* set configs. */


#ifdef SUPPORT_EXACT_CORRECTNESS
void DoExactCorrectness(FBLatInfo *fbInfo, Lattice *lat);   
#endif

