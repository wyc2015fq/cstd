# Selenium基础知识(三)元素判断 - _天枢 - 博客园
## [Selenium基础知识(三)元素判断](https://www.cnblogs.com/yhleng/p/7509205.html)
2017-09-12 11:30 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7509205)
一、由**于有些页面加载完后，某些元素还没有加载，这样就会导致异常，脚本稳定性变差**
**为了解决这个问题，selenium提供了WebDriverWait以及implicitly_wait()等待**
**WebDriverWait显示等待：**
expected_conditions类，提供预期条件判断：
```
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as ex
WebDriverWait(driver,5,0.5).until(ex.presence_of_element_located((By.ID,"kw")))
'''
WebDriverWait(driver,timeout,poll_frequency)在单位时间内，检测元素是否存在。返回bool型
:parameter
driver 驱动
timeout等待时间
poll_frequency检测时间间隔
-------------------------------------------------------------------
一般WebDriverWait配合until和until_not使用
在单位时间内，until直到返回True，until_not相返
WebDriverWait(driver,timeout,poll_frequency).unitl(ex.presence_of_element_located((By.ID,"kw")))
需要使用到的包：
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as ex
from selenium.webdriver.common.by import By
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
presence_of_element_located是判断元素是否存在
ex下还有很多的方法供使用，例如，判断复选是否是勾选等
*****************************************************************
使用时需要注意事项:ex.presence_of_element_located((By.ID,"kw"))
这个方法，要有两个括号，因为参数只能是一个元组
'''
```
 expected_conditions类：
```
ex.element_located_selection_state_to_be((By.ID,"kw")) #元素的选中状态是否符合预期
ex.element_selection_state_to_be(element) #与上一个用法相同，区别在于一个参数是定位，一个是定位后的元素
ex.alert_is_present() #判断页面上，是否存在Alert弹出框
ex.element_located_to_be_selected((By.ID,"kw")) #某个预期元素是否被选中
ex.element_to_be_selected(element) #与上一个用法相同，区别在于一个参数是定位，一个是定位后的元素
ex.element_to_be_clickable() #判断元素是否可见并且可以点击
ex.frame_to_be_available_and_switch_to_it() #判断该表单是否，可以切换进去，可以返回True，并且Switch进去，否则返回False
ex.invisibility_of_element_located() #判断某元素是否存在于Dom树或不可见
ex.new_window_is_opened() #是否有窗口被打开
ex.presence_of_all_elements_located()
ex.presence_of_element_located() #判读元素是否存在
ex.text_to_be_present_in_element() #判读元素中的Text是否包含了预期字符串
ex.text_to_be_present_in_element_value#判断元素的Value是否包含了预期字符串
ex.title_contains(title) #判读当前页面，标题是否包含预期字符串
ex.title_is(title) #判读当前页面，标题是否为预期
ex.visibility_of_all_elements_located()
ex.visibility_of_element_located((By.ID,"kw")) #判断某元素是否可见
ex.visibility_of(element) #与上个用法相同，区别在于参数是定位后的元素，上一个是传的定位
ex.visibility_of_any_elements_located((By.CLASS_NAME,"a#")) #判断是否至少有一个元素在页面中可见,如果定位到就返回列表
```
is_displayed()元素定位下有这个方法，也可以判断元素是否可见
```
driver.find_element_by_id('kw').is_displayed()
```
** 隐式等待：implicitly_wait()**
**通过一段时间的等待，等待页面加载完成**
```
driver = webdriver.Ie()
driver.implicitly_wait(10)
driver.get('https://www.baidu.com/')
```
implicitly_wait(s)参数为秒
