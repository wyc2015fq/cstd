# python 发送email邮件带附件 - _天枢 - 博客园
## [python 发送email邮件带附件](https://www.cnblogs.com/yhleng/p/9322366.html)
2018-07-17 11:16 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9322366)
EMAIL功能实现：
**1.发送EMAIL带附件，并且带压缩文件夹做为附件**
```
#_*_coding:utf-8_*_
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.base import MIMEBase
from email.header import Header
from email import encoders
import time
import yaml,os,base64
from pos.lib import gl,scripts
class EmailClass(object):
    def __init__(self):
        self.curDateTime = str(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime())) #当前日期时间
        self.config = scripts.getYamlfield(gl.configFile) #配置文件路径
        self.sender = self.config['EMAIL']['Smtp_Sender'] # 从配置文件获取，发件人
        self.receivers = self.config['EMAIL']['Receivers']  # 从配置文件获取，接收人
        self.msg_title = self.config['EMAIL']['Msg_Title'] #从配置文件获取，邮件标题
        self.sender_server = self.config['EMAIL']['Smtp_Server'] #从配置文件获取，发送服务器
        self.From = self.config['EMAIL']['From']
        self.To = self.config['EMAIL']['To']
    '''
    配置邮件内容
    '''
    @property
    def setMailContent(self):
        print self.receivers
        msg = MIMEMultipart()
        msg['From'] = Header(self.From,'utf-8')
        msg['To'] = self.To
        msg['Subject'] = Header('%s%s'%(self.msg_title,self.curDateTime),'utf-8')
        #附件路径
        dirpath = gl.reportPath
        zipfile = os.path.join(os.path.dirname(dirpath), 'report.zip')
        reportfile = os.path.join(gl.reportPath, 'Report.html')
        scripts.zipDir(dirpath,zipfile) #压缩报告
        #增加邮件内容为html
        fp = open(reportfile, 'rb')
        reportHtmlText = fp.read()
        msg.attach(MIMEText(reportHtmlText,'html','utf-8'))
        fp.close()
        #增加附件
        html = self.addAttach(zipfile,filename='Report%s.zip'%self.curDateTime) #自动化测试报告附件
        msg.attach(html)
        return msg
    '''
    增加附件
    '''
    def addAttach(self,apath,filename='Report.html'):
        with open(apath, 'rb') as fp:
            attach = MIMEBase('application','octet-stream')
            attach.set_payload(fp.read())
            attach.add_header('Content-Disposition', 'attachment', filename=filename)
            encoders.encode_base64(attach)
            fp.close()
            return attach
    '''
    发送电子邮件
    '''
    def sendEmail(self,message):
        try:
            smtpObj = smtplib.SMTP()
            smtpObj.connect(self.sender_server,25)
            smtpObj.login(self.sender,self.config['EMAIL']['Password'])
            smtpObj.sendmail(self.sender,self.receivers , message.as_string())
            smtpObj.quit()
            print "邮件发送成功"
        except smtplib.SMTPException as ex:
            print "Error: 无法发送邮件.%s"%ex
    #发送调用
    @property
    def send(self):
        self.sendEmail(self.setMailContent)
if __name__=="__main__":
    EmailClass().send
```
**2.压缩文件夹及文件代码：**
```
def zipDir(dirpath,outFullName):
    """
    压缩指定文件夹
    :param dirpath: 目标文件夹路径
    :param outFullName: 压缩文件保存路径+xxxx.zip
    :return: 无
    """
    zip = zipfile.ZipFile(outFullName,"w",zipfile.ZIP_DEFLATED)
    for path,dirnames,filenames in os.walk(dirpath):
        # 去掉目标跟路径，只对目标文件夹下边的文件及文件夹进行压缩
        fpath = path.replace(dirpath,'')
        for filename in filenames:
            zip.write(os.path.join(path,filename),os.path.join(fpath,filename))
    zip.close()
```
**3.配置文件config.yaml**
```
Smtp_Server: smtp.exmail.qq.com
    Smtp_Sender: xxxx@163.com
    Password: xxxxxx
    Msg_Title: UI自动化测试结果
    Receivers: ['xxxx@126.com']  #收件人
    From: 自动化测试系统
    To: 测试组
```
