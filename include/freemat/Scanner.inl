
static int TokenID = 1;

TokenValueType ReservedFind(const char* str, int len)
{
  static const char* sss = "|"
#define KEYWORDDEF(a, b)   #a "|"
#include "keyworddef.txt"
#undef KEYWORDDEF
      ;
  static TokenValueType toks[] = {
#define KEYWORDDEF(a, b)   b,
#include "keyworddef.txt"
#undef KEYWORDDEF
  };
  int k = STRsplitfind(sss, -1, str, len, 0, 0);
  return (k >= 0) ? toks[k] : TOK_INVALID;
}

static BOOL isalnumus(int a)
{
  return (isalnum(a) || (a == '_'));
}

static BOOL isablank(int a)
{
  return (a == ' ' || a == '\t' || a == '\r');
}

unsigned scanner_contextNum(scanner_t* s)
{
  return (s->m_ptr << 16 | s->m_linenumber);
}

void scanner_setToken(scanner_t* s, TokenValueType tok, const char* text, int len)
{
  token_set(s->m_tok, tok, s->m_ptr << 16 | s->m_linenumber, text, len);
  MUTEXLOCK(&lock);
  s->m_tok->m_UID = TokenID++;
  MUTEXUNLOCK(&lock);
}

BOOL scanner_done(scanner_t* s)
{
  return (s->m_ptr >= (int)(s->m_strlen));
}

int scanner_free(scanner_t* s)
{
  token_free(s->m_tok);
  return 0;
}

int scanner_copy(scanner_t* s, const scanner_t* s2)
{
  *s = *s2;
  return 0;
}

#define push(aaa)  s->m_ignorews[s->m_ignorewslen++] = aaa
#define pop()   if (s->m_ignorewslen>0) { s->m_ignorewslen--; }
int scanner_set(scanner_t* s, const char* buf, int buflen, const char* fname, BOOL octaveCompatibility)
{
  buflen = buflen < 0 ? strlen(buf) : buflen;
  s->octCompat = octaveCompatibility;
  s->m_text = buf;
  s->m_strlen = buflen;
  strcpy(s->m_filename, fname);
  s->m_ptr = 0;
  s->m_linenumber = 1;
  s->m_tokValid = FALSE;
  s->m_inContinuationState = FALSE;
  s->m_bracketDepth = 0;
  s->m_strlen = buflen;
  push(TRUE);
  s->m_debugFlag = FALSE;
  s->m_blobFlag = FALSE;
  return 0;
}

BOOL scanner_inContinuationState(scanner_t* s)
{
  return s->m_inContinuationState;
}

BOOL scanner_inBracket(scanner_t* s)
{
  return (s->m_bracketDepth > 0);
}

void scanner_consume(scanner_t* s)
{
  s->m_tokValid = FALSE;
}

int scanner_current(scanner_t* s)
{
  if (s->m_ptr < s->m_strlen) {
    return s->m_text[s->m_ptr];
  }
  else {
    return 0;
  }
}

int scanner_previous(scanner_t* s)
{
  if (s->m_ptr) {
    return s->m_text[s->m_ptr - 1];
  }
  else {
    return 0;
  }
}

void scanner_pushWSFlag(scanner_t* s, BOOL ignoreWS)
{
  push(ignoreWS);
}

void scanner_popWSFlag(scanner_t* s)
{
  pop();
}

int scanner_ahead(scanner_t* s, int n)
{
  if ((s->m_ptr + n) >= (int)(s->m_strlen)) {
    return 0;
  }
  else {
    return s->m_text[s->m_ptr + n];
  }
}

BOOL scanner_peek(scanner_t* s, int chars, int tok)
{
  return (scanner_ahead(s, chars) == tok);
}
void scanner_fetchContinuation(scanner_t* s)
{
  s->m_ptr += 3;

  while ((scanner_current(s) != '\n') && (s->m_ptr < s->m_strlen)) {
    s->m_ptr++;
  }

  if (scanner_current(s) == '\n') {
    s->m_linenumber++;
    s->m_ptr++;
  }

  s->m_inContinuationState = TRUE;
}

void scanner_fetchComment(scanner_t* s)
{
  while ((scanner_current(s) != '\n') && (s->m_ptr < s->m_strlen)) {
    s->m_ptr++;
  }
}

int scanner_fetchString(scanner_t* s)
{
  int len = 0;
  str_t ret[1] = {0};

  // We want to advance, but skip double quotes
  //  while ((next() != ') || ((next() == ') && (next(2) == ')) && (next() != '\n')
  while (((scanner_ahead(s, len + 1) != '\'') ||
      ((scanner_ahead(s, len + 1) == '\'') && (scanner_ahead(s, len + 2) == '\''))) &&
      (scanner_ahead(s, len + 1) != '\n')) {
    if ((scanner_ahead(s, len + 1) == '\'') &&
        (scanner_ahead(s, len + 2) == '\'')) {
      len += 2;
    }
    else {
      len++;
    }
  }

  if (scanner_ahead(s, len + 1) == '\n') {
    throw_Exception("unterminated string");
  }

  str_setstr(ret, s->m_text + s->m_ptr + 1, len);
  str_replace_str(ret, STR1("''"), STR1("'"));
  scanner_setToken(s, TOK_STRING, ret->s, ret->l);
  s->m_ptr += len + 2;
  str_free(ret);
  return 0;
}

void scanner_fetchWhitespace(scanner_t* s)
{
  int len = 0;

  while (isablank(scanner_ahead(s, len))) {
    len++;
  }

  scanner_setToken(s, TOK_SPACE, 0, 0);
  s->m_ptr += len;
}

// A Blob is either:
//   1.  A regular string (with quote delimiters)
//   2.  A sequence of characters with either a whitespace
//       a comma or a colon.
void scanner_fetchBlob(scanner_t* s)
{
  if (scanner_current(s) == '\'') {
    scanner_fetchString(s);
    s->m_tokValid = TRUE;
  }
  else {
    int len = 0;

    while ((scanner_ahead(s, len) != '\n') && (!isablank(scanner_ahead(s, len))) &&
        (scanner_ahead(s, len) != '%') && (scanner_ahead(s, len) != ',') &&
        (scanner_ahead(s, len) != ';')) {
      len++;
    }

    if (len > 0) {
      scanner_setToken(s, TOK_STRING, s->m_text + s->m_ptr, len);
      s->m_ptr += len;
      s->m_tokValid = TRUE;
    }
  }
}

void scanner_fetchIdentifier(scanner_t* s)
{
  int len = 0;
  TokenValueType k;

  while (isalnumus(scanner_ahead(s, len))) {
    len++;
  }

  // Collect the identifier into a string
  k = ReservedFind(s->m_text + s->m_ptr, len);

  if (k != TOK_INVALID) {
    scanner_setToken(s, k, 0, 0);
  }
  else {
    scanner_setToken(s, TOK_IDENT, s->m_text + s->m_ptr, len);
  }

  s->m_ptr += len;
}

//A number consists of something like:
//{integer}.{integer}E{sign}{integer}
//   s1   s2   s3   s4  s5    s6
// .{integer}E{sign}{integer}
//
// <Float><Exponent>
// <Float>
// <Integer>
//
// flags - int, float, double, complex
//

void scanner_fetchNumber(scanner_t* s)
{
  const char* numtext;
  int numtextlen, len = 0, lookahead = 0;
  BOOL imagnumber = FALSE;
  BOOL singleprecision = FALSE;

  while (isdigit(scanner_ahead(s, len))) {
    len++;
  }

  lookahead = len;

  if (scanner_ahead(s, lookahead) == '.') {
    lookahead++;
    len = 0;

    while (isdigit(scanner_ahead(s, len + lookahead))) {
      len++;
    }

    lookahead += len;
  }

  if ((scanner_ahead(s, lookahead) == 'E') ||
      (scanner_ahead(s, lookahead) == 'e')) {
    lookahead++;

    if ((scanner_ahead(s, lookahead) == '+') ||
        (scanner_ahead(s, lookahead) == '-')) {
      lookahead++;
    }

    len = 0;

    while (isdigit(scanner_ahead(s, len + lookahead))) {
      len++;
    }

    lookahead += len;
  }

  if ((scanner_ahead(s, lookahead) == 'f') || (scanner_ahead(s, lookahead) == 'F')) {
    singleprecision = TRUE;
    lookahead++;
  }

  if ((scanner_ahead(s, lookahead) == 'd') || (scanner_ahead(s, lookahead) == 'D')) {
    lookahead++;
  }

  // Recognize the complex constants, but strip the "i" off
  if ((scanner_ahead(s, lookahead) == 'i') ||
      (scanner_ahead(s, lookahead) == 'I') ||
      (scanner_ahead(s, lookahead) == 'j') ||
      (scanner_ahead(s, lookahead) == 'J')) {
    imagnumber = TRUE;
  }

  // Back off if we aggregated a "." from "..." into the number
  if (((scanner_ahead(s, lookahead - 1) == '.') &&
      (scanner_ahead(s, lookahead) == '.') &&
      (scanner_ahead(s, lookahead + 1) == '.') &&
      (scanner_ahead(s, lookahead + 2) != '.')) ||
      ((scanner_ahead(s, lookahead - 1) == '.') &&
          ((scanner_ahead(s, lookahead) == '*') ||
              (scanner_ahead(s, lookahead) == '/') ||
              (scanner_ahead(s, lookahead) == '\\') ||
              (scanner_ahead(s, lookahead) == '^') ||
              (scanner_ahead(s, lookahead) == '\'')))) {
    lookahead--;
  }

  numtext = s->m_text + s->m_ptr;
  numtextlen = lookahead;
  s->m_ptr += lookahead;

  if (imagnumber) {
    s->m_ptr++;
  }

  if (!imagnumber) {
    scanner_setToken(s, singleprecision ? TOK_REALF : TOK_REAL, numtext, numtextlen);
  }
  else {
    scanner_setToken(s, singleprecision ? TOK_IMAGF : TOK_IMAG, numtext, numtextlen);
  }
}

BOOL scanner_tryFetchBinary(scanner_t* s, const char* op, TokenValueType tok)
{
  if ((scanner_current(s) == op[0]) && (scanner_ahead(s, 1) == op[1])) {
    scanner_setToken(s, tok, 0, 0);
    s->m_ptr += 2;
    return TRUE;
  }

  return FALSE;
}

BOOL scanner_tryFetchBinary1(scanner_t* s, char op, TokenValueType tok)
{
  if (scanner_current(s) == op) {
    scanner_setToken(s, tok, 0, 0);
    s->m_ptr += 1;
    return TRUE;
  }

  return FALSE;
}

void scanner_fetchOther(scanner_t* s)
{
  if (scanner_current(s) == '.') {
    if (scanner_tryFetchBinary(s, ".*", TOK_DOTTIMES)) {
      return;
    }

    if (scanner_tryFetchBinary(s, "./", TOK_DOTRDIV)) {
      return;
    }

    if (scanner_tryFetchBinary(s, ".\\", TOK_DOTLDIV)) {
      return;
    }

    if (scanner_tryFetchBinary(s, ".^", TOK_DOTPOWER)) {
      return;
    }

    if (scanner_tryFetchBinary(s, ".'", TOK_DOTTRANSPOSE)) {
      return;
    }
  }

  if (scanner_tryFetchBinary(s, "++", TOK_INCR)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "--", TOK_DECR)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "+=", TOK_PLUS_EQ)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "-=", TOK_MINUS_EQ)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "<=", TOK_LEQ)) {
    return;
  }

  if (scanner_tryFetchBinary(s, ">=", TOK_GEQ)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "==", TOK_EQU)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "!=", TOK_NEQ)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "~=", TOK_NEQ)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "&&", TOK_SAND)) {
    return;
  }

  if (scanner_tryFetchBinary(s, "||", TOK_SOR)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '<', TOK_LSS)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '>', TOK_GTR)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, ';', TOK_SEMICOLON)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, ',', TOK_COMMA)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '\n', TOK_LF)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '=', TOK_EQ)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '|', TOK_OR)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '&', TOK_AND)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, ':', TOK_COLON)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '+', TOK_PLUS)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '-', TOK_MINUS)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '*', TOK_MUL)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '/', TOK_RDIV)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '\\', TOK_LDIV)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '~', TOK_TILDE)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '^', TOK_CARET)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '(', TOK_LBRACKET0)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, ')', TOK_RBRACKET0)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '[', TOK_LBRACKET1)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, ']', TOK_RBRACKET1)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '{', TOK_LBRACKET2)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '}', TOK_RBRACKET2)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '.', TOK_DOT)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '@', TOK_AT)) {
    return;
  }

  if (scanner_tryFetchBinary1(s, '\'', TOK_SINGLE_QUOTE)) {
    return;
  }

  //scanner_setToken(s, s->m_text[s->m_ptr].unicode());
  if (s->m_text[s->m_ptr] == '[') {
    s->m_bracketDepth++;
  }

  if (s->m_text[s->m_ptr] == ']') {
    s->m_bracketDepth = MIN(0, s->m_bracketDepth - 1);
  }

  if (s->m_text[s->m_ptr] == '{') {
    s->m_bracketDepth++;
  }

  if (s->m_text[s->m_ptr] == '}') {
    s->m_bracketDepth = MIN(0, s->m_bracketDepth - 1);
  }

  s->m_ptr++;
}


void scanner_fetch(scanner_t* s)
{
  if (s->m_ptr >= s->m_strlen) {
    scanner_setToken(s, TOK_EOF, 0, 0);
  }
  else if (scanner_current(s) == '%') {
    scanner_fetchComment(s);
    return;
  }
  else if ((scanner_current(s) == '.') && (scanner_ahead(s, 1) == '.') && (scanner_ahead(s, 2) == '.')) {
    scanner_fetchContinuation(s);
    return;
  }
  else if (s->m_blobFlag && !isablank(scanner_current(s)) &&
      (scanner_current(s) != '\n') && (scanner_current(s) != ';') &&
      (scanner_current(s) != ',') && (scanner_current(s) != '\'') &&
      (scanner_current(s) != '%')) {
    scanner_fetchBlob(s);
  }
  else if (isalpha(scanner_current(s))) {
    scanner_fetchIdentifier(s);
  }
  else if (isdigit(scanner_current(s)) || ((scanner_current(s) == '.') && isdigit(scanner_ahead(s, 1)))) {
    scanner_fetchNumber(s);
  }
  else if (isablank(scanner_current(s))) {
    scanner_fetchWhitespace(s);

    if (s->m_ignorews[s->m_ignorewslen - 1]) {
      return;
    }
  }
  else if ((scanner_current(s) == '\'') && !((scanner_previous(s) == '\'') ||
      (scanner_previous(s) == ')') ||
      (scanner_previous(s) == ']') ||
      (scanner_previous(s) == '}') ||
      (isalnumus(scanner_previous(s))))) {
    scanner_fetchString(s);
  }
  else {
    scanner_fetchOther(s);
  }

  s->m_tokValid = TRUE;
}

token_t* scanner_next(scanner_t* s)
{
  while (!s->m_tokValid) {
    scanner_fetch(s);

    if ((s->m_ptr < s->m_strlen) && (scanner_current(s) == '\n')) {
      s->m_linenumber++;
    }
  }

  if (s->m_inContinuationState && s->m_tokValid && !(s->m_tok->m_tok == TOK_EOF)) {
    s->m_inContinuationState = FALSE;
  }

  return s->m_tok;
}

int scanner_snippet(scanner_t* s, unsigned pos1, unsigned pos2, token_t* tok)
{
  unsigned ptr1 = pos1 >> 16;
  unsigned ptr2 = pos2 >> 16;
  tok->m_text = s->m_text + ptr1;
  tok->m_textlen = ptr2 - ptr1 - 1;
  return 0;
}

int scanner_context(scanner_t* s, unsigned pos, str_t* str)
{
  str_t prevline[1] = {0};
  str_t retstring[1] = {0};
  int line_start = 0;
  int linenumber = 1;
  int line_stop;
  int offset;
  pos = pos >> 16;
  line_stop = STR_find_chr(s->m_text, s->m_strlen, '\n', 0);

  while (((int)(pos) > line_stop) && (linenumber < 10000)) {
    str_setstr(prevline, s->m_text + line_start, line_stop - line_start);
    line_start = line_stop + 1;
    line_stop = STR_find_chr(s->m_text, s->m_strlen, '\n', line_start);
    linenumber++;
  }

  if ((strlen(s->m_filename) > 0) && (linenumber < 10000)) {
    str_format(retstring, 0, " at line number: %d of file %s\n", linenumber, s->m_filename);
  }
  else {
    str_setstr(retstring, "\n", -1);
  }

  str_format(retstring, 1, "     %s\n     ", prevline->s);
  str_cat(retstring, STR2(s->m_text + line_start, line_stop - line_start));
  offset = pos - line_start - 1;

  if (offset < 0) {
    offset = 0;
  }

  str_cat(retstring, STR1("\n     "));
  str_cat_nchr(retstring, ' ', offset);
  str_cat(retstring, STR1("^"));
  str_setstr(str, retstring->s, retstring->l);
  str_free(prevline);
  str_free(retstring);
  return 0;
}

int scanner_context2(scanner_t* s, str_t* str)
{
  return scanner_context(s, scanner_contextNum(s), str);
}
