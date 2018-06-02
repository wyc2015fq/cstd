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
// The following defines are not meant to be changeable.
// They are for readability only.
#include <stdlib.h>
#define MAXCHR 256
#define CHRBIT 8
#define BITBLK MAXCHR/CHRBIT
#define MAXTAG 10
#define MAXNFA 2048
#define NOTFOUND -1
#define OKP 1
#define NOP 0
#define CHR 1
#define ANY 2
#define CCL 3
#define BOL 4
#define EOL 5
#define BOT 6
#define EOT 7
#define BOW 8
#define EOW 9
#define REF 10
#define CLO 11
#define END 0
#define BLKIND 0170
#define BITIND 07
#define ASCIIB 0177
typedef int BOOL;
#define TRUE 1
#define FALSE 1
typedef struct regex_t {
  int bol;
  int sta;
  int failure;
  int bopat[MAXTAG];
  int eopat[MAXTAG];
  int tagstk[MAXTAG]; /* subpat tag stack..*/
  char nfa[MAXNFA]; /* automaton.. */
  char bittab[BITBLK]; /* bit table for CCL pre-set bits... */
} regex_t;
// The following defines are not meant to be changeable.
// They are for readability only.
const char bitarr[] = {1, 2, 4, 8, 16, 32, 64, '\200'};
#define badpat(x) (*s->nfa = END, x)
int regex_clear(regex_t* s)
{
  int i;
  for (i = 0; i < MAXTAG; i++) {
    //free(s->pat[i]);
    //s->pat[i] = 0;
    s->bopat[i] = NOTFOUND;
    s->eopat[i] = NOTFOUND;
  }
  return 0;
}
BOOL regex_grabmatches(regex_t* s, const char* ci, char** pat)
{
  BOOL success = TRUE;
  unsigned int i, j;
  for (i = 0; i < MAXTAG; i++) {
    if ((s->bopat[i] != NOTFOUND) && (s->eopat[i] != NOTFOUND)) {
      unsigned int len = s->eopat[i] - s->bopat[i];
      pat[i] = (char*)malloc((len + 1) * sizeof(char));
      if (pat[i]) {
        for (j = 0; j < len; j++) {
          pat[i][j] = ci[s->bopat[i] + j];
        }
        pat[i][len] = '\0';
      }
      else {
        success = FALSE;
      }
    }
  }
  return success;
}
int regex_chset(regex_t* s, char c)
{
  s->bittab[((c) & BLKIND) >> 3] |= bitarr[(c) & BITIND];
  return 0;
}
int regex_chsetwithcase(regex_t* s, char c, BOOL caseSensitive)
{
  if (caseSensitive) {
    regex_chset(s, c);
  }
  else {
    if ((c >= 'a') && (c <= 'z')) {
      regex_chset(s, c);
      regex_chset(s, (char)(c - 'a' + 'A'));
    }
    else if ((c >= 'A') && (c <= 'Z')) {
      regex_chset(s, c);
      regex_chset(s, (char)(c - 'A' + 'a'));
    }
    else {
      regex_chset(s, c);
    }
  }
  return 0;
}
const char escapeValue(char ch)
{
  switch (ch) {
  case 'a':
    return '\a';
  case 'b':
    return '\b';
  case 'f':
    return '\f';
  case 'n':
    return '\n';
  case 'r':
    return '\r';
  case 't':
    return '\t';
  case 'v':
    return '\v';
  }
  return 0;
}
const char* re_comp(regex_t* s, const char* pat, int length, BOOL caseSensitive, BOOL posix)
{
  char* mp = s->nfa; /* s->nfa pointer */
  char* lp; /* saved pointer.. */
  char* sp = s->nfa; /* another one.. */
  char* mpMax = mp + MAXNFA - BITBLK - 10;
  int tagi = 0; /* tag stack index */
  int tagc = 1; /* actual tag count */
  int i, n;
  int c1, c2;
  char mask; /* xor mask -CCL/NCL */
  const char* p;
  if (!pat || !length) {
    if (s->sta) {
      return 0;
    }
    else {
      return badpat("No previous regular expression");
    }
  }
  s->sta = NOP;
  p = pat; /* pattern pointer */
  for (i = 0; i < length; i++, p++) {
    if (mp > mpMax) {
      return badpat("Pattern too long");
    }
    lp = mp;
    switch (*p) {
    case '.': /* match any char.. */
      *mp++ = ANY;
      break;
    case '^': /* match beginning.. */
      if (p == pat) {
        *mp++ = BOL;
      }
      else {
        *mp++ = CHR;
        *mp++ = *p;
      }
      break;
    case '$': /* match endofline.. */
      if (!*(p + 1)) {
        *mp++ = EOL;
      }
      else {
        *mp++ = CHR;
        *mp++ = *p;
      }
      break;
    case '[': /* match char class..*/
      *mp++ = CCL;
      i++;
      if (*++p == '^') {
        mask = '\377';
        i++;
        p++;
      }
      else {
        mask = 0;
      }
      if (*p == '-') { /* real dash */
        i++;
        regex_chset(s, *p++);
      }
      if (*p == ']') { /* real brace */
        i++;
        regex_chset(s, *p++);
      }
      while (*p && *p != ']') {
        if (*p == '-' && *(p + 1) && *(p + 1) != ']') {
          i++;
          p++;
          c1 = *(p - 2) + 1;
          i++;
          c2 = *p++;
          while (c1 <= c2) {
            regex_chsetwithcase(s, (char)(c1++), caseSensitive);
          }
        }
        else if (*p == '\\' && *(p + 1)) {
          char escape;
          i++;
          p++;
          escape = escapeValue(*p);
          if (escape) {
            regex_chsetwithcase(s, escape, caseSensitive);
          }
          else {
            regex_chsetwithcase(s, *p, caseSensitive);
          }
          i++;
          p++;
        }
        else {
          i++;
          regex_chsetwithcase(s, *p++, caseSensitive);
        }
      }
      if (!*p) {
        return badpat("Missing ]");
      }
      for (n = 0; n < BITBLK; s->bittab[n++] = (char) 0) {
        *mp++ = (char)(mask ^ s->bittab[n]);
      }
      break;
    case '*': /* match 0 or more.. */
    case '+': /* match 1 or more.. */
      if (p == pat) {
        return badpat("Empty closure");
      }
      lp = sp; /* previous opcode */
      if (*lp == CLO) { /* equivalence.. */
        break;
      }
      switch (*lp) {
      case BOL:
      case BOT:
      case EOT:
      case BOW:
      case EOW:
      case REF:
        return badpat("Illegal closure");
      default:
        break;
      }
      if (*p == '+')
        for (sp = mp; lp < sp; lp++) {
          *mp++ = *lp;
        }
      *mp++ = END;
      *mp++ = END;
      sp = mp;
      while (--mp > lp) {
        *mp = mp[-1];
      }
      *mp = CLO;
      mp = sp;
      break;
    case '\\': /* tags, backrefs .. */
      i++;
      switch (*++p) {
      case '<':
        *mp++ = BOW;
        break;
      case '>':
        if (*sp == BOW) {
          return badpat("Null pattern inside \\<\\>");
        }
        *mp++ = EOW;
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        n = *p - '0';
        if (tagi > 0 && s->tagstk[tagi] == n) {
          return badpat("Cyclical reference");
        }
        if (tagc > n) {
          *mp++ = (char)(REF);
          *mp++ = (char)(n);
        }
        else {
          return badpat("Undetermined reference");
        }
        break;
      case 'a':
      case 'b':
      case 'n':
      case 'f':
      case 'r':
      case 't':
      case 'v':
        *mp++ = CHR;
        *mp++ = escapeValue(*p);
        break;
      default:
        if (!posix && *p == '(') {
          if (tagc < MAXTAG) {
            s->tagstk[++tagi] = tagc;
            *mp++ = BOT;
            *mp++ = (char)(tagc++);
          }
          else {
            return badpat("Too many \\(\\) pairs");
          }
        }
        else if (!posix && *p == ')') {
          if (*sp == BOT) {
            return badpat("Null pattern inside \\(\\)");
          }
          if (tagi > 0) {
            *mp++ = (char)(EOT);
            *mp++ = (char)(s->tagstk[tagi--]);
          }
          else {
            return badpat("Unmatched \\)");
          }
        }
        else {
          *mp++ = CHR;
          *mp++ = *p;
        }
      }
      break;
    default : /* an ordinary char */
      if (posix && *p == '(') {
        if (tagc < MAXTAG) {
          s->tagstk[++tagi] = tagc;
          *mp++ = BOT;
          *mp++ = (char)(tagc++);
        }
        else {
          return badpat("Too many () pairs");
        }
      }
      else if (posix && *p == ')') {
        if (*sp == BOT) {
          return badpat("Null pattern inside ()");
        }
        if (tagi > 0) {
          *mp++ = (char)(EOT);
          *mp++ = (char)(s->tagstk[tagi--]);
        }
        else {
          return badpat("Unmatched )");
        }
      }
      else if (caseSensitive) {
        *mp++ = CHR;
        *mp++ = *p;
      }
      else {
        *mp++ = CCL;
        mask = 0;
        regex_chsetwithcase(s, *p, FALSE);
        for (n = 0; n < BITBLK; s->bittab[n++] = (char) 0) {
          *mp++ = (char)(mask ^ s->bittab[n]);
        }
      }
      break;
    }
    sp = lp;
  }
  if (tagi > 0) {
    return badpat((posix ? "Unmatched (" : "Unmatched \\("));
  }
  *mp = END;
  s->sta = OKP;
  return 0;
}
//
// regex_pmatch: internal routine for the hard part
//
// This code is partly snarfed from an early grep written by
// David Conroy. The backref and tag stuff, and various other
// innovations are by oz.
//
// special case optimizations: (s->nfa[n], s->nfa[n+1])
// CLO ANY
// We KNOW .* will match everything upto the
// end of line. Thus, directly go to the end of
// line, without recursive regex_pmatch calls. As in
// the other closure cases, the remaining pattern
// must be matched by moving backwards on the
// string recursively, to find a match for xy
// (x is ".*" and y is the remaining pattern)
// where the match satisfies the LONGEST match for
// x followed by a match for y.
// CLO CHR
// We can again scan the string forward for the
// single char and at the point of s->failure, we
// execute the remaining s->nfa recursively, same as
// above.
//
// At the end of a successful match, s->bopat[n] and s->eopat[n]
// are set to the beginning and end of subpatterns matched
// by tagged expressions (n = 1 to 9).
//
extern int re_fail(char*, char);
// character classification table for word boundary operators BOW
// and EOW. the reason for not using ctype macros is that we can
// let the user add into our own table. see ModifyWord. This table
// is not in the bitset form, since we may wish to extend it in the
// future for other character classifications.
//
// TRUE for 0-9 A-Z a-z _
//
static char chrtyp[MAXCHR] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 0, 0, 0, 0, 1, 0, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 0, 0, 0, 0, 0
};
#define inascii(x) (0177&(x))
#define iswordc(x) chrtyp[inascii(x)]
#define isinset(x,y) ((x)[((y)&BLKIND)>>3] & bitarr[(y)&BITIND])
// skip values for CLO XXX to skip past the closure
//
#define ANYSKIP 2 /* [CLO] ANY END ... */
#define CHRSKIP 3 /* [CLO] CHR chr END ... */
#define CCLSKIP 34 /* [CLO] CCL 32bytes END ... */
int regex_pmatch(regex_t* s, const char* ci, int lp, int endp, char* ap)
{
  int op, c, n;
  int e; /* extra pointer for CLO */
  int bp; /* beginning of subpat.. */
  int ep; /* ending of subpat.. */
  int are; /* to save the line ptr. */
  while ((op = *ap++) != END)
    switch (op) {
    case CHR:
      if (ci[lp++] != *ap++) {
        return NOTFOUND;
      }
      break;
    case ANY:
      if (lp++ >= endp) {
        return NOTFOUND;
      }
      break;
    case CCL:
      c = ci[lp++];
      if (!isinset(ap, c)) {
        return NOTFOUND;
      }
      ap += BITBLK;
      break;
    case BOL:
      if (lp != s->bol) {
        return NOTFOUND;
      }
      break;
    case EOL:
      if (lp < endp) {
        return NOTFOUND;
      }
      break;
    case BOT:
      s->bopat[*ap++] = lp;
      break;
    case EOT:
      s->eopat[*ap++] = lp;
      break;
    case BOW:
      if (lp != s->bol && iswordc(ci[lp - 1]) || !iswordc(ci[lp])) {
        return NOTFOUND;
      }
      break;
    case EOW:
      if (lp == s->bol || !iswordc(ci[lp - 1]) || iswordc(ci[lp])) {
        return NOTFOUND;
      }
      break;
    case REF:
      n = *ap++;
      bp = s->bopat[n];
      ep = s->eopat[n];
      while (bp < ep)
        if (ci[bp++] != ci[lp++]) {
          return NOTFOUND;
        }
      break;
    case CLO:
      are = lp;
      switch (*ap) {
      case ANY:
        while (lp < endp) {
          lp++;
        }
        n = ANYSKIP;
        break;
      case CHR:
        c = *(ap + 1);
        while ((lp < endp) && (c == ci[lp])) {
          lp++;
        }
        n = CHRSKIP;
        break;
      case CCL:
        while ((lp < endp) && isinset(ap + 1, ci[lp])) {
          lp++;
        }
        n = CCLSKIP;
        break;
      default:
        s->failure = TRUE;
        //re_fail("closure: bad nfa.", *ap);
        return NOTFOUND;
      }
      ap += n;
      while (lp >= are) {
        if ((e = regex_pmatch(s, ci, lp, endp, ap)) != NOTFOUND) {
          return e;
        }
        --lp;
      }
      return NOTFOUND;
    default:
      //re_fail("re_exec: bad nfa.", (char)(op));
      return NOTFOUND;
    }
  return lp;
}
// ModifyWord:
// add new characters into the word table to change re_exec's
// understanding of what a word should look like. Note that we
// only accept additions into the word definition.
//
// If the string parameter is 0 or null string, the table is
// reset back to the default containing A-Z a-z 0-9 _. [We use
// the compact bitset representation for the default table]
//
static char deftab[16] = { 0, 0, 0, 0, 0, 0, '\377', 003, '\376', '\377', '\377', '\207', '\376', '\377', '\377', 007 };
int ModifyWord(char* s)
{
  int i;
  if (!s || !*s) {
    for (i = 0; i < MAXCHR; i++) {
      if (!isinset(deftab, i)) {
        iswordc(i) = 0;
      }
    }
  }
  else {
    while (*s) {
      iswordc(*s++) = 1;
    }
  }
  return 0;
}
// regex_substitute:
// substitute the matched portions of the src in dst.
//
// & substitute the entire matched pattern.
//
// \digit substitute a subpattern, with the given tag number.
// Tags are numbered from 1 to 9. If the particular
// tagged subpattern does not exist, null is substituted.
int regex_substitute(regex_t* s, const char* ci, char* src, char* dst)
{
  char c;
  int pin;
  int bp;
  int ep;
  if (!*src || !s->bopat[0]) {
    return 0;
  }
  while ((c = *src++) != 0) {
    switch (c) {
    case '&':
      pin = 0;
      break;
    case '\\':
      c = *src++;
      if (c >= '0' && c <= '9') {
        pin = c - '0';
        break;
      }
    default:
      *dst++ = c;
      continue;
    }
    if ((bp = s->bopat[pin]) != 0 && (ep = s->eopat[pin]) != 0) {
      while (ci[bp] && bp < ep) {
        *dst++ = ci[bp++];
      }
      if (bp < ep) {
        return 0;
      }
    }
  }
  *dst = (char) 0;
  return 1;
}
// re_exec:
// execute s->nfa to find a match.
//
// special cases: (s->nfa[0])
// BOL
// Match only once, starting from the
// beginning.
// CHR
// First locate the character without
// calling regex_pmatch, and if found, call
// regex_pmatch for the remaining string.
// END
// re_comp failed, poor luser did not
// check for it. Fail fast.
//
// If a match is found, s->bopat[0] and s->eopat[0] are set
// to the beginning and the end of the matched fragment,
// respectively.
int re_exec(regex_t* s, const char* ci, int lp, int endp)
{
  char c;
  int ep = NOTFOUND;
  char* ap = s->nfa;
  s->bol = lp;
  s->failure = 0;
  regex_clear(s);
  switch (*ap) {
  case BOL: /* anchored: match from BOL only */
    ep = regex_pmatch(s, ci, lp, endp, ap);
    break;
  case EOL: /* just searching for end of line normal path doesn't work */
    if (*(ap + 1) == END) {
      lp = endp;
      ep = lp;
      break;
    }
    else {
      return 0;
    }
  case CHR: /* ordinary char: locate it fast */
    c = *(ap + 1);
    while ((lp < endp) && (ci[lp] != c)) {
      lp++;
    }
    if (lp >= endp) { /* if EOS, fail, else fall thru. */
      return 0;
    }
  default: /* regular matching all the way. */
    while (lp < endp) {
      ep = regex_pmatch(s, ci, lp, endp, ap);
      if (ep != NOTFOUND) {
        break;
      }
      lp++;
    }
    break;
  case END: /* munged automaton. fail always */
    return 0;
  }
  if (ep == NOTFOUND) {
    return 0;
  }
  s->bopat[0] = lp;
  s->eopat[0] = ep;
  return 1;
}
#undef MAXCHR
#undef CHRBIT
#undef BITBLK
#undef MAXTAG
#undef MAXNFA
#undef NOTFOUND
#undef OKP
#undef NOP
#undef CHR
#undef ANY
#undef CCL
#undef BOL
#undef EOL
#undef BOT
#undef EOT
#undef BOW
#undef EOW
#undef REF
#undef CLO
#undef END
#undef BLKIND
#undef BITIND
#undef ASCIIB
#undef inascii
#undef iswordc
#undef isinset
#undef ANYSKIP
#undef CHRSKIP
#undef CCLSKIP

