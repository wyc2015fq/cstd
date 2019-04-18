# commons-email && Java mail 邮件发送 附件文件名过长的问题 版本1.5及以上 - z69183787的专栏 - CSDN博客
2018年02月02日 14:19:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：712
[http://blog.csdn.net/albert0707/article/details/69284700](http://blog.csdn.net/albert0707/article/details/69284700)
在使用javamail 1.5版本的时候，如果附件文件名过长，会错误显示为“未命名的附件 00439.dat”
解决方案如下：
一、Javamail jar包退回1.4版本
二、修改参数mail.mime.splitlongparameters为 false
1、添加以下红色代码
messageBodyPart.setFileName(**MimeUtility.encodeText**(docName));
2、
在new MimeMessage、new MimeMultipart、new MimeBodyPart 之前（一个比较靠前的位置，如果在new MimeMultipart之后添加，有可能无效），添加如下红色代码。
**修改完成后记得重启Tomcat**
System.getProperties().setProperty("mail.mime.splitlongparameters", "false");
Properties props = new Properties();
**[java]**[view
 plain](http://blog.csdn.net/albert0707/article/details/69284700#)[copy](http://blog.csdn.net/albert0707/article/details/69284700#)
- try {  
-             props.put("mail.smtp.host", host);  
-             props.put("mail.smtp.auth", "true");  
-             props.put("username", from);  
-             props.put("password", pwd);  
- 
- //          System.getProperties().setProperty("mail.mime.encodefilename", "true");
- //          System.getProperties().setProperty("mail.mime.decodefilename", "true");
- //          System.getProperties().setProperty("mail.mime.encodeparameters", "false");
- //          System.getProperties().setProperty("mail.mime.decodeparameters", "false");
- 
- 
-             System.getProperties().setProperty("mail.mime.splitlongparameters", "false");  
- 
- 
-             MailAuthenticatorService auth = new MailAuthenticatorService(from,  
-                     pwd);  
-             Session session = Session.getInstance(props, auth);  
-             session.setDebug(true);  
- //          session.setDebug(false);
-             Transport transport = null;  
-             MimeMessage message = new MimeMessage(session);  
-             message.setFrom(new InternetAddress(from));  
-             transport = session.getTransport("smtp");  
-             InternetAddress[] addresses = new InternetAddress[receiver.size()];  
- for (int i = 0; i < receiver.size(); i++) {  
-                 addresses[i] = new InternetAddress(receiver.get(i)  
-                         .getMailAddress());  
-                 logger.debug("==>receiver mail address: "
-                         + receiver.get(i).getMailAddress());  
-             }  
-             message.setRecipients(Message.RecipientType.TO, addresses);  
-             message.setSubject(theme);  
-             Multipart multipart = new MimeMultipart();  
-             BodyPart contentPart = new MimeBodyPart();  
-             contentPart.setText(content);  
-             multipart.addBodyPart(contentPart);  
-             BodyPart messageBodyPart = new MimeBodyPart();  
-             DataSource source = new FileDataSource(attachFilePath);  
-             messageBodyPart.setDataHandler(new DataHandler(source));  
-             docName = "中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文中文.doc";  
- //          docName = "asdfghjklpasdfghjklpasdfghjklpasdfghjklpasdfghjklpasdfghjklpasdfghjklpasdfghjklpasdfghjklpasdfghjklpasdfghjklp.doc";
- //          docName = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890.doc";
- 
- //          messageBodyPart.setFileName(docName);
-             messageBodyPart.setFileName(MimeUtility.encodeText(docName));  
- 
- 
- 
-             multipart.addBodyPart(messageBodyPart);  
-             message.setContent(multipart);  
-             message.saveChanges();  
-             transport.connect(host, user, pwd);  
-             transport.sendMessage(message, message.getAllRecipients());  
-             transport.close();  
-             logger.debug("email send finish");  
- 
-         } catch (Exception e) {  
-             e.printStackTrace();  
-             logger.error(e.getMessage());  
-             logger.error("email send error");  
-         }  
- 
