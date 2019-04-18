# Windows下使用websocketpp - 深之JohnChen的专栏 - CSDN博客

2018年11月29日 19:16:40[byxdaz](https://me.csdn.net/byxdaz)阅读数：878


WebSocket protocol 是HTML5一种新的协议，它是实现了浏览器与服务器全双工通信。WebSocket协议解析参考这篇文章[http://www.cnblogs.com/chyingp/p/websocket-deep-in.html](http://www.cnblogs.com/chyingp/p/websocket-deep-in.html)

一、下载websocketpp、boost、openssl

WebSocketpp只是一个库，本身不需要搭建什么环境，只要新建的项目引入相关的库就行。但是WebSocketpp依赖于boost库和openssl库，这两个库需要编译生成lib库文件，在使用WebSocketpp库的工程中，需要引入boost库和openssl库的头文件和lib文件。openssl库是可选的。

https://github.com/zaphoyd/websocketpp

https://sourceforge.net/projects/boost/files/boost/1.62.0/ 下载boost最新版本1.62.0

https://github.com/openssl/openssl/releases下载openssl1.1.0版本

websocket使用wss协议,使用websocketpp库时，需要使用openssl库支持。

在工程属性中添加openssl头文件目录引用，增加openssl库文件目录引用。

在工程代码中添加对openssl部份引用:

#pragma comment(lib, "libeay32.lib")

#pragma comment(lib, "ssleay32.lib")

二、新建一个vs工程，书写websocket相关代码，配置工程属性。

第一步，把boost的根目录添加到include。

第二步，把websoketpp的子目录添加到include，比如将websocketpp-0.8.1中websoketpp目录拷贝本工程中，同时添加到include

第三步，把boost/stage/lib添加到静态库。

websocketpp-0.8.1中examples目录中有很少实例代码，可以参考。

![](https://img-blog.csdnimg.cn/20181129191329357.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2J5eGRheg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181129191241660.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2J5eGRheg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181130111115328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2J5eGRheg==,size_16,color_FFFFFF,t_70)

1、出现错误    C4996    'std::copy::_Unchecked_iterators::_Deprecate': Call to 'std::copy' with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'    WebSocketTest    c:\program files (x86)\microsoft visual studio 14.0\vc\include\xutility    2372    

解决方法：添加参数“_SCL_SECURE_NO_WARNINGS”（SCL安全无警告，S代表Safety 安全、C代表Check检查、L代表List清单）到预处理器定义中，不警告这种不安全的操作方式，方法如下：

项目->属性->配置属性->C/C++->预处理器->预处理器定义（末尾加上“_SCL_SECURE_NO_WARNINGS”即可（注意分号隔开））

2、在ie9-ie11浏览器测试成功。在edge浏览器中测试，发现不成功。在chrome浏览器中测试成功。

3、tcp连接建立之后接受数据，解析http请求websocket upgrade命令，成功失败做对应的处理，参考connection_impl.hpp文件

connection_impl.hpp文件的handle_read_http_response函数解析客户端数据。

3.1）http请求是websocket upgrade命令，执行log_open_result

        3.1.1）执行open handler回调函数，在此函数函数中可以获取到客户端的请求方法、命令、参数等。

        3.1.2）handle_read_frame读取帧数据

3.2）http请求非websocket upgrade命令，返回错误，terminate关闭连接。

三、服务器实例代码

```
// WebSocketTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <list>

#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

//名称与值数据对
struct NameAndValue
{
	std::string strName;
	std::string strValue;
};
// 字符串分割
int StringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator);
//去前后空格
std::string& StringTrim(std::string &str);
//获取请求命令与参数
bool GetReqeustCommandAndParmeter(std::string strUri, std::string & strRequestOperateCommand, std::list<NameAndValue> & listRequestOperateParameter);

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

bool validate(server *, websocketpp::connection_hdl) {
	//sleep(6);
	return true;
}

void on_http(server* s, websocketpp::connection_hdl hdl) {
	server::connection_ptr con = s->get_con_from_hdl(hdl);

	std::string res = con->get_request_body();

	std::stringstream ss;
	ss << "got HTTP request with " << res.size() << " bytes of body data.";

	con->set_body(ss.str());
	con->set_status(websocketpp::http::status_code::ok);
}

void on_fail(server* s, websocketpp::connection_hdl hdl) {
	server::connection_ptr con = s->get_con_from_hdl(hdl);

	std::cout << "Fail handler: " << con->get_ec() << " " << con->get_ec().message() << std::endl;
}

void on_open(server* s, websocketpp::connection_hdl hdl) {
	//申请websocket upgrade成功之后，调用open_handler函数，回调on_open。
	//在这里，可以获取http请求的地址、参数信息。
	std::cout << "open handler" << std::endl;
	/*
	server::connection_ptr con = s->get_con_from_hdl(hdl);
	websocketpp::config::core::request_type requestClient = con->get_request();
	std::string strMethod = requestClient.get_method();		//请求方法
	std::string strUri = requestClient.get_uri();			//请求uri地址，可以解析参数
	std::string strRequestOperateCommand = "";				//操作类型
	std::list<NameAndValue> listRequestOperateParameter;	//操作参数列表	
	GetReqeustCommandAndParmeter(strUri, strRequestOperateCommand, listRequestOperateParameter);
	std::cout << "command:" << strRequestOperateCommand << std::endl;
	*/
}

void on_close(websocketpp::connection_hdl hdl) {
	std::cout << "Close handler" << std::endl;
}

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	/*
		hdl.lock().get() 获得连接标识
		msg->get_payload() 是收到的消息内容
		msg->get_opcode() 是收到消息的类型 ，包含：文本TEXT,二进制BINARY等等
	*/
	std::cout << "on_message called with hdl: " << hdl.lock().get()
		<< " and message: " << msg->get_payload()
		<< std::endl;

	try {
		/*
			发送消息
			s->send(
			hdl, //连接
			msg->get_payload(), //消息
			msg->get_opcode());//消息类型
		*/
		s->send(hdl, msg->get_payload(), msg->get_opcode());
	}
	catch (websocketpp::exception const & e) {
		std::cout << "Echo failed because: "
			<< "(" << e.what() << ")" << std::endl;
	}
}

int main() {
	server print_server;
	try {
		// Set logging settings
		print_server.set_access_channels(websocketpp::log::alevel::all);
		print_server.set_error_channels(websocketpp::log::elevel::all);
		//print_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Register our message handler
		print_server.set_message_handler(bind(&on_message, &print_server, ::_1, ::_2));
		print_server.set_http_handler(bind(&on_http, &print_server, ::_1));
		print_server.set_fail_handler(bind(&on_fail, &print_server, ::_1));
		print_server.set_open_handler(bind(&on_open, &print_server, ::_1));
		print_server.set_close_handler(bind(&on_close, ::_1));
		print_server.set_validate_handler(bind(&validate, &print_server, ::_1));

		// Initialize ASIO
		print_server.init_asio();
		print_server.set_reuse_addr(true);

		// Listen on port 9100
		print_server.listen(9100);
		// Start the server accept loop
		print_server.start_accept();

		// Start the ASIO io_service run loop
		print_server.run();

		//stop
		print_server.stop();
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	}
	catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "other exception" << std::endl;
	}
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
		if (temp.length() > 0) {
			dst.push_back(temp);
			nCount++;
		}
		offset = pos + 1;
	}

	// 分割第n个
	temp = src.substr(offset, src.length() - offset);
	if (temp.length() > 0) {
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
//获取请求命令与参数
bool GetReqeustCommandAndParmeter(std::string strUri, std::string & strRequestOperateCommand, std::list<NameAndValue> & listRequestOperateParameter)
{
	bool bRet = false;
	std::vector<std::string> vecRequest;
	int nRetSplit = StringSplit(vecRequest, strUri, "?");
	if (nRetSplit > 0)
	{
		if (vecRequest.size() == 1)
		{
			strRequestOperateCommand = vecRequest[0];
		}
		else if (vecRequest.size() > 1)
		{
			strRequestOperateCommand = vecRequest[0];
			std::string strRequestParameter = vecRequest[1];
			std::vector<std::string> vecParams;
			nRetSplit = StringSplit(vecParams, strRequestParameter, "&");
			if (nRetSplit > 0)
			{
				std::vector<std::string>::iterator iter, iterEnd;
				iter = vecParams.begin();
				iterEnd = vecParams.end();
				for (iter; iter != iterEnd; iter++)
				{
					std::vector<std::string> vecNameOrValue;
					nRetSplit = StringSplit(vecNameOrValue, *iter, "=");
					if (nRetSplit > 0)
					{
						NameAndValue nvNameAndValue;
						nvNameAndValue.strName = vecNameOrValue[0];
						nvNameAndValue.strValue = "";
						if (vecNameOrValue.size() > 1)
						{
							nvNameAndValue.strValue = vecNameOrValue[1];
						}
						//insert
						listRequestOperateParameter.push_back(nvNameAndValue);
					}
				}
			}
		}
		else
		{

		}
	}
	return bRet;
}
```

四、web客户端实例代码

test.html

```
<!DOCTYPE html>    
<html>    

<head>    

<meta charset="UTF-8">  

</head>    

<body>    

    <div>    

        <input type="submit" value="Start" onclick="startSendDataToServer()" />    

    </div>    

    <div id="messages"></div>    

    <script type="text/javascript">    

        //var webSocket =  new WebSocket('ws://localhost:9100/websocket?userid=admin');  

        var webSocket =  new WebSocket('ws://localhost:9100');  

    

        webSocket.onerror = function(event) {    

            onError(event)    

        };    

    

        webSocket.onopen = function(event) {    

            onOpen(event)    

        };    

    

        webSocket.onmessage = function(event) {    

            onMessage(event)    

        };    

    

        function onMessage(event) {    

            document.getElementById('messages').innerHTML     

                += '<br />Get Message From Server: ' + event.data;    

        }    

    

        function onOpen(event) {    

            document.getElementById('messages').innerHTML     

                = 'Connection established';    

        }    

    

        function onError(event) {    

            alert(event.data);    

            alert("error");    

        }    

    

        function startSendDataToServer() {
            webSocket.send('hello');    
            return false;    

        }    

    </script>    

</body>    

</html>
```

[代码下载](https://download.csdn.net/download/byxdaz/10817641)

WebSocket在线测试工具：[http://ws.douqq.com/](http://ws.douqq.com/)

