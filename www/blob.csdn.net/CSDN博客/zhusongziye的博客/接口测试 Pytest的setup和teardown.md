# 接口测试 Pytest的setup和teardown - zhusongziye的博客 - CSDN博客





2018年04月12日 20:30:07[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：714








pytest实际上是python自带测试框架unittest的扩展，那么pytest是如何实现unittest中的setup和teardown的呢？




### pytest初始化的类别和作用域
- 
模块级别（Module level setup/teardown）：作用于一个模块内的所有class和def，对于所有class和def，setup和teardown只执行一次




defsetup_module(module):

""" 

setup any state specific to the 
execution of the given module.

"""

defteardown_module(module):

""" 
teardown any state that was previously 
setup with a setup_module
method.
"""





- 
类级别（Class level setup/teardown）：作用于一个class内中的所有test,所有用例只执行一次setup，当所有用例执行完成后，才会执行teardown




@classmethod
defsetup_class(cls):

""" setup any state specific to the execution 
of the given class (which
usually contains tests).
"""


@classmethod
defteardown_class(cls):""" teardown any state that was previously 
setup with a call to
setup_class.
"""- 
方法和函数级别（Method and function level setup/teardown）：作用于单个测试用例，若用例没有执行（如被skip了）或失败了，则不会执行teardown




defsetup_method(self, method):

""" 
setup any state tied to the execution 
of the given method in a
class. setup_method is invoked for 
every test method of a class.
"""


defteardown_method(self, method):

"""
teardown any state that was 
previously setup with a setup_method
call.
"""



若用例直接写在模块中，而不是在类中，则用：
defsetup_function(function):

""" 
setup any state tied to the execution of the given function.
Invoked for every test function in the module.
"""

defteardown_function(function):

""" 
teardown any state that was previously setup with
 a setup_function call.
"""``- 
pytest.fixture()装饰函数，结合yield实现初始化和teardown
举个例子(pytest）文档中的：



import smtplib
import pytest
@pytest.fixture(scope="module")defsmtp():
    smtp = smtplib.SMTP("smtp.gmail.com",587, timeout=5)
    yield smtp # provide the fixture valueprint("teardown smtp")


    smtp.close()



运行结果：

```
$ pytest -s -q --tb=no
FFteardown smtp2 failed in 0.12 seconds
```

### pytest用例初始化操作的示例

为了体现初始化和环境恢复，本节演示采用邮件发送的脚本，可查看邮件发送的脚本：python发送邮件脚本或者参考文章：SMTP发送邮件。

#### 1、setup_method(self, method)
- 
在test_method.py中构建了3个测试用例，每个用例在执行前后都会执行setup_method/teardown_method连接smtp和断开smtp。




import smtplib
from email.mime.text import MIMEText
from email.header import Header
from email.mime.multipart import MIMEMultipart
import pytest

classTestSmtp():# 发件人和收件人,换成你自己的发件人、收件人qq号
    sender ="sender@qq.com"
    receivers ="rece@qq.com"# 邮箱服务器
    smtpserver ="smtp.qq.com"
    smtpport =465# 连接邮箱服务器，qq邮箱和密码,换成自己的
    username ="sendr@qq.com"
    password ="qq mail's password"
    smtp = smtplib.SMTP_SSL()defsetup_method(self, method):
        self.smtp.connect(self.smtpserver, self.smtpport)
        self.smtp.login(self.username, self.password)print("成功登录")defteardown_method(self, method):
        self.smtp.quit()print("断开连接")deftest_send_text(self):# 邮件发送、接收人员，邮件标题、正文
        msg = MIMEText("微信公众号号：开源优测","plain","utf-8")
        msg["From"]= self.sender
        msg["To"]= self.receivers
        msg["Subject"]= Header("开源优测_DeepTest_from_chenlele_text","utf-8")# 发送邮件
        self.smtp.sendmail(self.sender, self.receivers, msg.as_string())deftest_send_html(self):
        msg = MIMEText("<p>微信公众号号：开源优测</p><a href='http://www.testingunion.com'>开源优测社区</a>>","html","utf-8")
        msg["From"]= self.sender
        msg["To"]= self.receivers
        msg["Subject"]= Header("开源优测_DeepTest_from_chenlele_html","utf-8")# 发送邮件
        self.smtp.sendmail(self.sender, self.receivers, msg.as_string())deftest_send_attchment(self):# 邮件格式说明、发送、接收人员信息、邮件标题
        msg = MIMEMultipart()
        msg["From"]= self.sender
        msg["To"]= self.receivers
        msg["Subject"]= Header("开源优测_DeepTest_from_chenlele","utf-8")# 构建带附件的邮件正文
        msg.attach(MIMEText("微信公众号：开源优测","plain","utf-8"))# 构造附件,多个附件同理
        attach1 = MIMEText(open("judge_leap.json",'rb').read(),"base64","utf-8")
        attach1["Content-Type"]="application/octet-stream"# 这里filename随意写，将会在邮件中显示
        attach1["Content-Disposition"]="attrachment;filename=code.py"# 关联附件到正文
        msg.attach(attach1)# 发送邮件
        self.smtp.sendmail(self.sender, self.receivers, msg.as_string())


- 
查看结果，采用pytest -s -q 运行，-s 可以查看打印信息，-q减少输出信息：




![](https://img-blog.csdn.net/20180412202810636?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

test_method结果.png

- 




#### 2、setup_class(cls)
- 
作用于class的setup_class/teardown_class，类中所有的用例只会执行一次，如图所示；

- 
ps:用例与test_method.py的一致，参考上一串代码。


![](https://img-blog.csdn.net/20180412202849187?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

test_class.png




#### 3、setup_module(module)
- 
setup_module/teardown_module在一个模块内，只会执行一次，作用于模块内的所有用例

- 
示例中构建了2个class和1个def，共4个用例，可以看到，4个用例只执行了一次module


![](https://img-blog.csdn.net/20180412202905187?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

test_module结果.png




##### test_module.py



import smtplib
from email.mime.text import MIMEText
from email.header import Header
from email.mime.multipart import MIMEMultipart
import pytest

info ={"sender":"your@qq.com","receivers":"yourother@qq.com","smtpserver":"smtp.qq.com","smtpport":465,"username":"your@qq.com","password":"yourpassword","smtp": smtplib.SMTP_SSL()}

defsetup_module(module):
    info["smtp"].connect(info["smtpserver"], info["smtpport"])
    info["smtp"].login(info["username"], info["password"])print("成功登录")defteardown_module(module):
    info["smtp"].quit()print("断开连接")

classTestSendText():deftest_send_text(self):# 邮件发送、接收人员，邮件标题、正文
        msg = MIMEText("微信公众号号：开源优测","plain","utf-8")
        msg["From"]=info["sender"]
        msg["To"]= info["receivers"]
        msg["Subject"]= Header(
                "开源优测_DeepTest_from_chenlele_text",
                "utf-8")# 发送邮件
        info["smtp"].sendmail(info["sender"],
            info["receivers"],
            msg.as_string())deftest_send_html(self):
        msg = MIMEText("<p>微信公众号号：开源优测</p><a href='http://www.testingunion.com'>开源优测社区</a>>","html","utf-8")
        msg["From"]= info["sender"]
        msg["To"]= info["receivers"]
        msg["Subject"]= Header(
            "开源优测_DeepTest_from_chenlele_html",
"utf-8")# 发送邮件
        info["smtp"].sendmail(info["sender"],
                info["receivers"],
                msg.as_string())


classTestSendAttach():deftest_send_attchment(self):# 邮件格式说明、发送、接收人员信息、邮件标题
        msg = MIMEMultipart()
        msg["From"]= info["sender"]
        msg["To"]= info["receivers"]
        msg["Subject"]= Header(
                "开源优测_DeepTest_from_chenlele",
"utf-8")# 构建带附件的邮件正文
        msg.attach(MIMEText("微信公众号：开源优测",
"plain","utf-8"))# 构造附件,多个附件同理
        attach1 = MIMEText(open("judge_leap.json",'rb').read(),"base64","utf-8")
        attach1["Content-Type"]="application/octet-stream"# 这里filename随意写，将会在邮件中显示
        attach1["Content-Disposition"]="attrachment;filename=code.py"# 关联附件到正文
        msg.attach(attach1)# 发送邮件
        info["smtp"].sendmail(info["sender"],
                info["receivers"],
                msg.as_string())


deftest_send_text_out():# 邮件发送、接收人员，邮件标题、正文
    msg = MIMEText("微信公众号号：开源优测","plain","utf-8")
    msg["From"]=info["sender"]
    msg["To"]= info["receivers"]
    msg["Subject"]= Header("class外的用例执行","utf-8")# 发送邮件
    info["smtp"].sendmail(info["sender"], info["receivers"], msg.as_string())



#### 4、pytest.fixture()
- 
pytest.fixture采用yield实现setup和teardown操作，yield提供的参数为函数名称

- 
与setup_module类似，pytest.fixture可作用于一个模块内的所有def和class。区别在于，必须将pytest.fixture()装饰的函数作为参数传递给用例。

- 
pytest.fixture()装饰的函数必须作为参数传递给用例吗？
1）、将class中的smtp_ini都删除，class中的用例执行失败，def用例执行成功；
2）、将class中test_send_text的smtp_ini保留，其余2个删除，class中的用例都执行成功？这是为什么呢？只有1个用力传入了参数，但所有用例都执行成功了。
3）、将class和def中的smtp_ini都删除，用例全部执行失败。

- 
ps：用例内容与test_module.py的一致，就不粘代码了。


![](https://img-blog.csdn.net/20180412202941519?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

image.png

### 总结

4种方式的作用域：
- 
setup_method:仅作用于class用例集中的用例，置于class内,每个用例都会调用一次

- 
setup_function:作用于独立的def用例，不可作用于class内的用例

- 
setup_class:作用于class用例集中的用例，置于class内，只在class用例执行的开始执行setup_class,结束时执行teardown_class

- 
setup_module:作用于模块内的所有用例，置于class外，只在所以用例的开始执行setup_module,结束时执行teardown_module

- 
pytest.fixture():作用于模块内的所有用例，但需要传递装饰函数为参数，可置于class内或class外

- 






