# Selenium基础知识(四)表单切换 - _天枢 - 博客园
## [Selenium基础知识(四)表单切换](https://www.cnblogs.com/yhleng/p/7509913.html)
2017-09-12 14:17 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7509913)
在测试过程中，经常会碰到frame和iframe，嵌套等情况
这种情况下直接通过id,name等等是无法定位到的
好在selenium替我们想到了这个问题switch_to方法解决问题
switch_to先定位iframe，根据iframe的id或name进行定位；那么，iframe没有id或name怎么办；
```
#可以用xpath先定位iframe呀
iframe = driver.find_element_by_xpath('//*[@class="kw"]')
#再将iframe定位对象，传给switch_to.frame对象
driver.switch_to.frame(iframe)
```
操作完当前iframe后，还可以使用**switch_to.default_conent()**跳回最外层页面
```
driver.switch_to_default_content()
```
-----------------------------------------------我是华丽的分隔线----------------------------------------------------------------------
