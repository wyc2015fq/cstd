
# selenium webdriver+python基本操作 - anlcy - 博客园






# [selenium webdriver+python基本操作](https://www.cnblogs.com/camilla/p/7217976.html)
\# -*- coding:utf-8 -*-
\#导入模块
from selenium import webdriver
from selenium.common.exceptions import NoSuchElementException
\#选择浏览器
driver = webdriver.Firefox()
\#打开url:
driver.get("http://www.baidu.com")
\#等待
driver.implicitly_wait(30)
driver.set_page_load_timeout(30)
driver.set_script_timeout(30)
\#关闭浏览器
driver.quit()/driver.close()
\#前进/后退
driver.forward()/driver.back()
\#刷新
driver.refresh()
\#返回当前页面标题
driver.title
\#返回当前页面url
driver.current_url
\#返回当前浏览器的所有窗口
driver.window_handles
\#返回当前浏览器的窗口句柄
driver.current_window_handle
\#选择窗口
driver.switch_to_window("window_name")
\#对话框操作
driver.switch_to_alert() \#选择窗口对象
driver.accept()  \#点击确认
driver.dismiss() \#点击取消
driver.text       \#获取文本值ֵ
driver.send_keys("key")   \#输入值ֵ
\#定位元素（单个）
driver.find_element(by="id",value=None)
\#Id定位
driver.find_element_by_id("id_name")
\#Name定位
driver.find_element_by_name("name")
\#Css定位
driver.find_element_by_css_selector("\#foo")
\#Xpath定位
driver.find_element_by_xpath("//select[@id='nr']/option[2]")
\#Link定位
driver.find_element_by_link_text(u"链接")
\#Tag定位
driver.find_element_by_tag_name("foo")
\#截取当前页面
driver.get_screenshot_as_file('/Screenshots/foo.png')





