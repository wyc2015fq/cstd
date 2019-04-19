# 模拟页面输入：python + selenium + chrome - 宇宙浪子的专栏 - CSDN博客
2015年02月06日 10:58:42[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：2523
需要的工具：
1、python 2.7 下载地址[http://www.python.org/getit/](http://www.python.org/getit/)
2、setuptools-0.6c11.win32-py2.7 下载地址[http://pypi.python.org/pypi/setuptools](http://pypi.python.org/pypi/setuptools)
3、pip-1.2.1.tar.gz 下载地址 [http://pypi.python.org/pypi/pip/1.2.1](http://pypi.python.org/pypi/pip/1.2.1)
4、npp.6.2.2.Installer 下载地址[http://notepad-plus-plus.org/](http://notepad-plus-plus.org/)
5、chromedriver_win_23.0.1240.0 下载地址[http://code.google.com/p/chromedriver/downloads/detail?name=chromedriver_win_23.0.1240.0.zip&can=2&q=](http://code.google.com/p/chromedriver/downloads/detail?name=chromedriver_win_23.0.1240.0.zip&can=2&q=)
6、selenium-2.27.0.tar.gz（下载地址[http://pypi.python.org/pypi/selenium](http://pypi.python.org/pypi/selenium)）
安装：
1、安装python2.7
2、配置环境变量添加python27的路径。C:\python27\
3、安装setuptools
4、将pip-1.2.1.tar.gz解压到C:\python27\Scripts\下面
5、运行cmd，进入C:\python27\Scripts\pip-1.2.1\，输入python setup.py install
6、运行cmd，进入C:\python27\Scripts\，运行easy_install pip
7、安装selenium
7.1 网络安装 pip install -U selenium，出现Successfully installed selenium安装成功；
7.2 离线安装 解压缩selenium-2.22.1.tar. 把 selenium 整个文件夹 放入 Python27\Lib\site-packages 目录下
8、安装webdriver
8.1 firefox
将selenium-ide-1.6.0.xpi拖进firefox浏览器，即开始安装
    Firefox浏览器选装firebug、FirePath插件
8.2 chrome
解压chromedriver.exe到Python的安装目录下，如C:\Python27。
添加C:\Users\Administrator\AppData\Local\Google\Chrome\Application\（chrome安装路径，这里是win7下的安装路径）到环境变量path
8.3 IE
     复制IEDriverServer.exe到C:\Python27
    设置IE浏览器，Internet选线安全，把各模式的“启动保护模式”设置成一样（或者全部启动，或者全部不启动）。
自定义模块
9、测试代码
from selenium import webdriver
# 创建一个chrome实例
driver = webdriver.Chrome() 
#（这个是制定google浏览器，    指定IE webdriver        driver webdriver.Ie()，    指定Firefox webdriver driver webdriver.Firefox()）
# 到百度主页
driver.get("[http://www.baidu.com](http://www.baidu.com/)")
# 定位到搜索输入框
inputElement = driver.find_element_by_xpath ("//input[@name='wd']")
# 输入查找内容
inputElement.send_keys("Cheese!")
# 点击百度一下
submitElement.submit()
# 输出网页标题
print driver.title
#退出webdriver
driver.quit()
一、相关网页地址：
1，phantomjs：这里给出github地址及其说明。
[https://github.com/ariya/phantomjs/wiki/Supported-Web-Standards](https://github.com/ariya/phantomjs/wiki/Supported-Web-Standards)
2，phantomjs:这里给出一个例子的地址
[http://pm163.lofter.com/post/aa404_197d8e](http://pm163.lofter.com/post/aa404_197d8e)
3，js对搜索引擎的影响：
[http://www.gooseeker.com/cn/node/knowledgebase/javascriptvsseo](http://www.gooseeker.com/cn/node/knowledgebase/javascriptvsseo)
4，selenium项目来源：
[http://www.infoq.com/cn/news/2011/06/selenium-arch](http://www.infoq.com/cn/news/2011/06/selenium-arch)
5，selenium 文档之一：
[http://docs.seleniumhq.org/docs/03_webdriver.jsp](http://docs.seleniumhq.org/docs/03_webdriver.jsp)
6，基于python的selenium及下载页：
[http://selenium-python.readthedocs.org/en/latest/installation.html#introduction](http://selenium-python.readthedocs.org/en/latest/installation.html#introduction)
[https://pypi.python.org/pypi/selenium](https://pypi.python.org/pypi/selenium)
7，基于PYthon的官方文档和非官方文档
[http://selenium.googlecode.com/svn/trunk/docs/api/py/index.html](http://selenium.googlecode.com/svn/trunk/docs/api/py/index.html)
[http://selenium-python.readthedocs.org/en/latest/index.html](http://selenium-python.readthedocs.org/en/latest/index.html)
8,基于chrome的webdriver：这里注意chorme的版本，现在是27.
[https://code.google.com/p/selenium/wiki/ChromeDriver](https://code.google.com/p/selenium/wiki/ChromeDriver)
[https://code.google.com/p/chromedriver/downloads/list](https://code.google.com/p/chromedriver/downloads/list)
