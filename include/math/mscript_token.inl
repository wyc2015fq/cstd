enum {
#define MSCRIPT_TOKEN_DEF(a, b, c) a,
#include "mscript_token.txt"
#include "mscript_token_binopt.txt"
#include "mscript_token_keyword.txt"
#undef MSCRIPT_TOKEN_DEF
};
const char* mscript_token2str(int tok)
{
  switch (tok) {
#define MSCRIPT_TOKEN_DEF(a, b, c) case a: return b;
#include "mscript_token.txt"
#include "mscript_token_binopt.txt"
#include "mscript_token_keyword.txt"
#undef MSCRIPT_TOKEN_DEF
  }
  return "unkown";
}
int mscript_init(mscript_engine_t* me, char* str, int len)
{
  me->cur = me->str = str, me->send = str + len;
  return 0;
}
#define curchr(me) *(me->cur)
int mscript_nextchr(mscript_engine_t* me)
{
  int ch;
  while ((me->cur < me->send) && (ch = *me->cur++) && (' ' == ch || '\t' == ch || '\r' == ch));
  return ch;
}
int mscript_next_token(mscript_engine_t* me, str_t* s)
{
  char* str = me->cur;
  int i;
  char ch;
  s->l = 0;
  while ((me->cur < me->send) && (ch = *me->cur++) && (' ' == ch || '\t' == ch || '\r' == ch));
  if (me->cur >= me->send) {
    return TOK_EOF;
  }
  switch (ch) {
  case '\n':
    me->line++;
    return TOK_EOL;
  case ',':
    return TOK_COMMA;
  case ':':
    return TOK_COLON;
  case ';':
    return TOK_SEMICOLON;
  case '(':
    return TOK_PAREN_L;
  case ')':
    return TOK_PAREN_R;
  case '{':
    return TOK_BRACE_L;
  case '}':
    return TOK_BRACE_R;
  case '-': {
    ch = curchr(me);
    switch (ch) {
    case '-':
      return me->cur++, TOK_DEC;
    case '=':
      return me->cur++, TOK_MINUS_EQ;
    }
    return TOK_MINUS;
  }
  case '+': {
    ch = curchr(me);
    switch (ch) {
    case '-':
      return me->cur++, TOK_INC;
    case '=':
      return me->cur++, TOK_PLUS_EQ;
    }
    return TOK_PLUS;
  }
  case '&':
    ch = curchr(me);
    if ('&' == ch) {
      return me->cur++, TOK_SAND;
    }
    return TOK_AND;
  case '|':
    ch = curchr(me);
    if ('|' == ch) {
      return me->cur++, TOK_SOR;
    }
    return TOK_OR;
  case '~':
    ch = curchr(me);
    if ('|' == ch) {
      return me->cur++, TOK_NEQ;
    }
    return TOK_NOT;
  case '*':
    return TOK_MUL;
  case '/':
    return TOK_RDIV;
  case '=':
    ch = curchr(me);
    if ('=' == ch) {
      return me->cur++, TOK_EQU;
    }
    return TOK_ASSIGNMENT;
  case '<':
    ch = curchr(me);
    if ('=' == ch) {
      return me->cur++, TOK_LEQ;
    }
    return TOK_GTR;
  case '>':
    ch = curchr(me);
    if ('=' == ch) {
      return me->cur++, TOK_GEQ;
    }
    return TOK_LSS;
  case '!':
    ASSERT(0);
    return 0;
  case '.': {
    ch = curchr(me);
    switch (ch) {
    case '*':
      return me->cur++, TOK_DOTTIMES;
    case '/':
      return me->cur++, TOK_DOTRDIV;
    case '\\':
      return me->cur++, TOK_DOTLDIV;
    case '^':
      return me->cur++, TOK_DOTPOWER;
    case '\'':
      return me->cur++, TOK_DOTTRANSPOSE;
    }
    return TOK_DOT;
  }
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9': {
    s->s = str;
    while (me->cur < me->send && ((curchr(me) >= '0' && curchr(me) <= '9')) && (ch == *me->cur++));
    if ('.' == curchr(me)) {
      me->cur++;
      while (me->cur < me->send && ((curchr(me) >= '0' && curchr(me) <= '9')) && (ch == *me->cur++));
    }
    s->l = me->cur - s->s;
    if ('f' == ch || 'F' == ch) {
      return me->cur++, TOK_REALF;
    }
    if ('d' == ch || 'D' == ch) {
      return me->cur++, TOK_REAL;
    }
    return TOK_REAL;
  }
  case '\'':
  case '\"': {
    s->s = me->cur;
    while (me->cur < me->send && !('\\' != ch && '"' == curchr(me)) && (ch == *me->cur++));
    s->l = me->cur - s->s;
    me->cur++;
    return TOK_STRING;
  }
  default: {
    typedef struct {
      int tok;
      str_t str;
    } KEYWORD;
    static const KEYWORD keywords[] = {
#define MSCRIPT_TOKEN_DEF(a, b, c) { a, sizeof(b), b},
#include "mscript_token_keyword.txt"
#undef MSCRIPT_TOKEN_DEF
    };
    if (isalpha(ch) || '_' == ch) {
      s->s = str;
    }
    while (me->cur < me->send && (isalnum(curchr(me)) || '_' == curchr(me)) && (ch == *me->cur++));
    s->l = me->cur - s->s;
    for (i = 0; i < countof(keywords); ++i) {
      int iCmp = STRcmp(keywords[i].str, *s);
      if (iCmp == 0) {
        return TOK_KEYWORD;
      }
    }
    return TOK_IDENT;
  }
  }
  ASSERT(0);
  return 0;
}

