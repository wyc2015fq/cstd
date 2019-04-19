# selenium元素单击不稳定解决方法 - _天枢 - 博客园
## [selenium元素单击不稳定解决方法](https://www.cnblogs.com/yhleng/p/9299317.html)
2018-07-12 14:33 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9299317)
**selenium自动化测试过程中，经常会发现某一元素单击，很不稳定，有时候执行了点击没有反映。**
**以下总结两种解决方法：都是通过js注入的方式去点击。**
**1.F12查一看，要点击的按钮，或连接，有没有onclick=func();这样调用方法。**
 如果有的话，可以直接用js调用方法名
```
driver.execute_script("func();”)
```
**2.找到元素，之后用js注入单击。**
有人可能会问，都找到元素了，直接element.click()不就好了。
这样是可以，但是本文的内容是，有一些不稳定。element.click()，有时定位到元素了，点击没生效。
```
element = driver.find_element(*loc)
        driver.execute_script("arguments[0].click();",element)
```
 期待你的加入；共同学习，一起进步：
python｜测试｜技术交流 qq群：563227894
python｜测试｜技术交流 qq群：563227894
python｜测试｜技术交流 qq群：563227894
