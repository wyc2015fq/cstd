# 详解电子邮件的POP3协议及最小化实现 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年03月21日 17:22:59[gloomyfish](https://me.csdn.net/jia20003)阅读数：4878








详细解释电子邮件中POP3协议及最小化实现

**1.     什么是POP3**

POP3的英文全称Post OfficeProtocol Version 3，POP3协议本质上是一种消息传递协议

主要用来在支持POP3客户端和POP3邮件服务器端通信，基于TCP协议之上的应用层协议

POP3协议主要用来读取邮件服务器上的电子邮件。发送邮件时候支持的协议为SMTP.

此外还有一种同时支持收发邮件协议IMAP.




直白的说，当你的邮件客户端检查你是否有新邮件以及读新邮件是，很可能协议为POP3

当你写完一封Email,发送给指定收件人时候，很可能是通过SMTP协议,当然还可以是IMAP。
|协议|支持发送邮件|支持接受邮件|
|----|----|----|
|**POP3**|No|Yes|
|**SMTP/ESMTP**|Yes|No|
|**IMAP**|Yes|Yes|



**2.     基本操作**

服务器端启动POP3服务在端口110, Google Mail是SSL加密的，在端口995。当客户端连

接成功之后，服务器端首先会发送一条问候信息(Greet).然后服务器端与客户端开始会话操作

知道客户端断开连接或者退出会话。

基本的会话类型分为两个阶段 – 第一阶段为授权状态；第二个阶段为传输状态



**授权状态（AUTHORIZATION State）**支持三个命令为【USER, PASS, QUIT】：

-         **USER** <name> -  <name>参数是必须的，指要求的UserID,通常为Email帐号

-         **PASS** <password><password>参数是必须的,指要求的Email帐号密码

-         **QUIT** 不需要任何参数，表示退出，结束与POP3服务器的会话



**传输状态（TRANSACTION State）**支持命令为【STAT, LIST, RETR, DELE, 

NOOP,LAST, RSET】

-         **STAT** 不需要任何参数，列出当前邮箱中所有的邮件总数及大小

-         **LIST** [message id] 参数可选，列出当前邮箱每封邮件及其大小

-         **RETR** <Message id> 参数是必须的，读出指定message id的邮件内容

-         **DELE** <Message id> 参数是必须的，删除指定message id 的邮件

-         **NOOP** 不需要任何参数，POP3服务器只是发一条问候消息作为响应

-         **LAST** 不需要任何参数，返回Client最后访问过的消息ID,如何没有任何email被客户端

读过，返回最大的Message Id 1， 如何邮箱里面没有任何email，返回 0

-         **RSET** 不需要任何参数, 任何被标记为删除的邮件将被恢复



最后，当QUIT命令发布以后客户端退出，服务器端则进入更新状态（UPDATE State）。

**一个是Google MailPOP3 会话的例子如下：**

![](http://hi.csdn.net/attachment/201203/21/0_1332321692B1vj.gif)

**3.     一个最小化命令集的Java实现**



```java
package com.gloomyfish.sslsocket.study;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;

public class POP3Client {
	private static DataOutputStream writer;
	public static void main(String[] args) throws Exception
	{
		int delay = 6000;// give enough delay to get response message from POP server
		SSLSocket sock = (SSLSocket)((SSLSocketFactory)SSLSocketFactory.getDefault()).createSocket("pop.gmail.com", 995);
		final BufferedReader br = new BufferedReader(new InputStreamReader(sock.getInputStream()));
		(new Thread(new Runnable()
		{
			public void run()
			{
				try
				{
					String line;
					while((line = br.readLine()) != null)
						System.out.println(" S: "+line);
				}
				catch (IOException e)
				{
					e.printStackTrace();
				}
			}
		})).start();
		writer = new DataOutputStream(sock.getOutputStream());
		
		// start to AUTHORIZATION State
		send("USER YOUREMAILID\r\n");
		Thread.sleep(delay);
		send("PASS YOURPASSWORD\r\n");
		Thread.sleep(delay);
		// end AUTHORIZATION State
		
		// start <<The TRANSACTION State>>
		send("STAT\r\n"); // check new arrived mail
		Thread.sleep(delay);
		
		// List Mail
		send("LIST 1\r\n"); // check new arrived mail
		Thread.sleep(delay);
		
		// Fetch the message here [RETR <SP> msg CRLF]
		send("RETR 1\r\n"); // check new arrived mail
		Thread.sleep(delay);
		
		// DELET
		send("DELE 1\r\n"); // check new arrived mail
		Thread.sleep(delay);
		
		// UNMARKER THE DELETED EMAIL
		send("RSET\r\n"); // check new arrived mail
		Thread.sleep(delay);
		// end The TRANSACTION State
		
		// issue the [quit] command from client.
		send("QUIT\r\n");
		Thread.sleep(delay);
	}
	
	private static void send(String s) throws Exception
	{
		writer.writeBytes(s);
		System.out.println(" C: "+s);
	}

}
```





