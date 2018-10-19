
#include <ctype.h>
#include <string.h>
#define VARENUM_DEF_DEF(VARENUM_DEF) \
VARENUM_DEF(_VT_I2, short, "%d", atoi(s)) \
VARENUM_DEF(_VT_I4, int, "%d", atoi(s)) \
VARENUM_DEF(_VT_I1, char, "%d", atoi(s)) \
VARENUM_DEF(_VT_I8, __int64, "%I64d", atoi(s)) \
VARENUM_DEF(_VT_R4, float, "%f", atof(s)) \
VARENUM_DEF(_VT_R8, double, "%f", atof(s))
//VARENUM_DEF(_VT_NULL, void*,"%s", "null"), var_free(v))
//VARENUM_DEF(_VT_LPSTR, char*, "%s", v->v), 0)
//VARENUM_DEF(_VT_UI1, unsigned char,"%u", atoi(s))
//VARENUM_DEF(_VT_UI2, unsigned short,"%u", atoi(s))
//VARENUM_DEF(_VT_UI4, unsigned int,"%u", atoi(s))
//VARENUM_DEF(_VT_UI8, unsigned __int64,"%I64d", atoi(s))

// {} brace
// () Parentheses
// [] square brackets
#define PYTOKOP_PUK_DEF(PYTOKOP) \
PYTOKOP(COMMA, ',') \
PYTOKOP(DOT, '.') \
PYTOKOP(COLON, ':') \
PYTOKOP(SEMICOLON, ';') \
PYTOKOP(PAREN_L, '(') \
PYTOKOP(PAREN_R, ')') \
PYTOKOP(BRACE_L, '{') \
PYTOKOP(BRACE_R, '}') \
PYTOKOP(SQUARE_L, '[') \
PYTOKOP(SQUARE_R, ']') \

#define PYTOKOP_GEN_DEF(PYTOKOP) \
PYTOKOP(NUL, "NUL") \
PYTOKOP(BEGIN, "BEGIN") \
PYTOKOP(END, "END") \
PYTOKOP(INDENT, "INDENT") \
PYTOKOP(EOL, "EOL") \
PYTOKOP(INDENTIFIER, "INDENTIFIER") \
PYTOKOP(NUMBER, "NUMBER") \
PYTOKOP(LINE_COMMENT, "LINE_COMMENT") \
PYTOKOP(BLOCK_COMMENT, "BLOCK_COMMENT") \
PYTOKOP(EOF, "EOF")

#define PYTOKOP_KEY_DEF(PYTOKOP) \
PYTOKOP(and, "and") \
PYTOKOP(del, "del") \
PYTOKOP(from, "from") \
PYTOKOP(not, "not") \
PYTOKOP(while, "while") \
PYTOKOP(as, "as") \
PYTOKOP(elif, "elif") \
PYTOKOP(global, "global") \
PYTOKOP(or, "or") \
PYTOKOP(with, "with") \
PYTOKOP(assert, "assert") \
PYTOKOP(else, "else") \
PYTOKOP(if, "if") \
PYTOKOP(pass, "pass") \
PYTOKOP(yield, "yield") \
PYTOKOP(break, "break") \
PYTOKOP(except, "except") \
PYTOKOP(import, "import") \
PYTOKOP(print, "print") \
PYTOKOP(class, "class") \
PYTOKOP(exec, "exec") \
PYTOKOP(in, "in") \
PYTOKOP(raise, "raise") \
PYTOKOP(continue, "continue") \
PYTOKOP(finally, "finally") \
PYTOKOP(is, "is") \
PYTOKOP(return, "return") \
PYTOKOP(def, "def") \
PYTOKOP(for, "for") \
PYTOKOP(lambda, "lambda") \
PYTOKOP(try, "try")

#define PYTOKOP_DEF(PYTOKOP) \
PYTOKOP_GEN_DEF(PYTOKOP) \
PYTOKOP_PUK_DEF(PYTOKOP) \
PYTOKOP_KEY_DEF(PYTOKOP)

enum PyTokOp {
#define PYTOKOP(name, msg)  OP_##name,
  PYTOKOP_GEN_DEF(PYTOKOP)
  PYTOKOP_PUK_DEF(PYTOKOP)
  PYTOKOP_KEY_DEF(PYTOKOP)
#undef PYTOKOP
OP_COUNT_OP,
};

const char* pytokopstr[] = {
#define PYTOKOP(name, msg)  #msg,
  PYTOKOP_DEF(PYTOKOP)
#undef PYTOKOP
};

struct PyTok {
  enum PyTokOp t;
  char* s;
  int l;
};

#define PY_ERR_DEF(PY_ERR) \
PY_ERR(OK, "OK") \
PY_ERR(UNNOWK_CHAR, "UNNOWK_CHAR") \
PY_ERR(EOF, "EOF")

enum PyErr{
#define PY_ERR(name, msg)  PyErr_##name,
  PY_ERR_DEF(PY_ERR)
#undef PY_ERR
};
const char* pyerrmsg[] = {
#define PY_ERR(name, msg)  msg,
  PY_ERR_DEF(PY_ERR)
#undef PY_ERR
};

struct PyScanner {
  char* str;
  int len;
  int pos;
  int line, col;
  int indent_pre;
  PyTok tok;
};

PyErr PyTok_set(PyScanner* s, enum PyTokOp t, int i) { s->tok.t = t, s->tok.s = s->str+i, s->tok.l = s->pos-i; return PyErr_OK; }
PyScanner PyScanner_New(char* str, int len) { PyScanner  t = { 0 }; t.str = str, t.len = len; return t; }

struct CharPos {
  int line, col;
};

CharPos pos2line(const char* s, int pos) {
  CharPos cp = {1, 1};
  int i = 0, cur_line_pos = 0;
  for (; i<pos; ++i) {
    char c = s[i];
    if (c == '\n') { cur_line_pos = i; ++cp.line; }
  }
  cp.col = pos - cur_line_pos;
  return cp;
}

PyErr PyScanner_next(PyScanner* s) {
  int i;
  char c;
  for (; s->pos < s->len;) {
    switch (c = s->str[i = s->pos++]) {
#define PYTOKOP(name, c)  case c: PyTok_set(s, OP_##name, i); return PyErr_OK; break;
      PYTOKOP_PUK_DEF(PYTOKOP)
#undef PYTOKOP
    case '\n':
      return PyTok_set(s, OP_EOL, s->pos);
    case '#':
      for (; s->pos < s->len && ('\n' != s->str[s->pos]); s->pos++);
      return PyTok_set(s, OP_LINE_COMMENT, i);
      break;
    case ' ':
      if (s->tok.t == OP_EOL) {
        for (; s->pos < s->len && (' ' == s->str[s->pos]); s->pos++);
        c = s->str[s->pos];
        if (c != '\n' && c != '\r') {
          int l = s->pos - i;
          if (s->indent_pre < l) {
            s->indent_pre = l;
            return PyTok_set(s, OP_BEGIN, i);
          } else if (s->indent_pre > l) {
            s->indent_pre = l;
            return PyTok_set(s, OP_END, i);
          }
        }
      }
      break;
    default:
      if (isalpha(c)) {
        for (; s->pos < s->len && isalpha(s->str[s->pos]); s->pos++);
#define PYTOKOP(name, msg)  if (0==strncmp(msg, s->str + i, strlen(msg))) { return PyTok_set(s, OP_##name, i); }
        PYTOKOP_KEY_DEF(PYTOKOP)
#undef PYTOKOP
          for (; s->pos < s->len && isalnum(s->str[s->pos]); s->pos++);
        return PyTok_set(s, OP_INDENTIFIER, i);
      }
#define isnumb(c)  ('0'<=(c) && (c)<='9')
      else if (isnumb(c)) {
        for (; s->pos < s->len && isnumb(s->str[s->pos]); s->pos++);
        return PyTok_set(s, OP_NUMBER, i);
      }
      break;
    }
  }

  return PyErr_EOF;
}
