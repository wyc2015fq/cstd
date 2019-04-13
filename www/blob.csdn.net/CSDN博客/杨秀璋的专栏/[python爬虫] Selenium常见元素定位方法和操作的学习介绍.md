
# [python爬虫] Selenium常见元素定位方法和操作的学习介绍 - 杨秀璋的专栏 - CSDN博客

2016年07月10日 13:10:10[Eastmount](https://me.csdn.net/Eastmount)阅读数：61066标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)[Selenium																](https://so.csdn.net/so/search/s.do?q=Selenium&t=blog)[基础知识																](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)[鼠标操作																](https://so.csdn.net/so/search/s.do?q=鼠标操作&t=blog)[键盘操作																](https://so.csdn.net/so/search/s.do?q=键盘操作&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=鼠标操作&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)个人分类：[Python爬虫																](https://blog.csdn.net/Eastmount/article/category/5758691)
[
																					](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)[
							](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=Selenium&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=Python&t=blog)

这篇文章主要Selenium+Python自动测试或爬虫中的常见定位方法、鼠标操作、键盘操作介绍，希望该篇基础性文章对你有所帮助，如果有错误或不足之处，请海涵~
前文目录：
[[Python爬虫]
 在Windows下安装PhantomJS和CasperJS及入门介绍(上)](http://blog.csdn.net/eastmount/article/details/47023199)
[[Python爬虫]
 在Windows下安装PIP+Phantomjs+Selenium](http://blog.csdn.net/eastmount/article/details/47785123)
[[Python爬虫]
 Selenium自动访问Firefox和Chrome并实现搜索截图](http://blog.csdn.net/eastmount/article/details/47799865)
[[Python爬虫] Selenium实现自动登录163邮箱和Locating Elements介绍](http://blog.csdn.net/eastmount/article/details/47825633)
[[Python爬虫] Selenium+Phantomjs动态获取CSDN下载资源信息和评论](http://blog.csdn.net/eastmount/article/details/47907341)

## 一. 定位元素方法

## 官网地址：
## [http://selenium-python.readthedocs.org/locating-elements.html](http://selenium-python.readthedocs.org/locating-elements.html)
## 这里有各种策略用于定位网页中的元素(locate elements)，你可以选择最适合的方案，Selenium提供了一下方法来定义一个页面中的元素：
find_element_by_id
find_element_by_name
find_element_by_xpath
find_element_by_link_text
find_element_by_partial_link_text
find_element_by_tag_name
find_element_by_class_name
find_element_by_css_selector
下面是查找多个元素（这些方法将返回一个列表）：
find_elements_by_name
find_elements_by_xpath
find_elements_by_link_text
find_elements_by_partial_link_text
find_elements_by_tag_name
find_elements_by_class_name
find_elements_by_css_selector
除了上面给出的公共方法，这里也有两个在页面对象定位器有用的私有方法。这两个私有方法是find_element和find_elements。
常用方法是通过xpath相对路径进行定位，同时CSS也是比较好的方法。举例：

```python
<html>
 <body>
  <form id="loginForm">
   <input name="username" type="text" />
   <input name="password" type="password" />
   <input name="continue" type="submit" value="Login" />
   <input name="continue" type="button" value="Clear" />
  </form>
</body>
<html>
```
定位username元素的方法如下：
```python
username = driver.find_element_by_xpath("//form[input/@name='username']")
username = driver.find_element_by_xpath("//form[@id='loginForm']/input[1]")
username = driver.find_element_by_xpath("//input[@name='username']")
```
[1] 第一个form元素通过一个input子元素，name属性和值为username实现
[2] 通过id=loginForm值的form元素找到第一个input子元素
[3] 属性名为name且值为username的第一个input元素

## 二. 操作元素方法
在讲述完定位对象(locate elements)之后我们需要对该已定位对象进行操作，通常所有的操作与页面交互都将通过WebElement接口，常见的操作元素方法如下：
clear 清除元素的内容
send_keys 模拟按键输入
click 点击元素
submit 提交表单
举例自动访问FireFox浏览器自动登录163邮箱。

```python
from selenium import webdriver  
from selenium.webdriver.common.keys import Keys  
import time
# Login 163 email
driver = webdriver.Firefox()  
driver.get("http://mail.163.com/")
elem_user = driver.find_element_by_name("username")
elem_user.clear
elem_user.send_keys("15201615157")  
elem_pwd = driver.find_element_by_name("password")
elem_pwd.clear
elem_pwd.send_keys("******")  
elem_pwd.send_keys(Keys.RETURN)
#driver.find_element_by_id("loginBtn").click()
#driver.find_element_by_id("loginBtn").submit()
time.sleep(5)  
assert "baidu" in driver.title  
driver.close()  
driver.quit()
```
首先通过name定位用户名和密码，再调用方法clear()清除输入框默认内容，如“请输入密码”等提示，通过send_keys("**")输入正确的用户名和密码，最后通过click()点击登录按钮或send_keys(Keys.RETURN)相当于回车登录，submit()提交表单。
PS：如果需要输入中文，防止编码错误使用send_keys(u"中文用户名")。
![](https://img-blog.csdn.net/20150830204117581)


## 三. WebElement接口获取值
通过WebElement接口可以获取常用的值，这些值同样非常重要。
size 获取元素的尺寸
text 获取元素的文本
get_attribute(name) 获取属性值
location 获取元素坐标，先找到要获取的元素，再调用该方法
page_source 返回页面源码
driver.title 返回页面标题
current_url 获取当前页面的URL
is_displayed() 设置该元素是否可见
is_enabled() 判断元素是否被使用
is_selected() 判断元素是否被选中
tag_name 返回元素的tagName
举例代码如下：

```python
from selenium import webdriver  
from selenium.webdriver.common.keys import Keys  
import time
driver = webdriver.PhantomJS(executable_path="G:\phantomjs-1.9.1-windows\phantomjs.exe")   
driver.get("http://www.baidu.com/")
size = driver.find_element_by_name("wd").size
print size
#尺寸: {'width': 500, 'height': 22}
news = driver.find_element_by_xpath("//div[@id='u1']/a[1]").text
print news
#文本: 新闻
href = driver.find_element_by_xpath("//div[@id='u1']/a[2]").get_attribute('href')
name = driver.find_element_by_xpath("//div[@id='u1']/a[2]").get_attribute('name')
print href,name
#属性值: http://www.hao123.com/ tj_trhao123
location = driver.find_element_by_xpath("//div[@id='u1']/a[3]").location
print location
#坐标: {'y': 19, 'x': 498}
print driver.current_url
#当前链接: https://www.baidu.com/
print driver.title
#标题: 百度一下， 你就知道
result = location = driver.find_element_by_id("su").is_displayed()
print result
#是否可见: True
```
其中图片解释如下图所示。
![](https://img-blog.csdn.net/20150830212125212)



## 四. 鼠标操作
在现实的自动化测试中关于鼠标的操作不仅仅是click()单击操作，还有很多包含在ActionChains类中的操作。如下：
context_click(elem) 右击鼠标点击元素elem，另存为等行为
double_click(elem) 双击鼠标点击元素elem，地图web可实现放大功能
drag_and_drop(source,target) 拖动鼠标，源元素按下左键移动至目标元素释放
move_to_element(elem) 鼠标移动到一个元素上
click_and_hold(elem) 按下鼠标左键在一个元素上
perform() 在通过调用该函数执行ActionChains中存储行为
举例如下图所示，获取通过鼠标右键另存为百度图片logo。代码：
```python
import time
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.action_chains import ActionChains
driver = webdriver.Firefox()
driver.get("http://www.baidu.com")
#鼠标移动至图片上 右键保存图片
elem_pic = driver.find_element_by_xpath("//div[@id='lg']/img")
print elem_pic.get_attribute("src")
action = ActionChains(driver).move_to_element(elem_pic)
action.context_click(elem_pic)
#重点:当右键鼠标点击键盘光标向下则移动至右键菜单第一个选项
action.send_keys(Keys.ARROW_DOWN)
time.sleep(3)
action.send_keys('v') #另存为
action.perform()
#获取另存为对话框(失败)
alert.switch_to_alert()
alert.accept()
```
效果如下图所示，通过xpath定位到图片位置并右击鼠标，在弹出的菜单中选择“另存为图片”。但是如何点击“另存为对话框”的“保存”按钮是个难点，目前刚学习阶段，境界没到无法解决。原因：
WebDriver cannot directly interact with dialog windows this is because dialog windows are the domain of the operating system and
 not the webpage.
![](https://img-blog.csdn.net/20150901180745999)![](https://img-blog.csdn.net/20150901180812130)
该部分推荐参考资料：
[selenium 右键下载图片，结合sikuli - tobecrazy](http://www.cnblogs.com/tobecrazy/p/4523314.html)
[Selenium WebDriver 中鼠标和键盘事件分析及扩展](http://www.ibm.com/developerworks/cn/java/j-lo-keyboard/)
[Selenium Windows Save/Open Open Dialouge - StackOver](http://stackoverflow.com/questions/15576430/selenium-windows-save-open-open-dialouge)
书籍《selenium2 python自动化测试》 作者：虫师


## 五. 键盘操作
参考：[http://selenium-python.readthedocs.org/api.html](http://selenium-python.readthedocs.org/api.html)
前面讲述了鼠标操作，现在讲述键盘操作。在webdriver的Keys类中提供了键盘所有的按键操作，当然也包括一些常见的组合键操作如Ctrl+A(全选)、Ctrl+C(复制)、Ctrl+V(粘贴)。更多键参考官方文档对应的编码。
send_keys(Keys.ENTER) 按下回车键
send_keys(Keys.TAB) 按下Tab制表键
send_keys(Keys.SPACE) 按下空格键space
send_keys(Kyes.ESCAPE) 按下回退键Esc
send_keys(Keys.BACK_SPACE) 按下删除键BackSpace
send_keys(Keys.SHIFT) 按下shift键
send_keys(Keys.CONTROL) 按下Ctrl键
send_keys(Keys.ARROW_DOWN) 按下鼠标光标向下按键
send_keys(Keys.CONTROL,'a') 组合键全选Ctrl+A
send_keys(Keys.CONTROL,'c') 组合键复制Ctrl+C
send_keys(Keys.CONTROL,'x') 组合键剪切Ctrl+X
send_keys(Keys.CONTROL,'v') 组合键粘贴Ctrl+V
这里使用的例子参考虫师的书籍《selenium2 python自动化测试》，推荐该书给大家。代码还是非常有意思的，大家自己去感受下吧~
```python
#coding=utf-8
import time
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
driver = webdriver.Firefox()
driver.get("http://www.baidu.com")
#输入框输入内容
elem = driver.find_element_by_id("kw")
elem.send_keys("Eastmount CSDN")
time.sleep(3)
#删除一个字符CSDN 回退键
elem.send_keys(Keys.BACK_SPACE)
elem.send_keys(Keys.BACK_SPACE)
elem.send_keys(Keys.BACK_SPACE)
elem.send_keys(Keys.BACK_SPACE)
time.sleep(3)
#输入空格+"博客"
elem.send_keys(Keys.SPACE)
elem.send_keys(u"博客")
time.sleep(3)
#ctrl+a 全选输入框内容
elem.send_keys(Keys.CONTROL,'a')
time.sleep(3)
#ctrl+x 剪切输入框内容
elem.send_keys(Keys.CONTROL,'x')
time.sleep(3)
#输入框重新输入搜索
elem.send_keys(Keys.CONTROL,'v')
time.sleep(3)
#通过回车键替代点击操作
driver.find_element_by_id("su").send_keys(Keys.ENTER)
time.sleep(3)
driver.quit()
```
最后希望文章对你有所帮助吧，如果有错误或不足之处，还请海涵~同时接下来学习下Linux和下面作者的文章。
[http://www.dotblogs.com.tw/larrynung/archive/2012/09/26/75065.aspx](http://www.dotblogs.com.tw/larrynung/archive/2012/09/26/75065.aspx)
（By:Eastmount 2016-7-10 下午1点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


