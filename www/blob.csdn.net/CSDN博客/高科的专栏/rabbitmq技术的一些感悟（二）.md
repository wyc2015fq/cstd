
# rabbitmq技术的一些感悟（二） - 高科的专栏 - CSDN博客

2014年08月19日 21:01:22[高科](https://me.csdn.net/pbymw8iwm)阅读数：12548


上一节文章主要是说了一下rabbitmq的安装以及搭建好环境的一些命令，以及常用的api调用，其实自从google被封掉之后，我之前收藏的很多技术连接都已经被禁止访问了，这个是多么可悲的一件事情啊，说多了都是泪。
首先，我先写一段消费者的模块，建立连接，初始化amq以及销毁连接：

```python
Comsumer::Comsumer(){
}
void Comsumer::init(){
	conn = amqp_new_connection();
	int sockfd = amqp_open_socket(g_logSrv.m_conf.m_hostname.c_str(), g_logSrv.m_conf.m_port);
	amqp_set_sockfd(conn,sockfd);
	LogServer::die_on_amqp_error(amqp_login(conn, g_logSrv.m_conf.m_vhosts.c_str(), 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,  g_logSrv.m_conf.m_username.c_str(),  g_logSrv.m_conf.m_psw.c_str()),"Logging in");
	amqp_channel_open(conn, (amqp_channel_t)g_logSrv.m_conf.m_channel);
	LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");
	for(list<ramqserverInfo>::iterator it = g_logSrv.m_conf.getAmqserverInfo.begin(); it != g_logSrv.m_conf.getAmqserverInfo.end(); it++){
		amqp_exchange_declare(conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(it->exchange.c_str()), amqp_cstring_bytes("direct"), 0, 0, amqp_empty_table);
		LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring exchange");
		amqp_queue_declare(conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(it->queue.c_str()), 0, 1, 0, 1, amqp_empty_table); // durable && auto-delete 
		LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
		
		amqp_queue_bind(conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(it->queue.c_str()), amqp_cstring_bytes(it->exchange.c_str()), amqp_cstring_bytes(it->route.c_str()), amqp_empty_table);  
		LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding"); 
		amqp_basic_consume(conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(it->queue.c_str()), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
		LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");
		connList.push_back(*it);
	}
}
Comsumer::~Comsumer(){
	if(conn == NULL){
		return;
	}
	LogServer::die_on_amqp_error(amqp_channel_close(conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, AMQP_REPLY_SUCCESS), "Closing channel");
	LogServer::die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");	
	LogServer::die_on_error(amqp_destroy_connection(conn), "Ending connection");
}
```

然后再写一段消费者的队列，他也是可持久化的队列：
```python
Producer::Producer(){
}
void Producer::init(){
	conn.conn = amqp_new_connection();
	int sockfd = amqp_open_socket(g_logSrv.m_conf.m_hostname.c_str(), g_logSrv.m_conf.m_port);
	amqp_set_sockfd(conn.conn,sockfd);
	LogServer::die_on_amqp_error(amqp_login(conn.conn, g_logSrv.m_conf.m_vhosts.c_str(), 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,  g_logSrv.m_conf.m_username.c_str(),  g_logSrv.m_conf.m_psw.c_str()),"Logging in");
	amqp_channel_open(conn.conn, (amqp_channel_t)g_logSrv.m_conf.m_channel);
	LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn.conn), "Opening channel");
	for(list<ramqserverInfo>::iterator it = g_logSrv.m_conf.commonAmqserver.begin(); it != g_logSrv.m_conf.commonAmqserver.end(); it++){
		amqp_exchange_declare(conn.conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(it->exchange.c_str()), amqp_cstring_bytes("direct"), 0, 0, amqp_empty_table);
		LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn.conn), "Declaring exchange");
		amqp_queue_declare(conn.conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(it->queue.c_str()), 0, 1, 0, 1, amqp_empty_table); // durable && auto-delete 
		LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn.conn), "Declaring queue");
		amqp_queue_bind(conn.conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(it->queue.c_str()), amqp_cstring_bytes(it->exchange.c_str()), amqp_cstring_bytes(it->route.c_str()), amqp_empty_table);  
		LogServer::die_on_amqp_error(amqp_get_rpc_reply(conn.conn), "Binding"); 	
		connList.push_back(*it);
	}
	conn.props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
	conn.props.content_type = amqp_cstring_bytes("text/plain");
	conn.props.delivery_mode = 2;
}
Producer::~Producer(){
	if(conn.conn == NULL){
		return;
	}
	LogServer::die_on_amqp_error(amqp_channel_close(conn.conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, AMQP_REPLY_SUCCESS), "Closing channel");
	LogServer::die_on_amqp_error(amqp_connection_close(conn.conn, AMQP_REPLY_SUCCESS), "Closing connection");
	LogServer::die_on_error(amqp_destroy_connection(conn.conn), "Ending connection");
}
```
最后就是如何向rabbitmq中写消息和取消息了：
```python
void LogServer::WriteToAmqQueue(bool isQuick, string str){
	if(isQuick){
		uint8_t size = m_produce.connQuickList.size();
		uint8_t index = rand()%size;
		LogServer::die_on_error(amqp_basic_publish(m_produce.connQuick.conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(m_produce.connQuickList[index].exchange.c_str()), amqp_cstring_bytes(m_produce.connQuickList[index].route.c_str()), 0, 0, &m_produce.connQuick.props, amqp_cstring_bytes(str.c_str())), "Publishing");
	}else{
		uint8_t size = m_produce.connList.size();
		uint8_t index = rand()%size;
		LogServer::die_on_error(amqp_basic_publish(m_produce.conn.conn, (amqp_channel_t)g_logSrv.m_conf.m_channel, amqp_cstring_bytes(m_produce.connList[index].exchange.c_str()), amqp_cstring_bytes(m_produce.connList[index].route.c_str()), 0, 0, &m_produce.conn.props, amqp_cstring_bytes(str.c_str())), "Publishing");
	}	
}
void LogServer::runAmqConsumer(){
	m_produce.init();
	m_consumer.init();
	uint32_t tickTime = time(0);
	while(1){
		amqp_rpc_reply_t ret;
		amqp_envelope_t envelope;
		memset(&envelope,0,sizeof(envelope));
		envelope.message.body.bytes={0};
		amqp_maybe_release_buffers(m_consumer.conn);
		ret = amqp_consume_message(m_consumer.conn, &envelope, NULL, 0);
		if (ret.reply_type == AMQP_RESPONSE_NORMAL ){
			char* str = (char*)envelope.message.body.bytes;
			if(str == NULL){
				return;
			}
			str[envelope.message.body.len-1] = '\0';
			LOG(DEBUG)("recv from amq:len:%d,%s\n",envelope.message.body.len,str);
			vector<string> vectStr;
			Util::strSplit(str, "|", vectStr);			
			if(vectStr.size() > 8 && (g_logSrv.m_conf.quickWords.find(vectStr[8]) != g_logSrv.m_conf.quickWords.end())){
				WriteToAmqQueue(true,str);
			}
			else
			{
				WriteToAmqQueue(false,str);
			}
			amqp_destroy_envelope(&envelope);
		//	pushMsgQueue(str);
			str = NULL;
		}
		else{
			amqp_destroy_envelope(&envelope);
			return;
		}
		if(tickTime + FAMETICK < time(0) && m_conf.loadConfig() != 0){
			tickTime = time(0);
			if(LogServer::m_stop == true){
				m_produce.init();
				m_consumer.init();
				stop();
			}
			continue;
		}
	}
}
```


