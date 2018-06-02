#ifndef _DISTANCE_H_
#define _DISTANCE_H_
// Õ≥º∆chi squareæ‡¿Î
#define CHI_SQUARE(_N, _A, _B, _D, TYPE) do {                                 \
    int _I=0;                                                                   \
    _D=0;                                                                       \
    for (; _I<=(_N-4); _I+=4) {                                                 \
      TYPE t0 = *(_A+_I  ) + *(_B+_I  ), t1 = *(_A+_I+1) + *(_B+_I+1);          \
      TYPE t2 = *(_A+_I+2) + *(_B+_I+2), t3 = *(_A+_I+3) + *(_B+_I+3);          \
      if (t0) { TYPE _t = *(_A+_I  ) - *(_B+_I  ); _D += (_t*_t)/ t0; }         \
      if (t1) { TYPE _t = *(_A+_I+1) - *(_B+_I+1); _D += (_t*_t)/ t1; }         \
      if (t2) { TYPE _t = *(_A+_I+2) - *(_B+_I+2); _D += (_t*_t)/ t2; }         \
      if (t3) { TYPE _t = *(_A+_I+3) - *(_B+_I+3); _D += (_t*_t)/ t3; }         \
    }                                                                           \
    for (; _I < _N; ++_I ) {                                                    \
      TYPE t0 = *(_A+_I  ) + *(_B+_I  );                                        \
      if (t0) { TYPE _t = *(_A+_I  ) - *(_B+_I  ); _D += (_t*_t)/ t0; }         \
    }                                                                           \
  } while(0)
// ≈∑ œæ‡¿Î (a-b)^2
#define EUCLID_DISTOR2(_N, _A, _B, _D, TYPE) do {                             \
    int _I;                                                                     \
    _D=0;                                                                       \
    for (_I = 0; _I <= (_N-4); _I += 4 ) {                                      \
      TYPE t0 = *(_A+_I) - *(_B+_I);                                            \
      TYPE t1 = *(_A+_I+1) - *(_B+_I+1);                                        \
      _D += t0*t0 + t1*t1;                                                      \
      t0 = *(_A+_I+2) - *(_B+_I+2);                                             \
      t1 = *(_A+_I+3) - *(_B+_I+3);                                             \
      _D += t0*t0 + t1*t1;                                                      \
    }                                                                           \
    for (; _I < _N; ++_I ) {                                                    \
      TYPE t = *(_A+_I) - *(_B+_I);                                             \
      _D += t*t;                                                                \
    }                                                                           \
  } while(0)
#define ABS_DISTOR2(_N, _A, _B, _D, TYPE) do {                                \
    int _I;                                                                     \
    _D=0;                                                                       \
    for(_I = 0; _I <= (_N-4); _I += 4 ) {                                       \
      TYPE t0 = ABSSUB(*(_A+_I), *(_B+_I));                                     \
      TYPE t1 = ABSSUB(*(_A+_I+1), *(_B+_I+1));                                 \
      _D += t0 + t1;                                                            \
      t0 = ABSSUB(*(_A+_I+2), *(_B+_I+2));                                      \
      t1 = ABSSUB(*(_A+_I+3), *(_B+_I+3));                                      \
      _D += t0 + t1;                                                            \
    }                                                                           \
    for( ; _I < _N; _I++ ) {                                                    \
      TYPE t0 = ABSSUB(*(_A+_I), *(_B+_I));                                     \
      _D += t0;                                                                 \
    }                                                                           \
  } while(0)
#define DOT(_N, _A, _B, _D, TYPE) do {                                        \
    int _I;                                                                     \
    _D=0;                                                                       \
    for (_I = 0; _I <= (_N-4); _I += 4 ) {                                      \
      _D += *(_A+_I) * *(_B+_I) + *(_A+_I+1) * *(_B+_I+1)                       \
          + *(_A+_I+2) * *(_B+_I+2) + *(_A+_I+3) * *(_B+_I+3);                \
    }                                                                           \
    for (; _I < _N; ++_I ) {                                                    \
      _D += *(_A+_I) * *(_B+_I);                                                \
    }                                                                           \
  } while(0)
#endif // _DISTANCE_H_

