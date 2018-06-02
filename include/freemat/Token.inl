
typedef int TokenValueType;
enum {
#define TOKENDEF(a)  TOK_##a,
#include "tokendef.txt"
#undef TOKENDEF
};

#define MUTEXLOCK(X)  {}
#define MUTEXUNLOCK(X)  {}

typedef struct token_t {
  TokenValueType m_tok;
  unsigned m_pos;
  const char* m_text;
  int m_textlen;
  int m_UID;
} token_t;

typedef struct tree_t {
  token_t m_node[1];
  int m_childrenlen;
  struct tree_t* m_children;
} tree_t;

#define throw_ParseException(a, b)  return 0;

typedef struct scanner_t {
  unsigned lastpos;
  char lasterr[256];
  BOOL octCompat;

  char m_filename[256];
  const char* m_text;
  int m_strlen;
  int m_ptr;
  int m_linenumber;
  BOOL m_ignorews[100];
  int m_ignorewslen;
  token_t m_tok[1];
  BOOL m_tokValid;
  BOOL m_debugFlag;
  BOOL m_inContinuationState;
  int  m_bracketDepth;
  BOOL m_blobFlag;
  //void setDebug(BOOL debugFlag) {m_debugFlag = debugFlag;}
  //unsigned position() {return m_ptr;}
} scanner_t;

typedef struct ArrayVector {
  mxArray* v;
  int n;
} ArrayVector;

int mxFreeArrays(mxArray* v, int n) {
  int i;
  for (i=0; i<n; ++i) {
    mxFreeArray(v+i);
  }
  return 0;
}
#if 1
int ArrayVector_free(ArrayVector* s) {
  int i;
  for (i=0; i<s->n; ++i) {
    mxFreeArray(s->v+i);
  }
  free(s->v);
  s->v = NULL;
  s->n = 0;
  return 0;
}
int ArrayVector_setsize(ArrayVector* s, int n) {
  int old_n = s->n;
  if (old_n<n) {
    REALLOC(mxArray, s->v, n);
    MEMSET(s->v+old_n, 0, n-old_n);
  } else {
    for (; old_n>n; ) {
      --old_n;
      mxFreeArray(s->v+old_n);
    }
  }
  return 0;
}
#endif

int token_free(token_t* s)
{
  //str_free(s->m_text);
  memset(s, 0, sizeof(token_t));
  return 0;
}
int token_set(token_t* s, TokenValueType tok, unsigned pos, const char* text, int len)
{
  s->m_tok = tok;
  s->m_pos = pos;
  s->m_text = text;
  s->m_textlen = len;
  return 0;
}

int token_settok(token_t* s, const token_t* tok)
{
  token_set(s, tok->m_tok, tok->m_pos, tok->m_text, tok->m_textlen);
  return 0;
}
BOOL token_is(const token_t* s, TokenValueType b)
{
  return s->m_tok == b;
}

BOOL token_isBinaryOperator(const token_t* s)
{
  return ((s->m_tok == TOK_PLUS) || (s->m_tok == TOK_MINUS) ||
      (s->m_tok == TOK_MUL) || (s->m_tok == TOK_RDIV) ||
      (s->m_tok == TOK_LDIV) || (s->m_tok == TOK_CARET) ||
      (s->m_tok == TOK_GTR) || (s->m_tok == TOK_LSS) ||
      (s->m_tok == TOK_COLON) || (s->m_tok == TOK_LEQ) ||
      (s->m_tok == TOK_GEQ) || (s->m_tok == TOK_EQU) ||
      (s->m_tok == TOK_NEQ) || (s->m_tok == TOK_SOR) ||
      (s->m_tok == TOK_SAND) || (s->m_tok == TOK_DOTTIMES) ||
      (s->m_tok == TOK_DOTRDIV) || (s->m_tok == TOK_DOTLDIV) ||
      (s->m_tok == TOK_DOTPOWER) || (s->m_tok == TOK_OR) ||
      (s->m_tok == TOK_AND));
}

BOOL token_isUnaryOperator(const token_t* s)
{
  return ((s->m_tok == TOK_PLUS) || (s->m_tok == TOK_MINUS) || (s->m_tok == '~')
      || (s->m_tok == TOK_UNARY_MINUS) ||
      (s->m_tok == TOK_UNARY_PLUS) ||
      (s->m_tok == TOK_INCR) ||
      (s->m_tok == TOK_DECR));
}


BOOL token_isRightAssociative(const token_t* s)
{
  return (s->m_tok == TOK_CARET);
}

int TokenToString(const token_t* b, char* buf, int len)
{
  const char* text = b->m_text ? b->m_text : "";

  switch (b->m_tok) {
#define TOKENDEF(a)  case TOK_##a : _snprintf(buf, len, "%i %s ", b->m_tok, #a , text); strncat(buf, text, b->m_textlen); return 0;
#include "tokendef.txt"
#undef TOKENDEF
  };

  return 0;
}

//QTextStream& operator<<(QTextStream& o, const token_t* b) {
//  o << TokenToString(b) << " (" << (b.position() >> 16)
//    << "," << (b.position() & 0xffff) << ")\r\n";
//  return o;
//}

static int LineNumber(int x)
{
  return (x & 0xffff);
}

int token_print(const token_t* b)
{
  char buf[256];
  TokenToString(b, buf, 256);
  printf("%s ()\n", buf, b->m_pos >> 16, LineNumber(b->m_pos));
  return 0;
}


int token_save(serialize_t* s, const token_t* b)
{
  serialize_putShort(s, b->m_tok);
  serialize_putInt(s, b->m_pos);
  serialize_putString(s, STR2(b->m_text, b->m_textlen));
  return 0;
}

int token_load(serialize_t* s, token_t* b)
{
  str_t str[1] = {0};
  b->m_tok = (TokenValueType)serialize_getShort(s);
  b->m_pos = serialize_getInt(s);
  serialize_getString(s, str);
  b->m_text = str->s;
  b->m_textlen = str->l;
  return 0;
}

int token_fillArray(const token_t* s, mxArray* arr)
{
  double d;
  float f;

  switch (s->m_tok) {
  default:
    return 0;

  case TOK_REAL:
    d = atof(s->m_text);
    mxSetDoubleScalar(arr, d);
    break;

  case TOK_IMAG:
    d = atof(s->m_text);
    mxSetDoubleComplex(arr, 0, d);
    break;

  case TOK_REALF:
    f = (float)atof(s->m_text);
    mxSetSingleScalar(arr, f);
    break;

  case TOK_IMAGF:
    f = (float)atof(s->m_text);
    mxSetSingleComplex(arr, 0, f);
    break;

  case TOK_STRING:
    mxSetString(arr, s->m_text, s->m_textlen);
    break;
  }

  return 0;
}
