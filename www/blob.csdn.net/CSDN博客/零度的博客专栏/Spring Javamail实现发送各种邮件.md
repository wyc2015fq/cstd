# Spring Javamail实现发送各种邮件 - 零度的博客专栏 - CSDN博客
2016年04月06日 16:03:18[零度anngle](https://me.csdn.net/zmx729618)阅读数：5331
Spring邮件抽象层的主要包为org.springframework.mail。它包括了发送电子邮件的主要接口MailSender，和*值对象*SimpleMailMessage，它封装了简单邮件的属性如*from*, *to*,*cc*, *subject*,*text*。
 包里还包含一棵以MailException为根的checked Exception继承树，它们提供了对底层邮件系统异常的高级别抽象。 要获得关于邮件异常层次的更丰富的信息，请参考Javadocs。
为了使用*JavaMail*中的一些特色, 比如MIME类型的信件, Spring提供了MailSender的一个子接口, 即org.springframework.mail.javamail.JavaMailSender。Spring还提供了一个回调接口org.springframework.mail.javamail.MimeMessagePreparator, 
 用于准备JavaMail的MIME信件。
这里简单的介绍了如何使用spring发送各种形式的邮件以及配置。
1、在src目录下建立mail.properties文件里边包含一下内容
mail.host=smtp.exmail.qq.com(建议使用这个，如果使用smtp.qq.com可能会出现安全认证的问题
mail.username=你的邮箱名
mail.password=你的邮箱密码
mail.from=发送方的邮箱
2、使用spring配置
||<?xml version="1.0" encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:p="http://www.springframework.org/schema/p"xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd"><!-- 加载Properties文件 --><beanid="configurer"class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer"><propertyname="locations"><list><value>classpath:mail.properties</value></list></property></bean><beanid="mailMessage"class="org.springframework.mail.SimpleMailMessage"><propertyname="from"><value>${mail.from}</value></property><!-- 查看SimpleMailMessage源码还可以注入标题,内容等 --></bean><!-- 申明JavaMailSenderImpl对象 --><beanid="mailSender"class="org.springframework.mail.javamail.JavaMailSenderImpl"><propertyname="defaultEncoding"value="UTF-8"/><propertyname="host"value="${mail.host}"/><propertyname="username"value="${mail.username}"/><propertyname="password"value="${mail.password}"/><propertyname="javaMailProperties"><props><!-- 设置认证开关 --><propkey="mail.smtp.auth">true</prop><!-- 启动调试开关 --><propkey="mail.debug">true</prop><!-- 设置发送延时 --><propkey="mail.smtp.timeout">0</prop></props></property></bean></beans>|
##### [来自CODE的代码片](https://code.csdn.net/snippets/287433)
Spring_mail_applicationContext.xml
3、发送简单的邮件
||packagemail;importjavax.mail.MessagingException;importorg.springframework.context.ApplicationContext;importorg.springframework.context.support.ClassPathXmlApplicationContext;importorg.springframework.mail.MailSender;importorg.springframework.mail.SimpleMailMessage;/** * 本类测试邮件发送Html形式 *  * @author Eternity_._ *  */publicclassSingleMailSend{staticApplicationContextactx=newClassPathXmlApplicationContext("applicationContext.xml");staticMailSendersender=(MailSender)actx.getBean("mailSender");staticSimpleMailMessagemailMessage=(SimpleMailMessage)actx.getBean("mailMessage");publicstaticvoidmain(Stringargs[])throwsMessagingException{mailMessage.setSubject("你好");mailMessage.setText("这个是一个通过Spring框架来发送邮件的小程序");mailMessage.setTo("9197****1@qq.com");sender.send(mailMessage);}}|
##### [来自CODE的代码片](https://code.csdn.net/snippets/287436)
Spring_mail_SingleMail.java
4、发送带有图片的邮件，以嵌入HTML的方式
||packagespringmail;importjava.io.File;importjavax.mail.MessagingException;importjavax.mail.internet.MimeMessage;importorg.springframework.context.ApplicationContext;importorg.springframework.context.support.ClassPathXmlApplicationContext;importorg.springframework.core.io.FileSystemResource;importorg.springframework.mail.javamail.JavaMailSenderImpl;importorg.springframework.mail.javamail.MimeMessageHelper;publicclassSpringAttachedImageMail{publicstaticvoidmain(String[]args)throwsMessagingException{ApplicationContextctx=newClassPathXmlApplicationContext("applicationContext.xml");JavaMailSenderImplsender=(JavaMailSenderImpl)ctx.getBean("mailSender");MimeMessagemailMessage=sender.createMimeMessage();MimeMessageHelpermessageHelper=newMimeMessageHelper(mailMessage,true);messageHelper.setFrom("9197****1@qq.com");messageHelper.setTo("9197****1@qq.com");messageHelper.setSubject("测试邮件中嵌套图片!！");// true 表示启动HTML格式的邮件messageHelper.setText("<html><head></head><body><h1>hello!!spring image html mail</h1>"+"<a href=http://www.baidu.com>baidu</a>"+"<img src=cid:image/></body></html>",true);FileSystemResourceimg=newFileSystemResource(newFile("单.png"));messageHelper.addInline("image",img);//跟cid一致sender.send(mailMessage);System.out.println("邮件发送成功...");}}|
##### [来自CODE的代码片](https://code.csdn.net/snippets/287446)
Spring_mail_AttachedFileMail.java
5、发送带有附件的邮件
||packagespringmail;importjava.io.File;importjavax.mail.MessagingException;importjavax.mail.internet.MimeMessage;importorg.springframework.context.ApplicationContext;importorg.springframework.context.support.ClassPathXmlApplicationContext;importorg.springframework.core.io.FileSystemResource;importorg.springframework.mail.javamail.JavaMailSenderImpl;importorg.springframework.mail.javamail.MimeMessageHelper;publicclassSpringAttachedImageMail{publicstaticvoidmain(String[]args)throwsMessagingException{ApplicationContextctx=newClassPathXmlApplicationContext("applicationContext.xml");JavaMailSenderImplsender=(JavaMailSenderImpl)ctx.getBean("mailSender");MimeMessagemailMessage=sender.createMimeMessage();MimeMessageHelpermessageHelper=newMimeMessageHelper(mailMessage,true);messageHelper.setFrom("9197****1@qq.com");messageHelper.setTo("9197****1@qq.com");messageHelper.setSubject("测试邮件中嵌套图片!！");// true 表示启动HTML格式的邮件messageHelper.setText("<html><head></head><body><h1>hello!!spring image html mail</h1>"+"<a href=http://www.baidu.com>baidu</a>"+"<img src=cid:image/></body></html>",true);FileSystemResourceimg=newFileSystemResource(newFile("单.png"));messageHelper.addAttachment("单.png",file);//添加到附件sender.send(mailMessage);System.out.println("邮件发送成功...");}}|
##### [来自CODE的代码片](https://code.csdn.net/snippets/287459)
Spring_mail_AttachedFileMail.java
6、如果使用的是QQ邮箱的话建议把这几项都选上，不然可能调试不能通过。
![](https://img-blog.csdn.net/20140411212600625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZXRlcm5pdHkwXzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
附：所需的jar：[http://download.csdn.net/detail/eternity0_0/7179617](http://download.csdn.net/detail/eternity0_0/7179617)
