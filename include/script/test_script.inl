// App.cpp : Defines the entry point for the console application.
//
#include "std/types_c.h"
//#include "cfile.h"
//#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <time.h>
//#include <windows.h>
#include "var.inl"
#include "ScriptEngine.inl"
#define MATHFUN1_DEF(NAME)  bool __func_##NAME(var_t* pvArgs, int nArgs, var_t* out) { if (nArgs != 1) { throw EXCEP_ARGUMENTS; } var_set_r8(out, NAME(var_todouble(pvArgs))); return true; }
MATHFUN1_DEF(sqrt)
MATHFUN1_DEF(sin)
MATHFUN1_DEF(cos)
bool __func_strlen(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  int res = 0;
  if (_VT_LPSTR == pvArgs->t) {
    res = strlen((char*)(pvArgs->v));
  }
  var_set_i4(out, res);
  return true;
}
bool __func_strupper(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  var_change_type(pvArgs, _VT_LPSTR, out);
  int cch = strlen((char*)(out->v));
  CharUpperBuffA((char*)(out->v), cch);
  return true;
}
bool __func_strlower(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  var_change_type(pvArgs, _VT_LPSTR, out);
  int cch = strlen((char*)(out->v));
  CharLowerBuffA((char*)(out->v), cch);
  return true;
}
bool __func_atoi(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  if (_VT_LPSTR == pvArgs->t) {
    int x = atoi((char*)(pvArgs->v));
    var_set_i4(out, x);
  }
  return true;
}
bool __func_substr(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs < 2) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_LPSTR || pvArgs[0].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  char* bstr = (char*)pvArgs[0].v;
  long cch = (long) strlen(bstr);
  int start = var_toint(pvArgs + 1);
  int len = nArgs == 3 ? var_toint(pvArgs + 2) : (cch);
  if (start >= cch) {
    var_set_impl(out, _VT_LPSTR, "", 0, 1);
    return true;
  }
  if (start + len > cch) {
    len -= start + len - cch;
  }
  var_set_impl(out, _VT_LPSTR, bstr + start, len, 1);
  return true;
}
bool __func_strstr(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 2) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_LPSTR || pvArgs[0].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[1].t != _VT_LPSTR || pvArgs[1].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  const char* pwstr = strstr((const char*)pvArgs[0].v, (const char*)pvArgs[1].v);
  if (pwstr == NULL) {
    var_set_impl(out, _VT_LPSTR, "", 0, 1);
  }
  else {
    var_set_lpstr(out, pwstr);
  }
  return true;
}
bool __func_strpos(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 2) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_LPSTR || pvArgs[0].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[1].t != _VT_LPSTR || pvArgs[1].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  const char* pwstr = strstr((const char*)pvArgs[0].v, (const char*)pvArgs[1].v);
  int ret = (pwstr == NULL) ? -1 : (long)(pwstr - (const char*)pvArgs[0].v);
  var_set_i4(out, ret);
  return true;
}
bool __func_fopen(var_t* pvArgs, int nArgs, var_t* out)
{
  FILE* hFile = NULL;
  if (nArgs != 2) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_LPSTR || pvArgs[0].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[1].t != _VT_LPSTR || pvArgs[1].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  hFile = fopen((const char*)pvArgs[0].v, (const char*)pvArgs[1].v);
  var_set_i4(out, (long)hFile);
  return true;
}
bool __func_fclose(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_I4) {
    throw EXCEP_ARGUMENTS;
  }
  FILE* hFile = *(FILE**) pvArgs[0].v;
  if (hFile == NULL) {
    return true;
  }
  fclose(hFile);
  return true;
}
bool __func_fgets(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_I4) {
    throw EXCEP_ARGUMENTS;
  }
  FILE* hFile = *(FILE**) pvArgs[0].v;
  if (hFile == NULL) {
    return true;
  }
  char szBuffer[400] = { 0 };
  int nRead = 0;
  for (; ;) {
    char c = '\0';
    fread(&c, 1, 1, hFile);
    if (c == '\r') {
      continue;
    }
    if (c == '\n') {
      break;
    }
    szBuffer[nRead] = c;
    if (++nRead >= sizeof(szBuffer) - 1) {
      break;
    }
  }
  var_set_lpstr(out, szBuffer);
  return true;
}
bool __func_fputs(var_t* pvArgs, int nArgs, var_t* out)
{
  int dwWritten = 0;
  if (nArgs != 2) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_I4) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[1].t != _VT_LPSTR || pvArgs[1].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  FILE* hFile = *(FILE**) pvArgs[0].v;
  if (hFile) {
    char szBuffer[405] = { 0 };
    strcpy(szBuffer, (const char*)pvArgs[1].v);
    strcat(szBuffer, "\r\n");
    dwWritten = fwrite(szBuffer, strlen(szBuffer), 1, hFile);
  }
  var_set_i4(out, dwWritten);
  return true;
}
bool __func_feof(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_I4) {
    throw EXCEP_ARGUMENTS;
  }
  FILE* hFile = *(FILE**) pvArgs[0].v;
  int ret = 0;
  if (hFile != NULL) {
    ret = feof(hFile);
  }
  var_set_i4(out, ret);
  return true;
}
bool __func_sleep(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  int ms = var_toint(pvArgs);
  Sleep(ms);
  return true;
}
#if 0
bool __func_eval(var_t* pvArgs, int nArgs, var_t* out)
{
  if (nArgs != 1) {
    throw EXCEP_ARGUMENTS;
  }
  if (pvArgs[0].t != _VT_LPSTR || pvArgs[0].v == NULL) {
    throw EXCEP_ARGUMENTS;
  }
  int iStack = __pushstack();
  tok.Assign((const char*)pvArgs[0].v, 1);
  t = tok.GetToken();
  __eval(out);
  __popstack(iStack);
  return true;
}
#endif
bool __func_exit(var_t* pvArgs, int nArgs, var_t* out)
{
  throw EXCEP_NOP;
  return true;
}
bool __func_print(var_t* pvArgs, int nArgs, var_t* out)
{
  char buf[1024];
  var_tostr(pvArgs, buf, 1024);
  printf(buf);
  return true;
}
#if 0
class Globals : public IScriptCallback
{
public:
  bool GetProperty(str_t pstrName, var_t* out, int nout) {
    if (str_cmp_c(pstrName, "now") == 0) {
      time_t rawtime;
      struct tm* timeinfo;
      time(&rawtime);
      timeinfo = localtime(&rawtime);
      var_set_lpstr(out, asctime(timeinfo));
      return true;
    }
    return false;
  }
  bool SetProperty(str_t pstrName, const var_t* vRet) {
    return false;
  }
  bool Method(str_t pstrName, var_t* pvArgs, int nArgs, var_t* out, int nout) {
    switch (pstrName.s[0]) {
    case 'a':
      if (str_cmp_c(pstrName, "atoi") == 0) {
        return __func_atoi(pvArgs, nArgs, out);
      }
      break;
    case 'e':
      //if( str_cmp_c(pstrName, "eval") == 0 ) return __func_eval(pvArgs, nArgs, out);
      if (str_cmp_c(pstrName, "exit") == 0) {
        return __func_exit(pvArgs, nArgs, out);
      }
      break;
    case 'f':
      if (str_cmp_c(pstrName, "fopen") == 0) {
        return __func_fopen(pvArgs, nArgs, out);
      }
      if (str_cmp_c(pstrName, "fclose") == 0) {
        return __func_fclose(pvArgs, nArgs, out);
      }
      if (str_cmp_c(pstrName, "fgets") == 0) {
        return __func_fgets(pvArgs, nArgs, out);
      }
      if (str_cmp_c(pstrName, "fputs") == 0) {
        return __func_fputs(pvArgs, nArgs, out);
      }
      if (str_cmp_c(pstrName, "feof") == 0) {
        return __func_feof(pvArgs, nArgs, out);
      }
      break;
    case 's':
      switch (pstrName.s[1]) {
      case 't':
        if (str_cmp_c(pstrName, "strlen") == 0) {
          return __func_strlen(pvArgs, nArgs, out);
        }
        if (str_cmp_c(pstrName, "strlen") == 0) {
          return __func_strlen(pvArgs, nArgs, out);
        }
        if (str_cmp_c(pstrName, "strstr") == 0) {
          return __func_strstr(pvArgs, nArgs, out);
        }
        if (str_cmp_c(pstrName, "strpos") == 0) {
          return __func_strpos(pvArgs, nArgs, out);
        }
        if (str_cmp_c(pstrName, "strupper") == 0) {
          return __func_strupper(pvArgs, nArgs, out);
        }
        if (str_cmp_c(pstrName, "strlower") == 0) {
          return __func_strlower(pvArgs, nArgs, out);
        }
        break;
      case 'u':
        if (str_cmp_c(pstrName, "substr") == 0) {
          return __func_substr(pvArgs, nArgs, out);
        }
        break;
      case 'l':
        if (str_cmp_c(pstrName, "sleep") == 0) {
          return __func_sleep(pvArgs, nArgs, out);
        }
        break;
      case 'q':
        if (str_cmp_c(pstrName, "sqrt") == 0) {
          return __func_sqrt(pvArgs, nArgs, out);
        }
        break;
      }
      break;
    case 't':
      if (str_cmp_c(pstrName, "tick") == 0) {
        unsigned int t = clock();
        var_set_i4(out, t);
        return true;
      }
      break;
    case 'p':
      if (str_cmp_c(pstrName, "print") == 0) {
        return __func_print(pvArgs, nArgs, out);
      }
      break;
    }
    return false;
  }
};
class App : public IScriptCallback
{
public:
  long a;
  bool GetProperty(str_t pstrName, var_t* out, int nout) {
    if (str_cmp_c(pstrName, "a") == 0) {
      var_set_i4(out, a)
      return true;
    }
    return false;
  }
  bool SetProperty(str_t pstrName, const var_t* vValue) {
    if (str_cmp_c(pstrName, "a") == 0) {
      a = var_toint(vValue);
      return true;
    }
    return false;
  }
  bool Method(str_t pstrName, var_t* pvArgs, int nArgs, var_t* out, int nout) {
    if (str_cmp_c(pstrName, "foo") == 0) {
      var_t v[1] = {0};
      var_set_i4(v, a);
      var_change_type(v, _VT_LPSTR, out);
      return true;
    }
    return false;
  }
};
int test_script()
{
  //int i=0, j=1, k=2;
  //i+=7+=k+=1;
  str_t code[1] = {0};
  _chdir("E:/code/cstd/include/script");
  str_load("test.txt", code);
  str_load("test1.txt", code);
  App app;
  Globals globals;
  CScriptEngine env[1] = {0};
  var_t out[10] = {0};
  RegisterGlobals(env, &globals);
  RegisterObject(env, "App", &app);
  int bRes = RunScript(env, code->s);
  bRes = Evaluate(env, "print(xyz)");
  str_free(code);
  return 0;
}
#else

#endif
