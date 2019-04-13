
# Django之“学习笔记”网站开发2 - OraYang的博客 - CSDN博客

2018年01月19日 11:21:02[OraYang](https://me.csdn.net/u010665216)阅读数：262所属专栏：[python科学计算、游戏开发、后台开发](https://blog.csdn.net/column/details/18109.html)



# 引言
前面我们讲述了[Django之“学习笔记”网站开发1](http://blog.csdn.net/u010665216/article/details/79095113)，接下来我们介绍如何实现账户系统。
Web应用程序的核心是让任何用户都能够注册账户并能够使用它，不管用户身处何方。在本次分享中，我们将创建一些表单让用户能够添加主题和条目，以及编辑既有的条目。同时我们还将学习Django如何防范对基于表单的网页发起的常见攻击。然后我们将创建一个用户身份验证系统，供用户创建账户，并让有些页面只能供已登陆的用户访问。接下来，我们将修改一些视图函数，使得用户只能看得到自己的数据。
# 让用户能够输入数据
我们希望用户可以使用以下三种功能：
> 添加新的topic

> 添加新条目

> 编辑既有条目
接下来讲述如何实现上述三个功能。
## 添加新主题
创建基于表单的页面的方法几乎与前面创建网页一样：定义一个URL，编写一个视图函数并编写一个模板。但是两者间存在一个主要差别：需要导入包含表单的模块。
![这里写图片描述](https://img-blog.csdn.net/20180118215819992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 添加新条目
这里我们再次定义URL,编写视图函数和模板，并链接到添加新条目的网页。我们和添加新主题一样，在forms.py文件中添加一个新的类用来创建一个与模型Entry相关联的表单。
![这里写图片描述](https://img-blog.csdn.net/20180118221637691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 编辑条目
这里我们不赘述了，直接给出效果：
![这里写图片描述](https://img-blog.csdn.net/20180118222157993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 创建用户账户
这一节，我们将建立一个用户注册和身份验证系统，让用户能够注册账户，进而登陆和注销。这里我们将创建一个新的应用程序，其中包含了处理用户账户相关的所有功能。同时我们对模型的topic进行修改，让每个主题都归属于特定的用户。
## 应用程序users
这里我们首先使用命令startapp来创建一个名为users的应用程序：
```python
$
```
```python
python manage.py startapp users
```
然后在settings.py文件中添加users应用程序
## 登录界面
我们首先来实现登陆页面的功能。这里我们使用Django提供的默认的登陆视图，因此这里的URL模式会稍有不同。users文件夹下新建一个名叫urls.py的文件。
![这里写图片描述](https://img-blog.csdn.net/20180119092709040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 注销
这里我们不创建用于注销的页面，而是让用户只需单击一个链接就能注销并返回到主页。为此，我们将为注销链接定义一个URL模式，编写一个视图函数，并在base.html中添加一个注销链接。
![这里写图片描述](https://img-blog.csdn.net/20180119093742135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 注册
下面来创建一个让新用户注册的页面。我们将使用Django提供的表单UserCreationForm,但编写自己的视图函数和模板。
![这里写图片描述](https://img-blog.csdn.net/20180119102832843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 让用户拥有自己的数据
用户应该能够输入其专有的数据，因此我们将创建一个系统，确定各项数据所属的用户，再限制对页面的访问，让用户只能使用自己的数据。接下来，我们将修改模型Topic,让每个主题都归于特定的用户。这也将影响条目，因为每个条目都属于特定的主题。我们先来限制对一些页面的访问。
Django提供了装饰器@login_required，让你能够轻松地实现这样的目标：对于某些页面，只允许已登陆的用户访问它们。装饰器（decorator）是放在函数定义前面的指令，python在函数运行前，根据它来修改函数代码的行为。
## 只允许用户访问自己的主题
这里只需要在views.py文件中对topics()函数中的request对象添加一个user属性。
# 总结
在本次分享中，我们介绍了如何使用表单来让用户添加新主题、添加新条目和编辑既有条目。然后我们又学习了如何实现用户账户，以及使用装饰器@login_required静止未登录的用户访问特定页面。最后我们还学习了如何修改视图函数使用户只看到属于他的数据。这次代码都是基于Django2.0的，大家如果在参考一些旧的解决方案，如果发生报错，可以参考本文的源码链接，或直接评论，所有demo都是跑过一遍，确认无错的。
# 源码
github：[传送门](https://github.com/shawshany/Django-Web2)

