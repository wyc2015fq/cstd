# Macaca(二) - web自动化之 Chrome实例 - zhusongziye的博客 - CSDN博客





2018年04月11日 21:01:53[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：895








Macaca的环境安装完成之后，接下来，可以开启使用Macaca尝试web自动化测试了。步骤如下：1、启动Macaca Server命令如下：macaca server --verbose

![](https://img-blog.csdn.net/20180411210019782?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


如果此时，chrome驱动没有安装的话，就会报如下错误（找不到macaca-chrome）：

![](https://img-blog.csdn.net/2018041121003524?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


在session.js 的10-18行中，启动驱动程序 是根据 用户设置的平台和浏览器名字，来启动对应的浏览器驱动程序 。所以。。要在不同的平台进行自动化测试，就需要事先安装好对应的驱动。

源码文件目录：npm\node_modules\macaca-cli\node_modules\webdriver-server\lib\server\controllers\session.js

![](https://img-blog.csdn.net/20180411210055377?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


去下载与chrome浏览器版本对应的驱动程序，然后放置在macaca-chrome下对应的目录里。在我的环境中，安装之后的chrome驱动目录为：npm\node_modules\macaca-chrome\node_modules\macaca-chromedriver\exec\chromedriverXX.exe

![](https://img-blog.csdn.net/20180411210110280?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


安装完成之后，再去启动Macaca Server就ok.

2、安装python客户端python命令：pip install wd官方使用文档：https://macacajs.github.io/wd.py/官方源码目录：https://github.com/macacajs/wd.py

3、使用python客户端代码来编写自动化用例 - 实现from macaca import WebDriver

desired_caps = {‘browserName’: ‘Chrome’, # Electon, Safari(iOS).‘platformName’: ‘desktop’, # iOS, Android.‘platformVersion’: ‘*’,‘autoAcceptAlerts’: True # Accept the Alerts in page automaticalliy.}

driver = WebDriver(desired_caps)driver.init()driver.get(“http://www.baidu.com”)

ps：相关的API可以通过官方文档来查看，或者通过查看WebDriver中的方法来查看。与Selenium基本保持一致。






