#undef _snprintf
#undef strcpy
//#include "debug.h"
#include "httpget.inl"
#include "str/test_printf.inl"
//#include "http.inl"
int test_httpget()
{
  const char* url = 0;
  url = "http://blog.sina.com.cn/s/blog_80b4f5d10100sshv.html";
  url = "http://www.baidu.com/link?url=8AFhAcRHkf4hmIP-RTcTRh3d35V0wSDLTwhAmtNUK5v94PyzJgiz3RuZj4LlxRGPfTY-pUxUpBpddYquLjLPfpvBhoUlYvdUQH-Im9XnJnG";
  url = "http://pic64.nipic.com/file/20150422/11284670_094621781000_2.jpg";
  url = "http://aadyw.cc/Column/listlunli.html";
  url = "http://downa.dypics.com/vod/a/042815add863.rmvb";
  url = "http://tu.arting365.com/view/1133697";
  url = "http://dx124.downyouxi.com/chujifeilong.exe";
  url = "http://img.7160.com/uploads/allimg/150506/9-150506150112.jpg";
  //test_printf();
  httpgetui();
  //httpget(url, 80, "D:\\pub\\www", 0, 0, 0, 0, -1);
  return 0;
}

