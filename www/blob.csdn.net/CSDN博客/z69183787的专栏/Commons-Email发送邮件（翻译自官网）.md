# Commons-Email发送邮件（翻译自官网） - z69183787的专栏 - CSDN博客
2016年09月05日 15:47:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3821
所需jar包：
```java
<dependency>
    <groupId>org.apache.commons</groupId>
    <artifactId>commons-email</artifactId>
    <version>1.3.1</version>
</dependency>
```
```
<dependency>
    <groupId>commons-logging</groupId>
    <artifactId>commons-logging</artifactId>
    <version>1.1.1</version>
</dependency>
```
1、发送简单文本
```java
Email email = new SimpleEmail();
email.setHostName("smtp.googlemail.com");
email.setSmtpPort(465);
email.setAuthenticator(new DefaultAuthenticator("username", "password"));
email.setSSLOnConnect(true);
email.setFrom("user@gmail.com");
email.setSubject("TestMail");
email.setMsg("This is a test mail ... :-)");
email.addTo("foo@bar.com");
email.send();
```
setHostName("mail.myserver.com")设置出口smtp地址，也可以在system.property系统变量mail.host中设置
2、文本+附件
```java
import org.apache.commons.mail.*;
...
  // Create the attachment
  EmailAttachment attachment = new EmailAttachment();
  attachment.setPath("mypictures/john.jpg");
  attachment.setDisposition(EmailAttachment.ATTACHMENT);
  attachment.setDescription("Picture of John");
  attachment.setName("John");
  // Create the email message
  MultiPartEmail email = new MultiPartEmail();
  email.setHostName("mail.myserver.com");
  email.addTo("jdoe@somewhere.org", "John Doe");
  email.setFrom("me@apache.org", "Me");
  email.setSubject("The picture");
  email.setMsg("Here is the picture you wanted");
  // add the attachment
  email.attach(attachment);
  // send the email
  email.send();
```
你也可以用使用EmailAttachment发送有效url的附件，当邮件发送时会自动进行附件下载，以下是例子
```java
import org.apache.commons.mail.*;
...
  // Create the attachment
  EmailAttachment attachment = new EmailAttachment();
  attachment.setURL(new URL("http://www.apache.org/images/asf_logo_wide.gif"));
  attachment.setDisposition(EmailAttachment.ATTACHMENT);
  attachment.setDescription("Apache logo");
  attachment.setName("Apache logo");
  // Create the email message
  MultiPartEmail email = new MultiPartEmail();
  email.setHostName("mail.myserver.com");
  email.addTo("jdoe@somewhere.org", "John Doe");
  email.setFrom("me@apache.org", "Me");
  email.setSubject("The logo");
  email.setMsg("Here is Apache's logo");
  
  // add the attachment
  email.attach(attachment);
  // send the email
  email.send();
```
3、发送格式化HTML
```java
import org.apache.commons.mail.HtmlEmail;
...
  // Create the email message
  HtmlEmail email = new HtmlEmail();
  email.setHostName("mail.myserver.com");
  email.addTo("jdoe@somewhere.org", "John Doe");
  email.setFrom("me@apache.org", "Me");
  email.setSubject("Test email with inline image");
  
  // embed the image and get the content id
  URL url = new URL("http://www.apache.org/images/asf_logo_wide.gif");
  String cid = email.embed(url, "Apache logo");
  
  // set the html message
  email.setHtmlMsg("<html>The apache logo - <img src=\"cid:"+cid+"\"></html>");
  // set the alternative message
  email.setTextMsg("Your email client does not support HTML messages");
  // send the email
  email.send();
```
embed()返回值为一个随机生成的字符串，用来表示引用的图片标签；同时相应的取消了setmsg ，改成 setHtmlMsg与setTextMsg区分html与纯文本
4、发送格式为html及内嵌附件
解决上3中需要提供cid并拼接图片的问题，使用模板化html ImageHtmlEmail  发出，解决了需要将外部图片转换为inline内联图片的问题
```java
import org.apache.commons.mail.HtmlEmail;
...
  // load your HTML email template
  String htmlEmailTemplate = ".... <img src=\"http://www.apache.org/images/feather.gif\"> ....";
  // define you base URL to resolve relative resource locations
  URL url = new URL("http://www.apache.org");
  // create the email message
  ImageHtmlEmail email = new ImageHtmlEmail();
  email.setDataSourceResolver(new DataSourceUrlResolver(url));
  email.setHostName("mail.myserver.com");
  email.addTo("jdoe@somewhere.org", "John Doe");
  email.setFrom("me@apache.org", "Me");
  email.setSubject("Test email with inline image");
  
  // set the html message
  email.setHtmlMsg(htmlEmailTemplate);
  // set the alternative message
  email.setTextMsg("Your email client does not support HTML messages");
  // send the email
  email.send();
```
首先我们声明了一个html模板并包含图片url，所有图片都会自动转化为内联图片，通过DataSourceResolver解析器
最后附上自己使用的例子：
```java
HtmlEmail email = new HtmlEmail ();
        email.setHostName("mail.xxxx.com");
        email.setAuthentication("leo.li@xxxx.com", "1qaz!QAZ");
        email.setCharset("UTF-8");
        try {
            String[] receivers = new String[]{"leo.li@xxxx.com", "zs.zhou@xxxx.com"};
            for (String receiver : receivers) {
                email.addTo(receiver.trim());
            }
            email.setFrom("leo.li@xxxx.com", "leoeleoeleole");
            email.setSubject("test");
            email.setMsg("<html><div style=\"background:red;\">草草草擦凑啊</div></html>");
List<String> list = new ArrayList<String>();
            list.add("C:\\Users\\Administrator\\Desktop\\0010164 xxxx表.xls");
            if (!CollectionUtils.isEmpty(list)) {
                for (String attachment : list) {
                    EmailAttachment attach = new EmailAttachment();
                    File f = new File(attachment);
                    attach.setName(MimeUtility.encodeText(f.getName()));
                    attach.setPath(attachment);
                    email.attach(attach);
                }
            }
           String result  = email.send();
```
