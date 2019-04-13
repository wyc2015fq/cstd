
# Python查找最新测试报告到邮件功能 - anlcy - 博客园






# [Python查找最新测试报告到邮件功能](https://www.cnblogs.com/camilla/p/8037225.html)
\#coding=utf-8
import smtplib from email.mime.text
import MIMEText import unittest
import HTMLTestRunner import time,os
\#=============定义发送邮件==========
def send_mail(file_new):
\#发信邮箱
mail_from='testingwtb@126.com'
\#收信邮箱
mail_to='xiaoming@126.com'
\#定义正文
f = open(file_new, 'rb')
mail_body = f.read()
f.close()
msg=MIMEText(mail_body,_subtype='html',_charset='utf-8')
\#定义标题
msg['Subject']=u"自动化测试报告"
\#定义发送时间（不定义的可能有的邮件客户端会不显示发送时间）
msg['date']=time.strftime('%a, %d %b %Y %H:%M:%S %z') smtp=smtplib.SMTP()
\#连接SMTP服务器，此处用的126的SMTP服务器
smtp.connect('smtp.126.com')
\#用户名密码
smtp.login('testingwtb@126.com','123456')
smtp.sendmail(mail_from,mail_to,msg.as_string())
smtp.quit()
print 'email has send out !'
\#======查找测试报告目录，找到最新生成的测试报告文件====
def send_report(testreport):
result_dir = testreport lists=os.listdir(result_dir)
lists.sort(key=lambda fn: os.path.getmtime(result_dir+"\\"+fn))
\#print (u'最新测试生成的报告： '+lists[-1])
\#找到最新生成的文件
file_new = os.path.join(result_dir,lists[-1])
print file_new
\#调用发邮件模块
send_mail(file_new)
\#================将用例添加到测试套件===========
def creatsuite(): testunit=unittest.TestSuite()
\#定义测试文件查找的目录
test_dir='.\\test_case'
\#定义discover方法的参数
discover=unittest.defaultTestLoader.discover(test_dir,pattern ='test*.py',top_level_dir=None)
\#discover方法筛选出来的用例，循环添加到测试套件中
for test_case in discover:
print test_case testunit.addTests(test_case)
return testunit
if __name__ == '__main__':
now = time.strftime("%Y-%m-%d %H_%M_%S")
testreport = 'E:\\test_object\\report\\'
filename = testreport+now+'result.html'
fp = file(filename, 'wb')
runner =HTMLTestRunner.HTMLTestRunner( stream=fp, title=u'自动化测试报告', description=u'用例执行情况：')
alltestnames = creatsuite()
runner.run(alltestnames)
fp.close()  \#关闭生成的报告
send_report(testreport) \#发送报告





