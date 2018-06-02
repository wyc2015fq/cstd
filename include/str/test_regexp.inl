#include "math/regexp.inl"
int fgetline(const char* fname, int iline, char* buf, int buflen)
{
  int ret = 0, len;
  FILE* pf = fopen(fname, "rb");
  if (NULL == pf) {
    return 0;
  }
  for (; iline > 0; --iline) {
    fgets(buf, buflen, pf);
  }
  ret = fgets(buf, buflen, pf) > 0;
  len = strlen(buf);
  for (; len > 0 && '\n' == buf[len - 1] || '\r' == buf[len - 1] || ' ' == buf[len - 1] || '\t' == buf[len - 1]; --len);
  buf[len] = 0;
  fclose(pf);
  return ret;
}
int test_regexp()
{
  int i = 0, len = 0, infixlen = 0, postfixlen;
  char buf[256];
  char infix[256];
  char postfix[256];
  nfa_t nfa[1] = {0};
  dfa_t dfa[1] = {0};
  const char* fn = "test_regexp.txt";
  for (i = 0; fgetline(fn, i, buf, 256); ++i) {
    len = strlen(buf);
    re_check(buf, len);
    infixlen = re_to_infix(buf, len, infix, 256);
    postfixlen = re_infix_to_postfix(infix, infixlen, postfix, 256);
    printf("%s\n", buf);
    printf("%s\n", infix);
    printf("%s\n", postfix);
    re2nfa(postfix, postfixlen, nfa);
    re_print_NFA(nfa);
    nfa2dfa(nfa, dfa);
  }
  return 0;
}

