# python邮件发送 - nosmatch的专栏 - CSDN博客
2011年07月21日 15:34:07[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：8996
                 　　在基于互联网的应用中，程序经常需要自动地发送电子邮件。如：一个网站的注册系统会在用户注册时发送一封邮件来确认注册；当用户忘记登陆密码的时候，通过邮件来取回密码。smtplib模块是python中smtp(简单邮件传输协议)的客户端实现。我们可以使用smtplib模块，轻松的发送电子邮件。下面的例子用了不到十行代码来发送电子邮件： 
- #coding=gbk 
- 
- import smtplib   
- 
- smtp = smtplib.SMTP()   
- smtp.connect("smtp.yeah.net", "25")   
- smtp.login('用户名', '密码')   
- smtp.sendmail('from@yeah.net', 'to@21cn.com', 'From: from@yeah.net/r/nTo: to@21cn.com/r/nSubject: this is a email from python demo/r/n/r/nJust for test~_~')   
- smtp.quit()  
这个例子够简单吧^_^！下面详细介绍stmplib模块中的类和方法。
## smtplib.SMTP([host[, port[, local_hostname[, timeout]]]])
　　SMTP类构造函数，表示与SMTP服务器之间的连接，通过这个连接我们可以向smtp服务器发送指令，执行相关操作（如：登陆、发送邮件）。该类提供了许多方法，将在下面介绍。它的所有参数都是可选的，其中host参数表示smtp服务器主机名，上面例子中的smtp主机为"smtp.yeah.net"；port表示smtp服务的端口，默认是25；如果在创建SMTP对象的时候提供了这两个参数，在初始化的时候会自动调用connect方法去连接服务器。
　　smtplib模块还提供了SMTP_SSL类和LMTP类，对它们的操作与SMTP基本一致。
　　smtplib.SMTP提供的方法：
### SMTP.set_debuglevel(level)
　　设置是否为调试模式。默认为False，即非调试模式，表示不输出任何调试信息。
### SMTP.connect([host[, port]])
　　连接到指定的smtp服务器。参数分别表示smpt主机和端口。注意: 也可以在host参数中指定端口号（如：smpt.yeah.net:25），这样就没必要给出port参数。
### SMTP.docmd(cmd[, argstring])
　　向smtp服务器发送指令。可选参数argstring表示指令的参数。下面的例子完全通过调用docmd方法向服务器发送指令来实现邮件的发送（在smtp.yeah.net邮件服务器上试验通过。其他邮件服务器没有试过）：
- import smtplib, base64, time  
- userName = base64.encodestring('from').strip()  
- password = base64.encodestring('password').strip()  
- smtp = smtplib.SMTP()  
- smtp.connect("smtp.yeah.net:25")  
- print smtp.docmd('helo', 'from')  
- print smtp.docmd('auth login')  
- print smtp.docmd(userName)  
- print smtp.docmd(password)  
- print smtp.docmd('mail from:', '<from@yeah.net>')  
- print smtp.docmd('rcpt to:', '<from@yeah.net>')  
- #data 指令表示邮件内容 
- print smtp.docmd('data')  
- print smtp.docmd('''''from: from@yeah.net
- to: from@yeah.net
- subject: subject
- email body
- .
- ''')  
- smtp.quit()  
### SMTP.helo([hostname]) 
　　使用"helo"指令向服务器确认身份。相当于告诉smtp服务器“我是谁”。
### SMTP.has_extn(name) 
　　判断指定名称在服务器邮件列表中是否存在。出于安全考虑，smtp服务器往往屏蔽了该指令。
### SMTP.verify(address) 
　　判断指定邮件地址是否在服务器中存在。出于安全考虑，smtp服务器往往屏蔽了该指令。
### SMTP.login(user, password) 
　　登陆到smtp服务器。现在几乎所有的smtp服务器，都必须在验证用户信息合法之后才允许发送邮件。
### SMTP.sendmail(from_addr, to_addrs, msg[, mail_options, rcpt_options]) 
　　发送邮件。这里要注意一下第三个参数，msg是字符串，表示邮件。我们知道邮件一般由标题，发信人，收件人，邮件内容，附件等构成，发送邮件的时候，要注意msg的格式。这个格式就是smtp协议中定义的格式。在上面的例子中，msg的值为：
- '''''From: from@yeah.net
- To: to@21cn.com
- Subject: test
- 
- just for test'''
　　这个字符串的的意思表示邮件发件人为"from@yeah.net"，收件人为" to@21cn.com"，邮件标题为"test"，邮件内容为"just for test"。细心的你可能会疑问：如果要发送的邮件内容很复杂，包含图片、视频、附件等内容，按照MIME的格式来拼接字符串，将是一件非常麻烦的事。不用担心，python已经考虑到了这点，它为我们提供了email模块，使用该模块可以轻松的发送带图片、视频、附件等复杂内容的邮件。在介绍完smtplib模块之后，我会简单介绍email模块的基本使用。
### SMTP.quit() 
　　断开与smtp服务器的连接，相当于发送"quit"指令。
## email及其相关子模块
　　emial模块用来处理邮件消息，包括MIME和其他基于[**RFC 2822**](http://rfc.net/rfc2822.html)的消息文档。使用这些模块来定义邮件的内容，是非常简单的。下面是一些常用的类：
class email.mime.multipart.MIMEMultipart: 多个MIME对象的集合。
class email.mime.audio.MIMEAudio: MIME音频对象。
class email.mime.image.MIMEImage: MIME二进制文件对象。
class email.mime.text.MIMEText: MIME文本对象。
　     看上面的解释可能会觉得云里雾里，其实我对smtp, MIME的理解也很肤浅。但在大多数时候，我们只要会用就可以了。下面是一个简单的例子来演示如何使用这些类来发  送带附件的邮件： 
        #coding=gbk 
- import smtplib, mimetypes  
- from email.mime.text import MIMEText  
- from email.mime.multipart import MIMEMultipart  
- from email.mime.image import MIMEImage  
- 
- msg = MIMEMultipart()  
- msg['From'] = "from@yeah.net"
- msg['To'] = 'to@21cn.com'
- msg['Subject'] = 'email for tesing'
- 
- #添加邮件内容 
- txt = MIMEText("这是邮件内容~~")  
- msg.attach(txt)  
- 
- #添加二进制附件 
- fileName = r'e:/PyQt4.rar'
- ctype, encoding = mimetypes.guess_type(fileName)  
- if ctype isNoneor encoding isnotNone:  
-     ctype = 'application/octet-stream'
- maintype, subtype = ctype.split('/', 1)  
- att1 = MIMEImage((lambda f: (f.read(), f.close()))(open(fileName, 'rb'))[0], _subtype = subtype)  
- att1.add_header('Content-Disposition', 'attachment', filename = fileName)  
- msg.attach(att1)  
- 
- #发送邮件 
- smtp = smtplib.SMTP()  
- smtp.connect('smtp.yeah.net:25')  
- smtp.login('from', '密码')  
- smtp.sendmail('from@yeah.net', 'to@21cn.com', msg.as_string())  
- smtp.quit()  
- print'邮件发送成功'
　　是不是很简单。简单就是美，用最少的代码把问题解决，这就是Python。更多关于smtplib的信息，请参考Python手册 [smtplib](http://docs.python.org/library/smtplib.html#module-smtplib)模块。
发送不带附件的简单的邮件的代码：
  1 import smtplib
  2 from email.mime.text import MIMEText
  3 from email.MIMEMultipart import MIMEMultipart
  4 
  5 def sendsimplemail (warning):
  6         msg = MIMEText(warning)
  7         msg['Subject'] = 'python first mail'
  8         msg['From'] = ['whz1016@163.com'](mailto:' rel=)
  9         try:
 10                 smtp = smtplib.SMTP()
 11                 smtp.connect(r'smtp.163.com')
 12                 smtp.login(['whz1016@163.com'](mailto:' rel=), '密码')
 13                 smtp.sendmail(['whz1016@163.com'](mailto:' rel=), ['whz1016@163.com'], msg.as_string())
 14                 smtp.close()
 15         except Exception, e:
 16                         print e
 17 
 18 if __name__ == '__main__':
 19         sendsimplemail(warning = "This is a warning!!!")   
