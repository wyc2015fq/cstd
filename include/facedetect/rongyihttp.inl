
#ifndef _RONGYIHTTP_INL_
#define _RONGYIHTTP_INL_
#include "str.h"
#include "facelib/facedetect/facedetect.h"
#include <curl/curl.h>

static long CONNECTION_TIMEOUT_SEC = 5;
static long TIMEOUT_SEC = 10;

typedef struct HttpParam
{
  const char* strName;
  const char* strValue;
} HttpParam;

typedef struct DownLoadInfo
{
	int bWrite;
	FILE* fp;
} DownLoadInfo;

enum RongyiCHttpRet
{
	RONGYICHTTPRET_FILE_OPEN_FILE = CURL_LAST + 1,
};

static bool g_http_inited = false;

void http_init()
{
	if (!g_http_inited) {
		curl_global_init(CURL_GLOBAL_ALL); 
		g_http_inited = true;
	}
}

void http_uninit()
{
	if (g_http_inited){
		curl_global_cleanup();
		g_http_inited = false;
	}
}

static size_t http_OnDownloadHeader(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	if( NULL == buffer ) {
		return -1;
	}

	do 
	{
		if (size * nmemb < 12/*size of "HTTP/1.1 302"*/){
			break;
		}

		char* pTemp = (char*)buffer;
		if (0!=memcmp(pTemp, "HTTP", 4)&& 0!=memcmp(pTemp, "http", 4)){
			break;
		}

		int indexTemp = 5;
		long retStartIndex = 0;
		while(indexTemp < (int)(size*nmemb)){
			if (pTemp[indexTemp] == ' '){
				retStartIndex = ++indexTemp;
				break;
			}
			++indexTemp;
		}

		long retEndIndex = 0;
		while(indexTemp < (int)(size*nmemb)){
			if (pTemp[indexTemp] == ' '){
				retEndIndex = indexTemp;
				break;
			}
			++indexTemp;
		}

		if (retEndIndex <= retStartIndex){
			break;
		}

		long retsize = (retEndIndex - retStartIndex) < 3 ? (retEndIndex - retStartIndex) : 3;

		DownLoadInfo* pInfo = (DownLoadInfo*)lpVoid;
		if (0 == strncmp(pTemp + retStartIndex, "200", retsize)){
			pInfo->bWrite = true;
		}else{
			pInfo->bWrite = false;
		}

	} while(0);

	return size*nmemb;
}

static size_t http_OnBrokenDownloadHeader(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	if( NULL == buffer ){
		return -1;
	}

	do 
	{
		if (size * nmemb < 12/*size of "HTTP/1.1 302"*/){
			break;
		}

    char* pTemp = (char*)buffer;
		if (0!=memcmp(pTemp, "HTTP", 4)&& 0!=memcmp(pTemp, "http", 4)){
			break;
		}

		long indexTemp = 5;
		long retStartIndex = 0;
		while(indexTemp < (int)(size*nmemb)){
			if (pTemp[indexTemp] == ' '){
				retStartIndex = ++indexTemp;
				break;
			}
			++indexTemp;
		}

		long retEndIndex = 0;
		while(indexTemp < (long)(size*nmemb)){
			if (pTemp[indexTemp] == ' '){
				retEndIndex = indexTemp;
				break;
			}
			++indexTemp;
		}

		if (retEndIndex <= retStartIndex){
			break;
		}

    long retsize = (retEndIndex - retStartIndex) < 3 ? (retEndIndex - retStartIndex) : 3;
    DownLoadInfo* pInfo = (DownLoadInfo*)lpVoid;

    if (0 == strncmp(pTemp + retStartIndex, "200", retsize) || 0 == strncmp(pTemp + retStartIndex, "206", retsize)){
      pInfo->bWrite = true;
    }else{
			pInfo->bWrite = false;
		}

	} while(0);
	
	return size*nmemb;
}

static size_t OnDownloadData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	DownLoadInfo* pInfo = (DownLoadInfo*)lpVoid;
	if( NULL == pInfo || NULL == buffer ){
		return -1;
	}

	if (pInfo->bWrite){
		return fwrite(buffer, size, nmemb, pInfo->fp);
	}else{
		return size*nmemb;
	}
}

int http_Download(const char* strUrl, const char* strFileName)
{
	int res;
	CURL* curl = curl_easy_init();
	if(NULL == curl){
		return CURLE_FAILED_INIT;
	}

	FILE* fp = fopen(strFileName, "wb+");
	if (!fp){
		curl_easy_cleanup(curl);
		return RONGYICHTTPRET_FILE_OPEN_FILE;
	}

	DownLoadInfo cInfo;
	cInfo.fp = fp;

	curl_easy_setopt(curl, CURLOPT_URL, strUrl);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);

	// callback function
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http_OnDownloadHeader);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&cInfo);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnDownloadData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&cInfo);

	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	res = curl_easy_perform(curl);

	fclose(fp);
	curl_easy_cleanup(curl);

	return res;
}

int http_BrokenDownload(const char* strUrl, const char* strFileName)
{
	int res;
	CURL* curl = curl_easy_init();
	if(NULL == curl){
		return CURLE_FAILED_INIT;
	}

	FILE* fp = fopen(strFileName, "ab+");
	if (!fp){
		curl_easy_cleanup(curl);
		return RONGYICHTTPRET_FILE_OPEN_FILE;
	}

	fseek(fp, 0 , SEEK_END);
	curl_off_t filecurrentsize = ftell(fp);

	DownLoadInfo cInfo;
	cInfo.fp = fp;

	curl_easy_setopt(curl, CURLOPT_URL, strUrl);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);

	// callback function
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, http_OnBrokenDownloadHeader);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA,&cInfo.bWrite);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnDownloadData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&cInfo);

	// range
	curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, filecurrentsize);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	
	res = curl_easy_perform(curl);

	fclose(fp);
	curl_easy_cleanup(curl);

	return res;
}

static size_t http_OnResponse(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
  str_t* str = (str_t*)(lpVoid);
  char* pData = (char*)buffer;
  if( NULL == str || NULL == buffer )
  {
    return -1;
  }
  
  str_cat(str, STR2(pData, size * nmemb));
  return nmemb;
}

// brief HTTP POST请求
// param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
// param strPost 输入参数,使用如下格式para1=val2&…
// param strResponse 输出参数,返回的内容
// return 返回是否Post成功
int http_post(const char* strUrl, const char* strPost, str_t* strResponse)
{
  CURLcode res;
  http_init();
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_OnResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_SEC);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

//
// brief HTTP GET请求
// param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
// param strResponse 输出参数,返回的内容
// return 返回是否Post成功
int http_get(const char* strUrl, char* strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_OnResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	/**
	* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
	* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
	*/
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_SEC);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

//
// brief HTTPS POST请求,无证书版本
// param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
// param strPost 输入参数,使用如下格式para1=val1&…
// param strResponse 输出参数,返回的内容
// param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
// return 返回是否Post成功
int http_Posts(const char* strUrl, const char* strPost, char* strResponse, const char* pCaPath)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_OnResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		//缺省情况就是PEM，所以无需设置，另外支持DER
		//curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_SEC);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

//
// brief HTTPS GET请求,无证书版本
// param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
// param strResponse 输出参数,返回的内容
// param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
// return 返回是否Post成功
int http_Gets(const char* strUrl, char* strResponse, const char* pCaPath)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_OnResponse);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_SEC);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

int http_post_file(const char*url, const HttpParam* cFileParam, int FileParamNum, const HttpParam* cValueParam, int ValueParamNum, str_t* strResponse, unsigned dwSendSpeedKB /* = 0 */)
{  
	CURL *curl;  
	CURLcode res = CURLE_OK; 
  int i;
	struct curl_httppost *formpost=NULL;  
	struct curl_httppost *lastptr=NULL;  
	struct curl_slist *headerlist=NULL;  
	static const char buf[] = "Expect:";
  
	//curl_global_init(CURL_GLOBAL_ALL);
	for (i = 0; i < FileParamNum; ++i) {
		if (cFileParam[i].strName[0]) {
			char strFileName[256] = "";
			/* Fill in the file upload field */
			curl_formadd(&formpost,&lastptr, CURLFORM_COPYNAME, cFileParam[i].strName, CURLFORM_FILE, cFileParam[i].strValue, CURLFORM_END);  
      GetFileNameCopy(cFileParam[i].strValue, strFileName, countof(strFileName));
			/* Fill in the filename field */  
			curl_formadd(&formpost,&lastptr, CURLFORM_COPYNAME, "filename", CURLFORM_COPYCONTENTS, strFileName, CURLFORM_END); 
    } else {
			break;
		}
	}
  
	for (i = 0; i < ValueParamNum; ++i)	{
		if (0 != strcmp(cValueParam[i].strName, ""))	{
			curl_formadd(&formpost,&lastptr, CURLFORM_COPYNAME , cValueParam[i].strName, CURLFORM_COPYCONTENTS, cValueParam[i].strValue, CURLFORM_END);
		}	else {
			break;
    }
  }
  
  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not wanted */  
  headerlist = curl_slist_append(headerlist, buf);  
  if(curl) {
    /* what URL that receives this POST */  
    
    curl_easy_setopt(curl, CURLOPT_URL, url);  
    if ( 1/*(argc == 2)&& (!strcmp(argv[1], "noexpectheader"))*/ )  
      /* only disable 100-continue header if explicitly requested */  
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);  
    
    // speed control
    //CURLcode curl_easy_setopt(CURL *handle, CURLOPT_MAX_SEND_SPEED_LARGE,curl_off_t maxspeed);
    if (dwSendSpeedKB != 0){
      curl_easy_setopt(curl, CURLOPT_MAX_SEND_SPEED_LARGE, (curl_off_t)dwSendSpeedKB * 1024);
    }
    
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_OnResponse);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)strResponse);
    
    /* Perform the request, res will get the return code */  
    res = curl_easy_perform(curl);  
    /* Check for errors */  
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    
    /* always cleanup */  
    curl_easy_cleanup(curl); 
    /* then cleanup the formpost chain */  
    curl_formfree(formpost);  
    /* free slist */  
    curl_slist_free_all (headerlist);  
	}

	return res;
}

typedef struct {
  const uchar *buf;
  int len;
  int pos;
} readarg_t;
size_t readcb(void *ptr, size_t size, size_t nitems, void *stream)
{
  readarg_t *rarg = (readarg_t *)stream;
  int len = rarg->len - rarg->pos;
  if (len > (int)(size * nitems))
    len = size * nitems;
  memcpy(ptr, rarg->buf + rarg->pos, len);
  rarg->pos += len;
  printf("readcb: %d bytes\n", len);
  return len;
}

int http_post_mem(const char* url, const HttpParam* cValueParam, int ValueParamNum, const uchar* postdata, int datalen, str_t* strResponse)
{
  CURL *curl = NULL;
  CURLcode res;
  int i;
  readarg_t rarg;
  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  
  for (i = 0; i < ValueParamNum; ++i)	{
    if (0 != strcmp(cValueParam[i].strName, ""))	{
      curl_formadd(&formpost,&lastptr, CURLFORM_COPYNAME , cValueParam[i].strName, CURLFORM_COPYCONTENTS, cValueParam[i].strValue, CURLFORM_END);
    }	else {
      break;
    }
  }
  
  rarg.buf = postdata;
  rarg.len = datalen;
  rarg.pos = 0;
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_OnResponse);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)strResponse);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, rarg.len);
  curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
#if 0
  /* this works, but "Expect: 100-continue" is not added */
  curl_easy_setopt(curl, CURLOPT_READFUNCTION, readcb);
  curl_easy_setopt(curl, CURLOPT_READDATA, &rarg);
#else
  /* this works fine */
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, rarg.buf);
#endif
  res = curl_easy_perform(curl);
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));  
  } 
  /* always cleanup */
  curl_easy_cleanup(curl);  
  
  /* then cleanup the formpost chain */  
  curl_formfree(formpost);  
  /* free slist */
  curl_slist_free_all(headerlist);  
  return res;
}

#include "compress/zip.inl"
#include "codec/md5.inl"

int zip_files(const fileinfo_t* fi, int n, buf_t* bf) {
  int i, size, sz2=0;
  uchar* zbuf = NULL;
  uchar* fbuf=NULL;
  TZip zip[1] = {0};
  const char* pass = "";
  uint maxfilesize = 0;
  //char fname[256];
  size = 0;
  for (i=0; i<n; ++i) {
    size += fi[i].size;
    maxfilesize = MAX(maxfilesize, fi[i].size);
  }
  size += 1024000;
  maxfilesize += 10;
  bfsetsize(bf, size+maxfilesize);
  zbuf = bf->data;
  fbuf = bf->data+size;
  TZip_OpenMem(zip, zbuf, size, pass);
  for (i=0; i<n; ++i) {
    int flen = loadfile(fi[i].name, fbuf, maxfilesize, 0);
    const char* fname = GetFileNameExt(fi[i].name);
    TZip_AddMem(zip, fname, fbuf, flen);
  }
  TZip_Close(zip);
  bf->len = zip->writ;
  //savefile("D:/aaa.zip", bf->data, bf->len);
  return bf->len;
}
CC_INLINE char* cstr_catdup2(const char* s0, int l0, const char* s1, int l1) {
  char* s;
  CSTRINITLEN(s0, l0);
  CSTRINITLEN(s1, l1);
  s = MALLOC(char, (l0+l1+1));
  memcpy(s, s0, l0);
  memcpy(s+l0, s1, l1);
  s[l0+l1]=0;
  return s;
}
int zip_mem_files(const char* fmt, int id, const buf_t* bfs, int n, buf_t* bf) {
  int i, size, sz2=0;
  uchar* zbuf = NULL;
  //uchar* fbuf=NULL;
  TZip zip[1] = {0};
  const char* pass = "";
  int maxfilesize = 0;
  //char fname[256];
  size = 0;
  for (i=0; i<n; ++i) {
    size += bfs[i].len;
    maxfilesize = MAX(maxfilesize, bfs[i].len);
  }
  size += 1024000;
  maxfilesize += 10;
  bfsetsize(bf, size+maxfilesize);
  zbuf = bf->data;
  //fbuf = bf->data+size;
  TZip_OpenMem(zip, zbuf, size, pass);
  for (i=0; i<n; ++i) {
    char fname[32];
    _snprintf(fname, countof(fname), fmt, id+i);
    TZip_AddMem(zip, fname, bfs[i].data, bfs[i].len);
  }
  TZip_Close(zip);
  bf->len = zip->writ;
  //savefile("D:/aaa.zip", bf->data, bf->len);
  return bf->len;
}
int remove_files(const str_t* fpath, int n, buf_t* bf) {
  int i;
  for (i=0; i<n; ++i) {
    remove(fpath[i].s);
  }
  return 0;
}

#include "parser/cjson.inl"

//接口参数：

//名称	意义	备注
//mall_id	商场id	必须
//terminal_id	机器标号。如：e9198cad-0543-433c-a7ca-e0aaa7941302	必须
//face_data	上报人脸统计json数据	必须
//json数据按以下顺序传九个字段，包含20条
//face_code	人脸识别唯一id	必须
//enter_time	进入时间	必须
//left_time	离开时间	必须
//face_no	人脸识别编号	必须
//sideface	正脸标识	必须
//age	年龄	预留
//gender	性别	预留
//glass	是否佩戴眼镜	预留
//smile	是否微笑	预留

int faceobj_sprintf(char* buf, char* end, const faceobj_t* fo, int n, int bImage) {
  int i;
  if (bImage) {
    str_t str[1] = {0};
    for (i=0; i<n; ++i) {
      const faceobj_t* f = fo + i;
      const char* fmt;
      if (n==1) {
        fmt = "[[%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\"]]";
      } else {
        if (0==i) {
          fmt = "[[%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\"]\n";
        } else if ((n-1)==i) {
          fmt = ",[%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\"]]";
        } else {
          fmt = ",[%d,%d,%d,%d,%d,%d,%d,%d,%d,\"%s\"]";
        }
      }
      const buf_t* bf = fo[i].image;
      str_setsize(str, (bf->len*2+10));
      str->l = base64_encode(bf->data, bf->len, str->s);
      str->s[str->l] = 0;
#if 0
      buf_t bf1[1] = {0};
      bfsetsize(bf1, str->l);
      int l = base64_decode(str->s, str->l, bf1->data);
    img_t im1[1] = {0};
    imload_mem(bf1->data, l, 3, im1);
#endif

      if (strchr(str->s, ' ')) {
        int asdf=0;
      }
      _snprintf(buf, end-buf, fmt, f->face_code, f->enter_time, f->left_time, f->id, f->sideface, f->age, f->gender, f->glass, f->smile, str->s);
      buf += strlen(buf);
    }
    str_free(str);
    *buf = 0;
  } else {
    for (i=0; i<n; ++i) {
      const faceobj_t* f = fo + i;
      const char* fmt;
      if (n==1) {
        fmt = "[[%d,%d,%d,%d,%d,%d,%d,%d,%d]]";
      } else {
        if (0==i) {
          fmt = "[[%d,%d,%d,%d,%d,%d,%d,%d,%d]";
        } else if ((n-1)==i) {
          fmt = ",[%d,%d,%d,%d,%d,%d,%d,%d,%d]]";
        } else {
          fmt = ",[%d,%d,%d,%d,%d,%d,%d,%d,%d]";
        }
      }
      _snprintf(buf, end-buf, fmt, f->face_code, f->enter_time, f->left_time, f->id, f->sideface, f->age, f->gender, f->glass, f->smile);
      buf += strlen(buf);
    }
    *buf = 0;
  }
  return end - buf;
}

int cjson_parse_response(const char* content) {
  cJSON *pJsRoot = cJSON_Parse(content), *pJsCode;
  int ErrNO = -1;
  if (pJsRoot) {
    if ((pJsCode = cJSON_GetObjectItem(pJsRoot, "code"))) {
      if (pJsCode->valuestring) {
        ErrNO = atoi(pJsCode->valuestring);
      } else {
        ErrNO = pJsCode->valueint;
      }
    }
    cJSON_Delete(pJsRoot);
  }
  return ErrNO==200;
}

int rongyihttp_report_face_data(const char* url, const char* mac_address, const faceobj_t* f, int n, bool bImage) {
  int ret = 0;
  if (n>0) {
    str_t strResponse[1] = {0};
    str_t strPostData[1] = {0};
    str_setsize(strPostData, (bImage ? 100000 : 100)*n+200);
    _snprintf(strPostData->s, strPostData->l, "mac_address=%s&face_data=", mac_address);
    int len = strlen(strPostData->s);
    faceobj_sprintf(strPostData->s+len, strPostData->s+strPostData->l, f, n, bImage);
    strPostData->l = strlen(strPostData->s);
    strPostData->s[strPostData->l] = 0;
    str_save("D://aaa.txt", strPostData);
    int iSendRet = http_post(url, strPostData->s, strResponse);
    if(iSendRet == 0) {
      ret = cjson_parse_response(strResponse->s);
      //printf("%s\n", strResponse->s);
    }
    str_free(strResponse);
    str_free(strPostData);
  }
  return ret;
}

int rongyihttp_upload_mem(const char* url, const char* mall_id, const char* terminal_id, const char* machine_no, const char* stime, const buf_t* bf, const char* tmppath) {
  char md5sum[64] = {0};
  char file[256] = "";
  str_t strResponse[1] = {0};
  int m_dwUploadSpeedKB = 0;
  int ret = 0;
  md5_string(bf->data, bf->len, md5sum, countof(md5sum));
  _snprintf(file, countof(file), "%s/%s_%s_%s.zip", tmppath, mall_id, md5sum, stime);
  //ret = http_post_file(url, cFileParam, countof(cFileParam), cValueParam, countof(cValueParam), strResponse, m_dwUploadSpeedKB);
  
  if (1) {
    HttpParam cFileParam[] = {{"file", file}};
    HttpParam cValueParam[] = {{"mall_id", mall_id}, {"terminal_id", terminal_id}, {"md5sum", md5sum}, {"machine_no", machine_no}};
    //ret = http_post_mem(url, cValueParam, countof(cValueParam), bf->data, bf->len, strResponse);
    buf_save(file, bf);
    ret = http_post_file(url, cFileParam, countof(cFileParam), cValueParam, countof(cValueParam), strResponse, 0);
    sys_rmfile(file);
  } else {
    HttpParam cValueParam[] = {{"filename", file}, {"mall_id", mall_id}, {"terminal_id", terminal_id}, {"md5sum", md5sum}, {"machine_no", machine_no}};
    ret = http_post_mem(url, cValueParam, countof(cValueParam), bf->data, bf->len, strResponse);
  }
  if (0) {
    _snprintf(file, countof(file), "D:/%s.zip", md5sum);
    buf_save(file, bf);
    buf_t bf1[1] = {0};
    buf_load(file, bf1);
    char str[64];
    md5_string(bf1->data, bf1->len, str, countof(str));
    bffree(bf1);
  }
  //+	md5sum	0x0018b3fc "79ab1fd11f64ef99a743d0163ef9e728"
  printf("upload[%d][%s][%s][%s]\n", ret, url, file, strResponse->s);

  // 4. update to DB
  if(ret == 0) {
    // char* content = "{\"meta\":{\"errno\":1530400,\"msg\":\"mall id \u4e0d\u80fd\u4e3a\u7a7a\u3002 machine no \u4e0d\u80fd\u4e3a\u7a7a\u3002 terminal id \u4e0d\u80fd\u4e3a\u7a7a\u3002 md5sum \u4e0d\u80fd\u4e3a\u7a7a\u3002\"},\"result\":{}}";
    int ErrNO = cjson_parse_response(strResponse->s);
    ret = 0 == ErrNO;
  } else {
    ret = 0;
  }
  str_free(strResponse);
  return ret;
}
int rongyihttp_upload_folder(const char* host, const char* mall_id, const char* terminal_id, const char* machine_no, const char* path, const char* tmppath) {
  dirlist_t dl[1] = {0};
  buf_t bf[1] = {0};
  int ret = 0, i;
  dirlist(dl, path, "*.jpg", FF_FULLNAME);
  //n = MIN(dl->n, 10);
  for (i=0; i<dl->n; i+=200) {
    int n = MIN(200, dl->n - i);
    zip_files(dl->v+i, n, bf);
    const char* stime = _strtime2();
    ret = rongyihttp_upload_mem(host, mall_id, terminal_id, machine_no, stime, bf, tmppath);
    if (ret) {
    }
  }
  dirlist_free(dl);
  bffree(bf);
  return ret;
}
int rongyihttp_upload_mem_files(const char* host, const char* mall_id, const char* terminal_id, const char* machine_no, int imgid, buf_t* bfs, int nfile, const char* zippath, const char* tmppath) {
  buf_t bf[1] = {0};
  int ret = 0, i;
  char buf[256];
  {
    dirlist_t dl[1] = {0};
    dirlist(dl, zippath, "*.zip", FF_FULLNAME);
    for (i=0; i<dl->n; i++) {
      const char* name = dl->v[i].name;
      buf_load(name, bf);
      GetFileNameCopy(name, buf, countof(buf));
      ret = rongyihttp_upload_mem(host, mall_id, terminal_id, machine_no, buf, bf, tmppath);
      andprintf("rongyihttp_upload_mem %s %s! host=%s\n", buf, ret ? "ok" : "fail", host);
      if (ret) {
        sys_rmfile(name);
      }
    }
    dirlist_free(dl);
  }
  for (i=0; i<nfile; i+=200) {
    int n = MIN(200, nfile - i);
    const char* stime = _strtime2();
    zip_mem_files("image/%d.jpg", imgid, bfs+i, n, bf);
    imgid += n;
    ret = rongyihttp_upload_mem(host, mall_id, terminal_id, machine_no, stime, bf, tmppath);
    andprintf("rongyihttp_upload_mem %s %s! host=%s\n", stime, ret ? "ok" : "fail", host);
    if (!ret) {
      _snprintf(buf, countof(buf), "%s/%s.zip", zippath, stime);
      buf_save(buf, bf);
    }
  }
  bffree(bf);
  // sys_rmfile();
  return imgid;
}
#endif // _RONGYIHTTP_INL_
