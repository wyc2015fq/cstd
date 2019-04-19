# Selenium基础知识(八)文本域处理textarea - _天枢 - 博客园
## [Selenium基础知识(八)文本域处理textarea](https://www.cnblogs.com/yhleng/p/7511136.html)
2017-09-12 17:11 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7511136)
**textarea文本域selenium有时处理不了，但是我们可以采用迂回，解决这个问题**
**那么，我们用到了execute_script()方法来调用javascript来解决这个问题**
**这其中涉及DOM，需要对这个有一定了解。**
**![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170912171020407-328434866.png)**
有一些文本域，通过id、name等可以定位到，但是sendkeys就不管用了。
```
jsStr = "var kw = document.getElementById('kw') ;kw.value='javascript';"
    driver.execute_script(jsStr)
```
**webdriver提供了execute_script来执行javascript脚本**
