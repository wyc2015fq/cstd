# Python 使用smtp发送邮件失败 在Windows Server R2012上发送失败的问题 - DumpDoctorWang的博客 - CSDN博客





2018年05月13日 16:40:08[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：388








我使用Python编写了一段使用stmp发送邮件的例子，在本机上发送能成功，但是在我的Windows Server R2012服务器上一直发送失败，一直卡在连接SMTP服务器那一步，经过一番折腾，终于可以了。接下来给出发送失败和发送成功的代码。

一、失败代码

```python
# -*- coding: UTF-8 -*-

import smtplib
import email.mime.multipart
import email.mime.text

# 邮箱配置
my_email_config = {'host': 'smtp.163.com',              # 设置服务器，可换成其他服务器
                   'sender': 'you_email@163.com',       # 发件人
                   'password': 'auth_path',             # 163邮箱的客户端授权密码，不是邮箱密码，怎么设置可自行百度
                   'receiver': 'receiver@qq.com',       # 收件人
                   }

def send_email(mail_subject, mail_content):
    """
    发送邮件
    :param mail_subject 邮件主题
    :param mail_content: 邮件内容
    :return: 空
    """
    # 第三方 SMTP 服务，推荐163
    host = my_email_config['host']  # 设置服务器
    sender = my_email_config['sender']  # 用户名
    password = my_email_config['password']  # 口令
    receiver = my_email_config['receiver']  # 接收邮件，可设置为你的QQ邮箱或者其他邮箱

    msg = email.mime.multipart.MIMEMultipart()

    '''
    邮件主题为‘test’的时候就会出现错误，换成其他词就好了。。我也不知道这是什么奇葩的原因 
    '''
    msg['Subject'] = mail_subject
    msg['From'] = sender
    msg['To'] = receiver

    txt = email.mime.text.MIMEText(mail_content)
    msg.attach(txt)

    smtp_server = smtplib.SMTP_SSL()
    smtp_server.connect(host, 25)
    # 登录
    smtp_server.login(sender, password)
    # 发送邮件
    smtp_server.sendmail(sender, receiver, msg.as_string())
    # 退出
    smtp_server.quit()
    print('邮件发送成功，主题：%s' % mail_subject)


if __name__ == "__main__":
    subject = "想你了"
    content = '''
           你好，小明
       '''
    send_email(subject, content)
```

二、成功代码

```python
# -*- coding: UTF-8 -*-

import smtplib
import email.mime.multipart
import email.mime.text

# 这里需要导入自己的信息
try:
    # from config import my_email_config
    from config_backup import my_email_config
except ImportError:
    # from .config import my_email_config
    from .config_backup import my_email_config
# 邮箱配置
my_email_config = {'host': 'smtp.163.com',              # 设置服务器，可换成其他服务器
                   'sender': 'you_email@163.com',       # 发件人
                   'password': 'auth_path',             # 163邮箱的客户端授权密码，不是邮箱密码，怎么设置可自行百度
                   'receiver': 'receiver@qq.com',       # 收件人
                   }

def send_email(mail_subject, mail_content):
    """
    发送邮件
    :param mail_subject 邮件主题
    :param mail_content: 邮件内容
    :return: 空
    """
    # 第三方 SMTP 服务，推荐163
    host = my_email_config['host']  # 设置服务器
    sender = my_email_config['sender']  # 用户名
    password = my_email_config['password']  # 口令
    receiver = my_email_config['receiver']  # 接收邮件，可设置为你的QQ邮箱或者其他邮箱

    msg = email.mime.multipart.MIMEMultipart()

    '''
    邮件主题为‘test’的时候就会出现错误，换成其他词就好了。。我也不知道这是什么奇葩的原因 
    '''
    msg['Subject'] = mail_subject
    msg['From'] = sender
    msg['To'] = receiver

    txt = email.mime.text.MIMEText(mail_content)
    msg.attach(txt)

    # 这里要使用SSL，这样就能在Windows Server R2012上发送邮件了
    smtp_server = smtplib.SMTP_SSL()
    smtp_server.connect(host, 465)
    # 登录
    smtp_server.login(sender, password)
    # 发送邮件
    smtp_server.sendmail(sender, receiver, msg.as_string())
    # 退出
    smtp_server.quit()
    print('邮件发送成功，主题：%s' % mail_subject)


if __name__ == "__main__":
    subject = "想你了"
    content = '''
           你好，小明
       '''
    send_email(subject, content)
```







