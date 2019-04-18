# Spring3.2下使用JavaMailSenderImpl类发送邮件 - z69183787的专栏 - CSDN博客
2016年12月19日 17:15:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：869
1、JavaMailSenderImpl类
　　Spring的邮件发送的核心是MailSender接口，在Spring3.0中提供了一个实现类JavaMailSenderImpl，这个类是发送邮件的核心类。
可以通过在配置文件中配置使用，当然也可以自己硬编码到代码中。
2、准备工作
　　搭建spring环境，此处默认学习者已搭建成功。
　　下载JavaMail jar包，导入工程。
[http://www.oracle.com/technetwork/java/javamail/index-138643.html](http://www.oracle.com/technetwork/java/javamail/index-138643.html)
3、案例代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import javax.mail.MessagingException;
import javax.mail.internet.MimeMessage;
import org.apache.velocity.app.VelocityEngine;
import org.apache.velocity.exception.VelocityException;
import org.springframework.core.io.ClassPathResource;
import org.springframework.core.io.FileSystemResource;
import org.springframework.mail.SimpleMailMessage;
import org.springframework.mail.javamail.JavaMailSenderImpl;
import org.springframework.mail.javamail.MimeMessageHelper;
import org.springframework.ui.velocity.VelocityEngineFactoryBean;
import org.springframework.ui.velocity.VelocityEngineUtils;
public class SpringMailSender {
    // Spring的邮件工具类，实现了MailSender和JavaMailSender接口
    private JavaMailSenderImpl mailSender;
    //使用Velocity模板，需要Velocity的jar包，然后需要声明一个VelocityEngine对象
    private VelocityEngine velocityEngine;
    
    /**
     * 创建邮件发送器
     */
    public SpringMailSender() {
        // 初始化JavaMailSenderImpl，当然推荐在spring配置文件中配置，这里是为了简单
        mailSender = new JavaMailSenderImpl();
        // 设置参数
        mailSender.setHost("smtp.qq.com");
        mailSender.setUsername("123456789@qq.com");
        mailSender.setPassword("555555");
        
        //声明一个VelocityEngine对象后，需要在构造函数中初始化（IoC is optional）
        // Velocity的参数，通过VelocityEngineFactoryBean创建VelocityEngine，也是推荐在配置文件中配置的
        Properties props = System.getProperties();
        props.put("resource.loader", "class");
        props.put("class.resource.loader.class",
                        "org.apache.velocity.runtime.resource.loader.ClasspathResourceLoader");
        VelocityEngineFactoryBean v = new VelocityEngineFactoryBean();
        v.setVelocityProperties(props);
        try {
            velocityEngine = v.createVelocityEngine();
        } catch (VelocityException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    /**
     * 
    * 方法名: simpleSend
    * 方法作用: TODO 简单邮件发送
    * 创建人：Jxufe HeHaiYang
    * 创建时间：2015-2-7 下午06:47:30   
    * @param @throws Exception    
    * 返回值类型： void    
    * @throws
     */
    public void simpleSend() throws Exception {
        // 构建简单邮件对象
        //SimpleMailMessages实现了MimeMessageHelper，为普通邮件模板，支持文本。
        SimpleMailMessage smm = new SimpleMailMessage();
        // 设定邮件参数
        smm.setFrom(mailSender.getUsername());
        smm.setTo("234567890@qq.com");
        smm.setSubject("Hello world");
        smm.setText("nice !");
        // 发送邮件
        mailSender.send(smm);
    }
    
    
    /**
     * 
    * 方法名: attachedSend
    * 方法作用: TODO 带附件的邮件发送
    * 创建人：Jxufe HeHaiYang
    * 创建时间：2015-2-7 下午06:47:22   
    * @param @throws MessagingException    
    * 返回值类型： void    
    * @throws
     */
    public void attachedSend() throws MessagingException {
        //使用JavaMail的MimeMessage，支付更加复杂的邮件格式和内容
        //MimeMessages为复杂邮件模板，支持文本、附件、html、图片等。
        MimeMessage msg = mailSender.createMimeMessage();
        //创建MimeMessageHelper对象，处理MimeMessage的辅助类
        MimeMessageHelper helper = new MimeMessageHelper(msg, true);
        //使用辅助类MimeMessage设定参数
        helper.setFrom(mailSender.getUsername());
        helper.setTo("234567890@qq.com");
        helper.setSubject("Hello Attachment");
        helper.setText("This is a mail with attachment");
        //加载文件资源，作为附件
        //文件地址相对应src目录
        ClassPathResource file = new ClassPathResource("ehcache.xsd");
        //加入附件
        helper.addAttachment("ehcache.xsd", file);
        
        //发送邮件
        mailSender.send(msg);
    }
    
    /**
     * 
    * 方法名: richContentSend
    * 方法作用: TODO 发送富文本邮件
    * 创建人：Jxufe HeHaiYang
    * 创建时间：2015-2-7 下午06:47:14   
    * @param @throws MessagingException    
    * 返回值类型： void    
    * @throws
     */
    public void richContentSend() throws MessagingException {
        MimeMessage msg = mailSender.createMimeMessage();
     
        MimeMessageHelper helper = new MimeMessageHelper(msg, true);
     
        helper.setFrom(mailSender.getUsername());
        helper.setTo("234567890@qq.com");
        helper.setSubject("邮件标题");
        //第二个参数true，表示text的内容为html，然后注意<img/>标签，src='cid:file'，'cid'是contentId的缩写，'file'是一个标记，
需要在后面的代码中调用MimeMessageHelper的addInline方法替代成文件
        helper.setText(
                "<body><p style='color:red;'>Hello Html Email</p><img src='cid:file'/></body>",
                true);
        //文件地址相对应src目录
        ClassPathResource file = new ClassPathResource("logo.png");
        //文件地址对应系统目录
//        FileSystemResource file = new FileSystemResource("C:\\Users\\HIYOUNG\\Desktop\\logo.png");
        helper.addInline("file", file);
     
        mailSender.send(msg);
    }
    
    /**
     * 
    * 方法名: templateSend
    * 方法作用: TODO  使用Velocity模板发送邮件
    * 创建人：Jxufe HeHaiYang
    * 创建时间：2015-2-7 下午06:47:05   
    * @param @throws MessagingException    
    * 返回值类型： void    
    * @throws
     */
    public void templateSend() throws MessagingException {
        // 声明Map对象，并填入用来填充模板文件的键值对
        Map<String, Object> model = new HashMap<String, Object>();
        model.put("user", "hehaiyang");
        model.put("content", "good evening !");
        //Spring提供的VelocityEngineUtils将模板进行数据填充，并转换成普通的String对象
        //文件地址相对应src目录
        String emailText = VelocityEngineUtils.mergeTemplateIntoString(
                velocityEngine, "/velocity/mail.vm", model);
        // 和上面一样的发送邮件的工作
        MimeMessage msg = mailSender.createMimeMessage();
        MimeMessageHelper helper = new MimeMessageHelper(msg, true);
        helper.setFrom(mailSender.getUsername());
        helper.setTo("234567890@qq.com");
        helper.setSubject("Rich content mail");
        helper.setText(emailText, true);
     
        mailSender.send(msg);
    }
    
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
Velocity模板mail.vm
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<html>
<head>
<style type="text/css">
h4{
    color:red;
    background:#efefef;
}
</style>
</head>
<body>
　　<h4>${user} </h4>
　　<p><p>
　　<i>${content}</i>
</body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
4、可能遇到的问题
异常1：
```
spring 发送邮件时遇到异常org.springframework.mail.MailAuthenticationException
org.springframework.mail.MailAuthenticationException: Authentication failed; nested exception is javax.mail.AuthenticationFailedException
```
解决方法：
　　如果遇到所示异常，请检查你所配置的发送邮箱，有没有对外开放POP3/SMTP服务。
异常2：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    Exception in thread "main" java.lang.NoClassDefFoundError: com/sun/mail/util/LineInputStream  
        at javax.mail.Session.loadProvidersFromStream(Session.java:928)  
        at javax.mail.Session.access$000(Session.java:174)  
        at javax.mail.Session$1.load(Session.java:870)  
        at javax.mail.Session.loadResource(Session.java:1084)  
        at javax.mail.Session.loadProviders(Session.java:889)  
        at javax.mail.Session.<init>(Session.java:210)  
        at javax.mail.Session.getInstance(Session.java:249)  
   
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
解决方法：
如果你使用的是JDK1.5以上环境，那么你应该会遇到所示异常，因为环境下javaee.jar包与java mail 包有冲突。
　　找到javaee.jar包删除里面的mail 包以及mail下属的包，去除冲突。
5、测试
　　更改SpringMailSender调用的方法，逐个测试发送邮件。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public class SenderTest {
     public static void main(String[] args){
        try {
            SpringMailSender m=new SpringMailSender();
            m.richContentSend();
            System.out.println("发送成功!");
        } catch (Exception e) {
            System.out.println(e.toString());
        }
        
     }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
