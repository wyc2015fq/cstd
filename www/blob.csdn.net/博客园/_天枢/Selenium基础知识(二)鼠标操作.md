# Selenium基础知识(二)鼠标操作 - _天枢 - 博客园
## [Selenium基础知识(二)鼠标操作](https://www.cnblogs.com/yhleng/p/7508648.html)
2017-09-12 10:04 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7508648)
**一、鼠标操作**
**这个需要使用webdriver下的ActionChains类，这个类是操作鼠标操作的：**
```
from selenium.webdriver import ActionChains
```
**鼠标操作可分为三类：鼠标移动、鼠标拖拽、鼠标点击**
```
element = driver.find_element_by_name('tj_settingicon')
#鼠标点击
ActionChains(driver).click(element).perform() #单击某元素
ActionChains(driver).click_and_hold(element).perform() #在此元素上按下左键不放
ActionChains(driver).context_click(element).perform() #在此元素上单击右键
ActionChains(driver).double_click(element).perform() #在此元素上双击
#鼠标拖拽
ActionChains(driver).drag_and_drop(source,target).perform() #从一个元素的位置，拖至另一个元素位置松开
ActionChains(driver).drag_and_drop_by_offset(source,xoffset,yoffset) #以坐标的形式拖拽，x,y
#鼠标移动
ActionChains(driver).move_by_offset(x,y) #移动到（x,y）坐标位置
ActionChains(driver).move_to_element(element) #鼠标移动到某个元素上
ActionChains(driver).move_to_element_with_offset(element,x,y) #移动到某个元素上，然后，在移动到相对坐标（x,y）上
```
上图所示，会看到，每个方法后都跟了一个perform()很奇怪是不是，这个perform相当于submit提交。
如果你的方法后边不写这个，举个例子，百度一下，你就知道
```
ActionChains(driver).click(element)  没有preform（）
```
如果后边没有跟这个，你可以看到，鼠标是点击了，但是没有进行搜索，所以perform相当于提交
**示例代码:**
百度搜索selenium,点击百度一下；鼠标移动到，设置上，点击动态显示的列表，高级搜索
```
#*_*coding:utf-8*_*
from selenium import webdriver
from selenium.webdriver import ActionChainsimport time
driver = webdriver.Ie()
driver.get('https://www.baidu.com/')
#输入selenium 搜索
driver.find_element_by_id('kw').send_keys('selenium')
driver.find_element_by_id('su').click()
#移动到 设置
element = driver.find_element_by_name('tj_settingicon')
ActionChains(driver).move_to_element(element).perform()
#单击，弹出的Ajax
driver.find_element_by_link_text('高级搜索').click()
```
