
# C\# 网络编程之基于SMTP发送电子邮件 - 杨秀璋的专栏 - CSDN博客

2014年06月21日 04:17:09[Eastmount](https://me.csdn.net/Eastmount)阅读数：6175标签：[C\#网络编程																](https://so.csdn.net/so/search/s.do?q=C#网络编程&t=blog)[smtp发送邮件																](https://so.csdn.net/so/search/s.do?q=smtp发送邮件&t=blog)[SmtpClient																](https://so.csdn.net/so/search/s.do?q=SmtpClient&t=blog)[MailMessage																](https://so.csdn.net/so/search/s.do?q=MailMessage&t=blog)[方法及源码																](https://so.csdn.net/so/search/s.do?q=方法及源码&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=MailMessage&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=SmtpClient&t=blog)个人分类：[C\#网络编程																](https://blog.csdn.net/Eastmount/article/category/1514655)
[
																					](https://so.csdn.net/so/search/s.do?q=SmtpClient&t=blog)所属专栏：[C\# 网络编程知识](https://blog.csdn.net/column/details/eastmount-wl.html)[
							](https://so.csdn.net/so/search/s.do?q=SmtpClient&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=smtp发送邮件&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=C#网络编程&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=C#网络编程&t=blog)

**本文主要讲述基于C\#网络编程的发送邮件的编程,邮件发送功能是基于邮件协议的,常见的电子邮件协议有SMTP(简单邮件传输协议)、POP3(邮局协议)、IMAP(Internet邮件访问协议),文章主要参考周存杰的《C\#网络编程实例教程》.这也是最后一篇参照该书的网络编程文章,之后的该系列文章都是基于网络实际应用的,不会再大量讲述原理知识.**
## 一.SMTP协议
**SMTP协议是TCP/IP协议家族定义的机器间交换邮件的标准,它主要负责底层邮件系统如何将一个报文从一台机器传输到另一台机器,而不关心邮件如何存储及传输速度.它的步骤基本如下:**
**客户端首先建立与服务器TCP连接,然后服务器发送220报文(服务就绪),客户端收到220报文后发送HELLO命令,服务器收到HELLO后作出响应,最后服务器和客户端可以开始邮件通信.**
**本文不准备对SMTP命令(该命令定义了邮件传输或用户定义的系统功能)和SMTP应答状态码进行描述,你只需要知道使用SMTP发送邮件的操作通常如下:**
**1.由MAIL命令(用于将邮件发送到一个多个邮箱)开始给出发送者标识,RCPT命令(用于确定邮件内容的接受者)给出接受者信息.**
**2.用DATA命令(用于邮件内容加入缓冲区,<CRLF>.<CRLF>标识结束)列出发送邮件的内容.**
**3.邮件内容指示符确认操作,如果此命令被接受,接收方返回一个250 OK应答.**
**.NET平台上SmtpMail类实现了对SMTP协议的封装.所以我主要通过这些类介绍如何SMTP邮件收发.**
**POP3:Post Office Protocol,目前版本为POP3,它是把邮件从电子邮箱中传输到本地计算机的协议.**
**IMAP:Internet Message Access Protocol,是POP3的一种替代协议,提供了邮件检索和邮件处理的新功能,用户可以完全不必下载邮件正文就可以看到邮件的标题摘要,从邮件客户端软件就可以对服务器上的邮件和文件夹目录进行操作.**
## 二.SMTP协议封装的类(旧版)
## 1.
## SmtpMail类
## 该类用于发送邮件,其命名空间为System.Web.Mail.该类常用属性只有一个:
## public static string SmtpServer {get; set;}
## 获取或设置SMTP服务器名称,如不设置将使用本地主机名.如果提示添加命名空间不存在,需要在项目中添加引用,添加System.Web.dll即可.
```python
//SMTP类常用方法
//构造方法
public SmtpMail();
//Send()方法 发送邮件 返回值：无
public static void Send(
    MailMessage message //邮件
);
public static void Send(
    string from,        //发送者地址
    string to,          //收信人地址
    string subject,     //邮件主题
    string messageText  //邮件内容
);
```
**2.MailMessage类**
**其命名控件为System.Web.Mail,该类用于设置邮件内容以及与邮件内容相关的信息,如发送人地址、收信人地址等.**
```python
//MailMessage常用属性
//1.Attachments属性 用于获取附件的文件列表
public IList Attachments {get;}
//2.Bcc属性 用于获取或设置暗送于地址,被暗送者的地址
public string Bcc {get; set;}
//3.Body属性 用于获取或设置邮件内容
public string Body {get; set;}
//4.BodyFormat属性 用于获取或设置邮件内容的格式 有Html Text格式
public MailFormat BodyFormat {get; set;}
//5.Cc属性 用于获取或设置抄送于地址,被抄送者的地址
public string Cc {get; set;}
//6.From属性 用于获取或设置发信人的地址
public string From {get; set;}
//7.Headers属性 用于获取邮件标头
public IDictionary Headers {get;}
//8.Priority属性 用于获取或设置邮件的优先级,包括High Low Normal
public MailPriority Priority {get; set;}
//9.Subject属性 用于获取或设置邮件的主题
public string Subject {get; set;}
//10.To属性 用于获取或设置邮件的收信人地址
public string To {get; set;}
//构造方法
 public MailMessage();
```
**3.MailAttachment类**
**该类用于构造和设置邮件的附件,命名空间为System.Web.Mail.**
```python
//MailAttachment类常用属性
//Encoding属性 用于设置附件的编码
public MailEncoding Encoding {get;}
//Filename属性 用于设置附件的文件名
public string Filename {get;}
//构造方法
public MailAttachment(
    string filename        //文件名
);
public MailAttachment(
    string filename,       //文件名
    MailEncoding encoding  //编码
);
```
## 三.SMTP协议封装的类(新版)
**使用旧版的SMTP可能会出现很多错误,譬如“SendUsing 配置值无效”或“服务器未响应”等,但是它与新版的方法及属性基本没有变换.**
**C\#更新的smtp命名空间为System.Net.Mail,其中SmtpClient类用于smtp发送邮件.**
**1.Host属性用于设置主机名或IP地址**
**2.Port属性用于设置SMTP事务端口**
**3.Credentials属性用于验证寄件者的认证**
**4.DeliveryMethod属性用于指定如何处理待送的电子邮件讯息**
**5.Send方法用于将电子邮件发送到 SMTP 服务器以便传递**
**MailMessage类中属性也有变化,如To属性(邮件收信人地址属性)变为只读属性需要通过Add函数添加收件人邮件地址,但大体都与旧版的相同.**
**Attachment类是在System.Net.Mail中新增,表示电子邮件的附件,以前System.Web.Mail中的是MailAttachment类.下面是具体的发送邮件的实例源代码及效果:**
```python
//添加命名空间
using System.Net.Mail;
//添加私有成员
private MailMessage msg;      //用于构造邮件属性和方法
private Attachment att;       //用于构造邮件附件属性和方法
public Form1()
{
    InitializeComponent();
    msg = new MailMessage();  //实例化
}
//点击"发送邮件"按钮
private void button1_Click(object sender, EventArgs e)
{
    try
    {
        //To邮件收信人地址属性 只读属性不能赋值
        msg.To.Add(textBox1.Text);
        //From邮件发件人地址属性
        msg.From = new MailAddress(textBox2.Text);
        //Subject邮件主题属性
        msg.Subject = textBox3.Text;
        msg.SubjectEncoding = Encoding.Default;
        //Body设置邮件内容属性
        msg.Body = richTextBox1.Text;
        msg.BodyEncoding = Encoding.Default;
        //设置邮件的优先级Priority属性
        if (radioButton1.Checked)
            msg.Priority = MailPriority.High;
        else if (radioButton2.Checked)
            msg.Priority = MailPriority.Low;
        else if (radioButton3.Checked)
            msg.Priority = MailPriority.Normal;
        else
            msg.Priority = MailPriority.Normal;
        //发送邮件
        SmtpClient client = new SmtpClient();
        //邮件服务器设置smtp端口 默认25
        client.Host = "smtp.163.com";           
        client.Port = 25;                       
        //邮件发送方式 通过网络发送到SMTP服务器
        client.DeliveryMethod = System.Net.Mail.SmtpDeliveryMethod.Network;
        //凭证 发件人登录邮箱的用户名和密码
        client.Credentials = new System.Net.NetworkCredential("1520161xxxx", "19911203xxxx");
        client.Send(msg);
        MessageBox.Show("邮件发送成功！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);  
    }
    catch (Exception m) //异常处理
    {
        MessageBox.Show(m.Message);
    }
}
//点击"添加附件"按钮
private void button2_Click(object sender, EventArgs e)
{
    OpenFileDialog openFileDialog = new OpenFileDialog();
    openFileDialog.CheckFileExists = true;  //不存在文件名显示警告
    openFileDialog.ValidateNames = true;    //值接受Win32文件
    openFileDialog.Multiselect = false;     //不允许多选文件
    openFileDialog.Filter = "所有文件(*.*)|*.*";
            
    //添加附件 现仅支持添加一个附件
    if (openFileDialog.ShowDialog() == DialogResult.OK)
    {
        richTextBox1.Text = openFileDialog.FileName;
        att = new Attachment(openFileDialog.FileName);
        msg.Attachments.Add(att);
    }
}
//点击"删除附件"按钮
private void button3_Click(object sender, EventArgs e)
{
    msg.Attachments.Clear();
}
```
**运行结果如下图所示:**
![](https://img-blog.csdn.net/20140621035106656)
![](https://img-blog.csdn.net/20140621035405890)
**这仅仅是C\#使用SMTP发送邮件的基础知识,读者可以自己完善做成更好的界面.**
## 四.总结
**你也可以调用Windows自带的邮件发送程序实现,Windows自带Outlook Express软件,可以通过函数ShellExecute()或CreateProcess()调用Outlook Express,Ctrl+R调用"运行",输入“mailto:eastmount@163.com”即可调用发送邮件.使用该软件时首先需要测试账户,我绑定的是163邮箱.读者感兴趣可以自己完成.**
**本文主要讲述C\#网络编程中SMTP邮件协议,如何发送邮件的过程,并比较了新版的和旧版的方法.这将是我的C\#网络编程的最后一篇文章,接下来想学习C\#网络编程爬虫、下载网上图片、数据挖掘结合之类的知识.希望文章对大家有所帮助,如果有错误或不足之处,请海涵!目前法国VS瑞士 3:0.**
**(By:Eastmount 2014-6-21 夜4点 原创CSDN****http://blog.csdn.net/eastmount/****)**
**推荐相参考的关资料,非常不错,值得学习:**
**1.[C\# 网络编程系列]专题十：实现简单的邮件收发器 -- Learning_Hard**
**http://blog.csdn.net/learning_hard/article/details/9071041**
**2.C\#使用163的SMTP服务器发送邮件  -- PowerCoder**
**http://www.cnblogs.com/OpenCoder/archive/2010/07/16/1779247.html**
**3.C\#邮件下载 -- zhouquanandy**
**http://download.csdn.net/detail/zhouquanandy/4444802**
**4.《C\#网络编程实例教程》-- 周存杰**
**5.C\#发送邮件（添加附件）-- 期待秋天的叶**
**http://blog.csdn.net/kkkkkxiaofei/article/details/7941239**

