
#ifndef _STDC_FLAGS_C_H_
#define _STDC_FLAGS_C_H_

#include "error_c.h"
#include "string_c.h"
#define PARAMTYPEDEF_DEF(DEF) \
  DEF(int, FLAGS_INT) \
  DEF(bool, FLAGS_BOOLEAN) \
  DEF(double, FLAGS_REAL) \
  DEF(flags_string, FLAGS_STRING) \
  DEF(float, FLAGS_FLOAT) \
  DEF(unsigned int, FLAGS_UNSIGNED_INT) \
  DEF(int64, FLAGS_UINT64) \
DEF(unsigned char, FLAGS_UCHAR)


#define _FLAGS_HPP_
#ifndef _FLAGS_HPP_
#define _FLAGS_HPP_

#include <stdarg.h>
#include <assert.h>
//#include "wstd/flags_string.hpp"


void printMessage() const
{
  if (this->about_message != "") {
    printf("%s\n", this->about_message.c_str());
  }
  printf("Usage: %s [params] ", this->app_name.c_str());
  size_t i;
  for (i = 0; i < this->data_size_; i++) {
    if (this->data[i].number > -1) {
      flags_string name = this->data[i].keys[0].substr(1, this->data[i].keys[0].length() - 1);
      printf("%s ", name.c_str());
    }
  }
  printf("\n\n");
  for (i = 0; i < this->data_size_; i++) {
    if (this->data[i].number == -1) {
      printf("\t");
      for (size_t j = 0; j < this->data[i].keys.size(); j++) {
        flags_string k = this->data[i].keys[j];
        if (k.length() > 1) {
          printf("--");
        } else {
          printf("-");
        }
        printf("%s", k.c_str());
        if (j != this->data[i].keys.size() - 1) {
          printf(", ");
        }
      }
      flags_string dv = cat_string(this->data[i].def_value);
      if (dv.compare("") != 0) {
        printf(" (value:%s)", dv.c_str());
      }
      printf("\n\t\t%s\n", this->data[i].help_message.c_str());
    }
  }
  printf("\n");
  for (i = 0; i < this->data_size_; i++) {
    if (this->data[i].number != -1) {
      printf("\t");
      flags_string k = this->data[i].keys[0];
      k = k.substr(1, k.length() - 1);
      printf("%s", k.c_str());
      flags_string dv = cat_string(this->data[i].def_value);
      if (dv.compare("") != 0) {
        printf(" (value:%s)", dv.c_str());
      }
      printf("\n\t\t%s\n", this->data[i].help_message.c_str());
    }
  }
}

#endif // _COMMAND_LINE_PARSER_HPP_

enum _FLAGS_Type {
  _FLAGS_INT = 0,
  _FLAGS_BOOLEAN = 1,
  _FLAGS_REAL = 2,
  _FLAGS_STRING = 3,
  _FLAGS_MAT = 4,
  _FLAGS_MAT_VECTOR = 5,
  _FLAGS_ALGORITHM = 6,
  _FLAGS_FLOAT = 7,
  _FLAGS_UNSIGNED_INT = 8,
  _FLAGS_UINT64 = 9,
  _FLAGS_UCHAR = 11,
  _FLAGS_SCALAR = 12
};

struct _FlagDefine {
  struct _FlagDefine* next;
  _FLAGS_Type type;
  const char* name;
  void* value;
  const char* msg;
};

static struct _FlagDefine** _allflag() {
  static struct _FlagDefine* __allflag = NULL;
  return &__allflag;
}

static int _flag_define(_FlagDefine* newflag, _FLAGS_Type type, const char* name, void* value, const char* msg)
{
  struct _FlagDefine** pflags = _allflag();
  while (*pflags) {
    pflags = &((*pflags)->next);
  }
  *pflags = newflag;
  newflag->type = type;
  newflag->name = name;
  newflag->value = value;
  newflag->msg = msg;
  newflag->next = NULL;
  return 0;
}


#define DEFINE_string(name, val, txt)  static _FlagDefine _FLAGS_##name; static const char*  FLAGS_##name = val; static int IDFLAGS_##name = _flag_define(&_FLAGS_##name, _FLAGS_STRING,  #name, &FLAGS_##name, txt)
#define DEFINE_int32(name, val, txt)   static _FlagDefine _FLAGS_##name; static int32 FLAGS_##name = val; static int IDFLAGS_##name = _flag_define(&_FLAGS_##name, _FLAGS_INT,     #name, &FLAGS_##name, txt)
#define DEFINE_bool(name, val, txt)    static _FlagDefine _FLAGS_##name; static bool    FLAGS_##name = val; static int IDFLAGS_##name = _flag_define(&_FLAGS_##name, _FLAGS_BOOLEAN, #name, &FLAGS_##name, txt)


static int from_str(const char* str, _FLAGS_Type type, void* dst)
{
  char* endptr = (char*)str;
  switch (type) {
  case _FLAGS_INT:
    *(int*)dst = (int)strtol(str, &endptr, 10);
    break;
  case _FLAGS_BOOLEAN:
    if (0 == stricmp(str, "true")) {
      *(bool*)dst = true;
    } else if (0 == stricmp(str, "false")) {
      *(bool*)dst = false;
    } else {
      *(bool*)dst = strtoul(str, &endptr, 10) != 0;
    }
    break;
  case _FLAGS_UNSIGNED_INT:
    *(unsigned*)dst = (unsigned)strtoul(str, &endptr, 10);;
    break;
  case _FLAGS_UINT64:
    *(uint64*)dst = (uint64)_atoi64(str);
    break;
  case _FLAGS_FLOAT:
    *(float*)dst = (float)strtod(str, &endptr);
    break;
  case _FLAGS_REAL:
    *(double*)dst = (double)strtod(str, &endptr);
    break;
  case _FLAGS_STRING:
    *(char**)dst =  (char*)str;
    break;
#if 0
  case _FLAGS_SCALAR: {
    CScalar & scalar = *(CScalar*)dst;
    for (int i = 0; i < 4 && !ss.eof(); ++i) {
      ss >> scalar[i];
    }
  }
  break;
#endif
  default:
    CC_Error(CC_StsBadArg, "unknown/unsupported parameter type");
    return 0;
  }
  return 1;
}

static int apply_params(const char* key, int len, const char* value)
{
  struct _FlagDefine* flags = *_allflag();
  for (; flags; flags = flags->next) {
    if (0 == strncmp(flags->name, key, len)) {
      return from_str(value, flags->type, flags->value);
    }
  }
  return 0;
}

// parse argv
static int ParseCommandLineFlags(int argc, const char* const argv[], bool flag)
{
  int i;
  for (i = 1; i < argc;) {
    const char* s = argv[i++];
    int l = strlen(s);
    bool hasSingleDash = l > 1 && s[0] == '-';
    if (hasSingleDash) {
      bool hasDoubleDash = l > 2 && s[1] == '-';
      char* key = (char*)s + (hasDoubleDash ? 2 : 1);
      char* value = NULL;
      int keylen = strlen(key);
      char* equalsPos = strchr(key, '=');
      if (equalsPos) {
        value = (char*)equalsPos + 1;
        //key = key.substr(0, equalsPos);
        keylen = equalsPos - key;
      } else if (i < argc && '-' != argv[i][0]) {
        value = (char*)argv[i++];
      }
      if (value && key) {
        int valuelen = strlen(value);
        strim(&key, &keylen, " ");
        strim(&value, &valuelen, " ");
        apply_params(key, keylen, value);
      }
    }
  }
  return 0;
}

static void SetUsageMessage(const char* msg)
{
  printf(msg);
}
static void SetVersionString(const char* msg)
{
  printf(msg);
}
static void ShowUsageWithFlagsRestrict(const char* msg, const char* ms)
{
}

#endif // _STDC_FLAGS_C_H_
