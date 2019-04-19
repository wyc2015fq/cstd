# JavaMail实现邮件发送工具类 - 零度的博客专栏 - CSDN博客
2016年04月14日 17:17:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：1111
```
package org.nercita.bcp.util;
import java.io.IOException;
import java.util.Date;
import java.util.Properties;
import javax.activation.DataHandler;
import javax.activation.FileDataSource;
import javax.mail.Authenticator;
import javax.mail.Message;
import javax.mail.Message.RecipientType;
import javax.mail.MessagingException;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeBodyPart;
import javax.mail.internet.MimeMessage;
import javax.mail.internet.MimeMultipart;
import org.nercita.bcp.system.util.CustomSystemException;
import org.nercita.core.utils.PropertyUtils;
import org.nercita.core.utils.UUIDUtils;
/**
 * 邮件发送工具类
 * @author zhangwenchao
 *
 */
public class MailUtils {
	public static Properties sysProps = null;
	static{
		try {
			sysProps = PropertyUtils.loadProperties("application.properties");	
		} catch (IOException e) {
			e.printStackTrace();
			throw new CustomSystemException("1002", "属性文件加载异常");
		}
	}
	
    public  static final String ENCODEING = sysProps.getProperty("ENCODEING"); //编码  
    
    public  static final String DN = sysProps.getProperty("DN"); //域名  
    
    private static final String PROTOCOL = sysProps.getProperty("PROTOCOL");// 协议
    
    private static final String HOST = sysProps.getProperty("HOST"); // 服务器地址         
  
    private static final String FROM = sysProps.getProperty("FROM"); // 发件人的邮箱  
  
    private static final String USERNAME = sysProps.getProperty("USERNAME"); // 账号  
  
    private static final String PASSWORD = sysProps.getProperty("PASSWORD");; // 密码  
  
    
    /**
     * 发送简单邮件
     */
   public  static void  sendMail(String subject, String to, String content){
	      
	   //1.设置邮件环境
	   Properties props = new Properties();
	   props.setProperty("mail.transport.protocol", PROTOCOL);
	   props.setProperty("mail.host", HOST); 
	   props.setProperty("mail.smtp.auth", "true");
	   
	   
	   //2.获取session连接对象
	   Session session = Session.getInstance(props,new Authenticator() {
		@Override
		protected PasswordAuthentication getPasswordAuthentication() {
			
			return new PasswordAuthentication(USERNAME,PASSWORD);
		}
		   
	   });
	   session.setDebug(true);
	    //3. 创建邮件
	   Message message = new MimeMessage(session);
	   
	   try {
		   
		   message.setFrom(new InternetAddress(FROM));  //设置发件人   
		   message.setSubject(subject); //标题
		   message.addRecipient(RecipientType.TO, new InternetAddress(to)); //收件人
		   message.setSentDate(new Date()); //时间
		   message.setText(content);  //正文
		   
		   //3. 发送邮件
		   Transport.send(message);	
		   
	   } catch (MessagingException e) {		   
			e.printStackTrace();
			throw new CustomSystemException("1001", "邮件发送异常"); 
	   }
	   
	 
   }
   
   
   /**
    * 发送HTML邮件
    */
  public  static void  sendHTMLMail(String subject, String to, String content){
	   
	   //1.设置邮件环境
	   Properties props = new Properties();
	   props.setProperty("mail.transport.protocol", PROTOCOL);
	   props.setProperty("mail.host", HOST); 
	   props.setProperty("mail.smtp.auth", "true");
	   
	   
	   //2.获取session连接对象
	   Session session = Session.getInstance(props,new Authenticator() {
		@Override
		protected PasswordAuthentication getPasswordAuthentication() {
			
			return new PasswordAuthentication(USERNAME,PASSWORD);
		}
		   
	   });
	   session.setDebug(true);
	   
	   try {
		    //3. 创建邮件
		   Message message = new MimeMessage(session);
		   
		   message.setFrom(new InternetAddress(FROM));  //设置发件人
		   message.setSubject(subject); //标题
		   message.addRecipient(RecipientType.TO, new InternetAddress(to)); //收件人
		   message.setSentDate(new Date()); //时间		  
		   
		   MimeBodyPart bodyPart = new MimeBodyPart();
		   bodyPart.setContent(content,"text/html; charset=UTF-8");
		   
		   MimeMultipart mimeMultipart = new MimeMultipart();
		   mimeMultipart.addBodyPart(bodyPart);
		   
		   message.setContent(mimeMultipart);
		   
		   message.saveChanges(); //设置邮件内容与消息头一至
		   
		   //3. 发送邮件
		   Transport.send(message);
		
	   } catch (MessagingException e) {
		   
			e.printStackTrace();
			throw new CustomSystemException("1001", "邮件发送异常"); 
	   }
	   
	 
  }
  
  
  /**
   * 发送含有内嵌图像资源的邮件
   */
 public  static void  sendRelatedMail(String subject, String to, String content, String... relatedFilePath){
	   
	   //1.设置邮件环境
	   Properties props = new Properties();
	   props.setProperty("mail.transport.protocol", PROTOCOL);
	   props.setProperty("mail.host", HOST); 
	   props.setProperty("mail.smtp.auth", "true");
	   
	   
	   //2.获取session连接对象
	   Session session = Session.getInstance(props,new Authenticator() {
		@Override
		protected PasswordAuthentication getPasswordAuthentication() {
			
			return new PasswordAuthentication(USERNAME,PASSWORD);
		}
		   
	   });
	   session.setDebug(true);
	   
	   try {
		    //3. 创建邮件
		   Message message = new MimeMessage(session);
		   
		   message.setFrom(new InternetAddress(FROM));  //设置发件人
		   message.setSubject(subject); //标题
		   message.addRecipient(RecipientType.TO, new InternetAddress(to)); //收件人
		   message.setSentDate(new Date()); //时间
		   
		  
		   
		   //html文本信息
		   MimeBodyPart bodyPart1 = new MimeBodyPart();
		   bodyPart1.setContent(content+"<br /><img src=cid:img_1  />", "text/html; charset=UTF-8");		  		   
		   
		   
		   //图像信息
		   MimeBodyPart bodyPart2 = new MimeBodyPart();
		   bodyPart2.setDataHandler(new DataHandler(new FileDataSource(relatedFilePath[0])));
		   bodyPart2.setContentID("img_1");
		   
		   //组装
		   MimeMultipart mimeMultipart = new MimeMultipart();
		   mimeMultipart.addBodyPart(bodyPart1);
		   mimeMultipart.addBodyPart(bodyPart2);
		   message.setContent(mimeMultipart);
		   
		   //合并消息
		   message.saveChanges(); //设置邮件内容与消息头一至
		   
		   //3. 发送邮件
		   Transport.send(message);
		
	   } catch (MessagingException e) {
		   
			e.printStackTrace();
			throw new CustomSystemException("1001", "邮件发送异常"); 
	   }
	   
	 
 }
 
 
 
 
 /**
  * 发送含有内嵌图像资源的邮件
  */
public  static void  sendAttachMail(String subject, String to, String content, String... attachFilePath){
	   
	   //1.设置邮件环境
	   Properties props = new Properties();
	   props.setProperty("mail.transport.protocol", PROTOCOL);
	   props.setProperty("mail.host", HOST); 
	   props.setProperty("mail.smtp.auth", "true");
	   
	   
	   //2.获取session连接对象
	   Session session = Session.getInstance(props,new Authenticator() {
		@Override
		protected PasswordAuthentication getPasswordAuthentication() {
			
			return new PasswordAuthentication(USERNAME,PASSWORD);
		}
		   
	   });
	   session.setDebug(true);
	   
	   try {
		    //3. 创建邮件
		   Message message = new MimeMessage(session);
		   
		   message.setFrom(new InternetAddress(FROM));  //设置发件人
		   message.setSubject(subject); //标题
		   message.addRecipient(RecipientType.TO, new InternetAddress(to)); //收件人
		   message.setSentDate(new Date()); //时间
		   
		  
		   
		   //定义Html文本信息
		   MimeBodyPart bodyPart1 = new MimeBodyPart();
		   bodyPart1.setContent(content, "text/html; charset=UTF-8");		  		   
		   
		   
		   //附件
		   MimeBodyPart bodyPart2 = new MimeBodyPart();
		   bodyPart2.setDataHandler(new DataHandler(new FileDataSource(attachFilePath[0])));
		   bodyPart2.setFileName(attachFilePath[0].substring(attachFilePath[0].lastIndexOf("\\")+1));
		   
		   //组装
		   MimeMultipart mimeMultipart = new MimeMultipart();
		   mimeMultipart.addBodyPart(bodyPart1);
		   mimeMultipart.addBodyPart(bodyPart2);
		   message.setContent(mimeMultipart);
		   
		   //合并消息
		   message.saveChanges(); //设置邮件内容与消息头一至
		   
		   //3. 发送邮件
		   Transport.send(message);
		
	   } catch (MessagingException e) {		
			e.printStackTrace();
			throw new CustomSystemException("1001", "邮件发送异常"); 
	   }
	   
	 
  }
  
  
public static void main(String[] args) {
	  
	String activationCode  = UUIDUtils.getUUID();
	activationCode = "5ff6681cce0649158b4b3a87cd944050";
	String msg="<h3>点击下面链接激活账户</h3><h4><a href='"+MailUtils.DN+"/system/user/activate?code="+activationCode+"'>"+MailUtils.DN+"/system/user/activate?code="+activationCode+"</a><h4>";
	MailUtils.sendHTMLMail("官方账户注册激活邮件", "zhangwenchao729@163.com", msg);
}
  
    
    
	
	
}
```
