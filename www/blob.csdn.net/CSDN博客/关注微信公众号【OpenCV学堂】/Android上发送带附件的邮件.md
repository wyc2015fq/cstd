# Android上发送带附件的邮件 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2014年05月10日 19:17:04[gloomyfish](https://me.csdn.net/jia20003)阅读数：7478








准备工作-下载最新版本的JMail

[https://java.net/projects/javamail/pages/Home#Download_JavaMail_1.5.2_Release](https://java.net/projects/javamail/pages/Home#Download_JavaMail_1.5.2_Release)



[http://www.oracle.com/technetwork/java/javase/downloads/index-135046.html](http://www.oracle.com/technetwork/java/javase/downloads/index-135046.html)

在android上发送邮件方式：

第一种：借助GMail APP客户端，缺点是必须使用GMail帐号，有点是比较方便

不需要写很多代码，但是不是很灵活。

第二种：基于JMail实现，可以很灵活的自己设置各种属性，不需要GMail帐号

在第二种方式的实现之前，看一下JMail对EMail结构的划分：

![](https://img-blog.csdn.net/20140510190517453)


基于SMTP协议发送EMail，所以客户端必须要知道SMTP的主机

腾讯邮件的SMTP主机为：stmp.qq.com端口为465基于SSL协议

最后我做了一个简单的封装，把发送文本加图像附件的功能做出了

一个单独的Class，只要调用一下即可完成：



```java
package com.gloomyfish.jmail.demo;

import java.util.Date;
import java.util.Properties;

import javax.activation.DataHandler;
import javax.activation.DataSource;
import javax.activation.FileDataSource;
import javax.mail.Address;
import javax.mail.Message;
import javax.mail.Multipart;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;

public class EMailSender {

	private String host;
	private String port;
	private String userName;
	private String password;
	private String[] images;

	public String[] getImagePath() {
		return images;
	}

	public void setImagePath(String[] imagePath) {
		this.images = imagePath;
	}

	public EMailSender(String host, String port, String userName, String password) 
	{
		this.host = host;
		this.port = port;
		this.userName = userName;
		this.password = password;
	}

	public void sendEmail(String subject, String recepits, String sender, String content) 
	{
		Properties props = new Properties();
		props.put("mail.smtp.host", host);  //设置smtp的服务器地址
		// props.put("mail.smtp.starttls.enable", "true");
		// props.put("mail.smtp.port", port); // 设置端口
		// props.put("mail.smtp.auth", "true"); //设置smtp服务器要身份验证。
		
		props.put("mail.smtp.socketFactory.port", port);
		props.put("mail.smtp.socketFactory.class", "javax.net.ssl.SSLSocketFactory");
		props.put("mail.smtp.auth", "true");
		props.put("mail.smtp.port", port);
		
		// 返回授权Base64编码
		PopupAuthenticator auth = new PopupAuthenticator(userName, password);
		// 获取会话对象
		Session session = Session.getInstance(props, auth);	
		// 设置为DEBUG模式
		session.setDebug(true);
		
		// 邮件内容对象组装
		MimeMessage message = new MimeMessage(session);
		try
		{
			Address addressFrom = new InternetAddress(sender, "Jia Zhi Gang");
			Address addressTo = new InternetAddress(recepits, "My QQ E-Mail");
			message.setSubject(subject);
			message.setSentDate(new Date());
			message.setFrom(addressFrom);
			message.addRecipient(Message.RecipientType.TO,addressTo);
			 
			// 邮件文本/HTML内容
			Multipart multipart = new MimeMultipart();
			MimeBodyPart messageBodyPart = new MimeBodyPart();
	        messageBodyPart.setContent(content, "text/html");
	        multipart.addBodyPart(messageBodyPart);
	        
	        // 添加邮件附件
	        if (images != null && images.length > 0) {
	            for (String filePath : images) {
	                MimeBodyPart attachPart = new MimeBodyPart();	 
	                DataSource source = new FileDataSource(filePath);
	                attachPart.setDataHandler(new DataHandler(source));
	                attachPart.setFileName(filePath);
	                multipart.addBodyPart(attachPart);
	            }
	        }

	        // 保存邮件内容
	        message.setContent(multipart);
			
			// 获取SMTP协议客户端对象，连接到指定SMPT服务器
			Transport transport = session.getTransport("smtp");
			transport.connect(host, Integer.parseInt(port), userName, password);
			System.out.println("connet it success!!!!");
			
			// 发送邮件到SMTP服务器
			Thread.currentThread().setContextClassLoader( getClass().getClassLoader() );
			Transport.send(message);
			System.out.println("send it success!!!!");
			
			// 关闭连接
			transport.close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}

	public String getHost() {
		return host;
	}

	public void setHost(String host) {
		this.host = host;
	}

	public String getPort() {
		return port;
	}

	public void setPort(String port) {
		this.port = port;
	}

	public String getUserName() {
		return userName;
	}

	public void setUserName(String userName) {
		this.userName = userName;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

}
```



用户授权类：



```java
package com.gloomyfish.jmail.demo;


import javax.mail.Authenticator;
import javax.mail.PasswordAuthentication;


class PopupAuthenticator extends Authenticator {
	private String userName;
	private String password;
	public PopupAuthenticator(String userName, String password)
	{
		this.userName = userName;
		this.password = password;
	}
    public PasswordAuthentication getPasswordAuthentication() {
        return new PasswordAuthentication(userName, password);
    }
}
```
特别注意：


1.在android上发送邮件必须自己导入三个相关的JAVA文件

![](https://img-blog.csdn.net/20140510191421359)


上述JAR文件的下载地址已经在文章开始处给出！



