# python学习笔记(三十三) -- 邮件发送 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月11日 23:13:21[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：257
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[前言](#%E5%89%8D%E8%A8%80)

[SMPT发送邮件](#SMPT%E5%8F%91%E9%80%81%E9%82%AE%E4%BB%B6)

[简单案例](#%E7%AE%80%E5%8D%95%E6%A1%88%E4%BE%8B)

[设置主题、收件人、发件人名称](#%E8%AE%BE%E7%BD%AE%E4%B8%BB%E9%A2%98%E3%80%81%E6%94%B6%E4%BB%B6%E4%BA%BA%E3%80%81%E5%8F%91%E4%BB%B6%E4%BA%BA%E5%90%8D%E7%A7%B0)

[发送HTML邮件](#%C2%A0%E5%8F%91%E9%80%81HTML%E9%82%AE%E4%BB%B6)

[发送附件 以及图片](#%E5%8F%91%E9%80%81%E9%99%84%E4%BB%B6%C2%A0%E4%BB%A5%E5%8F%8A%E5%9B%BE%E7%89%87)

[同时支持HTML和Plain格式](#%C2%A0%E5%90%8C%E6%97%B6%E6%94%AF%E6%8C%81HTML%E5%92%8CPlain%E6%A0%BC%E5%BC%8F)

[加密SMTP](#%E5%8A%A0%E5%AF%86SMTP)

# 前言

```
邮件发送到接收流程：
    发件人 -> MUA -> MTA -> MTA -> 若干个MTA -> MDA <- MUA <- 收件人
    MUA：Mail User Agent——邮件用户代理  如outlook
    MTA：Mail Transfer Agent——邮件传输代理  outlook等软件会发给MTA
    MDA：Mail Delivery Agent——邮件投递代理  邮件内容保存在MDA
    当收件上登陆邮箱时会从 MDA 获取邮件内容
    
发邮件时，MUA和MTA使用的协议就是SMTP：
    Simple Mail Transfer Protocol，后面的MTA到另一个MTA也是用SMTP协议。
    
收邮件时，MUA和MDA使用的协议有两种：
    POP：Post Office Protocol，目前版本是3，俗称POP3；
    IMAP：Internet Message Access Protocol，目前版本是4。
```

# SMPT发送邮件

## 简单案例

使用如下

```python
from email.mime.text import MIMEText   # 负责构建邮件

# 参数一：邮件正文
# 参数二：邮件内容类型，plain代表纯文本
msg = MIMEText('Hello,send by Python..', 'plain', 'utf-8')
# 输入Email地址和口令
from_addr = input('From:')
password = input('Password:')
# 输入收件人地址：
to_addr = input('To:')
# 输入SMIP服务器地址：
smtp_server = input('SMTP server:')  # 形如 smtp.qq.com 这是qq邮箱的smtp服务器地址

import smtplib  # 负责发送邮件
#  连接SMTP服务器
server = smtplib.SMTP(smtp_server, 25)  # SMTP 协议默认端口是25
# server = smtplib.SMTP_SSL(smtp_server, 465)  使用SSL协议的话 端口号就是465,SSL能够对传输内容以及用户名密码进行加密

#  使其可以打印出和SMTP服务器交互的所有信息,默认参数为false
server.set_debuglevel(1)
#  输入用户名密码登陆SMTP
server.login(from_addr, password)
#  发送邮件
#  参数一 发件人
#　参数二 收件人 可以有多个，是个list
#  参数三 邮件内容
server.sendmail(from_addr, [to_addr], msg.as_string())  # as_string将MIMEText转换成str
server.quit()
```

我是用qq邮箱向163邮箱发送的邮件，其中qq邮箱的密码不是我们登陆的密码，而是邮箱设置中的授权码。

![](https://img-blog.csdn.net/20180911161337966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

收到的邮件如下：

注意如果使用了SSL，会出现在垃圾邮件中...并且无主题

![](https://img-blog.csdn.net/20180911161527444?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180911161625854?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 没使用SSL 就可以在收件箱中看到

![](https://img-blog.csdn.net/20180911162819970?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过上面的图可以看出，邮件的 主题 均为 无主题，而发件人是我的qq号(我给抹了)。如果想更改主题和收件人以及发件人的内容，我们需要更改MIMEText对象的一些内容，如下：

## 设置主题、收件人、发件人名称

```python
from email.header import Header
from email.mime.text import MIMEText
from email.utils import parseaddr, formataddr
import smtplib

def _format_addr(s):
    name, addr = parseaddr(s)  # 将字符串拆分开为一个元组包含两个元素
    # 前一部分收件人和发件人的姓名，后一部分是地址

    # Header是MIME 头信息 对象，类似于Request Header，encode函数将Header对象转换成RFC格式
    # formataddr 和 parseaddr 作用相反
    return formataddr((Header(name, 'utf-8').encode(), addr))

from_addr = input('From: ')
password = input('Password: ')
to_addr = input('To: ')
smtp_server = input('SMTP server: ')

msg = MIMEText('hello, send by Python...', 'plain', 'utf-8')
msg['From'] = _format_addr('自己 <%s>' % from_addr)
msg['To'] = _format_addr('张子豪 <%s>' % to_addr)
msg['Subject'] = Header('SMTP测试……', 'utf-8').encode()

server = smtplib.SMTP(smtp_server, 25)
server.set_debuglevel(1)
server.login(from_addr, password)  # 与SMTP服务器连接
server.sendmail(from_addr, [to_addr], msg.as_string())  # 发送信息
server.quit()
```

效果如下 

![](https://img-blog.csdn.net/20180911213428770?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面所提到的RFC格式就类似下面这样：

```
From: =?utf-8?b?UHl0aG9u54ix5aW96ICF?= <xxxxxx@qq.com>
To: =?utf-8?b?566h55CG5ZGY?= <xxxxxx@163.com>
Subject: =?utf-8?b?5p2l6IeqU01UUOeahOmXruWAmeKApuKApg==?=
```

##  发送HTML邮件

就是邮件的内容是HTML形式的

使用方式如下：

```python
msg = MIMEText('<html><body><h1>Hello</h1>' +      # 发送的内容是html元素
    '<p>send by <a href="http://www.python.org">张子豪</a>...</p>' +
    '</body></html>', 'html', 'utf-8')  # 将第二个参数改为html
```

![](https://img-blog.csdn.net/20180911214534799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 发送附件 以及图片

如果Email中要加上附件怎么办？带附件的邮件可以看做包含若干部分的邮件：文本和各个附件本身，所以，可以构造一个`MIMEMultipart`对象代表邮件本身，然后往里面加上一个`MIMEText`作为邮件正文，再继续往里面加上表示附件的`MIMEBase`对象即可，如果想将附件的图片放入正文当中，则正文应是html形式的，然后在src标签中通过引用附件的id来获取到图片，注意以这种形式附件不会在收件人下方显示出来，而是当你鼠标放在正文中的图片上出现下载按钮。

如下：

```python
from email import encoders
from email.header import Header
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart, MIMEBase
from email.utils import parseaddr, formataddr
import smtplib
import requests

def _format_addr(s):
    name, addr = parseaddr(s)  # 将字符串拆分开为一个元组包含两个元素
    # 前一部分收件人和发件人的姓名，后一部分是地址

    # Header是MIME 头信息 对象，类似于Request Header，encode函数将Header对象转换成RFC格式
    # formataddr 和 parseaddr 作用相反
    return formataddr((Header(name, 'utf-8').encode(), addr))

# 获得本人博客页面源码
r = requests.get('https://blog.csdn.net/weixin_40247263/article/list/1')

from_addr = input('From: ')
password = input('Password: ')
to_addr = input('To: ')
smtp_server = input('SMTP server: ')

# 邮件对象:
msg = MIMEMultipart()
msg['From'] = _format_addr('自己 <%s>' % from_addr)
msg['To'] = _format_addr('张子豪 <%s>' % to_addr)
msg['Subject'] = Header('SMTP测试……', 'utf-8').encode()

# 邮件正文是MIMEText:
msg.attach(MIMEText('<h1>Hello</h1>' +
                    '<p><img src="cid:0"></p>' +   # 引用附件的图片，将其放入正文当中
                    r.text, 'html', 'utf-8'))

# 添加附件就是加上一个MIMEBase，从本地读取一个图片:
with open(r'C:\Users\Administrator\Desktop\img\1.jpg', 'rb') as f:
    # 设置附件的MIME和文件名，这里是jpg类型:
    mime = MIMEBase('image', 'jpg', filename='test.jpg') # filename代表下载以后的文件名
    # 加上必要的头信息:
    mime.add_header('Content-Disposition', 'attachment', filename='test.jpg')
    mime.add_header('Content-ID', '<0>')   # 给附件起个id值，说白了就是唯一标识
    mime.add_header('X-Attachment-Id', '0')  # 这行跟网络连接有关，如果不加会报socket.gaierror
    # 把附件的内容读进来:
    mime.set_payload(f.read())
    # 用Base64编码:
    encoders.encode_base64(mime)
    # 添加到MIMEMultipart:
    msg.attach(mime)

server = smtplib.SMTP(smtp_server, 25)
server.set_debuglevel(1)
server.login(from_addr, password)  # 与SMTP服务器连接
server.sendmail(from_addr, [to_addr], msg.as_string())  # 发送信息
server.quit()
```

效果如下：

![](https://img-blog.csdn.net/20180911224945588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 如果我们没有将附件的图片放入正文当中，效果如下：

![](https://img-blog.csdn.net/201809112250581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##  同时支持HTML和Plain格式

如果我们发送HTML邮件，收件人通过浏览器或者Outlook之类的软件是可以正常浏览邮件内容的，但是，如果收件人使用的设备太古老，查看不了HTML邮件怎么办？

办法是在发送HTML的同时再附加一个纯文本，如果收件人无法查看HTML格式的邮件，就可以自动降级查看纯文本邮件。

利用`MIMEMultipart`就可以组合一个HTML和Plain，要注意指定subtype是`alternative`：

```python
from email import encoders
from email.header import Header
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart, MIMEBase
from email.utils import parseaddr, formataddr
import smtplib
import requests

def _format_addr(s):
    name, addr = parseaddr(s)  # 将字符串拆分开为一个元组包含两个元素
    # 前一部分收件人和发件人的姓名，后一部分是地址

    # Header是MIME 头信息 对象，类似于Request Header，encode函数将Header对象转换成RFC格式
    # formataddr 和 parseaddr 作用相反
    return formataddr((Header(name, 'utf-8').encode(), addr))

# 获得本人博客页面源码
r = requests.get('https://blog.csdn.net/weixin_40247263/article/list/1')

from_addr = input('From: ')
password = input('Password: ')
to_addr = input('To: ')
smtp_server = input('SMTP server: ')

# 邮件对象:
msg = MIMEMultipart('alternative')
msg['From'] = _format_addr('自己 <%s>' % from_addr)
msg['To'] = _format_addr('张子豪 <%s>' % to_addr)
msg['Subject'] = Header('SMTP测试……', 'utf-8').encode()

msg.attach(MIMEText('看到这行说明你该换电脑了！！！', 'plain', 'utf-8'))
# 邮件正文是MIMEText:
msg.attach(MIMEText('<h1>Hello</h1>' +
                    '<p><img src="cid:0"></p>' +   # 引用附件的图片，将其放入正文当中
                    r.text, 'html', 'utf-8'))

# 添加附件就是加上一个MIMEBase，从本地读取一个图片:
with open(r'C:\Users\Administrator\Desktop\img\1.jpg', 'rb') as f:
    # 设置附件的MIME和文件名，这里是jpg类型:
    mime = MIMEBase('image', 'jpg', filename='test.jpg')
    # 加上必要的头信息:
    mime.add_header('Content-Disposition', 'attachment', filename='test.jpg')
    mime.add_header('Content-ID', '<0>')   # 给附件起个id值，说白了就是唯一标识
    mime.add_header('X-Attachment-Id', '0')  # 这行跟网络连接有关，如果不加会报socket.gaierror
    # 把附件的内容读进来:
    mime.set_payload(f.read())
    # 用Base64编码:
    encoders.encode_base64(mime)
    # 添加到MIMEMultipart:
    msg.attach(mime)

server = smtplib.SMTP(smtp_server, 25)
server.set_debuglevel(1)
server.login(from_addr, password)  # 与SMTP服务器连接
server.sendmail(from_addr, [to_addr], msg.as_string())  # 发送信息
server.quit()
```

效果和之前的一样

如果你没有指定`MIMEMultipart的`subtype，即没有传参，那么最终的效果是下面这样。

![](https://img-blog.csdn.net/20180911230747507?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 加密SMTP

使用标准的25端口连接SMTP服务器时，使用的是明文传输，发送邮件的整个过程可能会被窃听。要更安全地发送邮件，可以加密SMTP会话，实际上就是先创建SSL安全连接，然后再使用SMTP协议发送邮件。

某些邮件服务商，例如Gmail，提供的SMTP服务必须要加密传输。我们来看看如何通过Gmail提供的安全SMTP发送邮件。

必须知道，Gmail的SMTP端口是587，因此，修改代码如下：

```
smtp_server = 'smtp.gmail.com'
smtp_port = 587
server = smtplib.SMTP(smtp_server, smtp_port)
server.starttls()
# 剩下的代码和前面的一模一样:
server.set_debuglevel(1)
...
```

只需要在创建`SMTP`对象后，立刻调用`starttls()`方法，就创建了安全连接。后面的代码和前面的发送邮件代码完全一样。

