
# Python+Selenium2 搭建自动化测试环境 - 阳光岛主 - CSDN博客

2018年04月02日 10:52:18[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1829标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)[Selenium																](https://so.csdn.net/so/search/s.do?q=Selenium&t=blog)[xvfb																](https://so.csdn.net/so/search/s.do?q=xvfb&t=blog)[firefox																](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)[PyVirtualDisplay																](https://so.csdn.net/so/search/s.do?q=PyVirtualDisplay&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=xvfb&t=blog)个人分类：[Linux/Shell																](https://blog.csdn.net/sunboy_2050/article/category/655460)
[
																								](https://so.csdn.net/so/search/s.do?q=xvfb&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=Selenium&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=Selenium&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=Python&t=blog)

米扑科技的许多项目都用到了爬虫采集网页数据，突破反爬虫、自动化测试、回归测试也要求米扑考虑构建自动化，来提高整个团队的极致工作效率。
由于忙于需求以及产品的流程规范，现在对于测试技术方面的研究也积累了很多。不过不管做什么，做好最重要！
搞自动化主要是出于团队建设考虑，一方面为了提供测试部门的工作效率，保障产品质量；另一方面，也是为了提升团队成员的测试技能，保证Team良性发展。不过不管如何，自动化是必须要搞，不然繁琐的回归测试是没有任何效率保证和质量保障的。
初步计划通过**Python**作为脚本语言，**Selenium**作为**web**端的测试工具，目前主要是基于web端来构建的。
米扑博客原文：[Python+Selenium2 搭建自动化测试环境](https://blog.mimvp.com/article/5488.html)
**Python 安装**
yum -y update
yum -y install gcc gcc-g++ python python-devel python-pip
yum -y install Xvfb firefox
pip install pyvirtualdisplay
yum -y install gcc gcc-c++ autoconf libjpeg libjpeg-devel libpng libpng-devel freetype freetype-devel libxml2 libxml2-devel zlib zlib-devel glibc glibc-devel glib2 glib2-devel bzip2 bzip2-devel ncurses ncurses-devel curl curl-devel e2fsprogs e2fsprogs-devel krb5 krb5-devel libidn libidn-devel openssl openssl-devel openldap openldap-devel nss_ldap openldap-clients openldap-servers unzip subversion
pypi 官网：[https://pypi.python.org/pypi](https://pypi.python.org/pypi)
pip 官网：[https://pypi.python.org/pypi/pip/](https://pypi.python.org/pypi/pip/)
bs4 官网：[https://pypi.python.org/pypi/beautifulsoup4/](https://pypi.python.org/pypi/beautifulsoup4/)
setuptools 官网：[https://pypi.python.org/pypi/setuptools/](https://pypi.python.org/pypi/setuptools/)
selenium 官网：[https://pypi.python.org/pypi/selenium](https://pypi.python.org/pypi/selenium#downloads)
tornado 官网：[https://pypi.python.org/pypi/tornado](https://pypi.python.org/pypi/tornado)
tornado 官网：[http://www.tornadoweb.org](http://www.tornadoweb.org/)
tornado github：[https://github.com/tornadoweb/tornado/](https://github.com/tornadoweb/tornado/)
PyVirtualDisplay 官网：[https://pypi.python.org/pypi/PyVirtualDisplay](https://pypi.python.org/pypi/PyVirtualDisplay)（包含 xvfb）
PyVirtualDisplay github：[https://github.com/ponty/PyVirtualDisplay](https://github.com/ponty/PyVirtualDisplay)
firefox 老版本下载：[http://ftp.mozilla.org/pub/firefox/releases/](http://ftp.mozilla.org/pub/firefox/releases/)（推荐）
本节主要记录简单搭建Python+Selenium测试环境的过程，具体如下：
**基础环境：****windows**7 64bit
**1、构建python开发环境**，版本为当前最新版本python2.7.5
在python官方网站选择下载最新windows安装包：python-2.7.5.amd64.msi，
注意这里选择64bit的。安装完之后，需要在系统的环境变量path中加入C:\Python27,然后可以在命令行。
**2、SetupTools和pip工具安装**
这两个工具都是属于python的第三方工具包软件，有点类似于linux下的安装包软件，不过pip比SetupTools功能更强大。
SetupTools官方解释：Download, build, install, upgrade, and uninstall Python packages -- easily!
在python的官方网站上可以找到SetupTools的下载，这里Windows只提供了32bit的下载，setuptools-0.6c11.win32-py2.7.exe，直接双击安装即可。
pip官方解释：A tool for installing and managing Python packages.
cmd进入命令行：easy_install pip 在线安装即可。
备注：此处需要注意的是，当安装SetupTools之后，就可以在python安装目录下看到Script目录，如下图所示：
![pythonselenium2-to-build-automated-test-01](https://blog.mimvp.com/wp-content/uploads/2014/08/pythonselenium2-to-build-automated-test-01.jpg)
这个目录生成之后，需要在系统环境变量的中加入 path：C:\Python27\Scripts，然后才可以在命令使用easy_install命令进行pip在线安装。
**3、安装 Selenium**
selenium 官方下载：[https://pypi.python.org/pypi/selenium\#downloads](https://pypi.python.org/pypi/selenium#downloads)
selenium 最新版本：[selenium-2.53.4.tar.gz](https://pypi.python.org/packages/78/38/ecb7e3e184439c48dde9cc1b5955048e4d952b8c978aa4d414516c3c74e5/selenium-2.53.4.tar.gz)
这里因为需要将Python和Selenium进行组合，当然Selenium也提供了基于python的实现，所以就需要把Selenium的包安装到python库中去，以便于python开发时进行调用。
在cmd进入命令行：pip install selenium执行之后，将自动化搜寻最新的selenium版本下载并安装，如下图所示：
![pythonselenium2-to-build-automated-test-02](https://blog.mimvp.com/wp-content/uploads/2014/08/pythonselenium2-to-build-automated-test-02.jpg)
以上显示，则表明在线安装selenium成功！
**4、Python + Selenium 示例**
这里可以直接在python的编辑中编写如下程序，并保存hello_selenium.py
|1
|2
|3
|4
|5
|6
|7
|8
|from|selenium|import|webdriver
|driver|=|webdriver.Firefox()
|driver.get(‘https:|/|/|blog.mimvp.com’)
|assert|"blog.mimvp.com 博客"|.decode(|'utf-8'|)|in|driver.title
|print|driver.title
|driver.close()
|

在python编辑器里面操作F5运行即可，看看是否成功调用Firefox浏览器。。。
以上一个基础的Python+Selenium的自动化环境已经搭建完成。
**Selenium 多浏览器实现**
构建Python+Selenium2自动化测试环境完成之后，就需要测试支持python的selenium的版本是否都支持在不同浏览器上运行，当前我们分别在三个最通用的浏览器上（IE，Chrome，FireFox）通过脚本来测试。
**1）****IE 浏览器**
在IE浏览器上运行测试脚本，首先需要下载IEDriverServer.exe，放在IE浏览器的安装目录且同级目录下，脚本如下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|import|os
|from|selenium|import|webdriver
|from|selenium.webdriver.common.keys|import|Keys
|iedriver|=|"C:Program FilesInternet ExplorerIEDriverServer.exe"|\# IE driver
|os.environ[|"webdriver.ie.driver"|]|=|iedriver
|driver|=|webdriver.Ie(iedriver)
|driver.get(|"https://blog.mimvp.com"|)
|assert|"Python"|in|driver.title
|elem|=|driver.find_element_by_name(|"q"|)
|elem.send_keys(|"selenium"|)
|elem.send_keys(Keys.RETURN)
|assert|"mimvp.com"|in|driver.title
|driver.close()
|driver.quit()
|

**2）Chrome 浏览器**
在Chrome浏览器上运行测试脚本，首先需要下载ChromeDriver.exe，放在Chrome浏览器的安装目录且同级目录下，脚本如下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|import|os
|from|selenium|import|webdriver
|from|selenium.webdriver.common.keys|import|Keys
|chromedriver|=|"C:Program Files (x86)GoogleChromeApplicationchromedriver.exe"
|os.environ[|"webdriver.chrome.driver"|]|=|chromedriver
|driver|=|webdriver.Chrome(chromedriver)
|driver.get(|"https://blog.mimvp.com"|)
|assert|"Python"|in|driver.title
|elem|=|driver.find_element_by_name(|"q"|)
|elem.send_keys(|"selenium"|)
|elem.send_keys(Keys.RETURN)
|assert|"mimvp.com"|in|driver.title
|driver.close()
|driver.quit()
|

**注意官网的介绍**：
Chrome Driver is maintained / supported by the Chromium project iteslf.  看来如果使用 new ChromeDriver() 的话，应该要安装 Chromium 而不是 Chrome，我现在懒得折腾了，有兴趣的童鞋可以试验一下。
**3） Firefox 浏览器**
在Firefox浏览器上运行测试脚本，具体如下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|from|selenium|import|webdriver
|from|selenium.webdriver.common.keys|import|Keys
|driver|=|webdriver.Firefox()
|driver.get(|"https://blog.mimvp.com/"|)
|assert|"Python"|in|driver.title
|elem|=|driver.find_element_by_name(|"q"|)
|elem.send_keys(|"selenium"|)
|elem.send_keys(Keys.RETURN)
|assert|"mimvp.com"|in|driver.title
|driver.close()
|driver.quit()
|

**Selenium 虚拟浏览器静默执行**（不打开窗体）
使用selenium打开网页时，FireFox浏览器会在虚拟窗体中显示，不会在当前用户窗体中打开。
**应用场景：**
非常适合在服务器端执行；非常人性化的不打扰当前用户工作，赞！
On *nix, you can run WebDriver in a headless (virtual) display to hide the browser. This can be done with[Xvfb](http://en.wikipedia.org/wiki/Xvfb).
I personally use Python on Linux, and the[PyVirtualDisplay](http://pypi.python.org/pypi/PyVirtualDisplay)module to handle Xvfb for me.
Code for running headless would look like this:
`#!/usr/bin/env python
from pyvirtualdisplay import Display
from selenium import webdriver``display = Display(visible=0, size=(800, 600))
display.start()``# now Firefox will run in a virtual display. 
# you will not see the browser.
browser = webdriver.Firefox()
browser.get('https://www.mimvp.com')
print browser.title
browser.quit()``display.stop()`Install dependencies on Debian/Ubuntu:
`$ sudo apt-get install xvfb python-pip
$ sudo pip install pyvirtualdisplay`或 CentOS 系统上
$ yum install**Xvfb**python-pip// yum install xorg-x11-server-Xvfb$ yum --enablerepo=remi install firefox
$ pip install pyvirtualdisplay或 AWS EC2 系统上
$ wget https://lambda-linux.io/epll-release-2015.09-1.1.ll1.noarch.rpm$ yum -y install epll-release-2015.09-1.1.ll1.noarch.rpm
$ yum --enablerepo=epll install firefox-compat下载firefox，解压，软链接
wget  http://download.firefox.com.cn/releases/firefox/45.0/en-US/Firefox-latest-x86_64.tar.bz2
tar jxvf Firefox-latest-x86_64.tar.bz2
mv Firefox-latest-x86_64 firefox
ln -s  /home/ec2-user/tool-server/firefox/firefox /usr/bin/firefox
参考：[Announcing Firefox Browser Support for Amazon Linux](https://lambda-linux.io/blog/2015/01/28/announcing-firefox-browser-support-for-amazon-linux/)
firefox 更多老版本下载：[http://ftp.mozilla.org/pub/firefox/releases/](http://ftp.mozilla.org/pub/firefox/releases/)（推荐）
**错误1：**
如果报错，则更新selenium到最新版，错误信息：
Message: Can't load the profile. Profile Dir: %s If you specified a log_file in the FirefoxBinary constructor, check it for details.
pip install -U selenium(高版本CentOS 7、Firefox、Selenium，此方法没用)
**错误2：**
easyprocess.EasyProcessCheckInstalledError: cmd=['Xvfb', '-help']
OSError=[Errno 2] No such file or directory
Program install error!
解决：
|1
|pip|install|xvfbwrapper
|

**错误3：**
selenium.common.exceptions.WebDriverException: Message: 'geckodriver' executable needs to be in PATH.
这是因为Firefox高版本需要使用 geckodriver 来驱动，不再使用Seleniu默认自带的Firefox webdriver。
我们只需要在[github](https://github.com/mozilla/geckodriver/releases)下载 geckodriver 并将其所在的路径设为环境变量即可解决。
解决：
|1
|brew|install|geckodriver
|

或下载安装低版本firefox
firefox-45.0.2.tar.bz2
firefox 更多老版本下载：[http://ftp.mozilla.org/pub/firefox/releases/](http://ftp.mozilla.org/pub/firefox/releases/)（推荐）
**应用实例：**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|import|bs4
|from|selenium|import|webdriver
|from|pyvirtualdisplay|import|Display

|def|spider_url(|self|, url, index, total):
|print|(|"%d/%d  -  url: %s"|%|(index, total, url))
|content|=|''
|browser|=|None
|table_soup|=|[]
|try|:
|display|=|Display(visible|=|0|, size|=|(|800|,|600|))
|display.start()
|\# now Firefox will run in a virtual display.
|\# you will not see the browser.
|browser|=|webdriver.Firefox()|\# 打开 FireFox 浏览器
|browser.set_page_load_timeout(|60|)
|browser.get(url)
|content|=|browser.find_element_by_class_name(|'table'|)|\# 通过标记id 获取网页的内容
|content|=|browser.page_source
|self|.kill_firefox(browser)
|display.stop()
|content|=|bs4.BeautifulSoup(content, from_encoding|=|'GB18030'|)
|table_soup|=|content.find(|'table'|, {|"class"|:|"table"|}).find_all(|"tr"|)
|except|Exception as ex:
|print|(|"error msg: "|+|str|(ex))
|self|.kill_firefox(browser)
|

**还有一种静默执行的方法**（供参考）：
I easily managed to hide the browser window.
Just[install PhantomJS](http://phantomjs.org/download.html). Then, change this line:
`driver = webdriver.Firefox()`to:
`driver = webdriver.PhantomJS()`The rest of your code won't need to be changed and no browser will open. For debugging purposes, use`driver.save_screenshot('screen.png')`at different steps of your code.
**总结**
通过以上三个不同浏览器上的测试，说明selenium在python中的运用于其Java版本都是一样。
由于Firefox是默认安装路径，webdriver可以正常访问找到他，如果非系统默认安装路径，则需要跟IE和Chrome一样来设置driver路径。



