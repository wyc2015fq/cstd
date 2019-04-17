# rabbitmq在c、java、python互通时的程序 - zp704393004的专栏 - CSDN博客





2018年05月07日 09:00:47[原来未知](https://me.csdn.net/zp704393004)阅读数：152








java：在与python或者java语言以及C语言互通时，需要有更改，现在的程序如下：

生产者的java程序：


package RabbitMQMaven_P.test_P;

/**
 * Hello world!
 *
 */
import com.rabbitmq.client.Channel;  
import com.rabbitmq.client.Connection;  
import com.rabbitmq.client.ConnectionFactory;  
public class App 
{
    private final static String QUEUE_NAME = "hello";  
    private final static String EXCHANGE_NAME = "hello-exchange";  

    public static void main( String[] args )
    {
        ConnectionFactory cf = new ConnectionFactory();  
        //rabbitmq监听IP  
        cf.setHost("localhost");  
        //rabbitmq默认监听端口，注意要记得开启端口  
        cf.setPort(5672);  

        //设置访问的用户  
        cf.setUsername("admin");  
        cf.setPassword("admin");  
        try
        {
        //建立连接  
        Connection conn = cf.newConnection();  
        //创建消息通道  
        Channel channel = conn.createChannel();  

        String msg = "hello world!!!! 你好222~";  
        //创建hello队列  
        channel.queueDeclare(QUEUE_NAME, false, false, false, null);  
        channel.exchangeDeclare(EXCHANGE_NAME, "direct",true); 
        channel.queueBind(QUEUE_NAME, EXCHANGE_NAME, "hola"); 
        //发送消息  
        channel.basicPublish(EXCHANGE_NAME, QUEUE_NAME, null, msg.getBytes());  
        //channel.basicPublish(EXCHANGE_NAME, QUEUE_NAME, null, msg.getBytes());  
        System.out.println("send msg: "+ msg + "\n to queue: ["+ QUEUE_NAME +"]  !\n"+"add to exchange:["+EXCHANGE_NAME+"]!");  

        channel.close();  
        conn.close();  
        }catch(Exception e1)
        {
            System.out.println( "try 2" );
        }
        System.out.println( "Hello World2!" );
    }


}

消费者的程序：
package RabbitMQMaven_P.test;

/**
 * Hello world!
 *
 */
import java.io.IOException;  
import java.util.UUID;  
import java.util.concurrent.ExecutorService;  
import java.util.concurrent.Executors;  
import java.util.concurrent.ScheduledExecutorService;  
import java.util.concurrent.TimeUnit;  
import java.util.concurrent.TimeoutException;  

import com.rabbitmq.client.Channel;  
import com.rabbitmq.client.Connection;  
import com.rabbitmq.client.ConnectionFactory; 
import com.rabbitmq.client.ConsumerCancelledException;  
import com.rabbitmq.client.QueueingConsumer;  
import com.rabbitmq.client.QueueingConsumer.Delivery;  
import com.rabbitmq.client.ShutdownSignalException;  
import com.rabbitmq.client.AMQP;  
import com.rabbitmq.client.Consumer;  
import com.rabbitmq.client.DefaultConsumer;  
import com.rabbitmq.client.Envelope; 

public class App 
{
    private final static String QUEUE_NAME = "hello"; 
    private final static String EXCHANGE_NAME = "hello-exchange"; 
    public static void main( String[] args )
    {
        ConnectionFactory cf = new ConnectionFactory();  
        //rabbitmq监听IP  
        cf.setHost("localhost");  
        //rabbitmq默认监听端口，注意要记得开启端口  
        cf.setPort(5672);  

        //设置访问的用户  
        cf.setUsername("admin");  
        cf.setPassword("admin");  

        try
        {
            //建立连接
        Connection conn = cf.newConnection();
            //创建消息通道  
        Channel channel = conn.createChannel();
        //创建hello队列  
        channel.queueDeclare(QUEUE_NAME, false, false, false, null);  
        channel.exchangeDeclare(EXCHANGE_NAME, "direct",true); 
        channel.queueBind(QUEUE_NAME, EXCHANGE_NAME, "hola"); 
        System.out.println(" Waiting for msg....");  
        //创建消费者，并接受消息  
        Consumer consumer = new DefaultConsumer(channel) {  
            @Override  
            public void handleDelivery(String consumerTag, Envelope envelope, AMQP.BasicProperties properties, byte[] body)  
                    throws IOException {  
                String msg = new String(body, "UTF-8");  
                System.out.println("Received is = '" + msg + "'");  
            }  
        };  
        channel.basicConsume(QUEUE_NAME, true, consumer);
        //channel.basicConsume(EXCHANGE_NAME, true, consumer);
        }catch(Exception e1)
        {
            System.out.println( "try 1" );
        }
        System.out.println( "Hello World!" );
    }

}

####################################################################################################-

python :消费者


*- coding: UTF-8 -*-  

import pika 
username = 'admin' 
pwd = 'admin' 
credentials = pika.PlainCredentials(username,pwd) 
conn_params = pika.ConnectionParameters('localhost',credentials=credentials) 

conn_broker = pika.BlockingConnection(conn_params) 

channel = conn_broker.channel() 

channel.exchange_declare(exchange ="hello-exchange",#声明交换器 
                exchange_type="direct", 
                passive=False, 
                durable=True, 
                auto_delete=False) 
#channel.exchange_declare(exchange="hello-exchange", 
#                type = "direct", 
#                passive =  False, 
#                durable = True, 
#                auto_delete = False) 
channel.queue_declare("hello")#声明队列 
channel.queue_bind(queue="hello",#通过键“hola”将队列与交换器i绑定 
            exchange="hello-exchange", 
            routing_key="hola") 

def msg_consumer(channel,method,header,body):#处理传入的消息的函数 
    channel.basic_ack(delivery_tag=method.delivery_tag)#确认消息 
    if body=="quit": 
        channel.basic_cancel(consumer_tag="hello-consumer")#停止消费并且退出 
        channel.stop_consuming() 
    else: 
        print body 
    return 
#channel.basic_consume(); 
channel.basic_consume(msg_consumer,queue="hello",#订阅消费者 
                consumer_tag="hello-consumer") 
print ' [*] Waiting for messages. To exit press CTRL+C' 
channel.start_consuming()


python：生产者

#-*- coding: UTF-8 -*-  
import pika,sys 
import os 

username = 'admin' 
pwd = 'admin' 
credentials = pika.PlainCredentials(username,pwd) 
conn_params = pika.ConnectionParameters('localhost',credentials=credentials) 

conn_broker = pika.BlockingConnection(conn_params)#创建到服务器的链接 

channel = conn_broker.channel()#获得信道 

channel.exchange_declare(exchange="hello-exchange",#声明交换器 
                exchange_type="direct", 
                passive= False, 
                durable=True, 
                auto_delete=False) 
channel.queue_declare("hello")#声明队列 
channel.queue_bind(queue="hello",#通过键“hola”将队列与交换器i绑定 
            exchange="hello-exchange", 
            routing_key="hola") 
#msg = sys.argv[1] 
msg="hello,consumer!!!!!!!!!!!!!你好!!!!!!!!!!!!" 
msg_props = pika.BasicProperties() 
msg_props.content_type = "text/plain"#创建纯文本消息 

channel.basic_publish(body=msg,#发布消息 
                exchange = "hello-exchange", 


                routing_key = "hola")

####################################################################################################

c语言：

消费者

/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MIT
 *
 * Portions created by VMware are Copyright (c) 2007-2012 VMware, Inc.
 * All Rights Reserved.
 *
 * Portions created by Tony Garnock-Jones are Copyright (c) 2009-2010
 * VMware, Inc. and Tony Garnock-Jones. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ***** END LICENSE BLOCK *****
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp.h>
#include <amqp_framing.h>

#include <assert.h>

#include "utils.h"

int main(int argc, char const * const *argv) {
  char const *hostname;
  int port;
  char const *exchange;
  char const *bindingkey;
  char const *qname="hello";

  int sockfd;
  amqp_connection_state_t conn;

  amqp_bytes_t queuename;

  queuename=amqp_cstring_bytes(qname);
  if (argc < 5) {
    fprintf(stderr, "Usage: amqp_listen host port exchange bindingkey\n");
    return 1;
  }

  hostname = argv[1];
  port = atoi(argv[2]);
  exchange = argv[3];
  bindingkey = argv[4];

  conn = amqp_new_connection();

  die_on_error(sockfd = amqp_open_socket(hostname, port), "Opening socket");
  amqp_set_sockfd(conn, sockfd);
  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "admin", "admin"),
            "Logging in");
  amqp_channel_open(conn, 1);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  {
    amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, queuename, 1, 1, 1, 1,
                            amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
    queuename = amqp_bytes_malloc_dup(r->queue);
    if (queuename.bytes == NULL) {
      fprintf(stderr, "Out of memory while copying queue name");
      return 1;
    }
  }

  amqp_queue_bind(conn, 1, queuename, amqp_cstring_bytes(exchange), amqp_cstring_bytes(bindingkey),
          amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding queue");

  amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

  {
    amqp_frame_t frame;
    int result;

    amqp_basic_deliver_t *d;
    amqp_basic_properties_t *p;
    size_t body_target;
    size_t body_received;

    while (1) {
      amqp_maybe_release_buffers(conn);
      result = amqp_simple_wait_frame(conn, &frame);
      printf("Result %d\n", result);
      if (result < 0)
    break;

      printf("Frame type %d, channel %d\n", frame.frame_type, frame.channel);
      if (frame.frame_type != AMQP_FRAME_METHOD)
    continue;

      printf("Method %s\n", amqp_method_name(frame.payload.method.id));
      if (frame.payload.method.id != AMQP_BASIC_DELIVER_METHOD)
    continue;

      d = (amqp_basic_deliver_t *) frame.payload.method.decoded;
      printf("Delivery %u, exchange %.*s routingkey %.*s\n",
         (unsigned) d->delivery_tag,
         (int) d->exchange.len, (char *) d->exchange.bytes,
         (int) d->routing_key.len, (char *) d->routing_key.bytes);

      result = amqp_simple_wait_frame(conn, &frame);
      if (result < 0)
    break;

      if (frame.frame_type != AMQP_FRAME_HEADER) {
    fprintf(stderr, "Expected header!");
    abort();
      }
      p = (amqp_basic_properties_t *) frame.payload.properties.decoded;
      if (p->_flags & AMQP_BASIC_CONTENT_TYPE_FLAG) {
    printf("Content-type: %.*s\n",
           (int) p->content_type.len, (char *) p->content_type.bytes);
      }
      printf("----\n");

      body_target = frame.payload.properties.body_size;
      body_received = 0;
      printf( "body_target= %d \n",(int)body_target);

      while (body_received < body_target) {
    result = amqp_simple_wait_frame(conn, &frame);
      printf("--1--\n");
    if (result < 0)
      break;

    if (frame.frame_type != AMQP_FRAME_BODY) {
         printf("--2--\n");
      fprintf(stderr, "Expected body!");
      abort();
    }


    body_received += frame.payload.body_fragment.len;
    assert(body_received <= body_target);
      printf("--3--\n");
    amqp_dump(frame.payload.body_fragment.bytes,
          frame.payload.body_fragment.len);
      printf("--4--\n");
      }

      if (body_received != body_target) {
    /* Can only happen when amqp_simple_wait_frame returns <= 0 */
    /* We break here to close the connection */
    break;
      }
    }
  }

  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
  die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");

  return 0;


}

生产者：

/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MIT
 *
 * Portions created by VMware are Copyright (c) 2007-2012 VMware, Inc.
 * All Rights Reserved.
 *
 * Portions created by Tony Garnock-Jones are Copyright (c) 2009-2010
 * VMware, Inc. and Tony Garnock-Jones. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ***** END LICENSE BLOCK *****
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <stdint.h>
#include <amqp.h>
#include <amqp_framing.h>

#include "utils.h"

int main(int argc, char const * const *argv) {
  char const *hostname;
  int port;
  char const *exchange;
  char const *routingkey;
  char const *messagebody;

  int sockfd;
  amqp_connection_state_t conn;

  if (argc < 6) {
    fprintf(stderr, "Usage: amqp_sendstring host port exchange routingkey messagebody\n");
    return 1;
  }

  hostname = argv[1];
  port = atoi(argv[2]);
  exchange = argv[3];
  routingkey = argv[4];
  messagebody = argv[5];

  conn = amqp_new_connection();//声明一个新的amqp connection

  die_on_error(sockfd = amqp_open_socket(hostname, port), "Opening socket");/*//hostname：服务主机，port：端口，amqp_open_socket：获取socket*/
  amqp_set_sockfd(conn, sockfd);/*//amqp_set_sockfd：将amqp connection和sockfd进行绑定*/
  die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "admin", "admin"),
            "Logging in");/*//amqp_login：用于登录RabbitMQ server，主要目的为了进行权限管理；*/
  amqp_channel_open(conn, 1);/*//用于关联conn和channel*/
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");

  {
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_cstring_bytes("text/plain");
    props.delivery_mode = 2; /* persistent delivery mode */
    die_on_error(amqp_basic_publish(conn,
                    1,
                    amqp_cstring_bytes(exchange),
                    amqp_cstring_bytes(routingkey),
                    0,
                    0,
                    &props,
                    amqp_cstring_bytes(messagebody)),
         "Publishing");
  }

  die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
  die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
  die_on_error(amqp_destroy_connection(conn), "Ending connection");
  return 0;
}


C语言的运行命令

./examples/amqp_sendstring localhost 5672 hello-exchange hola "hello world"  




./examples/amqp_listen localhost 5672 hello-exchange hola














