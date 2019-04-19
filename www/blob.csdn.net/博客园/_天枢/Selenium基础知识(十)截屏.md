# Selenium基础知识(十)截屏 - _天枢 - 博客园
## [Selenium基础知识(十)截屏](https://www.cnblogs.com/yhleng/p/7514341.html)
2017-09-13 12:46 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7514341)
自动化测试过程中，经常会用截图的方式，更直观的显示展示错误信息；selenium截图的三种方式：
```
driver.get_screenshot_as_file(r'd:\selenium.png') #保存到文件
    driver.get_screenshot_as_base64() #截图，base64的编码格式，在html界面输出的时候会用到
    driver.get_screenshot_as_png() #保存的是二进制数据
```
截图到文件，代码展示:
```
driver = webdriver.Ie()
driver.get('https://www.baidu.com/')
#输入selenium 搜索
driver.find_element_by_id('kw').send_keys('selenium')
driver.find_element_by_id('su').click()
driver.get_screenshot_as_file(r'd:\selenium.png')
driver.quit()
```
