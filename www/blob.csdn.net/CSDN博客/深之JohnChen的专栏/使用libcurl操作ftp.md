# 使用libcurl操作ftp - 深之JohnChen的专栏 - CSDN博客

2018年11月15日 20:20:49[byxdaz](https://me.csdn.net/byxdaz)阅读数：1022


       使用libcurl操作ftp存储文件、目录等，操作之前可以先登录，也可以不登录再请求添加ftp和用户密码信息，libcurl会检测没有登录的话会自动登录请求。

1、ftp上传文件，需要自动创建目录时，需要设置CURLOPT_FTP_CREATE_MISSING_DIRS属性。

ftp上传文件时，目录不存在时，设置下面的属性，可以在上传文件时，自动创建目录。

curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);

也可以通过ftp命令MKD创建目录。

2、目录和文件命令

MKD：创建目录，发送创建目录命令之后，会返回CURLE_FTP_COULDNT_RETR_FILE（19，这是一个奇怪的回答“RETR”命令或一个零字节传输完成。）

CWD：切换目录

RMD: 删除目录

PWD：返回当前目录

DELE：删除文件

NLST、LIST、MLSD三个命令都可以获取FTP指定目录下的文件信息，如文件名称、文件大小、文件类型、文件最后修改时间等。

NLST：返回获取文件名。如：00008d38072c01ba00e41507ba1bcb20.mdat\r\n03258da63bccf1b387185bdd0b7afd76.mdat\r\n

LIST：返回获取文件类型、修改日期、大小、名称等信息。如：-rw-r--r-- 1 ftp ftp         154168 Jan 20 18:53 00008d38072c01ba00e41507ba1bcb20.mdat\r\n-rw-r--r-- 1 ftp ftp         154168 Feb 23 09:36 03258da63bccf1b387185bdd0b7afd76.mdat\r\n

MLSD：返回获取文件类型、修改日期、大小、名称等信息，用“;”分隔各个属性。如：type=file;modify=20180120185307;size=154168; 00008d38072c01ba00e41507ba1bcb20.mdat\r\ntype=file;modify=20180223013632;size=154168; 03258da63bccf1b387185bdd0b7afd76.mdat\r\n

```
CURL *curl = NULL;
curl = curl_init();
curl_easy_setopt(curl, CURLOPT_URL, url);
curl_easy_setopt(curl, CURLOPT_USERPWD, user_key);
curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "MLSD");//type=file;modify=20180223013632;size=154168; 03258da63bccf1b387185bdd0b7afd76.mdat
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getFTPFileList);
curl_perform(curl);
curl_exit(curl);
```

3、CURLOPT_QUOTE、CURLOPT_POSTQUOTE、CURLOPT_CUSTOMREQUEST区别

CURLOPT_QUOTE，一组先于 FTP 请求的在服务器上执行的FTP命令。

CURLOPT_POSTQUOTE，在 FTP 请求执行完成后，在服务器上执行的一组array格式的 FTP 命令。

CURLOPT_QUOTE 和 CURLOPT_POSTQUOTE

这两个选项的功能类似，它们的共同点都是给 FTP 或 SFTP 传递命令。这些命令应该放在 struct slist 链表中存储，使用时需要用 curl_slist_append() 函数将这些命令打包起来，然后一起发送出去。

它们的不同点是：CURLOPT_QUOTE 选项要求命令要在 FTP 传输请求之前就要发送到库，而 CURLOPT_POSTQUOTE 则可以在 FTP 传输请求发送完后发送。

CURLOPT_CUSTOMREQUEST，自定义请求方式。

4、实例

//FtpOperate.h

```
#include <list>

//ftp目录文件信息
struct FtpDirFileInfo
{
	char szFileName[MAX_PATH];				//文件名称
	unsigned char ucIsDir;					//是否是目标
	unsigned char ucReserve[3];				//保留值
	unsigned int uiFileSize;				//文件大小
};

	/*
	功能：执行ftp命令(执行一组命令方式)(用于创建目录，删除目录等)
	参数：curl，表示curl；pUrl，表示ftp的url地址；usPort，表示ftp端口；pUsername，表示ftp登录用户名；pPassword，表示ftp登录密码；
		  pCommand，命令；bIsGetResponseData，表示是否获取返回数据；strReponse，返回数据；nConnectTimeout，连接超时时间，单位秒；nTimeout，操作等待超时时间，单位秒；
	返回值：int，0，表示成功；非0，表示错误码
	*/
	int  ExecuteFtpRequestCommandByQuote(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pCommand,bool bIsGetResponseData,std::string & strReponse, int nConnectTimeout = 30, int nTimeout = 30);
	/*
	功能：执行ftp命令(自定义请求方式)
	参数：curl，表示curl；pUrl，表示ftp的url地址；usPort，表示ftp端口；pUsername，表示ftp登录用户名；pPassword，表示ftp登录密码；
	pCommand，命令；bIsGetResponseData，表示是否获取返回数据；strReponse，返回数据；nConnectTimeout，连接超时时间，单位秒；nTimeout，操作等待超时时间，单位秒；
	返回值：int，0，表示成功；非0，表示错误码
	*/
	int  ExecuteFtpRequestCommandByCustomRequest(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pCommand, bool bIsGetResponseData, std::string & strReponse, int nConnectTimeout = 30, int nTimeout = 30);

	//ftp登录
	int  FtpLogin(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, std::string response);

	/*
	功能：获取指定目录的所包含文件和目录
	参数：curl，表示curl；pUrl，表示ftp的url地址；usPort，表示ftp端口；pUsername，表示ftp登录用户名；pPassword，表示ftp登录密码；
	      pPathName，表示目录，NULL时表示主目录，非NULL，指定某个目录；listFtpDirFileInfo，表示返回的目录和文件列表
	返回值：int，0，表示成功；非0，表示错误码
	*/
	int FtpGetAllDirAndFile(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pPathName, std::list<FtpDirFileInfo> & listFtpDirFileInfo);

	//是否存在该目录
	bool FtpExistDir(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pPathName);

	//创建目录
	int FtpCreateDir(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pPathName);

	//删除目录
	int FtpDeleteDir(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pPathName);

	//删除文件
	int FtpDeleteFile(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pFilePathName);

	//上传文件
	int FtpUploadFile(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword,char *pRomuteStoreFilePathName, FILE * fpUploadFile, int nUploadFileSize, int nConnectTimeout = 30, int nTimeout = 30);

	//下载文件
	int FtpDownloadFile(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword,char *pRomuteFilePathName, FILE * fpDownloadFile, int nConnectTimeout = 30, int nTimeout = 30);
```

//FtpOperate.cpp

```
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include "FtpOperate.h"

std::wstring AsciiToUnicode(const std::string& str)
{
	// 预算-缓冲区中宽字节的长度  
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
	// 给指向缓冲区的指针变量分配内存  
	wchar_t *pUnicode = (wchar_t*)malloc(sizeof(wchar_t)*unicodeLen);
	// 开始向缓冲区转换字节  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, unicodeLen);
	std::wstring ret_str = pUnicode;
	free(pUnicode);
	return ret_str;
}

std::string UnicodeToUtf8(const std::wstring& wstr)
{
	// 预算-缓冲区中多字节的长度  
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	// 给指向缓冲区的指针变量分配内存  
	char *pAssii = (char*)malloc(sizeof(char)*ansiiLen);
	// 开始向缓冲区转换字节  
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, ansiiLen, nullptr, nullptr);
	std::string ret_str = pAssii;
	free(pAssii);
	return ret_str;
}

//ANSI转UTF8
std::string AsciiToUtf8(const std::string& str)
{
	return UnicodeToUtf8(AsciiToUnicode(str));
}

//UTF8转ANSI
std::string Utf8toAscii(const std::string strUTF8)
{
	std::string  strAnsi = "";
	//获取转换为多字节后需要的缓冲区大小，创建多字节缓冲区
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];
	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8.c_str(), -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strAnsi = szBuffer;
	//清理内存
	delete[]szBuffer;
	delete[]wszBuffer;
	return strAnsi;
}

// 字符串分割
int StringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator)
{
	if (src.empty() || separator.empty())
		return 0;

	int nCount = 0;
	std::string temp;
	size_t pos = 0, offset = 0;

	// 分割第1~n-1个
	while ((pos = src.find_first_of(separator, offset)) != std::string::npos)
	{
		temp = src.substr(offset, pos - offset);
		if (temp.length() >= 0) {
			dst.push_back(temp);
			nCount++;
		}
		offset = pos + 1;
	}

	// 分割第n个
	temp = src.substr(offset, src.length() - offset);
	if (temp.length() >= 0) {
		dst.push_back(temp);
		nCount++;
	}

	return nCount;
}

//去前后空格
std::string& StringTrim(std::string &str)
{
	if (str.empty()) {
		return str;
	}
	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ") + 1);
	return str;
}

//ftp请求命令应答数据回调函数
size_t ftp_req_reply(void *ptr, size_t size, size_t nmemb, void *stream)
{
	if (stream == NULL || ptr == NULL || size == 0)
		return 0;

	size_t realsize = size * nmemb;
	std::string *buffer = (std::string*)stream;
	if (buffer != NULL)
	{
		buffer->append((const char *)ptr, realsize);
	}
	return realsize;
}

/*
功能：执行ftp命令(执行一组命令方式)(用于创建目录，删除目录等)
参数：curl，表示curl；pUrl，表示ftp的url地址；usPort，表示ftp端口；pUsername，表示ftp登录用户名；pPassword，表示ftp登录密码；
pCommand，命令；bIsGetResponseData，表示是否获取返回数据；strReponse，返回数据；nConnectTimeout，连接超时时间，单位秒；nTimeout，操作等待超时时间，单位秒；
返回值：int，0，表示成功；非0，表示错误码
*/
int  ExecuteFtpRequestCommandByQuote(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pCommand, bool bIsGetResponseData, std::string & strReponse,int nConnectTimeout,int nTimeout)
{
	if (bIsGetResponseData)
	{
		strReponse.clear();
	}
	curl_easy_reset(curl);
	char szCurlUrl[128] = { 0 };
	sprintf_s(szCurlUrl, "ftp://%s/", pUrl);
	curl_easy_setopt(curl, CURLOPT_URL, szCurlUrl);
	curl_easy_setopt(curl, CURLOPT_PORT, usPort);
	// user & pwd
	char szUsernameAndPassword[64] = { 0 };
	sprintf_s(szUsernameAndPassword, "%s:%s", pUsername, pPassword);
	curl_easy_setopt(curl, CURLOPT_USERPWD, szUsernameAndPassword);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	struct curl_slist* headerlist = NULL;
	headerlist = curl_slist_append(headerlist, pCommand);
	curl_easy_setopt(curl, CURLOPT_QUOTE, headerlist);

	if (bIsGetResponseData)
	{
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_req_reply);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strReponse);
	}

	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nConnectTimeout);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
	curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);	//目录不存在时，上传文件时，先创建目录
	CURLcode res = curl_easy_perform(curl);
	if (headerlist != NULL)
	{
		curl_slist_free_all(headerlist); //free the list again
	}
	// Check for errors
	if (res != CURLE_OK)
	{
		return 1;
	}
	return 0;
}
/*
功能：执行ftp命令(自定义请求方式)
参数：curl，表示curl；pUrl，表示ftp的url地址；usPort，表示ftp端口；pUsername，表示ftp登录用户名；pPassword，表示ftp登录密码；
pCommand，命令；bIsGetResponseData，表示是否获取返回数据；strReponse，返回数据；nConnectTimeout，连接超时时间，单位秒；nTimeout，操作等待超时时间，单位秒；
返回值：int，0，表示成功；非0，表示错误码
*/
int  ExecuteFtpRequestCommandByCustomRequest(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pCommand, bool bIsGetResponseData, std::string & strReponse, int nConnectTimeout, int nTimeout)
{
	strReponse.clear();
	curl_easy_reset(curl);
	char szCurlUrl[128] = { 0 };
	sprintf_s(szCurlUrl, "ftp://%s/", pUrl);
	curl_easy_setopt(curl, CURLOPT_URL, szCurlUrl);
	curl_easy_setopt(curl, CURLOPT_PORT, usPort);
	// user & pwd
	char szUsernameAndPassword[64] = { 0 };
	sprintf_s(szUsernameAndPassword, "%s:%s", pUsername, pPassword);
	curl_easy_setopt(curl, CURLOPT_USERPWD, szUsernameAndPassword);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, pCommand);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_req_reply);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strReponse);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nConnectTimeout);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
	curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);	//目录不存在时，上传文件时，先创建目录
	CURLcode res = curl_easy_perform(curl);
	// Check for errors
	if (res != CURLE_OK)
	{
		if ((strstr(pCommand, "MKD ") != NULL || strstr(pCommand, "RMD ") != NULL) && res == CURLE_FTP_COULDNT_RETR_FILE)
		{
			return 0;	//目录操作时，错误码为完成的传输尺寸为零字节
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

//ftp登录
int  FtpLogin(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, std::string response)
{
	response.clear();
	curl_easy_reset(curl);
	char szCurlUrl[128] = { 0 };
	sprintf_s(szCurlUrl, "ftp://%s/", pUrl);
	curl_easy_setopt(curl, CURLOPT_URL, szCurlUrl);
	curl_easy_setopt(curl, CURLOPT_PORT, usPort);
	/*user & pwd*/
	char szUsernameAndPassword[64] = { 0 };
	sprintf_s(szUsernameAndPassword, "%s:%s", pUsername, pPassword);
	curl_easy_setopt(curl, CURLOPT_USERPWD, szUsernameAndPassword);
	//dir create purview
	curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ftp_req_reply);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	CURLcode res = curl_easy_perform(curl);
	/* Check for errors */
	if (res != CURLE_OK)
	{
		return 1;
	}
	return 0;
}

/*
功能：获取指定目录的所包含文件和目录
参数：curl，表示curl；pUrl，表示ftp的url地址；usPort，表示ftp端口；pUsername，表示ftp登录用户名；pPassword，表示ftp登录密码；
pPathName，表示目录，NULL时表示主目录，非NULL，指定某个目录；listFtpDirFileInfo，表示返回的目录和文件列表
返回值：int，0，表示成功；非0，表示错误码
*/
int FtpGetAllDirAndFile(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pPathName, std::list<FtpDirFileInfo> & listFtpDirFileInfo)
{
	/*
	-rw-r--r-- 1 ftp ftp       11650932 Nov 09 18:43 aaa.avi
	drwxr-xr-x 1 ftp ftp              0 Nov 15 14:37 abc
	*/
	listFtpDirFileInfo.clear();
	char szCommand[MAX_PATH] = { 0 };
	if (pPathName != NULL)
	{
		sprintf_s(szCommand, "LIST %s", pPathName);
	}
	else
	{
		sprintf_s(szCommand, "%s", "LIST");
	}
	std::string strFileList = "", strFileListAscii = "";
	int nRet = ExecuteFtpRequestCommandByCustomRequest(m_curl, m_szFtpUrl, m_usFtpPort, m_szFtpLoginUsername, m_szFtpLoginPassword, szCommand, true, strFileList);
	if (nRet == 0 && strFileList.size() > 0)
	{
		FtpDirFileInfo fdfiFtpDirFileInfo = { 0 };
		strFileListAscii = Utf8toAscii(strFileList);
		std::vector<std::string> listRecord;
		int nRetSplit = StringSplit(listRecord, strFileListAscii,"\r\n");
		if (nRetSplit > 0)
		{
			std::vector<std::string>::iterator iterRecord, iterEndRecord;
			iterRecord = listRecord.begin();
			iterEndRecord = listRecord.end();
			for (iterRecord; iterRecord != iterEndRecord; iterRecord++)
			{
				memset(&fdfiFtpDirFileInfo, 0, sizeof(FtpDirFileInfo));
				std::vector<std::string> listItem;
				nRetSplit = StringSplit(listItem, iterRecord->c_str(), " ");
				if (nRetSplit > 0)
				{
					int n = 0;
					std::vector<std::string>::iterator iter, iterEnd;
					iter = listItem.begin();
					iterEnd = listItem.end();
					for (iter; iter != iterEnd; iter++)
					{
						if (n == 0)
						{
							if (iter->at(0) == 'd' || iter->at(0) == 'D')
							{
								fdfiFtpDirFileInfo.ucIsDir = 1;
							}
							else
							{
								fdfiFtpDirFileInfo.ucIsDir = 0;
							}
						}
						else if (n == 4)
						{
							fdfiFtpDirFileInfo.uiFileSize = atol(iter->c_str());
						}
						else if (n == listItem.size() - 1)
						{
							strcpy_s(fdfiFtpDirFileInfo.szFileName, iter->c_str());
						}
						n++;
					}
					//insert
					listFtpDirFileInfo.push_back(fdfiFtpDirFileInfo);
				}
			}
		}
	}
	return nRet;
}

//是否存在该目录
bool FtpExistDir(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pPathName)
{
	char szComand[MAX_PATH] = { 0 };
	sprintf_s(szComand, "LIST %s", pPathName);
	std::string strResponse = "";
	int nRet = ExecuteFtpRequestCommandByCustomRequest(curl, pUrl, usPort, pUsername, pPassword, szComand, false, strResponse);
	if (nRet == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//创建目录
int FtpCreateDir(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pPathName)
{
	int nRet = 0;
	std::string strResponse = "";
	char szComand[MAX_PATH] = { 0 };
	char szPathNameTemp[MAX_PATH] = { 0 };
	//逐级创建目录，有些ftp不支持多级目录一次性创建
	char *pFindFlag = NULL;
	char *pSrc = pPathName;
	while (true)
	{
		pFindFlag = strchr(pSrc, '/');
		if (pFindFlag == NULL)
		{
			sprintf_s(szComand, "MKD %s", pPathName);
			nRet = ExecuteFtpRequestCommandByQuote(curl, pUrl, usPort, pUsername, pPassword, szComand, false, strResponse);
			break;
		}
		else
		{
			memset(szPathNameTemp, 0, sizeof(szPathNameTemp));
			memcpy(szPathNameTemp, pPathName, pFindFlag - pPathName);
			sprintf_s(szComand, "MKD %s", szPathNameTemp);
			nRet = ExecuteFtpRequestCommandByQuote(curl, pUrl, usPort, pUsername, pPassword, szComand, false, strResponse);
			pSrc += (pFindFlag - pSrc + 1);
			if (pSrc == NULL || *pSrc == '\0')
			{
				break;
			}
		}
	} 
	return 0;
}

//删除目录
int FtpDeleteDir(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pPathName)
{
	char szComand[MAX_PATH] = { 0 };
	sprintf_s(szComand, "RMD %s", pPathName);
	std::string strResponse = "";
	return ExecuteFtpRequestCommandByQuote(curl, pUrl, usPort, pUsername, pPassword, szComand,false,strResponse);
}

//删除文件
int FtpDeleteFile(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pFilePathName)
{
	char szComand[MAX_PATH] = { 0 };
	sprintf_s(szComand, "DELE %s", pFilePathName);
	std::string strResponse = "";
	return ExecuteFtpRequestCommandByQuote(curl, pUrl, usPort, pUsername, pPassword, szComand, false, strResponse);
}

//上传文件
int  FtpUploadFile(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pRomuteStoreFilePathName, FILE * fpUploadFile, int nUploadFileSize, int nConnectTimeout, int nTimeout)
{
	curl_easy_reset(curl);
	char szCurlUrl[MAX_PATH] = { 0 };
	sprintf_s(szCurlUrl, "ftp://%s/%s", pUrl, pRomuteStoreFilePathName);
	curl_easy_setopt(curl, CURLOPT_URL, szCurlUrl);
	curl_easy_setopt(curl, CURLOPT_PORT, usPort);
	/*user & pwd*/
	char szUsernameAndPassword[64] = { 0 };
	sprintf_s(szUsernameAndPassword, "%s:%s", pUsername, pPassword);
	curl_easy_setopt(curl, CURLOPT_USERPWD, szUsernameAndPassword);

	if (fpUploadFile != NULL)
	{
		fseek(fpUploadFile, 0, SEEK_SET);
	}
	curl_easy_setopt(curl, CURLOPT_READDATA, fpUploadFile);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
	curl_easy_setopt(curl, CURLOPT_INFILESIZE, nUploadFileSize);
	curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nConnectTimeout);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
	CURLcode res = curl_easy_perform(curl);
	/* Check for errors */
	if (res != CURLE_OK)
	{
		return 1;
	}
	return 0;
}
//下载文件
int  FtpDownloadFile(CURL * curl, char *pUrl, unsigned short usPort, char *pUsername, char *pPassword, char *pRomuteFilePathName, FILE * fpDownloadFile, int nConnectTimeout, int nTimeout)
{
	curl_easy_reset(curl);
	char szCurlUrl[MAX_PATH] = { 0 };
	sprintf_s(szCurlUrl, "ftp://%s/%s", pUrl,pRomuteFilePathName);
	curl_easy_setopt(curl, CURLOPT_URL, szCurlUrl);
	curl_easy_setopt(curl, CURLOPT_PORT, usPort);
	/*user & pwd*/
	char szUsernameAndPassword[64] = { 0 };
	sprintf_s(szUsernameAndPassword, "%s:%s", pUsername, pPassword);
	curl_easy_setopt(curl, CURLOPT_USERPWD, szUsernameAndPassword);

	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fpDownloadFile);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, nConnectTimeout);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, nTimeout);
	CURLcode res = curl_easy_perform(curl);
	/* Check for errors */
	if (res != CURLE_OK)
	{
		return 1;
	}
	return 0;
}
```

//调用

```
CURL * m_curl = curl_easy_init();

	FILE *fpUploadFile = fopen("c:\\devconfig.tar", "rb");
	unsigned int nUploadFileSize = 0;
	fseek(fpUploadFile, 0, SEEK_END);
	nUploadFileSize = ftell(fpUploadFile);
	nRet = FtpUploadFile(m_curl, m_szFtpUrl, m_usFtpPort, m_szFtpLoginUsername, m_szFtpLoginPassword,"88/ABA/111/22/a.tar", fpUploadFile, nUploadFileSize);
	std::list<FtpDirFileInfo> listFtpDirFileInfo;
	nRet = FtpGetAllDirAndFile(m_curl, m_szFtpUrl, m_usFtpPort, m_szFtpLoginUsername, m_szFtpLoginPassword, "88/ABA/111", listFtpDirFileInfo);
	FILE *fpDownloadFile = fopen("d:\\aaaaaaaaaaaaa.tar", "wb");
	nRet = FtpDownloadFile(m_curl, m_szFtpUrl, m_usFtpPort, m_szFtpLoginUsername, m_szFtpLoginPassword, "88/ABA/111/22/a.tar", fpDownloadFile);
	fclose(fpUploadFile);
	fclose(fpDownloadFile);
	std::string strFileList = "";
	nRet = ExecuteFtpRequestCommandByCustomRequest(m_curl, m_szFtpUrl, m_usFtpPort, m_szFtpLoginUsername, m_szFtpLoginPassword, "LIST abc",true, strFileList);
	if (nRet == 0 && strFileList.size() > 0)
	{
		strFileListAscii = Utf8toAscii(strFileList);
	}
	nRet = ExecuteFtpRequestCommandByQuote(m_curl, m_szFtpUrl, m_usFtpPort, m_szFtpLoginUsername, m_szFtpLoginPassword, "RMD abc",false, strFileList);
	curl_easy_cleanup(m_curl);
```

