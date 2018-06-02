typedef struct nfaedge_t {
  int s, e;
  int c;
} nfaedge_t;
typedef struct nfa_t {
  nfaedge_t st[256];
  int nst;
  int maxst;
  int ntype;
  int start;
  int s, e;
} nfa_t;
// AB A B
// i -----> j i -----> k ------> j
//
//
//
// A
// A|B ----->
// i -----> j i B j
// ----->
//
//
// A* e A e
// i -----> j i -----> k ------> j
//
// <re> ::= <expr> { <expr> }
// | <re> '|' <re>
// <expr> ::= <term>
// | <term> '*'
// <term> ::= <label>
// | '(' <re> ')'
// <label> ::= <symbol>
// | '[' <range> { <range> } ']'
// | '[' ']' { <range> } ']'
// | '[' '^' <range> { <range> } ']'
// | '[' '^' ']' { <range> } ']'
// <range> ::= <symbol>
// | <symbol> '-' <symbol>
// <symbol> ::= '.'
// | 0 .. n (any element of alphabet)
// | '\' <symbol>
//
int renfa_re(nfa_t* nfa, uchar* str, int slen, int i);
#define pushedge(nfa, ss, ee, cc) (nfa->st[nfa->nst].s=ss, nfa->st[nfa->nst].e=ee, nfa->st[nfa->nst].c=cc, nfa->nst++)
#define sss "()*|"
int renfa_symbol(nfa_t* nfa, uchar* str, int slen, int i)
{
  nfa->st[nfa->s].e = nfa->nst;
  for (; i < slen; ++i) {
    uchar ch = str[i];
    if (0 == strchr(sss, ch)) {
      nfa->st[nfa->e].s = pushedge(nfa, nfa->s, nfa->e, ch);
    }
    else {
      break;
    }
  }
  return i;
}
int renfa_label(nfa_t* nfa, uchar* str, int slen, int i)
{
  if ('(' == str[i]) {
    i = renfa_re(nfa, str, slen, i);
    assert(')' == str[i]);
  }
  else {
    i = renfa_symbol(nfa, str, slen, i);
  }
  return i;
}
int renfa_term(nfa_t* nfa, uchar* str, int slen, int i)
{
  i = renfa_label(nfa, str, slen, i);
  return i;
}
int renfa_expr(nfa_t* nfa, uchar* str, int slen, int i)
{
  i = renfa_term(nfa, str, slen, i);
  if (i < slen && '*' == str[i]) {
  }
  return i;
}
int renfa_re(nfa_t* nfa, uchar* str, int slen, int i)
{
  i = renfa_expr(nfa, str, slen, i);
  for (i = 0; i < slen;) {
    uchar ch = str[i++];
    switch (ch) {
    case '|':
      i = renfa_re(nfa, str, slen, i);
      break;
    default:
      i = renfa_expr(nfa, str, slen, i);
      break;
    }
  }
  return i;
}
int renfa_comp(nfa_t* nfa, uchar* str, int slen)
{
  pushedge(nfa, 0, 0, 0);
  renfa_re(nfa, str, slen, 0);
  return 0;
}

