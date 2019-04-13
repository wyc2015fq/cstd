
# python的开发环境配置-Eclipse-PyDev插件安装 - anlcy - 博客园






# [python的开发环境配置-Eclipse-PyDev插件安装](https://www.cnblogs.com/camilla/p/7132136.html)
安装PyDev插件的两种安装方法：
1、百度搜索PyDev 2.4.0.zip，下载后解压，得到Plugins和Feature文件夹，复制两文件夹到Eclipse目录，覆盖即可。
插件的版本要对应python的版本，版本下载地址：https://sourceforge[.NET](http://lib.csdn.net/base/dotnet)/projects/pydev/files/pydev/
完成后重启Eclipse，若在Eclipse菜单Help->About Eclipse->Installation Detail->Plug-ins，能看到PyDev组件，则表示安装成功。
![](https://images0.cnblogs.com/blog/548251/201307/20020314-37eef9e35450452ca4a222baeb40c72e.jpg)
2、直接在Eclipse中选择菜单：Help—Install New Software..—Add，输入[http://pydev.org/updates](http://pydev.org/updates)，下载并安装。
![](https://images0.cnblogs.com/blog/548251/201307/20020827-d5044714c0584b949a7c0fd177fc8485.jpg)
**配置****PyDev**
安装好 PyDev 之后，需要配置 Python/Jython 解释器，配置过程很简单。
在 Eclipse 菜单栏中，选择 Window > Preferences > Pydev > Interpreter - Python，在这里配置 Python/解释器,添加已安装的解释器。这里，Python 安装在 C:\Python27 路径下。单击 New，选择 Python 解释器 python.exe，打开后显示出一个包含很多复选框的窗口，选择需要加入系统**PYTHONPATH****的路径，单击****Ok****。**
![](https://images0.cnblogs.com/blog/548251/201307/20021936-5e9a95bd1f9e4774bde66105cc25a3d6.jpg)
#### 第七步：执行Selenium实例
下面，我们来创建一个python项目。
在 Eclipse 菜单栏中，选择 File > New > Project > Pydev > Pydev Project，新建项目：PythonCase，单击 Next。
![](https://images0.cnblogs.com/blog/548251/201307/20021159-7ad5fc43bfdc4625a90e3f98cd0eb273.jpg)
完成后如下：
![](https://images0.cnblogs.com/blog/548251/201307/20022102-336ba396b6eb4345867987658ebb383a.jpg)
**创建****Python****包和模块**
接下来，在刚创建的项目中开始创建 Python 包和模块。
进入 Pydev 透视图，在 Python Package Explorer 中，右键单击 src，选择 New->Pydev Package，输入 Package 名称Python27。
单击 Finish，Python 包就创建好了，此时，自动生成__init__.py 文件，该文件不包含任何内容。
注意：
如果在创建项目的时候没有选中“Create default src folder and add it to the pythonpath”复选框，则需要通过 File > New > Other > Source Folder 手动创建一个源代码文件夹src。
创建完 Pydev Package 后，右键单击创建的包，选择 New->Pydev Module，输入模块名称PythonCase1.py Finish。这样，Python 模块就建成了。
![](https://images0.cnblogs.com/blog/548251/201307/20022300-edc9f4b63119429ca0a7ceaca2f0bab0.jpg)
修改PythonCase1.py内容如下：
\#-*-conding=utf-8 -*-
from selenium import webdriver
if __name__ ==*"__main__"*:
driver = webdriver.Firefox()
driver.implicitly_wait(30)
driver.get(*"http://www.google.com.hk"*)
driver.find_element_by_name(*"q"*).send_keys(*"hello**Selenium!"*)
driver.find_element_by_name(*"q"*).submit()
print*'Page title is:'*,driver.title
driver.quit()
**执行脚本**
运行Run_selenium.bat，启动Selenium RC服务器。右击PythonCase1.py，Run As->Python Run，执行成功结果如下：
![](https://images0.cnblogs.com/blog/548251/201307/20022548-378632c4144f4f1ca92eff5e65c60284.jpg)
如果插件安装失败，检查是否版本不对：
安装的是JDK7（也就是Java 7），而Pydev的最新版5.0.0是需要Java 8的。支持Java 7的最后一个版本是pydev 4.5.5。pydev 4.5.5最后支持java7的版本
**解决办法：**
既然如此，那我们安装pydev 4.5.5就行了。
可以看到下面有这么一句：
> See:
> [update sites page](http://www.pydev.org/update_sites/index.html)
> for the update site of older versions of PyDev.
如果需要安装旧版本的pydev，去update sites page。 那就打开这个网站，找到pydev 4.5.5. 链接如下：[http://www.pydev.org/update_sites/index.html](http://www.pydev.org/update_sites/index.html)
打开之后，可以看到pydev 4.5.5版本的插件更新的地址为：
> [http://www.pydev.org/update_sites/4.5.5/](http://www.pydev.org/update_sites/4.5.5/)
点击菜单【help】->【Install New Software】，输入上面Pydev 4.5.5的地址。然后选择PyDev和PyDev Mylyn Integration，按照提示一步步完成。完成之后，重启eclipse，就可以看到PyDev了。





