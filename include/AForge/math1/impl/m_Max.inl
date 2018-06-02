{
  int i, j;
  int rows = asz.len[0], cols = asz.len[1];
  
  // Search down columns
  if (dimension == 1) {
    if (cols>0) {
      for (i=0; i<rows; ++i) {
        imax[i] = 0;
      }
      for (i = 0; i < rows; i++){
        for (j = 1; j < cols; j++){
          if (__COMP(a[i*al+j], a[i*al+imax[i]])){
            imax[i] = j;
          }
        }
      }
    } else {
      for (i=0; i<rows; ++i) {
        imax[i] = -1;
      }
    }
  } else {
    if (rows>0) {
      for (j = 0; j < cols; j++) {
        imax[j] = 0;
      }
      for (i = 1; i < rows; i++) {
        for (j = 0; j < cols; j++) {
          if (a[i*al+j] > a[imax[j]*al+j]){
            imax[j] = i;
          }
        }
      }
    } else {
      for (j = 0; j < cols; j++) {
        imax[j] = -1;
      }
    }
  }
}
