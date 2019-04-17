# Apache Veloctiy模板引擎示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年10月09日 17:23:40[boonya](https://me.csdn.net/boonya)阅读数：1079








首先，如果你对Velocity不是很了解，还是建议你去apache的官方网站上去走走....这是velocity的官网：[http://velocity.apache.org/](http://velocity.apache.org/)

当然如果你对英文文档不是很感冒，这里也有好的资料：

[Velocity 文档(1)](http://my.oschina.net/aiguozhe/blog/39553)
[Velocity 文档(2)](http://my.oschina.net/aiguozhe/blog/39556)
[Velocity 文档(3)](http://my.oschina.net/aiguozhe/blog/59321)

下面我就正式说说我做的项目啦...

项目结构：

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_velocity_str.png)

运行"helloWorld.vm"模板效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_velocity_hello.png)

运行"userInfo.vm"模板效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_velocity_user.png)

运行"emailTemplate.vm"模板效果：

![](http://images.cnblogs.com/cnblogs_com/hongten/458383/o_hongten_velocity_email.png)

# 代码部分

测试代码：



```java
/**
 * 
 */
package com.b510.velocity.test;

import java.io.StringWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;

import com.b510.velocity.bean.Mail;
import com.b510.velocity.bean.User;

/**
 * 测试类
 * 
 * @author hongten<br>
 * @date 2013-3-9
 */
public class VelocityTest {

    public static final String HELLO_WORLD_VM_PATH = "vms/helloWorld.vm";
    public static final String USER_INFO_VM_PATH = "vms/userInfo.vm";
    public static final String EMAIL_TEMPLATE_VM_PATH = "vms/emailTemplate.vm";

    public static void main(String[] args) throws Exception {
        sayHelloFromVM(HELLO_WORLD_VM_PATH);
        testUser(USER_INFO_VM_PATH);
        testEmail(EMAIL_TEMPLATE_VM_PATH);
    }

    /**
     * 简单的hello world
     * 
     * @param fileVM
     * @throws Exception
     */
    public static void sayHelloFromVM(String fileVM) throws Exception {
        VelocityEngine ve = new VelocityEngine();
        ve.init();
        Template t = ve.getTemplate(fileVM);
        VelocityContext context = new VelocityContext();
        context.put("hello", "Hello world!!");
        StringWriter writer = new StringWriter();
        t.merge(context, writer);
        System.out.println(writer.toString());
    }

    /**
     * test User
     * 
     * @param fileVM
     * @throws Exception
     */
    public static void testUser(String fileVM) throws Exception {
        VelocityEngine ve = new VelocityEngine();
        ve.init();

        Template template = ve.getTemplate(fileVM);
        VelocityContext velocityContext = new VelocityContext();
        User user = new User();
        user.setEmail("hongtenzone@foxmail.com");
        user.setName("hongten");
        user.setBirthday("1990-11-18");
        velocityContext.put("user", user);
        StringWriter stringWriter = new StringWriter();
        template.merge(velocityContext, stringWriter);

        System.out.println(stringWriter.toString());
    }

    /**
     * test email
     * 
     * @param fileVM
     * @throws Exception
     */
    public static void testEmail(String fileVM) throws Exception {
        VelocityEngine velocityEngine = new VelocityEngine();
        velocityEngine.init();

        Template template = velocityEngine.getTemplate(fileVM);
        VelocityContext velocityContext = new VelocityContext();
        Mail mail = new Mail();
        mail.setContent("2013年腾讯开发者新扶持政策解读及创业机会所在");
        mail.setReceiverMail("hongtenzone@foxmail.com");
        mail.setReceiverName("Hongten");
        mail.setSenderMail("opensns_noreply@tencent.com");
        mail.setSenderName("腾讯开放平台");
        mail.setSenderWebSite("open.qq.com");
        SimpleDateFormat simpleDateFormat = new SimpleDateFormat(
                "yyyy-MM-dd HH:mm:ss");
        mail.setDate(simpleDateFormat.format(new Date()));
        velocityContext.put("mail", mail);
        StringWriter stringWriter = new StringWriter();
        template.merge(velocityContext, stringWriter);

        System.out.println(stringWriter.toString());
    }
}
```


实体代码：





```java
/**
 * 
 */
package com.b510.velocity.bean;


/**
 * 用户实体类
 * 
 * @author hongten<br>
 * @date 2013-3-9
 */
public class User {

    /**
     * 用户编号
     */
    private Integer id;
    /**
     * 用户名称
     */
    private String name;
    /**
     * 密码
     */
    private String password;
    /**
     * 生日
     */
    private String birthday;
    /**
     * 邮箱
     */
    private String email;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getBirthday() {
        return birthday;
    }

    public void setBirthday(String birthday) {
        this.birthday = birthday;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

}

/**
 * 
 */
package com.b510.velocity.bean;

/**
 * 邮件
 * 
 * @author hongten<br>
 * @date 2013-3-9
 */
public class Mail {

    private Integer id;
    /**
     * 发件人
     */
    private String senderName;
    /**
     * 发件人邮箱
     */
    private String senderMail;
    /**
     * 发件人网址
     */
    private String senderWebSite;
    /**
     * 收件人
     */
    private String receiverName;
    /**
     * 收件人邮箱
     */
    private String receiverMail;
    /**
     * 内容
     */
    private String content;
    /**
     * 日期
     */
    private String date;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getSenderName() {
        return senderName;
    }

    public void setSenderName(String senderName) {
        this.senderName = senderName;
    }

    public String getSenderMail() {
        return senderMail;
    }

    public void setSenderMail(String senderMail) {
        this.senderMail = senderMail;
    }

    public String getReceiverName() {
        return receiverName;
    }

    public void setReceiverName(String receiverName) {
        this.receiverName = receiverName;
    }

    public String getReceiverMail() {
        return receiverMail;
    }

    public void setReceiverMail(String receiverMail) {
        this.receiverMail = receiverMail;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }

    public String getSenderWebSite() {
        return senderWebSite;
    }

    public void setSenderWebSite(String senderWebSite) {
        this.senderWebSite = senderWebSite;
    }

}
```

/Apache-Velocity-java/vms/helloWorld.vm




```
##test hello world!

$hello
```

/Apache-Velocity-java/vms/userInfo.vm




```
##测试User

A: what's your name?
B: $user.name

A: what's your birthday?
B: $user.birthday

A: what's your email address?
B: $user.email

A: good!
```

/Apache-Velocity-java/vms/emailTemplate.vm




```
##测试 email

$mail.senderName message notification
Sender       :   $mail.senderName<$mail.senderMail>        
Date         :   $mail.date
Receiver     :   $mail.receiverName<$mail.receiverMail>

Dear $mail.receiverMail:
$mail.senderName send a message notification as following:

$mail.content

please quick login the $mail.senderWebSite message center and have a look.

                                                      $mail.senderName Team
```




**因为velocity源码中默认的编码为"ISO-8859-1"：**



```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

# ----------------------------------------------------------------------------
# T E M P L A T E  E N C O D I N G
# ----------------------------------------------------------------------------

 input.encoding=ISO-8859-1
 output.encoding=ISO-8859-1

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



**所以，如果出现乱码我们可以设置velocity的编码格式：**

```
VelocityEngine velocityEngine = new VelocityEngine();
     velocityEngine.setProperty("input.encoding", "UTF-8");
     velocityEngine.setProperty("output.encoding", "UTF-8");
     velocityEngine.init();
```

**这样就可以解决velocity的乱码问题啦...**



**源码下载：**[http://files.cnblogs.com/hongten/Apache-Velocity-java.rar](http://files.cnblogs.com/hongten/Apache-Velocity-java.rar)

文章来源：[http://www.cnblogs.com/hongten/archive/2013/03/09/hongten_apache_velocity.html](http://www.cnblogs.com/hongten/archive/2013/03/09/hongten_apache_velocity.html)







