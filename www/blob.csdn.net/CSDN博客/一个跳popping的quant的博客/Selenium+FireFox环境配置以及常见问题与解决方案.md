# Selenium+FireFox环境配置以及常见问题与解决方案 - 一个跳popping的quant的博客 - CSDN博客





2018年07月28日 19:12:02[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：1409








为了可以爬取动态网页的内容，配置了Selenium+Firefox的环境。

前提：

Anaconda3

Win10

Firefox 61.0.1

Selenium 3.13

### 1、下载Firefox驱动

地址：[https://github.com/mozilla/geckodriver/releases/](https://github.com/mozilla/geckodriver/releases/)

选择合适版本的geckodriver，然后解压得到exe文件，并将geckodriver放到一个指定的位置下（我将它放在了G:\Anaconda3\Scripts下），然后将这个位置配置到Path环境变量中。

### 2、运行代码测试

```python
from selenium import webdriver

import time
driver = webdriver.Firefox()
driver.maximize_window()
time.sleep(5)
driver.get('https://www.baidu.com')
driver.close()
```

如果弹出了百度的界面就说明成功了。

### 3、常见问题

1)、selenium.common.exceptions.WebDriverException: Message: 'geckodriver' executable needs to be in PATH.

出现上面的问题说明了没有将geckodriver配置到Path环境变量中，只需将geckodriver加入到Path中即可。可以通过cmd输入geckodriver，如果出现了信息就说明配置好了。

![](https://img-blog.csdn.net/20180728190515252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 2）、ConnectionAbortedError: [WinError 10053] 你的主机中的软件中止了一个已建立的连接

先确认是否是版本的问题。

![](https://img-blog.csdn.net/20180728190916267?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0ZyYW5raWVIZWxsbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果版本没有问题的话可以下载v0.20.0 版本的驱动。



