
# Windows环境selenium+Python环境配置 - anlcy - 博客园






# [Windows环境selenium+Python环境配置](https://www.cnblogs.com/camilla/p/7130656.html)
**1.安装Python**
访问Python官方网站。
根据自己的操作系统32/64 位，选择相应的版本。
安装过程我就不详细描述了，动动手指头，**Google**一下，你就知道。我的安装目录为：C:\Python35
**2.验证是否安装成功**
进入cmd(windows命令提示符)下面输入"python"命令。
Tips:
如果提示python不是内部或外部命令！去配置一下环境变量即可。
修改我的电脑->属性->高级->环境变量->系统变量中的PATH为:
变量名：PATH
变量值：C:\Python35;
也可以在Python安装界面，勾选Add Python3.5 to PATH复选框，在安装完成后自动完成path的配置**工作**。
**3.安装pip**首先需要在Python的官网上去下载，下载地址是：https://pypi.python.org/pypi/pip\#downloads
下载完成之后，解压到一个文件夹，用CMD控制台进入解压目录，输入：
$ python setup.py install
安装好之后，我们直接在命令行输入pip，同样会显示‘pip’不是内部命令，也不是可运行的程序。因为我们还没有添加环境变量。
按照之前介绍的添加环境变量的方法，我们在PATH最后添加：
C:\Python35\Scripts;
然后重新打开CMD验证。
Tips：
在最新的Python安装包中已经集成了pip，可以在Python的安装目录C:\Python35\Scripts下查看是否有pip.exe或pip3.exe文件。
**4.安装Selenium**
通过pip命令可以直接安装
在命令窗口输入pip install -U selenium安装selenium
**5.第一个自动化脚本**
完成了以上的准备工作，我们的Selenium+Python自动化测试环境就搭建好了，下面就可以编写自动化脚本了。
from selenium import webdriver
from time import sleep或者from selenium.webdriver.common.keys import Keys
driver = webdriver.Firefox()
driver.get("http://www.baidu.com")
print('设置浏览器全屏打开')
driver.maximize_window()
driver.find_element_by_xpath(".//*[@id='kw']").send_keys("python")
driver.find_element_by_xpath(".//*[@id='su']").click()
sleep(5)
driver.quit()
**6.安装浏览器驱动**
各个浏览器驱动下载地址：http://www.seleniumhq.org/download/
chromeDriver下载地址：[http://chromedriver.storage.googleapis.com/index.html](http://chromedriver.storage.googleapis.com/index.html)
然后将下载得到的exe文件放到python的安装目录下。
安装完成后可以用IE和Chrome来替换Firefox运行上面的例子。
将
driver = webdriver.Firefox()改为driver = webdriver.Firefox(log_path = None) 不写日志
1.启用firefox，查看C:\Python27\Lib\site-packages\selenium\webdriver\firefox中的webdriver.py，在def_init_函数中，executable_path="geckodriver",之前搭建的环境上是executable_path="wires"；
2.用geckodriver，geckodriver是一原生态的第三方浏览器，对于selenium3.x版本都会使用geckodriver来驱动firefox，所以需要下载geckodriver.exe,下载地址：https://github.com/mozilla/geckodriver/releases3.放在C:\Python27/driver（查看环境变量path中是否添加C:\Python27/driver该路径） 驱动路径也要加到环境变量中
替换为：
driver = webdriver.Chrome()
或：
driver = webdriver.Ie()
如果程序能够调用相应的浏览器运行，说明浏览器驱动安装成功。





