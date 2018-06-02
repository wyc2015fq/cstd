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
  T* WR = (T*)malloc(sizeof(T)*n);
  T* WI = (T*)malloc(sizeof(T)*n);
  T *VL = NULL;
  int LDVL = 1;
  T *VR = v;
  int LDVR = n;
  int ILO;
  int IHI;
  T* SCALE = (T*)malloc(sizeof(T)*n);
  T ABNRM;
  T* RCONDE = (T*)malloc(sizeof(T)*n);
  T* RCONDV = (T*)malloc(sizeof(T)*n);
  int LWORK;
  T WORKSZE;
  int* IWORK = (int*)malloc(sizeof(int)*(2*n-2));
  int INFO;
  T* WORK;
  int i;
  
  LWORK = -1;
  Tgeevx( &BALANC, &JOBVL, &JOBVR, &SENSE, &N, a, &LDA, WR, WI,
    VL, &LDVL, VR, &LDVR, &ILO, &IHI, SCALE, &ABNRM,
    RCONDE, RCONDV, &WORKSZE, &LWORK, IWORK, &INFO,1,1,1,1 );
  
  LWORK = (int) WORKSZE;
  WORK = (T*)malloc(sizeof(T)*LWORK);
  Tgeevx( &BALANC, &JOBVL, &JOBVR, &SENSE, &N, a, &LDA, WR, WI,
    VL, &LDVL, VR, &LDVR, &ILO, &IHI, SCALE, &ABNRM,
    RCONDE, RCONDV, WORK, &LWORK, IWORK, &INFO,1,1,1,1 );
  for (i=0;i<N;i++) {
    d[2*i] = WR[i];
    d[2*i+1] = WI[i];
  }
  free(IWORK);
  free(WORK);
  free(WR);
  free(WI);
  free(SCALE);
  free(RCONDE);
  free(RCONDV);
}
#undef Tgeevx