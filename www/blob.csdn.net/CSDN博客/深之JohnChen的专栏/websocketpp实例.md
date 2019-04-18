# websocketpp实例 - 深之JohnChen的专栏 - CSDN博客

2018年12月03日 16:04:47[byxdaz](https://me.csdn.net/byxdaz)阅读数：831


//服务端

```
//WebSocketServerOpreate.h

#pragma once

#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <boost/thread.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#define			Base_Uri_Length			128

//名称与值数据对
struct NameAndValue
{
	std::string strName;
	std::string strValue;
};

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

class WebSocketServerOpreate
{
public:
	WebSocketServerOpreate();
	~WebSocketServerOpreate();

	int Init(unsigned short usPort,char *pBaseUri="/ws");
	int Uninit();
	int StartWork();
	int StopWork();

protected:
	int ThreadProccess();
	void InsertClientConnection(websocketpp::connection_hdl hdl);
	void DeleteClientConnection(websocketpp::connection_hdl hdl);

public:
	bool validate(server *s, websocketpp::connection_hdl hdl);
	void on_http(server* s, websocketpp::connection_hdl hdl);
	void on_fail(server* s, websocketpp::connection_hdl hdl);
	//连接打开回调函数
	void on_open(server* s, websocketpp::connection_hdl hdl);
	//连接关闭回调函数
	void on_close(server *s, websocketpp::connection_hdl hdl);
	//接收websocket数据回调函数
	void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);

public:
	// 字符串分割
	static int StringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator);
	//去前后空格
	static std::string& StringTrim(std::string &str);
	//获取请求命令与参数
	static bool GetReqeustCommandAndParmeter(std::string strUri, std::string & strRequestOperateCommand, std::vector<NameAndValue> & listRequestOperateParameter);

protected:
	unsigned short m_usPort;
	char m_szBaseUri[Base_Uri_Length];
	server m_server;
	boost::thread * m_threadMain;
	bool m_bThreadExit;
	std::list<websocketpp::connection_hdl> m_listClientConnection;
};

//WebSocketServerOpreate.cpp

#include "stdafx.h"
#include "WebSocketServerOpreate.h"

#include "jsoncpp/include/json/json.h"

#ifdef _DEBUG
#pragma comment(lib,"jsoncpp/lib/debug/lib_json.lib")
#else
#pragma comment(lib,"jsoncpp/lib/release/lib_json.lib")
#endif

WebSocketServerOpreate::WebSocketServerOpreate()
{
	m_usPort = 9100;
	memset(m_szBaseUri, 0, sizeof(m_szBaseUri));
	m_bThreadExit = true;
	m_threadMain = NULL;
	m_listClientConnection.clear();
}

WebSocketServerOpreate::~WebSocketServerOpreate()
{
	if (m_threadMain != NULL)
	{
		delete m_threadMain;
		m_threadMain = NULL;
	}
}

bool WebSocketServerOpreate::validate(server * s, websocketpp::connection_hdl hdl) {
	//sleep(6);
	return true;
}

void WebSocketServerOpreate::on_http(server* s, websocketpp::connection_hdl hdl) {
	server::connection_ptr con = s->get_con_from_hdl(hdl);

	std::string res = con->get_request_body();

	std::stringstream ss;
	ss << "got HTTP request with " << res.size() << " bytes of body data.";

	con->set_body(ss.str());
	con->set_status(websocketpp::http::status_code::ok);
}

void WebSocketServerOpreate::on_fail(server* s, websocketpp::connection_hdl hdl) {
	server::connection_ptr con = s->get_con_from_hdl(hdl);

	std::cout << "Fail handler: " << con->get_ec() << " " << con->get_ec().message() << std::endl;
}

void WebSocketServerOpreate::on_open(server* s, websocketpp::connection_hdl hdl) {
	//申请websocket upgrade成功之后，调用open_handler函数，回调on_open。
	//在这里，可以获取http请求的地址、参数信息。
	std::cout << "open handler" << std::endl;
	InsertClientConnection(hdl);
	server::connection_ptr con = s->get_con_from_hdl(hdl);
	websocketpp::config::core::request_type requestClient = con->get_request();
	std::string strMethod = requestClient.get_method();		//请求方法
	std::string strUri = requestClient.get_uri();			//请求uri地址，可以解析参数
	std::string strRequestOperateCommand = "";				//操作类型
	std::vector<NameAndValue> listRequestOperateParameter;	//操作参数列表	
	GetReqeustCommandAndParmeter(strUri, strRequestOperateCommand, listRequestOperateParameter);
	std::cout << "command:" << strRequestOperateCommand << std::endl;
	if (strcmp(strRequestOperateCommand.c_str(), m_szBaseUri) == 0)
	{
		if (listRequestOperateParameter.size() >= 2)
		{
			//验证用户和密码，返回登录结果
			if (strcmp(listRequestOperateParameter[0].strValue.c_str(), "admin") == 0 && strcmp(listRequestOperateParameter[1].strValue.c_str(), "admin") == 0)
			{
				Json::Value root;
				root["operatetype"] = 2;					//操作类型，比如登录、登录应答、注销等
				root["ret"] = 0;
				root["sessionid"] = "1110-1111-9999-3333";
				std::string strLoginResponse = root.toStyledString();
				s->send(hdl, strLoginResponse.c_str(), websocketpp::frame::opcode::TEXT);
			}
			else
			{
				Json::Value root;
				root["operatetype"] = 2;					//操作类型，比如登录、登录应答、注销等
				root["ret"] = 1;
				root["sessionid"] = "";
				std::string strLoginResponse = root.toStyledString();
				s->send(hdl, strLoginResponse.c_str(), websocketpp::frame::opcode::TEXT);
			}
		}
		else
		{
			Json::Value root;
			root["operatetype"] = 2;					//操作类型，比如登录、登录应答、注销等
			root["ret"] = 2;
			root["sessionid"] = "";
			std::string strLoginResponse = root.toStyledString();
			s->send(hdl, strLoginResponse.c_str(), websocketpp::frame::opcode::TEXT);
		}
	}
	else
	{
		//error request
	}
}

void WebSocketServerOpreate::on_close(server *s, websocketpp::connection_hdl hdl) {
	std::cout << "Close handler" << std::endl;
	DeleteClientConnection(hdl);
}

// Define a callback to handle incoming messages
void WebSocketServerOpreate::on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
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

int WebSocketServerOpreate::Init(unsigned short usPort, char *pBaseUri)
{
	int nRet = 0;
	m_usPort = usPort;
	strcpy_s(m_szBaseUri, pBaseUri);
	try {
		// Set logging settings
		m_server.set_access_channels(websocketpp::log::alevel::all);
		m_server.set_error_channels(websocketpp::log::elevel::all);
		//m_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Register our message handler
		m_server.set_message_handler(bind(&WebSocketServerOpreate::on_message,this, &m_server, ::_1, ::_2));
		m_server.set_http_handler(bind(&WebSocketServerOpreate::on_http, this, &m_server, ::_1));
		m_server.set_fail_handler(bind(&WebSocketServerOpreate::on_fail, this, &m_server, ::_1));
		m_server.set_open_handler(bind(&WebSocketServerOpreate::on_open, this, &m_server, ::_1));
		m_server.set_close_handler(bind(&WebSocketServerOpreate::on_close, this, &m_server, ::_1));
		m_server.set_validate_handler(bind(&WebSocketServerOpreate::validate, this, &m_server, ::_1));

		// Initialize ASIO
		m_server.init_asio();
		m_server.set_reuse_addr(true);

		// Listen on port
		m_server.listen(m_usPort);
		// Start the server accept loop
		m_server.start_accept();
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
		nRet = -1;
	}
	catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
		nRet = -2;
	}
	catch (...) {
		std::cout << "other exception" << std::endl;
		nRet = -3;
	}
	
	return nRet;
}
int WebSocketServerOpreate::Uninit()
{
	return 0;
}
int WebSocketServerOpreate::StartWork()
{
	m_bThreadExit = false;
	m_threadMain = new boost::thread(boost::bind(&WebSocketServerOpreate::ThreadProccess, this));
	if (m_threadMain == NULL)
	{
		m_bThreadExit = true;
		return -1;
	}
	else
	{
		return 0;
	}
}
int WebSocketServerOpreate::StopWork()
{
	//stop
	m_bThreadExit = true;
	m_server.stop();
	return 0;
}

int WebSocketServerOpreate::ThreadProccess()
{
	while (true)
	{
		if (m_bThreadExit)
		{
			break;
		}

		m_server.poll_one();
		Sleep(100);
	}
	return 0;
}

void WebSocketServerOpreate::InsertClientConnection(websocketpp::connection_hdl hdl)
{
	m_listClientConnection.push_back(hdl);
}
void WebSocketServerOpreate::DeleteClientConnection(websocketpp::connection_hdl hdl)
{
	std::list<websocketpp::connection_hdl>::iterator iter, iterEnd;
	iter = m_listClientConnection.begin();
	iterEnd = m_listClientConnection.end();
	for (iter; iter != iterEnd; iter++)
	{
		server::connection_ptr conInput = m_server.get_con_from_hdl(hdl);
		server::connection_ptr conSrc = m_server.get_con_from_hdl(*iter);
		if (conInput == conInput)
		{
			m_listClientConnection.erase(iter);
			break;
		}
	}
}

// 字符串分割
int WebSocketServerOpreate::StringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator)
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
 std::string& WebSocketServerOpreate::StringTrim(std::string &str)
{
	 if (str.empty()) {
		 return str;
	 }
	 str.erase(0, str.find_first_not_of(" "));
	 str.erase(str.find_last_not_of(" ") + 1);
	 return str;
}
//获取请求命令与参数
bool WebSocketServerOpreate::GetReqeustCommandAndParmeter(std::string strUri, std::string & strRequestOperateCommand, std::vector<NameAndValue> & listRequestOperateParameter)
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

//main.cpp
// WebSocketTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "WebSocketServerOpreate.h"

int main() {
	WebSocketServerOpreate serverWebSocketServerOpreate;
	serverWebSocketServerOpreate.Init(9100);
	serverWebSocketServerOpreate.StartWork();

	while (true)
	{
		Sleep(200);
	}

	serverWebSocketServerOpreate.StopWork();
	serverWebSocketServerOpreate.Uninit();
}
```

//客户端

```
//WebSocketClientOperate.h
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>
#include <boost/thread.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

enum LoginStatus
{
	LoginStatus_LoginSuccessed = 0,			//登录成功
	LoginStatus_LoginFail = 1,					//登录失败
	LoginStatus_NotLogin = 2,					//未登录
	LoginStatus_Logining = 3,					//登录中
};

class WebSocketClientOperate
{
public:
	WebSocketClientOperate();
	virtual ~WebSocketClientOperate();

	int Init(char *pHostname, unsigned short usPort,char *pBaseUri, char *pLoginUsername, char *pLoginPassword);
	int Init(char *pHostname, unsigned short usPort, char *pUri);
	int Uninit();

	int StartWork();
	int StopWork();

protected:
	int login();
	int logout();

	int ThreadProccess();

protected:
	//消息回调函数
	void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg);
	//连接上服务器回调函数
	void on_open(client* c, websocketpp::connection_hdl hdl);
	//服务器断开连接回调函数
	void on_close(client* c, websocketpp::connection_hdl hdl);
	//无法连接上服务器回调函数，socket在内部已关闭上层无需再关闭
	void on_termination_handler(client* c, websocketpp::connection_hdl hdl);

	int closeConnect();
	int  sendTextData(char *pText);
	int  sendBinaryData(unsigned char *pData, int nSize);

protected:
	boost::thread * m_threadMain;
	bool m_bThreadExit;
	bool m_bIsConnectedServer;
	bool m_bSubscribeDataSuccess;
	std::string m_strHostname;
	std::string m_strWSUrl;
	std::string m_strUsername;
	client m_client;
	client::connection_ptr m_connection;
	int m_nLoginStatus;
	std::string m_strLoginSessionId;
	DWORD m_dwLoginRequestTime;
};

//WebSocketClientOperate.cpp
#include "stdafx.h"
#include "WebSocketClientOperate.h"
#include "InnerFunction.h"
#include <mmsystem.h>
#include "jsoncpp/include/json/json.h"

#ifdef _DEBUG
#pragma comment(lib,"jsoncpp/lib/debug/lib_json.lib")
#else
#pragma comment(lib,"jsoncpp/lib/release/lib_json.lib")
#endif
#pragma comment(lib,"winmm.lib")

#define Min_Login_Time				30000

WebSocketClientOperate::WebSocketClientOperate()
{
	m_threadMain = NULL;
	m_bThreadExit = true;
	m_bIsConnectedServer = false;
	m_strHostname.clear();
	m_strWSUrl.clear();
	m_strUsername.clear();
	m_strLoginSessionId.clear();
	m_connection = NULL;
	m_nLoginStatus = LoginStatus_NotLogin;
	m_bSubscribeDataSuccess = false;
	m_dwLoginRequestTime = 0;
}

WebSocketClientOperate::~WebSocketClientOperate()
{
}

int WebSocketClientOperate::Init(char *pHostname, unsigned short usPort, char *pBaseUri, char *pLoginUsername, char *pLoginPassword)
{
	m_strHostname = pHostname;
	m_strUsername = pLoginUsername;
	char szUrl[1024] = { 0 };
	sprintf_s(szUrl, "ws://%s:%d/%s?uid=%s&pwd=%s", pHostname, usPort, pBaseUri, pLoginUsername, pLoginPassword);
	m_strWSUrl = szUrl;
	try {
		// Set logging to be pretty verbose (everything except message payloads)
		m_client.set_access_channels(websocketpp::log::alevel::all);
		m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize ASIO
		m_client.init_asio();
		m_client.set_reuse_addr(true);

		// Register our message handler
		m_client.set_message_handler(bind(&WebSocketClientOperate::on_message, this, &m_client, ::_1, ::_2));
		m_client.set_open_handler(bind(&WebSocketClientOperate::on_open, this, &m_client, ::_1));
		m_client.set_close_handler(bind(&WebSocketClientOperate::on_close, this, &m_client, ::_1));
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
		return -2;
	}

	return 0;
}
int WebSocketClientOperate::Init(char *pHostname, unsigned short usPort, char *pUri)
{
	m_strHostname = pHostname;
	char szUrl[1024] = { 0 };
	sprintf_s(szUrl, "ws://%s:%d/%s", pHostname, usPort, pUri);
	m_strWSUrl = szUrl;
	try {
		// Set logging to be pretty verbose (everything except message payloads)
		m_client.set_access_channels(websocketpp::log::alevel::all);
		m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize ASIO
		m_client.init_asio();
		m_client.set_reuse_addr(true);

		// Register our message handler
		m_client.set_message_handler(bind(&WebSocketClientOperate::on_message, this, &m_client, ::_1, ::_2));
		m_client.set_open_handler(bind(&WebSocketClientOperate::on_open, this, &m_client, ::_1));
		m_client.set_close_handler(bind(&WebSocketClientOperate::on_close, this, &m_client, ::_1));
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
		return -2;
	}

	return 0;
}
int WebSocketClientOperate::Uninit()
{
	return 0;
}

int WebSocketClientOperate::StartWork()
{
	m_bThreadExit = false;
	m_threadMain = new boost::thread(boost::bind(&WebSocketClientOperate::ThreadProccess, this));
	if (m_threadMain == NULL)
	{
		m_bThreadExit = true;
		return -1;
	}
	else
	{
		return 0;
	}
}
int WebSocketClientOperate::StopWork()
{
	int nRet = 0;
	m_bThreadExit = true;
	m_client.stop();
	if (m_threadMain != NULL)
	{
		m_threadMain->join();
		delete m_threadMain;
		m_threadMain = NULL;
	}
	m_nLoginStatus = LoginStatus_NotLogin;
	return nRet;
}

int WebSocketClientOperate::login()
{
	int nRet = 0;
	try {
		//防止频繁登录
		if (m_dwLoginRequestTime != 0 && m_nLoginStatus != LoginStatus_LoginSuccessed)
		{
			DWORD dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - m_dwLoginRequestTime) <= Min_Login_Time)
			{
				return 1;
			}
		}

		m_nLoginStatus = LoginStatus_Logining;
		m_dwLoginRequestTime = timeGetTime();

		websocketpp::lib::error_code ec;
		m_connection = m_client.get_connection(m_strWSUrl, ec);
		if (ec)
		{
			std::cout << "could not create connection because: " << ec.message() << std::endl;
			m_nLoginStatus = LoginStatus_LoginFail;
			return -1;
		}

		// Note that connect here only requests a connection. No network messages are
		// exchanged until the event loop starts running in the next line.
		m_client.connect(m_connection);
		//设置连接断开通知handler
		m_connection->set_termination_handler(bind(&WebSocketClientOperate::on_termination_handler, this, &m_client, ::_1));
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
		m_nLoginStatus = LoginStatus_LoginFail;
		return -2;
	}
	return nRet;
}

int WebSocketClientOperate::logout()
{
	m_nLoginStatus = LoginStatus_NotLogin;
	return -1;
}

int WebSocketClientOperate::ThreadProccess()
{
	while (true)
	{
		if (m_bThreadExit)
		{
			break;
		}

		if (!m_bIsConnectedServer)
		{
			if (m_nLoginStatus == LoginStatus_LoginFail || m_nLoginStatus == LoginStatus_NotLogin)
			{
				login();
			}
		}
		else
		{
			sendTextData("aaaaaa");
		}
		if (m_nLoginStatus == LoginStatus_LoginSuccessed)
		{
			m_client.poll_one();
			Sleep(100);
		}
		else
		{
			//首次连接之后，需要快速握手，不能等待长时间，否则websocket握手失败；websocket握手成功之后，可以等待长时间
			m_client.poll_one();
			Sleep(100);
		}

	}
	return 0;
}

// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void WebSocketClientOperate::on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::cout << "on_message called with hdl: " << hdl.lock().get()
		<< " and message: " << msg->get_payload()
		<< std::endl;

	//不同业务，接收不同数据内容，解析相应的数据并处理
	std::string strRecvice = Utf8toAscii(msg->get_payload());
	Json::Reader reader;
	Json::Value root;
	bool bRet = reader.parse(strRecvice, root);
	if (bRet)
	{
		int nRetErrorCode = root["ret"].asInt();;
		int nOperateType = root["operatetype"].asInt();
		if (nOperateType == 2)									//登录应答
		{
			m_strLoginSessionId = root["sessionid"].asString();
			if (nRetErrorCode == 0)
			{
				//登录成功
				m_nLoginStatus = LoginStatus_LoginSuccessed;
			}
		}
		else
		{

		}
	}
}

void WebSocketClientOperate::on_open(client* c, websocketpp::connection_hdl hdl) {
	std::cout << "open handler" << std::endl;
	client::connection_ptr con = c->get_con_from_hdl(hdl);
	//websocketpp::config::core_client::request_type requestClient = con->get_request();
	if (con->get_ec().value() != 0)
	{
		m_bIsConnectedServer = false;
	}
	else
	{
		m_bIsConnectedServer = true;
	}
}

void WebSocketClientOperate::on_close(client* c, websocketpp::connection_hdl hdl)
{
	closeConnect();
	m_bSubscribeDataSuccess = false;
	m_bIsConnectedServer = false;
	m_nLoginStatus = LoginStatus_NotLogin;
}

void WebSocketClientOperate::on_termination_handler(client* c, websocketpp::connection_hdl hdl)
{
	closeConnect();
	m_nLoginStatus = LoginStatus_LoginFail;
	m_bIsConnectedServer = false;
}

int WebSocketClientOperate::closeConnect()
{
	int nRet = 0;
	try {
		if (m_connection != NULL && m_connection->get_state() == websocketpp::session::state::value::open)
		{
			websocketpp::close::status::value cvValue = 0;
			std::string strReason = "";
			m_connection->close(cvValue, strReason);
		}
	}
	catch (websocketpp::exception const & e)
	{
		std::cout << e.what() << std::endl;
		nRet = -1;
	}
	return nRet;
}

int  WebSocketClientOperate::sendTextData(char *pText)
{
	int nRet = 0;
	try {
		websocketpp::lib::error_code ec;
		ec = m_connection->send(pText);
		if (ec) {
			std::cout << "Echo failed because: " << ec.message() << std::endl;
			nRet = -1;
		}
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
		return -2;
	}

	return nRet;
}

int  WebSocketClientOperate::sendBinaryData(unsigned char *pData, int nSize)
{
	int nRet = 0;
	try {
		websocketpp::lib::error_code ec;
		ec = m_connection->send(pData, nSize, websocketpp::frame::opcode::binary);
		if (ec) {
			std::cout << "Echo failed because: " << ec.message() << std::endl;
			nRet = -1;
		}
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
		return -2;
	}

	return nRet;
}

//main.cpp
#include "stdafx.h"

#include "WebSocketClientOperate.h"

int main(int argc, char* argv[]) {
	WebSocketClientOperate clientWebSocketClientOperate;
	clientWebSocketClientOperate.Init("127.0.0.1", 9100,"ws", "admin", "admin");
	clientWebSocketClientOperate.StartWork();

	while (true)
	{
		Sleep(200);
	}
	return 0;
}
```

