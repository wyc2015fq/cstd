# Selenium基础知识(六)下拉列表定位 - _天枢 - 博客园
## [Selenium基础知识(六)下拉列表定位](https://www.cnblogs.com/yhleng/p/7510235.html)
2017-09-12 15:05 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7510235)
**1、下拉列表定位**
要选择下拉列表中的元素，要先定位到，下拉列表元素，然后可以通过xpath去点击，表内内容
例如，百度搜索-->百度设置－－>搜索设置－－>选择下拉列表框内"每页显示50条
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170912145851766-1468899757.png)
**#先定位元素**
**element = driver.find_element_by_id("nr") **
**#再通过元素下找xpath定位到内容，并点击**
***element.find_element_by_xpath("//option[@value='50']").click()***
**#当然如果，页面上只有一个下拉列表//option；也可以这样写:直接一步定位**
***driver.find_element_by_xpath("//option[@value='50']").click()***
```
#输入selenium 搜索
    driver.find_element_by_id('kw').send_keys('selenium')
    driver.find_element_by_id('su').click()
    #移动到 设置
    element = driver.find_element_by_name('tj_settingicon')
    ActionChains(driver).move_to_element(element).perform()
    #单击，弹出的Ajax
    driver.find_element_by_link_text('搜索设置').click()
    #下拉列表处理
    element = driver.find_element_by_id("nr")
    element*.find_element_by_xpath("//option[@value='50']").click()*
```
