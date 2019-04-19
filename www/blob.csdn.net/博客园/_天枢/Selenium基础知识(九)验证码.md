# Selenium基础知识(九)验证码 - _天枢 - 博客园
## [Selenium基础知识(九)验证码](https://www.cnblogs.com/yhleng/p/7514326.html)
2017-09-13 12:38 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7514326)
**关于Selenium处理验证码总结下：**
**1、去掉验证码(这个为了测试，去掉可能性不大)**
**2、万能验证码(让开发给做一个万能验证码，可能性也不大)**
**3、OCR光学识别，python包Python-tesseract 配合image;这个方法，对于现在验证码多样化来说，识别率不高。**
**4、cookies机制(有些网站可以记住登录，下次自动登录，就是将用户信息写到了cookies中)**
**那么，我们是不是可以用代码去写cookies呢。**
**python提供了获取cookies增加cookies等用法；那么要增加我们的用户到cookies，最主要我们需要知道key**
**driver.get_cookies() #通过此方法，可以获取到cookies,可以获取到key**
```
driver.get_cookies()
    driver.add_cookie("{'name':'loginUserName','Value':'username'}")
    driver.add_cookie("{'name':'loginPassword','Value':'password'}")
```
