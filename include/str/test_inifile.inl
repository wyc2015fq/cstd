#include "str/inifile.inl"
#include "cmath.h"
int test_inifile()
{
  const char* strFilePath = "test.ini";
  const char* strmap = "abcdefghijklmnopqrstuvwxyz0123456789";
  char strSection[256];
  char strSectionKey[256];
  char strValue[256];
  char strValue2[256];
  int i = 0, secid, maxlen = 8;
  
#ifdef _WIN32
  const char* inifile = "C://mainwindow.ini";
#endif
  int video_trans = ini_get_int("demo", "video_trans", 0, inifile);
  int screen_flip = ini_get_int("demo", "screen_flip", 0, inifile);
  int screen_flop = ini_get_int("demo", "screen_flop", 0, inifile);

  for (i = 0; i < 100; ++i) {
    randstr(NULL, strValue, maxlen, strmap);
    randstr(NULL, strSectionKey, maxlen, strmap);
    secid = rand() % 5;
    _snprintf(strSection, 256, "key_%d", secid);
    ini_set_string(strSection, strSectionKey, strValue, strFilePath); //写入ini文件中相应字段
    ini_get_string(strSection, strSectionKey, 0, strValue2, 256, strFilePath); //写入ini文件中相应字段
    ASSERT(0 == strcmp(strValue2, strValue));
  }
  return 0;
}

