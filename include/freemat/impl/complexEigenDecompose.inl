{
  T* a = (T*)A->data;
  T* d = (T*)D_out->data;
  T* v = V_out ? (T*)V_out->data : NULL;
  BOOL eigenvectors = TRUE;
  BOOL balance = balanceFlag;
  char BALANC = balance ? 'B' : 'N';
  
  char JOBVL = 'N';
  char JOBVR = eigenvectors ? 'V' : 'N';
  char SENSE = 'N';
  int N = n;
  int LDA = n;
  T *W = d;
  T *VL = NULL;
  int LDVL = 1;
  T *VR = v;
  int LDVR = n;
  int ILO;
  int IHI;
  T1* SCALE = (T1*)malloc(sizeof(T)*n);
  T1 ABNRM;
  T1* RCONDE = (T1*)malloc(sizeof(T)*n);
  T1* RCONDV = (T1*)malloc(sizeof(T)*n);
  int LWORK;
  T1* RWORK = (T1*)malloc(sizeof(T)*2*n);
  int INFO;
  T WORKSZE[2];
  T* WORK;
  LWORK = -1;
  Tgeevx( &BALANC, &JOBVL, &JOBVR, &SENSE, &N, a, &LDA, W,
    VL, &LDVL, VR, &LDVR, &ILO, &IHI, SCALE, &ABNRM,
    RCONDE, RCONDV, WORKSZE, &LWORK, RWORK, &INFO,1,1,1,1 );
  LWORK = (int)((T1*)WORKSZE)[0];
  WORK = (T*)malloc(sizeof(T)*2*LWORK);
  Tgeevx( &BALANC, &JOBVL, &JOBVR, &SENSE, &N, a, &LDA, W,
    VL, &LDVL, VR, &LDVR, &ILO, &IHI, SCALE, &ABNRM,
    RCONDE, RCONDV, WORK, &LWORK, RWORK, &INFO,1,1,1,1 );
  free(WORK);
  free(SCALE);
  free(RCONDE);
  free(RCONDV);
  free(RWORK);
}
#undef Tgeevx
