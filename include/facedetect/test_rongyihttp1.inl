
#include "cstd.h"
#include "cfile.h"
#include "str.h"
#include "rongyihttp.inl"
//#include "ext/zip.inl"
//#include "ext/unzip.inl"
//#include"ext/ZipImplement.inl"

#include "compress/test_zip.inl"
int test_http() {
  char* path;
  char* host;
  //return test_zip();
  if (0) {
    int strErrNO = -1;
    char* content = "{\"meta\":{\"errno\":1530400,\"msg\":\"mall id \u4e0d\u80fd\u4e3a\u7a7a\u3002 machine no \u4e0d\u80fd\u4e3a\u7a7a\u3002 terminal id \u4e0d\u80fd\u4e3a\u7a7a\u3002 md5sum \u4e0d\u80fd\u4e3a\u7a7a\u3002\"},\"result\":{}}";
    cJSON *pJsRoot = cJSON_Parse(content);
    if (pJsRoot) {
      cJSON* pJsMeta = cJSON_GetObjectItem(pJsRoot, "meta");
      if (pJsMeta) {
        cJSON* pJsErrno = cJSON_GetObjectItem(pJsMeta, "errno");
        if (pJsErrno) {
          strErrNO = pJsMeta->valueint;
        }
      }
      cJSON_Delete(pJsRoot);
    }
  }
  http_init();
  host = "http://dev.state.hdp.rongyi.com"; //¿ª·¢
  host = "http://state.hdp.rongyi.com"; // V6
  host = "http://qa.report.mcms.rongyi.com"; // ²âÊÔ
  host = "http://state.hdp.preview.rongyi.com"; // V4
  path = "D:/dt/out";
  rongyi_http_upload_folder(host, "11111111111", "22222222222222", "3333333333333", path);
  return 0;
}
