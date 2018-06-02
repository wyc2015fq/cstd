
typedef struct
{
  const char* m_path;
  double m_last_number;
  char   m_last_command;
} path_tokenizer;


CC_INLINE void path_tokenizer_init_char_mask(char* mask, const char* char_set)
{
  memset(mask, 0, 256/8);
  while(*char_set) 
  {
    unsigned c = (unsigned)(*char_set++) & 0xFF;
    mask[c >> 3] |= 1 << (c & 7);
  }
}


CC_INLINE bool path_tokenizer_contains(const char* mask, unsigned c)
{
  return (mask[(c >> 3) & (256/8-1)] & (1 << (c & 7))) != 0;
}

CC_INLINE bool path_tokenizer_is_command(path_tokenizer* tok, unsigned c)
{
  static int inited=0;
  static char m_commands_mask[256/8];
  if (!inited) {
    path_tokenizer_init_char_mask(m_commands_mask,   "+-MmZzLlHhVvCcSsQqTtAaFfPp");
    inited = 1;
  }
  return path_tokenizer_contains(m_commands_mask, c);
}

CC_INLINE bool path_tokenizer_is_numeric(path_tokenizer* tok, unsigned c)
{
  static int inited=0;
  static char m_numeric_mask[256/8];
  if (!inited) {
    path_tokenizer_init_char_mask(m_numeric_mask, ".Ee0123456789");
    inited = 1;
  }
  return path_tokenizer_contains(m_numeric_mask, c);
}

CC_INLINE bool path_tokenizer_is_separator(path_tokenizer* tok, unsigned c)
{
  static int inited=0;
  static char m_separators_mask[256/8];
  if (!inited) {
    path_tokenizer_init_char_mask(m_separators_mask, " ,\t\n\r");
    inited = 1;
  }
  return path_tokenizer_contains(m_separators_mask, c);
}


CC_INLINE void path_tokenizer_set_path_str(path_tokenizer* tok, const char* str)
{
  tok->m_path = str;
  tok->m_last_command = 0;
  tok->m_last_number = 0.0;
}


CC_INLINE bool path_tokenizer_parse_number(path_tokenizer* tok)
{
  char buf[256]; // Should be enough for any number
  char* buf_ptr = buf;
  
  // Copy all sign characters
  while(buf_ptr < buf+255 && *tok->m_path == '-' || *tok->m_path == '+')
  {
    *buf_ptr++ = *tok->m_path++;
  }
  
  // Copy all numeric characters
  while(buf_ptr < buf+255 && path_tokenizer_is_numeric(tok, *tok->m_path))
  {
    *buf_ptr++ = *tok->m_path++;
  }
  *buf_ptr = 0;
  tok->m_last_number = atof(buf);
  return true;
}

CC_INLINE bool path_tokenizer_next_impl(path_tokenizer* tok)
{
  if(tok->m_path == 0) return false;
  
  // Skip all white spaces and other garbage
  while(*tok->m_path && !path_tokenizer_is_command(tok, *tok->m_path) && !path_tokenizer_is_numeric(tok, *tok->m_path)) 
  {
    if(!path_tokenizer_is_separator(tok, *tok->m_path))
    {
      char buf[100];
      sprintf(buf, "next : Invalid Character %c", *tok->m_path);
      return false;
    }
    tok->m_path++;
  }
  
  if(*tok->m_path == 0) return false;
  
  if(path_tokenizer_is_command(tok, *tok->m_path))
  {
    // Check if the command is a numeric sign character
    if(*tok->m_path == '-' || *tok->m_path == '+')
    {
      return path_tokenizer_parse_number(tok);
    }
    tok->m_last_command = *tok->m_path++;
    while(*tok->m_path && path_tokenizer_is_separator(tok, *tok->m_path)) tok->m_path++;
    if(*tok->m_path == 0) return true;
  }
  return path_tokenizer_parse_number(tok);
}

CC_INLINE bool path_tokenizer_next(path_tokenizer* tok, char cmd, double* pnumber)
{
  if(!path_tokenizer_next_impl(tok)) return false;//("parse_path: Unexpected end of path");
  if(tok->m_last_command != cmd) {
    char buf[100];
    sprintf(buf, "parse_path: Command %c: bad or missing parameters", cmd);
    return false;
  }
  *pnumber = tok->m_last_number;
  return true;
}


