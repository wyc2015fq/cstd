# python学习笔记(三十四) -- 邮件接收 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月13日 01:41:25[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：38
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



# 邮件接收 

## 思路

构造一个邮件对象就是一个`Messag`对象，如果构造一个`MIMEText`对象，就表示一个文本邮件对象，如果构造一个`MIMEImage`对象，就表示一个作为附件的图片，要把多个对象组合起来，就用`MIMEMultipart`对象，而`MIMEBase`可以表示任何对象。它们的继承关系如下：

```
Message
+- MIMEBase
   +- MIMEMultipart
   +- MIMENonMultipart
      +- MIMEMessage
      +- MIMEText
      +- MIMEImage
```

收取邮件就是编写一个**MUA**作为客户端，从**MDA**把邮件获取到用户的电脑或者手机上。收取邮件最常用的协议是**POP**协议，目前版本号是3，俗称**POP3**。

Python内置一个`poplib`模块，实现了POP3协议，可以直接用来收邮件。

注意到POP3协议收取的不是一个已经可以阅读的邮件本身，而是邮件的原始文本，这和SMTP协议很像，SMTP发送的也是经过编码后的一大段文本。

要把POP3收取的文本变成可以阅读的邮件，还需要用`email`模块提供的各种类来解析原始文本，变成可阅读的邮件对象。

所以，收取邮件分两步：

第一步：用`poplib`把邮件的原始文本下载到本地；

第二部：用`email`解析原始文本，还原为邮件对象。

## 简单案例

```python
import poplib
from email.parser import Parser

# 输入邮件地址，口令和POP3服务器地址
email = input('Email:')
password = input('Password:')
pop3_server = input('POP3 server')

# 连接到POP3服务器：
server = poplib.POP3_SSL(pop3_server, 995)
# 开启调试信息
server.set_debuglevel(1)
# 打印
print(server.getwelcome().decode('utf-8'))

# 身份认证
server.user(email)
server.pass_(password)

# stat()返回邮件数量和占用空间
print('Messages: %s . Size: %s' % server.stat())  # stat函数返回一个tuple
# list()返回所有邮件的编号
resp, mails, octets = server.list()
print(resp, mails, octets)  # resp响应状态，mails所有邮件编号列表，octetes 八位组 可以理解为字节的另外一个叫法

# 获取最新的一封邮件，注意索引号从1开始
index = len(mails)
print('邮件个数: %d' % index)
# 根据索引位获取最后一封邮件的原始文本数据
resp, lines, octets = server.retr(index)  # lines 代表返回的邮件对象中的每一行文本数据，数据类型为byte
print(resp, lines, octets)
# lines存储了邮件的原始文件的每一行
# 可以获得整个邮件的原始版本
msg_content = b'\r\n'.join(lines).decode('utf-8')  # 列表中每个元素中加上换行符

# 解析邮件
msg = Parser().parsestr(msg_content)  # 通过字符串 创建消息结构

# 关闭连接
server.quit()
```

## 复杂案例

```python
# -*- coding: utf-8 -*-

from email.parser import Parser
from email.header import decode_header
from email.utils import parseaddr

import poplib

# 输入邮件地址, 口令和POP3服务器地址:
email = input('Email: ')
password = input('Password: ')
pop3_server = input('POP3 server: ')

def guess_charset(msg):  # 获取邮件编码
    charset = msg.get_charset()
    if charset is None:
        content_type = msg.get('Content-Type', '').lower()
        pos = content_type.find('charset=')
        if pos >= 0:
            charset = content_type[pos + 8:].strip()
    return charset

def decode_str(s):
    value, charset = decode_header(s)[0]
    if charset:
        value = value.decode(charset)
    return value

def print_info(msg, indent=0):
    if indent == 0:
        # for循环用来打印邮件头信息
        for header in ['From', 'To', 'Subject']:  # 邮件头信息包括主题、收件人、发件人
            value = msg.get(header, '')  # 获取 某个头信息的值
            if value:   # 如果该头信息值不为空
                if header == 'Subject':  # 如果是 主题
                    value = decode_str(value)   # 将主题内容解码,对应着发邮件时的Head().encode()
                else:   # 如果是收件人或发件人
                    hdr, addr = parseaddr(value)  # 先将收件人和发件人的名称与地址拆开，对应formataddr()函数
                    name = decode_str(hdr)  # 再将收件人发件人名称部分解码，对应着发邮件时的Head().encode()
                    value = u'%s <%s>' % (name, addr)  # 再进行组装
            print('%s%s: %s' % ('  ' * indent, header, value))
    # 如果该邮件同时支持HTML和Plain格式，则说明该邮件可能由多钟结构组成
    if (msg.is_multipart()):
        parts = msg.get_payload()  # get_payload这这里会返回一个list，代表multipart包含的邮件对象列表
        for n, part in enumerate(parts):  # 通过enumerate创建枚举对象，n代表索引
            print('%spart %s' % ('  ' * indent, n))
            print('%s--------------------' % ('  ' * indent))
            print_info(part, indent + 1)  # 递归调用
    else:
        content_type = msg.get_content_type()  # 获取发送邮件支持的类型、文本或html
        if content_type == 'text/plain' or content_type=='text/html':
            content = msg.get_payload(decode=True)  # 提取文本内容并解码，这里的解码是对邮件信息解码，不是平时常用的byte解码
            charset = guess_charset(msg)  # 获得邮件编码
            if charset:
                content = content.decode(charset)  # 根据获得到的编码对byte进行解码
            print('%sText: %s' % ('  ' * indent, content + '...'))
            print('%sAttachment: %s' % ('  ' * indent, content_type))

# 连接到POP3服务器:
# server = poplib.POP3(pop3_server)
server = poplib.POP3_SSL(pop3_server, 995)
# 可以打开或关闭调试信息:
server.set_debuglevel(1)
# 可选:打印POP3服务器的欢迎文字:
print(server.getwelcome().decode('utf-8'))
# 身份认证:
server.user(email)
server.pass_(password)
# stat()返回邮件数量和占用空间:
print('Messages: %s. Size: %s' % server.stat())
# list()返回所有邮件的编号:
resp, mails, octets = server.list()
# 可以查看返回的列表类似[b'1 82923', b'2 2184', ...]
print(mails)
# 获取最新一封邮件, 注意索引号从1开始:
index = len(mails)
resp, lines, octets = server.retr(index)
# lines存储了邮件的原始文本的每一行,
# 可以获得整个邮件的原始文本:
msg_content = b'\r\n'.join(lines).decode('utf-8')
# 解析邮件,对应encode_base64
msg = Parser().parsestr(msg_content)
print_info(msg)
# 可以根据邮件索引号直接从服务器删除邮件:
# server.dele(index)
# 关闭连接:
server.quit()
```

