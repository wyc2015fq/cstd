void V32Clear(void* p, const unsigned long v, const long n)
{
  unsigned long* pv = (unsigned long*)p;
  unsigned long* pe = pv + n;
  for (; pv != pe;) {
    *pv++ = v;
  }
}
void V32Clear32(void* p, const unsigned long v, const long n)
{
  unsigned long* pv = (unsigned long*)p;
  unsigned long* pe = pv + n;
  for (; pv != pe;) {
    *pv++ = v;
  }
}

