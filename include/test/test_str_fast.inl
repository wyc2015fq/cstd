
char *substring(char *string,unsigned int start, ...){
  va_list args;
  int unsigned i=0;
  long end=-1;
  long length=strlen(string);
  char *to_string;
  va_start(args,start);
  end=va_arg(args,int);
  va_end(args);
  if(end==-1){
    end=length;
  }
  to_string = (char*)malloc(end+1);
  strncpy(to_string,string+start,end);
  to_string[end] = 0;
  return to_string;
}

void fast_to_upper(const char *src, char *dest, size_t len) {
  size_t i = 0;
  const uint64 *src_data = (const uint64*) src;
  uint64 *dest_data = (uint64*) dest;
  size_t blocks = len / sizeof(uint64);
  uint64 src8, a, b;
  unsigned char src_char, a_char, b_char;
  
  for (; i < blocks; i++) {
    src8 = src_data[i] & 0x7f7f7f7f7f7f7f7f;
    a = src8 + 0x0505050505050505;
    b = src8 + 0x7f7f7f7f7f7f7f7f;
    a = a & b;
    a = a & (a >> 1) & 0x2020202020202020;
    dest_data[i] = src8 - a;
  }
  i *= sizeof(uint64);
  for (; i < len; i++) {
    src_char = src[i] & 0x7f;
    a_char = src_char + 0x05;
    b_char = src_char + 0x7f;
    a_char = a_char & b_char;
    a_char = a_char & (a_char >> 1) & 0x20;
    dest[i] = src_char - a_char;
  }
  dest[i] = 0;
}

int test_str_fast() {
  char* bb = substring("asdfasdf", 2);
  char* aa = substring("asdfasdf", 2, 2);
  char buf[256];
  fast_to_upper("adfadf", buf, 256);
  return 0;
}