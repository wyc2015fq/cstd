{
  int M = mxGetM(A);
  int N = mxGetN(A);
  int LDA = M, INFO, IWORKSIZE;
  T WORKSIZE[2] = {0};
  mxArray ttt[5] = {0};
  mxArray* IPIV = ttt+0;
  mxArray* WORK = ttt+1;
  mxCopyArray(B, A);
  mxSetNumericMatrix(IPIV, MIN(M, N), 1, mxINT32_CLASS, 0);
  // Do the decomposition
  getrf(&M, &N, (T*)B->data, &LDA, (int*)IPIV->data, &INFO);
  // Compute the inverse
  IWORKSIZE = -1;
  getri(&N, (T*)B->data, &LDA, (int*)IPIV->data, WORKSIZE, &IWORKSIZE, &INFO);
  IWORKSIZE = (int)(WORKSIZE[0].r);
  mxSetNumericMatrix(WORK, IWORKSIZE, 1, A->classID, 0);
  getri(&N, (T*)B->data, &LDA, (int*)IPIV->data, (T*)WORK->data, &IWORKSIZE, &INFO);

  if (INFO > 0) {
    mxFillArray(B, Inf(), 0);
  }
}
#undef getrf
#undef getri