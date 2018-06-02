

/*            File: HMap.c  - MAP Model Updates                */


void InitMap(void);
/* 
   Initialise configuration variables for the MAP adaptation
   library module.
*/

void MAPUpdateModels(HMMSet *hset, UPDSet uflags);
/*
  Using the accumulates obtained using FB perform Gauvain
  and Lee MAP update on the model parameters specified
  by uflags. 

  Note:
  1) MAP transition updates nit supported.
  2) TIEDHS model kind MAP updates not supported
*/



