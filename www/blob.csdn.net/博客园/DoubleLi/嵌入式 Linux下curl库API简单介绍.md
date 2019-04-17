# 嵌入式 Linux下curl库API简单介绍 - DoubleLi - 博客园






1：CURLcode curl_global_init(long flags);

    这个函数全局需要调用一次（多次调用也可以，不过没有必要）， 所以这也是把Curlplus设计成单体类的原因，curl_global_init函数在其他libcurl函数调用前至少调用一次，程序最后需要调用curl_global_cleanup,进行清理。

参数：flags 

CURL_GLOBAL_ALL Initialize everything possible. This sets all known bits.

CURL_GLOBAL_SSL Initialize SSL

CURL_GLOBAL_WIN32 Initialize the Win32 socket libraries.

CURL_GLOBAL_NOTHING Initialise nothing extra. This sets no bit.



CURLcode 是一个enum，当CURLcode为CURLE_OK时，表示函数执行成功，否则失败，具体错误原因可以查看<curl/curl.h>文件内的定义。



2：curl_easy_init() - Start a libcurl easy session

curl_easy_init用来初始化一个CURL的指针(有些像返回FILE类型的指针一样). 相应的在调用结束时要用curl_easy_cleanup函数清理. 一般curl_easy_init意味着一个会话的开始. 它的返回值是CURL *，curl_easy_init函数是线程相关的，也就是说不能在一个线程中调用另外一个线程通过curl_easy_init创建的CURL指针。



3：CURLcode curl_easy_setopt(CURL *handle, CURLoption option, parameter); 

描述: 这个函数最重要了.几乎所有的curl 程序都要频繁的使用它.它告诉curl库.程序将有如何的行为. 比如要查看一个网页的html代码等.，要想具体了解CURL的行为，必须对CURLoption有足够的了解，具体可以参考:

http://curl.haxx.se/libcurl/c/curl_easy_setopt.html



这里有两个类型不易理解CURLOPT_WRITEFUNCTION，CURLOPT_WRITEDATA

curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Curlplus::writer);



设置一个回调函数，这个回调函数的格式是

size_t function( void *ptr, size_t size, size_t nmemb, void *stream);

ptr,返回数据的指针

size,返回数据每块的大小

nmemb,返回数据的块数（这里返回数据串的真正大小为size*nmemb）

stream，是curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer); 中的buffer的指针。

在上面的例子中，buffer设置为一个string对象，所以，在回调函数writer中有了writerData->append(data, len); 



4：CURLcode curl_easy_perform(CURL *handle);

执行远程请求



参考资料

http://curl.haxx.se/

http://curl.haxx.se/lxr/source/docs/examples/



基于curl 的C API写了一个扩展C++ singleton类（当然curl也有C++ API），这个单体类只是对HTTP请求做了简单封装，提供post，get方法，并得到请求url内的返回值（保存到string对象中），也很容易扩展到其他协议上去。

Curlplus.h文件

#ifndef _CURLPLUS_H__

#define _CURLPLUS_H__ 



#ifndef __CURL_CURL_H

#include <curl/curl.h>

#endif



#ifndef __CURL_EASY_H

#include <curl/easy.h>

#endif



#include <memory>

#include <string> 



using namespace::std; 



namespace CP

{

    class Curlplus

    {

    public:

        static  Curlplus& get_instance();

        string post(const string& url,const string& content) const;

        string get(const string& url) const;

    protected:

        Curlplus(void);

        ~Curlplus(void);

        Curlplus(const Curlplus&);

        Curlplus& operator=(const Curlplus&);

        static int writer(char *data, size_t size, size_t nmemb,std::string *writerData);

    private:

        static auto_ptr<Curlplus> _instance;

        inline void _setCurlopt(CURL *curl,const string& url) const;

        // default timeout 300s

        static const int _defaulttimeout = 300;

        static string buffer;

        friend class auto_ptr<Curlplus>;

    };

}



#endif 



Curlpuls.cc文件 



#ifndef SPIVOT_CURLPLUS_H__

#include "Curlplus.h"

#endif 



using namespace std;

using namespace CP;





auto_ptr<Curlplus> Curlplus::_instance;

string Curlplus::buffer;

static char errorBuffer[CURL_ERROR_SIZE]; 



Curlplus& Curlplus::get_instance()

{

    if(_instance.get() == NULL)

    {

        _instance.reset(new Curlplus());

    } 



    return *_instance.get();

}



int Curlplus::writer(char *data, size_t size, size_t nmemb, string *writerData)

{

    if (writerData == NULL)

        return 0;

    int len = size*nmemb;

    writerData->append(data, len);



    return len;

}



Curlplus::Curlplus(void)

{ 



    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);

    if(code != CURLE_OK)

    {

        cout << "curl_init failed, error code is: " << code;

    }

}



Curlplus::~Curlplus(void)

{

    curl_global_cleanup();

} 



string Curlplus::post(const string& url, const string& content) const

{

    buffer="";

    CURL *curl = curl_easy_init();

    if(curl == NULL)

    {

        cout << "curl_easy_init failed ";

    }  



    _setCurlopt(curl,url);

    curl_easy_setopt(curl, CURLOPT_POST, 1 );

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());   



    CURLcode code = curl_easy_perform(curl);

    if(code != CURLE_OK)

    {

        cout << "curl_easy_perform failed: "<< code;

    }

    curl_easy_cleanup(curl);



    return buffer;

} 



string Curlplus::get(const string& url) const

{

    buffer="";

    CURL *curl = curl_easy_init();

    if(curl == NULL)

    {

        cout << "curl_easy_init failed ";

    }  



    _setCurlopt(curl,url);

    CURLcode code = curl_easy_perform(curl); 



    if(code != CURLE_OK)

    {

        cout << "curl_easy_perform failed: "<< code;

    }

    curl_easy_cleanup(curl);

    return buffer;

}



void Curlplus::_setCurlopt(CURL *curl,const string& url) const {

    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0);

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, _defaulttimeout);

    //curl_easy_setopt(curl, CURLOPT_VERBOSE, true);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Curlplus::writer);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);



}









