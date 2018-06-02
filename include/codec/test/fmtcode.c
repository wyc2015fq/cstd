
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macro.h"
#include "fileio.h"
#include "debug.h"

typedef enum {
  t_SPACE,
    t_NEWLINE,
    t_BLOCK_BEGIN,
    t_BLOCK_END,
    t_BRACKET_BEGIN,
    t_BRACKET_END,
    t_BRACKET2_BEGIN,
    t_BRACKET2_END,
    t_IDENTIFIER,
    t_STRING,
    t_CHAR,
    t_SLASH,
    t_BACKSLASH,
    t_DEFINE,
    t_COLON,
    t_COMMENT,
    t_INCLUDE,
    t_SEMICOLON,
    t_COMMA, //逗号
    t_MARKER,
    t_UNKNOW,
    t_MAXTYPE,
};

#define STRING_COMMENT_BEGIN   "/*"
#define STRING_COMMENT_END     "*/"
#define STRING_COMMENT_LINE    "\n"
#define CHAR_NEWLINE           '\n'
#define CHAR_INCLUDE           '#'
#define CHAR_SEMICOLON         ';'
#define CHAR_COMMA             ','
#define CHAR_COLON             ':'
#define CHAR_BLOCK_BEGIN       '{'
#define CHAR_BLOCK_END         '}'
#define CHAR_BRACKET_BEGIN     '('
#define CHAR_BRACKET_END       ')'
#define CHAR_BRACKET2_BEGIN    '['
#define CHAR_BRACKET2_END      ']'
#define CHAR_SLASH             '\\'
#define CHAR_BACKSLASH         '/'
#define CHAR_CHAR_BEGIN        '\''
#define CHAR_CHAR_END          '\''
#define CHAR_STRING_BEGIN      '\"'
#define CHAR_STRING_END        '\"'
#define SPACESTR               " \t(){}[]\",;\n"
#define CHECKSPACE_T(_S, _OP, _DO) for (; ' ' _OP *(_S) || '\t' _OP *(_S) || '\r' _OP *(_S); _DO (_S))
#define CHECKSPACE_T2(_S, _OP, _DO) for (; '\n' _OP *(_S) || ' ' _OP *(_S) || '\t' _OP *(_S) || '\r' _OP *(_S); _DO (_S))

typedef struct fmtinfo {
  int space_num;
  int block_dep;
  int bracket_dep;
  int bracket2_dep;
  int s_line_begin;
  int max_char_per_line;
  char* line_begin;
} fmtinfo;

int indent(fmtinfo* fi, char* d, char** pd, char* s, int slen, int t) {
  char* d0=d;
  if (t_SEMICOLON==t) {
    int asdf=0;
  }
  if (fi->block_dep<0) {
    fi->block_dep=0;
  }
  if (fi->s_line_begin<0) {
    fi->s_line_begin=0;
  }
  if (0) { // 折行处理
    char* a=0;
    if ((d-fi->line_begin+slen)>fi->max_char_per_line
      && slen>1
      //&& a=strpbrk(fi->line_begin, "=")
      ) {
      *d++='\n';
      memset(d, ' ', fi->block_dep*(fi->space_num));
      fi->line_begin=d;
    }
  }
  if (fi->s_line_begin) {
    if (t_NEWLINE!=t && t_SEMICOLON!=t) {
      memset(d, '\n', fi->s_line_begin), d+=fi->s_line_begin;
      fi->s_line_begin=0;
    }
    if (
      fi->block_dep
      && t_BLOCK_BEGIN!=t
      && t_COLON!=t//不缩进标号
      && t_INCLUDE!=t//不缩进 #include
      && t_DEFINE!=t//不缩进 #define
      && t_NEWLINE!=t//不缩进回车
      //&& t_COMMENT!=t//不缩进注释
      ) {
      memset(d, ' ', fi->block_dep*(fi->space_num)), d+=fi->block_dep*(fi->space_num);
    }
  } else {
    if (t_SEMICOLON!=t && t_COLON!=t && t_COMMA!=t
      //&& t_BLOCK_BEGIN!=t
      && t_INCLUDE!=t//不缩进 #include
      && t_BLOCK_END!=t//
      && t_BRACKET_BEGIN!=t //&& t_BRACKET_END!=t// 括号左空格
      //&& 0==bracket_dep// 括号中空格
      && 0==(fi->bracket2_dep)// 方括号左空格
      && t_BRACKET2_BEGIN!=t && t_BRACKET2_END!=t// 方括号中空格
      ) {
      *d++ = ' ';
    }
  }
  if (
    t_NEWLINE==t
    ||t_BLOCK_BEGIN==t
    //||t_COMMENT==t
    ||t_COLON==t
    ) {
    fi->s_line_begin = 1;
  } else if (t_BLOCK_END==t) {
    fi->s_line_begin = 1+(fi->block_dep==0);
  } else if (t_SEMICOLON==t) {
    if (!(fi->bracket_dep)) { fi->s_line_begin=1; }
  } else if (t_DEFINE==t) {
    fi->s_line_begin = 1;
  }
  {
    if (CHAR_NEWLINE!=*s) {
      memcpy(d, s, slen);
      d+=slen;
    }
    *d=0;
  }
  if (pd) *pd=d;
  return 0;
}

int strsplit(fmtinfo* fi, char* s, char** beg, char** end) {
  int t = t_IDENTIFIER;
  CHECKSPACE_T(s, ==, ++);
  *beg = s;
  if (CHAR_BLOCK_BEGIN==*s) {
    ++s, t = t_BLOCK_BEGIN, ++(fi->block_dep);
  } else if (CHAR_BLOCK_END==*s) {
    ++s, t = t_BLOCK_END, --(fi->block_dep);
  } else if (CHAR_BRACKET_BEGIN==*s) {
    ++s, t = t_BRACKET_BEGIN, ++(fi->bracket_dep), ++(fi->block_dep);
  } else if (CHAR_BRACKET_END==*s) {
    ++s, t = t_BRACKET_END, --(fi->bracket_dep), --(fi->block_dep);
  } else if (CHAR_BRACKET2_BEGIN==*s) {
    ++s, t = t_BRACKET2_BEGIN, ++(fi->bracket2_dep), ++(fi->block_dep);
  } else if (CHAR_BRACKET2_END==*s) {
    ++s, t = t_BRACKET2_END, --(fi->bracket2_dep), --(fi->block_dep);
  } else if (CHAR_SEMICOLON==*s) {
    ++s, t = t_SEMICOLON;
  } else if (CHAR_COMMA==*s) {
    ++s, t = t_COMMA;
  } else if (CHAR_NEWLINE==*s) {
    ++s, t = t_NEWLINE;
    CHECKSPACE_T2(s, ==, ++);
  } else if (CHAR_INCLUDE==*s) {
    t = t_INCLUDE;
    if (s[1]=='d') {
      t = t_DEFINE;
    }
    for (; (s = strchr(++s, '\n')) && (s[-2]==CHAR_SLASH||s[-1]==CHAR_SLASH);) {
      int a=0;
    }
    s = (NULL==s) ? (*beg + strlen(*beg)) : (s);
  } else if (CHAR_BACKSLASH==*s && (CHAR_BACKSLASH==s[1] || '*'==s[1])) {
    t = t_COMMENT;
    if (CHAR_BACKSLASH==s[1]) {
      s = strchr(s, '\n');
      s = (NULL==s) ? (*beg + strlen(*beg)) : (s);
    } else {
      s = strstr(s, STRING_COMMENT_END);
      s = (NULL==s) ? (*beg + strlen(*beg)) : (s+2);
    }
  } else if (CHAR_STRING_BEGIN==*s) {
    t = t_STRING;
    for (; (s = strchr(++s, CHAR_STRING_END)) && s[-1]==CHAR_SLASH;);
    s = (NULL==s) ? (*beg + strlen(*beg)) : (s+1);
  } else if (CHAR_CHAR_BEGIN==*s) {
    t = t_CHAR;
    for (; (s = strchr(++s, CHAR_CHAR_END)) && s[-1]==CHAR_SLASH;);
    s = (NULL==s) ? (*beg + strlen(*beg)) : (s+1);
  } else if (isalnum(*s)||'_'==*s||CHAR_SLASH==*s||
    '+'==*s||'-'==*s||'*'==*s||'/'==*s||'%'==*s||'^'==*s||
    '!'==*s||'?'==*s||'.'==*s||':'==*s||'#'==*s||'>'==*s||
    '<'==*s||'='==*s||'&'==*s||'|'==*s||','==*s||';'==*s) {
    t = t_IDENTIFIER;
    s = strpbrk(s, SPACESTR);
    s = (NULL==s) ? (*beg + strlen(*beg)) : (s);
    if (CHAR_NEWLINE==*s) {
      int asdf=0;
    }
    {
      char* s1=s;
      CHECKSPACE_T(s1, ==, ++);
      if (':'==*s1) {
        s=s1+1;
        t = t_COLON;
      }
    }
  } else {
    t = t_UNKNOW;
  }
  if (t==t_UNKNOW || NULL==s) {
    int a=0;
  }
  *end=s;
  return t;
}

// space:=' ','\t','\n'
// block_begin:='{'
// block_end:='}'
// word_begin:='('
// word_end:=')'
// indent(space)=NUL
// indent(braak)=NUL
// indent(block_begin)=++dep,s_block==1 ? '\n'+' '*dep : ' ', out(block_begin)
int fmtcode(char* s, char* send, char* d, char** pd) {
  char* beg, *d0=d;
  int t;
  fmtinfo fi;
  {
    fi.space_num=2;
    fi.block_dep=0;
    fi.bracket_dep=0;
    fi.bracket2_dep=0;
    fi.s_line_begin=1;
    fi.max_char_per_line=50;
    fi.line_begin=d;
  }
  for (; s < send; ) {
    int n;
    t = strsplit(&fi, s, &beg, &s);
    if (t==t_UNKNOW || NULL==s) {
      break;
    }
    n = MIN(s-beg, 1000);
    if (0==strcmp(beg, "#define")) {
      int aaf=0;
    }
    if (s < send)
    indent(&fi, d, &d, beg, s-beg, t);
    //printf(buf2);
    //printf("%s\n", buf);
  }
  *d++='\n';
  if (pd) *pd=d;
  return 1;
}
