# Python Splinter自动化测试基础 - zhusongziye的博客 - CSDN博客





2018年03月10日 09:06:02[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：135








Splinter介绍

Splinter是一个用Python语言开发的开源web应用测试工具；
Splinter对已有的自动化工具（如：Selenium、phantomJS）进行抽象，形成一个全新的上层应用API，使用它编写web自动化脚本变的更加容易。




### Splinter安装

Splinter依赖以下包：

Selenium（版本>=24）

Django（1.7>版本>=1.5.8）

Flask(版本>=0.10)

lxml(版本>=2.3.6)

zope.testbrowser(版本>=4.0.4)

注：前提是环境中已经安装好Python，小编的测试平台是window10x64
- 
从Python官网（https://pypi.python.org/pypi/splinter/）上下载Splinter；


![](https://img-blog.csdn.net/20180310090207768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




2.将下载好的splinter解压缩到指定的文件夹中，小编的目录是D:\soft\soft\Splinter

![](https://img-blog.csdn.net/20180310090243217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




3.使用cmd进入终端，并cd到splinter解压缩后得目录

![](https://img-blog.csdn.net/20180310090256818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




4.使用命令Python setup.py build构建一下

![](https://img-blog.csdn.net/20180310090307792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




5.执行脚本命令Python setup.py install 进行安装splinter

![](https://img-blog.csdn.net/20180310090318889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




6.检查splinter是否安装成功，在Python从splinter.browser中导入Browser，没有任何报错就是splinter安装成功了

![](https://img-blog.csdn.net/20180310090329457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




7.下载火狐浏览器驱动Geckodriver及实例代码截图

（https://github.com/mozilla/geckodriver/releases），启动浏览器并进入百度页面，




![](https://img-blog.csdn.net/20180310090342733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




基础示例代码：
fromsplinterimportBrowser


withBrowser()asbrowser:# Visit URLurl="http://www.baidu.com"

    # 访问百度    browser.visit(url)

    # 输入搜索关键字    browser.fill('kw','开源优测')

    # 点击“百度一下”按钮    # Find and click the 'search' buttonbutton=browser.find_by_id('su')# Interact with elementsbutton.click()


作者： 草虫


简书地址：https://www.jianshu.com/u/258bd71bc9ca

声明：转载，著作权归作者所有。



