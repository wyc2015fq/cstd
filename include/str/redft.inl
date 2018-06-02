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
typedef struct dfanode_t {
  int type; // 匹配类型 0 表示未结束
  int next[256];
  int cnt;
} dfanode_t;
typedef struct dfa_t {
  dfanode_t* st;
  int nst;
  int maxst;
  int ntype;
  int start;
} dfa_t;
int redfa_setsize(dfa_t* dfa, int newmaxst)
{
  if (newmaxst > dfa->maxst) {
    int maxst = dfa->maxst;
    dfa->maxst = newmaxst;
    REALLOC(dfanode_t, dfa->st, dfa->maxst);
    MEMSET(dfa->st + maxst, 0, dfa->maxst - maxst);
  }
  return 0;
}
// greedy 贪婪
int redfa_match(const dfa_t* dfa, const uchar* str, int slen, int notgreedy, int* ppos)
{
  int i, ret = 0, pos = -1;
  int st = dfa->start; // 起始状态
  for (i = 0; i < slen;) {
    uchar ch = str[i++];
    st = dfa->st[st].next[ch] - 1;
    if (st >= 0) {
      ret = dfa->st[st].type;
      if (ret) {
        pos = i;
        if (notgreedy) {
          break;
        }
      }
    }
    else {
      break;
    }
  }
  if (ppos) {
    *ppos = pos;
  }
  return ret;
}
typedef struct dfa_work_t {
  int st;
  int nend;
  int end[256];
} dfa_work_t;
int redfa_connect(dfa_t* dfa, const uchar* str, int slen, int i, dfa_work_t* dw)
{
  int st = dw->st;
  for (; i < slen;) {
    uchar ch = str[i++];
    switch (ch) {
    case '|':
      i = redfa_connect(dfa, str, slen, i, dw);
      break;
    case '\\':
      break;
    default:
      if (dfa->st[st].next[ch]) {
        st = dfa->st[st].next[ch] - 1;
      }
      else {
        dfa->nst++;
        dfa->st[st].next[ch] = dfa->nst + 1;
        dfa->st[st].cnt++;
        st = dfa->nst;
      }
      break;
    }
  }
  dw->end[dw->nend++] = st;
  return i;
}
// 连接 AB A.e=B
// 选择 A|B
int redfa_comp(dfa_t* dfa, const uchar* str, int slen)
{
  int i;
  dfa_work_t dw1[1] = {0};
  dw1->st = dfa->start;
  redfa_setsize(dfa, dfa->nst + slen + 1);
  redfa_connect(dfa, str, slen, 0, dw1);
  dfa->ntype++;
  for (i = 0; i < dw1->nend; ++i) {
    dfa->st[dw1->end[i]].type = dfa->ntype;
  }
  return dfa->ntype;
}
int redfa_randstr(dfa_t* dfa, uchar* str, int maxlen)
{
  int i, j, k;
  int st = dfa->start; // 起始状态
  for (i = 0; i < maxlen; ++i) {
    if (dfa->st[st].cnt) {
      int cnt = rand() % dfa->st[st].cnt;
      for (k = 0, j = 0; j < 256; ++j) {
        if (dfa->st[st].next[j]) {
          ++k;
          if (k == cnt) {
            st = dfa->st[st].next[j];
            str[i] = j;
            break;
          }
        }
      }
    }
    else {
      break;
    }
  }
  return i;
}
#define MAPNUMBER "0123456789"
#define MAPALPHA0 "abcdefghijklmnopqrstuvwxyz"
#define MAPALPHA1 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
int redfa_clear(dfa_t* dfa)
{
  MEMSET(dfa->st, 0, dfa->maxst);
  return 0;
}
int redfa_free(dfa_t* dfa)
{
  if (dfa->st) {
    FREE(dfa->st);
  }
  return 0;
}

