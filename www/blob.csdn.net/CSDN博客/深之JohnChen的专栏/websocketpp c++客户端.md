# websocketpp c++客户端 - 深之JohnChen的专栏 - CSDN博客

2018年11月29日 19:20:37[byxdaz](https://me.csdn.net/byxdaz)阅读数：793


利用websocketpp库编写websocket客户端程序。

一、阻塞方式实例

```
#include "stdafx.h"

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

bool bIsConnectedServer = false;

// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::cout << "on_message called with hdl: " << hdl.lock().get()
		<< " and message: " << msg->get_payload()
		<< std::endl;

	websocketpp::lib::error_code ec;

	c->send(hdl, msg->get_payload(), msg->get_opcode(), ec);
	if (ec) {
		std::cout << "Echo failed because: " << ec.message() << std::endl;
	}
}

void on_open(client* c, websocketpp::connection_hdl hdl) {
	std::cout << "open handler" << std::endl;
	client::connection_ptr con = c->get_con_from_hdl(hdl);
	//websocketpp::config::core_client::request_type requestClient = con->get_request();
	if (con->get_ec().value() != 0)
	{
		bIsConnectedServer = false;
	}
	else
	{
		bIsConnectedServer = true;
	}
}

int main(int argc, char* argv[]) {
	// Create a client endpoint
	client c;

	//std::string uri = "ws://localhost:9100/websocket?userid=admin";
	std::string uri = "ws://localhost:9100";

	if (argc == 2) {
		uri = argv[1];
	}

	try {
		// Set logging to be pretty verbose (everything except message payloads)
		c.set_access_channels(websocketpp::log::alevel::all);
		c.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize ASIO
		c.init_asio();
		c.set_reuse_addr(true);

		// Register our message handler
		c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
		c.set_open_handler(bind(&on_open, &c, ::_1));

		websocketpp::lib::error_code ec;
		client::connection_ptr con = c.get_connection(uri, ec);
		if (ec) {
			std::cout << "could not create connection because: " << ec.message() << std::endl;
			return 0;
		}

		// Note that connect here only requests a connection. No network messages are
		// exchanged until the event loop starts running in the next line.
		c.connect(con);

		// Start the ASIO io_service run loop
		// this will cause a single connection to be made to the server. c.run()
		// will exit when this connection is closed.
		c.run();
                c.stop();
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	}
}
```

二、线程方式实例

```
#include "stdafx.h"

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <iostream>

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

bool bIsConnectedServer = false;

// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::cout << "on_message called with hdl: " << hdl.lock().get()
		<< " and message: " << msg->get_payload()
		<< std::endl;

	websocketpp::lib::error_code ec;

	c->send(hdl, msg->get_payload(), msg->get_opcode(), ec);
	if (ec) {
		std::cout << "Echo failed because: " << ec.message() << std::endl;
	}
}
//连接上服务器，回调此函数
void on_open(client* c, websocketpp::connection_hdl hdl) {
	std::cout << "open handler" << std::endl;
	client::connection_ptr con = c->get_con_from_hdl(hdl);
	//websocketpp::config::core_client::request_type requestClient = con->get_request();
	if (con->get_ec().value() != 0)
	{
		bIsConnectedServer = false;
	}
	else
	{
		bIsConnectedServer = true;
	}
}

//服务器远程断开连接，回调此函数
void on_close(client* c, websocketpp::connection_hdl hdl)
{
	bIsConnectedServer = false;
        closeConnect();
}

//无法连接上服务器时，回调此函数，socket在内部已关闭上层无需再关闭
void on_termination_handler(client* c, websocketpp::connection_hdl hdl)
{
	bIsConnectedServer = false;
        closeConnect();
}

int main(int argc, char* argv[]) {
	// Create a client endpoint
	client c;

	//std::string uri = "ws://localhost:9100/websocket?userid=admin";
	std::string uri = "ws://localhost:9100";

	if (argc == 2) {
		uri = argv[1];
	}

	try {
		// Set logging to be pretty verbose (everything except message payloads)
		c.set_access_channels(websocketpp::log::alevel::all);
		c.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize ASIO
		c.init_asio();
		c.set_reuse_addr(true);

		// Register our message handler
		c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
		c.set_open_handler(bind(&on_open, &c, ::_1));
                c..set_close_handler(bind(on_close, &c, ::_1));

		websocketpp::lib::error_code ec;
		client::connection_ptr con = c.get_connection(uri, ec);
		if (ec) {
			std::cout << "could not create connection because: " << ec.message() << std::endl;
			return 0;
		}

		// Note that connect here only requests a connection. No network messages are
		// exchanged until the event loop starts running in the next line.
		c.connect(con);
                //设置连接断开通知handler
                con->set_termination_handler(bind(on_termination_handler, &c, ::_1));

		// Start the ASIO io_service run loop
		// this will cause a single connection to be made to the server. c.run()
		// will exit when this connection is closed.
		//c.run();

		//线程模式中使用run_one，模拟线程代码如下：
		while (true)
		{
			if (bIsConnectedServer)
			{
				con->send("aaaaa");//文本数据
				con->send("bbbbb", 5, websocketpp::frame::opcode::binary);//字节流
			}

			c.run_one();
			Sleep(500);		
		}
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	}
        c.stop();
}
```

主动关闭连接

```
int closeConnect()
{
	int nRet = 0;
	try {
		if (conn!= NULL && conn->get_state() == websocketpp::session::state::value::open)
		{
			websocketpp::close::status::value cvValue = 0;
			std::string strReason = "";
			conn->close(cvValue, strReason);
		}
	}
	catch (websocketpp::exception const & e)
	{
		nRet = -1;
	}
	return nRet;
}
```

主动发送数据

```
int  sendTextData(char *pText)
{
	int nRet = 0;
	try {
		websocketpp::lib::error_code ec;
		ec = conn->send(pText);
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

int sendBinaryData(unsigned char *pData, int nSize)
{
	int nRet = 0;
	try {
		websocketpp::lib::error_code ec;
		ec = conn->send(pData,nSize,websocketpp::frame::opcode::binary);
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
```

