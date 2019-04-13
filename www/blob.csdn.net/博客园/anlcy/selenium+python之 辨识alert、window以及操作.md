
# selenium+python之 辨识alert、window以及操作 - anlcy - 博客园






# [selenium+python之 辨识alert、window以及操作](https://www.cnblogs.com/camilla/p/7700143.html)

## 1.分辨
首先区别下alert、window和伪装对话框：
alert，浏览器弹出框，一般是用来确认某些操作、输入简单的text或用户名、密码等，根据浏览器的不同，弹出框的样式也不一样，不过都是很简单的一个小框。在firebug中是无法获取到该框的元素的，也就是说alert是不属于网页DOM树的。如下图所示：
![alert1](http://img.blog.csdn.net/20160824082650767)
![alert2](http://img.blog.csdn.net/20160824082752877)
![alert3](http://img.blog.csdn.net/20160824082830511)
window，浏览器窗口，点击一个链接之后可能会打开一个新的浏览器窗口，跟之前的窗口是平行关系（alert跟窗口是父子关系，或者叫从属关系，alert必须依托于某一个窗口），有自己的地址栏、最大化、最小化按钮等。这个很容易分辨。
div伪装对话框，是通过网页元素伪装成对话框，这种对话框一般比较花哨，内容比较多，而且跟浏览器一看就不是一套，在网页中用firebug能够获取到其的元素，如下图：
![div伪装](http://img.blog.csdn.net/20160824083702539)

## 2.alert操作
针对alert，selenium提供了相应的类来进行处理。
> selenium.webdriver.common.alert.
> Alert
> (
> driver
> )
先列出Alert的所有操作：
```python
Alert(driver)
```
```python
.accept()
```
```python
# 等同于点击“确认”或“OK”
Alert(driver)
```
```python
.dismiss()
```
```python
# 等同于点击“取消”或“Cancel”
Alert(driver)
```
```python
.authenticate(username,password)
```
```python
# 验证，针对需要身份验证的alert，目前还没有找到特别合适的示例页面
Alert(driver)
```
```python
.send_keys(keysToSend)
```
```python
# 发送文本，对有提交需求的prompt框（上图3）
Alert(driver)
```
```python
.text
```
```python
# 获取alert文本内容，对有信息显示的alert框
```
示例代码：
**示例1：switch_to.alert , accept() , text**
> [测试链接http://sahitest.com/demo/alertTest.htm](http://sahitest.com/demo/alertTest.htm)

```python
# -*- coding: utf-8 -*-
from selenium import webdriver
from
```
```python
time import
```
```python
sleep
driver = webdriver.Firefox()
driver.maximize_window()
driver.get(
```
```python
'http://sahitest.com/demo/alertTest.htm')
driver.find_element_by_name(
```
```python
'b1').click()
a1 = driver.switch_to.alert
```
```python
# 通过switch_to.alert切换到alert
```
```python
sleep(
```
```python
1)
```
```python
print a1.text
```
```python
# text属性输出alert的文本
a1.
```
```python
accept()
```
```python
# alert“确认”
```
```python
sleep(
```
```python
1)
driver.quit()
```
结果
```python
Alert
```
```python
Message
```
**示例2：Alert(driver) , dismiss()**
> [测试链接http://sahitest.com/demo/confirmTest.htm](http://sahitest.com/demo/confirmTest.htm)

```python
# -*- coding: utf-8 -*-
```
```python
from selenium
```
```python
import webdriver
```
```python
from time
```
```python
import sleep
```
```python
from selenium.webdriver.common.alert
```
```python
import Alert
driver = webdriver.Firefox()
driver.maximize_window()
driver.get(
```
```python
'http://sahitest.com/demo/confirmTest.htm')
driver.find_element_by_name(
```
```python
'b1').click()
a1 = Alert(driver)
```
```python
# 直接实例化Alert对象
sleep(
```
```python
1)
```
```python
print a1.text
```
```python
# text属性输出alert的文本
a1.dismiss()
```
```python
# alert“取消”
sleep(
```
```python
1)
driver.quit()
```
结果
```python
Some question?
```
**示例3：switch_to.alert , send_keys(keysToSend)**
> [测试链接http://sahitest.com/demo/promptTest.htm](http://sahitest.com/demo/promptTest.htm)

```python
# -*- coding: utf-8 -*-
from selenium import webdriver
from
```
```python
time import
```
```python
sleep
driver = webdriver.Firefox()
driver.maximize_window()
driver.get(
```
```python
'http://sahitest.com/demo/promptTest.htm')
driver.find_element_by_name(
```
```python
'b1').click()
a1 = driver.switch_to.alert
```
```python
# 通过switch_to.alert切换到alert
```
```python
sleep(
```
```python
1)
```
```python
print a1.text
```
```python
# text属性输出alert的文本
a1.send_keys(
```
```python
'send some words to alert!')
```
```python
# 往prompt型alert中传入字符串
```
```python
sleep(
```
```python
1)
a1.
```
```python
accept()
```
```python
print driver.find_element_by_name(
```
```python
't1').get_attribute(
```
```python
'value')
driver.quit()
```
结果
```python
Some prompt?
```
```python
send some
```
```python
words
```
```python
to alert!
```
authenticate(username,password)方法没有找到合适的示例网页，不在这里写示例代码，不过用法是和send_keys一样，不过是传入两个参数而已。并且这种框很少出现，便不做更多研究。
## 3.window操作
window类似于alert，不过与原window是平行的，所以只需要切换到新的window上便可以操作该window的元素。
> driver.switch_to.
> window
> (
> window_handle
> )
而window是通过window句柄handle来定位的。而selenium提供了两个属性方法来查询window句柄：
> driver.current_window_handle

> driver.window_handles
用以上两个属性获取到当前窗口以及所有窗口的句柄，就可以切换到其他的window了。
示例代码：
> [测试链接http://sahitest.com/demo/index.htm](http://sahitest.com/demo/index.htm)

```python
# -*- coding: utf-8 -*-
from selenium
```
```python
import webdriver
from time
```
```python
import sleep
from selenium.webdriver.common.alert
```
```python
import Alert
driver = webdriver.Firefox()
driver.maximize_window()
driver.get(
```
```python
'http://sahitest.com/demo/index.htm')
current_window = driver.current_window_handle
```
```python
# 获取当前窗口handle name
driver.find_element_by_link_text(
```
```python
'Window Open Test With Title').click()
all_windows = driver.window_handles
```
```python
# 获取所有窗口handle name
```
```python
# 切换window，如果window不是当前window，则切换到该window
```
```python
for
```
```python
window
```
```python
in
```
```python
all_windows:
```
```python
if
```
```python
window !=
```
```python
current_window:
        driver.switch_to.
```
```python
window(
```
```python
window)
```
```python
print driver.title
```
```python
# 打印该页面title
driver.close()
driver.switch_to.
```
```python
window(current_window)
```
```python
# 关闭新窗口后切回原窗口，在这里不切回原窗口，是无法操作原窗口元素的，即使你关闭了新窗口
```
```python
print driver.title
```
```python
# 打印原页面title
driver.quit()
```
结果：
```python
With Title
Sahi Tests
```
这里有一些注意事项：
只能通过window的name来切换窗口，但window的name不是你想象的窗口title之类的，而是一串字符串，如‘{976ae257-19be-4b32-a82e-4ba5063ed0a2}’，所以你用title、url之类的想切window是不可能的
关闭了新窗口之后，driver并不会自动跳转回原窗口，而是需要你switch回来，直接去定位窗口元素会报NoSuchElementException
## 3.div类对话框
div类对话框是普通的网页元素，通过正常的find_element就可以定位并进行操作。不在这里进行详述。注意设置一定的等待时间，以免还未加载出来便进行下一步操作，造成NoSuchElementException报错。





