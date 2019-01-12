#if !defined(AFX_SCRIPTENGINE_H__20050805_F673_570F_6CD8_0080AD509054__INCLUDED_)
#define AFX_SCRIPTENGINE_H__20050805_F673_570F_6CD8_0080AD509054__INCLUDED_

#define MAX_KEYWORD_LEN 32
#define MAX_STRING_LEN 300
#include "std/string_c.h"
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
  char* s;
  int l;
} tok_t;
#define tok_set_t(tok, t) tok_set_tp(tok, t, 0, 0)
#define tok_eqstr(tok, str) (0==strnicmp_c((tok)->s, (tok)->l, str, strlen(str), -1, 0, 0))
int tok_set_tp(tok_t* tok, int t, const void* p, int len)
{
  tok->type = t;
  tok->s = (char*)p;
  tok->l = len;
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
  if (tok->l == 0) {
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
struct IScriptCallback
{
  virtual bool GetProperty(char* s, var_t* out, int nout) = 0;
  virtual bool SetProperty(char* s, const var_t* vValue) = 0;
  virtual bool Method(char* s, var_t* pvArgs, int nArgs, var_t* out, int nout) = 0;
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
typedef struct ScriptEngine {
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
} ScriptEngine;
int try_ret()
{
  int adsf = 0;
  return 0;
}

int throw_error(ScriptEngine* env, int n, const char* nstr) {
  printf("line: %d err: %s", env->m_tok->m_iLineNo, nstr );
  return 0;
}
#define TRY_RETURN(x) {int ret = x; if (EXCEP_NOP!=ret) { try_ret(); return ret; } }
int Clear(ScriptEngine* env)
{
  int i;
  for (i = 0; i < env->m_nVars; i++) {
    var_free(&env->m_vars[i].vValue);
  }
  for (i = 0; i < env->m_nStackVars; i++) {
    var_free(env->m_StackVars + i);
  }
  env->m_nVars = 0;
  env->m_nFunctions = 0;
  env->m_nStackLevel = 0;
  env->m_nStackVars = 0;
  env->m_bCancelled = false;
  ::ZeroMemory(env->m_szErrorMsg, sizeof(env->m_szErrorMsg));
  ::ZeroMemory(env->m_szErrorCode, sizeof(env->m_szErrorCode));
  ::ZeroMemory(env->m_szErrorLine, sizeof(env->m_szErrorLine));
  if (env->m_pstrStore != NULL) {
    free(env->m_pstrStore);
  }
  env->m_pstrStore = NULL;
  env->m_pstrScript = NULL;
  return EXCEP_NOP;
}
int Cancel(ScriptEngine* env)
{
  env->m_bCancelled = true;
  return EXCEP_NOP;
}
///////////////////////////////////////////////////////////////////////////////////////
int __level1(ScriptEngine* env, var_t* out);
int __var_assign(ScriptEngine* env, var_t* out);
int __var_retrieve(ScriptEngine* env, var_t* out, int nout);
int __level6(ScriptEngine* env, var_t* out)
{
  switch (env->m_t->type) {
  case OP_PAREN_L: {
    GetToken(env->m_tok, env->m_t);
    var_t var[1] = {0};
    TRY_RETURN(__level1(env, var));
    if (env->m_t->type != OP_PAREN_R) {
      THROW(EXCEP_PAREN);
    }
    var_copy(out, var);
    var_free(var);
  }
  break;
  case OP_STRING: {
    char* p = strdup_nop(env->m_t->s, env->m_t->l);
    var_set_lpstr(out, p);
    free(p);
  }
  break;
  case OP_INTEGER: {
    int var = atoi(env->m_t->s);
    var_set_i4(out, var);
  }
  break;
  case OP_NUMBER: {
    double var = atof(env->m_t->s);
    var_set_r8(out, var);
  }
  break;
  case OP_INDENTIFIER: {
    __var_retrieve(env, out, 1);
    switch (env->m_t->type) {
    case OP_DOT: {
      tok_t t[1] = {0};
      GetToken(env->m_tok, env->m_t);
      if (env->m_t->type != OP_INDENTIFIER) {
        THROW(EXCEP_NAMEEXPECTED);
      }
      if (tok_eqstr(env->m_t, "toString")) {
        CheckError(::var_change_type(out, _VT_LPSTR, out));
      }
      //else if( t == "toDate" ) CheckError(::var_change_type(out, out, 0, _VT_DATE));
      else if (tok_eqstr(env->m_t, "toInteger")) {
        CheckError(::var_change_type(out, _VT_I4, out));
      }
      else if (tok_eqstr(env->m_t, "toNumber")) {
        CheckError(::var_change_type(out, _VT_R8, out));
      }
      else {
        THROW(EXCEP_NOTFOUND);
      }
      if (GetToken(env->m_tok, t) != OP_PAREN_L) {
        THROW(EXCEP_PAREN);
      }
      if (GetToken(env->m_tok, t) != OP_PAREN_R) {
        THROW(EXCEP_PAREN);
      }
      tok_free(t);
    }
    break;
    case OP_INC:
    case OP_DEC: {
      int x = var_toint(out) + ((env->m_t->type == OP_INC) ? 1 : -1);
      var_set_i4(out, x);
    }
    break;
    default:
      return EXCEP_NOP;
    }
    break;
    default:
      return EXCEP_NOP;
    }
    break;
  }
  GetToken(env->m_tok, env->m_t);
  return EXCEP_NOP;
}
int __level5(ScriptEngine* env, var_t* out)
{
  bool bUnary = false;
  if (env->m_t->type == OP_PLUS || env->m_t->type == OP_MINUS) {
    bUnary = true;
    GetToken(env->m_tok, env->m_t);
  }
  TRY_RETURN(__level6(env, out));
  if (bUnary) {
    CheckError(::var_neg(out, out));
  }
  return EXCEP_NOP;
}
int __level4(ScriptEngine* env, var_t* out)
{
  TRY_RETURN(__level5(env, out));
  while (env->m_t->type == OP_NOT) {
    CheckError(::var_not(out, out));
    GetToken(env->m_tok, env->m_t);
  }
  return EXCEP_NOP;
}
int __level3(ScriptEngine* env, var_t* out)
{
  TRY_RETURN(__level4(env, out));
  while (env->m_t->type == OP_MUL || env->m_t->type == OP_DIV || env->m_t->type == OP_MOD) {
    int tOp = env->m_t->type;
    GetToken(env->m_tok, env->m_t);
    var_t var[1] = {0};
    //out->locked=1;
    TRY_RETURN(__level4(env, var));
    switch (tOp) {
    case OP_MUL:
      CheckError(var_mul(out, var, out));
      break;
    case OP_DIV:
      CheckError(var_div(out, var, out));
      break;
    case OP_MOD:
      CheckError(var_mod(out, var, out));
      break;
    }
    var_free(var);
  }
  return EXCEP_NOP;
}
int __level2(ScriptEngine* env, var_t* out)
{
  TRY_RETURN(__level3(env, out));
  while (env->m_t->type == OP_PLUS || env->m_t->type == OP_MINUS) {
    int tOp = env->m_t->type;
    GetToken(env->m_tok, env->m_t);
    var_t var[1] = {0};
    TRY_RETURN(__level3(env, var));
    switch (tOp) {
    case OP_PLUS:
      //var_change_type(var, out->t, var);
      CheckError(::var_add(out, var, out));
      break;
    case OP_MINUS:
      //var_change_type(var, out->t, var);
      CheckError(::var_sub(out, var, out));
      break;
    }
    var_free(var);
  }
  return EXCEP_NOP;
}
int __level1(ScriptEngine* env, var_t* out)
{
  TRY_RETURN(__level2(env, out));
  while (env->m_t->type == OP_CMP_EQU || env->m_t->type == OP_CMP_NEQ || env->m_t->type == OP_CMP_LSS || env->m_t->type == OP_CMP_GTR || env->m_t->type == OP_CMP_LEQ || env->m_t->type == OP_CMP_GEQ) {
    int tOp = env->m_t->type;
    GetToken(env->m_tok, env->m_t);
    var_t var[1] = {0};
    TRY_RETURN(__level2(env, var));
    int ret = 0, x = var_cmp(out, var);
    switch (tOp) {
    case OP_CMP_EQU:
      ret = x == 0;
      break;
    case OP_CMP_NEQ:
      ret = x != 0;
      break;
    case OP_CMP_LSS:
      ret = x < 0;
      break;
    case OP_CMP_GTR:
      ret = x > 0;
      break;
    case OP_CMP_LEQ:
      ret = x <= 0;
      break;
    case OP_CMP_GEQ:
      ret = x >= 0;
      break;
    default:
      ASSERT(0);
      break;
    }
    var_set_i4(out, ret);
    var_free(var);
  }
  return EXCEP_NOP;
}
int __level0(ScriptEngine* env, var_t* out)
{
  TRY_RETURN(__level1(env, out));
  while (env->m_t->type == OP_AND || env->m_t->type == OP_OR || env->m_t->type == OP_XOR) {
    int tOp = env->m_t->type;
    GetToken(env->m_tok, env->m_t);
    var_t var[1] = {0};
    TRY_RETURN(__level1(env, var));
    switch (tOp) {
    case OP_AND:
      CheckError(::var_and(out, var, out));
      break;
    case OP_OR:
      CheckError(::var_or(out, var, out));
      break;
    case OP_XOR:
      CheckError(::var_xor(out, var, out));
      break;
    }
    var_free(var);
  }
  return EXCEP_NOP;
}
int __eval(ScriptEngine* env, var_t* out)
{
  TRY_RETURN(__level0(env, out));
  while (env->m_t->type == OP_COMMA) {
    GetToken(env->m_tok, env->m_t);
    TRY_RETURN(__level0(env, out));
  }
  return EXCEP_NOP;
}
int __pushstack(ScriptEngine* env)
{
  STACKLEVEL& stack = env->m_Stack[env->m_nStackLevel];
  memset(&stack, 0, sizeof(STACKLEVEL));
  stack.iVarIndex = env->m_nVars;
  stack.iStackVarIndex = env->m_nStackVars;
  stack.pstrCode = env->m_tok->m_pstr;
  stack.iLineNo = env->m_tok->m_iLineNo;
  stack.t = env->m_t->type;
  if (++env->m_nStackLevel >= MAX_STACKDEPTH) {
    THROW(EXCEP_STACKOVERFLOW);
  }
  return env->m_nStackLevel;
}
int __popstack(ScriptEngine* env, int iStack)
{
  STACKLEVEL& stack = env->m_Stack[iStack - 1];
  int i;
  for (i = env->m_nVars - 1; i >= stack.iVarIndex; --i) {
    ::var_free(&env->m_vars[i].vValue);
  }
  for (i = env->m_nStackLevel - 1; i >= stack.iStackVarIndex; --i) {
    ::var_free(&env->m_StackVars[i]);
  }
  Assign(env->m_tok, stack.pstrCode, stack.iLineNo);
  env->m_nVars = stack.iVarIndex;
  env->m_nStackVars = stack.iStackVarIndex;
  tok_set_t(env->m_t, stack.t);
  env->m_nStackLevel = iStack - 1;
  return EXCEP_NOP;
}
int __statement(ScriptEngine* env);
int __exec(ScriptEngine* env, OPTYPE Stop);
int __block(ScriptEngine* env)
{
  if (env->m_t->type == OP_SEMICOLON) {
    return EXCEP_NOP;
  }
  if (env->m_t->type != OP_BRACE_L) {
    TRY_RETURN(__statement(env));
    GetToken(env->m_tok, env->m_t);
  }
  else {
    int iStack = __pushstack(env);
    int e = __exec(env, OP_BRACE_R);
    __popstack(env, iStack);
    SkipTo(env->m_tok, env->m_t, OP_BRACE_R);
    TRY_RETURN(e);
  }
  if (env->m_t->type == OP_SEMICOLON) {
    GetToken(env->m_tok, env->m_t);
  }
  return EXCEP_NOP;
}
int __var_find_define(ScriptEngine* env, const char* tName, int tNameLen);
int __var_assign(ScriptEngine* env, var_t* out)
{
  int i;
  tok_t tName[1] = {0};
  tok_t t[1] = {0};
  tok_cpy(tName, env->m_t);
  int tOp = PeekToken(env->m_tok, t);
  switch (tOp) {
  case OP_PAREN_L: {
    TRY_RETURN(__var_retrieve(env, out, 1));
    tok_free(tName);
    tok_free(t);
  }
  return EXCEP_NOP;
  case OP_EQUAL:
  case OP_PLUSEQUAL:
  case OP_MINUSEQUAL:
    GetToken(env->m_tok, env->m_t);
    GetToken(env->m_tok, env->m_t);
    TRY_RETURN(__eval(env, out));
    break;
  case OP_INC:
  case OP_DEC:
    TRY_RETURN(__eval(env, out));
    break;
  case OP_DOT:
    break;
  default:
    ASSERT(EXCEP_SYNTAXERROR);
    break;
  }
  //for (int i = env->m_nVars - 1; i >= 0; --i)
  {
    i = __var_find_define(env, tName->s, tName->l);
    //if (tok_eqstr(tName, env->m_vars[i].szName))
    {
      switch (tOp) {
      case OP_INC:
      case OP_DEC:
      case OP_EQUAL:
        CheckError(::var_copy(&env->m_vars[i].vValue, out));
        break;
      case OP_PLUSEQUAL:
        CheckError(::var_add(&env->m_vars[i].vValue, out, &env->m_vars[i].vValue));
        break;
      case OP_MINUSEQUAL:
        CheckError(::var_sub(&env->m_vars[i].vValue, out, &env->m_vars[i].vValue));
        break;
      default:
        THROW(EXCEP_SYNTAXERROR);
      }
      var_copy(out, &env->m_vars[i].vValue);
      tok_free(t);
      tok_free(tName);
      return EXCEP_NOP;
    }
  }
  for (i = 0; i < env->m_nObjects; i++) {
    if (strlen(env->m_Objects[i].szName) == 0) {
      if (env->m_Objects[i].pCallback->SetProperty(tName->s, out)) {
        tok_free(t);
        tok_free(tName);
        return EXCEP_NOP;
      }
    }
  }
  for (i = 0; i < env->m_nObjects; i++) {
    if (tok_eqstr(tName, env->m_Objects[i].szName)) {
      if (GetToken(env->m_tok, env->m_t) != OP_DOT) {
        THROW(EXCEP_NAMEEXPECTED);
      }
      GetToken(env->m_tok, env->m_t);
      tok_cpy(tName, env->m_t);
      if (PeekToken(env->m_tok, t) == OP_EQUAL) {
        GetToken(env->m_tok, env->m_t);
        GetToken(env->m_tok, env->m_t);
        TRY_RETURN(__eval(env, out));
      }
      if (!env->m_Objects[i].pCallback->SetProperty(tName->s, out)) {
        THROW(EXCEP_NOTFOUND);
      }
      tok_free(tName);
      tok_free(t);
      return EXCEP_NOP;
    }
  }
  THROW(EXCEP_NOTFOUND);
  tok_free(tName);
  tok_free(t);
  return EXCEP_NOP;
}
int __var_getargs(ScriptEngine* env, var_t* pArgs, int& nArgs);
int __var_define(ScriptEngine* env);
int __var_define(ScriptEngine* env, const char* pstrName, int NameLen, var_t* vValue);
int __fun_call(ScriptEngine* env, FUNCTIONDEF* pFunction, const var_t* args, int nArgs, var_t* out, int nout)
{
  int iStack = __pushstack(env);
  int j = 0;
  tok_t t[1] = {0};
  var_t v[1] = {0};
  Assign(env->m_tok, pFunction->pstrCode, pFunction->iLineNo);
  for (j = 0; j < pFunction->oArgs; ++j) {
    GetToken(env->m_tok, t);
    if (t->type != OP_INDENTIFIER) {
      THROW(EXCEP_SYNTAXERROR);
    }
    TRY_RETURN(__var_define(env, t->s, t->l, v));
    GetToken(env->m_tok, t);
    if (t->type == OP_COMMA) {
    }
    else if (t->type != OP_EQUAL) {
      THROW(EXCEP_PAREN);
    }
  }
  GetToken(env->m_tok, t);
  if (t->type != OP_INDENTIFIER) {
    THROW(EXCEP_SYNTAXERROR);
  }
  if (strcmp_c(t->s, t->l, pFunction->szName, -1) != 0) {
    THROW(EXCEP_SYNTAXERROR);
  }
  GetToken(env->m_tok, t);
  if (t->type != OP_PAREN_L) {
    THROW(EXCEP_SYNTAXERROR);
  }
  GetToken(env->m_tok, env->m_t);
  j = 0;
  while (env->m_t->type == OP_INDENTIFIER) {
    if (j < nArgs) {
      var_copy(v, &args[j++]);
    }
    TRY_RETURN(__var_define(env, env->m_t->s, env->m_t->l, v));
    GetToken(env->m_tok, env->m_t);
    if (env->m_t->type == OP_COMMA) {
      GetToken(env->m_tok, env->m_t);
    }
    else if (env->m_t->type != OP_PAREN_R) {
      THROW(EXCEP_PAREN);
    }
  }
  var_free(v);
  if (env->m_t->type != OP_PAREN_R) {
    THROW(EXCEP_PAREN);
  }
  if (GetToken(env->m_tok, t) != OP_BRACE_L) {
    THROW(EXCEP_PAREN);
  }
  tok_free(t);
  {
    int e = __exec(env, OP_BRACE_R);
    if (e != EXCEP_NOP) {
      if (e != EXCEP_RETURN) {
        return e;
      }
      else {
        //int iStackVar = env->m_Stack[iStack - 1].iStackVarIndex;
        //ASSERT(iStackVar<=env->m_nStackVars);
        //::var_copy(out, &env->m_StackVars[env->m_nStackVars-1]);
      }
    }
  }
  nout = MIN(nout, pFunction->oArgs);
  for (j = 0; j < nout; ++j) {
    var_copy(out + j, &env->m_vars[env->m_Stack[iStack - 1].iVarIndex + j].vValue);
  }
  __popstack(env, iStack);
  return EXCEP_NOP;
}
int __var_retrieve(ScriptEngine* env, var_t* out, int nout)
{
  int i;
  tok_t tName[1] = {0};
  tok_t t[1] = {0};
  for (i = env->m_nVars - 1; i >= 0; --i) {
    if (tok_eqstr(env->m_t, env->m_vars[i].szName)) {
      ::var_copy(out, &env->m_vars[i].vValue);
      GetToken(env->m_tok, env->m_t);
      return EXCEP_NOP;
    }
  }
  for (i = 0; i < env->m_nObjects; i++) {
    if (tok_eqstr(env->m_t, env->m_Objects[i].szName)) {
      if (GetToken(env->m_tok, tName) != OP_DOT) {
        THROW(EXCEP_NAMEEXPECTED);
      }
      GetToken(env->m_tok, tName);
      GetToken(env->m_tok, env->m_t);
      if (env->m_t->type == OP_PAREN_L) {
        GetToken(env->m_tok, env->m_t);
        int nArgs = MAX_ARGUMENTS;
        var_t args[MAX_ARGUMENTS];
        TRY_RETURN(__var_getargs(env, args, nArgs));
        if (!env->m_Objects[i].pCallback->Method(tName->s, args, nArgs, out, nout)) {
          THROW(EXCEP_NOTFOUND);
        }
      }
      else {
        if (!env->m_Objects[i].pCallback->GetProperty(tName->s, out, nout)) {
          THROW(EXCEP_NOTFOUND);
        }
      }
      tok_free(tName);
      return EXCEP_NOP;
    }
  }
  tok_cpy(tName, env->m_t);
  GetToken(env->m_tok, env->m_t);
  if (env->m_t->type == OP_PAREN_L) {
    GetToken(env->m_tok, env->m_t);
    int i, nArgs = MAX_ARGUMENTS;
    var_t args[MAX_ARGUMENTS] = {0};
    TRY_RETURN(__var_getargs(env, args, nArgs));
    for (i = 0; i < env->m_nFunctions; i++) {
      if (tok_eqstr(tName, env->m_Functions[i].szName)) {
        return __fun_call(env, env->m_Functions + i, args, nArgs, out, nout);
      }
    }
    for (i = 0; i < env->m_nObjects; i++) {
      if (strlen(env->m_Objects[i].szName) == 0) {
        if (env->m_Objects[i].pCallback->Method(tName->s, args, nArgs, out, nout)) {
          return EXCEP_NOP;
        }
      }
    }
  }
  else {
    for (i = 0; i < env->m_nObjects; i++) {
      if (strlen(env->m_Objects[i].szName) == 0) {
        if (env->m_Objects[i].pCallback->GetProperty(tName->s, out, nout)) {
          return EXCEP_NOP;
        }
      }
    }
  }
  THROW(EXCEP_NOTFOUND);
}
int __break(ScriptEngine* env)
{
  return EXCEP_BREAK;
}
int __return(ScriptEngine* env)
{
  if (env->m_nStackLevel == 0) {
    THROW(EXCEP_SCOPE);
  }
  if (env->m_nStackVars >= MAX_STACKDEPTH) {
    THROW(EXCEP_STACKOVERFLOW);
  }
  GetToken(env->m_tok, env->m_t);
  var_t var[1] = {0};
  TRY_RETURN(__eval(env, var));
  var_free(var);
  if (env->m_t->type != OP_SEMICOLON) {
    THROW(EXCEP_SYNTAXERROR);
  }
  for (int i = 0; i < env->m_nStackVars - 1; i++) {
    //::var_copy(&env->m_Stack[i].vRet, &stack.vRet);
  }
  THROW(EXCEP_RETURN);
}
//
int __func_define(ScriptEngine* env)
{
  if (env->m_nStackLevel != 0) {
    THROW(EXCEP_SCOPE);
  }
  tok_t tName[1] = {0};
  tok_t t[1] = {0};
  int tOp;
  const char* pstrCode = env->m_tok->m_pstr;
  int iargs = 0, oargs = 0;
  GetToken(env->m_tok, tName);
  if (tName->type != OP_INDENTIFIER) {
    THROW(EXCEP_NAMEEXPECTED);
  }
  for (; 1;) {
    tOp = GetToken(env->m_tok, t);
    if (OP_COMMA == tOp) {
      ++oargs;
      tOp = GetToken(env->m_tok, t);
      if (tName->type != OP_INDENTIFIER) {
        THROW(EXCEP_NAMEEXPECTED);
      }
      continue;
    }
    else if (OP_PAREN_L == tOp) {
      break;
    }
    else if (OP_EQUAL == tOp) {
      ++oargs;
      tOp = GetToken(env->m_tok, t);
      if (tName->type != OP_INDENTIFIER) {
        THROW(EXCEP_NAMEEXPECTED);
      }
      tok_cpy(tName, t);
      tOp = GetToken(env->m_tok, t);
      break;
    }
    THROW(EXCEP_NAMEEXPECTED);
  }
  for (int i = 0; i < env->m_nFunctions; i++) {
    if (tok_eqstr(env->m_t, env->m_Functions[i].szName)) {
      THROW(EXCEP_DUPLICATE);
    }
  }
  FUNCTIONDEF& func = env->m_Functions[env->m_nFunctions];
  strcpy_c(func.szName, 0, MAX_KEYWORD_LEN, tName->s, tName->l);
  func.iArgs = iargs;
  func.oArgs = oargs;
  if (tOp != OP_PAREN_L) {
    THROW(EXCEP_PAREN);
  }
  func.pstrCode = pstrCode;
  func.iLineNo = env->m_tok->m_iLineNo;
  SkipTo(env->m_tok, env->m_t, OP_PAREN_R);
  if (env->m_t->type != OP_BRACE_L) {
    THROW(EXCEP_PAREN);
  }
  SkipTo(env->m_tok, env->m_t, OP_BRACE_R);
  if (++env->m_nFunctions >= MAX_FUNCTIONS) {
    THROW(EXCEP_CODEOVERFLOW);
  }
  return EXCEP_NOP;
}
int __var_getargs(ScriptEngine* env, var_t* pArgs, int& nArgs)
{
  int nMaxArgs = nArgs;
  nArgs = 0;
  while (env->m_t->type != OP_PAREN_R) {
    if (nArgs >= nMaxArgs) {
      THROW(EXCEP_PARAMOVERFLOW);
    }
    TRY_RETURN(__level0(env, &pArgs[nArgs++]));
    if (env->m_t->type == OP_COMMA) {
      GetToken(env->m_tok, env->m_t);
    }
    else if (env->m_t->type != OP_PAREN_R) {
      THROW(EXCEP_PAREN);
    }
  }
  if (env->m_t->type != OP_PAREN_R) {
    THROW(EXCEP_PAREN);
  }
  GetToken(env->m_tok, env->m_t);
  return EXCEP_NOP;
}
int __var_define(ScriptEngine* env)
{
  do {
    tok_t tName[1] = {0};
    GetToken(env->m_tok, tName);
    if (tName->type != OP_INDENTIFIER) {
      THROW(EXCEP_NAMEEXPECTED);
    }
    int iNameIndex = env->m_nStackLevel == 0 ? 0 : env->m_Stack[env->m_nStackLevel - 1].iVarIndex + 1;
    while (iNameIndex < env->m_nVars) {
      if (tok_eqstr(tName, env->m_vars[iNameIndex++].szName)) {
        THROW(EXCEP_DUPLICATE);
      }
    }
    VARIABLE& var = env->m_vars[env->m_nVars];
    var_t v[1] = {0};
    TRY_RETURN(__var_define(env, tName->s, tName->l, v));
    GetToken(env->m_tok, env->m_t);
    if (env->m_t->type == OP_EQUAL) {
      GetToken(env->m_tok, env->m_t);
      TRY_RETURN(__level0(env, &var.vValue));
    }
  }
  while (env->m_t->type == OP_COMMA);
  if (env->m_t->type != OP_SEMICOLON) {
    THROW(EXCEP_SYNTAXERROR);
  }
  //GetToken(env->m_tok, env->m_t);
  return EXCEP_NOP;
}
int __var_define(ScriptEngine* env, const char* pstrName, int pstrNameLen, var_t* vValue)
{
  VARIABLE& var = env->m_vars[env->m_nVars];
  memset(&var.vValue, 0, sizeof(VARIABLE));
  strcpy_c(var.szName, 0, MAX_KEYWORD_LEN, pstrName, pstrNameLen);
  if (vValue) {
    ::var_copy(&var.vValue, vValue);
  }
  if (++env->m_nVars >= MAX_VARIABLES) {
    THROW(EXCEP_CODEOVERFLOW);
  }
  return EXCEP_NOP;
}
int __var_find_define(ScriptEngine* env, const char* tName, int tNameLen)
{
  int i;
  for (i = env->m_nVars; --i >= 0;) {
    if (0 == strcmp_c(tName, tNameLen, env->m_vars[i].szName, -1)) {
      return i;
    }
  }
  __var_define(env, tName, tNameLen, NULL);
  return env->m_nVars - 1;
}
int env_var_get(ScriptEngine* env, const char* tName, int tNameLen, var_t* v)
{
  int i;
  for (i = env->m_nVars; --i > 0;) {
    if (0 == strcmp_c(tName, tNameLen, env->m_vars[i].szName, -1)) {
      var_copy(v, &env->m_vars[i].vValue);
      return i;
    }
  }
  return 0;
}
int __forloop(ScriptEngine* env)
{
  tok_t t[1] = {0};
  var_t tmp[1] = {0};
  if (GetToken(env->m_tok, t) != OP_PAREN_L) {
    THROW(EXCEP_PAREN);
  }
  int iStack = __pushstack(env);
  GetToken(env->m_tok, env->m_t);
#if 0
  if (env->m_t->type == OP_VAR) {
    TRY_RETURN(__var_define(env));
  }
  else
#endif
  {
    TRY_RETURN(__var_assign(env, tmp));
  }
  const char* pstrEval = env->m_tok->m_pstr;
  for (; ;) {
    GetToken(env->m_tok, env->m_t);
    var_t eval[1] = {0};
    TRY_RETURN(__eval(env, eval));
    if (var_toint(eval) != 1) {
      break;
    }
    if (env->m_t->type != OP_SEMICOLON) {
      THROW(EXCEP_SYNTAXERROR);
    }
    const char* pstrContinue = env->m_tok->m_pstr;
    SkipTo(env->m_tok, env->m_t, OP_PAREN_R);
    {
      int e = __block(env);
      if (e == EXCEP_BREAK) {
        break;
      }
      else if (e != EXCEP_NOP) {
        return e;
      }
    }
    env->m_tok->m_pstr = pstrContinue;
    GetToken(env->m_tok, env->m_t);
    if (env->m_t->type != OP_PAREN_R) {
      TRY_RETURN(__var_assign(env, tmp));
    }
    if (env->m_t->type != OP_PAREN_R) {
      THROW(EXCEP_PAREN);
    }
    env->m_tok->m_pstr = pstrEval;
    var_free(eval);
  }
  var_free(tmp);
  __popstack(env, iStack);
  SkipTo(env->m_tok, env->m_t, OP_BRACE_R);
  return EXCEP_NOP;
}
int __whileloop(ScriptEngine* env)
{
  GetToken(env->m_tok, env->m_t);
  if (env->m_t->type != OP_PAREN_L) {
    THROW(EXCEP_PAREN);
  }
  int iStack = __pushstack(env);
  for (const char* pstrContinue = env->m_tok->m_pstr; ; env->m_tok->m_pstr = pstrContinue) {
    GetToken(env->m_tok, env->m_t);
    var_t eval[1] = {0};
    TRY_RETURN(__eval(env, eval));
    if (var_toint(eval) != 1) {
      break;
    }
    if (env->m_t->type != OP_PAREN_R) {
      THROW(EXCEP_PAREN);
    }
    GetToken(env->m_tok, env->m_t);
    {
      int e = __block(env);
      if (e == EXCEP_BREAK) {
        break;
      }
      else if (e != EXCEP_NOP) {
        return e;
      }
    }
    var_free(eval);
  }
  __popstack(env, iStack);
  SkipTo(env->m_tok, env->m_t, OP_BRACE_R);
  return EXCEP_NOP;
}
int __skipblock(ScriptEngine* env)
{
  if (env->m_t->type == OP_BRACE_L) {
    SkipTo(env->m_tok, env->m_t, OP_BRACE_R);
  }
  else {
    SkipTo(env->m_tok, env->m_t, OP_SEMICOLON);
  }
  return EXCEP_NOP;
}
int __ifcondit(ScriptEngine* env)
{
  tok_t t[1] = {0};
  int x;
  if (GetToken(env->m_tok, t) != OP_PAREN_L) {
    THROW(EXCEP_PAREN);
  }
  GetToken(env->m_tok, env->m_t);
  var_t eval[1] = {0};
  TRY_RETURN(__eval(env, eval));
  GetToken(env->m_tok, env->m_t);
  x = var_toint(eval);
  if (x != 0) {
    TRY_RETURN(__block(env));
    while (env->m_t->type == OP_ELSE) {
      GetToken(env->m_tok, env->m_t);
      if (env->m_t->type == OP_IF) {
        GetToken(env->m_tok, env->m_t);
        SkipTo(env->m_tok, env->m_t, OP_PAREN_R);
      }
      __skipblock(env);
    }
  }
  else {
    __skipblock(env);
    while (env->m_t->type == OP_ELSE) {
      GetToken(env->m_tok, env->m_t);
      if (env->m_t->type == OP_IF) {
        TRY_RETURN(__ifcondit(env));
      }
      else {
        TRY_RETURN(__block(env));
      }
    }
  }
  var_free(eval);
  return EXCEP_NOP;
}
int __switchcondit(ScriptEngine* env)
{
  tok_t t[1] = {0};
  if (GetToken(env->m_tok, t) != OP_PAREN_L) {
    THROW(EXCEP_PAREN);
  }
  GetToken(env->m_tok, env->m_t);
  var_t eval[1] = {0};
  TRY_RETURN(__eval(env, eval));
  if (env->m_t->type != OP_PAREN_R) {
    THROW(EXCEP_PAREN);
  }
  int iStack = __pushstack(env);
  if (GetToken(env->m_tok, t) != OP_BRACE_L) {
    THROW(EXCEP_PAREN);
  }
  GetToken(env->m_tok, env->m_t);
  while (env->m_t->type == OP_CASE) {
    GetToken(env->m_tok, env->m_t);
    var_t val[1] = {0};
    TRY_RETURN(__eval(env, val));
    if (env->m_t->type != OP_COLON) {
      THROW(EXCEP_SYNTAXERROR);
    }
    if (0 == var_cmp(eval, val)) {
      int e = __exec(env, OP_BRACE_R);
      if (e != EXCEP_BREAK && e != EXCEP_NOP) {
        return e;
      }
      break;
    }
    else {
      while (env->m_t->type != OP_CASE && env->m_t->type != OP_DEFAULT && env->m_t->type != OP_BRACE_R && env->m_t->type != OP_END) {
        TRY_RETURN(__skipblock(env));
      }
      if (env->m_t->type == OP_DEFAULT) {
        if (GetToken(env->m_tok, t) != OP_COLON) {
          THROW(EXCEP_SYNTAXERROR);
        }
        GetToken(env->m_tok, env->m_t);
        TRY_RETURN(__block(env));
        break;
      }
    }
    var_free(val);
  }
  __popstack(env, iStack);
  SkipTo(env->m_tok, env->m_t, OP_BRACE_R);
  var_free(eval);
  return EXCEP_NOP;
}
int __statement(ScriptEngine* env)
{
  switch (env->m_t->type) {
    //case OP_VAR: TRY_RETURN(__var_define(env)); break;
  case OP_INDENTIFIER:
    {
      var_t tmp[1] = {0};
      TRY_RETURN(__var_assign(env, tmp));
      var_free(tmp);
    }
    break;
  case OP_FUNCTION:
    TRY_RETURN(__func_define(env));
    break;
  case OP_IF:
    TRY_RETURN(__ifcondit(env));
    break;
  case OP_FOR:
    TRY_RETURN(__forloop(env));
    break;
  case OP_WHILE:
    TRY_RETURN(__whileloop(env));
    break;
  case OP_SWITCH:
    TRY_RETURN(__switchcondit(env));
    break;
  case OP_BRACE_L:
    TRY_RETURN(__block(env));
    break;
  case OP_BREAK:
    TRY_RETURN(__break(env));
    break;
  case OP_RETURN:
    TRY_RETURN(__return(env));
    return EXCEP_NOP;
  case OP_SEMICOLON:
    GetToken(env->m_tok, env->m_t);
    break;
  default:
    THROW(EXCEP_KEYWORD);
  }
  return EXCEP_NOP;
}
int __exec(ScriptEngine* env, OPTYPE Stop)
{
  GetToken(env->m_tok, env->m_t);
  while (env->m_t->type != OP_END && env->m_t->type != Stop) {
    TRY_RETURN(__statement(env));
    if (env->m_bCancelled) {
      THROW(EXCEP_CANCELLED);
    }
  }
  return EXCEP_NOP;
}
int __run(ScriptEngine* env, const char* pstrCode)
{
  Assign(env->m_tok, pstrCode, 1);
  TRY_RETURN(__exec(env, OP_END));
  return EXCEP_NOP;
}
int RunScript(ScriptEngine* env, const char* pstrCode)
{
  Clear(env);
  int cchLen = strlen(pstrCode) + 1;
  env->m_pstrStore = env->m_pstrScript = (char*) malloc((sizeof(char) * cchLen));
  if (env->m_pstrStore == NULL) {
    return false;
  }
  env->m_pstrStore[0] = '\0';
  strcpy(env->m_pstrStore, pstrCode);
  TRY_RETURN(__run(env, env->m_pstrStore));
  return EXCEP_NOP;
}
#include "std/fileio_c.h"
int RunScriptFromFile(ScriptEngine* env, const char* pstrFilename)
{
  FILE* hFile = fopen(pstrFilename, "rb");
  if (hFile == NULL) {
    return false;
  }
  size_t dwSize = fsize(hFile);
  char* pstrCode = (char*) malloc(dwSize + 1);
  DWORD dwRead = 0;
  dwRead = fread(pstrCode, dwSize, 1, hFile);
  fclose(hFile);
  if (dwRead != dwSize) {
    free(pstrCode);
    return false;
  }
  pstrCode[dwSize] = '\0';
  int bRes = RunScript(env, pstrCode);
  free(pstrCode);
  return bRes;
}
bool Evaluate(ScriptEngine* env, const char* pstrCode)
{
  ::ZeroMemory(env->m_szErrorMsg, sizeof(env->m_szErrorMsg));
  int cchLen = strlen(pstrCode) + 1;
  char* pwstr = env->m_pstrScript = (char*) malloc((sizeof(char) * cchLen));
  if (pwstr == NULL) {
    return false;
  }
  pwstr[0] = '\0';
  strcpy(pwstr, pstrCode);
  int iStack = __pushstack(env);
  __run(env, pwstr);
  __popstack(env, iStack);
  free(pwstr);
  return strlen(env->m_szErrorMsg) == 0;
}
bool RegisterObject(ScriptEngine* env, const char* pstrName, IScriptCallback* pCallback)
{
  if (env->m_nObjects >= MAX_OBJECTS) {
    return false;
  }
  CALLBACKDEF& obj = env->m_Objects[env->m_nObjects];
  ZeroMemory(&obj, sizeof(obj));
  strcpy(obj.szName, pstrName);
  obj.pCallback = pCallback;
  env->m_nObjects++;
  return true;
}
bool RegisterGlobals(ScriptEngine* env, IScriptCallback* pCallback)
{
  return RegisterObject(env, "", pCallback);
}
int GetErrorMessage(ScriptEngine* env, int& iLineNum, char* pstrMessage, int cchMessage, char* pstrCode, int cchCode, char* pstrLine, int cchLine)
{
  iLineNum = env->m_iErrorLineNum;
  if (pstrMessage != NULL) {
    strncpy(pstrMessage, env->m_szErrorMsg, cchMessage);
  }
  if (pstrCode != NULL) {
    strncpy(pstrCode, env->m_szErrorCode, cchCode);
  }
  if (pstrLine != NULL) {
    strncpy(pstrLine, env->m_szErrorLine, cchLine);
  }
  return 0;
}
int _InterpretError(ScriptEngine* env, EXCEP e)
{
  struct {
    EXCEP e;
    LPCTSTR pstrMessage;
  } Errors[] = {
    { EXCEP_SYNTAXERROR, "Syntax error" },
    { EXCEP_TYPEMISMATCH, "Type mismatch" },
    { EXCEP_NAMEEXPECTED, "Name exptected" },
    { EXCEP_CODEOVERFLOW, "Compiler limitation reached" },
    { EXCEP_PARAMOVERFLOW, "Too many parameters" },
    { EXCEP_STACKOVERFLOW, "Stack overflow" },
    { EXCEP_SCOPE, "Invalid scope" },
    { EXCEP_NOTFOUND, "Undefined" },
    { EXCEP_KEYWORD, "Keyword expected" },
    { EXCEP_DUPLICATE, "Duplicated entry" },
    { EXCEP_PAREN, "Parens mixup" },
    { EXCEP_ARGUMENTS, "Bad arguments" },
    { EXCEP_ERROR, "Error" },
    { EXCEP_BREAK, "Unscoped break statement" },
    { EXCEP_RETURN, "Unscoped return statement" },
    { EXCEP_CANCELLED, "Cancelled" }
  };
  for (int i = 0; i < sizeof(Errors) / sizeof(Errors[0]); i++) {
    if (Errors[i].e == e) {
      strcpy(env->m_szErrorMsg, Errors[i].pstrMessage);
    }
  }
  env->m_iErrorLineNum = env->m_tok->m_iLineNo;
  const char* p = env->m_tok->m_pstr - env->m_t->l;
  if (p < env->m_pstrScript) {
    p = env->m_pstrScript;
  }
  strcpy(env->m_szErrorCode, p);
  while (p > env->m_pstrScript && *(p - 1) != '\n') {
    p--;
  }
  const char* pLen = p;
  while (*pLen != '\0' && *pLen != '\n' && *pLen != '\r') {
    pLen++;
  }
  strcpy(env->m_szErrorLine, p);
  return 0;
}
#endif // !defined(AFX_SCRIPTENGINE_H__20050805_F673_570F_6CD8_0080AD509054__INCLUDED_)

