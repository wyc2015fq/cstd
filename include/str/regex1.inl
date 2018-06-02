#define NOP 0
#define CHR 1
#define ANY 2
#define COR 3 //char or
#define SOR 4 //string or
#define RPS 5 //repeat string
#define RPC 6 //repeat char
#define BOL 7
#define EOL 8
#define BLK 9
#define REF 10
int re_comp(const char* s, int l, uchar* r, int n)
{
  int i, i1 = 0, t = 0, n1, l2;
  int stk[10] = {0};
  int k = 0;
  int ch;
  if (l <= 0) {
    return 0;
  }
  r[t] = 0;
  r[t + 1] = NOP;
  for (i = 0; i < l;) {
    switch (ch = s[i++]) {
    case '.':
      if (NOP != r[t + 1]) {
        t += r[t] + 2;
      }
      r[t] = 0;
      r[t + 1] = ANY;
      break;
    case '$':
    case '^':
      if (NOP != r[t + 1]) {
        t += r[t] + 2;
      }
      r[t] = 0;
      r[t + 1] = '^' == ch ? BOL : EOL;
      break;
      //case '+':
    case '*':
      if (CHR == r[t + 1]) {
        if (r[t] > 1) {
          r[t]--; // 退回一个字符
          t += r[t] + 2;
          r[t + 2] = r[t - 1];
        }
        r[t] = 3; //修改为*
        r[t + 1] = RPC;
      }
      else {
        n1 = r[t] + 2;
        memmove(r + t + 2, r + t, n1);
        r[t] = n1;
        r[t + 1] = RPS;
      }
      break;
    case '|': {
      n1 = r[t] + 2;
      memmove(r + t + 2, r + t, n1);
      l2 = re_comp(s + i, l - i, r + t + n1 + 2, n - t - n1 - 2);
      if (CHR == r[t + 3] && 1 == r[t + 2]) {
        if (COR == r[t + n1 + 3]) {
          r[t] = r[t + n1 + 2] + 1;
          r[t + 1] = COR;
          r[t + 2] = r[t + 4];
          memmove(r + t + 3, r + t + n1 + 4, r[t + n1 + 2]);
        }
        else if (CHR == r[t + n1 + 3] && 1 == r[t + n1 + 2]) {
          r[t] = 2;
          r[t + 1] = COR;
          r[t + 2] = r[t + 4];
          r[t + 3] = r[t + n1 + 4];
        }
        else {
          r[t] = n1 + l2;
          r[t + 1] = SOR;
        }
      }
      else {
        r[t] = n1 + l2;
        r[t + 1] = SOR;
      }
      i = l;
    }
    break;
    case '(': {
      int dep = 0;
      if (NOP != r[t + 1]) {
        t += r[t] + 2;
      }
      for (i1 = i; i < l; ++i) {
        dep += '(' == s[i];
        dep -= ')' == s[i];
        if (-1 == dep) {
          break;
        }
      }
      ASSERT(')' == s[i]);
      r[t] = re_comp(s + i1, i - i1, r + t + 2, n - t - 2);
      r[t + 1] = BLK;
      ++i;
    }
    break;
    case '\\':
      if ('1' <= s[i] && '9' >= s[i]) {
        t += r[t] + 2;
        r[t] = 3;
        r[t + 1] = REF;
        r[t + 2] = s[i++] - '1';
      }
      else {
        if (NOP != r[t + 1] && CHR != r[t + 1]) {
          r[t += r[t] + 2] = 0;
        }
        r[t + 1] = CHR;
        r[2 + t + r[t]] = s[i - 1];
        r[t]++;
      }
      break;
    default:
      if (NOP != r[t + 1] && CHR != r[t + 1]) {
        r[t += r[t] + 2] = 0;
      }
      r[t + 1] = CHR;
      r[2 + t + r[t]] = s[i - 1];
      r[t]++;
      break;
    }
  }
  r[t += r[t] + 2] = 0;
  return t;
}
#define CMDBEG 1
#define CMDEND 2
#define CMDTSK 3
#define CAPBEG 4
#define CAPEND 5
// 深度优先搜索
int re_match(const uchar* p, int np, const char* c, int s, int e, int* ret, int* cap, int* pncap)
{
  int n1, i = 0, j = 0, k = 0, ch, cmd, prevn = 0, nn;
  int f0, l0;
  int stk[1000];
  int nret = 0, maxret = 0, n = 0;
  int maxn = 0;
  int ipref = 0;
  int pref[20];
  int tcap[20];
  int icap = 0;
  ret[0] = -1;
  stk[n++] = CMDEND, stk[n++] = 0, stk[n++] = np, stk[n++] = CMDTSK, stk[n++] = s, stk[n++] = 0, stk[n++] = CMDBEG;
#define PUSH(ff, ll) if ((ff)<(ll)) {(stk[n++]=(ff), stk[n++]=(ll), stk[n++]=CMDTSK), maxn=MAX(n, maxn), ASSERT(n<countof(stk));}
  for (; n > 0;) {
    cmd = stk[--n];
    if (CMDTSK == cmd) {
      l0 = stk[--n], f0 = stk[--n];
    }
    else {
      if (CMDBEG == cmd) {
        prevn = stk[--n];
        s = stk[--n];
        icap = 0;
      }
      else if (CMDEND == cmd) {
        if (s > ret[0]) {
          ret[0] = s;
          memcpy(cap, tcap, icap * sizeof(int));
          *pncap = icap;
        }
      }
      else if (CAPBEG == cmd) {
        tcap[icap++] = s;
      }
      else if (CAPEND == cmd) {
        tcap[icap++] = s;
      }
      continue;
    }
    if (f0 == l0) {
      continue;
    }
    n1 = p[f0++];
    switch (ch = p[f0++]) {
    case ANY:
      if (s < e) {
        PUSH(f0 + n1, l0);
        ++s;
      }
      break;
    case REF:
      ASSERT(ipref > 0);
      if (ipref >= 2) {
        ipref -= 2;
        PUSH(pref[ipref], pref[ipref + 1]);
        //return re_match(r, stk, n, s, e);
      }
      break;
    case BLK:
      //pref[ipref++] = f0;
      //pref[ipref++] = f0+n1;
      PUSH(f0 + n1, l0);
      stk[n++] = CAPEND;
      PUSH(f0, f0 + n1);
      stk[n++] = CAPBEG;
      break;
    case EOL:
      if (s == e) {
        PUSH(f0 + n1, l0);
      }
      else if ('\n' == c[s]) {
        ++s;
        PUSH(f0 + n1, l0);
      }
      else if (1 < e - s && '\r' == c[s] && '\n' == c[s + 1]) {
        s += 2;
        PUSH(f0 + n1, l0);
      }
      else {
        n = prevn;
      }
      break;
    case BOL:
      if (0 == s) {
        PUSH(f0 + n1, l0);
      }
      else {
        n = prevn;
      }
      break;
    case CHR:
      if (n1 <= (e - s) && 0 == memcmp(c + s, p + f0, n1)) {
        PUSH(f0 + n1, l0);
        s += n1;
      }
      else {
        n = prevn;
      }
      break;
#define BEGCMD() {int _nn=n; stk[n++]=CMDEND
#define ENDCMD() stk[n++]=s, stk[n++]=_nn, stk[n++]=CMDBEG;}
    case COR: // [abc]
      if (memchr(p + f0, c[s], n1)) {
        nn = n - prevn - 1;
        n = prevn;
        ++s;
        BEGCMD();
        memmove(stk + n, stk + prevn + 1, nn * sizeof(int));
        n += nn;
        PUSH(f0 + n1, l0);
        ENDCMD();
      }
      else {
        n = prevn;
      }
      break;
    case SOR:
      nn = n - prevn - 1;
      n = prevn;
      if (1) {
        int p2 = f0 + p[f0] + 2;
        BEGCMD();
        memmove(stk + n, stk + prevn + 1, nn * sizeof(int));
        n += nn;
        PUSH(f0 + n1, l0);
        PUSH(p2, p2 + p[p2] + 2);
        ENDCMD();
      }
      if (1) {
        int p1 = f0;
        BEGCMD();
        memmove(stk + n, stk + prevn + 1, nn * sizeof(int));
        n += nn;
        PUSH(f0 + n1, l0);
        PUSH(p1, p1 + p[p1] + 2);
        ENDCMD();
      }
      break;
    case RPC: // *
      nn = n - prevn - 1;
      n = prevn;
      if (s < e) {
        if (c[s] == p[f0]) {
          ++s;
          BEGCMD();
          memmove(stk + n, stk + prevn + 1, nn * sizeof(int));
          n += nn;
          PUSH(f0 - 2, l0);
          ENDCMD();
        }
      }
      if (1) {
        BEGCMD();
        memmove(stk + n, stk + prevn + 1, nn * sizeof(int));
        n += nn;
        PUSH(f0 + n1, l0);
        ENDCMD();
      }
      break;
    case RPS: // *
      nn = n - prevn - 1;
      n = prevn;
      if (s < e) {
        BEGCMD();
        memmove(stk + n, stk + prevn + 1, nn * sizeof(int));
        n += nn;
        PUSH(f0 - 2, l0);
        PUSH(f0, f0 + p[f0] + 2);
        ENDCMD();
      }
      if (1) {
        BEGCMD();
        memmove(stk + n, stk + prevn + 1, nn * sizeof(int));
        n += nn;
        PUSH(f0 + n1, l0);
        ENDCMD();
      }
      break;
    default:
      ASSERT(0);
      break;
    }
  }
  {
    int i;
    for (i = 0; i < icap; i += 2) {
      printf("[%d %d]", cap[i], cap[i + 1]);
    }
    printf("maxn=%d\n", maxn);
  }
  return nret;
}
int re_exec(const uchar* p, int np, const char* c, int s, int e)
{
  int ret[10];
  int cap[20] = {0};
  int ncap = 0;
  int nret = re_match(p, np, c, s, e, ret, cap, &ncap);
  return ret[0];
}
#undef NOP
#undef CHR
#undef ANY
#undef RPS
#undef OR
#undef PUSH
#undef ENDCMD
#undef BEGCMD

