{
  T* a = (T*)A->data;
  T1* d = (T1*)D_out->data;
  T* v = V ? (T*)V_out->data : NULL;
  int eigenvectors = FALSE;
  char JOBZ = eigenvectors ? 'V' : 'N';
  char UPLO = 'U';
  int N = n;
  int LDA = n;
  T1* RWORK = (T1*)malloc(sizeof(T1)*MAX(1,3*N-2));
  int LWORK;
  int INFO;
  T WORKSZE[2];
  T* WORK;
  LWORK = -1;
  Theev(&JOBZ,&UPLO,&N,a,&LDA,d,WORKSZE,&LWORK,RWORK,&INFO,1,1);
  LWORK = (int) ((T1*)WORKSZE)[0];
  WORK = (T*)malloc(sizeof(T)*LWORK);
  Theev(&JOBZ,&UPLO,&N,a,&LDA,d,WORK,&LWORK,RWORK,&INFO,1,1);
  if (eigenvectors) {
    memcpy(v,a,2*n*n*sizeof(T));
  }
  free(WORK);
  free(RWORK);
}
#undef Theev