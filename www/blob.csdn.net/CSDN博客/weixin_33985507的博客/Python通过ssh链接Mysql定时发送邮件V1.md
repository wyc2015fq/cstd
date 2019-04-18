# Python通过ssh链接Mysql定时发送邮件V1 - weixin_33985507的博客 - CSDN博客
2017年05月04日 21:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
![2315442-846329455d33953f.png](https://upload-images.jianshu.io/upload_images/2315442-846329455d33953f.png)
吾生也有涯，而知也无涯，以有涯随无涯，殆已
来公司几个月了，和同事接触下来发现了一些比较现实的问题，其他部门有很多临时性的需求，要查数据，而分析师就两个人，而且还要做一些其他的数据，那么是不是可以通过邮件自动化来解决一些常规性的数据需求，这也是其他一些公司对此类需求的一贯做法。在和同事沟通之后，我就决定来实现一下，之前的公司是有人已经把那一套东西搭建好了，自己只是拿来用，改一下代码就好了，这次要我从零开始搞这个了。接下来就说一下这个过程。
我是在自己电脑装了虚拟机，然后装了ubuntu的操作系统，自己搭了一套开发环境，python安装的是2.7的，安装了mysql数据库。在搭建环境的过程中，需要安装一些工具包，看情况安装吧，不行就百度。
本文的结构如下：
1、ssh通道建立
2、读取数据库数据、保存Excel格式数据
3、读取数据，形成Html格式邮件正文，设置邮件相关参数
4、设置定时发送邮件
在第一步建立ssh通道的时候，需要安装ssh包，这个过程需要安装一些辅助的包，在这个过程看安装的提示就可以。我之前在网上搜了一些脚本，但是没有成功，本文的脚本是测试通过的，这一句我感觉是容易弄错的 port=server.local_bind_port，之前是填了具体的端口。
```
def dbconnect_ssh(ssh_host,ssh_port,ssh_password,ssh_user,db_host
,db_name,sql,db_port,db_user,db_passwd):
    with SSHTunnelForwarder(
            (ssh_host, ssh_port),
            ssh_password = ssh_password,
            #ssh_pkey=keyfile,
            ssh_username = ssh_user,
            remote_bind_address = (db_host, db_port)
    ) as server:
        db = MySQLdb.connect(
            host='127.0.0.1',
            port=server.local_bind_port,
            user=db_user,
            passwd=db_passwd,
            db=db_name,
            charset="utf8",
            cursorclass=MySQLdb.cursors.DictCursor)
```
在第二步中，从数据库读取数据是比较简单的，在网上搜出来很多，我这边是做了一个小处理，就是通过读取一个sql.txt文件，在里面放我的代码，这样就比较有通用性了。
```
#读取sql文件
sql = ''
    f = open(sql_file,'r')
    lines = f.readlines()
    for line in lines:
        line = line.strip('\n')
        sql += line + ' '
#读取数据库数据，在这里我使用了xlwt包来把数据保存在excel里面
cursor = db.cursor()
        cursor.execute(sql)
        cursor.scroll(0,mode='absolute')
        results = cursor.fetchall()
        fields = cursor.description
        workbook = xlwt.Workbook(encoding='utf-8')
        sheet = workbook.add_sheet('sheet1',cell_overwrite_ok=True)
        for field in range(0,len(fields)):
            sheet.write(0,field,fields[field][0])
        row = 1
        col = 0
        for row in range(1,len(results)+1):
            for col in range(0,len(fields)):
                tmp_field = fields[col][0]
                tmp_dir = results[row-1]
                    #sheet.write(row,col,tmp)
                sheet.write(row,col,tmp_dir.get(tmp_field,0))
        workbook.save("/home/data/datacenter/r_key_index/r_key_index.xls")
        db.commit()
```
第三步，需要设置邮件相关的参数，主要是发件人、收件人、抄送人、主题、正文等，其他的一些不是很重要，在邮件的正文是通过Html的语言进行组合的，这里面用了几个函数来形成的，自己测试一下就可以了。
最后一步，在测试没有问题之后，就可以部署自动邮件了，由于我使用的ubuntu的操作系统，cron表达式略有不同，它没有秒的概念，这也浪费了我一些时间，需要注意。
最后呈上完整代码，作为对借鉴过的代码的一个回馈吧。这只是第一个比较粗糙的版本，后面还会继续优化，欢迎高手指点。
```
#!/usr/bin/python
# -*- coding: utf-8 -*-
# code by Alex
import MySQLdb
from sshtunnel import SSHTunnelForwarder
import MySQLdb.cursors
import os
import sys
import time
from smtplib import SMTP
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.application import MIMEApplication
import datetime
import os.path
import mimetypes
from email.MIMEBase import MIMEBase
from email import Encoders
import pickle
import xlrd,xlwt
reload(sys)
sys.setdefaultencoding('utf8')
def dbconnect_ssh(ssh_host,ssh_port,ssh_password,ssh_user,db_host,db_name,sql,db_port,db_user,db_passwd):
    with SSHTunnelForwarder(
            (ssh_host, ssh_port),
            ssh_password = ssh_password,
            #ssh_pkey=keyfile,
            ssh_username = ssh_user,
            remote_bind_address = (db_host, db_port)
    ) as server:
        db = MySQLdb.connect(
            host='127.0.0.1',
            port=server.local_bind_port,
            user=db_user,
            passwd=db_passwd,
            db=db_name,
            charset="utf8",
            cursorclass=MySQLdb.cursors.DictCursor)
        cursor = db.cursor()
        cursor.execute(sql)
        cursor.scroll(0,mode='absolute')
        results = cursor.fetchall()
        fields = cursor.description
        workbook = xlwt.Workbook(encoding='utf-8')
        sheet = workbook.add_sheet('sheet1',cell_overwrite_ok=True)
        for field in range(0,len(fields)):
            sheet.write(0,field,fields[field][0])
        row = 1
        col = 0
        for row in range(1,len(results)+1):
            for col in range(0,len(fields)):
                tmp_field = fields[col][0]
                tmp_dir = results[row-1]
                    #sheet.write(row,col,tmp)
                sheet.write(row,col,tmp_dir.get(tmp_field,0))
        workbook.save("/home/data/datacenter/r_key_index/r_key_index.xls")
        db.commit()
        
        collect = []
        for result in results:
            print result
            collect.append(result)
        db.close()
        cursor.close()
        return collect
def send_mail(config):
    print 'Sending Mail...'
    message = MIMEMultipart()
    message["Accept-Charset"] = "ISO-8859-1,utf-8"
    message['From'] = 'ta@qq.com'
    message['To'] =','.join(config['to'])
    message['CC'] = ','.join(config['cc'])
    message['Subject'] = config['subject']
    message['Date'] = time.ctime(time.time())
    message['Reply-To'] = 'ta@qq.com'
    message['X-Priority'] = '3'
    message['X-MSMail-Priority'] = 'Normal'
    if config['text']:
        text = config['text']
        message.attach(text)
    smtp = SMTP(config['server'], config['port'])
    username = 'ta@qq.com'
    smtp.login(username, 'password')
    smtp.sendmail(username, config['to'] + config['cc'], message.as_string())
    print 'Send Mail OK'
    smtp.close()
    time.sleep(1)
def send_mail_to_test(context):
    send_mail({
        'to': ['ta@qq.com'],
        'cc': ['ta@qq.com'],
        'server': 'smtp.exmail.qq.com',
        'port': 25,
        'subject': '【上海数据部】数据小时播报【自动发送】',
        'username': 'ta@qq.com',
        'password': 'password',
        'text': context}
    )
def message_from_excel(filename):
    wb = xlrd.open_workbook(filename)
    ws = wb.sheet_by_name('sheet1')
    html_content = "<html>" + "<meta charset='utf-8'>" + "<p>Dear all,</p><p>以下为今天三地用户数、订单数、金额：</p>"
    content = "<table border='2'><tbody>"
    for rownum in range(ws.nrows):
        content += "<tr>"
        for colnum in range(ws.ncols):
            content += "<td> {0} </td>".format(ws.cell(rownum,colnum).value)
        content += "</tr>"
    content += "</tbody></table>"
    html_body = "<body>" + content + '<p>说明：</p><p>1、以上数据为上海数据部开发，自动发送，邮件在每日12点发送，如有疑问请联系ta@qq.com</p><p>2、如有需求变更，请联系ta@qq.com</p>' + "</body><br></br>"
    html_content += html_body
    html_content += "</html>"
    context = context = MIMEText(html_content,_subtype='html',_charset='utf-8')
    send_mail_to_test(context)
if __name__ == "__main__":
    sql_file = '/home/data/datacenter/r_key_index/sql.txt'
    ssh_host = "1.1.1.1"
    ssh_port = 1124
    #keyfile = "/root/.ssh/id_rsa"        
    ssh_password = "password"
    ssh_user = "user"
    db_host = "1.1.1.1"
    db_name = "name"
    sql = ''
    f = open(sql_file,'r')
    lines = f.readlines()
    for line in lines:
        line = line.strip('\n')
        sql += line + ' '
    db_port = 3306
    db_user = "select"
    db_passwd = "password"
    dbconnect_ssh(ssh_host,ssh_port,ssh_password,ssh_user,db_host,db_name,sql,db_port,db_user,db_passwd)
    filename = '/home/data/datacenter/r_key_index/r_key_index.xls'
    if not os.path.exists(filename):
        os.mknod(filename)
    filename = '/home/data/datacenter/r_key_index/r_key_index.xls'
    print filename
    attachfile = filename
    message_from_excel(filename)
```
