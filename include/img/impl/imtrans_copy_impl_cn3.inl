{
  int i, j;
  for (i=0; i<h; ++i) {
    const arrtype* A0 = (const arrtype*)(A + i*al);
    for (j=0; j<w; ++j) {
      int j1 = ismirrx ? (h-1-j) : j;
      int i1 = ismirry ? (h-1-i) : i;
      int j2 = istrans ? i1 : j1;
      int i2 = istrans ? j1 : i1;
      const arrtype* a = (const arrtype*)(A0 + j*ai);
      arrtype* b = (arrtype*)(B + i2*bl + j2*bi);
      b[0] = a[0];
      b[1] = a[1];
      b[2] = a[2];
    }
  }
}
