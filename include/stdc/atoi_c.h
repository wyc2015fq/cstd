
static int atoi_c(const char *nptr)
{
  int c;              /* current char */
  int total;      /* current total */
  int sign;           /* if '-', then negative, otherwise positive */

                      /* skip whitespace */
  while (isspace((int)(unsigned char)*nptr))
    ++nptr;

  c = (int)(unsigned char)*nptr++;
  sign = c;           /* save sign indication */
  if (c == '-' || c == '+')
    c = (int)(unsigned char)*nptr++;    /* skip sign */

  total = 0;

  while (isdigit(c)) {
    total = 10 * total + (c - '0');     /* accumulate digit */
    c = (int)(unsigned char)*nptr++;    /* get next char */
  }

  if (sign == '-')
    return -total;
  else
    return total;   /* return result, negated if necessary */
}

static int64_t atoi64_c(const char *nptr)
{
  int c;              /* current char */
  int64_t total;      /* current total */
  int sign;           /* if '-', then negative, otherwise positive */

                      /* skip whitespace */
  while (isspace((int)(unsigned char)*nptr))
    ++nptr;

  c = (int)(unsigned char)*nptr++;
  sign = c;           /* save sign indication */
  if (c == '-' || c == '+')
    c = (int)(unsigned char)*nptr++;    /* skip sign */

  total = 0;

  while (isdigit(c)) {
    total = 10 * total + (c - '0');     /* accumulate digit */
    c = (int)(unsigned char)*nptr++;    /* get next char */
  }

  if (sign == '-')
    return -total;
  else
    return total;   /* return result, negated if necessary */
}
