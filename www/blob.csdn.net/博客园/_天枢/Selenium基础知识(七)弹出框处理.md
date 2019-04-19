# Selenium基础知识(七)弹出框处理 - _天枢 - 博客园
## [Selenium基础知识(七)弹出框处理](https://www.cnblogs.com/yhleng/p/7510366.html)
2017-09-12 15:25 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7510366)
**使用switch_to.alert方法来处理弹页面弹出的警告框**
**页面常见警告框种类：alert/confirm 确认框/prompt**
**switch_to.alert().accept()**
**switch_to.alert().text() #返回alert/confirm/prompt等警告框中的文本**
**switch_to.alert().dismiss() #解散警告框**
**send_keys('') #将文本发送到，警告框**
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
    element.find_element_by_xpath("//option[@value='50']").click()
    #保存设置
    driver.find_element_by_link_text('保存设置').click()
    time.sleep(2)
    **driver.switch_to.alert.accept() **#Alert消息框处理
```
百度，设置保存后，弹出alert框；此处接受此框。
