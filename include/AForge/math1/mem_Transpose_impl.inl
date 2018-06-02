{
  const arrtype* a = (const arrtype*)a0;
  arrtype* b = (arrtype*)b0;
  al/=sizeof(a[0]);
  bl/=sizeof(b[0]);
  for (i = 0; i < h; i++) {
    for (j = 0; j < w; j++) {
      b[j*bl+i] = a[i*al+j];
    }
  }
}