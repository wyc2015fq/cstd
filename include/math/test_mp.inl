#include "mp.inl"
#include "test_ntt.inl"
int test_mp()
{
  char buf[10000];
  mpz_t a[10] = {0};
  mpz_t* b = a + 1;
  mpz_t* c = a + 2;
  mpz_t* d = a + 3;
  mpz_t* e = a + 4;
  str_t s[1] = {0};
  int i, j, n = countof(buf) - 1;
  const char* ffmt = "%.4f ";
  for (i = 0; i < 1000; ++i) {
    const char* pp = "1000000000000000";
    printf("%4d ", i);
    if (0) {
      mpz_setstr(a, STR1("1"));
      mpz_shl(a, MPBITS * 11, a);
      mpz_setstr(a, STR1("2461941558251771232274"));
      mpz_mul(a, a, c);
      mpz_mul_kara(a, a, e);
      if (1) {
        printf("\n");
        mps_print(a->data, a->len);
        mps_print(c->data, c->len);
        mps_print(e->data, e->len);
      }
      mpz_adds(c, 1, c);
      {
        utime_start(_start_time);
        mpz_div(c, a, d, e);
        printf(ffmt, utime_elapsed(_start_time));
      }
      if (!(1 == e->len && 1 == e->data[0])) {
        mps_div(c->data, a->data, d->data, e->data, c->len, a->len);
      }
      ASSERT(1 == e->len && 1 == e->data[0]);
      ASSERT(0 == mpz_cmp(a, d));
    }
    if (0) {
      mpbase_t mask = MP_MASK;
      mpz_setstr(a, STR1("1"));
      mpz_shl(a, MPBITS * 2, a);
      mpz_subs(a, 1, a);
      for (j = 0; j < a->len; ++j) {
        ASSERT(a->data[j] == mask);
      }
    }
    if (1) {
      randstrnum(buf, n);
      //strcpy(buf, pp);
      {
        utime_start(_start_time);
        mpz_setstr(a, STR1(buf));
        printf(ffmt, utime_elapsed(_start_time));
      }
      mpz_shl(a, MPBITS - 2, b);
      mpz_shr(b, MPBITS - 2, c);
      ASSERT(0 == mpz_cmp(a, c));
      {
        utime_start(_start_time);
        mpz_tostr(a, s);
        printf(ffmt, utime_elapsed(_start_time));
      }
      ASSERT(0 == cstr_cmp(*s, STR1(buf), 1));
    }
    if (1) {
      mpz_shl(a, 1, b);
      mpz_add(a, a, c);
      mpz_sub(c, a, d);
      if (mpz_cmp(b, c)) {
        int k;
        for (k = 0; k < a->len; ++k) {
          printf("%0x  %0x  %0x\n", a->data[k], b->data[k], c->data[k]);
        }
        mps_add(a->data, a->data, d->data, a->len);
        k = 0;
      }
      ASSERT(0 == mpz_cmp(b, c));
      ASSERT(0 == mpz_cmp(a, d));
    }
    if (1) {
      mpz_shl(a, 2, b);
      mpz_sub_mul(b, a, 4, c);
      ASSERT(0 == c->len);
    }
    if (1) {
      mpz_clone(a, b);
      printf("%4d ", a->len);
      {
        utime_start(_start_time);
        mpz_mul(a, b, c);
        printf(ffmt, utime_elapsed(_start_time));
      }
      {
        utime_start(_start_time);
        mpz_mul_kara(a, b, e);
        printf(ffmt, utime_elapsed(_start_time));
      }
      if (0) {
        printf("\n");
        mpz_tostr(a, s);
        printf("%s\n", s->s);
        mpz_tostr(b, s);
        printf("%s\n", s->s);
        mpz_tostr(c, s);
        printf("%s\n", s->s);
        mpz_tostr(e, s);
        printf("%s\n", s->s);
      }
      mpz_adds(c, 1, c);
      {
        utime_start(_start_time);
        mpz_div(c, a, d, e);
        printf(ffmt, utime_elapsed(_start_time));
      }
      //{utime_start(_start_time);
      //e->len = bigu_div(c->len, c->data, a->len, a->data, &d->len, d->data, e->data);
      //printf(ffmt, utime_elapsed(_start_time));}
      if (!(1 == e->len && 1 == e->data[0])) {
        mps_div(c->data, a->data, d->data, e->data, c->len, a->len);
      }
      ASSERT(0 == mpz_cmp(b, d));
      ASSERT(1 == e->len && 1 == e->data[0]);
    }
    //printf("%s\n", buf);
    printf("\n");
  }
  mpz_frees(a, 10);
  str_free(s);
  return 0;
}

