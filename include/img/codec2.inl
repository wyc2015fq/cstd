#define TOHEX(x) ((x)>9 ? (x)+55 : (x)+48)
int is_little_endian(void)
{
  unsigned int tmp = 0x4567;
  char* ch = (char*)&tmp;

  if (ch[0] == (char)0x67) {
    printf("little_endian/n");
    return 1;  //小端模式时最小字节放在最低的地址处，即第一个地址处是最小的。
  }
  else {
    printf("big_endian/n");
    return 0;
  }
}
void URLEncode(char* szIn, char** pOut)
{
  int nInLenth = strlen(szIn);
  int i, nFlag = 0;
  BYTE byte;
  char* szOut;
  *pOut = (char*)pmalloc(nInLenth * 3);
  szOut = *pOut;

  for (i = 0; i < nInLenth; i++) {
    byte = szIn[i];

    if (isalnum(byte)) {
      szOut[nFlag++] = byte;
    }
    else {
      if (isspace(byte)) {
        szOut[nFlag++] = '+';
      }
      else {
        szOut[nFlag++] = '%';
        szOut[nFlag++] = TOHEX(byte >> 4);
        szOut[nFlag++] = TOHEX(byte % 16);
      }
    }
  }

  szOut[nFlag] = '\0';
}