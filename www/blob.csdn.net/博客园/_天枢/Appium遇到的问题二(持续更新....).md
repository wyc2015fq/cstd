# Appium遇到的问题二(持续更新....) - _天枢 - 博客园
## [Appium遇到的问题二(持续更新....)](https://www.cnblogs.com/yhleng/p/7505616.html)
2017-09-11 16:34 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7505616)
**Python版:**
**1、运行Appium遇到的错误：此问题是由于JDK版本要在1.7及以上。**
**Android开发要求。**
A new session could not be created. 
```
C:\Python27\python.exe D:/Demo/temp/appium.py
Traceback (most recent call last):
  File "D:/Demo/temp/appium.py", line 3, in <module>
    from appium import webdriver
  File "D:\Demo\temp\appium.py", line 14, in <module>
    driver = webdriver.Remote('http://localhost:4723/wd/hub', desired_caps)
  File "C:\Python27\Lib\site-packages\selenium\webdriver\remote\webdriver.py", line 140, in __init__
    self.start_session(desired_capabilities, browser_profile)
  File "C:\Python27\Lib\site-packages\selenium\webdriver\remote\webdriver.py", line 229, in start_session
    response = self.execute(Command.NEW_SESSION, parameters)
  File "C:\Python27\Lib\site-packages\selenium\webdriver\remote\webdriver.py", line 297, in execute
    self.error_handler.check_response(response)
  File "C:\Python27\Lib\site-packages\selenium\webdriver\remote\errorhandler.py", line 194, in check_response
    raise exception_class(message, screen, stacktrace)
selenium.common.exceptions.WebDriverException: Message: A new session could not be created. (Original error: No app set; either start appium with --app or pass in an 'app' value in desired capabilities, or set androidPackage to launch pre-existing app on device)
Process finished with exit code 1
```
**2、同上边的问题**
**A new session could not be created. **
**还有可能是因为appPackage或者appActivity名字配置不对。**
**具体怎么去查找app的这两个属性，第四篇 文章有介绍怎么去获得。**
