
# selenium关于断言的使用 - anlcy - 博客园






# [selenium关于断言的使用](https://www.cnblogs.com/camilla/p/7803649.html)
**基本介绍：**
Selenium工具专门为WEB应用程序编写的一个验收测试工具。
Selenium的核心：browser bot,是用JAVASCRIPT编写的。
Selenium工具有4种：Selenium IDE, Selenium Control, Selenium Core
这儿我们主要总结了Selenium-IDE工具
Selenium-IDE只限于firefox浏览器中使用。
**Selenium命令分成三类：**
**操作(action)、辅助(accessors)和断言(assertion)：**
**操作action：**
模拟用户与 Web 应用程序的交互。一般用于操作应用程序的状态。
如点击链接，选择选项的方式进行工作。如果一个动作执行失败，或是有错误，当前的测试将会停止执行。
**操作中常见命令有**：open（打开页面）
click（点击）
clickAndWait（点击并等待）
type（文本类型）
select（选择下拉菜单）
selectWindow（选择弹出窗口）
pause（等待指定时间，以毫秒为单位，即要睡眠的时间）
setSpeed(设定执行速度。以毫秒延迟间隔长度。默认没有延迟，即为0)
setTimeout(指定等待动作完成的等待时间。默认为30秒。
需要等待的动作包括了OPEN 和WAITFOR)
goBack（模拟用户点击其浏览器上的“back”按钮）
close（模拟用户点击弹出窗体或表单标题栏上的”关闭”按钮）
click与clickAndWait的区别：
例如对比录制脚本：
Comand      Target
click             css=input[type=submit]           //句一
clickAndWait  css=input[type=submiit]          //句二
转成PHPUNIT后代码为：
$this->click(“css=input[type=submit]“);        //此句对应上面的 句一
$this->click(“css=input[type=submit]“);        //此句和下一句，对应上面的 句二
$this->waitForPageToLoad(“30000″);
区别在于：clickAndWait后会有一个默认的页面等待时间为30秒；而click没有等待时间；
Andwait这个后缀，告诉我们，该命令将使浏览器向服务器产生一个请求，使Selenium等待加载一个新的页面。
**辅助accessors：**
这是辅助工具。用于检查应用程序的状态并将结果存储到变量中。
如：storeElementPresent(locator,variableName)
其中参数：locator 表示元素定位器；variableName 用于存储结果的变量名。
即将locator定位到的状态存储到variableName变量中。
如果该元素出现返回true，否则返回false
可同断言一同使用。
**断言assertion：**
验证应用程序的状态是否同所期望的一致。
常见的断言包括:验证页面内容，如标题是否为X或当前位置是否正确，或是验证该复选框是否被勾选。
selenium 提供了三种模式的断言：assert 、verify、waitfor
Assert 失败时，该测试将终止。
Verify 失败时，该测试将继续执行，并将错误记入日显示屏 。也就是说允许此单个 验证通过。确保应用程序在正确的页面上
Waitfor 用于等待某些条件变为真。可用于 AJAX 应用程序的测试。
如果该条件为真，他们将立即成功执行。如果该条件不为真，则将失败并暂停测试。直到超过当前所设定的超过时间。一般跟setTimeout时间一起使用
## 断言常用的有：
**assertLocation**（判断当前是在正确的页面）、
**assertTitle**（检查当前页面的 title 是否正确）、
**assertValue**（检查 input 的值， checkbox 或 radio，有值为”on”无为”off”）、
**assertSelected**（检查 select 的下拉菜单中选中是否正确）、
**assertSelectedOptions**（检查下拉菜单中的选项的是否正确）、
**assertText**（检查指定元素的文本）、
**assertTextPresent**（检查在当前给用户显示的页面上是否有出现指定的文本）、
**assertTextNotPresent**（检查在当前给用户显示的页面上是否没有出现指定的文本）、
**assertAttribute**（检查当前指定元素的属性的值）、
**assertTable**（检查 table 里的某个 cell 中的值）、
**assertEditable**（检查指定的 input 是否可以编辑）、
**assertNotEditable**（检查指定的 input 是否不可以编辑）、
**assertAlert**（检查是否有产生带指定 message 的 alert 对话框）、
**verifyTitle**（检查预期的页面标题）
**verifyTextPresent**（验证预期的文本是否在页面上的某个位置）
**verifyElementPresent**（验证预期的UI元素，它的HTML标签的定义，是否在当前网页上）
**verifyText**（核实预期的文本和相应的HTML标签是否都存在于页面上）
**verifyTable**（验证表的预期内容）
**waitForPageToLoad**（暂停执行，直到预期的新的页面加载）
**waitForElementPresent**（等待检验某元素的存在。为真时，则执行。)
验证和断言的区别：验证失败后不影响脚本的继续执行，断言失败后将停止脚本的执行。
## 异常类型
**AssertionError:**assert语句失败
**AttributeError**:试图访问一个对象没有的属性
**IOError**:输入输出异常，基本是无法打开文件
**ImportError**:无法引入模块或者包，基本是路径问题
**IndentationError**:语法错误，代码没有正确的对齐
**IndexError**:下标索引超出序列边界
**KeyError**：试图访问字典里不存在的键
**KeyboadrInterrupt**:Ctrl+c被按下
**NameError**:使用一个还未赋值对象的变量
**SyntaxError**:python代码逻辑语法错误，不能执行
**TypeError**:传入的对象类型与要求不符
**UnboundLocalError:**试图访问一个还未设置的全局变量，基本上是由于另有一个同名的全局变量，导致你以为在访问
**ValueError**:传入一个不被期望的值，即使类型正确
## UnitTest框架中的断言
在unittest单元测试框架中，TestCase类提供了一些方法来检查并报告故障
![](https://images2017.cnblogs.com/blog/74815/201711/74815-20171108134119872-165546597.png)
例子：
>>> try:
num = input("Enter a number:")
assert (num == 10),"The number is not 10"
except AssertionError,msg:
print msg
print ("Sadly ,num not equals to 10")

Enter a number:10

====================================
>>> try:
num = input("Enter a number:")
assert (num == 10),"The number is not 10"
except AssertionError,msg:
print msg
print ("Sadly ,num not equals to 10")

Enter a number:20
The number is not 10
Sadly ,num not equals to 10
文件显示：
\# -*- coding:utf-8 -*-
import unittest
class Assert_test(unittest.TestCase):
def test_case(self):
try:
num = input("Enter a number")
self.assertEqual(num, 10, msg="The number is not 10"),
except AssertionError, msg:
print msg

if __name__ == "__main__":
unittest.main()





