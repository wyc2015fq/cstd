
# python使用 HTMLTestRunner.py生成测试报告 - anlcy - 博客园






# [python使用 HTMLTestRunner.py生成测试报告](https://www.cnblogs.com/camilla/p/7243044.html)

### HTMLTestRunner.py python 2版本
下载地址：[http://tungwaiyip.info/software/HTMLTestRunner.html](http://tungwaiyip.info/software/HTMLTestRunner.html)
使用时，先建立一个”PyDev Package“，将下载下来的[HTMLTestRunner.py](http://tungwaiyip.info/software/HTMLTestRunner_0_8_2/HTMLTestRunner.py)文件拷贝在该目录下。
例子：testcase5_dynamic.py
![复制代码](https://common.cnblogs.com/images/copycode.gif)
importunittestfrom dev.widgetimportWidgetclassWidgetTestCase(unittest.TestCase):defsetUp(self):
        self.widget=Widget()deftearDown(self):
        self.widget.dispose()
        self.widget=NonedeftestSize(self):
        self.assertEqual(self.widget.getSize(), (40,40),"Wrong")deftestResize(self):
        self.widget.resize(100, 100)
        self.assertEqual(self.widget.getSize(), (100,100),"Wrong")![复制代码](https://common.cnblogs.com/images/copycode.gif)
html_report.py：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
\#coding:utf-8from libimportHTMLTestRunnerimportunittestfrom testcase5_dynamicimportWidgetTestCaseif__name__=='__main__':
    suite=unittest.makeSuite(WidgetTestCase)
    filename='D:\\myreport.html'fp=file(filename,'wb')
    runner=HTMLTestRunner.HTMLTestRunner(fp,title=u'my unit test',description=u'This is a report test')
    runner.run(suite)![复制代码](https://common.cnblogs.com/images/copycode.gif)
Run的时候，需要使用Python Run，使用Python unit-test跑测试生成不了myreport.html，目前还不知道为什么。
有时候，不会立即生成D：\\myreport.html，我们可以自己先建立一个空的myreport.html，这样再运行之后打开就会看到报告内容。
![](https://images2015.cnblogs.com/blog/777574/201512/777574-20151202160026549-1975665573.jpg)
### HTMLTestRunner.py 的python3 版本
由于 HTMLTestRunner.py 原本就是python2版本，目前还没找到python3版本，所以需要我们自己修改 HTMLTestRunner.py 文件。
#### 1. 修改的地方
问题一：No module named StringIO
原因：python 3 中 没有 StringIO 这个模块。这里我们需要使用io 这个模块来代替。
解决方法：
第94行引入的名称要改，从 import StringIO 改成import io。
相应的，539行 self.outputBuffer = StringIO.StringIO() 要改成self.outputBuffer = io.BytesIO()
问题二：AttributeError: 'dict' object has no attribute 'has_key'
原因：python 3 字典类型的object 已经不支持 has_key函数，我们需要使用in 来进行遍历。
解决方法：
定位到642行，if not rmap.has_key(cls): 需要换成 if not cls in rmap:
问题三：'str' object has no attribute 'decode'
原因：python3 里面对字符的操作中，decode已经拿掉了。
解决方法：
定位到772行，把 ue = e.decode('latin-1') 直接改成 ue = e 。
另外766还有类似的uo = o.decode('latin-1')，改成 uo=o ；
问题四 ：TypeError: can't concat bytes to str
原因：定位一下，报在了778行的内容escape(uo+ue) 
。这是因为我们上面给uo赋值的时候，走的是else流程，uo被赋值的是bytes类型的值。 
而bytes类型不能直接转化为str类型。所以我们需要在前面给uo赋值的时候先将bytes类型转换为 str类型。
解决方法：
修改768行的 uo = o ，直接改成 uo = o.decode('utf-8') 。
另外 774还有类似的  ue = e， 改成 ue = e.decode('utf-8')。
问题五：TypeError: unsupported operand type(s) for >>: 'builtin_function_or_method' and 'RPCProxy'
原因： python3  不支持 print >> sys.stderr 这种写法，这里定义输出流的话，采用print("This is print str",file=sys.stderr) 这种方式。
解决方法：
定位到631行，把print的语句修改掉，原来是print >>sys.stderr, '\nTime Elapsed: 
%s' % (self.stopTime-self.startTime)， 可改成 print('\nTime Elapsed: %s' % 
(self.stopTime-self.startTime),file=sys.stderr)
问题六：TypeError: 'str' does not support the buffer interface
原因：定位一下，问题出在118行，这里s是str类型，我们需要把传过来的s转化为bytes类型。
解决方法：
定位到118行，把 self.fp.write(s) 修改为 self.fp.write(bytes(s,'UTF-8')) 即可。

#### 2. 保存
修改后对HTMLTestRunner.py 保存一下。
#### 3. 调用语句更改
python3 里面打开文件使用 open，不要再去用file了。
即 fp = file(filename,'wb')替换成 fp = open(filename,'wb')；
关闭该文件可用fp.close()
备注： 改动之后，中文也不会乱码。

### HTMLTestRunner.py 的使用备注
1. 问题： 执行测试用例的过程中，不会打印任何东西，导致上个厕所或第二天回来时，根本不知道执行到哪了，或者执行多少测试用例了。
解决思路： 每次执行一个测试用例时，就打印该测试用例的名称。
解决方案： 调用HTMLTestRunner时，定义 verbosity 为大于1的整数，比如说 2：
runner=HTMLTestRunner.HTMLTestRunner(fp,title="xxxx",description="xxx",verbosity=2)
在控制台console 就可以看到每执行一条用例，就会有如下输出：
E  test (testcases.login.testcase1.MyTest)
ok  test (testcases.login.testcase2.MyTest)
F  test (testcases.login.testcase3.MyTest)
2. HTMLTestRunner 脚本阅读。
![复制代码](https://common.cnblogs.com/images/copycode.gif)
classOutputRedirector(object)\#将输出进行重定向classTemplate_mixin(object)\#定义生成HTML结果文件所需要的模板。\#如果我们想改变HTML的格式等待，可以在这里进行改动class_TestResult(TestResult)\#定义继承自 unittest.TestResult 的 类。\#这里重写了 unittest.TestResult 的多个方法，比如 startTest(self, test) 等等classHTMLTestRunner(Template_mixin)\#这里可以说是使用 HTMLTestRunner.py 的入口。定义了多个我们可以看到的方法，比如 run(self, test)classTestProgram(unittest.TestProgram)\#这里继承自 unittest.TestProgram 类，重写了 runTests 方法。\#用于命令行执行测试![复制代码](https://common.cnblogs.com/images/copycode.gif)
3. 缺点：使用HTMLTestRunner的执行测试用例的过程中，如果中间中断执行，则已经执行完的用例结果也不会打印到html文件。
目前我这边是自己编写脚本生成html来代替使用 HTMLTestRunner ，感觉 HTMLTestRunner 应该没有每执行一条用例就讲结果写入到html结果文件的方法。
测试案例：
1.登录百度云
2.进入“立即注册百度账号”网页
3.进入”会员中心”网页
4.生成测试报告的文件名为：2015-01-02result.html。
测试脚本：
```python
#coding=utf-8      #防止中文乱码
```
```python
from selenium
```
```python
import webdriver
```
```python
from selenium.webdriver.common.by
```
```python
import By
```
```python
#加载键盘使用的模块
```
```python
from selenium.webdriver.common.keys
```
```python
import Keys
```
```python
from selenium.webdriver.support.ui
```
```python
import Select
```
```python
from selenium.common.exceptions
```
```python
import NoSuchElementException
```
```python
#加载unittest模块
```
```python
import unittest
```
```python
import time
```
```python
import re
```
```python
#加载HTMLTestRunner，用于生成HTMLreuslt
```
```python
import HTMLTestRunner
```
```python
class
```
```python
BaiduYun
```
```python
(unittest.TestCase):
```
```python
def
```
```python
setUp
```
```python
(self):
        self.browser=webdriver.Chrome()
        self.browser.implicitly_wait(
```
```python
30)
        self.base_url=
```
```python
"http://yun.baidu.com"
        self.verficationErrors=[]
        self.accept_next_alert=
```
```python
True
```
```python
def
```
```python
Login
```
```python
(self):
        browser=self.browser
        browser.get(self.base_url+
```
```python
'/')
```
```python
u"""百度云登录"""
        browser.find_element_by_name(
```
```python
"userName").clear()           
        username=browser.find_element_by_name(
```
```python
"userName")              
        username.send_keys(
```
```python
"alu***")
        username.send_keys(Keys.TAB)
        time.sleep(
```
```python
2)
        password=browser.find_element_by_name(
```
```python
"password")
        password.send_keys(
```
```python
"***")
        password.send_keys(Keys.ENTER)
        time.sleep(
```
```python
3)
        browser.close()
```
```python
def
```
```python
Register
```
```python
(self):
        browser=self.browser
        browser.get(self.base_url+
```
```python
'/')
```
```python
u"""立即注册百度账号"""
        browser.find_element_by_class_name(
```
```python
"link-create").click()
        time.sleep(
```
```python
2)
        browser.close()
```
```python
def
```
```python
Link
```
```python
(self):
        browser=self.browser
        browser.get(self.base_url+
```
```python
'/')
```
```python
u"""会员中心"""
        browser.find_element_by_link_text(
```
```python
"会员中心").click()
        time.sleep(
```
```python
2)
        browser.close()
```
```python
def
```
```python
tearDown
```
```python
(self):
        self.browser.quit()
        self.assertEqual([],self.verficationErrors)
```
```python
if __name__==
```
```python
"__main__":
```
```python
#unittest.main()
    testunit=unittest.TestSuite()
```
```python
#将测试用例加入到测试容器中
    testunit.addTest(BaiduYun(
```
```python
"Login"))
    testunit.addTest(BaiduYun(
```
```python
"Register"))
    testunit.addTest(BaiduYun(
```
```python
"Link"))
```
```python
#获取当前时间，这样便于下面的使用。
    now = time.strftime(
```
```python
"%Y-%m-%M-%H_%M_%S",time.localtime(time.time()))
```
```python
#打开一个文件，将result写入此file中
    fp=open(
```
```python
"result"+now+
```
```python
".html",
```
```python
'wb')
    runner=HTMLTestRunner.HTMLTestRunner(stream=fp,title=
```
```python
'test result',description=
```
```python
u'result:')
    runner.run(testunit) 
    fp.close()
```

```python
F5，运行，就这样得到了test result
```





