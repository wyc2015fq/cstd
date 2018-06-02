{
  T* a = (T*)A->data;
  T* d = (T*)D_out->data;
  T* v = V_out ? (T*)V_out->data : NULL;
  int eigenvectors = FALSE;
  char JOBZ = eigenvectors ? 'V' : 'N';
  char UPLO = 'U';
  int N = n;
  int LDA = n;
  int INFO;
  T WORKSZE;
  int LWORK = -1;
  T* WORK;
  Tsyev(&JOBZ, &UPLO, &N, a, &LDA, d, &WORKSZE, &LWORK, &INFO, 1, 1);
  LWORK = (int) WORKSZE;
  WORK = (T*)malloc(sizeof(T)*LWORK);
  Tsyev(&JOBZ, &UPLO, &N, a, &LDA, d, WORK, &LWORK, &INFO, 1, 1);
  if (eigenvectors) {
    memcpy(v,a,n*n*sizeof(T));
  }
  free(WORK);
}
#undef Tsyev
