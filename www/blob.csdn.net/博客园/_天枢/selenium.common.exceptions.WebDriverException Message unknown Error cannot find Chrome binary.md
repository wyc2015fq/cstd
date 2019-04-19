# selenium.common.exceptions.WebDriverException: Message: unknown Error: cannot find Chrome binary - _天枢 - 博客园
## [selenium.common.exceptions.WebDriverException: Message: unknown Error: cannot find Chrome binary](https://www.cnblogs.com/yhleng/p/9503819.html)
2018-08-20 09:34 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9503819)
使用Chrome浏览器时，经常会遇到以下报错：浏览器没有调用起来
```
selenium.common.exceptions.WebDriverException: Message: unknown Error: cannot find Chrome binary
```
通常由两种问题引起的：
1.ChromeDriver.exe驱动有问题(包括版本，路径等等)
2.Chrome.exe本身有问题。
解决方案：三个
1.指定chromedriver.exe驱动绝对路径
```
driver = webdriver.Chrome(r'd:\xxx\chromedriver.exe')
```
2.添加chrome.exe到系统path环境变量
3.在代码中指定chrome.exe绝对路径。设置binary_location属性
```
option = webdriver.ChromeOptions()
    option.binary_location=r'C:\Users\Administrator\AppData\Local\Google\Chrome\Application\chrome.exe'
    driver = webdriver.Chrome()
    driver.get('https://www.baidu.com')
```
