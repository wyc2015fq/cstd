#include "curl/curl.h"
#include <stdio.h>
int test_curl() {
  CURL *curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.cnblogs.com/hibraincol/"); 
    res = curl_easy_perform(curl); 
    if (0!=res) {      
      printf("curl error: %d\n", res);
    }
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}
