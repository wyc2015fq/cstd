
# [CentOS Python系列] 一.阿里云服务器安装部署及第一个Python爬虫代码实现 - 杨秀璋的专栏 - CSDN博客

2018年02月13日 16:10:22[Eastmount](https://me.csdn.net/Eastmount)阅读数：7095所属专栏：[Python爬虫之Selenium+Phantomjs+CasperJS](https://blog.csdn.net/column/details/eastmount-spider.html)[CentOS云服务端之Python爬虫及数据库知识](https://blog.csdn.net/column/details/19699.html)



从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。
Python基础知识系列：[Pythonj基础知识学习与提升](http://blog.csdn.net/column/details/eastmount-python.html)
Python网络爬虫系列：[Python爬虫之Selenium+Phantomjs+CasperJS](http://blog.csdn.net/column/details/eastmount-spider.html)
Python数据分析系列：[知识图谱、web数据挖掘及NLP](http://blog.csdn.net/column/details/eastmount-kgdmnlp.html)
它们都是基于Windows系统下的Python编程，每个系列都从安装过程、基础知识到实际应用三个方面进行讲解，但在写这些文章的时候，始终有两个缺陷：一是没有介绍Linux系统下的Python编程，并且所有代码使用的都是Python 2.7版本；另一方面是如何结合服务器，真实的项目中，通常会将Python代码托管到服务器中。
![这里写图片描述](https://img-blog.csdn.net/20180212160809122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161506304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161524640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同时，随着人工智能和深度学习的风暴来临，Python变得越来越火热，作者也准备从零学习这些知识，写相关文章。本篇文章是作者学习部署阿里云服务器 CentOS环境，然后完成了第一个Python BeautifulSoup爬虫，是非常基础的入门文章，后面将陆续深入，学习如何将Python定时爬虫托管到服务器、爬取的数据存储至数据库中、爬虫数据分析等知识。
文章非常基础，希望这系列文章对您有所帮助，如果有错误或不足之处，还请海涵。


## 一.
## 阿里云服务器CentOS搭建
阿里云的CentOS云系统是一个集成了Python环境基于yum安装的镜像，包含Nginx、MySQL、Pyenv、IPython等。

> CentOS（Community Enterprise Operating System，中文意思是：社区企业操作系统）是Linux发行版之一，它是来自于Red Hat Enterprise Linux依照开放源代码规定释出的源代码所编译而成。由于出自同样的源代码，因此有些要求高度稳定性的服务器以CentOS替代商业版的Red Hat Enterprise Linux使用。两者的不同，在于CentOS并不包含封闭源代码软件。
阿里云提供了很多的产品供大家购买使用，非常方便，这里作者随便购买了一个，不一定适用，自己也是摸着石头过河，一点点尝试吧，为了学习这钱其实还是值的。

**1.购买centOS系统**
如君云时代提供的环境地址：Python运行环境（CentOS6.8 64位）
![](https://img-blog.csdn.net/20180212160826122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

该产品的说明如下所示：

![](https://img-blog.csdn.net/20180212212449501?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**2.点击购买，进入选择配置页面进行购买**
![](https://img-blog.csdn.net/20180212214917847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**3.购买成功之后，生成如下的订单页面**
这里作者选择了云服务器ECS、镜像CentOs6.8，支持Python运行环境、djangoV1.1、nginxV1.1、mysqlV5.7。
![](https://img-blog.csdn.net/20180212234748174?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**4.在菜单页面点击设置密码，登录名root、登录密码需设****置**
![](https://img-blog.csdn.net/20180212235319113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**5.点击“去下单”按钮并支付**
![](https://img-blog.csdn.net/2018021223552963?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**6.去到阿里云控制台，并点击云服务器ECS**
![](https://img-blog.csdn.net/20180213000457255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后在左边栏目点击“云服务器ECS”图标，生成如下图所示界面，可以看到我们的服务器已经运转。
![](https://img-blog.csdn.net/20180213000635454?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**7.在控制台中实现云服务器远程登录**
![](https://img-blog.csdn.net/20180213090250299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
点击“远程登录”后弹出的界面会提示远程连接密码，请记住这个密码，它将一直用于远程登录服务器。
![](https://img-blog.csdn.net/20180213090529995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

接下来就是输入远程连接密码实现登录。
![](https://img-blog.csdn.net/20180213090721514?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**8.在弹出的黑框中，输入登录名root，密码为第4步骤设置的自定义密码**
![](https://img-blog.csdn.net/20180213090920808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意：讲到这里一个简单的阿里云服务器就搭建成了，其他云服务器都是类似的配置方法，比较简单，这里作者使用的是CentOS系统，接下来将带领大家体验Linux的基础命令及写第一个Python代码。



## 二.
## CentOS基础命令及Python入门

作者首先给出Linux系统下常见的命令，详见：Putty基础教程之（一）.入门命令学习及编写HTML
```python
cd 文件夹名：查看路径
dir：查看当前路径下的所有文件
ls：列出当前文件夹下所有内容
pwd：显示当前路径
clear： 清空终端屏幕命令，Ctrl+L清除屏幕
mkdir [dir]： 新建文件夹
mv *** ****：移动或者重命名
rm *** ***：删除一次可以删除多个
cp：拷贝
touch 文件名： 新建文件
vi 文件名： 编辑文件命令
cat 文件名： 查看文件内容
wget （路径）：下载
unzip ***.zip：压缩文件到当前目录
ls -ltr  |  cat  -vte： 显示文件夹下所有内容详细信息
```
**1.清屏 命令：clear**
![](https://img-blog.csdn.net/20180213092810200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**2.查看文件内容**
命令：dir或ls
```python
$ls
```
```python
列出当前文件夹下所有内容
```
```python
$ls
```
```python
-o
```
```python
列出当前文件夹中所有内容，含详细信息，但不列出
```
```python
group
```
```python
$ls
```
```python
-l
```
```python
同上，含
```
```python
group
```
```python
信息
```
```python
$ls
```
```python
-a
```
```python
列出当前文件夹中所有内容，包含以”
```
```python
.
```
```python
”开头的文件
```
```python
$ls
```
```python
-t
```
```python
按更改时间排序
```
```python
$ls
```
```python
-v
```
```python
按版本先后排序
```
如下图所示，可以看到我已经创建的三个Python文件，“ls -o”显示详细信息。
![](https://img-blog.csdn.net/20180213093448691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**3.新建文件夹**
命令：mkdir 文件夹
创建文件夹eastmount，执行”mkdir eastmount”后显示新建文件夹。
![](https://img-blog.csdn.net/20180213094019941?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**4.查看路径**
命令：cd 进入文件夹
Change Directory ，”cd yxz”表示进入yxz文件夹，”cd ~”表示返回home根目录，”cd ..”表示返回上一层目录（类似DOS）。
更改目录 Change Directory (cd)：cd /path/to/directory/，有DOS基础的同学应该知道，路径可以用相对也可以用绝对。
![](https://img-blog.csdn.net/20180213095710476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**5.创建Python文件**
命令：touch first.py
在eastmount目录创建first.py文件，如果touch了已有的文件，则会更新那个文件的日期为当前。
![](https://img-blog.csdn.net/20180213113341870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**6.查看Python版本**
命令：python -V
可以看到是Python3.4.1，需要注意print()为输出，同时调用一些扩展包方法也有别于Python2.7版本。
![](https://img-blog.csdn.net/20180213113815999?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**7.编辑Python程序**
命令： vi first.py
注意：
(1) 输入”vi first.py”后，用vi打开文件后，是处于「命令行模式（command mode）」，您要切换到「插入模式（Insert mode）」才能够输入文字。切换方法：在「命令行模式（command mode）」下按一下字母「i」就可以进入「插入模式（Insert mode）」，这时候你就可以开始输入文字了。
(2) 编辑好后，需从插入模式切换为命令行模式才能对文件进行保存，切换方法：按「ESC」键。
(3) 保存并退出文件：在命令模式下输入:wq即可！（别忘了wq前面的:）
![](https://img-blog.csdn.net/20180213124649134?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
执行步骤：
```python
输出指令：vi first.py
点击键盘：i键 - 编辑代码(Insert)
输出代码：
```
```python
#!/usr/bin/python
```
```python
print(
```
```python
"this is my first python code."
```
```python
)
print(
```
```python
"hello,Eastmount!!!"
```
```python
)
print(
```
```python
"Bye, Nana."
```
```python
)
点击键盘：Esc键
输出命令：:wq
```
![](https://img-blog.csdn.net/20180213124757287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
注意，我们也可以使用“vim first.py”编辑代码，并且它是彩色显示，如下图所示：
![](https://img-blog.csdn.net/20180213125326535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**8.运行Python程序**
命令：python first.py
输出结果如下所示：
```python
this
```
```python
is
```
```python
my first python code.
Hello, Eastmount!!!
Bye, Nana.
```
![](https://img-blog.csdn.net/20180213125037915?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意：使用Linux系统时，如果一个屏幕显示不下内容，想切换上一页下一页，可以使用“Shift+PageUp”快捷键查看上一页内容，“Shift+PageDown”查看下一页内容。

## 三. Python实现BeautifulSoup爬虫代码
Python可以调用pip工具安装扩展包，这是非常方便的一个手段，这里我们通过该扩展包安装BeautifulSoup和Selenium爬虫扩展包。
**安装Selenium扩展包**
命令：pip install selenium
![](https://img-blog.csdn.net/20180213115013697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装BeautifulSoup扩展包
命令：pip install bs4
![](https://img-blog.csdn.net/20180213131222990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**查看已安装扩展包**
命令：pip list
![](https://img-blog.csdn.net/20180213131547980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**创建第一个爬虫代码bsfirst.py**
命令：touch bsfirst.py
**编辑代码**
命令：vim bsfirst.py
![](https://img-blog.csdn.net/20180213131929421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**输入代码**
```python
#!/usr/bin/python
```
```python
import
```
```python
sys
```
```python
import
```
```python
urllib
```
```python
import
```
```python
urllib.request
```
```python
from
```
```python
bs4
```
```python
import
```
```python
BeautifulSoup
print(sys.getdefaultencoding())
url =
```
```python
"http://www.baidu.com"
```
```python
print(url)
content = urllib.request.urlopen(url).read()
```
```python
#print(content)
```
```python
soup = BeautifulSoup(content,
```
```python
"html.parser"
```
```python
)
print(soup.title)
print(soup.title.get_text())
```
![](https://img-blog.csdn.net/20180213132719757?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**输出结果**
![](https://img-blog.csdn.net/20180213133152530?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意，它输出了中文乱码，下一篇文章将讲解怎么解决该问题。


## 四. Putty基础用法
PuTTY是一个Telnet、SSH、rlogin、纯TCP以及串行接口连接软件。PuTTY为一开放源代码软件，主要由Simon Tatham维护，使用MIT licence授权。
随着Linux在服务器端应用的普及，Linux系统管理越来越依赖于远程。在各种远程登录工具中，Putty是出色的工具之一。它是一个免费的、Windows x86平台下的Telnet、SSH和rlogin客户端，但是功能丝毫不逊色于商业的Telnet类工具。

**连接远程服务器**
运行界面如下图所示：
![](https://img-blog.csdn.net/20180213133435892?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**登录阿里云服务器**
在Host Name填写公网IP地址，比如39.xx.xx.166，点击“Open”按钮，如下图所示。

![](https://img-blog.csdn.net/20180213133645754?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
登录后的界面如下图所示，填写登录名“root”以及对应登录密码。

![](https://img-blog.csdn.net/20180213133958356?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后就可以操作该远程服务器了，比如运行我们的bsfirst.py爬虫代码。

![](https://img-blog.csdn.net/20180213134258584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
可以看到，爬取了百度搜索官网的标题“百度一下，你就知道”，更多BeautifulSoup知识推荐下面，这里不再深入。 通过Putty我们可以更好的管理远程服务器以及上传文件到服务器上，总之，希望这篇基础文章对您有所帮助，尤其是刚接触云服务器的新手，如果您是高手，还请多提意见，共同提高。
祝大家新年快乐，又一年过去了，娜我们来年一起进步加油。
（By：Eastmount CSDN 秀璋 2018-02-13 中午1点[http://blog.csdn.net/Eastmount](http://blog.csdn.net/Eastmount)）



