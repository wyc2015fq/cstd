#pragma once
#include "string"
using namespace std;

const int POST_FILE_MAX_COUNT = 5;
const int POST_VALUES_MAX_COUNT = 10;
struct HttpParam
{
	std::string strName;
	std::string strValue;
	HttpParam()
	{
		strName = "";
		strValue = "";
	}
};
struct HttpPostFileParam
{
	HttpParam cFileParam[POST_FILE_MAX_COUNT];
	HttpParam cValueParam[POST_VALUES_MAX_COUNT];
};

class RongyiCHttpClient
{
public:
	RongyiCHttpClient(void);
	virtual ~RongyiCHttpClient(void);

	static void Init();
	static void DeInit();

	/**
	* @brief HTTP POST请求
	* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
	* @param strPost 输入参数,使用如下格式para1=val2&…
	* @param strResponse 输出参数,返回的内容
	* @return 返回是否Post成功
	*/
	int Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);

	/**
	* @brief HTTP GET请求
	* @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com
	* @param strResponse 输出参数,返回的内容
	* @return 返回是否Post成功
	*/
	int Get(const std::string & strUrl, std::string & strResponse);

	/**
	* @brief HTTPS POST请求,无证书版本
	* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
	* @param strPost 输入参数,使用如下格式para1=val1&…
	* @param strResponse 输出参数,返回的内容
	* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	* @return 返回是否Post成功
	*/
	int Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath = NULL);

	/**
	* @brief HTTPS GET请求,无证书版本
	* @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com
	* @param strResponse 输出参数,返回的内容
	* @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性.
	* @return 返回是否Post成功
	*/
	int Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath = NULL);


	int BrokenDownload(const std::string & strUrl, const std::string& strFileName);

	int Download(const std::string & strUrl, const std::string& strFileName);

	int http_post_file(const char *url, const HttpPostFileParam& cParam, std::string& strResponse, unsigned dwSendSpeedKB = 0);
};
