# 邮件：事务失败。 服务器响应为:DT:SPM 163 smtp - 左直拳的马桶_日用桶 - CSDN博客
2017年01月03日 19:21:14[左直拳](https://me.csdn.net/leftfist)阅读数：31414
几年前我做的一个项目，日发邮件最高峰时几十万。自以为对邮件发送方面已经有了一定认识，所以近期机缘巧合之下，又有项目需要发送邮件，不禁自信满满，暗自庆幸能不手到擒来乎？
不想老革命遇到新问题。我原先的邮件发送机制，是生成eml文件，然后扔到windows自带的smtp服务器的pickup目录下，系统即自动发送（见拙作：[邮件发送一点心得](http://blog.csdn.net/leftfist/article/details/2078034)）。
但今时今日，这些好像都是老皇历了。IIS7默认并没有SMTP，如果操作系统是server，还可以通过添加功能加上去，但WIN7就不行了。偏偏，我们的系统就部署在WIN7下。
我第一个应对之道是看WIN7能不能装那个自带的SMTP，结果是不行，死活没找到。
第二个办法是装个第三方的SMTP，找来找去，找到一个叫做什么FREE SMTP的，装上去以后，打开看，好像忒简单了点。根本没有啥PICK目录的，应该是只能通过代码，搞什么几次握手之类的方法来进行调用。我很烦这个东西。
最后，决定还是直接使用smtp.163.com。网上例子比比皆是。
```
public interface IEmail
{
    bool Send(EmailParam param);
    string Mess { get; }
}
public class EmailParam
{
    public string From { get; set; }
    public string To { get; set; }//接收人；多个邮箱用分号隔开
    public string CC { get; set; }//抄送；多个邮箱用分号隔开
    public string Subject { get; set; }//标题
    public string Body { get; set; }//邮件正文
}
public class Sender : IEmail
{
    SmtpClient client;
    public Sender()
    {
        // <add key="eml_host" value="smtp.163.com"/>
        // <add key="eml_account" value="chendaqu"/>
        // <add key="eml_pwd" value="1234abcd"/>
        string host = ConfigurationManager.AppSettings["eml_host"];
        string account = ConfigurationManager.AppSettings["eml_account"];
        string pwd = ConfigurationManager.AppSettings["eml_pwd"];
        client = new SmtpClient();
        client.Host = host;
        client.UseDefaultCredentials = false;
        client.DeliveryMethod = SmtpDeliveryMethod.Network;
        client.Credentials = new System.Net.NetworkCredential(account,pwd);
        client.Port = 25;
    }
    public string Mess { get; set; }
    public bool Send(EmailParam param)
    {
        MailMessage mail = new MailMessage();
        mail.From = new MailAddress(param.From);
        getCollection(mail.To,param.To);
        getCollection(mail.CC, param.CC);
        mail.Subject = param.Subject;
        mail.Body = param.Body;
        mail.SubjectEncoding = Encoding.UTF8;
        mail.BodyEncoding = Encoding.UTF8;
        mail.Priority = MailPriority.Normal;
        mail.IsBodyHtml = true;
        mail.DeliveryNotificationOptions = DeliveryNotificationOptions.OnFailure;
        try
        {
            client.Send(mail);
        }
        catch(Exception ex)
        {
            Mess = ex.Message;
            return false;
        }
        return true;
    }
    MailAddressCollection getCollection(MailAddressCollection collection,string arstring)
    {
        if (!String.IsNullOrEmpty(arstring))
        {
            string[] ar = arstring.Split(';');
            foreach (string item in ar)
            {
                if (String.IsNullOrEmpty(item)) continue;
                collection.Add(item);
            }
        }
        return collection;
    }
}
```
使用单元测试进行调用：
```
[TestMethod()]
public void SendTest()
{
    IEmail sender = new Sender();
    bool ok = sender.Send(new EmailParam()
    {
        Subject = "邮件测试7",
        Body = "Hello World!",
        From = "chendaqu@163.com",
        To = "178879771@qq.com;461198190@qq.com",
    });
    Assert.AreEqual(true, ok);
}
```
结果刚开始非常顺利，都能收到邮件；但该单元测试运行了几次以后，就发送失败了。提示：
`错误：邮件：事务失败。 服务器响应为:DT:SPM 163 smtp11,D8CowACXNqusR1pYQe3vGA--.16469S2 1482311598,please see http://mail.163.com/help/help_spam_16.htm?ip=219.136.75.223&hostid=smtp11&time=1482311598`
开始时以为是因为163邮箱的反垃圾邮件机制，猛查资料，结果一无所获。焦虑攻心之下，我第二天居然感冒了。后来才知道是QQ邮箱的发垃圾邮件机制所致。同一个发送请求中，有一个地址失败，其他的都失败了，所以称为“事务”？
后来就是莫名其妙地好了，可能是邮件内容有所变化，QQ又认为它不是垃圾了。靠。
注： 
我这个163邮箱，开启了客户端授权，未清楚对应对反垃圾机制是否有用，好像不开启这个客户端授权也能发送。开启了这个客户端授权后，代码中的密码要使用这个客户端授权密码，而不是登录邮箱的密码。 
![这里写图片描述](https://img-blog.csdn.net/20170104113115166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
================================== 
以上就是垃圾。这个问题等于没解决。163的SMTP一言不合就将你的邮件当成垃圾邮件，直到第二天才解封。但是很奇怪，采用foxmail作为客户端来发送同样的邮件，却没有问题，说到底还是代码的问题？（outlook没试过，估计也不会有问题）
