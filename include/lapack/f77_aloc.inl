
static char * F77_aloc(integer Len, char *whence)
{
  static integer memfailure = 3;
  char *rv;
  unsigned int uLen = (unsigned int) Len;        /* for K&R C */
  
  if (!(rv = (char*)malloc(uLen))) {
    fprintf(stderr, "malloc(%u) failure in %s\n", uLen, whence);
    //                exit_(&memfailure);
  }
  return rv;
}

