

#include "string.inl"
enum codepage
{
  cp_gb, cp_utf8, cp_utf16, cp_utf32
};

#define STD_CODECVT_NOT_SUPPORTED

//GCC 4.7.0 does not implement the <codecvt> and codecvt_utfx classes
#ifndef STD_CODECVT_NOT_SUPPORTED
#include <codecvt>
#endif

#if defined(NANA_WINDOWS)
#include <windows.h>
#endif

/// return a pointer to the code unit of the character at pos
static const char* char_ptr(const char* text, unsigned pos)
{
  const unsigned char* ustr = (const unsigned char*)(text);
  const unsigned char* const end = ustr + strlen(text);
  
  for (unsigned i = 0; i != pos; ++i)
  {
    const auto uch = *ustr;
    if (uch < 0x80)
    {
      ++ustr;
      continue;
    }
    
    if (uch < 0xC0)        // use police ?
      return NULL;
    
    if ((uch < 0xE0) && (ustr + 1 < end)) //? *(ustr + 1) < 0xE0 
      ustr += 2;
    else if (uch < 0xF0 && (ustr + 2 <= end))
      ustr += 3;
    else if (uch < 0x1F && (ustr + 3 <= end))
      ustr += 4;
    else
      return NULL;
  }
  
  return (const char*)(ustr);
}

/// return a code point (max 16 bits?) and the len in code units of the character at pos
static wchar_t char_at(const char* text_utf8, unsigned pos, unsigned * len)
{
  if (!text_utf8)
    return 0;
  
  if (pos)
  {
    text_utf8 = char_ptr(text_utf8, pos);
    if (!text_utf8)
      return 0;
  }
  
  const wchar_t uch = *reinterpret_cast<const unsigned char*>(text_utf8);
  if (uch < 0x80)
  {
    if (len)
      *len = 1;
    
    return *text_utf8;  // uch ?
  }
  
  if (uch < 0xC0)    // use police or ??
  {
    if (len)
      *len = 0;
    
    return 0;
  }
  
  const char* end = text_utf8 + strlen(text_utf8);
  
  if (uch < 0xE0 && (text_utf8 + 1 <= end))
  {
    if (len)
      *len = 2;
    return (wchar_t(uch & 0x1F) << 6) | (reinterpret_cast<const unsigned char*>(text_utf8)[1] & 0x3F);
  }
  else if (uch < 0xF0 && (text_utf8 + 2 <= end))
  {
    if (len)
      *len = 3;
    
    return ((((uch & 0xF) << 6) | (reinterpret_cast<const unsigned char*>(text_utf8)[1] & 0x3F)) << 6) | (reinterpret_cast<const unsigned char*>(text_utf8)[2] & 0x3F);
  }
  else if (uch < 0x1F && (text_utf8 + 3 <= end))
  {
    if (len)
      *len = 4;
    return ((((((uch & 0x7) << 6) | (reinterpret_cast<const unsigned char*>(text_utf8)[1] & 0x3F)) << 6) | (reinterpret_cast<const unsigned char*>(text_utf8)[2] & 0x3F)) << 6) | (reinterpret_cast<const unsigned char*>(text_utf8)[3] & 0x3F);
  }
  
  if (len)
    *len = 0;
  
  return 0;
}

/// candidate to be more general??
#if !defined(NANA_WINDOWS)
class locale_initializer
{
public:
  static void init()
  {
    static bool initialized = false;
    if (initialized) return;
    
    initialized = true;
    //Only set the C library locale
    setlocale(LC_CTYPE, "");
  }
};
#endif

/// convert wchar C string from ? ANSI code page CP_ACP (windows) or LC_CTYPE c locale (-nix) into cp_utf8 string
static char* utf16_to_gb(char*& dst_, int& pos, const char* s)
{
  const wchar* wcstr = (const wchar*)s;
#if defined(NANA_WINDOWS)
  int bytes = ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, 0, 0, 0, 0);
  if(bytes > 1)
  {
    MYREALLOC(dst_, pos + bytes - 1);
    ::WideCharToMultiByte(CP_ACP, 0, wcstr, -1, (dst_+pos), bytes - 1, 0, 0);
  }
  return dst_;
#else
  locale_initializer::init();
  mbstate_t mbstate = mbstate_t();
  size_t len = wcsrtombs(NULL, &wcstr, 0, &mbstate);
  if(len == static_cast<size_t>(-1))
    return false;
  
  if(len)
  {
    MYREALLOC(dst_, pos+len);
    wcsrtombs(dst_+pos, &s, len, &mbstate);
  }
#endif
  return dst_;
}

/// convert a char C-string from The system default Windows ANSI code page CP_ACP or from LC_CTYPE c locale (-nix) into cp_utf16 wstring
static char* gb_to_utf16(char*& dst_, int& pos, const char* s)
{
#if defined(NANA_WINDOWS)
  int chars = ::MultiByteToWideChar(CP_ACP, 0, s, -1, 0, 0);
  if(chars > 1)
  {
    MYREALLOC(dst_, pos + chars - 1);
    ::MultiByteToWideChar(CP_ACP, 0, s, -1, (wchar*)(dst_+pos), chars - 1);
  }
#else
  locale_initializer::init();
  mbstate_t mbstate = mbstate_t();
  size_t len = mbsrtowcs(NULL, &s, 0, &mbstate);
  if(len == static_cast<size_t>(-1))
    return false;
  
  if(len)
  {
    MYREALLOC(wcstr, pos+len);
    mbsrtowcs(wcstr, &s, len, &mbstate);
  }
  else {
    wstr_clear(wcstr);
  }
#endif
  return dst_;
}

/// playing with the idea - we need a mechanisme to set a user selected police - Testing an abtract interphase
enum encoding_error_police
{
  /// the current nana default: it is safe - you may want to keep it ! use the other at your risk: mainly for debugging
  utf8_error_police,
    utf8_error_police_def_char,
    utf8_error_police_throw,
    utf8_error_police_latin,
    utf8_error_police_system,
};

static unsigned long next_code_point(const unsigned char*& current_code_unit, const unsigned char* end, enum encoding_error_police err)
{
  switch(err) {
  case utf8_error_police:
    current_code_unit = end;
    return 0;
  case utf8_error_police_def_char: 
    if(current_code_unit < end)
      ++current_code_unit;
    return 0;
  case utf8_error_police_throw: 
    current_code_unit = end;
    return 0;
  case utf8_error_police_latin: 
    return *(current_code_unit++);
  case utf8_error_police_system: 
    {
      char* wc = NULL;
      int ret=0, pos=0;
      gb_to_utf16(wc, pos, reinterpret_cast<const char*>(current_code_unit));
      current_code_unit++;
      ret = ((wchar*)wc)[0];      // use utf16char but what endian?
      FREE(wc);
      return ret;
    }
  default:
    return 0;
  }
  return 0;
}

static encoding_error_police def_encoding_error_police = utf8_error_police_system;

/// return the first code point and move the pointer to next character, springing to the end by errors
static unsigned long utf8char(const unsigned char*& p, const unsigned char* end)
{
  if(p != end)
  {
    if(*p < 0x80)        // ASCII char   0-127 or 0-0x80
    {
      return *(p++);
    }
    unsigned ch = *p;
    unsigned long code;
    if(ch < 0xC0)       // error? - move to end. Posible ANSI or ISO code-page 
    {
      //return *(p++); // temp: assume equal
      //p = end;
      //return 0;
      return next_code_point(p, end, def_encoding_error_police);
    }
    else if(ch < 0xE0 && (p + 1 <= end))      // two byte chararcter
    {
      code = ((ch & 0x1F) << 6) | (p[1] & 0x3F);
      p += 2;
    }
    else if(ch < 0xF0 && (p + 2 <= end))     // 3 byte character
    {
      code = ((((ch & 0xF) << 6) | (p[1] & 0x3F)) << 6) | (p[2] & 0x3F);
      p += 3;
    }
    else if(ch < 0x1F && (p + 3 <= end))   // 4 byte character
    {
      code = ((((((ch & 0x7) << 6) | (p[1] & 0x3F)) << 6) | (p[2] & 0x3F)) << 6) | (p[3] & 0x3F);
      p += 4;
    }
    else    //  error, go to end
    {
      p = end;
      return 0;
    }
    return code;
  }
  return 0;
}
static
unsigned long utf16char(const unsigned char* & bytes, const unsigned char* end, bool le_or_be)
{
  unsigned long code;
  if(le_or_be)
  {
    if((end - bytes >= 4) && ((bytes[1] & 0xFC) == 0xD8))
    {
      //32bit encoding
      unsigned long ch0 = bytes[0] | (bytes[1] << 8);
      unsigned long ch1 = bytes[2] | (bytes[3] << 8);
      
      code = ((ch0 & 0x3FF) << 10) | (ch1 & 0x3FF);
      bytes += 4;
    }
    else if(end - bytes >= 2)
    {
      code = bytes[0] | (bytes[1] << 8);
      bytes += 2;
    }
    else
    {
      bytes = end;
      return 0;
    }
  }
  else
  {
    if((end - bytes >= 4) && ((bytes[0] & 0xFC) == 0xD8))
    {
      //32bit encoding
      unsigned long ch0 = (bytes[0] << 8) | bytes[1];
      unsigned long ch1 = (bytes[2] << 8) | bytes[3];
      code = (((ch0 & 0x3FF) << 10) | (ch1 & 0x3FF)) + 0x10000;
      bytes += 4;
    }
    else if(end - bytes >= 2)
    {
      code = (bytes[0] << 8) | bytes[1];
      bytes += 2;
    }
    else
    {
      bytes = end;
      return 0;
    }
  }
  return code;
}
static
unsigned long utf32char(const unsigned char* & bytes, const unsigned char* end, bool le_or_be)
{
  if(end - bytes >= 4)
  {
    unsigned long code;
    if(le_or_be)
      code = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
    else
      code = bytes[3] | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);
    bytes += 4;
    return code;
  }
  bytes = end;
  return 0;
}
static
char* put_utf8char(char* s, int& pos, unsigned long code)
{
  if(code < 0x80)
  {
    s[pos++] = static_cast<char>(code);
  }
  else if(code < 0x800)
  {
    s[pos++] = static_cast<char>(0xC0 | (code >> 6));
    s[pos++] = static_cast<char>(0x80 | (code & 0x3F));
  }
  else if(code < 0x10000)
  {
    s[pos++] = static_cast<char>(0xE0 | (code >> 12));
    s[pos++] = static_cast<char>(0x80 | ((code >> 6) & 0x3F));
    s[pos++] = static_cast<char>(0x80 | (code & 0x3F));
  }
  else
  {
    s[pos++] = static_cast<char>(0xF0 | (code >> 18));
    s[pos++] = static_cast<char>(0x80 | ((code >> 12) & 0x3F));
    s[pos++] = static_cast<char>(0x80 | ((code >> 6) & 0x3F));
    s[pos++] = static_cast<char>(0x80 | (code & 0x3F));
  }
  return s;
}

//le_or_be, true = le, false = be
static char* put_utf16char(char* s, int& pos, unsigned long code, bool le_or_be)
{
  if(code <= 0xFFFF)
  {
    if(le_or_be)
    {
      s[pos++] = static_cast<char>(code & 0xFF);
      s[pos++] = static_cast<char>((code & 0xFF00) >> 8);
    }
    else
    {
      s[pos++] = static_cast<char>((code & 0xFF00) >> 8);
      s[pos++] = static_cast<char>(code & 0xFF);
    }
  }
  else
  {
    unsigned long ch0 = (0xD800 | ((code - 0x10000) >> 10));
    unsigned long ch1 = (0xDC00 | ((code - 0x10000) & 0x3FF));
    
    if(le_or_be)
    {
      s[pos++] = static_cast<char>(ch0 & 0xFF);
      s[pos++] = static_cast<char>((ch0 & 0xFF00) >> 8);
      
      s[pos++] = static_cast<char>(ch1 & 0xFF);
      s[pos++] = static_cast<char>((ch1 & 0xFF00) >> 8);
    }
    else
    {
      s[pos++] = static_cast<char>((ch0 & 0xFF00) >> 8);
      s[pos++] = static_cast<char>(ch0 & 0xFF);
      
      s[pos++] = static_cast<char>((ch1 & 0xFF00) >> 8);
      s[pos++] = static_cast<char>(ch1 & 0xFF);
    }
  }
  return s;
}
static char* put_utf32char(char* s, int& pos, unsigned long code, bool le_or_be)
{
  if(le_or_be)
  {
    s[pos++] = static_cast<char>(code & 0xFF);
    s[pos++] = static_cast<char>((code & 0xFF00) >> 8);
    s[pos++] = static_cast<char>((code & 0xFF0000) >> 16);
    s[pos++] = static_cast<char>((code & 0xFF000000) >> 24);
  }
  else
  {
    s[pos++] = static_cast<char>((code & 0xFF000000) >> 24);
    s[pos++] = static_cast<char>((code & 0xFF0000) >> 16);
    s[pos++] = static_cast<char>((code & 0xFF00) >> 8);
    s[pos++] = static_cast<char>(code & 0xFF);
  }
  return s;
}
static char* utf8_to_utf16(char*& str, int& pos, const char* s)
{
  bool le_or_be = true;
  int ssize = strlen(s);
  const unsigned char * bytes = reinterpret_cast<const unsigned char*>(s);
  const unsigned char * end = bytes + ssize;
  int len = pos + 3*ssize;
  //If there is a BOM, ignore it.
  MYREALLOC(str, len+4);
  if(ssize >= 3)
  {
    if(bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
    {
      bytes += 3;
      put_utf16char(str, pos, 0xFEFF, le_or_be);
    }
  }
  
  while(bytes != end)
  {
    put_utf16char(str, pos, utf8char(bytes, end), le_or_be);
  }
  ASSERT(pos < len);
  str[pos] = 0;
  return str;
}
static char* utf8_to_utf32(char*& str, int& pos, const char* s)
{
  bool le_or_be = true;
  int ssize = strlen(s);
  const unsigned char * bytes = reinterpret_cast<const unsigned char*>(s);
  const unsigned char * end = bytes + ssize;
  int len = pos + 4*ssize;
  MYREALLOC(str, len+4);
  //If there is a BOM, ignore it.
  if(ssize >= 3)
  {
    if(bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
    {
      bytes += 3;
      put_utf32char(str, pos, 0xFEFF, le_or_be);
    }
  }
  
  while(bytes != end)
  {
    put_utf32char(str, pos, utf8char(bytes, end), le_or_be);
  }
  ASSERT(pos < len);
  str[pos] = 0;
  return str;
}
static
char* utf16_to_utf8(char*& str, int& pos, const char* s)
{
  int ssize = strlen(s);
  const unsigned char * bytes = reinterpret_cast<const unsigned char*>(s);
  const unsigned char * end = bytes + ssize;
  bool le_or_be = true;
  int len = pos+4*ssize;
  MYREALLOC(str, len+4);
  //If there is a BOM, ignore it
  if(ssize >= 2)
  {
    if(bytes[0] == 0xFF && bytes[1] == 0xFE)
    {
      bytes += 2;
      le_or_be = true;
      
      str[pos++] = (char)0xEF;
      str[pos++] = (char)0xBB;
      str[pos++] = (char)0xBF;
    }
    else if(bytes[0] == 0xFE && bytes[1] == 0xFF)
    {
      bytes += 2;
      le_or_be = false;
      str[pos++] = (char)(0xEF);
      str[pos++] = (char)(0xBB);
      str[pos++] = (char)(0xBF);
    }
  }
  
  while(bytes != end)
  {
    put_utf8char(str, pos, utf16char(bytes, end, le_or_be));
  }
  ASSERT(pos < len);
  str[pos] = 0;
  return str;
}
static
char* utf16_to_utf32(char*& str, int& pos, const char* s)
{
  int ssize = strlen(s);
  const unsigned char * bytes = reinterpret_cast<const unsigned char*>(s);
  const unsigned char * end = bytes + ssize;
  bool le_or_be = true;
  int len = 4*ssize;
  MYREALLOC(str, len+4);
  
  //If there is a BOM, ignore it
  if(ssize >= 2)
  {
    if(bytes[0] == 0xFF && bytes[1] == 0xFE)
    {
      bytes += 2;
      le_or_be = true;
      put_utf32char(str, pos, 0xFEFF, true);
    }
    else if(bytes[0] == 0xFE && bytes[1] == 0xFF)
    {
      bytes += 2;
      le_or_be = false;
      put_utf32char(str, pos, 0xFEFF, false);
    }
  }
  
  while(bytes != end)
  {
    put_utf32char(str, pos, utf16char(bytes, end, le_or_be), le_or_be);
  }
  ASSERT(pos < len);
  str[pos] = 0;
  return str;
}
static
char* utf32_to_utf8(char*& str, int& pos, const char* s)
{
  int ssize = strlen(s);
  const unsigned char * bytes = reinterpret_cast<const unsigned char*>(s);
  const unsigned char * end = bytes + (ssize & (~4 + 1));
  int len = pos + 4*ssize;
  MYREALLOC(str, len+4);
  
  
  bool le_or_be = true;
  //If there is a BOM, ignore it
  if(ssize >= 4)
  {
    if(bytes[0] == 0 && bytes[1] == 0 && bytes[2] == 0xFE && bytes[3] == 0xFF)
    {
      le_or_be = false;
      bytes += 4;
      str[pos++] = (char)0xEF;
      str[pos++] = (char)0xBB;
      str[pos++] = (char)0xBF;
    }
    else if(bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0 && bytes[3] == 0)
    {
      le_or_be = true;
      bytes += 4;
      str[pos++] = (char)0xEF;
      str[pos++] = (char)0xBB;
      str[pos++] = (char)0xBF;
    }
  }
  
  while(bytes < end)
  {
    put_utf8char(str, pos, utf32char(bytes, end, le_or_be));
  }
  ASSERT(pos < len);
  str[pos] = 0;
  return str;
}
static char* utf32_to_utf16(char*& dst_, int& pos, const char* s)
{
  int ssize = strlen(s);
  const unsigned char * bytes = reinterpret_cast<const unsigned char*>(s);
  const unsigned char * end = bytes + (ssize & (~4 + 1));
  bool le_or_be = true;
  int len = pos + 4*ssize;
  MYREALLOC(dst_, len+4);
  //If there is a BOM, ignore it
  if(ssize >= 4)
  {
    if(bytes[0] == 0 && bytes[1] == 0 && bytes[2] == 0xFE && bytes[3] == 0xFF)
    {
      le_or_be = false;
      bytes += 4;
      put_utf16char(dst_, pos, 0xFEFF, false);
    }
    else if(bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0 && bytes[3] == 0)
    {
      le_or_be = true;
      bytes += 4;
      put_utf16char(dst_, pos, 0xFEFF, true);
    }
  }
  
  while(bytes < end)
  {
    put_utf16char(dst_, pos, utf32char(bytes, end, le_or_be), le_or_be);
  }
  ASSERT(pos < len);
  dst_[pos] = 0;
  return dst_;
}
static char* gb_to_utf8(char*& dst_, int& pos, const char* src_) {
  char* wcstr = NULL;
  int pos1 = 0;
  utf16_to_utf8(dst_, pos, gb_to_utf16(wcstr, pos1, src_));
  FREE(wcstr);
  return dst_;
}
static char* gb_to_utf32(char*& dst_, int& pos, const char* src_) {
  char* wcstr = NULL;
  int pos1 = 0;
  utf16_to_utf32(dst_, pos, gb_to_utf16(wcstr, pos1, src_));
  FREE(wcstr);
  return dst_;
}
static char* utf32_to_gb(char*& dst_, int& pos, const char* src_) {
  char* wcstr = NULL;
  int pos1 = 0;
  utf16_to_utf32(dst_, pos, utf32_to_utf16(wcstr, pos1, src_));
  FREE(wcstr);
  return dst_;
}
static char* utf8_to_gb(char*& dst_, int& pos, const char* src_) {
  char* wcstr = NULL;
  int pos1 = 0;
  utf16_to_utf32(dst_, pos, utf8_to_utf16(wcstr, pos1, src_));
  FREE(wcstr);
  return dst_;
}
static char* codecvt(char*& dst_, const char* src_, codepage src_cp, codepage encoding)
{
  int pos = 0;
  if (src_cp != encoding) {
    astr_cpy(dst_, pos, src_);
    return dst_;
  }

#define CODECVTCASE(a, b)  case ((cp_ ## a)<<8)|(cp_ ## b): a ## _to_ ## b (dst_, pos, src_)
  switch(src_cp)
  {
    CODECVTCASE(gb, utf8);
    CODECVTCASE(gb, utf16);
    CODECVTCASE(gb, utf32);
    CODECVTCASE(utf8, gb);
    CODECVTCASE(utf8, utf16);
    CODECVTCASE(utf8, utf32);
    CODECVTCASE(utf16, gb);
    CODECVTCASE(utf16, utf8);
    CODECVTCASE(utf16, utf32);
    CODECVTCASE(utf32, gb);
    CODECVTCASE(utf32, utf8);
    CODECVTCASE(utf32, utf16);
  default:
    ASSERT(0);
    break;
  }
#undef CODECVTCASE
  return dst_;
}

bool is_utf8(const char* str, size_t len)
{
	const unsigned char* ustr = (const unsigned char*)(str);
  const unsigned char* end = ustr + len;
  
  while (ustr < end)
  {
    const auto uv = *ustr;
    if (uv < 0x80)
    {
      ++ustr;
      continue;
    }
    
    if (uv < 0xC0)
      return false;
    
    if ((uv < 0xE0) && (end - ustr > 1))
      ustr += 2;
    else if ((uv < 0xF0) && (end - ustr > 2))
      ustr += 3;
    else if ((uv < 0x1F) && (end - ustr > 3))
      ustr += 4;
    else
      return false;
  }
  return true;
}



//end class charset


