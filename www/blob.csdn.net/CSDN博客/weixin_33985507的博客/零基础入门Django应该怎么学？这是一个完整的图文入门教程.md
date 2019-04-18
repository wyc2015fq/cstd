# 零基础入门Django应该怎么学？这是一个完整的图文入门教程 - weixin_33985507的博客 - CSDN博客
2018年07月27日 09:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
**本文面向：有Python基础，刚接触web框架的Django初学者。**
**环境：windows7 python3.5.1 pycharm Django 1.10版 pip3**
一、Django简介
百度百科：一个开放源代码的Web框架，由Python语言编写......
重点：一个大而全的框架，啥都替你考虑好了。
**1. web框架介绍**
具体介绍Django之前，必须先介绍WEB框架的概念。
web框架： 别人已经设定好的一个web网站模板，你学习它的规则，然后“填空”或“修改”成你自己需要的样子。
一般web框架的架构是这样的：
![12684358-e66e0af19a712b66](https://upload-images.jianshu.io/upload_images/12684358-e66e0af19a712b66)
其它基于Python的web框架，如tornado、flask、webpy都是在这个范围内进行增删裁剪的。例如Tornado用的是自己的异步非阻塞“wsgi”，flask则只提供了最精简和基本的框架。Django直接使用WSGI，并实现了大部分Web应用相关的功能。
**2. MVC/MTV介绍**
**MVC**百度百科：全名Model View Controller，是模型(model)－视图(view)－控制器(controller)的缩写，一种软件工程典范，用业务逻辑、数据、界面显示分离的方法组织代码，将业务逻辑聚集到一个部件里面，在改进和个性化定制界面及用户交互的同时，不需要重新编写业务逻辑。
通俗解释：一种代码和文件的组织和管理形式！不要被缩写吓到了，这其实就是把不同类型的文件放到不同的目录下的一种方法，然后取了个高大上的名字。当然，它带来的好处有很多，比如前后端分离，松耦合等等，在使用中你慢慢体会就会逐渐明白它，具体就不详细说明了。
其中：
模型(model)：定义数据库相关的内容，一般放在models.py文件中。
视图(view)：定义HTML等静态网页文件相关，也就是那些HTML、css、js等前端的东西。
*控制器(controller)：定义业务逻辑相关，就是你的主要代码。
**MTV**: Django觉得MVC的字面意思很别扭，不太符合它的理念，就给它改了一下。view不再是HTML相关，而是主业务逻辑V了，相当于控制器。HTML被放在Templates中，称作模板T，于是MVC就变成了MTV。这其实就是一个文字游戏，和MVC本质上是一样的，换了个名字和叫法而已，换汤不换药。
**3.Django的MTV模型组织**
目录分开，就必须有机制将他们在内里进行耦合。在Django中，典型的业务流程如下图所示：
![12684358-6b0f830de8cfeeca](https://upload-images.jianshu.io/upload_images/12684358-6b0f830de8cfeeca)
二、Django项目实例
**1. 程序安装**
python3.5、pip3及pycharm请自行安装。
（1）安装Django：
这里只介绍较为简单的pip3命令安装方式。
win+r，调出cmd，运行命令：pip3 install django，自动安装Pypi提供的最新版本。
![12684358-80b7610943399784](https://upload-images.jianshu.io/upload_images/12684358-80b7610943399784)
安装完成后如下图所示：
![12684358-877c948980363e48](https://upload-images.jianshu.io/upload_images/12684358-877c948980363e48)
（2）配置系统环境 成功安装Django后，在下图中的路径可找到django-admin.exe文件，将它加入操作系统环境变量中。这样在以后的调用会比较方便。
![12684358-11c879004095e25f](https://upload-images.jianshu.io/upload_images/12684358-11c879004095e25f)
![12684358-1646d0a163f8114b](https://upload-images.jianshu.io/upload_images/12684358-1646d0a163f8114b)
![12684358-d69719bc3bf391c1](https://upload-images.jianshu.io/upload_images/12684358-d69719bc3bf391c1)
运行：django-admin help，能看到下面的内容表示安装过程OK。
![12684358-672c692800bc3079](https://upload-images.jianshu.io/upload_images/12684358-672c692800bc3079)
**2. 创建django项目**
在linux等命令行界面下，使用django提供的命令和vim也能进行项目开发。但是，这里推荐使用pycharm这个目前最好的Python开发IDE，它功能强大，界面友好。（下面所有的操作都在pycharm中进行。）
点击：file-->new project，出现下面的对话框。
![12684358-c0d6ba4a8ef84018](https://upload-images.jianshu.io/upload_images/12684358-c0d6ba4a8ef84018)
选择Django栏目，输入项目名称，这里采用国际惯例的mysite。选择Python解释器版本，点击create创建。
Django将自动生成下面的目录结构：
![12684358-f8daa555fdfcc97f](https://upload-images.jianshu.io/upload_images/12684358-f8daa555fdfcc97f)
与项目同名的目录中是配置文件，templates目录是HTML文件存放处，也就是MTV中的T。manage.py是django项目管理文件。
![12684358-dc4730e23c46422b](https://upload-images.jianshu.io/upload_images/12684358-dc4730e23c46422b)
**3. 创建APP**
在每个Django项目中可以包含多个APP，相当于一个大型项目中的分系统、子模块、功能部件等等，相互之间比较独立，但也可以有联系。
所有的APP共享项目资源。
在pycharm下方的terminal终端中输入命令：
python manage.py startapp cmdb
这样就创建了一个叫做cmdb的APP，django自动生成“cmdb”文件夹。
![12684358-94b4fc387c8865f4](https://upload-images.jianshu.io/upload_images/12684358-94b4fc387c8865f4)
**4. 编写路由**
路由都在urls文件里，它将浏览器输入的url映射到相应的业务处理逻辑。
简单的urls编写方法如下图：
![12684358-2a30cf3dc03821ed](https://upload-images.jianshu.io/upload_images/12684358-2a30cf3dc03821ed)
**5. 编写业务处理逻辑**
业务处理逻辑都在views.py文件里。
![12684358-0772067167cc5138](https://upload-images.jianshu.io/upload_images/12684358-0772067167cc5138)
通过上面两个步骤，我们将index这个url指向了views里的index（）函数，它接收用户请求，并返回一个“hello world”字符串。
**6. 运行web服务**
现在我们已经可以将web服务运行起来了。
命令行的方式是：python manage.py runserver 127.0.0.1:8000
但在pycharm中，你可以这么干：
在上部工具栏中找到下面图示的图标。
![12684358-1a416667935dc339](https://upload-images.jianshu.io/upload_images/12684358-1a416667935dc339)
点击下拉箭头
![12684358-36ab7b2363eb3c4f](https://upload-images.jianshu.io/upload_images/12684358-36ab7b2363eb3c4f)
点击edit configurations
![12684358-6061bb1e1836237a](https://upload-images.jianshu.io/upload_images/12684358-6061bb1e1836237a)
在host中填入：127.0.0.1 port中填入：8000
OK确定之后，点击绿色的三角，web服务就运行起来了。
![12684358-46ba61d0a7c8190d](https://upload-images.jianshu.io/upload_images/12684358-46ba61d0a7c8190d)
按图所示，自动跳转到浏览器程序界面。显示的却是下图的404页面：
![12684358-3b561bd4476913de](https://upload-images.jianshu.io/upload_images/12684358-3b561bd4476913de)
修改一下url，添加“/index”，就一切ok了！
![12684358-bd932aabe3727111](https://upload-images.jianshu.io/upload_images/12684358-bd932aabe3727111)
至此，一个最简单的django编写的web服务就启动成功了。
**7. 返回HTML文件**
上面我们返回给用户浏览器的是什么？一个字符串！实际上这肯定不行，通常我们都是将HTML文件返回给用户。
下面，我们写这么一个index.html文件：
![12684358-458125b69e25f87a](https://upload-images.jianshu.io/upload_images/12684358-458125b69e25f87a)
再修改一下views文件：
![12684358-4b2eb9b42c6552a4](https://upload-images.jianshu.io/upload_images/12684358-4b2eb9b42c6552a4)
为了让django知道我们的HTML文件在哪里，需要修改settings文件的相应内容。但默认情况下，它正好适用，你无需修改。
![12684358-154c61054fc8b82f](https://upload-images.jianshu.io/upload_images/12684358-154c61054fc8b82f)
接下来，我们可以重新启动web服务。在浏览器刷新一下，你会看到带有样式的“hello world”。
注：这里有个小技巧，在多次频繁重启服务时，由于端口未释放的原因，容易启动不了服务，修改一下端口就OK了。
**8. 使用静态文件**
我们已经可以将HTML文件返还给用户了，但是还不够，前端三大块，HTML、CSS、js还有各种插件，它们齐全才是一个完整
的页面。在django中，一般将静态文件放在static目录中。接下来，在mysite中新建个static目录。
![12684358-8eab73079d9c7060](https://upload-images.jianshu.io/upload_images/12684358-8eab73079d9c7060)
你的CSS,JS和各种插件都可以放置在这个目录里。
为了让django找到这个目录，依然需要对settings进行配置：
![12684358-95907fe557be3602](https://upload-images.jianshu.io/upload_images/12684358-95907fe557be3602)
同样，在index.html文件中，可以引入js文件了：
![12684358-4ab39f53c625e258](https://upload-images.jianshu.io/upload_images/12684358-4ab39f53c625e258)
重新启动web服务，刷新浏览器，查看结果。
**9. 接收用户发送的数据**
上面，我们将一个要素齐全的HTML文件返还给了用户浏览器。但这还不够，因为web服务器和用户之间没有动态交互。
下面我们设计一个表单，让用户输入用户名和密码，提交给index这个url，服务器将接收到这些数据。
先修改index.html文件
="en">
="UTF-8">
首页
# 用户输入：
="/index/" method**=**"post">
="text" name**=**"username" />
="password" name**=**"password" />
="submit" value**=**"提交" />
然后修改views.py文件
![12684358-01dcfbec958729b3](https://upload-images.jianshu.io/upload_images/12684358-01dcfbec958729b3)
此时 ，重启web服务时，会出错，因为django有一个跨站请求保护机制，我们在index.html文件中加入一行{% csrf_token %}。
="en">
="UTF-8">
首页
# 用户输入：
="/index/" method**=**"post">
{% csrf_token %} 
="text" name**=**"username" />
="password" name**=**"password" />
="submit" value**=**"提交" />
再次进入浏览器，刷新页面：
![12684358-0653e80f69ac83da](https://upload-images.jianshu.io/upload_images/12684358-0653e80f69ac83da)
输入点东西，然后我们在pycharm中可以看到相应的数据。
**10. 返回动态页面**
我们收到了用户的数据，但返回给用户的依然是个静态页面，通常我们会根据用户的数据，进行处理后在返回给用户。
这时候，django采用自己的模板语言，类似jinja2，根据提供的数据，替换掉HTML中的相应部分，详细语法入门后再深入学习。
先改造views.py文件：
![12684358-ea525809ee656949](https://upload-images.jianshu.io/upload_images/12684358-ea525809ee656949)
再改造index.HTML文件：
="en">
="UTF-8">
首页
# 用户输入：
="/index/" method**=**"post">
{% csrf_token %}
="text" name**=**"username" />
="password" name**=**"password" />
="submit" value**=**"提交" />
# 用户展示：
="1">
用户名
密码
{% for line in data %}
{{ line.user }}
{{ line.pwd }}
{% endfor %}
重启服务，刷新浏览器：
![12684358-c332c85b11141f76](https://upload-images.jianshu.io/upload_images/12684358-c332c85b11141f76)
可以看到，我们获得了用户实时输入的数据，并将它实时展示在了用户页面上，这是个不错的交互过程。
**11. 使用数据库**
流程走到这里，django的MTV框架基本已经浮出水面了，只剩下最后的数据库部分了。
上面我们虽然和用户交互得很好，但并没有保存任何数据，页面一旦关闭，或服务器重启，一切都将回到原始状态。
使用数据库是毫无疑问的，django通过自带的ORM框架操作数据库，并且自带轻量级的sqlite3数据库。下面我们来看一看：
首先是注册app：
![12684358-a9d87bb5e553faf0](https://upload-images.jianshu.io/upload_images/12684358-a9d87bb5e553faf0)
不注册它，你的数据库就不知道该给哪个app创建表。
然后我们在settings中，配置数据库相关的参数，如果使用自带的sqlite，不需要修改。
![12684358-db4ace2ac9d333cf](https://upload-images.jianshu.io/upload_images/12684358-db4ace2ac9d333cf)
再编辑models.py文件，也就是MTV中的M。
![12684358-6c01c11cb26a39e8](https://upload-images.jianshu.io/upload_images/12684358-6c01c11cb26a39e8)
这里我们创建了2个字段，分别保存用户的名字和密码。
接下来要在pycharm的teminal中通过命令创建数据库的表了。有2条命令，分别是：
Python manage.py makemigrations
![12684358-2d73295ddcd90f93](https://upload-images.jianshu.io/upload_images/12684358-2d73295ddcd90f93)
再输入命令：Python manage.py migrate
![12684358-e6822cf5df850388](https://upload-images.jianshu.io/upload_images/12684358-e6822cf5df850388)
修改views.py中的业务逻辑
![12684358-e079532d7e091d64](https://upload-images.jianshu.io/upload_images/12684358-e079532d7e091d64)
重启web服务后，刷新浏览器页面，之后和用户交互的数据都能保存到数据库中。任何时候都可以从数据库中读取数据，展示到页面上。
至此，一个要素齐全，主体框架展示清晰的Django项目完成了，其实很简单是不是？
三、 Django总结
作为Python必学web框架的Django，它的功能强大，内容全面，但同时也意味着限制颇多，灵活性低，可修改性差，这就是鱼和熊掌不可兼得了。我们学习Django，其实就是学习一个软件，要理解它的基本原理，把握它整体框架，牢记一些基本规则，剩下的就是不断深入细节，然后熟能生巧、经验多少的问题了，不存在多高深的不可掌握技术。
关于学习方法的建议：学习任何东西，不要直接扎入细节，应该先了解它的外围知识，看看它的整体架构，再学习它的基本内容，然后才是深入学习，打磨技巧！
全文完，有不对之处恳请指正，觉得不错，就点赞支持一下。
