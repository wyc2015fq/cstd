#include "StdAfx.h"
#include "RongyiCHttpClient.h"
#include "curl.h"

static long CONNECTION_TIMEOUT_SEC = 5;
static long TIMEOUT_SEC = 10;

struct DownLoadInfo
{
	bool bWrite;
	FILE* fp;
	DownLoadInfo()
	{
		bWrite = false;
		fp = NULL;
	}
};

enum RongyiCHttpRet
{
	RONGYICHTTPRET_FILE_OPEN_FILE = CURL_LAST + 1,
};

static bool g_bInit = false;
RongyiCHttpClient::RongyiCHttpClient(void)
{
}

void RongyiCHttpClient::Init()
{
	if (!g_bInit){
		curl_global_init(CURL_GLOBAL_ALL); 
		g_bInit = true;
	}
}

void RongyiCHttpClient::DeInit()
{
	if (g_bInit){
		curl_global_cleanup();
		g_bInit = false;
	}
}

RongyiCHttpClient::~RongyiCHttpClient(void)
{
}

static size_t OnDownloadHeader(void* buffer, size_t size, size_t nmemb, void* lpVoid)
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
		string strHeadType(pTemp, 4);
		if (strHeadType != "HTTP" && strHeadType != "http"){
			break;
		}

		long indexTemp = 5;
		long retStartIndex = 0;
		while(indexTemp < size*nmemb){
			if (pTemp[indexTemp] == ' '){
				retStartIndex = ++indexTemp;
				break;
			}
			++indexTemp;
		}

		long retEndIndex = 0;
		while(indexTemp < size*nmemb){
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
		string strRet(pTemp + retStartIndex, retsize);

		bool* bWrite = (bool*)lpVoid;
		if (strRet == "200"){
			*bWrite = true;
		}else{
			*bWrite = false;
		}

	} while(0);

	return size*nmemb;
}

static size_t OnBrokenDownloadHeader(void* buffer, size_t size, size_t nmemb, void* lpVoid)
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
		string strHeadType(pTemp, 4);
		if (strHeadType != "HTTP" && strHeadType != "http"){
			break;
		}

		long indexTemp = 5;
		long retStartIndex = 0;
		while(indexTemp < size*nmemb){
			if (pTemp[indexTemp] == ' '){
				retStartIndex = ++indexTemp;
				break;
			}
			++indexTemp;
		}

		long retEndIndex = 0;
		while(indexTemp < size*nmemb){
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
		string strRet(pTemp + retStartIndex, retsize);

		bool* bWrite = (bool*)lpVoid;
		if (strRet == "200" || strRet == "206"){
			*bWrite = true;
		}else{
			*bWrite = false;
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

int RongyiCHttpClient::Download(const std::string & strUrl, const std::string & strFileName)
{
	int res;
	CURL* curl = curl_easy_init();
	if(NULL == curl){
		return CURLE_FAILED_INIT;
	}

	FILE* fp = fopen(strFileName.c_str(), "wb+");
	if (!fp){
		curl_easy_cleanup(curl);
		return RONGYICHTTPRET_FILE_OPEN_FILE;
	}

	DownLoadInfo cInfo;
	cInfo.fp = fp;

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);

	// callback function
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, OnDownloadHeader);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &cInfo.bWrite);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnDownloadData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&cInfo);

	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
	res = curl_easy_perform(curl);

	fclose(fp);
	curl_easy_cleanup(curl);

	return res;
}

int RongyiCHttpClient::BrokenDownload(const std::string & strUrl, const std::string & strFileName)
{
	int res;
	CURL* curl = curl_easy_init();
	if(NULL == curl){
		return CURLE_FAILED_INIT;
	}

	FILE* fp = fopen(strFileName.c_str(), "ab+");
	if (!fp){
		curl_easy_cleanup(curl);
		return RONGYICHTTPRET_FILE_OPEN_FILE;
	}

	fseek(fp, 0 , SEEK_END);
	curl_off_t filecurrentsize = ftell(fp);

	DownLoadInfo cInfo;
	cInfo.fp = fp;

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);

	// callback function
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, OnBrokenDownloadHeader);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &cInfo.bWrite);
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

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str || NULL == buffer )
	{
		return -1;
	}

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
	return nmemb;
}

int RongyiCHttpClient::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, CONNECTION_TIMEOUT_SEC);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT_SEC);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

int RongyiCHttpClient::Get(const std::string & strUrl, std::string & strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
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

int RongyiCHttpClient::Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
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

int RongyiCHttpClient::Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
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

int RongyiCHttpClient::http_post_file(const char *url, const HttpPostFileParam& cParam, std::string& strResponse, unsigned dwSendSpeedKB /* = 0 */)
{  
	CURL *curl;  
	CURLcode res;  
  
	struct curl_httppost *formpost=NULL;  
	struct curl_httppost *lastptr=NULL;  
	struct curl_slist *headerlist=NULL;  
	static const char buf[] = "Expect:";  
  
	//curl_global_init(CURL_GLOBAL_ALL);  
	for (int iIndex = 0; iIndex < POST_FILE_MAX_COUNT; ++iIndex)
	{
		if (cParam.cFileParam[iIndex].strName != "")
		{
			/* Fill in the file upload field */  
			curl_formadd(&formpost, &lastptr,  
				CURLFORM_COPYNAME, cParam.cFileParam[iIndex].strName.c_str(),  
				CURLFORM_FILE, cParam.cFileParam[iIndex].strValue.c_str(),  
				CURLFORM_END);

			string strFileName = cParam.cFileParam[iIndex].strValue;

			int iTemp = strFileName.rfind("\\");
			if (iTemp >= 0){
				strFileName = string(strFileName.c_str() + iTemp + 1, strFileName.length() - iTemp - 1);
			}
			
			/* Fill in the filename field */  
			curl_formadd(&formpost, &lastptr,  
				CURLFORM_COPYNAME, "filename",  
				CURLFORM_COPYCONTENTS, strFileName.c_str(),  
				CURLFORM_END); 
		}
		else
		{
			break;
		}
	}
  
	for (int iIndex = 0; iIndex < POST_VALUES_MAX_COUNT; ++iIndex)
	{
		if (cParam.cValueParam[iIndex].strName != "")
		{
			curl_formadd(&formpost, &lastptr, 
				CURLFORM_COPYNAME , cParam.cValueParam[iIndex].strName.c_str(),
				CURLFORM_COPYCONTENTS, cParam.cValueParam[iIndex].strValue.c_str(),
				CURLFORM_END);
		}
		else
		{
			break;
		}
	}
  
	curl = curl_easy_init();  
	/* initalize custom header list (stating that Expect: 100-continue is not 
		wanted */  
	headerlist = curl_slist_append(headerlist, buf);  
	if(curl) {  
	/* what URL that receives this POST */  

		curl_easy_setopt(curl, CURLOPT_URL, url);  
	if ( 1/*(argc == 2) && (!strcmp(argv[1], "noexpectheader"))*/ )  
		/* only disable 100-continue header if explicitly requested */  
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);  

	// speed control
	//CURLcode curl_easy_setopt(CURL *handle, CURLOPT_MAX_SEND_SPEED_LARGE,curl_off_t maxspeed);
	if (dwSendSpeedKB != 0){
		curl_easy_setopt(curl, CURLOPT_MAX_SEND_SPEED_LARGE, (curl_off_t)dwSendSpeedKB * 1024);
	}

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);  
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
  
	/* Perform the request, res will get the return code */  
	res = curl_easy_perform(curl);  
	/* Check for errors */  
	if(res != CURLE_OK)  
		fprintf(stderr, "curl_easy_perform() failed: %s\n",  
				curl_easy_strerror(res));  
  
	/* always cleanup */  
	curl_easy_cleanup(curl);  
  
	/* then cleanup the formpost chain */  
	curl_formfree(formpost);  
	/* free slist */  
	curl_slist_free_all (headerlist);  
	}  

	return 0;  
} 