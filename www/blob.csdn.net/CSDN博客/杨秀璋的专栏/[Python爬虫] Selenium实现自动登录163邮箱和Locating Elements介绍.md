
# [Python爬虫] Selenium实现自动登录163邮箱和Locating Elements介绍 - 杨秀璋的专栏 - CSDN博客

2015年08月21日 17:57:43[Eastmount](https://me.csdn.net/Eastmount)阅读数：23917所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)



前三篇文章介绍了安装过程和通过Selenium实现访问Firefox浏览器并自动搜索"Eastmount"关键字及截图的功能。而这篇文章主要简单介绍如何实现自动登录163邮箱，同时继续介绍Selenium+Python官网Locating Elements部分内容。
希望该篇基础性文章对你有所帮助，如果有错误或不足之处，请海涵~
[[Python爬虫]
 在Windows下安装PhantomJS和CasperJS及入门介绍(上)](http://blog.csdn.net/eastmount/article/details/47023199)
[[Python爬虫]
 在Windows下安装PIP+Phantomjs+Selenium](http://blog.csdn.net/eastmount/article/details/47785123)
[[Python爬虫] Selenium自动访问Firefox和Chrome并实现搜索截图](http://blog.csdn.net/eastmount/article/details/47799865)
注意：好像访问浏览器在C盘会自动生成文件越来越小，但可以清理，不知道为啥?

## 一. Selenium自动登录
代码如下所示：

```python
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import time
#模拟登陆163邮箱
driver = webdriver.Firefox()
driver.get("http://mail.163.com/")
#用户名 密码
elem_user = driver.find_element_by_name("username")
elem_user.send_keys("15201615157")
elem_pwd = driver.find_element_by_name("password")
elem_pwd.send_keys("********")
elem_pwd.send_keys(Keys.RETURN)
time.sleep(5)
assert "baidu" in driver.title
driver.close()
driver.quit()
```
运行结果如下图所示，自动打开Firefox浏览器并输入用户名和密码实现邮箱登录。
![](https://img-blog.csdn.net/20150821031951877)
代码非常简单，其中的原理也很简单：通过driver访问Firefox浏览器及URL，同时find_element_by_name找到网页HTML源代码中对应的值并填充，最后调用Keys实现模拟操作键盘Keys.RETURN实现。该断言结果是不存在的，主要是用于防止关闭浏览器。
同时输入用户名或密码错误会提示，其实就是浏览器。
![](https://img-blog.csdn.net/20150821041240596)
**原理解释**
两年前在学习C\#网络编程时，我成写过Winform自动访问163邮箱的文章：
[C\# 网络编程之网页自动登录 (一).使用WebBrower控件模仿登录](http://blog.csdn.net/eastmount/article/details/9703757)
通过对比，Python简短高效的优势就显示出来的，其中163邮箱登录界面HTML源码也没有修改和修复过，这是我意料之外的。
其中通过查找该登录页面发现用户名Id为“idInput”，密码Id为“pwdInput”，登录按钮ID为“loginBtn”。如图id和name：
<input class="" tabindex="1" title="请输入帐号"id="idInput" name="username"type="text" value=""..
<input class="" tabindex="2" title="请输入密码"id="pwdInput" name="password"type="password" />
<button id="loginBtn" class="" type="submit">登  录</button>
如下图所示一目了然：
![](https://img-blog.csdn.net/20150821034139613)
**这部分文章虽然简单，但是作为基础文章在合适不过了，同时通过webdriver的driver.find_element_by_name引出下面的基础知识介绍，毕竟实践例子才是学习Selenium的动力源泉。**
**同样下面这段代码可实现自动登录CSDN，是不是可以通过它实现暴力破解密码呢？**
```python
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
import time
driver = webdriver.Firefox()
driver.get("https://passport.csdn.net/account/login?from=http://my.csdn.net/my/mycsdn")
elem_user = driver.find_element_by_name("username")
elem_user.send_keys("Eastmount")
elem_pwd = driver.find_element_by_name("password")
elem_pwd.send_keys("********")
elem_pwd.send_keys(Keys.RETURN)
time.sleep(5)
assert "baidu" in driver.title
driver.close()
driver.quit()
```




## 二. Locating Elements介绍
PS：第一次上传翻译博文，如果有错误还请见谅！
官网地址：[http://selenium-python.readthedocs.org/locating-elements.html](http://selenium-python.readthedocs.org/locating-elements.html)
这里有各种策略用于定位网页中的元素(locate elements)，你可以选择最适合的方案，Selenium提供了一下方法来定义一个页面中的元素：
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
除了上面给出的公共方法，这里也有两个在页面对象定位器有用的私有方法。这两个私有方法是find_element和find_elements，用法示例：
```python
from selenium.webdriver.common.by import By
driver.find_element(By.XPATH, '//button[text()="Some text"]')
driver.find_elements(By.XPATH, '//button')
```
这些都是通过类可获取的属性：
```python
ID = "id"
XPATH = "xpath"
LINK_TEXT = "link text"
PARTIAL_LINK_TEXT = "partial link text"
NAME = "name"
TAG_NAME = "tag name"
CLASS_NAME = "class name"
CSS_SELECTOR = "css selector"
```

### 1 Locating By Id
当你知道一个元素的id属性时使用该功能。有了这个方法，用id属性值匹配时第一个被定位的元素将被返回。如果没有元素匹配id值，一个NoSuchElementException异常将会抛出。例如，参考这个页面源码：

```python
<html>
 <body>
  <form id="loginForm">
   <input name="username" type="text" />
   <input name="password" type="password" />
   <input name="continue" type="submit" value="Login" />
  </form>
 </body>
<html>
```
表单form元素可以被如下方式定位：
```python
login_form = driver.find_element_by_id('loginForm')
```

### 2 Locating By Name
当你知道一个元素的name属性时使用该方法。通过该方法，第一个满足name属性值的元素将被匹配返回，如果没有元素匹配，将抛出一个NoSuchElementException异常。例如，参考下面源码：

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
定位username&password元素方法如下：
```python
username = driver.find_element_by_name('username')
password = driver.find_element_by_name('password')
```
在"Clear"按钮之前会给出"Login"登录按钮：
```python
continue = driver.find_element_by_name('continue')
```

### 3 Locating By XPath

### XPath是用于定位XML文档中节点的语言。正如HTML可以是XML(XHTML)的一个实现，Selenium用户可以利用这个强大的语言来跟踪Web应用程序中的元素。XPath扩展已经超出(以及支持)了按照id或name属性定位的简单方法，并开发了各种新的可能，如定位页面上的第三个复选框(checkbox)。
### 其中使用XPath的一个主要原因是：当你没有一个合适的ID或Name属性来定位你需要查找的元素时，你可以使用XPath去定位这个绝对元素（不建议这样），或者相对一个有id或name属性的元素定位。XPath定位器也可以通过其他不止是id和name属性进行指定元素。
### 绝对XPath包含定位的所有元素，这些元素从根(HTML)到其结果可能会失败，只有稍微调整到应用程序。通过找到附近的一个元素的id或name属性(理想的父元素)，你才可以根据之间的关系定位到你追踪的元素。这是不太可能改变的，并且会使你的测试更加的健壮。例如参考下面这段源代码：
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
这个表单form元素可能通过如下方法被定位：
```python
login_form = driver.find_element_by_xpath("/html/body/form[1]")
login_form = driver.find_element_by_xpath("//form[1]")
login_form = driver.find_element_by_xpath("//form[@id='loginForm']")
```
[1] 绝对路径(如果HTML有稍微的改动，就会被破坏)
[2] 在HTML中的第一个表单元素
[3] 指定属性名称为id且值为loginForm的表单元素
定位username元素的方法如下：
```python
username = driver.find_element_by_xpath("//form[input/@name='username']")
username = driver.find_element_by_xpath("//form[@id='loginForm']/input[1]")
username = driver.find_element_by_xpath("//input[@name='username']")
```
[1] 第一个form元素通过一个input子元素，name属性和值为username实现
[2] 通过id=loginForm值的form元素找到第一个input子元素
[3] 属性名为name且值为username的第一个input元素
定位"Clear"按钮元素的方法如下：
```python
clear_button = driver.find_element_by_xpath("//input[@name='continue'][@type='button']")
clear_button = driver.find_element_by_xpath("//form[@id='loginForm']/input[4]")
```
[1] 属性名为name其值为continue和属性名为type其值为button的Input控件
[2] 属性id=loginForm的form元素的第四个input子元素
上面这些例子涉及一些基础知识，更多详情请参考下面的建议：
[W3Schools
 XPath Tutorial](http://www.w3schools.com/Xpath/)
[W3C
 XPath Recommendation](http://www.w3.org/TR/xpath)
[XPath
 Tutorial](http://www.zvon.org/comp/r/tut-XPath_1.html)- with interactive examples.
这里也有几个非常实用的附加组件，可以帮助发现元素的XPath：
[XPath
 Checker](https://addons.mozilla.org/en-US/firefox/addon/1095?id=1095)- suggests XPath and can be used to test XPath results.
[Firebug](https://addons.mozilla.org/en-US/firefox/addon/1843)-
 XPath suggestions are just one of the many powerful features of this very useful add-on.
[XPath
 Helper](https://chrome.google.com/webstore/detail/hgimnogjllphhhkhlmebbmlgjoejdpjl)- for Google Chrome

### 4 Locating Hyperlinks By Link Text

当你知道一个锚标记内使用链接文本就使用该方法。通过这个策略，第一个匹配这个link text值的元素将被返回。如果没有元素匹配这个链接文本，将抛出一个NoSuchElementException异常。示例的源代码如下：
```python
<html>
 <body>
  <p>Are you sure you want to do this?</p>
  <a href="continue.html">Continue</a>
  <a href="cancel.html">Cancel</a>
</body>
<html>
```
这个continue.html链接定位的方法如下,partial表示部分匹配：
```python
continue_link = driver.find_element_by_link_text('Continue')
continue_link = driver.find_element_by_partial_link_text('Conti')
```

### 5 Locating Elements By Tag Name
当你想通过tag name(标记名)定位一个元素时可以使用该方法。同样，第一个给出的tag name元素将被返回，如果没有匹配的标记名，将抛出一个NoSuchElementException异常。示例的源代码如下：
```python
<html>
 <body>
  <h1>Welcome</h1>
  <p>Site content goes here.</p>
</body>
<html>
```
定位heading(h1)元素的方法如下：
```python
heading1 = driver.find_element_by_tag_name('h1')
```

### 6 Locating Elements By Class Name

介绍类似，用于通过类属性名(class attribute name)进行定位一个元素。示例源代码如下：
```python
<html>
 <body>
  <p class="content">Site content goes here.</p>
</body>
<html>
```
其中元素"p"的定位方法如下：
```python
content = driver.find_element_by_class_name('content')
```

### 7 Locating Elements By CSS Selectors

当你想要通过CSS选择器语法定位一个元素时，可以使用该方法。它将返回第一个与CSS选择器匹配的元素，如果没有匹配CSS选择器的元素，将返回一个NoSuchElementException异常。实例源代码如下所示：
```python
<html>
 <body>
  <p class="content">Site content goes here.</p>
</body>
<html>
```
其中元素"p"的定位方法如下：
```python
content = driver.find_element_by_css_selector('p.content')
```
Sauce实验室有非常好的关于CSS选择器的文档：
[Sauce
 Labs has good documentation](http://saucelabs.com/resources/selenium/css-selectors)on CSS selectors.
（By:Eastmount 2015-8-21 下午6点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


