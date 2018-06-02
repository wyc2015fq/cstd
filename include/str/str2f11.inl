#ifndef _STR2F_INL_
#define _STR2F_INL_
// 字符串转换成浮点数
// 写一个转换函数，该函数的输入是一个表示浮点数的字符串，把该字符串转换成浮点数并输出。
// 条件：请考虑各种情况，并且代码中的循环尽量少，不能调用API或者crt库中的函数。
// 例如：输入字符串"345.7"，则输出浮点数345.7。
float str2f(char* pstrfloat)
{
  BOOL bNegative = FALSE;
  BOOL bDec = FALSE;
  char* pSor = 0;
  char chByte = '0';
  float fInteger = 0.0;
  float fDecimal = 0.0;
  float fDecPower = 0.1f;
  // check
  if (!pstrfloat) {
    return 0.0;
  }
  // 进行首位判断，判断是否是负数
  if (pstrfloat[0] == '-') {
    bNegative = TRUE;
    pSor = pstrfloat + 1;
  }
  else {
    bNegative = FALSE;
    pSor = pstrfloat;
  }
  while (*pSor != '\0') {
    chByte = *pSor;
    if (bDec) {
      // 小数
      if (chByte >= '0' && chByte <= '9') {
        fDecimal += (chByte - '0') * fDecPower;
        fDecPower = fDecPower * 0.1f;
      }
      else {
        return (bNegative ? -(fInteger + fDecimal) : fInteger + fDecimal);
      }
    }
    else {
      // 整数
      if (chByte >= '0' && chByte <= '9') {
        fInteger = fInteger * 10.0f + chByte - '0';
      }
      else if (chByte == '.') {
        bDec = true;
      }
      else {
        return (bNegative ? -fInteger : fInteger);
      }
    }
    pSor++;
  }
  return (bNegative ? -(fInteger + fDecimal) : fInteger + fDecimal);
}
#endif // _STR2F_INL_

