
static void xtoa_c(unsigned long val, char *buf, unsigned radix, int is_neg) {
  char *p;        /* pointer to traverse string */
  char *firstdig;     /* pointer to first digit */
  char temp;       /* temp char */
  unsigned digval;    /* value of digit */

  p = buf;

  if (is_neg) {
    /* negative, so output '-' and negate */
    *p++ = '-';
    val = (unsigned long)(-(long)val);
  }

  firstdig = p;      /* save pointer to first digit */

  do {
    digval = (unsigned)(val % radix);
    val /= radix;    /* get next digit */

              /* convert to ascii and store */
    if (digval > 9)
      *p++ = (char)(digval - 10 + 'a'); /* a letter */
    else
      *p++ = (char)(digval + '0');    /* a digit */
  } while (val > 0);

  /* We now have the digit of the number in the buffer, but in reverse
  order. Thus we reverse them now. */

  *p-- = '\0';      /* terminate string; p points to last digit */

  do {
    temp = *p;
    *p = *firstdig;
    *firstdig = temp;  /* swap *p and *firstdig */
    --p;
    ++firstdig;     /* advance to next two digits */
  } while (firstdig < p); /* repeat until halfway */
}

/* Actual functions just call conversion helper with neg flag set correctly,
and return pointer to buffer. */

char * itoa_c(int val, char *buf, int radix) {
  if (radix == 10 && val < 0)
    xtoa_c((unsigned long)val, buf, radix, 1);
  else
    xtoa_c((unsigned long)(unsigned int)val, buf, radix, 0);
  return buf;
}

char * ltoa_c(long val, char *buf, int radix) {
  xtoa_c((unsigned long)val, buf, radix, (radix == 10 && val < 0));
  return buf;
}

char * ultoa_c(unsigned long val, char *buf, int radix)
{
  xtoa_c(val, buf, radix, 0);
  return buf;
}

#ifndef _NO_INT64

/* stdcall is faster and smaller... Might as well use it for the helper. */
static void x64toa_c(uint64_t val, char *buf, unsigned radix, int is_neg)
{
  char *p;        /* pointer to traverse string */
  char *firstdig;     /* pointer to first digit */
  char temp;       /* temp char */
  unsigned digval;    /* value of digit */

  p = buf;

  if (is_neg)
  {
    *p++ = '-';     /* negative, so output '-' and negate */
    val = (uint64_t)(-(int64_t)val);
  }

  firstdig = p;      /* save pointer to first digit */

  do {
    digval = (unsigned)(val % radix);
    val /= radix;    /* get next digit */

              /* convert to ascii and store */
    if (digval > 9)
      *p++ = (char)(digval - 10 + 'a'); /* a letter */
    else
      *p++ = (char)(digval + '0');    /* a digit */
  } while (val > 0);

  /* We now have the digit of the number in the buffer, but in reverse
  order. Thus we reverse them now. */

  *p-- = '\0';      /* terminate string; p points to last digit */

  do {
    temp = *p;
    *p = *firstdig;
    *firstdig = temp;  /* swap *p and *firstdig */
    --p;
    ++firstdig;     /* advance to next two digits */
  } while (firstdig < p); /* repeat until halfway */
}

/* Actual functions just call conversion helper with neg flag set correctly,
and return pointer to buffer. */

char * i64toa_c(int64_t val, char *buf, int radix) {
  x64toa_c((uint64_t)val, buf, radix, (radix == 10 && val < 0));
  return buf;
}

char * ui64toa_c(uint64_t val, char *buf, int radix)
{
  x64toa_c(val, buf, radix, 0);
  return buf;
}

#endif /* _NO_INT64 */
