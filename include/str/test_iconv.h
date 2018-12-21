// charsetconv.cpp : Defines the entry point for the console application.
//

#include <iconv.h>

#ifndef ICONV_CONST
# define ICONV_CONST const
#endif

int convert(const char *from, const char *to, char* save, int savelen, char *src, int srclen)
{
  iconv_t cd;
  char   *inbuf = src;
  char *outbuf = save;
  size_t outbufsize = savelen;
  int status = 0;
  size_t  savesize = 0;
  size_t inbufsize = srclen;
  const char* inptr = inbuf;
  size_t      insize = inbufsize;
  char* outptr = outbuf;
  size_t outsize = outbufsize;
  cd = iconv_open(to, from);
  iconv(cd, NULL, NULL, NULL, NULL);
  if (inbufsize == 0) {
    status = -1;
    goto done;
  }

  while (insize > 0) {
    size_t res = iconv(cd, (ICONV_CONST char**)&inptr, &insize, &outptr, &outsize);
    if (outptr != outbuf) {
      int saved_errno = errno;
      int outsize = outptr - outbuf;
      strncpy(save + savesize, outbuf, outsize);
      errno = saved_errno;
    }
    if (res == (size_t)(-1)) {
      if (errno == EILSEQ) {
        int one = 1;
        iconvctl(cd, ICONV_SET_DISCARD_ILSEQ, &one);
        status = -3;
      }
      else if (errno == EINVAL) {
        if (inbufsize == 0) {
          status = -4;
          goto done;
        }
        else {
          break;
        }
      }
      else if (errno == E2BIG) {
        status = -5;
        goto done;
      }
      else {
        status = -6;
        goto done;
      }
    }
  }
  status = strlen(save);
done:
  iconv_close(cd);
  return status;
}

int test_iconv(int argc, _TCHAR* argv[])
{
  std::ifstream fs("c://1.txt");
  if (!fs.fail())
  {
    std::istreambuf_iterator<char> beg(fs), end;
    std::vector<char> buf(beg, end);
    std::string content(buf.begin(), buf.end());
    std::vector<unsigned char> tobuf(2 * content.length() + 16);
    //std::vector<unsigned char> tobuf2;
    std::size_t tolen = convert("UTF-8//IGNORE", "gb2312", (char *)&tobuf[0], tobuf.size(), (char *)content.c_str(), content.length());
    if (tolen < 0)
    {
      std::cerr << "Error/n";
    }
    else {

      tobuf.resize(tolen);
      std::cout << std::string(tobuf.begin(), tobuf.end()) << std::endl;
    }
    //tlconv(tobuf2, (const unsigned char *)&tobuf[0], tobuf.size());
  }
  return 0;
}
