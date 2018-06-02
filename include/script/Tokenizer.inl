#if !defined(AFX_TOKENIZER_H__20050805_FACF_B73F_147C_0080AD509054__INCLUDED_)
#define AFX_TOKENIZER_H__20050805_FACF_B73F_147C_0080AD509054__INCLUDED_
#define MAX_KEYWORD_LEN 32
#define MAX_STRING_LEN 300
typedef enum {
  OP_END = 0,
  OP_EOL,
  OP_INDENTIFIER,
  OP_COMMA,
  OP_DOT,
  OP_COLON,
  OP_SEMICOLON,
  OP_CHAR,
  OP_PAREN_L,
  OP_PAREN_R,
  OP_BRACE_L,
  OP_BRACE_R,
  OP_INTEGER,
  OP_NUMBER,
  OP_STRING,
  OP_EQUAL,
  OP_NOT,
  OP_PLUS,
  OP_MINUS,
  OP_PLUSEQUAL,
  OP_MINUSEQUAL,
  OP_DIV,
  OP_MUL,
  OP_MOD, //%
  OP_OR, //||
  OP_AND,
  OP_XOR,
  OP_INC,
  OP_DEC,
  OP_CMP_EQU,//等于
  OP_CMP_NEQ,//不等于
  OP_CMP_LSS,//小于
  OP_CMP_LEQ,//小于或等于
  OP_CMP_GTR,//大于
  OP_CMP_GEQ,//大于或等于
  //
  OP_IF,
  OP_ELSE,
  OP_FUNCTION,
  OP_SWITCH,
  OP_CASE,
  OP_DEFAULT,
  OP_WHILE,
  OP_FOR,
  OP_BREAK,
  OP_FOREACH,
  //OP_VAR,
  OP_RETURN,
  OP_USES,
} OPTYPE;
#define tok_free(x)
#define tok_cpy(a, b) *(a) = *(b)
#define THROW(x) {throw_error(env, env->m_tok->m_iLineNo, #x );} return x
typedef struct tok_t {
  int type;
  str_t s;
} tok_t;
#define tok_set_t(tok, t) tok_set_tp(tok, t, 0, 0)
#define tok_eqstr(tok, str) (0==str_cmp_c((tok)->s, str, 0))
int tok_set_tp(tok_t* tok, int t, const void* p, int len)
{
  tok->type = t;
  tok->s.s = (char*)p;
  tok->s.l = len;
  return t;
}
typedef struct CTokenizer {
  const char* m_pstr;
  int m_iLineNo;
} CTokenizer;
void Assign(CTokenizer* s, const char* pstr, int iLineNo)
{
  s->m_pstr = (pstr);
  s->m_iLineNo = iLineNo;
}
int GetToken(CTokenizer* s, tok_t* tok)
{
  int type = 0, stop = 0;
  while (!stop) {
    stop = 1;
    while (*s->m_pstr && (*s->m_pstr == ' ' || *s->m_pstr == '\t' || *s->m_pstr == '\r')) {
      s->m_pstr++;
    }
    if (*s->m_pstr == '\0') {
      return tok_set_t(tok, OP_END);
    }
    switch (*s->m_pstr) {
    case '\n':
      s->m_pstr++;
      s->m_iLineNo++;
      stop = 0;
      break;
    case ',':
      return s->m_pstr++, tok_set_t(tok, OP_COMMA);
    case '.':
      return s->m_pstr++, tok_set_t(tok, OP_DOT);
    case ':':
      return s->m_pstr++, tok_set_t(tok, OP_COLON);
    case ';':
      return s->m_pstr++, tok_set_t(tok, OP_SEMICOLON);
    case '(':
      return s->m_pstr++, tok_set_t(tok, OP_PAREN_L);
    case ')':
      return s->m_pstr++, tok_set_t(tok, OP_PAREN_R);
    case '{':
      return s->m_pstr++, tok_set_t(tok, OP_BRACE_L);
    case '}':
      return s->m_pstr++, tok_set_t(tok, OP_BRACE_R);
    case '-':
      s->m_pstr++;
      if (*s->m_pstr == '-') {
        return s->m_pstr++, tok_set_t(tok, OP_DEC);
      }
      if (*s->m_pstr == '=') {
        return s->m_pstr++, tok_set_t(tok, OP_MINUSEQUAL);
      }
      return tok_set_t(tok, OP_MINUS);
    case '+':
      s->m_pstr++;
      if (*s->m_pstr == '+') {
        return s->m_pstr++, tok_set_t(tok, OP_INC);
      }
      if (*s->m_pstr == '=') {
        return s->m_pstr++, tok_set_t(tok, OP_PLUSEQUAL);
      }
      return tok_set_t(tok, OP_PLUS);
    case '&':
      s->m_pstr++;
      if (*s->m_pstr == '&') {
        s->m_pstr++;
      }
      return tok_set_t(tok, OP_AND);
    case '|':
      s->m_pstr++;
      if (*s->m_pstr == '|') {
        s->m_pstr++;
      }
      return tok_set_t(tok, OP_OR);
    case '~':
      s->m_pstr++;
      return tok_set_t(tok, OP_XOR);
    case '*':
      s->m_pstr++;
      return tok_set_t(tok, OP_MUL);
    case '%':
      s->m_pstr++;
      return tok_set_t(tok, OP_MOD);
    case '/': {
      s->m_pstr++;
      if (*s->m_pstr == '/') {
        while (*s->m_pstr != '\0' && *s->m_pstr != '\n') {
          s->m_pstr++;
        }
        stop = 0;
        break;
      }
      if (*s->m_pstr == '*') {
        s->m_pstr++;
        while (*s->m_pstr != '\0' && !(*s->m_pstr == '*' && *(s->m_pstr + 1) == '/')) {
          s->m_pstr++;
        }
        if (*s->m_pstr != '\0') {
          s->m_pstr += 2;
        }
        stop = 0;
        break;
      }
      return tok_set_t(tok, OP_DIV);
    }
    break;
    case '=':
    case '<':
    case '>':
    case '!': {
      const char* p = s->m_pstr++;
      if (*s->m_pstr == '<' || *s->m_pstr == '>' || *s->m_pstr == '=') {
        s->m_pstr++;
      }
      tok_set_tp(tok, type, p, s->m_pstr - p);
      if (tok_eqstr(tok, "!")) {
        type = OP_NOT;
      }
      else if (tok_eqstr(tok, "=")) {
        type = OP_EQUAL;
      }
      else if (tok_eqstr(tok, "==")) {
        type = OP_CMP_EQU;
      }
      else if (tok_eqstr(tok, "!=")) {
        type = OP_CMP_NEQ;
      }
      else if (tok_eqstr(tok, "<")) {
        type = OP_CMP_LSS;
      }
      else if (tok_eqstr(tok, ">")) {
        type = OP_CMP_GTR;
      }
      else if (tok_eqstr(tok, "<=")) {
        type = OP_CMP_LEQ;
      }
      else if (tok_eqstr(tok, ">=")) {
        type = OP_CMP_GEQ;
      }
      else {
        ASSERT(0);
      }
      tok->type = type;
      return tok->type;
    }
    break;
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
      const char* p = s->m_pstr;
      int cCount = 0;
      type = OP_INTEGER;
      while (*s->m_pstr && *s->m_pstr >= '0' && *s->m_pstr <= '9') {
        s->m_pstr++;
        if (*s->m_pstr == '.') {
          type = OP_NUMBER, s->m_pstr++;
        }
        if (++cCount >= MAX_KEYWORD_LEN) {
          return tok_set_t(tok, OP_END);
        }
      }
      return tok_set_tp(tok, type, p, s->m_pstr - p);
    }
    break;
    case '\"': {
      char chDelim = *s->m_pstr;
      const char* p;
      int cCount = 0;
      s->m_pstr++; // skip quote
      type = OP_STRING;
      p = s->m_pstr;
      while (*s->m_pstr && *s->m_pstr != chDelim) {
        s->m_pstr++;
      }
      if (*s->m_pstr != chDelim) {
        return tok_set_t(tok, OP_END);
      }
      tok_set_tp(tok, type, p, s->m_pstr - p);
      s->m_pstr++; // skip quote
      return tok->type;
    }
    break;
    }
  }
  {
    int cCount = 0;
    const char* p = s->m_pstr;
    type = OP_INDENTIFIER;
    while (*s->m_pstr && (isalnum(*s->m_pstr) || *s->m_pstr == '_')) {
      s->m_pstr++;
    }
    tok_set_tp(tok, type, p, s->m_pstr - p);
    {
      typedef struct {
        char* pstrName;
        OPTYPE Op;
      } KEYWORD;
      static KEYWORD Keywords[] = {
        { "break", OP_BREAK },
        { "case", OP_CASE },
        { "default", OP_DEFAULT },
        { "else", OP_ELSE },
        { "for", OP_FOR },
        { "foreach", OP_FOREACH },
        { "function", OP_FUNCTION },
        { "if", OP_IF },
        { "return", OP_RETURN },
        { "switch", OP_SWITCH },
        { "uses", OP_USES },
        //{ "var", OP_VAR },
        { "while", OP_WHILE },
        { NULL, OP_END }
      };
      KEYWORD* k;
      for (k = Keywords; k->Op != OP_END; k++) {
        if (tok_eqstr(tok, k->pstrName)) {
          tok->type = k->Op;
          return tok->type;
        }
      }
    }
  }
  if (tok->s.l == 0) {
    return tok_set_tp(tok, OP_CHAR, s->m_pstr++, 1);
  }
  return tok->type;
}
int PeekToken(CTokenizer* s, tok_t* tok)
{
  const char* pstrOld = s->m_pstr; // backup old text position
  GetToken(s, tok);
  s->m_pstr = pstrOld;
  return tok->type;
}
int SkipTo(CTokenizer* s, tok_t* tok, OPTYPE Stop)
{
  int iBrace = 0;
  int iParen = 0;
  for (; ;) {
    if (tok->type == OP_BRACE_R) {
      iBrace--;
    }
    else if (tok->type == OP_PAREN_R) {
      iParen--;
    }
    if (tok->type == Stop && iBrace <= 0 && iParen <= 0) {
      return GetToken(s, tok);
    }
    if (tok->type == OP_END) {
      return tok->type;
    }
    else if (tok->type == OP_BRACE_L) {
      iBrace++;
    }
    else if (tok->type == OP_PAREN_L) {
      iParen++;
    }
    GetToken(s, tok);
  }
  return tok->type;
}
class IScriptCallback
{
public:
  virtual bool GetProperty(str_t s, var_t* out, int nout) = 0;
  virtual bool SetProperty(str_t s, const var_t* vValue) = 0;
  virtual bool Method(str_t s, var_t* pvArgs, int nArgs, var_t* out, int nout) = 0;
};
enum { MAX_VARIABLES = 100 };
enum { MAX_FUNCTIONS = 50 };
enum { MAX_OBJECTS = 20 };
enum { MAX_STACKDEPTH = 20 };
enum { MAX_ARGUMENTS = 12 };
typedef enum EXCEP {
  EXCEP_NOP = 0,
  EXCEP_SYNTAXERROR,
  EXCEP_TYPEMISMATCH,
  EXCEP_NAMEEXPECTED,
  EXCEP_CODEOVERFLOW,
  EXCEP_PARAMOVERFLOW,
  EXCEP_STACKOVERFLOW,
  EXCEP_SCOPE,
  EXCEP_NOTFOUND,
  EXCEP_KEYWORD,
  EXCEP_DUPLICATE,
  EXCEP_ERROR,
  EXCEP_PAREN,
  EXCEP_ARGUMENTS,
  EXCEP_BREAK,
  EXCEP_RETURN,
  EXCEP_CANCELLED,
};
typedef struct VARIABLE {
  char szName[MAX_KEYWORD_LEN];
  var_t vValue;
} VARIABLE;
typedef struct CALLBACKDEF {
  char szName[MAX_KEYWORD_LEN];
  IScriptCallback* pCallback;
} CALLBACKDEF;
typedef struct FUNCTIONDEF {
  char szName[MAX_KEYWORD_LEN];
  const char* pstrCode;
  int iLineNo;
  int iArgs; // 输入参数个数
  int oArgs; // 输出参数个数
} FUNCTIONDEF;
typedef struct STACKLEVEL {
  const char* pstrCode;
  int iLineNo;
  int iVarIndex;
  int iStackVarIndex;
  int t;
} STACKLEVEL;
typedef struct CScriptEngine {
  char* m_pstrScript;
  char* m_pstrStore;
  CTokenizer m_tok[1];
  tok_t m_t[1];
  char m_szErrorMsg[64];
  char m_szErrorCode[40];
  char m_szErrorLine[100];
  int m_iErrorLineNum;
  VARIABLE m_vars[MAX_VARIABLES];
  int m_nVars;
  CALLBACKDEF m_Objects[MAX_OBJECTS];
  int m_nObjects;
  FUNCTIONDEF m_Functions[MAX_FUNCTIONS];
  int m_nFunctions;
  STACKLEVEL m_Stack[MAX_STACKDEPTH];
  int m_nStackLevel;
  var_t m_StackVars[MAX_STACKDEPTH];
  int m_nStackVars;
  bool m_bCancelled;
} CScriptEngine;
int try_ret()
{
  int adsf = 0;
  return 0;
}

int throw_error(CScriptEngine* env, int n, const char* nstr) {
  printf("line: %d err: %s", env->m_tok->m_iLineNo, nstr );
  return 0;
}
#define TRY_RETURN(x) {int ret = x; if (EXCEP_NOP!=ret) { try_ret(); return ret; } }
#endif // !defined(AFX_TOKENIZER_H__20050805_FACF_B73F_147C_0080AD509054__INCLUDED_)

