// Encry.cpp: implementation of the CEncry class.
//
//////////////////////////////////////////////////////////////////////
#define MIN_ASC 32
#define MAX_ASC 126
#define NUM_ASC (MAX_ASC-MIN_ASC+1)
static int Encry_NumericPassword(const char* passWord)
{
  int value = 0;
  int ch = 0;
  int shift1 = 0;
  int shift2 = 0;
  int i;
  int slen, num1, num2;
  slen = strlen(passWord);
  for (i = 0; i < slen; i++) {
    //' Add the next letter.
    ch = passWord[i]; // Asc(Mid$(passWord, i, 1))
    num1 = (int)(ch * pow(2.0, shift1));
    num2 = (int)(ch * pow(2.0, shift2));
    value = value ^ num1;
    value = value ^ num2;
    //' Change the shift offsets.
    shift1 = (shift1 + 7) % 19;
    shift2 = (shift2 + 13) % 23;
  }
  return value;
}
static int Encry_EncryStr(const char* passWord, const char* from_text, char* to_text, int maxoutlen)
{
  int offset;
  int slen;
  int i ;
  int ch;
  char newchar;
  int outlen = 0;
  //' Initialize the random number generator.
  offset = Encry_NumericPassword(passWord);
  srand(offset);
  rand();
  slen = strlen(from_text);
  for (i = 0; i < slen; i++) {
    ch = from_text[i];
    if (ch >= MIN_ASC && ch <= MAX_ASC) {
      ch = ch - MIN_ASC;
      offset = (int)((NUM_ASC + 1) * rand());
      ch = ((ch + offset) % NUM_ASC);
      ch = ch + MIN_ASC;
      newchar = ch;
      if (outlen >= maxoutlen) {
        return -1;
      }
      to_text[outlen++] = newchar;
    }
  }
  to_text[outlen] = 0;
  return outlen;
}
static int Encry_UncryStr(const char* passWord, const char* from_text, char* to_text, int maxoutlen)
{
  int offset;
  int slen;
  int i;
  int ch;
  int outlen = 0;
  // Initialize the random number generator.
  offset = Encry_NumericPassword(passWord);
  srand(offset);
  rand();
  // Encipher the string.
  slen = strlen(from_text);
  for (i = 0 ; i < slen; i++) {
    ch = from_text[i];
    if (ch >= MIN_ASC && ch <= MAX_ASC) {
      ch = ch - MIN_ASC;
      offset = (int)((NUM_ASC + 1) * rand());
      ch = ((ch - offset) % NUM_ASC);
      if (ch < 0) {
        ch = ch + NUM_ASC;
        ch = ch + MIN_ASC;
        if (outlen >= maxoutlen) {
          return -1;
        }
        to_text[outlen++] = (char)(ch);
      }
    }
  }
  to_text[outlen] = 0;
  return outlen;
}
#undef NUM_ASC
#undef MAX_ASC
#undef MIN_ASC

