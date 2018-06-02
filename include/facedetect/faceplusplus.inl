
#ifndef _FACEPLUSPLUS_INL_
#define _FACEPLUSPLUS_INL_
#include "cstd.h"
#include "libfacerecog.h"
#include "curl/curl.h"
#include "faceplusplus_json.inl"

static size_t process_data(void *data, size_t size, size_t nmemb, char** pcontent)
{
  long sizes = size * nmemb;
  *pcontent = (char*)malloc(sizes + 10);
  if (*pcontent) {
    memcpy(*pcontent, data, sizes);
    //printf("s\n", *pcontent);
    (*pcontent)[sizes] = 0;
    return sizes;
  }
  return 0;
}

#include "curlhelp.inl"

int faceplusplus_file(const char* file, faceobj_t* fa) {
	CURL *curl;
	CURLcode res;
	char* content = NULL;
	int ret = -1;
	struct curl_slist *headerlist = NULL;
	headerlist = curl_slist_append(headerlist, "Content-Type:multipart/form-data");
	headerlist = curl_slist_append(headerlist, "Expect:");
	struct curl_httppost *formpost = NULL, *lastptr = NULL;
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "api_key", CURLFORM_COPYCONTENTS, "27A5JUsubCb5FgxEG2lxV-FO5NbQ8m-E", CURLFORM_END);
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "api_secret", CURLFORM_COPYCONTENTS, "0uYMv_pFRLUrr1PMT3-F2VJdXiXlZ182", CURLFORM_END);
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "return_attributes", CURLFORM_COPYCONTENTS, "gender,age,smiling,glass,facequality,blur", CURLFORM_END);
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "image_file", CURLFORM_FILE, file, CURLFORM_END);
  
  curl = curl_easy_init();
#if 1
	curl_easy_setopt(curl, CURLOPT_URL, "https://api-cn.faceplusplus.com/facepp/v3/detect");//url地址
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
#endif
  //curl_easy_setopt(curl, CURLOPT_URL, "https://www.faceplusplus.com.cn/");
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); 
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);//调试信息打开
  //curl_easy_setopt(curl, CURLOPT_URL, "http://www.cnblogs.com/hibraincol/");  
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);//这是write_data的第四个参数值
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, process_data);//对返回的数据进行操作的函数地址  
  res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
    printf("perform:%s\n", curl_easy_strerror_cn(res) );
  }
  else if (content) {
    long retcode = 0;
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &retcode);
    if ((res == CURLE_OK) && retcode == 200) {
      ret = faceplusplus_json_parse(content, fa);
    } else {
      printf( "curl_easy_getinfo error: %s\n", content );
    }
  }
	curl_easy_cleanup(curl);
	curl_formfree(formpost);
	curl_slist_free_all(headerlist);
  
  free(content);
#if 0
#endif
	return ret;
}
#include "img/imgio.inl"
int faceplusplus_image(const img_t* im, faceobj_t* fa) {
  int d = fa->face.w/3;
  const char* IMGTMPPATH = "D:\\aaa11.jpg";
  save_face(IMGTMPPATH, im, fa->face, fa->face.w/3);
  return faceplusplus_file(IMGTMPPATH, fa);
}

#endif // _FACEPLUSPLUS_INL_
