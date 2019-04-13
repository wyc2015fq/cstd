
# Python设计模式之中介模式简单示例 - jiahaowanhao的博客 - CSDN博客


2018年04月13日 20:38:59[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：33标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


[Python设计模式之中介模式简单示例](http://cda.pinggu.org/view/25263.html)
本文实例讲述了Python设计模式之中介模式。分享给大家供大家参考，具体如下：
Mediator Pattern：中介模式
中介模式提供了一系列统一的系统接口。此模式也被认为是行为模式，因为他能选择程序处理流程。
当许多类开始在交互中产生结果时，可以选用中介模式。当软件开始组织的时候，许多用户的要求添加更多的功能。
这就导致了要和以前的类不断交互，除了新类。随着系统的复杂度加大，类之间的交互变得频繁，维护代码变得困难。
中介模式 就是为了解决这个问题，通过允许类之间的松耦合。这样中介模式就能了解系统中所有类的功能。类的功能就是与中介类进行交互。当类与类之间需要交互的时候，类就发送信息给中介，中介就转发信息给被请求的类。通过这样，类与类之间的复杂度就减少了。
一个简单的中介模式例子：
一个类型的中介模式例子可以在测试自动框架（包含4个类，TC,TestManager，Reporter ，DB）中被证明。
1.TC类是测试的响应，借助方法setup(),execute(),tearDown()。
2.Reporter类调用
当测试分类开始执行时，调用prepare方法。
当测试分类完成执行时，调用report()方法 ，
框架的测试响应就是好的帮助文档。
我也没弄懂中介模式，让人犯晕！
代码贴出来：
import time
class TC:
def __init__(self):
self._tm = tm
self._bProblem = 0
def setup(self):
print "Setting up the Test"
time.sleep(1)
self._tm.prepareReporting()
def execute(self):
if not self._bProblem:
print "Executing the test"
time.sleep(1)
else:
print "Problem in setup,Test not executed."
def tearDown(self):
if not self._bProblem:
print "Tearing down"
time.sleep(1)
self._tm.publishReport()
else:
print "Test not executed.No tear down required."
def setTM(self, TM):
self._tm = tm
def setProblem(self, value):
self._bProblem = value
class Reporter:
def __init__(self):
self._tm = None
def prepare(self):
print "Reporter Class is preparing to report the results"
time.sleep(1)
def report(self):
print "Reporting the results of Test"
time.sleep(1)
def setTM(self, TM):
self._tm = tm
class DB:
def __init__(self):
self._tm = None
def insert(self):
print "Inserting the execution begin status in the Database"
time.sleep(1)
import random
if random.randrange(1,4) == 3:
return -1
def update(self):
print "Updating the test results in Database"
time.sleep(1)
def setTM(self, TM):
self._tm = tm
class TestManager:
def __init__(self):
self._reporter = None
self._db = None
self._tc = None
def prepareReporting(self):
rvalue = self._db.insert()
if rvalue == -1:
self._tc.setProblem(1)
self._reporter.prepare()
def setReporter(self, reporter):
self._reporter = reporter
def setDB(self, db):
self._db = db
def publishReport(self):
self._db.update()
rvalue = self._reporter.report()
def setTC(self, tc):
self._tc = tc
if __name__ == '__main__':
reporter = Reporter()
db = DB()
tm = TestManager()
tm.setReporter(reporter)
tm.setDB(db)
reporter.setTM(tm)
db.setTM(tm)
while(1):
tc = TC()
tc.setTM(tm)
tm.setTC(tc)
tc.setup()
tc.execute()
tc.tearDown()

运行结果：
![](http://files.jb51.net/file_images/article/201801/201819104647397.png?20180910470)

