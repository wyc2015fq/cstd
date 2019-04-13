
# Python 学习入门（5）—— 发送邮件 - 阳光岛主 - CSDN博客

2013年10月26日 22:57:31[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10205个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



下载[smtplib.py](http://svn.python.org/projects/python/trunk/Lib/smtplib.py)或[smtplib.py](http://hg.python.org/cpython/file/3.3/Lib/smtplib.py)
下载[email-6.0.0a1.tar.gz](https://pypi.python.org/pypi/email)或[Mail-2.1.0.tar.gz](https://pypi.python.org/pypi/Mail)
**安装：**
$ tar zxvf email-6.0.0a1.tar.gz
$ cdemail-6.0.0a1
$ sudo python setup.py build
$ sudo python setup.py install

使用Python的email模块来实现带有附件的邮件的发送。
**SMTP**(Simple Mail Transfer Protocol)
邮件传送代理 (Mail Transfer Agent，MTA) 程序使用SMTP协议来发送电邮到接收者的邮件服务器。SMTP协议只能用来发送邮件，不能用来接收邮件。大多数的邮件发送服务器 (Outgoing Mail Server) 都是使用SMTP协议。SMTP协议的默认TCP端口号是25。
SMTP协议的一个重要特点是它能够接力传送邮件。它工作在两种情况下：一是电子邮件从客户机传输到服务器；二是从某一个服务器传输到另一个服务器。

**POP3**(Post Office Protocol) &**IMAP**(Internet Message Access Protocol)
POP协议和IMAP协议是用于邮件接收的最常见的两种协议。几乎所有的邮件客户端和服务器都支持这两种协议。
POP3协议为用户提供了一种简单、标准的方式来访问邮箱和获取电邮。使用POP3协议的电邮客户端通常的工作过程是：连接服务器、获取所有信息并保存在用户主机、从服务器删除这些消息然后断开连接。POP3协议的默认TCP端口号是110。
IMAP协议也提供了方便的邮件下载服务，让用户能进行离线阅读。使用IMAP协议的电邮客户端通常把信息保留在服务器上直到用户显式删除。这种特性使得多个客户端可以同时管理一个邮箱。IMAP协议提供了摘要浏览功能，可以让用户在阅读完所有的邮件到达时间、主题、发件人、大小等信息后再决定是否下载。IMAP协议的默认TCP端口号是143。

**邮件格式 (****[RFC 2822](http://tools.ietf.org/html/rfc2822.html)****)**
每封邮件都有两个部分：邮件头和邮件体，两者使用一个空行分隔。
邮件头每个字段 (Field) 包括两部分：字段名和字段值，两者使用冒号分隔。
有两个字段需要注意：From和Sender字段。From字段指明的是邮件的作者，Sender字段指明的是邮件的发送者。如果From字段包含多于一个的作者，必须指定Sender字段；如果From字段只有一个作者并且作者和发送者相同，那么不应该再使用Sender字段，否则From字段和Sender字段应该同时使用。
邮件体包含邮件的内容，它的类型由邮件头的Content-Type字段指明。RFC 2822定义的邮件格式中，邮件体只是单纯的ASCII编码的字符序列。

**MIME**(Multipurpose Internet Mail Extensions) ([RFC 1341](http://tools.ietf.org/html/rfc1341.html))
MIME扩展邮件的格式，用以支持非ASCII编码的文本、非文本附件以及包含多个部分 (multi-part) 的邮件体等。

**Python email模块**
**1. class email.message.Message**
__getitem__，__setitem__实现obj[key]形式的访问。
Msg.attach(playload): 向当前Msg添加playload。
Msg.set_playload(playload): 把整个Msg对象的邮件体设成playload。
Msg.add_header(_name, _value, **_params): 添加邮件头字段。
**2. class email.mime.base.MIMEBase(_maintype, _subtype, **_params)**
所有MIME类的基类，是email.message.Message类的子类。
**3. class email.mime.multipart.MIMEMultipart()**
在3.0版本的email模块 (Python 2.3-Python 2.5) 中，这个类位于email.MIMEMultipart.MIMEMultipart。
这个类是MIMEBase的直接子类，用来生成包含多个部分的邮件体的MIME对象。
**4. class email.mime.text.MIMEText(_text)**
使用字符串_text来生成MIME对象的主体文本。

smtplib模块简介
*smtplib.SMTP([host[, port[, local_hostname[, timeout]]]])*
此为SMTP类构造函数，表示与SMTP服务器之间的连接，并根据这个连接向smtp服务器发送指令，执行相关操作（如：登陆、发送邮件），且每个参数都是可选的。
其中最重要的参数：
host：smtp服务器主机名
port：smtp服务的端口，默认是25；
如果在创建SMTP对象的时候提供了这两个参数，在初始化的时候会自动调用connect方法去连接服务器。
smtplib.SMTP还提供了如下方法：
*SMTP.set_debuglevel(level)：*设置是否为调试模式
SMTP.connect([host[, port]])：连接到指定的smtp服务器。参数分别表示 smpt主机和端口。
SMTP.docmd(cmd[, argstring])：向smtp服务器发送指令。
SMTP.helo([hostname]) ：使用"helo"指令向服务器确认身份。
SMTP.login(user, password)：登陆到smtp服务器。现在几乎所有smtp服务器，都必须在验证用户信息合法之后才允许发送邮件。（重要！）
*SMTP.sendmail(from_addr,to_addrs,msg[,mail_options,rcpt_options])：*发送邮件。这里要注意一下第三个参数，msg是字符串，表示邮件。我们知道邮件一般由标题，发信人，收件人，邮件内容，附件等构成，发送邮件的时候，要注意msg的格式。这个格式就是smtp协议中定义的格式。SMTP.quit() ：断开与smtp服务器的连接，相当于发送"quit"指令。（重要！）

**email 模块**
1.**class email.message.Message**
*__getitem__，__setitem__*实现obj[key]形式的访问。
*Msg.attach(playload):*向当前Msg添加playload。
*Msg.set_playload(playload):*
*Msg.add_header(_name, _value, **_params):*添加邮件头字段。
2.**class email.mime.base.MIMEBase(_maintype, _subtype, **_params)**
所有MIME类的基类，是email.message.Message类的子类。
3.**class email.mime.multipart.MIMEMultipart()**
在3.0版本的email模块 (Python 2.3-Python 2.5) 中，这个类位于email.MIMEMultipart.MIMEMult    ipart。这个类是MIMEBase的直接子类，用来生成包含多个部分的邮件体的MIME对象。
4.**class email.mime.text.MIMEText(_text)**
使用字符串_text来生成MIME对象的主体文本。

获得所需要使用的邮箱的host地址和port端口号，常用邮箱的smtp服务器地址和端口号如图：
![](https://img-blog.csdn.net/20150420173258272)


**代码实现**
```python
#!/usr/bin/env python
# -*- coding:utf-8 -*-
# copyright of mimvp.com
 
import os
import smtplib 
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.mime.text import MIMEText
from email.utils import formatdate
from email import encoders
 
class Mail(object):
    
    MAIL_SERVER = {
                      'server_smtp' : 'smtp.mimvp.com',
                      'from' : 'yang@mimvp.com',
                      'to' : ['463153470@qq.com', 'yanggang@163.com', 'yanggang@mi.com'],
                      'user_name' : 'yang@mimvp.com',
                      'user_pass' : '123456'
                  }    
    
    @classmethod
    def send_mail(cls, subject='', body='', attaches=[]): 
        mail_server = Mail.MAIL_SERVER.get('server_smtp')
        mail_from = Mail.MAIL_SERVER.get('from') 
        mail_to = Mail.MAIL_SERVER.get('to')
        mail_to_str = ";".join(Mail.MAIL_SERVER.get('to'))
        user_name = Mail.MAIL_SERVER.get('user_name')
        user_pass = Mail.MAIL_SERVER.get('user_pass')
        mail_body = body
     
        msg = MIMEMultipart() 
        msg['Subject'] = subject 
        msg['From'] = mail_from
        msg['To'] = mail_to_str
        msg['Date'] = formatdate(localtime=True) 
        msg.attach(MIMEText(mail_body)) 
     
        for attach in attaches: 
            part = MIMEBase('application', 'octet-stream') 
            part.set_payload(open(attach, 'rb').read()) 
            encoders.encode_base64(part) 
            part.add_header('Content-Disposition', 'attachment; filename="%s"' % os.path.basename(attach)) 
            msg.attach(part) 
    
        try:
            smtp = smtplib.SMTP(mail_server) 
            smtp.login(user_name, user_pass) 
            smtp.sendmail(mail_from, mail_to, msg.as_string()) 
            smtp.close()
            print("send email success!")
        except Exception as ex:
            print("send email fail. error: " + str(ex))
        
if __name__ == "__main__":
    
    attaches = []
    for i in range(5):
        i += 1
        filename = "file_" + str(i)
        f = open(filename, 'w+')
        f.write("i am ithomer.net " + str(i))
        f.close()
        attaches.append(filename)
    print attaches
    #files.append("getUrl.py")
    #files.append("getUrl.py")
    
    Mail.send_mail('test python email', 'body of www.mimvp.com', attaches)
```


**运行结果：**
$python email_test.py
['file_0', 'file_1', 'file_2', 'file_3', 'file_4']
send email success!
**发送的邮件内容：**
![](https://img-blog.csdn.net/20131209231621296)

无法import的原因(ImportError: No module named *****)
python中，每个py文件被称之为模块，每个具有__init__.py文件的目录被称为包。只要模块或者包所在的目录在sys.path中，就可以使用import 模块或import 包来使用。
如果想使用非当前模块中的代码，需要使用Import，这个大家都知道。
如果你要使用的模块（py文件）和当前模块在同一目录，只要import相应的文件名就好，比如在a.py中使用b.py：
import b
但是如果要import一个不同目录的文件(例如b.py)该怎么做呢？
首先需要使用sys.path.append方法将b.py所在目录加入到搜素目录中。然后进行import即可，例如
import sys
sys.path.append('c:\xxxx\b.py')
大多数情况，上面的代码工作的很好。但是如果你没有发现上面代码有什么问题的话，可要注意了，上面的代码有时会找不到模块或者包（ImportError: No module named xxxxxx），这是因为：
sys模块是使用c语言编写的，因此字符串支持 '\n', '\r', '\t'等来表示特殊字符。所以上面代码最好写成：
sys.path.append('c:\\xxx\\b.py')
或
sys.path.append('c:/xxxx/b.py')
这样可以避免因为错误的组成转义字符，而造成无效的搜索目录（sys.path）设置。


**参考推荐：**
[python发送邮件](http://www.cnblogs.com/lonelycatcher/archive/2012/02/09/2343480.html)
使用python偷偷发邮件
[email: Examples](http://docs.python.org/2/library/email-examples.html#email-examples)（官方示例）
[利用Python实现邮件的发送](http://jingyan.baidu.com/article/b24f6c822784b886bfe5dabe.html)
[python 七种邮件内容发送方法实例](http://www.jb51.net/article/49216.htm)（推荐）



