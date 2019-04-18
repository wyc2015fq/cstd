# java发送邮件 - z69183787的专栏 - CSDN博客
2013年10月24日 19:26:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1401
个人分类：[Java-知识																[邮件-发送Demo](https://blog.csdn.net/z69183787/article/category/2186437)](https://blog.csdn.net/z69183787/article/category/2175279)
需要jar包：mail.jar
SendMail.java
```java
/**
 * 
 */
package com.wonders.volunteer;
/**
 * 类简要描述信息。（必需） 类详细描述信息。（可选，可分多行描述）
 * 
 * @author zhoushun
 * @version 1.0
 * @since 1.0
 */
import java.util.Date;
import java.util.Properties;
import javax.mail.Message;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;
public class SendMail {
	public static void send(String mailbox, String title, String content) {
		String mail_from = Constant.mailAddress; // mailbox 发送到哪 title 标题
													// content 邮件内容
		try {
			Properties props = new Properties();
			props.put("mail.smtp.host", Constant.mailServer);
			props.put("mail.smtp.auth", "true");
			Session s = Session.getInstance(props);
			s.setDebug(true);
			MimeMessage message = new MimeMessage(s);
			InternetAddress from = new InternetAddress(mail_from);
			message.setFrom(from);
			InternetAddress to = new InternetAddress(mailbox);
			message.setRecipient(Message.RecipientType.TO, to);
			message.setSubject(title);
			message.setText(content);
			message.setContent(content, "text/html;charset=gbk");
			message.setSentDate(new Date());
			message.saveChanges();
			Transport transport = s.getTransport("smtp");
			transport.connect(Constant.mailServer, Constant.mailCount,
					Constant.mailPassword);
			transport.sendMessage(message, message.getAllRecipients());
			transport.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
```
constants.java
```java
/**
 * 
 */
package com.wonders.volunteer;
/**
 * 类简要描述信息。（必需）
 * 类详细描述信息。（可选，可分多行描述）
 *
 * @author zhoushun
 * @version 1.0
 * @since 1.0
 */
public class Constant { 
	public static final String mailAddress ="zhoushun@wondersgroup.com"; 
	public static final String mailCount ="xxxxxxx"; 
	public static final String mailPassword ="xxxxxx"; 
	public static final String mailServer ="smtp.wondersgroup.com"; 
	//pukeyouxintest, 
	}
```
发送代码：
```java
SendMail sm = new SendMail();
				String cotent = (String)mail.get(1)+" ： 您的申请已通过，具体站点时间如下：";
				cotent += (String)mail.get(4)+" "+(String)mail.get(5)+" 到 "+(String)mail.get(6)+" "+(String)mail.get(2)+" "+(String)mail.get(3)+"站。具体情况您可以联系该站点联系人："+(String)mail.get(8)+"。联系方式："+(String)mail.get(9)+"。";
		    	sm.send((String)mail.get(0),"志愿者申请审核结果及具体情况",cotent);
```
