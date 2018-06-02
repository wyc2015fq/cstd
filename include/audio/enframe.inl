//ENFRAME split signal up into (overlapping) frames: one per row. F=(X,WIN,INC)
//
// F = ENFRAME(X,LEN) splits the vector X up into
// frames. Each frame is of length LEN and occupies
// one row of the output matrix. The last few frames of X
// will be ignored if its length is not divisible by LEN.
// It is an error if X is shorter than LEN.
//
// F = ENFRAME(X,LEN,INC) has frames beginning at increments of INC
// The centre of frame I is X((I-1)*INC+(LEN+1)/2) for I=1,2,...
// The number of frames is fix((length(X)-LEN+INC)/INC)
//
// F = ENFRAME(X,WINDOW) or ENFRAME(X,WINDOW,INC) multiplies
// each frame by WINDOW(:)
// Copyright (C) Mike Brookes 1997
// Version: $Id: enframe.m,v 1.3 2005/02/21 15:22:12 dmb Exp $
//
// VOICEBOX is a MATLAB toolbox for speech processing.
// Home page: http://www.ee.ic.ac.uk/hp/staff/dmb/voicebox/voicebox.html
int enframe(int nx, const double* x, int nwin, double* win, int inc, img_t* f)
{
  int i, j, len = nwin, nf;
  if (inc <= 0) {
    inc = len;
  }
  ASSERT(len > 0);
  nf = ((nx - len + inc) / inc);
  imsetsize_f8(f, nf, len, 1);
  for (i = 0; i < nf; ++i) {
    MEMCPY(f->tt.f8 + i * len, x + i * inc, len);
  }
  if (win) {
    for (i = 0; i < nf; ++i) {
      double* f1 = f->tt.f8 + i * len;
      for (j = 0; j < nwin; ++j) {
        f1[j] *= win[j];
      }
    }
  }
  return 0;
}

