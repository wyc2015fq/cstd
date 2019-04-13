
# Django之“学习笔记”网站开发1 - OraYang的博客 - CSDN博客

2018年01月18日 20:09:58[OraYang](https://me.csdn.net/u010665216)阅读数：1211所属专栏：[python科学计算、游戏开发、后台开发](https://blog.csdn.net/column/details/18109.html)



# 引言
Django是一个Web框架–一套用于帮助开发交互式网站的工具。Django能够响应网页请求，还能让你轻松地读写数据库、管理用户等。
# 建立项目
## 项目说明
首先，我们需要列出开发的一些明确的目标，以突出开发的重点。我们指定的规范如下：
> 我们要编写一个名为”学习笔记“的web应用程序，让用户能够记录感兴趣的主题，并在学习每个主题的过程中添加日志条目。”学习笔记”的主页对这个网站进行描述，并邀请用户注册或登陆。用户登陆后，就可创建新主题、添加新条目以及阅读既有的条目
学习新的主题时，记录学到的知识可帮助跟踪和复习这些知识。优秀的应用程序让这个记录的过程简单易行。
## 建立虚拟环境并安装Django
虚拟环境是系统的一个位置，我们可以在其中安装包，并将其与其他python包隔离。
输入下面的命令，创建一个名为ll_env的虚拟环境：
```python
$ conda create -n ll_env python
```
```python
=
```
```python
3.6
```
关于conda create的使用方法：[传送门](https://conda.io/docs/commands/conda-create.html)
激活虚拟环境：
```python
$
```
```python
source
```
```python
activate ll_env
```
安装Django:
```python
$
```
```python
pip install
```
```python
Django
```
## 新建项目并创建数据库
这里我们新建一个项目：
```python
$
```
```python
django-admin.py startproject learning_log .
```
并创建数据库：
```python
python manage
```
```python
.py
```
```python
migrate
```
查看项目：
现在我们来运行服务看看实际的效果，运行如下命令：
```python
$
```
```python
python
```
```python
manage
```
```python
.py
```
```python
runserver
```
```python
[yourip]
```
```python
:8000
```
因为我这个项目是在远处的服务器搭建的，所以我设置远程访问，[yourip]:8000表示监听的ip及端口号,即你项目部署所在的ip。此时我们打开浏览器输入：
> [yourip]:8000,就会出现如下的界面：
![这里写图片描述](https://img-blog.csdn.net/20180118144533969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20180118144533969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如过不是这个界面，并且报错，或者打不开，请去learning_log文件夹下，找到settings.py文件，向文件中的ALLOWED_HOSTS = [ ]列表中添加如下信息：
[
](https://img-blog.csdn.net/20180118144533969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> ‘[yourip]’, ‘localhost’, ‘127.0.0.1’
[
](https://img-blog.csdn.net/20180118144533969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 创建应用程序
[
](https://img-blog.csdn.net/20180118144533969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)Django项目由一系列应用程序组成，它们协同工作，让项目成为一个整体。接下来我们先来创建一个应用程序。另起一个终端窗口（前面的runserver不关闭），切换到manage.py文件目录，执行如下命令：
[

](https://img-blog.csdn.net/20180118144533969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
$
```
```python
python manage.py startapp learning_logs
```
[
](https://img-blog.csdn.net/20180118144533969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)进入learning_logs文件夹，发现一共有几个文件：
![这里写图片描述](https://img-blog.csdn.net/20180118150819848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20180118150819848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接下来我们首先使用models.py文件来定义我们要在应用程序中管理的数据。
[
](https://img-blog.csdn.net/20180118150819848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 定义模型
[
](https://img-blog.csdn.net/20180118150819848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)我们来想想涉及的数据。每位用户都需要在学习笔记中创建很多主题。用户输入的每个条目都与特定主题相关联，这些条目将以文本的方式显示。我们还要存储每个条目的时间戳，以便能够告诉用户各个条目都是什么时候创建的。
在代码层面，model就是一个类，如下我们实现的用户将要存储的主题的模型类：
```python
from
```
```python
django.db
```
```python
import
```
```python
models
```
```python
# Create your models here.
```
```python
class
```
```python
Topic
```
```python
(models.Model)
```
```python
:
```
```python
"""A topic the user is learning about."""
```
```python
text = models.CharField(max_length=
```
```python
200
```
```python
)
    date_added = models.DateTimeField(auto_now_add=
```
```python
True
```
```python
)
```
```python
def
```
```python
__str__
```
```python
(self)
```
```python
:
```
```python
"""Return a string representation of the model."""
```
```python
return
```
```python
self.text
```
## 激活模型
前面我们startapp，创建了应用，但是还需要让Django将应用程序包含到项目中。这样才能够使用模型。这个我们需要去settings.py文件中设置。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20180118152938952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
接下来修改数据库，使得Django能够修改数据库，使其能够存储与模型Topic类相关的信息。因此我们“迁移数据库”：
```python
$
```
```python
python manage.py makemigrations learning_logs
```
```python
$
```
```python
python manage.py migrate
```
上述两个操作，分别完成了数据库的修改与更新。
## Django管理网站
为应用程序定义模型时，Django提供的管理网站（admin site）让你能够轻松地处理模型。网站的管理员能够使用管理网站，但普通用户不能使用。在本节中，我们将建立管理网站，并通过模型Topic类来添加一些主题。
首先创建超级用户
Django允许你创建具备所有权限的用户——超级用户。为了有效地管理Web应用程序，网站所有者通常需要访问网站存储的所有信息。
执行如下的命令创建超级用户：
```python
$
```
```python
python manage.py createsuperuser
```
命令执行时，需要你填写用户名、邮箱、密码
向管理网站注册模型
Django会自动在管理网站中添加一些模型，如User何Group,但对于我们创建的模型，必须手工注册。前面我们在创建应用程序learning_logs时，Django在models.py所在的目录中创建了一个名为admin.py的文件。
我们在文件中添加如下代码：
```python
# Register your models here.
```
```python
from
```
```python
learning_logs.models
```
```python
import
```
```python
Topic
admin.site.register(Topic)
```
接下来我们在浏览器地址中输入：
> [your ip]:8000/admin/

> 然后输入用户名和密码，最后你会得到如下的页面：
![这里写图片描述](https://img-blog.csdn.net/20180118155552566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20180118155552566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 添加主题

> 我们在网页上利用Topics旁的Add按钮来添加两个主题：machine learning及python game
![这里写图片描述](https://img-blog.csdn.net/20180118160456814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 定义模型Entry

> 接下来我们需要在models.py文件中添加一个Entry类来与特定主题相关联，代码如下：

> class
> Entry
> (models.Model)
> :
> """Something specific learned about a topic."""
> topic = models.ForeignKey(Topic,
> 'on_delete'
> )
    text = models.TextField()
    date_added = models.DateTimeField(auto_now_add=
> True
> )
> class
> Meta
> :
> verbose_name_plural =
> 'entries'
> def
> __str__
> (self)
> :
> """Return a string representation of the model."""
> return
> self.text[:
> 50
> ] +
> "..."
> 迁移、注册模型Entry

> 与模型Topic与一样，修改了models.py中的信息就需要迁移数据库，然后再admin.py中注册模型。输入以下的命令：

> 迁移数据库

> $
> python manage.py makemigrations learning_logs
> $
> python manage.py migrate
> 注册Entry

> from
> learning_logs.models
> import
> Topic, Entry
admin.site.register(Entry)
> 创建网页：学习笔记主页

> 使用Django创建网页的过程通常分三个阶段：定义URL、编写视图和编写模板。

> 映射URL

> 首先我们需要确定项目需要哪些URL。我们地址栏输入的：[yourip]:8000,返回的是默认的Django网站，接下来我们将修改这个基础的URL让其映射到“学习笔记”的主页。

> 我们首先修改learning_log文件夹下面的urls.py文件：

> from
> django.contrib
> import
> admin
> from
> django.urls
> import
> path,include
urlpatterns = [
    path(
> 'admin/'
> , admin.site.urls),
    path(
> ''
> ,include(
> 'learning_logs.urls'
> ),name=
> 'learning_logs'
> ),
]
> 接下来我们在learning_logs文件夹中创建另一个urls.py文件：

> from
> django.urls
> import
> path
> from
> .
> import
> views
urlpatterns = [
> \# Home page.
> path(
> ''
> , views.index, name=
> 'index'
> ),
]
> 编写视图

> 视图函数接收请求中的信息，准备好生成网页所需的数据，再将这些数据发送给浏览器。learning_logs中的文件views.py中可以用来创建视图。

> from
> django.shortcuts
> import
> render
> \# Create your views here.
> def
> index
> (request)
> :
> return
> render(request,
> 'learning_logs/index.html'
> )
> 编写模板

> 模板定义了网页的结构。模板指定了网页是什么样的，而每当网页被请求时，Django将填入相关的数据。我们在learning_logs文件夹中新建文件夹templates，同时在templates文件夹中新建文件夹learning_logs。在这个文件夹中我们新建html文件，并将其命名为index.html。

> 我们在index.html中写入如下信息：

> <
> p
> >
> Learning Log
> </
> p
> >
> <
> p
> >
> Learning Log helps you keep track of your learning, for any topic you're 
  learning about.
> </
> p
> >
> 创建其他网页

> 接下来，我们开始扩充”学习笔记“项目，我们将创建两个显示数据的网页，其中一个列出所有的主题，另一个显示特定主题的所有条目。对于每一个网页，我们都将指定其中的URL模式，编写一个视图函数，并编写一个模板。

> 模板继承

> 首先我们先来创建一个父模板，项目中的其他模板都将继承它。

> 显示所有主题的页面

> 接下来实现显示所有主题的网页以及显示特定主题的网页，使用的方法还是我们前面介绍的url设计、view函数填写、html模板的实现

> 显示所有主题的页面：
![这里写图片描述](https://img-blog.csdn.net/20180118194630186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20180118194630186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 显示特定主题的页面：
![这里写图片描述](https://img-blog.csdn.net/20180118195803396?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 总结

> 在这次文章中，首先介绍了如何使用Django来创建Web应用程序。我们制定了简要的项目规范，创建了虚拟环境，安装了Django，创建了一个项目，然后创建了应用程序，接着学习了数据库，创建了超级用户，并在管理网站中输入初始数据，我们最后讲述了如何定义URL、创建视图函数以及编写为网站创建网页的模板。在接下来的一篇博客中我们将实现网页与用户的交互以及用户注册系统。

> 项目源码

> github:
> [传送门](https://github.com/shawshany/Django-Web)


> [
](https://img-blog.csdn.net/20180118194630186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
