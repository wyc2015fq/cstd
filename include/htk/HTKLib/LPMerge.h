
/*      File: LPMerge:    LM interpolation                     */


/* !HVER!LPMerge:   3.4.1 [CUED 12/03/09] */

/* ------------------- Model interpolation  ----------------- */

#ifndef _LPMERGE_H
#define _LPMERGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LMODEL    32

typedef struct {
   char *fn;                /* LM filename */
   BackOffLM *lm;           /* the language model */
   float weight;            /* interpolation weight */
} LMInfo;

void InitPMerge(void);
/* 
   Initialise module 
*/

BackOffLM *MergeModels(MemHeap *heap, LMInfo *lmInfo, int nLModel, 
		       int nSize, WordMap *wList);
/*
   Interpolate models in lmInfo and return resulting model
*/

void NormaliseLM(BackOffLM *lm);
/* 
   Normalise probabilities and calculate back-off weights 
*/

/* -------------------- End of LPMerge.h ---------------------- */

#ifdef __cplusplus
}
#endif

#endif
