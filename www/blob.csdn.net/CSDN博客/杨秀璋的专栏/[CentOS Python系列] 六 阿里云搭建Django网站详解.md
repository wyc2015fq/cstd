
# [CentOS Python系列] 六.阿里云搭建Django网站详解 - 杨秀璋的专栏 - CSDN博客

2018年03月13日 17:34:34[Eastmount](https://me.csdn.net/Eastmount)阅读数：1328所属专栏：[CentOS云服务端之Python爬虫及数据库知识](https://blog.csdn.net/column/details/19699.html)



本篇文章主要介绍讲述部署阿里云服务器Django网站环境，并通过IP地址访问网页的过程。写代码过程中往往第一步需要解决的就是配置开发环境，对于新手来说，这是非常头疼的事情，而当配置好之后或者对于老手来说，我们才能去实现理想的功能。基础性文章，希望对您有所帮助，同时文章中存在错误或不足之处，还请海涵~
同时推荐前面作者另外三个Python系列文章。

从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。
Python基础知识系列：Pythonj基础知识学习与提升
Python网络爬虫系列：Python爬虫之Selenium+Phantomjs+CasperJS
Python数据分析系列：知识图谱、web数据挖掘及NLP
![这里写图片描述](https://img-blog.csdn.net/20180212160809122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161506304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161524640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
系列文章：
[CentOS Python系列] 一.阿里云服务器安装部署及第一个Python爬虫代码实现
[CentOS Python系列] 二.pscp上传下载服务器文件及phantomjs安装详解
[CentOS Python系列] 三.阿里云MySQL数据库开启配置及SQL语句基础知识
[[CentOS Python系列] 四.阿里云服务器CentOS连接远程MySQL数据库及pymsql](http://blog.csdn.net/eastmount/article/details/79341409)
[CentOS Python系列] 五.阿里云部署web环境及通过IP地址访问服务器网页


## 一. 创建第一个Django网站
首先打开Putty软件，填写远程IP地址“39.107.105.166”，然后root用户登录，如下图所示。

![](//img-blog.csdn.net/20180313152710818)

输入"pip list"指令可以看到已经安装的Django 1.10.6版本，如果您的系统没有安装，则使用pip安装。

![](//img-blog.csdn.net/2018031315403662)

**第一步 创建Django项目Yangxz**
命令：django-admin.py startproject Yangxz
创建Yangxz网站工程，同时使用ls查看工程中所包括的内容，如下图所示。
参考：[python网站开发] 一.Django入门知识及创建第一个网站
![](//img-blog.csdn.net/20180313155445318)

**第二步 启动项目服务**
命令：python manage.py runserver 8090
开启8090端口号，但是报错如下图所示，它提示我们还有配置没有完成，需要我们再配置一下。

![](//img-blog.csdn.net/20180313155815827)
```python
[root@iZ2ze9134z8zlqupc9t6mzZ Yangxz]# ls
manage.py  Yangxz
[root@iZ2ze9134z8zlqupc9t6mzZ Yangxz]# python manage.py runserver 8090
Performing system checks...
System check identified no issues (0 silenced).
You have 13 unapplied migration(s). Your project may not work properly until you apply 
the migrations for app(s): admin, auth, contenttypes, sessions.
Run 'python manage.py migrate' to apply them.
March 13, 2018 - 07:56:34
Django version 1.10.6, using settings 'Yangxz.settings'
Starting development server at http://127.0.0.1:8090/
Quit the server with CONTROL-C.
```
**第三步 解决上述错误**
命令：python manage.py migrate
migrate可以让我们在修改Model后可以在不影响现有数据的前提下重建表结构。
![](//img-blog.csdn.net/20180313160753626)
这时你会发现在上一层的hello目录下面多了一个文件dg.sqlite3，此时再调用“python manage.py runserver 8090”命令开启8090端口则成功，如下图所示。
![](//img-blog.csdn.net/20180313161132656)
此时，我们通过“http://39.107.105.166:8090/”访问网站却无法访问，如下图所示。
![](//img-blog.csdn.net/20180313161503468)



## 二. 配置服务端口号

接下来在CentOS服务器端开通端口号8080，步骤如下：
1.vi /etc/sysconfig/iptables
2.输入字符“i”，进入编辑模式（Insert）
3.加入如下代码，核心: -I INPUT -m state --state NEW -m tcp -p tcp --dport 8090 -j ACCEPT

```python
#Firewall configuration written by system-config-firewall
#Manual customization of this file is not recommanded.
*filter
:INPUT ACCEPT [0:0]
:FORWARD ACCEPT [0:0]
:OUTPUT ACCEPT [0:0]
-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
-A INPUT -p icmp -j ACCEPT
-A INPUT -i lo -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 80 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 3306 -j ACCEPT
-A INPUT -m state --state NEW -m tcp -p tcp --dport 9090 -j ACCEPT
-I INPUT -m state --state NEW -m tcp -p tcp --dport 8090 -j ACCEPT
-A INPUT -j REJECT --reject-with icmp-host-prohibited
-A FORWARD -j REJECT --reject-with icmp-host-prohibited
COMMIT
```
![](//img-blog.csdn.net/20180313163251717)

3.输入“:wq!”保存并退出
4.重启防火墙：service iptables restart
![](https://img-blog.csdn.net/20180224140113481)


注意，开放的端口一定要添加到 REJECT 之前。同时，有的教程会让你临时关闭防火墙(如果不关闭，则只能本机访问apache服务)，service iptables stop，但作者更推荐重启防火墙即可。



## 三. 配置阿里云端口号
如果您还是无法访问如下图所示，则需要设置阿里云的安全组规则，而网上更多的资料忽略了该步骤。下面进行简单叙述：

![](http://img-blog.csdn.net/20180313161503468)


第一步：打开阿里云服务器管理控制台页面，点击“控制台”按钮

![](https://img-blog.csdn.net/20180224140458724)


第二步：在弹出的页面中，选择“云服务器ECS”，然后点击我们运行中的服务器

![](https://img-blog.csdn.net/20180224140540361)

第三步：在弹出如下界面中点击“更多”，选择低端的“安全组配置”

![](https://img-blog.csdn.net/20180224140622188)


第四步：在“本实例安全组”界面选择“配置规则”。

![](https://img-blog.csdn.net/20180224140707567)


第五步：点击“添加安全组规则”。

![](https://img-blog.csdn.net/20180224140752860)

第六步：在“添加安全组规则”中选择自定义TCP协议，端口号为8090端口，授权对象为“0.0.0.0/0”，表示允许任意公网IP登录。

![](//img-blog.csdn.net/20180313164330797)

设置成功如下图所示：
![](//img-blog.csdn.net/20180313164500831)


第七步：输入命令“python manage.py runserver 0.0.0.0:8090 &”，最后的 & 符号表示在后台运行该进程。这里的IP地址如果用公网IP会运行不了，而用0.0.0.0则外网和127.0.0.1都能访问。
重点命令：python manage.py runserver 0.0.0.0:8090 &
而此时会报错如下所示，让我们修改Yangxz.settings文件。

![](//img-blog.csdn.net/2018031317073197)

**第八步：修改Yangxz.settings.py文件，在 ALLOWED_HOSTS=['*'] 中增加 *。**
命令：vim settings.py

![](//img-blog.csdn.net/20180313171006329)
**第九步：最后再次输入命令“****python manage.py runserver 0.0.0.0:8090 &****”安装服务。**
![](//img-blog.csdn.net/20180313171056983)
此时我们访问地址“http://39.107.105.166:8090/”能成功访问，表示我们的Django安装成。

![](//img-blog.csdn.net/20180313171302136)



## 四. 修改Django网站源码

Django网站目录如下：

```python
|-- Yangxz
|   |-- __init__.py
|   |-- settings.py
|   |-- urls.py
|   `-- wsgi.py
|
`-- db.sqlite3
`-- manage.py
```
说明如下：
Yangxz: 项目的容器。
manage.py: Django管理主程序。一个实用的命令行工具，可让你以各种方式与该 Django 项目进行交互。
__init__.py: 空文件。告诉Python该目录是一个Python包。
settings.py: 主配置文件。该Django项目的设置/配置。
urls.py: URL路由文件。Django项目的URL声明，一份由Django驱动的网站"目录"。
wsgi.py: 网络通信接口。一个WSGI兼容的Web服务器的入口，以便运行你的项目。

第一步 创建视图views.py文件
在Yangxz目录下创建一个views.py文件，用于书写业务处理逻辑。
命令：touch views.py
vim views.py
```python
#-*- coding:utf-8 -*-
from django.http import HttpResponse
 
def hello(request):
    return HttpResponse("Hello CSDN, My name is Eastmount！")
```
自定义hello方法，参数request固定的，用于返回HttpResponse。

![](//img-blog.csdn.net/20180313172020723)

第二步 配置URL
修改urls.py文件内容，将访问的内容改为views.py文件中的hello函数。
命令：vim urls.py

```python
from django.conf.urls import url
from django.contrib import admin
from Yangxz.views import *
urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'yangxz/', hello)
]
```
代码如下所示：
![](//img-blog.csdn.net/20180313172447385)

**第三步 网站展示**
代码需要导入views文件内容，才能引用hello方法。views.py视图文件在编译后会产生对应的views.pyc文件。然后启动Django服务器，在浏览器中访问即可，如下图所示：

![](//img-blog.csdn.net/2018031317271168)
如果没有该资源也不会报错“无法访问”，而是提示无法找到该资源，如下所示。

![](//img-blog.csdn.net/2018031317280388)

最后希望这篇文章对您有所帮助，在配置Django时需要注意以下几个常见错误：
1.需要调用 "vi /etc/sysconfig/iptables" 增加服务端口号；
2.需要在阿里云管理安全组中增加8090端口，如果未配置会提示无法访问；
3.通过“python manage.py runserver 0.0.0.0:8090 &”开启服务端口，注意&符号；
4.配置py文件时，需要先导入再使用，如“from Yangxz.views import *”；
5.注意在Yangxz.settings.py文件中修改代码 "ALLOWED_HOSTS=['*']"，增加 *；
6.最后访问的时候注意资源及文件名是否正确即可。

希望文章对你有所帮，如果文章中存在不足或错误的地方，还请海涵~
(By:Eastmount 2018-03-13 下午6点http://blog.csdn.net/eastmount/)


