# 使用Python实现邮件发送 - 小灰笔记 - CSDN博客





2017年04月20日 22:02:00[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：873








如果实现一个服务器的状态通知，可以选择通过邮件提醒的方式实现。Python自带的模块库中有支持smtp协议邮件发送的功能。

写了一个简单的小例子实现Python的邮件发送功能如下：

  1 #!/usr/bin/python  

  2 # -*- coding: cp936 -*-

  3 

  4 import os, smtplib, mimetypes

  5 from email.mime.text
import MIMEText

  6 from email.mime.image
import MIMEImage

  7 from email.mime.multipart
import MIMEMultipart

  8 

  9 mail_list = ["XXX@cqu.edu.cn"]

 10 mail_host =
"smtp.126.com"

 11 mail_user = "XXXX"

 12 mail_psswd = "XXXX"

 13 mail_psotfix = "126.com"

 14 mail_from = mail_user +
"<"+mail_user + "@" + mail_psotfix +
">"

 15 

 16 defsend_mail(subject, content, filename =
None):

 17 try:

 18         message = MIMEMultipart()

 19         message.attach(MIMEText(content))

 20         message["Subject"] = subject

 21         message["From"] = mail_from

 22         message["To"] =
";".join(mail_list)

 23 if filename != Noneand os.path.exists(filename):

 24             ctype, encoding = mimetypes.guess_type(filename)

 25 if ctype isNoneor encoding
isnotNone:

 26                 ctype =
"application/octet-stream"

 27             maintype, subtype = ctype.split("/",
1)

 28             attachment = MIMEImage((lambda f: (f.read(), f.close()))(open(filename,
"rb"))[0], _subtype = subtype)

 29             attachment.add_header("Content-Disposition",
"attachment", filename = filename)

 30             message.attach(attachment)

 31 

 32         smtp = smtplib.SMTP()

 33         smtp.connect(mail_host)

 34         smtp.login(mail_user, mail_psswd)

 35         smtp.sendmail(mail_from, mail_list, message.as_string())

 36         smtp.quit()

 37 

 38 returnTrue

 39 exceptException, errmsg:

 40 print"Send mail failed to: %s" % errmsg

 41 returnFalse

 42 

 43 if __name__ == "__main__":

 44 if send_mail("python email title",
"send from python", r"筷子兄弟 - 父亲.mp3"):

 45 print"send sucess"

 46 else:

 47 print"failed to send”







查看运行结果：

GreydeMac-mini:Chapter_02 greyzhang$ ls

c01.pydiff.zipmail.py

d1.pydns_parser.pypydiff.py

d2.pyemail_demo.pyreport.html

diff.pyjoin.py筷子兄弟 - 父亲.mp3

GreydeMac-mini:Chapter_02 greyzhang$ python email_demo.py 

send sucess




邮件发送成功，查看接收邮箱可以看到邮件已经收到。现在的唯一遗憾是附件的编码格式出了问题，成了乱码。

![](https://img-blog.csdn.net/20170420220058048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




