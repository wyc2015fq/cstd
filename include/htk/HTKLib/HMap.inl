

/*            File: HMap.c  - MAP Model Updates               */


/*
  Calculates the MAP estimate of the new model parameters ASSUMING
  that the stats associated with the updates have been stored
  (ie the forward-backward allignments have been performed).
*/

char *hmap_version = "!HVER!HMap: 3.4.1 [CUED 12/03/09]";
char *hmap_vc_id = "$Id: HMap.c,v 1.1.1.1 2006/10/11 09:54:57 jal58 Exp $";

#include <stdio.h>      /* Standard C Libraries */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>

#include "HShell.h"     /* HMM ToolKit Modules */
#include "HMem.h"
#include "HMath.h"
#include "HSigP.h"
#include "HWave.h"
#include "HAudio.h"
#include "HParm.h"
#include "HLabel.h"
#include "HModel.h"
#include "HTrain.h"
#include "HUtil.h"
#include "HAdapt.h"
#include "HFB.h"
#include "HMap.h"

#undef T_TOP
#undef T_ADT
#undef T_ACC
#undef T_TRE
#undef T_XFM
#undef T_SXF
#undef T_OBC
#undef T_SWP
#undef T_FRS

#define T_TOP   0001    /* Top level tracing */
#define T_UPD   0002    /* Model updates */

static int trace10     = 0;        /* Trace level */
static float minVar1  = 0.0;      /* minimum variance (diagonal only) */
static float mixWeightFloor1=0.0; /* Floor for mixture weights */
static Vector vFloor1[SMAX]; /* variance floor - default is all zero */
static int minEgs1    = 0;        /* min examples to train a model */
static float minObs  = 0;        /* min observations  to train a model */
static int maxM;
static int S;
static float mapTau     = 20.0;                /* Guides the MAP process */

static ConfParam *cParm10[MAXGLOBS];      /* config parameters */
static int nParm10 = 0;

/* EXPORT->InitMap: initialise configuration parameters */
void InitMap(void)
{
   int i;
   double f;

   Register(hmap_version,hmap_vc_id);
   nParm10 = GetConfig("HMAP", TRUE, cParm10, MAXGLOBS);
   if (nParm10>0){
     if (GetConfInt(cParm10,nParm10,"TRACE",&i)) trace10 = i;
     if (GetConfInt(cParm10,nParm10,"MINEGS",&i)) minEgs1 = i;
     if (GetConfFlt(cParm10,nParm10,"MINOBS",&f)) minObs = f;
     if (GetConfFlt(cParm10,nParm10,"MINVAR",&f)) minVar1 = f;
     if (GetConfFlt(cParm10,nParm10,"MAPTAU",&f)) mapTau = f;
     if (GetConfFlt(cParm10,nParm10,"MIXWEIGHTFLOOR",&f)) mixWeightFloor1 = MINMIX*f;
   }
}

/* --------------------------- Model Update --------------------- */

static int nFloorVar1 = 0;     /* # of floored variance comps */
static int nFloorVarMix1 = 0;  /* # of mix comps with floored vars */

/* FloorMixes1: apply floor to given mix set */
static void FloorMixes1(MixtureElem *mixes, int M, float floor)
{
   float sum,fsum,scale;
   MixtureElem *me;
   int m;
   
   sum = fsum = 0.0;
   for (m=1,me=mixes; m<=M; m++,me++) {
      if (me->weight>floor)
         sum += me->weight;
      else {
         fsum += floor; me->weight = floor;
      }
   }
   if (fsum>1.0) HError(2328,"FloorMixes1: Floor sum too large (%f)",fsum);
   if (fsum == 0.0) return;
   if (sum == 0.0) HError(2328,"FloorMixes1: No mixture weights above floor");
   scale = (1.0-fsum)/sum;
   for (m=1,me=mixes; m<=M; m++,me++)
      if (me->weight>floor) me->weight *= scale;
}

/* FloorTMMixes1: apply floor to given tied mix set */
static void FloorTMMixes1(Vector mixes, int M, float floor)
{
   float sum,fsum,scale,fltWt;
   int m;
   
   sum = fsum = 0.0;
   for (m=1; m<=M; m++) {
      fltWt = mixes[m];
      if (fltWt>floor)
         sum += fltWt;
      else {
         fsum += floor;
	 mixes[m] = floor;
      }
   }
   if (fsum>1.0) HError(2327,"FloorTMMixes1: Floor sum too large");
   if (fsum == 0.0) return;
   if (sum == 0.0) HError(2328,"FloorTMMixes1: No mixture weights above floor");
   scale = (1.0-fsum)/sum;
   for (m=1; m<=M; m++){
      fltWt = mixes[m];
      if (fltWt>floor)
	mixes[m] = fltWt*scale;
   }
}

/* FloorDProbs1: apply floor to given discrete prob set */
static void FloorDProbs1(ShortVec mixes, int M, float floor)
{
   float sum,fsum,scale,fltWt;
   int m;
   
   sum = fsum = 0.0;
   for (m=1; m<=M; m++) {
      fltWt = Short2DProb(mixes[m]);
      if (fltWt>floor)
         sum += fltWt;
      else {
         fsum += floor;
	 mixes[m] = DProb2Short(floor);
      }
   }
   if (fsum>1.0) HError(2327,"FloorDProbs1: Floor sum too large (%f)",fsum);
   if (fsum == 0.0) return;
   if (sum == 0.0) HError(2328,"FloorDProbs1: No probabilities above floor");
   scale = (1.0-fsum)/sum;
   for (m=1; m<=M; m++){
      fltWt = Short2DProb(mixes[m]);
      if (fltWt>floor)
	mixes[m] = DProb2Short(fltWt*scale);
   }
}

static void FloorMixtures(HSetKind hskind, StreamElem *ste, int M, float floor)
{
  switch (hskind){
  case DISCRETEHS:
    FloorDProbs1(ste->spdf.dpdf,M,floor);
    break;
  case TIEDHS:
    FloorTMMixes1(ste->spdf.tpdf,M,floor);
    break;
  case PLAINHS:
  case SHAREDHS:
    FloorMixes1(ste->spdf.cpdf+1,M,floor);
    break;
  }
}


/* UpdateWeights1: use acc values to calc new estimate of mix weights */
static void UpdateWeights1(HMMSet *hset, int px, HLink hmm)
{
   int i,s,m,M=0,N,vSize;
   float x,occi,denom,tmp;
   WtAcc *wa;
   StateElem *se;
   StreamElem *ste;
   MixtureElem *me;
   
   N = hmm->numStates;
   se = hmm->svec+2; 
   for (i=2; i<N; i++,se++){
      ste = se->info->pdf+1; 
      for (s=1;s<=S; s++,ste++){
	vSize = hset->swidth[s];
	wa = (WtAcc *)ste->hook;
	switch (hset->hsKind){
  	case TIEDHS:
	  M=hset->tmRecs[s].nMix;
	  break;
	case DISCRETEHS:
	case PLAINHS:
	case SHAREDHS:
	  M=ste->nMix;
	  break;
	}
	if (wa != NULL) {
	  occi = wa->occ; 
	  if (occi>0) {
	    me = ste->spdf.cpdf + 1; denom=0;
	    for (m=1; m<=M; m++,me++){
	      tmp = me->weight*vSize*mapTau -1;
	      if (tmp<0) tmp = 0;
	      denom += tmp;
	    }
	    me = ste->spdf.cpdf + 1;
	    for (m=1; m<=M; m++,me++){
	      tmp = me->weight*vSize*mapTau -1;
	      if (tmp<0) tmp = 0;
	      x = (tmp + wa->c[m])/(denom + occi); 
	      if (x>1.0){
		if (x>1.001)
		  HError(2393,"UpdateWeights1: Model %d[%s]: mix too big in %d.%d.%d %5.5f",
			 px,HMMPhysName(hset,hmm),i,s,m,x);
		x = 1.0; 
	      }
	      switch (hset->hsKind){
	      case TIEDHS:
		ste->spdf.tpdf[m] = x;
		break;
	      case DISCRETEHS:
		ste->spdf.dpdf[m]=DProb2Short(x);
		break;
	      case PLAINHS:
	      case SHAREDHS:
		me=ste->spdf.cpdf+m;
		me->weight = x;
		break;
	      }
	    }
	    if (mixWeightFloor1>0.0){
	      FloorMixtures(hset->hsKind,ste,M,mixWeightFloor1);		 
	    }
	    /* Force a normalisation becomes of weird zeroing .... */
	    if ((hset->hsKind == PLAINHS) || (hset->hsKind == SHAREDHS)) {
	      me = ste->spdf.cpdf + 1; x=0;
	      for (m=1; m<=M; m++,me++)
		x += me->weight;
	      if (x>1.001)
		HError(-1,"Updating Weights, sum too large (%f)\n",x);
	      me = ste->spdf.cpdf + 1;
	      for (m=1; m<=M; m++,me++)
		me->weight /= x;	      
	    } 
	  }
	  ste->hook = NULL;
	}
      }
   }
}
      
/* UpdateMeans1: use acc values to calc new estimate of means */
static int UpdateMeans1(HMMSet *hset, int px, HLink hmm)
{
   int i,s,m,k,M,N,vSize,nmapped;
   float occim;
   MuAcc *ma;
   StateElem *se;
   StreamElem *ste;
   MixtureElem *me;
   Vector mean;
   
   N = hmm->numStates; nmapped=0;
   se = hmm->svec+2; 
   for (i=2; i<N; i++,se++){
      ste = se->info->pdf+1; 
      for (s=1;s<=S;s++,ste++){
         vSize = hset->swidth[s];
         me = ste->spdf.cpdf + 1; M = ste->nMix;
         for (m=1;m<=M;m++,me++)
            if (MixWeight(hset,me->weight) > MINMIX){
               mean = me->mpdf->mean;
               ma = (MuAcc*)GetHook(mean);
               if (ma != NULL){
                  occim = ma->occ;
                  if (occim > 0.0) {
		    if (occim > minObs) nmapped++;
		    for (k=1; k<=vSize; k++)
		      mean[k] = ( mean[k] * mapTau + (ma->mu[k] + mean[k]*occim) )/( mapTau + occim );
                  } 
                  SetHook(mean,NULL);
               }
            }
      }
   }
   return(nmapped);
}

/* UpdateVars1: use acc values to calc new estimate of variances */
static void UpdateVars1(HMMSet *hset, int px, HLink hmm)
{
   int i,s,m,k,M,N,vSize;
   float occim,x,muDiffk,dmu;
   Vector minV;
   VaAcc *va;
   MuAcc *ma;
   StateElem *se;
   StreamElem *ste;
   MixtureElem *me;
   Vector mean,var;
   Covariance cov;
   Boolean mixFloored,shared;
   
   N = hmm->numStates;
   se = hmm->svec+2; 
   for (i=2; i<N; i++,se++){
      ste = se->info->pdf+1;
      for (s=1;s<=S;s++,ste++){
         vSize = hset->swidth[s];
         minV = vFloor1[s];
         me = ste->spdf.cpdf + 1; M = ste->nMix;
         for (m=1;m<=M;m++,me++)
	   if (MixWeight(hset,me->weight) > MINMIX){
               cov = me->mpdf->cov;
               va = (VaAcc*)GetHook(cov.var);
               mean = me->mpdf->mean;
               ma = (MuAcc*)GetHook(mean);
               if (va != NULL){
                  occim = va->occ;
                  mixFloored = FALSE;
                  if (occim > 0.0){
		    shared=(GetUse(cov.var)>1 || ma==NULL || ma->occ<=0.0);
                     if (me->mpdf->ckind==DIAGC) {
		         var = cov.var;
			 for (k=1; k<=vSize; k++){
			   if (shared) muDiffk = 0.0;
			   else {
			     dmu = (ma->mu[k])/(mapTau+occim);
			     muDiffk = 2*dmu*ma->mu[k] - dmu*dmu*occim;
			   }
                           x = (mapTau*var[k]  + va->cov.var[k] - muDiffk) / (mapTau + occim);
			   if (x<minV[k]) {
                             x = minV[k];
                              nFloorVar1++;
                              mixFloored = TRUE;
			    }
			   cov.var[k] = x;
			 }
       		     }
                     else { /* FULLC */
		       HError(999,"MAP estimation of full covariance matrices not supported");
		     }
		  }
                  if (mixFloored == TRUE) nFloorVarMix1++;
		  SetHook(cov.var,NULL);
               }
            }
      }
   }
}

static int TotMixInSet(HMMSet *hset)
{
   HMMScanState hss;
   HLink hmm;
   int nmix=0;

   NewHMMScan(hset,&hss);
   do {
     hmm = hss.hmm;
     while (GoNextState(&hss,TRUE)) {
       while (GoNextStream(&hss,TRUE)) {
	 if (hss.isCont)                     /* PLAINHS or SHAREDHS */
	   while (GoNextMix(&hss,TRUE)) {
	     if (!IsSeenV(hss.mp->mean)) {
	       nmix++;
	       TouchV(hss.mp->mean);
	     } 
	   }
       }
     }
   } while (GoNextHMM(&hss));
   EndHMMScan(&hss);
   return(nmix);
}

/* -------------------------- MAP code --------------------------------- */

/* EXPORT->MAPUpdateModels: update all models and save them in newDir if set,
   new files have newExt if set */
void MAPUpdateModels(HMMSet *hset, UPDSet uFlags)
{
  HMMScanState hss;
  HLink hmm;
  int px,n,nmapped=0,totM;

  if (hset->logWt == TRUE) HError(999,"HMap: requires linear weights");

  /* Intialise a few global variables */
  SetVFloor( hset, vFloor1, minVar1);
  maxM = MaxMixInSet(hset);
  totM = TotMixInSet(hset);
  S = hset->swidth[0];

  if (hset->hsKind == TIEDHS){ /* TIEDHS - update mu & var once per HMMSet */
    HError(999,"TIEDHS kind not currently supported in MAP estimation");
  }

  NewHMMScan(hset,&hss);
  px=1;
  do {   
    hmm = hss.hmm;
    n = (int)hmm->hook;
    if (n<minEgs1 && !(trace10&T_UPD))
      HError(-2331,"UpdateModels: %s[%d] copied: only %d egs\n",
	     HMMPhysName(hset,hmm),px,n);
    if (n>=minEgs1 && n>0) {
      if (uFlags & UPTRANS)
	HError(999,"No support for MAP updating transition probabilities");
      if (maxM>1 && uFlags & UPMIXES)
	UpdateWeights1(hset,px,hmm);
      if (hset->hsKind != TIEDHS){
	if (uFlags & UPVARS)
	  UpdateVars1(hset,px,hmm);
	if (uFlags & UPMEANS)
	  nmapped += UpdateMeans1(hset,px,hmm);
	if (uFlags & (UPMEANS|UPVARS))
	  FixGConsts(hmm);
      }  
    }
    px++;
  } while (GoNextHMM(&hss));
  EndHMMScan(&hss);
  if (trace10&T_TOP) {
    printf("Observed components (means) %d of %d: %.2f\n",nmapped,totM,100*(float)nmapped/(float)totM);
    if (nFloorVar1 > 0)
      printf("Total %d floored variance elements in %d different mixes\n",
	     nFloorVar1,nFloorVarMix1);
    fflush(stdout);
  }
}

#undef T_TOP
#undef T_ADT
#undef T_ACC
#undef T_TRE
#undef T_XFM
#undef T_SXF
#undef T_OBC
#undef T_SWP
#undef T_FRS
