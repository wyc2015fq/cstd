# websocketpp wss客户端代码 - 深之JohnChen的专栏 - CSDN博客

2018年11月30日 11:09:06[byxdaz](https://me.csdn.net/byxdaz)阅读数：623


 websocket使用wss协议,使用websocketpp库时，需要使用openssl库支持。

不使用ssl，即使用ws非wss

#include <websocketpp/config/asio_no_tls_client.hpp>

#include <websocketpp/client.hpp>

使用ssl，即使用wss

#include <websocketpp/config/asio_client.hpp>

#include <websocketpp/client.hpp>

在工程属性中添加openssl头文件目录引用，增加openssl库文件目录引用。

![](https://img-blog.csdnimg.cn/20181130110737331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2J5eGRheg==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181130110824690.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2J5eGRheg==,size_16,color_FFFFFF,t_70)

在工程代码中添加对openssl部份引用:

#pragma comment(lib, "libeay32.lib")

#pragma comment(lib, "ssleay32.lib")

```
#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <iostream>

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

bool bIsConnectedServer = false;

/// Verify that one of the subject alternative names matches the given hostname
bool verify_subject_alternative_name(const char * hostname, X509 * cert) {
	STACK_OF(GENERAL_NAME) * san_names = NULL;

	san_names = (STACK_OF(GENERAL_NAME) *) X509_get_ext_d2i(cert, NID_subject_alt_name, NULL, NULL);
	if (san_names == NULL) {
		return false;
	}

	int san_names_count = sk_GENERAL_NAME_num(san_names);

	bool result = false;

	for (int i = 0; i < san_names_count; i++) {
		const GENERAL_NAME * current_name = sk_GENERAL_NAME_value(san_names, i);

		if (current_name->type != GEN_DNS) {
			continue;
		}

		char * dns_name = (char *)ASN1_STRING_data(current_name->d.dNSName);

		// Make sure there isn't an embedded NUL character in the DNS name
		if (ASN1_STRING_length(current_name->d.dNSName) != strlen(dns_name)) {
			break;
		}
		// Compare expected hostname with the CN
		result = (_stricmp(hostname, dns_name) == 0);
	}
	sk_GENERAL_NAME_pop_free(san_names, GENERAL_NAME_free);

	return result;
}

/// Verify that the certificate common name matches the given hostname
bool verify_common_name(const char * hostname, X509 * cert) {
	// Find the position of the CN field in the Subject field of the certificate
	int common_name_loc = X509_NAME_get_index_by_NID(X509_get_subject_name(cert), NID_commonName, -1);
	if (common_name_loc < 0) {
		return false;
	}

	// Extract the CN field
	X509_NAME_ENTRY * common_name_entry = X509_NAME_get_entry(X509_get_subject_name(cert), common_name_loc);
	if (common_name_entry == NULL) {
		return false;
	}

	// Convert the CN field to a C string
	ASN1_STRING * common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
	if (common_name_asn1 == NULL) {
		return false;
	}

	char * common_name_str = (char *)ASN1_STRING_data(common_name_asn1);

	// Make sure there isn't an embedded NUL character in the CN
	if (ASN1_STRING_length(common_name_asn1) != strlen(common_name_str)) {
		return false;
	}

	// Compare expected hostname with the CN
	return (_stricmp(hostname, common_name_str) == 0);
}

/**
* This code is derived from examples and documentation found ato00po
* http://www.boost.org/doc/libs/1_61_0/doc/html/boost_asio/example/cpp03/ssl/client.cpp
* and
* https://github.com/iSECPartners/ssl-conservatory
*/
bool verify_certificate(const char * hostname, bool preverified, boost::asio::ssl::verify_context& ctx) {
	// The verify callback can be used to check whether the certificate that is
	// being presented is valid for the peer. For example, RFC 2818 describes
	// the steps involved in doing this for HTTPS. Consult the OpenSSL
	// documentation for more details. Note that the callback is called once
	// for each certificate in the certificate chain, starting from the root
	// certificate authority.

	// Retrieve the depth of the current cert in the chain. 0 indicates the
	// actual server cert, upon which we will perform extra validation
	// (specifically, ensuring that the hostname matches. For other certs we
	// will use the 'preverified' flag from Asio, which incorporates a number of
	// non-implementation specific OpenSSL checking, such as the formatting of
	// certs and the trusted status based on the CA certs we imported earlier.
	int depth = X509_STORE_CTX_get_error_depth(ctx.native_handle());

	// if we are on the final cert and everything else checks out, ensure that
	// the hostname is present on the list of SANs or the common name (CN).
	if (depth == 0 && preverified) {
		X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());

		if (verify_subject_alternative_name(hostname, cert)) {
			return true;
		}
		else if (verify_common_name(hostname, cert)) {
			return true;
		}
		else {
			return false;
		}
	}

	return preverified;
}

/// TLS Initialization handler
/**
* WebSocket++ core and the Asio Transport do not handle TLS context creation
* and setup. This callback is provided so that the end user can set up their
* TLS context using whatever settings make sense for their application.
*
* As Asio and OpenSSL do not provide great documentation for the very common
* case of connect and actually perform basic verification of server certs this
* example includes a basic implementation (using Asio and OpenSSL) of the
* following reasonable default settings and verification steps:
*
* - Disable SSLv2 and SSLv3
* - Load trusted CA certificates and verify the server cert is trusted.
* - Verify that the hostname matches either the common name or one of the
*   subject alternative names on the certificate.
*
* This is not meant to be an exhaustive reference implimentation of a perfect
* TLS client, but rather a reasonable starting point for building a secure
* TLS encrypted WebSocket client.
*
* If any TLS, Asio, or OpenSSL experts feel that these settings are poor
* defaults or there are critically missing steps please open a GitHub issue
* or drop a line on the project mailing list.
*
* Note the bundled CA cert ca-chain.cert.pem is the CA cert that signed the
* cert bundled with echo_server_tls. You can use print_client_tls with this
* CA cert to connect to echo_server_tls as long as you use /etc/hosts or
* something equivilent to spoof one of the names on that cert
* (websocketpp.org, for example).
*/
context_ptr on_tls_init(const char * hostname, websocketpp::connection_hdl) {
	context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

	try {
		ctx->set_options(boost::asio::ssl::context::default_workarounds |
			boost::asio::ssl::context::no_sslv2 |
			boost::asio::ssl::context::no_sslv3 |
			boost::asio::ssl::context::single_dh_use);

		ctx->set_verify_mode(boost::asio::ssl::verify_peer);
		ctx->set_verify_callback(bind(&verify_certificate, hostname, ::_1, ::_2));

		// Here we load the CA certificates of all CA's that this client trusts.
		ctx->load_verify_file("ca-chain.cert.pem");
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return ctx;
}

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

	std::string hostname = "139.199.61.178";
	std::string uri = "wss://139.199.61.178:8089/ws?uid=admin&pwd=admin";

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
		c.set_tls_init_handler(bind(&on_tls_init, hostname.c_str(), ::_1));

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
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	}
}
```

