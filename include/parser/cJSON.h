
#ifndef _CJSON_INL_
#define _CJSON_INL_
/* cjson */
/* JSON parser in C. */
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
/* cjson Types: */
#define cjson_False 0
#define cjson_True 1
#define cjson_NULL 2
#define cjson_Number 3
#define cjson_String 4
#define cjson_Array 5
#define cjson_Object 6
#define cjson_IsReference 256
#define cjson_StringIsConst 512
/* The cjson structure: */

typedef struct cjson {
	struct cjson* next, *prev; /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
  struct cjson* child; /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
  int type; /* The type of the item, as above. */
  char* valuestring; /* The item's string, if type==cjson_String */
  int valueint; /* The item's number, if type==cjson_Number */
  double valuedouble; /* The item's number, if type==cjson_Number */
  char* string; /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} cjson;

typedef struct cjson_Hooks {
  void* (*malloc_fn)(size_t sz);
  void (*free_fn)(void* ptr);
} cjson_Hooks;
/* Macros for creating things quickly. */
#define cjson_AddArrayToObject(object,name) cjson_AddItemToObject(object, name, cjson_CreateArray())
#define cjson_AddNullToObject(object,name) cjson_AddItemToObject(object, name, cjson_CreateNull())
#define cjson_AddTrueToObject(object,name) cjson_AddItemToObject(object, name, cjson_CreateTrue())
#define cjson_AddFalseToObject(object,name) cjson_AddItemToObject(object, name, cjson_CreateFalse())
#define cjson_AddBoolToObject(object,name,b) cjson_AddItemToObject(object, name, cjson_CreateBool(b))
#define cjson_AddIntToObject(object,name,n) cjson_AddItemToObject(object, name, cjson_CreateInt(n))
#define cjson_AddNumberToObject(object,name,n) cjson_AddItemToObject(object, name, cjson_CreateNumber(n))
#define cjson_AddStringToObject(object,name,s) cjson_AddItemToObject(object, name, cjson_CreateString(s))

#define cjson_AddNullToArray(object) cjson_AddItemToArray(object, cjson_CreateNull())
#define cjson_AddTrueToArray(object) cjson_AddItemToArray(object, cjson_CreateTrue())
#define cjson_AddFalseToArray(object) cjson_AddItemToArray(object, cjson_CreateFalse())
#define cjson_AddBoolToArray(object,b) cjson_AddItemToArray(object, cjson_CreateBool(b))
#define cjson_AddNumberToArray(object,n) cjson_AddItemToArray(object, cjson_CreateNumber(n))
#define cjson_AddStringToArray(object,s) cjson_AddItemToArray(object, cjson_CreateString(s))

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define cjson_SetIntValue(object,val) ((object)?(object)->valueint=(object)->valuedouble=(val):(val))
#define cjson_SetNumberValue(object,val) ((object)?(object)->valueint=(object)->valuedouble=(val):(val))
static const char* ep;
static const char* cjson_GetErrorPtr(void)
{
  return ep;
}
static int cjson_strcasecmp(const char* s1, const char* s2)
{
  if (!s1) {
    return (s1 == s2) ? 0 : 1;
  }
  if (!s2) {
    return 1;
  }
  for (; tolower(*s1) == tolower(*s2); ++s1, ++s2) {
    if (*s1 == 0) {
      return 0;
    }
  }
  return tolower(*(const unsigned char*)s1) - tolower(*(const unsigned char*)s2);
}
static void* (*cjson_malloc)(size_t sz) = malloc;
static void (*cjson_free)(void* ptr) = free;
static char* cjson_strdup(const char* str)
{
  size_t len;
  char* copy;
  len = strlen(str) + 1;
  if (!(copy = (char*)cjson_malloc(len))) {
    return 0;
  }
  memcpy(copy, str, len);
  return copy;
}
static void cjson_InitHooks(cjson_Hooks* hooks)
{
  if (!hooks) { /* Reset hooks */
    cjson_malloc = malloc;
    cjson_free = free;
    return;
  }
  cjson_malloc = (hooks->malloc_fn) ? hooks->malloc_fn : malloc;
  cjson_free = (hooks->free_fn) ? hooks->free_fn : free;
}
/* Internal constructor. */
static cjson* cjson_New_Item(void)
{
  cjson* node = (cjson*)cjson_malloc(sizeof(cjson));
  if (node) {
    memset(node, 0, sizeof(cjson));
  }
  return node;
}
static void cjson_Delete(cjson* c);
void cjson_Clear(cjson* c) {
  if (!(c->type & cjson_IsReference) && c->child) {
    cjson_Delete(c->child);
    c->child = NULL;
  }
  if (!(c->type & cjson_IsReference) && c->valuestring) {
    cjson_free(c->valuestring);
    c->valuestring = NULL;
  }
}
void cjson_DeleteOne(cjson* c) {
  if (c->next) {
    c->next->prev = c->prev;
  }
  if (c->prev) {
    c->prev->next = c->next;
  }
  cjson_Clear(c);
  if (!(c->type & cjson_StringIsConst) && c->string) {
    cjson_free(c->string);
    c->string = NULL;
  }
  cjson_free(c);
}
static cjson* cjson_SetItem(cjson* c, cjson* new_item) {
  if (c->next) {
    c->next->prev = new_item;
  }
  if (c->prev) {
    c->prev->next = new_item;
  }
  cjson_Clear(c);
  if (!(c->type & cjson_StringIsConst) && c->string) {
    cjson_free(c->string);
    c->string = NULL;
  }
  cjson_free(c);
  return new_item;
}
/* Delete a cjson structure. */
void cjson_Delete(cjson* c)
{
  cjson* next;
  while (c) {
    next = c->next;
    cjson_DeleteOne(c);
    c = next;
  }
}
/* Parse the input text to generate a number, and populate the result into item. */
static const char* parse_number(cjson* item, const char* num)
{
  double n = 0, sign = 1, scale = 0;
  int subscale = 0, signsubscale = 1;
  if (*num == '-') {
    sign = -1, num++; /* Has sign? */
  }
  if (*num == '0') {
    num++; /* is zero */
  }
  if (*num >= '1' && *num <= '9') do {
      n = (n * 10.0) + (*num++ -'0');
    }
    while (*num >= '0' && *num <= '9'); /* Number? */
  if (*num == '.' && num[1] >= '0' && num[1] <= '9') {
    num++; /* Fractional part? */
    do {
      n = (n * 10.0) + (*num++ -'0'), scale--;
    }
    while (*num >= '0' && *num <= '9');
  }
  if (*num == 'e' || *num == 'E') { /* Exponent? */
    num++;
    if (*num == '+') {
      num++;
    }
    else if (*num == '-') {
      signsubscale = -1, num++; /* With sign? */
    }
    while (*num >= '0' && *num <= '9') {
      subscale = (subscale * 10) + (*num++ - '0'); /* Number? */
    }
  }
  n = sign * n * pow(10.0, (scale + subscale * signsubscale)); /* number = +/- number.fraction * 10^+/- exponent */
  item->valuedouble = n;
  item->valueint = (int)n;
  item->type = cjson_Number;
  return num;
}
static int pow2gt(int x)
{
  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x + 1;
}
typedef struct {
  char* buffer;
  int length;
  int offset;
} printbuffer;
static char* ensure(printbuffer* p, int needed)
{
  char* newbuffer;
  int newsize;
  if (!p || !p->buffer) {
    return 0;
  }
  needed += p->offset;
  if (needed <= p->length) {
    return p->buffer + p->offset;
  }
  newsize = pow2gt(needed);
  newbuffer = (char*)cjson_malloc(newsize);
  if (!newbuffer) {
    cjson_free(p->buffer);
    p->length = 0, p->buffer = 0;
    return 0;
  }
  if (newbuffer) {
    memcpy(newbuffer, p->buffer, p->length);
  }
  cjson_free(p->buffer);
  p->length = newsize;
  p->buffer = newbuffer;
  return newbuffer + p->offset;
}
static int update(printbuffer* p)
{
  char* str;
  if (!p || !p->buffer) {
    return 0;
  }
  str = p->buffer + p->offset;
  return p->offset + (int)strlen(str);
}
/* Render the number nicely from the given item into a string. */
static char* print_number(cjson* item, printbuffer* p)
{
  char* str = 0;
  double d = item->valuedouble;
  if (d == 0) {
    if (p) {
      str = ensure(p, 2);
    }
    else {
      str = (char*)cjson_malloc(2); /* special case for 0. */
    }
    if (str) {
      strcpy(str, "0");
    }
  }
  else if (fabs(((double)item->valueint) - d) <= DBL_EPSILON && d <= INT_MAX && d >= INT_MIN) {
    if (p) {
      str = ensure(p, 21);
    }
    else {
      str = (char*)cjson_malloc(21); /* 2^64+1 can be represented in 21 chars. */
    }
    if (str) {
      sprintf(str, "%d", item->valueint);
    }
  }
  else {
    if (p) {
      str = ensure(p, 64);
    }
    else {
      str = (char*)cjson_malloc(64); /* This is a nice tradeoff. */
    }
    if (str) {
      if (fabs(floor(d) - d) <= DBL_EPSILON && fabs(d) < 1.0e60) {
        sprintf(str, "%.0f", d);
      }
      else if (fabs(d) < 1.0e-6 || fabs(d) > 1.0e9) {
        sprintf(str, "%e", d);
      }
      else {
        sprintf(str, "%g", d);
      }
    }
  }
  return str;
}
static unsigned parse_hex4(const char* str)
{
  unsigned h = 0;
  if (*str >= '0' && *str <= '9') {
    h += (*str) - '0';
  }
  else if (*str >= 'A' && *str <= 'F') {
    h += 10 + (*str) - 'A';
  }
  else if (*str >= 'a' && *str <= 'f') {
    h += 10 + (*str) - 'a';
  }
  else {
    return 0;
  }
  h = h << 4;
  str++;
  if (*str >= '0' && *str <= '9') {
    h += (*str) - '0';
  }
  else if (*str >= 'A' && *str <= 'F') {
    h += 10 + (*str) - 'A';
  }
  else if (*str >= 'a' && *str <= 'f') {
    h += 10 + (*str) - 'a';
  }
  else {
    return 0;
  }
  h = h << 4;
  str++;
  if (*str >= '0' && *str <= '9') {
    h += (*str) - '0';
  }
  else if (*str >= 'A' && *str <= 'F') {
    h += 10 + (*str) - 'A';
  }
  else if (*str >= 'a' && *str <= 'f') {
    h += 10 + (*str) - 'a';
  }
  else {
    return 0;
  }
  h = h << 4;
  str++;
  if (*str >= '0' && *str <= '9') {
    h += (*str) - '0';
  }
  else if (*str >= 'A' && *str <= 'F') {
    h += 10 + (*str) - 'A';
  }
  else if (*str >= 'a' && *str <= 'f') {
    h += 10 + (*str) - 'a';
  }
  else {
    return 0;
  }
  return h;
}
/* Parse the input text into an unescaped cstring, and populate item. */
static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static const char* parse_string(cjson* item, const char* str)
{
  const char* ptr = str + 1;
  char* ptr2;
  char* out;
  int len = 0;
  unsigned uc, uc2;
  if (*str != '\"') {
    ep = str; /* not a string! */
    return 0;
  }
  while (*ptr != '\"' && *ptr && ++len) if (*ptr++ == '\\') {
      ptr++; /* Skip escaped quotes. */
    }
  out = (char*)cjson_malloc(len + 1); /* This is how long we need for the string, roughly. */
  if (!out) {
    return 0;
  }
  ptr = str + 1;
  ptr2 = out;
  while (*ptr != '\"' && *ptr) {
    if (*ptr != '\\') {
      *ptr2++ = *ptr++;
    }
    else {
      ptr++;
      switch (*ptr) {
      case 'b':
        *ptr2++ = '\b';
        break;
      case 'f':
        *ptr2++ = '\f';
        break;
      case 'n':
        *ptr2++ = '\n';
        break;
      case 'r':
        *ptr2++ = '\r';
        break;
      case 't':
        *ptr2++ = '\t';
        break;
      case 'u': /* transcode utf16 to utf8. */
        uc = parse_hex4(ptr + 1);
        ptr += 4; /* get the unicode char. */
        if ((uc >= 0xDC00 && uc <= 0xDFFF) || uc == 0) {
          break; /* check for invalid. */
        }
        if (uc >= 0xD800 && uc <= 0xDBFF) { /* UTF16 surrogate pairs. */
          if (ptr[1] != '\\' || ptr[2] != 'u') {
            break; /* missing second-half of surrogate. */
          }
          uc2 = parse_hex4(ptr + 3);
          ptr += 6;
          if (uc2 < 0xDC00 || uc2 > 0xDFFF) {
            break; /* invalid second-half of surrogate. */
          }
          uc = 0x10000 + (((uc & 0x3FF) << 10) | (uc2 & 0x3FF));
        }
        len = 4;
        if (uc < 0x80) {
          len = 1;
        }
        else if (uc < 0x800) {
          len = 2;
        }
        else if (uc < 0x10000) {
          len = 3;
        }
        ptr2 += len;
        switch (len) {
        case 4:
          *--ptr2 = ((uc | 0x80) & 0xBF);
          uc >>= 6;
        case 3:
          *--ptr2 = ((uc | 0x80) & 0xBF);
          uc >>= 6;
        case 2:
          *--ptr2 = ((uc | 0x80) & 0xBF);
          uc >>= 6;
        case 1:
          *--ptr2 = (uc | firstByteMark[len]);
        }
        ptr2 += len;
        break;
      default:
        *ptr2++ = *ptr;
        break;
      }
      ptr++;
    }
  }
  *ptr2 = 0;
  if (*ptr == '\"') {
    ptr++;
  }
  item->valuestring = out;
  item->type = cjson_String;
  return ptr;
}
/* Render the cstring provided to an escaped version that can be printed. */
static char* print_string_ptr(const char* str, printbuffer* p)
{
  const char* ptr;
  char* ptr2, *out;
  int len = 0, flag = 0;
  unsigned char token;
  assert(str);
  for (ptr = str; *ptr; ptr++) {
    flag |= ((*ptr > 0 && *ptr < 32) || (*ptr == '\"') || (*ptr == '\\')) ? 1 : 0;
  }
  if (!flag) {
    len = (int)(ptr - str);
    if (p) {
      out = ensure(p, len + 3);
    }
    else {
      out = (char*)cjson_malloc(len + 3);
    }
    if (!out) {
      return 0;
    }
    ptr2 = out;
    *ptr2++ = '\"';
    strcpy(ptr2, str);
    ptr2[len] = '\"';
    ptr2[len + 1] = 0;
    return out;
  }
  if (!str) {
    if (p) {
      out = ensure(p, 3);
    }
    else {
      out = (char*)cjson_malloc(3);
    }
    if (!out) {
      return 0;
    }
    strcpy(out, "\"\"");
    return out;
  }
  ptr = str;
  while ((token = *ptr) && ++len) {
    if (strchr("\"\\\b\f\n\r\t", token)) {
      len++;
    }
    else if (token < 32) {
      len += 5;
    }
    ptr++;
  }
  if (p) {
    out = ensure(p, len + 3);
  }
  else {
    out = (char*)cjson_malloc(len + 3);
  }
  if (!out) {
    return 0;
  }
  ptr2 = out;
  ptr = str;
  *ptr2++ = '\"';
  while (*ptr) {
    if ((unsigned char)*ptr > 31 && *ptr != '\"' && *ptr != '\\') {
      *ptr2++ = *ptr++;
    }
    else {
      *ptr2++ = '\\';
      switch (token = *ptr++) {
      case '\\':
        *ptr2++ = '\\';
        break;
      case '\"':
        *ptr2++ = '\"';
        break;
      case '\b':
        *ptr2++ = 'b';
        break;
      case '\f':
        *ptr2++ = 'f';
        break;
      case '\n':
        *ptr2++ = 'n';
        break;
      case '\r':
        *ptr2++ = 'r';
        break;
      case '\t':
        *ptr2++ = 't';
        break;
      default:
        sprintf(ptr2, "u%04x", token);
        ptr2 += 5;
        break; /* escape and print */
      }
    }
  }
  *ptr2++ = '\"';
  *ptr2++ = 0;
  return out;
}
/* Invote print_string_ptr (which is useful) on an item. */
static char* print_string(cjson* item, printbuffer* p)
{
  return print_string_ptr(item->valuestring, p);
}
/* Predeclare these prototypes. */
static const char* parse_value(cjson* item, const char* value);
static char* print_value(cjson* item, int depth, int fmt, printbuffer* p);
static const char* parse_array(cjson* item, const char* value);
static char* print_array(cjson* item, int depth, int fmt, printbuffer* p);
static const char* parse_object(cjson* item, const char* value);
static char* print_object(cjson* item, int depth, int fmt, printbuffer* p);
/* Utility to jump whitespace and cr/lf */
static const char* skip(const char* in)
{
  while (in && *in && (unsigned char)*in <= 32) {
    in++;
  }
  return in;
}
/* Parse an object - create a new root, and populate. */
static cjson* cjson_ParseWithOpts(const char* value, const char** return_parse_end, int require_null_terminated)
{
  const char* end = 0;
  cjson* c = cjson_New_Item();
  ep = 0;
  if (!c) {
    return 0; /* memory fail */
  }
  end = parse_value(c, skip(value));
  if (!end) {
    cjson_Delete(c); /* parse failure. ep is set. */
    return 0;
  }
  /* if we require null-terminated JSON without appended garbage, skip and then check for a null terminator */
  if (require_null_terminated) {
    end = skip(end);
    if (*end) {
      cjson_Delete(c);
      ep = end;
      return 0;
    }
  }
  if (return_parse_end) {
    *return_parse_end = end;
  }
  return c;
}
/* Default options for cjson_Parse */
static cjson* cjson_Parse(const char* value)
{
  return cjson_ParseWithOpts(value, 0, 0);
}
/* Render a cjson item/entity/structure to text. */
static char* cjson_Print(cjson* item)
{
  return print_value(item, 0, 1, 0);
}
static char* cjson_PrintUnformatted(cjson* item)
{
  return print_value(item, 0, 0, 0);
}
static char* cjson_PrintBuffered(cjson* item, int prebuffer, int fmt)
{
  printbuffer p;
  p.buffer = (char*)cjson_malloc(prebuffer);
  p.length = prebuffer;
  p.offset = 0;
  return print_value(item, 0, fmt, &p);
  return p.buffer;
}
/* Parser core - when encountering text, process appropriately. */
static const char* parse_value(cjson* item, const char* value)
{
  if (!value) {
    return 0; /* Fail on null. */
  }
  if (!strncmp(value, "null", 4)) {
    item->type = cjson_NULL;
    return value + 4;
  }
  if (!strncmp(value, "false", 5)) {
    item->type = cjson_False;
    return value + 5;
  }
  if (!strncmp(value, "true", 4)) {
    item->type = cjson_True;
    item->valueint = 1;
    return value + 4;
  }
  if (*value == '\"') {
    return parse_string(item, value);
  }
  if (*value == '-' || (*value >= '0' && *value <= '9')) {
    return parse_number(item, value);
  }
  if (*value == '[') {
    return parse_array(item, value);
  }
  if (*value == '{') {
    return parse_object(item, value);
  }
  ep = value;
  return 0; /* failure. */
}
/* Render a value to text. */
static char* print_value(cjson* item, int depth, int fmt, printbuffer* p)
{
  char* out = 0;
  if (!item) {
    return 0;
  }
  if (p) {
    switch ((item->type) & 255) {
    case cjson_NULL: {
      out = ensure(p, 5);
      if (out) {
        strcpy(out, "null");
      }
      break;
    }
    case cjson_False: {
      out = ensure(p, 6);
      if (out) {
        strcpy(out, "false");
      }
      break;
    }
    case cjson_True: {
      out = ensure(p, 5);
      if (out) {
        strcpy(out, "true");
      }
      break;
    }
    case cjson_Number:
      out = print_number(item, p);
      break;
    case cjson_String:
      out = print_string(item, p);
      break;
    case cjson_Array:
      out = print_array(item, depth, fmt, p);
      break;
    case cjson_Object:
      out = print_object(item, depth, fmt, p);
      break;
    }
  }
  else {
    switch ((item->type) & 255) {
    case cjson_NULL:
      out = cjson_strdup("null");
      break;
    case cjson_False:
      out = cjson_strdup("false");
      break;
    case cjson_True:
      out = cjson_strdup("true");
      break;
    case cjson_Number:
      out = print_number(item, 0);
      break;
    case cjson_String:
      out = print_string(item, 0);
      break;
    case cjson_Array:
      out = print_array(item, depth, fmt, 0);
      break;
    case cjson_Object:
      out = print_object(item, depth, fmt, 0);
      break;
    }
  }
  return out;
}
/* Build an array from input text. */
static const char* parse_array(cjson* item, const char* value)
{
  cjson* child;
  if (*value != '[') {
    ep = value; /* not an array! */
    return 0;
  }
  item->type = cjson_Array;
  value = skip(value + 1);
  if (*value == ']') {
    return value + 1; /* empty array. */
  }
  item->child = child = cjson_New_Item();
  if (!item->child) {
    return 0; /* memory fail */
  }
  value = skip(parse_value(child, skip(value))); /* skip any spacing, get the value. */
  if (!value) {
    return 0;
  }
  while (*value == ',') {
    cjson* new_item;
    value = skip(value + 1);
    if (*value == ']') {
      return value + 1; /* end of array */
    }
    if (!(new_item = cjson_New_Item())) {
      return 0; /* memory fail */
    }
    child->next = new_item;
    new_item->prev = child;
    child = new_item;
    value = skip(parse_value(child, value));
    if (!value) {
      return 0; /* memory fail */
    }
  }
  if (*value == ']') {
    return value + 1; /* end of array */
  }
  ep = value;
  return 0; /* malformed. */
}
/* Render an array to text */
static char* print_array(cjson* item, int depth, int fmt, printbuffer* p)
{
  char** entries;
  char* out = 0, *ptr, *ret;
  int len = 5;
  cjson* child = item->child;
  int numentries = 0, i = 0, fail = 0;
  size_t tmplen = 0;
  /* How many entries in the array? */
  while (child) {
    numentries++, child = child->next;
  }
  /* Explicitly handle numentries==0 */
  if (!numentries) {
    if (p) {
      out = ensure(p, 3);
    }
    else {
      out = (char*)cjson_malloc(3);
    }
    if (out) {
      strcpy(out, "[]");
    }
    return out;
  }
  if (p) {
    /* Compose the output array. */
    i = p->offset;
    ptr = ensure(p, 1);
    if (!ptr) {
      return 0;
    } *ptr = '[';
    p->offset++;
    child = item->child;
    while (child && !fail) {
      print_value(child, depth + 1, fmt, p);
      p->offset = update(p);
      if (child->next) {
        len = fmt ? 2 : 1;
        ptr = ensure(p, len + 1);
        if (!ptr) {
          return 0;
        }*ptr++ = ',';
        if (fmt) {
          *ptr++ = ' ';
        }*ptr = 0;
        p->offset += len;
      }
      child = child->next;
    }
    ptr = ensure(p, 2);
    if (!ptr) {
      return 0;
    } *ptr++ = ']';
    *ptr = 0;
    out = (p->buffer) + i;
  }
  else {
    /* Allocate an array to hold the values for each */
    entries = (char**)cjson_malloc(numentries * sizeof(char*));
    if (!entries) {
      return 0;
    }
    memset(entries, 0, numentries * sizeof(char*));
    /* Retrieve all the results: */
    child = item->child;
    while (child && !fail) {
      ret = print_value(child, depth + 1, fmt, 0);
      entries[i++] = ret;
      if (ret) {
        len += (int)strlen(ret) + 2 + (fmt ? 1 : 0);
      }
      else {
        fail = 1;
      }
      child = child->next;
    }
    /* If we didn't fail, try to malloc the output string */
    if (!fail) {
      out = (char*)cjson_malloc(len);
    }
    /* If that fails, we fail. */
    if (!out) {
      fail = 1;
    }
    /* Handle failure. */
    if (fail) {
      for (i = 0; i < numentries; i++) if (entries[i]) {
          cjson_free(entries[i]);
        }
      cjson_free(entries);
      return 0;
    }
    /* Compose the output array. */
    *out = '[';
    ptr = out + 1;
    *ptr = 0;
    for (i = 0; i < numentries; i++) {
      tmplen = strlen(entries[i]);
      memcpy(ptr, entries[i], tmplen);
      ptr += tmplen;
      if (i != numentries - 1) {
        *ptr++ = ',';
        if (fmt) {
          *ptr++ = ' ';
        }*ptr = 0;
      }
      cjson_free(entries[i]);
    }
    cjson_free(entries);
    *ptr++ = ']';
    *ptr++ = 0;
  }
  return out;
}
/* Build an object from the text. */
static const char* parse_object(cjson* item, const char* value)
{
  cjson* child;
  if (*value != '{') {
    ep = value; /* not an object! */
    return 0;
  }
  item->type = cjson_Object;
  value = skip(value + 1);
  if (*value == '}') {
    return value + 1; /* empty array. */
  }
  item->child = child = cjson_New_Item();
  if (!item->child) {
    return 0;
  }
  value = skip(parse_string(child, skip(value)));
  if (!value) {
    return 0;
  }
  child->string = child->valuestring;
  child->valuestring = 0;
  if (*value != ':') {
    ep = value; /* fail! */
    return 0;
  }
  value = skip(parse_value(child, skip(value + 1))); /* skip any spacing, get the value. */
  if (!value) {
    return 0;
  }
  while (*value == ',') {
    cjson* new_item;
    value = skip(value + 1);
    if (*value == '}') {
      return value + 1; /* end of array */
    }
    if (!(new_item = cjson_New_Item())) {
      return 0; /* memory fail */
    }
    child->next = new_item;
    new_item->prev = child;
    child = new_item;
    value = skip(parse_string(child, value));
    if (!value) {
      return 0;
    }
    child->string = child->valuestring;
    child->valuestring = 0;
    if (*value != ':') {
      ep = value; /* fail! */
      return 0;
    }
    value = skip(parse_value(child, skip(value + 1))); /* skip any spacing, get the value. */
    if (!value) {
      return 0;
    }
  }
  if (*value == '}') {
    return value + 1; /* end of array */
  }
  ep = value;
  return 0; /* malformed. */
}
/* Render an object to text. */
static char* print_object(cjson* item, int depth, int fmt, printbuffer* p)
{
  char** entries = 0, **names = 0;
  char* out = 0, *ptr, *ret, *str;
  int len = 7, i = 0, j;
  cjson* child = item->child;
  int numentries = 0, fail = 0;
  size_t tmplen = 0;
  /* Count the number of entries. */
  while (child) {
    numentries++, child = child->next;
  }
  /* Explicitly handle empty object case */
  if (!numentries) {
    if (p) {
      out = ensure(p, fmt ? depth + 4 : 3);
    }
    else {
      out = (char*)cjson_malloc(fmt ? depth + 4 : 3);
    }
    if (!out) {
      return 0;
    }
    ptr = out;
    *ptr++ = '{';
    if (fmt) {
      *ptr++ = '\n';
      for (i = 0; i < depth - 1; i++) {
        *ptr++ = '\t';
      }
    }
    *ptr++ = '}';
    *ptr++ = 0;
    return out;
  }
  if (p) {
    /* Compose the output: */
    i = p->offset;
    len = fmt ? 2 : 1;
    ptr = ensure(p, len + 1);
    if (!ptr) {
      return 0;
    }
    *ptr++ = '{';
    if (fmt) {
      *ptr++ = '\n';
    } *ptr = 0;
    p->offset += len;
    child = item->child;
    depth++;
    while (child) {
      if (fmt) {
        ptr = ensure(p, depth);
        if (!ptr) {
          return 0;
        }
        for (j = 0; j < depth; j++) {
          *ptr++ = '\t';
        }
        p->offset += depth;
      }
      print_string_ptr(child->string, p);
      p->offset = update(p);
      len = fmt ? 2 : 1;
      ptr = ensure(p, len);
      if (!ptr) {
        return 0;
      }
      *ptr++ = ':';
      if (fmt) {
        *ptr++ = '\t';
      }
      p->offset += len;
      print_value(child, depth, fmt, p);
      p->offset = update(p);
      len = (fmt ? 1 : 0) + (child->next ? 1 : 0);
      ptr = ensure(p, len + 1);
      if (!ptr) {
        return 0;
      }
      if (child->next) {
        *ptr++ = ',';
      }
      if (fmt) {
        *ptr++ = '\n';
      }*ptr = 0;
      p->offset += len;
      child = child->next;
    }
    ptr = ensure(p, fmt ? (depth + 1) : 2);
    if (!ptr) {
      return 0;
    }
    if (fmt) for (i = 0; i < depth - 1; i++) {
        *ptr++ = '\t';
      }
    *ptr++ = '}';
    *ptr = 0;
    out = (p->buffer) + i;
  }
  else {
    /* Allocate space for the names and the objects */
    entries = (char**)cjson_malloc(numentries * sizeof(char*));
    if (!entries) {
      return 0;
    }
    names = (char**)cjson_malloc(numentries * sizeof(char*));
    if (!names) {
      cjson_free(entries);
      return 0;
    }
    memset(entries, 0, sizeof(char*)*numentries);
    memset(names, 0, sizeof(char*)*numentries);
    /* Collect all the results into our arrays: */
    child = item->child;
    depth++;
    if (fmt) {
      len += depth;
    }
    while (child) {
      names[i] = str = print_string_ptr(child->string, 0);
      entries[i++] = ret = print_value(child, depth, fmt, 0);
      if (str && ret) {
        len += (int)strlen(ret) + (int)strlen(str) + 2 + (fmt ? 2 + depth : 0);
      }
      else {
        fail = 1;
      }
      child = child->next;
    }
    /* Try to allocate the output string */
    if (!fail) {
      out = (char*)cjson_malloc(len);
    }
    if (!out) {
      fail = 1;
    }
    /* Handle failure */
    if (fail) {
      for (i = 0; i < numentries; i++) {
        if (names[i]) {
          cjson_free(names[i]);
        }
        if (entries[i]) {
          cjson_free(entries[i]);
        }
      }
      cjson_free(names);
      cjson_free(entries);
      return 0;
    }
    /* Compose the output: */
    *out = '{';
    ptr = out + 1;
    if (fmt) {
      *ptr++ = '\n';
    }*ptr = 0;
    for (i = 0; i < numentries; i++) {
      if (fmt) for (j = 0; j < depth; j++) {
          *ptr++ = '\t';
        }
      tmplen = strlen(names[i]);
      memcpy(ptr, names[i], tmplen);
      ptr += tmplen;
      *ptr++ = ':';
      if (fmt) {
        *ptr++ = '\t';
      }
      strcpy(ptr, entries[i]);
      ptr += strlen(entries[i]);
      if (i != numentries - 1) {
        *ptr++ = ',';
      }
      if (fmt) {
        *ptr++ = '\n';
      }*ptr = 0;
      cjson_free(names[i]);
      cjson_free(entries[i]);
    }
    cjson_free(names);
    cjson_free(entries);
    if (fmt) for (i = 0; i < depth - 1; i++) {
        *ptr++ = '\t';
      }
    *ptr++ = '}';
    *ptr++ = 0;
  }
  return out;
}
/* Get Array size/item / object item. */
static int cjson_GetArraySize(const cjson* array)
{
  if (array) {
    const cjson* c = array->child;
    int i = 0;
    while (c) {
      i++, c = c->next;
    }
    return i;
  }
  return 0;
}
static cjson* cjson_GetArrayItem(cjson* array, int item)
{
  if (array) {
    cjson* c = array->child;
    while (c && item > 0) {
      item--, c = c->next;
    }
    return c;
  }
  return NULL;
}
cjson* cjson_GetObjectItem(cjson* object, const char* string)
{
  if (object) {
    cjson* c = object->child;
    while (c && cjson_strcasecmp(c->string, string)) {
      c = c->next;
    }
    return c;
  }
  return NULL;
}
/* Utility for array list handling. */
static void suffix_object(cjson* prev, cjson* item)
{
  prev->next = item;
  item->prev = prev;
}
/* Utility for handling references. */
static cjson* create_reference(cjson* item)
{
  cjson* ref = cjson_New_Item();
  if (!ref) {
    return 0;
  }
  memcpy(ref, item, sizeof(cjson));
  ref->string = 0;
  ref->type |= cjson_IsReference;
  ref->next = ref->prev = 0;
  return ref;
}
/* Add item to array/object. */
static cjson* cjson_AddItemToArray(cjson* array, cjson* item)
{
  cjson* c = array->child;
  if (!item) {
    return item;
  }
  if (!c) {
    array->child = item;
  }
  else {
    while (c && c->next) {
      c = c->next;
    }
    suffix_object(c, item);
  }
  return item;
}
static cjson* cjson_AddItemToObject(cjson* object, const char* string, cjson* item)
{
  if (!item) {
    return item;
  }
  if (item->string) {
    cjson_free(item->string);
  }
  item->string = cjson_strdup(string);
  cjson_AddItemToArray(object, item);
  return item;
}
static cjson* cjson_AddItemToObjectCS(cjson* object, const char* string, cjson* item)
{
  if (!item) {
    return NULL;
  }
  if (!(item->type & cjson_StringIsConst) && item->string) {
    cjson_free(item->string);
  }
  item->string = (char*)string;
  item->type |= cjson_StringIsConst;
  cjson_AddItemToArray(object, item);
  return item;
}
static cjson* cjson_AddItemReferenceToArray(cjson* array, cjson* item)
{
  return cjson_AddItemToArray(array, create_reference(item));
}
static cjson* cjson_AddItemReferenceToObject(cjson* object, const char* string, cjson* item)
{
  return cjson_AddItemToObject(object, string, create_reference(item));
}
static cjson* cjson_DetachItemFromArray(cjson* array, int which)
{
  cjson* c = array->child;
  while (c && which > 0) {
    c = c->next, which--;
  }
  if (!c) {
    return 0;
  }
  if (c->prev) {
    c->prev->next = c->next;
  }
  if (c->next) {
    c->next->prev = c->prev;
  }
  if (c == array->child) {
    array->child = c->next;
  }
  c->prev = c->next = 0;
  return c;
}
static void cjson_DeleteItemFromArray(cjson* array, int which)
{
  cjson_Delete(cjson_DetachItemFromArray(array, which));
}
static cjson* cjson_DetachItemFromObject(cjson* object, const char* string)
{
  int i = 0;
  cjson* c = object->child;
  while (c && cjson_strcasecmp(c->string, string)) {
    i++, c = c->next;
  }
  if (c) {
    return cjson_DetachItemFromArray(object, i);
  }
  return 0;
}
static void cjson_DeleteItemFromObject(cjson* object, const char* string)
{
  cjson_Delete(cjson_DetachItemFromObject(object, string));
}
/* Replace array/object items with new ones. */
static void cjson_InsertItemInArray(cjson* array, int which, cjson* newitem)
{
  cjson* c = array->child;
  while (c && which > 0) {
    c = c->next, which--;
  }
  if (!c) {
    cjson_AddItemToArray(array, newitem);
    return;
  }
  newitem->next = c;
  newitem->prev = c->prev;
  c->prev = newitem;
  if (c == array->child) {
    array->child = newitem;
  }
  else {
    newitem->prev->next = newitem;
  }
}
static void cjson_ReplaceItemInArray(cjson* array, int which, cjson* newitem)
{
  cjson* c = array->child;
  while (c && which > 0) {
    c = c->next, which--;
  }
  if (!c) {
    return;
  }
  newitem->next = c->next;
  newitem->prev = c->prev;
  if (newitem->next) {
    newitem->next->prev = newitem;
  }
  if (c == array->child) {
    array->child = newitem;
  }
  else {
    newitem->prev->next = newitem;
  }
  c->next = c->prev = 0;
  cjson_Delete(c);
}
static void cjson_ReplaceItemInObject(cjson* object, const char* string, cjson* newitem)
{
  int i = 0;
  cjson* c = object->child;
  while (c && cjson_strcasecmp(c->string, string)) {
    i++, c = c->next;
  }
  if (c) {
    newitem->string = cjson_strdup(string);
    cjson_ReplaceItemInArray(object, i, newitem);
  }
}
/* Create basic types: */
static cjson* cjson_CreateNull(void)
{
  cjson* item = cjson_New_Item();
  if (item) {
    item->type = cjson_NULL;
  }
  return item;
}
static cjson* cjson_CreateTrue(void)
{
  cjson* item = cjson_New_Item();
  if (item) {
    item->type = cjson_True;
  }
  return item;
}
static cjson* cjson_CreateFalse(void)
{
  cjson* item = cjson_New_Item();
  if (item) {
    item->type = cjson_False;
  }
  return item;
}
static cjson* cjson_CreateBool(int b)
{
  cjson* item = cjson_New_Item();
  if (item) {
    item->type = b ? cjson_True : cjson_False;
  }
  return item;
}
#define _CJSON_SET_BOOL(item, num)       item->type = b ? cjson_True : cjson_False
#define _CJSON_SET_NUMBER(item, num)     item->type = cjson_Number;  item->valuedouble = num;  item->valueint = (int)num
#define _CJSON_SET_STRING(item, string)  item->type = cjson_String; item->valuestring = cjson_strdup(string)

static cjson* cjson_SetNumber(cjson* item, double num) {
  if (item) {
    cjson_Clear(item);
    _CJSON_SET_NUMBER(item, num);
  }
  return item;
}
static cjson* cjson_SetBool(cjson* item, bool b) {
  if (item) {
    cjson_Clear(item);
    _CJSON_SET_BOOL(item, b);
  }
  return item;
}
static cjson* cjson_CreateInt(int num)
{
  cjson* item = cjson_New_Item();
  if (item) {
    _CJSON_SET_NUMBER(item, num);
  }
  return item;
}
static cjson* cjson_CreateNumber(double num)
{
  cjson* item = cjson_New_Item();
  if (item) {
    _CJSON_SET_NUMBER(item, num);
  }
  return item;
}
static cjson* cjson_SetString(cjson* item, const char* string) {
  if (item) {
    cjson_Clear(item);
    _CJSON_SET_STRING(item, string);
  }
  return item;
}
static cjson* cjson_CreateString(const char* string)
{
  if (NULL == string) {
    int asdf = 0;
  }
  cjson* item = cjson_New_Item();
  if (item) {
    _CJSON_SET_STRING(item, string);
  }
  return item;
}
static cjson* cjson_CreateArray(void)
{
  cjson* item = cjson_New_Item();
  if (item) {
    item->type = cjson_Array;
  }
  return item;
}
static cjson* cjson_CreateObject(void)
{
  cjson* item = cjson_New_Item();
  if (item) {
    item->type = cjson_Object;
  }
  return item;
}
/* Create Arrays: */
static cjson* cjson_CreateIntArray(const int* numbers, int count)
{
  int i;
  cjson* n = 0, *p = 0, *a = cjson_CreateArray();
  for (i = 0; a && i < count; i++) {
    n = cjson_CreateNumber(numbers[i]);
    if (!i) {
      a->child = n;
    }
    else {
      suffix_object(p, n);
    }
    p = n;
  }
  return a;
}
static cjson* cjson_CreateFloatArray(const float* numbers, int count)
{
  int i;
  cjson* n = 0, *p = 0, *a = cjson_CreateArray();
  for (i = 0; a && i < count; i++) {
    n = cjson_CreateNumber(numbers[i]);
    if (!i) {
      a->child = n;
    }
    else {
      suffix_object(p, n);
    }
    p = n;
  }
  return a;
}
static cjson* cjson_CreateDoubleArray(const double* numbers, int count)
{
  int i;
  cjson* n = 0, *p = 0, *a = cjson_CreateArray();
  for (i = 0; a && i < count; i++) {
    n = cjson_CreateNumber(numbers[i]);
    if (!i) {
      a->child = n;
    }
    else {
      suffix_object(p, n);
    }
    p = n;
  }
  return a;
}
static cjson* cjson_CreateStringArray(const char** strings, int count)
{
  int i;
  cjson* n = 0, *p = 0, *a = cjson_CreateArray();
  for (i = 0; a && i < count; i++) {
    n = cjson_CreateString(strings[i]);
    if (!i) {
      a->child = n;
    }
    else {
      suffix_object(p, n);
    }
    p = n;
  }
  return a;
}
/* Duplication */
static cjson* cjson_Duplicate(cjson* item, int recurse)
{
  cjson* newitem, *cptr, *nptr = 0, *newchild;
  /* Bail on bad ptr */
  if (!item) {
    return 0;
  }
  /* Create new item */
  newitem = cjson_New_Item();
  if (!newitem) {
    return 0;
  }
  /* Copy over all vars */
  newitem->type = item->type & (~cjson_IsReference), newitem->valueint = item->valueint, newitem->valuedouble = item->valuedouble;
  if (item->valuestring) {
    newitem->valuestring = cjson_strdup(item->valuestring);
    if (!newitem->valuestring) {
      cjson_Delete(newitem);
      return 0;
    }
  }
  if (item->string) {
    newitem->string = cjson_strdup(item->string);
    if (!newitem->string) {
      cjson_Delete(newitem);
      return 0;
    }
  }
  /* If non-recursive, then we're done! */
  if (!recurse) {
    return newitem;
  }
  /* Walk the ->next chain for the child. */
  cptr = item->child;
  while (cptr) {
    newchild = cjson_Duplicate(cptr, 1); /* Duplicate (with recurse) each item in the ->next chain */
    if (!newchild) {
      cjson_Delete(newitem);
      return 0;
    }
    if (nptr) {
      nptr->next = newchild, newchild->prev = nptr; /* If newitem->child already set, then crosswire ->prev and ->next and move on */
      nptr = newchild;
    }
    else {
      newitem->child = newchild; /* Set newitem->child and move to it */
      nptr = newchild;
    }
    cptr = cptr->next;
  }
  return newitem;
}
static void cjson_Minify(char* json)
{
  char* into = json;
  while (*json) {
    if (*json == ' ') {
      json++;
    }
    else if (*json == '\t') {
      json++; /* Whitespace characters. */
    }
    else if (*json == '\r') {
      json++;
    }
    else if (*json == '\n') {
      json++;
    }
    else if (*json == '/' && json[1] == '/') while (*json && *json != '\n') {
        json++; /* double-slash comments, to end of line. */
      }
    else if (*json == '/' && json[1] == '*') {
      while (*json && !(*json == '*' && json[1] == '/')) {
        json++; /* multiline comments. */
      }
      json += 2;
    }
    else if (*json == '\"') {
      *into++ = *json++; /* string literals, which are \" sensitive. */
      while (*json && *json != '\"') {
        if (*json == '\\') {
          *into++ = *json++;
        }*into++ = *json++;
      }*into++ = *json++;
    }
    else {
      *into++ = *json++; /* All other characters. */
    }
  }
  *into = 0; /* and null-terminate. */
}
#ifdef TEST
/* Parse text to JSON, then render back to text, and print! */
static void doit(char* text)
{
  char* out;
  cjson* json;
  json = cjson_Parse(text);
  if (!json) {
    printf("Error before: [%s]\n", cjson_GetErrorPtr());
  }
  else {
    out = cjson_Print(json);
    cjson_Delete(json);
    printf("%s\n", out);
    free(out);
  }
}
/* Read a file, parse, render back, etc. */
static void dofile(char* filename)
{
  FILE* f;
  long len;
  char* data;
  f = fopen(filename, "rb");
  fseek(f, 0, SEEK_END);
  len = ftell(f);
  fseek(f, 0, SEEK_SET);
  data = (char*)malloc(len + 1);
  fread(data, 1, len, f);
  fclose(f);
  doit(data);
  free(data);
}
/* Used by some code below as an example datatype. */
struct record {
  const char* precision;
  double lat, lon;
  const char* address, *city, *state, *zip, *country;
};
/* Create a bunch of objects as demonstration. */
void create_objects()
{
  cjson* root, *fmt, *img, *thm, *fld;
  char* out;
  int i; /* declare a few. */
  /* Our "days of the week" array: */
  const char* strings[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  /* Our matrix: */
  int numbers[3][3] = {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}};
  /* Our "gallery" item: */
  int ids[4] = {116, 943, 234, 38793};
  /* Our array of "records": */
  struct record fields[2] = {
    {"zip", 37.7668, -1.223959e+2, "", "SAN FRANCISCO", "CA", "94107", "US"},
    {"zip", 37.371991, -1.22026e+2, "", "SUNNYVALE", "CA", "94085", "US"}
  };
  /* Here we construct some JSON standards, from the JSON site. */
  /* Our "Video" datatype: */
  root = cjson_CreateObject();
  cjson_AddItemToObject(root, "name", cjson_CreateString("Jack (\"Bee\") Nimble"));
  cjson_AddItemToObject(root, "format", fmt = cjson_CreateObject());
  cjson_AddStringToObject(fmt, "type", "rect");
  cjson_AddNumberToObject(fmt, "width", 1920);
  cjson_AddNumberToObject(fmt, "height", 1080);
  cjson_AddFalseToObject(fmt, "interlace");
  cjson_AddNumberToObject(fmt, "frame rate", 24);
  out = cjson_Print(root);
  cjson_Delete(root);
  printf("%s\n", out);
  free(out); /* Print to text, Delete the cjson, print it, release the string. */
  /* Our "days of the week" array: */
  root = cjson_CreateStringArray(strings, 7);
  out = cjson_Print(root);
  cjson_Delete(root);
  printf("%s\n", out);
  free(out);
  /* Our matrix: */
  root = cjson_CreateArray();
  for (i = 0; i < 3; i++) {
    cjson_AddItemToArray(root, cjson_CreateIntArray(numbers[i], 3));
  }
  /* cjson_ReplaceItemInArray(root,1,cjson_CreateString("Replacement")); */
  out = cjson_Print(root);
  cjson_Delete(root);
  printf("%s\n", out);
  free(out);
  /* Our "gallery" item: */
  root = cjson_CreateObject();
  cjson_AddItemToObject(root, "Image", img = cjson_CreateObject());
  cjson_AddNumberToObject(img, "Width", 800);
  cjson_AddNumberToObject(img, "Height", 600);
  cjson_AddStringToObject(img, "Title", "View from 15th Floor");
  cjson_AddItemToObject(img, "Thumbnail", thm = cjson_CreateObject());
  cjson_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
  cjson_AddNumberToObject(thm, "Height", 125);
  cjson_AddStringToObject(thm, "Width", "100");
  cjson_AddItemToObject(img, "IDs", cjson_CreateIntArray(ids, 4));
  out = cjson_Print(root);
  cjson_Delete(root);
  printf("%s\n", out);
  free(out);
  /* Our array of "records": */
  root = cjson_CreateArray();
  for (i = 0; i < 2; i++) {
    cjson_AddItemToArray(root, fld = cjson_CreateObject());
    cjson_AddStringToObject(fld, "precision", fields[i].precision);
    cjson_AddNumberToObject(fld, "Latitude", fields[i].lat);
    cjson_AddNumberToObject(fld, "Longitude", fields[i].lon);
    cjson_AddStringToObject(fld, "Address", fields[i].address);
    cjson_AddStringToObject(fld, "City", fields[i].city);
    cjson_AddStringToObject(fld, "State", fields[i].state);
    cjson_AddStringToObject(fld, "Zip", fields[i].zip);
    cjson_AddStringToObject(fld, "Country", fields[i].country);
  }
  /* cjson_ReplaceItemInObject(cjson_GetArrayItem(root,1),"City",cjson_CreateIntArray(ids,4)); */
  out = cjson_Print(root);
  cjson_Delete(root);
  printf("%s\n", out);
  free(out);
}
int test_cjson_()
{
  /* a bunch of json: */
  char text1[] = "{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\": \"rect\", \n\"width\": 1920, \n\"height\": 1080, \n\"interlace\": false,\"frame rate\": 24\n}\n}";
  char text2[] = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
  char text3[] = "[\n [0, -1, 0],\n [1, 0, 0],\n [0, 0, 1]\n ]\n";
  char text4[] = "{\n \"Image\": {\n \"Width\": 800,\n \"Height\": 600,\n \"Title\": \"View from 15th Floor\",\n \"Thumbnail\": {\n \"Url\": \"http:/*www.example.com/image/481989943\",\n \"Height\": 125,\n \"Width\": \"100\"\n },\n \"IDs\": [116, 943, 234, 38793]\n }\n }";
  char text5[] = "[\n {\n \"precision\": \"zip\",\n \"Latitude\": 37.7668,\n \"Longitude\": -122.3959,\n \"Address\": \"\",\n \"City\": \"SAN FRANCISCO\",\n \"State\": \"CA\",\n \"Zip\": \"94107\",\n \"Country\": \"US\"\n },\n {\n \"precision\": \"zip\",\n \"Latitude\": 37.371991,\n \"Longitude\": -122.026020,\n \"Address\": \"\",\n \"City\": \"SUNNYVALE\",\n \"State\": \"CA\",\n \"Zip\": \"94085\",\n \"Country\": \"US\"\n }\n ]";
  /* Process each json textblock by parsing, then rebuilding: */
  sys_chdir("E:/pub/bin/json");
  doit(text1);
  doit(text2);
  doit(text3);
  doit(text4);
  doit(text5);
  /* Parse standard testfiles: */
  /* dofile("../../tests/test1"); */
  /* dofile("../../tests/test2"); */
  /* dofile("../../tests/test3"); */
  /* dofile("../../tests/test4"); */
  /* dofile("../../tests/test5"); */
  /* Now some samplecode for building objects concisely: */
  create_objects();
  return 0;
}
#endif // TEST

static cjson *cjson_OpenFile(const char* file)
{
	FILE *f;
	int len;
	char *data;
	cjson *json;

	if (!file)  return NULL;
	f = fopen(file, "rb");
	if (!f) return NULL;
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);
	data = (char*)malloc(len + 1);
	if (!data)
	{
		fclose(f);
		return NULL;
	}
	fread(data, 1, len, f);
	fclose(f);
	json = cjson_Parse(data);
	free(data);
	return json;
}
static int cjson_SaveFile(const char* file, cjson *json)
{
	FILE *f;
	char *data;

	if (!file || !json) return -1;

	data = cjson_Print(json);
	if (!data) return -1;
	f = fopen(file, "w");
	if (!f) { free(data); return -1; }
	fseek(f, 0, SEEK_SET);
	fwrite(data, 1, strlen(data), f);
	fclose(f);
	free(data);
	return 0;
}

//////////////////////////////////////////////////////
static bool cjson_GetBool(cjson* item, bool default_bool) {
  return item ? item->type==cjson_True : default_bool;
}
static int cjson_GetInt(cjson* item, int default_int) {
  return item ? item->valueint : default_int;
}
static int cjson_GetFloat(cjson* item, double default_double) {
  return item ? item->valuedouble : default_double;
}
//////////////////////////////////////////////////////
// Object
bool cjson_HasObjectItem(cjson* object, const char* name) {
  return cjson_GetObjectItem(object, name) != NULL;
}
int cjson_GetObjectInt(cjson* object, const char* name, int default_int) {
  cjson* item = cjson_GetObjectItem(object, name);
  return item ? item->valueint : default_int;
}
double cjson_GetObjectFloat(cjson* object, const char* name, double default_double) {
  cjson* item = cjson_GetObjectItem(object, name);
  return item ? item->valuedouble : default_double;
}
bool cjson_GetObjectBool(cjson* object, const char* name, bool default_bool) {
  cjson* item = cjson_GetObjectItem(object, name);
  return cjson_GetBool(item, default_bool);
}
cjson* cjson_SetObjectItem(cjson* object, const char* name, cjson* new_item) {
  cjson* item = cjson_GetObjectItem(object, name);
  return item ? cjson_SetItem(item, new_item) : cjson_AddItemToObject(object, name, new_item);
}
cjson* cjson_AddObjectNumber(cjson* object, const char* name, double num) {
  return cjson_AddItemToObject(object, name, cjson_CreateNumber(num));
}
cjson* cjson_SetObjectNumber(cjson* object, const char* name, double num) {
  cjson* item = cjson_GetObjectItem(object, name);
  return item ? cjson_SetNumber(item, num) : cjson_AddObjectNumber(object, name, num);
}
cjson* cjson_SetObjectInt(cjson* object, const char* name, int num) {
  cjson* item = cjson_GetObjectItem(object, name);
  return item ? cjson_SetNumber(item, num) : cjson_AddIntToObject(object, name, num);
}
static cjson* cjson_AddObjectString(cjson* object, const char* name, const char* string) {
  return cjson_AddItemToObject(object, name, cjson_CreateString(string));
}
static cjson* cjson_AddObjectObject(cjson* object, const char* name) {
  return cjson_AddItemToObject(object, name, cjson_CreateObject());
}
static cjson* cjson_AddObjectArray(cjson* object, const char* name) {
  return cjson_AddItemToObject(object, name, cjson_CreateArray());
}
static cjson* cjson_SetObjectString(cjson* object, const char* name, const char* string) {
  cjson* item = cjson_GetObjectItem(object, name);
  return item ? cjson_SetString(item, string) : cjson_AddObjectString(object, name, string);
}
int cjson_GetObjectArraySize(cjson* object, const char* string) {
  cjson* item = cjson_GetObjectItem(object, string);
  return item ? cjson_GetArraySize(item) : 0;
}
static int cjson_GetEnum(const cjson* item, int default_enum, const char** enum_map, int enum_map_len) {
  if (item) {
    for (int i = 0; i < enum_map_len; ++i) {
      if (0 == strcmp(item->valuestring, enum_map[i])) {
        return i;
      }
    }
  }
  return default_enum;
}
#define CJSON_GETOBJECTENUM(object, name, default_enum, type)  (type)cjson_GetObjectEnum(object, name, default_enum, type ##_Name, countof( type ##_Name ))
static int cjson_GetObjectEnum(const cjson* object, const char* name, int default_enum, const char** enum_map, int enum_map_len) {
  if (object) {
    cjson* item = cjson_GetObjectItem((cjson*)object, name);
    return cjson_GetEnum(item, default_enum, enum_map, enum_map_len);
  }
  return default_enum;
}

//////////////////////////////////////////////////////
int cjson_GetArrayInt(cjson* array, int index, int default_int) {
  cjson* item = cjson_GetArrayItem(array, index);
  return item ? item->valueint : default_int;
}
double cjson_GetArrayFloat(cjson* array, int index, double default_double) {
  cjson* item = cjson_GetArrayItem(array, index);
  return item ? item->valuedouble : default_double;
}
static int cjson_GetFloatArray(cjson* array, double** arr) {
  int size = 0;
  if (array) {
    size = cjson_GetArraySize(array);
    MYREALLOC(*arr, size);
    for (int i = 0; i < size; ++i) {
      (*arr)[i] = cjson_GetArrayItem(array, i)->valuedouble;
    }
  }
  return size;
}
static int cjson_GetIntArray(cjson* array, int** arr) {
  int size = 0;
  if (array) {
    size = cjson_GetArraySize(array);
    MYREALLOC(*arr, size);
    for (int i = 0; i < size; ++i) {
      (*arr)[i] = cjson_GetArrayItem(array, i)->valueint;
    }
  }
  return size;
}
static int cjson_GetStringRefArray(cjson* array, char*** arr) {
  int size = 0;
  if (array) {
    size = cjson_GetArraySize(array);
    MYREALLOC(*arr, size);
    for (int i = 0; i < size; ++i) {
      (*arr)[i] = cjson_GetArrayItem(array, i)->valuestring;
    }
  }
  return size;
}

#endif // _CJSON_INL_
