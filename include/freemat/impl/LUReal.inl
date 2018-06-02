if (nargout <= 2) {
  if (p != 0) {
    T* l_data = (T*)l->data, *u_data = (T*)u->data, *a_data = (T*)A->data;
    int info, i, j, lrows, lcols, urows, ucols;
    int* piv, *fullpivot;
    int p = MIN(nrows, ncols);
    piv = (int*) malloc(sizeof(int) * p);
    lapack_fn(&nrows, &ncols, a_data, &nrows, piv, &info);
    //  dgetrf_(&nrows,&ncols,a_data,&nrows,piv,&info);
    // Adjust the pivot vector
    fullpivot = FixupPivotVector(nrows, piv, p);
    
    if (nrows > ncols) {
      lrows = nrows;
      lcols = ncols;
      urows = ncols;
      ucols = ncols;
      
      // If A is tall (more rows than columns), then we fill in the
      // diagonal of L with 1s
      for (i = 0; i < lcols; i++) {
        l_data[fullpivot[i] + i * lrows] = 1.0;
      }
      
      // Then we copy the decomposition out of A (everything below
      // the main diagonal)
      for (i = 1; i < lrows; i++) {
        for (j = 0; j < MIN(i, lcols); j++) {
          l_data[fullpivot[i] + j * lrows] = a_data[i + j * nrows];
        }
      }
      // And we copy the U matrix out of A (everything above and
      // including the main diagonal).
      for (i = 0; i < urows; i++) {
        for (j = i; j < ucols; j++) {
          u_data[i + j * urows] = a_data[i + j * nrows];
        }
      }
    }
    else {
      lrows = nrows;
      lcols = nrows;
      urows = nrows;
      ucols = ncols;
      
      // If A is short (more columns than rows) then L is square
      // and U is short.  We fill in the main diagonal of L with
      // 1s.
      for (i = 0; i < lcols; i++) {
        l_data[fullpivot[i] + i * lrows] = 1.0;
      }
      
      // Then we copy the decomposition out of A (everything below
      // the main diagonal
      for (i = 1; i < lrows; i++) {
        for (j = 0; j < i; j++) {
          l_data[fullpivot[i] + j * lrows] = a_data[i + j * nrows];
        }
      }
      
      // Then we copy the remainder (upper trapezoid) out of A into
      // U
      for (i = 0; i < nrows; i++) {
        for (j = i; j < ncols; j++) {
          u_data[i + j * urows] = a_data[i + j * nrows];
        }
      }
    }
    
    free(fullpivot);
  }
}
else if (nargout == 3) {
  mxSetNumericMatrix(piv, nrows, nrows, A->classID, 0);
  
  if (p != 0) {
    T* l_data = (T*)l->data, *u_data = (T*)u->data, *a_data = (T*)A->data;
    T* pmat = (T*)piv->data;
    int info, i, j, lrows, lcols, urows, ucols;
    int* piv, *fullpivot;
    int p = MIN(nrows, ncols);
    piv = (int*) malloc(sizeof(int) * p);
    //  sgetrf(&nrows,&ncols,a_data,&nrows,piv,&info);
    lapack_fn(&nrows, &ncols, a_data, &nrows, piv, &info);
    fullpivot = FixupPivotVector(nrows, piv, p);
    
    for (i = 0; i < nrows; i++) {
      pmat[i + fullpivot[i]*nrows] = 1;
    }
    
    free(fullpivot);
    
    if (nrows > ncols) {
      lrows = nrows;
      lcols = ncols;
      urows = ncols;
      ucols = ncols;
      
      // If A is tall (more rows than columns), then we fill in the
      // diagonal of L with 1s
      for (i = 0; i < lcols; i++) {
        l_data[i + i * lrows] = 1.0;
      }
      
      // Then we copy the decomposition out of A (everything below
      // the main diagonal)
      for (i = 1; i < lrows; i++) {
        for (j = 0; j < MIN(i, lcols); j++) {
          l_data[i + j * lrows] = a_data[i + j * nrows];
        }
      }
      
      // And we copy the U matrix out of A (everything above and
      // including the main diagonal).
      for (i = 0; i < urows; i++) {
        for (j = i; j < ucols; j++) {
          u_data[i + j * urows] = a_data[i + j * nrows];
        }
      }
    }
    else {
      lrows = nrows;
      lcols = nrows;
      urows = nrows;
      ucols = ncols;
      
      // If A is short (more columns than rows) then L is square
      // and U is short.  We fill in the main diagonal of L with
      // 1s.
      for (i = 0; i < lcols; i++) {
        l_data[i + i * lrows] = 1.0;
      }
      
      // Then we copy the decomposition out of A (everything below
      // the main diagonal
      for (i = 1; i < lrows; i++) {
        for (j = 0; j < i; j++) {
          l_data[i + j * lrows] = a_data[i + j * nrows];
        }
      }
      
      // Then we copy the remainder (upper trapezoid) out of A into
      // U
      for (i = 0; i < nrows; i++) {
        for (j = i; j < ncols; j++) {
          u_data[i + j * urows] = a_data[i + j * nrows];
        }
      }
    }
  }
}


#undef lapack_fn
