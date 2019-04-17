# HTTP服务端JSON服务端 - DoubleLi - 博客园






# HTTP服务端JSON服务端

**最后更新日期:**  2014-5-18

**Author: **Kagula

**阅读前提：** CMake工具的基本使用

**内容简介：**

   CPPCMS是个开源Web开发框架，通过它可以很容易实现HTTP服务和JSON服务，这里介绍CPPCMS开发环境的搭建。写一个CPPCMS[测试](http://lib.csdn.net/base/softwaretest)程序，它建立HTTP服务，向浏览器返回Hello,World页面。CPPCMS依赖的一些第三方库，其它地方已经介绍怎么Build，所以这里不重复了。

**环境：**Windows8.1 64bit、Visual Studio 2013 Professional SP1

zlib-1.2.8、openssl-1.0.1g、pcre-8.35、icu4c-53_1、  cppcms-1.0.4、[Python](http://lib.csdn.net/base/python)-3.4.0.、CMake2.8.12.2、

boost 1.55



**搭建CPPCMS开发环境**

CPPCMS依赖zlib、openssl、pcre、icu4c、[python](http://lib.csdn.net/base/python)和Win SDK等第三方库或工具



         在python官网下载、安装python-3.4.0.amd64.msi，不要忘记在向导中勾选，把python.exe的位置加入到系统的环境变量中。



我电脑中编译好的库，它们的位置

**zlib**

头文件位置：D:/SDK/zlib-1.2.8;D:\SDK\zlib-1.2.8\build;

库文件位置：D:/SDK/zlib-1.2.8/build/Release



**openssl**

头文件位置：D:\SDK\openssl-1.0.1g\include

库文件位置：D:\SDK\openssl-1.0.1g\out32dll



**icu4c**

         下载http://download.icu-project.org/files/icu4c/53.1/icu4c-53_1-src.zip文件

打开D:\SDK\icu4c-53_1-src\icu\source\allinone\allinone.sln文件，Build出Release版本即可。

头文件位置：D:\SDK\icu4c-53_1-src\icu\include

库文件位置：;D:\SDK\icu4c-53_1-src\icu\lib



**pcre**

         下载ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre-8.35.zip

         使用CMake工具在D:\SDK\pcre-8.35\build\下生成PCRE.sln文件，使用VisualStudio 2013打开并编译即可。

头文件位置：D:/SDK/pcre-8.35;D:\SDK\pcre-8.35\build;

库文件位置：D:\SDK\pcre-8.35\build\Release



**准备好后就可以构建cppcms库了**



从cppcms官网下载cppcms-1.0.4.tar.bz2文件并解压缩到“D:\SDK\cppcms-1.0.4”，打开cmake gui工具configure后，出现了很多变量

添加变量

CMAKE_INCLUDE_PATH

类型为filepath

值为

D:/SDK/pcre-8.35;D:\SDK\pcre-8.35\build;D:/SDK/zlib-1.2.8;D:\SDK\zlib-1.2.8\build;D:\SDK\icu4c-53_1-src\icu\include;D:\SDK\openssl-1.0.1g\include

添加变量

CMAKE_LIBRARY_PATH

类型为filepath

值为D:\SDK\pcre-8.35\build\Release;D:/SDK/zlib-1.2.8/build/Release;D:\SDK\icu4c-53_1-src\icu\lib;D:\SDK\openssl-1.0.1g\out32dll;C:\ProgramFiles (x86)\Microsoft SDKs\Windows\v7.1A\Lib

         重新configure后generate。打开新生成的sln文件。BuildSolution有四个选项Debug、MinSizeRel(最小发行版)、Release、RelWithDebInfo(带Debug信息的发行版)，我们这里选Release模式build。



**编译过程中碰到**

[1]zconf.h文件找不到的问题，这个文件在我的D:\SDK\zlib-1.2.8\build路径下，给它地址，这个问题解决。

[2]json.cpp 文件 return is_ 代码行通不过， 改为returnbool(is_)即可，因为Visual Studio 2013([C++](http://lib.csdn.net/base/cplusplus)11标准)要求istream显式(explicitly)转换bool类型。

[3]test_formatting.cpp和test_boundary.cpp源码文件因为出现特殊字符（其实是文件语言编码的问题）导致无法编译，跳过，因为这两个测试项目不能编译，不影响我们以后使用cppcms库。

[4]修改D:\SDK\cppcms-1.0.4\booster\booster\nowide\fstream.h文件第44行，

if(my_base_type::open(convert(s).c_str(),mode)){

修改为

if (std::basic_filebuf<CharType,Traits>::open(s, mode)) {

否则调用CPPCMS的Debug版会挂掉。

[5]由于CPPCMS的Release版本application类的main方法std::string实参释放时会出错，所以我把它改成const char *类型，下面三步是具体步骤

[5-1]“D:\SDK\cppcms-1.0.4\cppcms\application.h”文件第298行

virtual void main(std::string url);

修改为

virtual void main(const char* url);

[5-2]“D:\SDK\cppcms-1.0.4\src\application.cpp”文件第200行

void application::main(std::string  url)

修改为

void application::main(const char* url)

[5-3]“D:\SDK\cppcms-1.0.4\src\url_dispatcher.cpp”文件第49行

[6]修改json_rpc_server相关文件，使json服务可用

[6-1]

“D:\SDK\cppcms-1.0.4\cppcms\rpc_json.h”152行

virtual void main(std::string);

改为

virtual void main(const char*);

 [6-2]“D:\SDK\cppcms-1.0.4\src\rpc_json.cpp”148行

void json_rpc_server::main(std::string/*unused*/)

改为

void json_rpc_server::main(const char*/*unused*/)



app_->main(match_[select_]);

修改为

std::string tmp = match_[select_];

app_->main(tmp.c_str());

现在rebuild整个solution, 等待几分钟后，Visual studio 2013提示77个成功，2个失败（刚才说的文件语言编码问题），3个跳过。这样就可以了。Build后的cppcms.dll、cppcms.lib文件在...\cppcms-1.0.4\build\Release目录下可以找到。



         下面我们用CPPCMS写一个HelloWorld程序

## Hello World

    构建我们的第一个例子时使用了boost库用来把GBK字符串转utf字符串，我计算机的boost库安装路径是“D:\SDK\boost_1_55_0”。



         在Visual Studio中新建Win32 控制台项目，AdditonalOptions选项Empty Project。

设置头文件搜索路径

D:\SDK\cppcms-1.0.4

D:\SDK\cppcms-1.0.4\build

D:\SDK\cppcms-1.0.4\booster

D:\SDK\cppcms-1.0.4\build\booster

D:\SDK\boost_1_55_0



设置库文件搜索路径

D:\SDK\cppcms-1.0.4\build\booster\Release

D:\SDK\cppcms-1.0.4\build\Release

D:\SDK\boost_1_55_0\stage\lib



添加链接库cppcms.lib



复制下面7个动态链接库到你项目文件夹中，否则会提示找不到这些DLL文件

D:\SDK\pcre-8.35\build\Release\pcre.dll

D:\SDK\zlib-1.2.8\build\Release\zlib.dll

D:\SDK\cppcms-1.0.4\build\Release\cppcms.dll

D:\SDK\cppcms-1.0.4\build\booster\Release\booster.dll

D:\SDK\icu4c-53_1-src\icu\bin\icuuc53.dll

D:\SDK\icu4c-53_1-src\icu\bin\icudt53.dll

D:\SDK\icu4c-53_1-src\icu\bin\icuin53.dll

添加Source.cpp文件，源代码清单如下





**[cpp]**[view plain](http://blog.csdn.net/lee353086/article/details/40350063#)[copy](http://blog.csdn.net/lee353086/article/details/40350063#)



- #include <cppcms/application.h>  
- #include <cppcms/applications_pool.h>  
- #include <cppcms/service.h>  
- #include <cppcms/http_response.h>  
- 
- #include <boost/locale.hpp>  
- 
- class hello : public cppcms::application {  
- public:  
-     hello(cppcms::service &srv) :  
-         cppcms::application(srv)  
-     {  
-     }  
- void main(const char* url);  
- };  
- 
- void hello::main(const char* url)  
- {  
- //GBK转utf  
-     std::string  utfStr =   
-         boost::locale::conv::to_utf<char>("<h1>中文测试</h1>\n", "GBK");  
- 
- //输出  
-     response().out() <<  
- "<html>\n"  
- "<body>\n"  
-         <<utfStr<<  
- "</body>\n"  
- "</html>\n";  
- }  
- 
- 
- int main(int argc, char ** argv)  
- {  
- try  
-     {  
- /* 
-         命令行参数为 -c configure.js 
-         让CPPCMS读取configure.js里我们的配置信息 
-         */  
-         cppcms::service srv(argc, argv);  
- 
- //挂载用户的Web服务，application的继承类hello  
-         srv.applications_pool().mount(cppcms::applications_factory<hello>());  
- 
- //这里阻塞，等待用户请求  
-         srv.run();  
-     }  
- catch (std::exception const &e)  
-     {  
-         std::cerr << e.what() << std::endl;  
-     }  
- }  






添加configure.[js](http://lib.csdn.net/base/javascript)文件，源代码清单如下





**[javascript]**[view plain](http://blog.csdn.net/lee353086/article/details/40350063#)[copy](http://blog.csdn.net/lee353086/article/details/40350063#)



- {    
- "service" : {    
- "api" : "http",    
- "port" : 8080    
-     },    
- "http" : {    
- "script_names" : [ "/hello" ]    
-     }    
- }    






按[F5]以Debug方式运行即可。

现在你可以使用http://localhost:8080/这个地址访问你的HTTP服务器了。

## JSon服务器例子

         我测试了下面这个链接中贴出的代码，可以用，但是如果你的测试程序是在**Debug**模式，你只能调用**debug模式编译出来的CPPCMS动态库**，如果你是在**Release模式**，就只能调用**Release模式编译出来的CPPCMS动态库**，否则会抛出“bad allocation”的错误。



 服务端代码





**[cpp]**[view plain](http://blog.csdn.net/lee353086/article/details/40350063#)[copy](http://blog.csdn.net/lee353086/article/details/40350063#)



- #include <cppcms/application.h>  
- #include <cppcms/applications_pool.h>  
- #include <cppcms/service.h>  
- #include <cppcms/http_response.h>  
- #include <cppcms/rpc_json.h>  
- #include <cppcms/json.h>  
- #include <cppcms/mount_point.h>  
- #include <map>  
- #include <string>  
- 
- #include <exception>  
- #include <iostream>  
- 
- /* 
- 标题：测试CPPCMS提供的JSON_RPC服务功能 
- 来源： 
- 使用cppcms开发JSON_RPC服务 
- http://www.zeuux.com/group/candcplus/bbs/content/55785/ 
- 
- */  
- using namespace std;  
- using cppcms::rpc::json_rpc_server;  
- using cppcms::rpc::json_method;  
- 
- class calc_service : public json_rpc_server {  
- public:  
-     calc_service(cppcms::service &srv) : json_rpc_server(srv) {  
-         bind("sum",  
-             json_method(&calc_service::sum, this), method_role);  
-         bind("div",  
-             json_method(&calc_service::div, this), method_role);  
-     }  
- void sum(int x, int y) {  
-         cppcms::json::value v;  
-         v["x"] = x;  
-         v["y"] = y;  
-         v["result"] = x + y;  
-         return_result(v);  
-     }  
- void div(int x, int y) {  
- if (y == 0) {  
-             return_error("Division by zero.");  
-         }  
- else {  
-             return_result(x / y);  
-         }  
-     }  
- };  
- 
- class hello_service : public json_rpc_server {  
- public:  
-     hello_service(cppcms::service &srv) : json_rpc_server(srv) {  
-         bind("hello",  
-             json_method(&hello_service::hello, this), method_role);  
-         bind("hello_all",  
-             json_method(&hello_service::hello_all, this), method_role);  
-         bind("hello_x",  
-             json_method(&hello_service::hello_x, this), method_role);  
-     }  
- void hello(string name) {  
-         string say = "Hello, " + name + ".";  
-         return_result(say);  
-     }  
- void hello_all(vector<string> names) {  
-         string say = "Hello, ";  
- for (unsigned i = 0; i<names.size(); i++) {  
-             say += names[i] + " ";  
-         }  
-         return_result(say);  
-     }  
- void hello_x(cppcms::json::value val) {  
-         return_result(val);  
-     }  
- };  
- 
- int main(int argc, char ** argv)  
- {  
- try {  
-         cppcms::service srv(argc, argv);  
- 
-         srv.applications_pool().mount(  
-             cppcms::applications_factory<calc_service>(),  
-             cppcms::mount_point("/calc")  
-             );  
- 
-         srv.applications_pool().mount(  
-             cppcms::applications_factory<hello_service>(),  
-             cppcms::mount_point("/hello")  
-             );  
-         srv.run();  
-     }  
- catch (exception const &e) {  
-         cerr << e.what() << endl;  
-     }  
- return 0;  
- }  








测试服务端代码的html源码





**[cpp]**[view plain](http://blog.csdn.net/lee353086/article/details/40350063#)[copy](http://blog.csdn.net/lee353086/article/details/40350063#)



- <html>  
- <body>  
-     <script type="text/javascript">  
-         function call() {  
- 
-             var xhr = new XMLHttpRequest();  
-             xhr.open("post", 'http://localhost:8080/calc');  
- // Required by JSON-RPC over HTTP  
-             xhr.setRequestHeader("Content-Type", "application/json");  
- 
- // It is better to use real formatter like JSON.js  
-             x = parseInt(document.getElementById('x').value);  
-             y = parseInt(document.getElementById('y').value);  
-             var request = '{"method":"div","params":[' + x + ',' + y + '],"id":1}';  
- 
-             xhr.onreadystatechange = function () {  
- if (xhr.readyState === 4) {  
-                     res = 'Unknown Error';  
- if (xhr.status === 200) {  
- // Don't call eval in real code use some parser  
-                         var result = eval('(' + xhr.responseText + ')');  
- if (result.error == null) {  
-                             res = result.result;  
-                         }  
- else {  
-                             res = result.error;  
-                         }  
-                     }  
-                     document.getElementById('result').innerHTML = res;  
-                 }  
-             }  
-             xhr.send(request);  
- return false;  
-         }  
-     </script>  
-     <form onsubmit="return call();">  
-         <p>  
-             <input type="text" id="x" />  
-             <input type="submit" value="/" />  
-             <input type="text" id="y" /> =  
-             <span id="result"></span>  
-         </p>  
-     </form>  
- </body>  
- </html>  



configure.js代码







**[javascript]**[view plain](http://blog.csdn.net/lee353086/article/details/40350063#)[copy](http://blog.csdn.net/lee353086/article/details/40350063#)



- {  
- "service" : {  
- "api" : "http",  
- "port" : 8080,  
-         },  
- "http" : {  
- "script_names" : [ "/calc","/hello" ]  
-     }  
- }  








## 补充阅读资料

CPPCMS在[Linux](http://lib.csdn.net/base/linux)下的编译和安装

http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/csfreebird/article/details/6730623



官网的Hello World例子

http://cppcms.com/wikipp/en/page/cppcms_1x_tut_hello



深入学习 CppCMS

http://remonstrate.wordpress.com/2012/04/09/%E6%B7%B1%E5%85%A5%E5%AD%A6%E4%B9%A0-cppcms/



CppCMS和Nginx协同工作

http://www.cnblogs.com/believeit/archive/2011/09/03/2183531.html



CppCMS支持文件上传

http://www.oschina[.net](http://lib.csdn.net/base/dotnet)/question/565065_66895



boost库学习随记五 Boost.Locale 之字符转换 gbkutf8 big5 string wstring等

http://blog.csdn[.Net](http://lib.csdn.net/base/dotnet)/leitianjun/article/details/24658655



How to Build libiconv with Microsoft VisualStudio

http://www.codeproject.com/Articles/302012/How-to-Build-libiconv-with-Microsoft-Visual-Studio

注意，IE6不支持JSON_RPC技术。









