
# linux shell 发送email 邮件 - Augusdi的专栏 - CSDN博客


2015年04月22日 15:47:17[Augusdi](https://me.csdn.net/Augusdi)阅读数：4939


**方法一：简单邮件发送**
echo 'hello world' | mail -s "Subject" -t  yanggang@ithomer.com,yanggang_2050@163.com    -a From:463103470@qq.com
效果截图
![](http://hi.csdn.net/attachment/201109/15/0_1316072917eb0k.gif)

**方法二： 文本格式发送邮件**
**[python]**[view plain](http://blog.csdn.net/ithomer/article/details/6776067#)[copy](http://blog.csdn.net/ithomer/article/details/6776067#)[print](http://blog.csdn.net/ithomer/article/details/6776067#)[?](http://blog.csdn.net/ithomer/article/details/6776067#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

\# !/bin/sh
from_name="from@yanggang"
from="yanggang@fruitsmobile.com"
to="yanggang_2050@163.com"
email_title="Test Mail"
email_content="/home/barry/top800/test/output_email.html"
email_subject="Top800_Games_Free_USA"
echo -e"To: \"${email_title}\" <${to}>\nFrom: \"${from_name}\" <${from}>\nSubject: ${email_subject}\n\n`cat ${email_content}`"| /usr/sbin/sendmail -t
效果截图：
![](http://hi.csdn.net/attachment/201109/15/0_1316072815lSj6.gif)

**方法三：html格式发送邮件**
**[python]**[view plain](http://blog.csdn.net/ithomer/article/details/6776067#)[copy](http://blog.csdn.net/ithomer/article/details/6776067#)[print](http://blog.csdn.net/ithomer/article/details/6776067#)[?](http://blog.csdn.net/ithomer/article/details/6776067#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

\# !/bin/sh

from='yanggang@fruitsmobile.com'
to='yanggang_2050@163.com'
email_date=''
email_content='/home/barry/top800/test/output_email.html'
email_subject='Top800_Game_Free_USA'

function send_email(){
email_date=$(date"+%Y-%m-%d_%H:%M:%S")
echo $email_date
email_subject=$email_subject"__"$email_date
echo $email_subject
cat $email_content | formail -I"From: $from"-I"MIME-Version:1.0"-I"Content-type:text/html;charset=gb2312"-I"Subject: $email_subject"| /usr/sbin/sendmail -oi $to
}
send_email
效果截图：
![](http://hi.csdn.net/attachment/201109/15/0_13160723790253.gif)

**[源码下载](http://download.csdn.net/detail/sunboy_2050/3604325)****（linux shell 发送email 邮件）**

**--------------------------------**

CentOS是一个非常不错的免费开源Linux系统，许多站点首选的平台。
然而CentOS默认不能发送邮件，需要发送邮件的童鞋可以安装一个sendmail程序。
**安装sendmail**
执行： [root@vps478753 ~]\#yum -y install sendmail
程序会自动搜索出sendmail安装程序自动安装，安装好sendmail以后执行以下命令启动sendmail
[root@vps478753 ~]\#/etc/init.d/sendmail start
Starting sendmail:                                         [  OK  ]
Starting sm-client:                                        [  OK  ]
启动以后我们可以执行mail命令测试一下是否能发送邮件
一、通过文件内容发送邮件
[root@vps478753 ~]\#mail -s 'Test mail' support@ithomer.net < /etc/passwd
很快收到邮件了，正文是 /etc/passwd 文件的内容

二、使用管道符直接发送邮件内容
如果不想通过文件发送邮件内容也可以这么发送
[root@vps478753 ~]\#echo "This is test mail" | mail -s 'Test mail' support@ithomer.net
以上效果同文件发送邮件内容一样
如果提示mail: command not found
[root@vps478753 ~]\#mail -s 'Test mail'support@ithomer.net < /etc/passwd
-bash: mail: command not found
那么就是没有安装mail命令，此时需要安装mail命令
[root@vps478753 ~]\#yum install mailx -y
然后再重新发送以下邮件就好了！


**参考推荐：**
[4 Ways to Send Email From Linux Command Line](http://tecadmin.net/ways-to-send-email-from-linux-command-line/#)
[shell 发邮件命令之 mail](http://blog.csdn.net/kevinew/article/details/9147951)

[http://blog.csdn.net/ithomer/article/details/6776067](http://blog.csdn.net/ithomer/article/details/6776067)



