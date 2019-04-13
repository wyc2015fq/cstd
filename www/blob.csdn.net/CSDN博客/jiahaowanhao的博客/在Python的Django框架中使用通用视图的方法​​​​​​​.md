
# 在Python的Django框架中使用通用视图的方法​​​​​​​ - jiahaowanhao的博客 - CSDN博客


2018年08月21日 07:42:20[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：52


**[在Python的Django框架中使用通用视图的方法](http://cda.pinggu.org/view/26432.html)**
这篇文章主要介绍了在Python的Django框架中使用通用视图的方法,同时提到了相关的安全问题，需要的朋友可以参考下
使用通用视图的方法是在URLconf文件中创建配置字典，然后把这些字典作为URLconf元组的第三个成员。
例如，下面是一个呈现静态“关于”页面的URLconf：
from django.conf.urls.defaults import *
from django.views.generic.simple import direct_to_template
urlpatterns = patterns('',
(r'^about/$', direct_to_template, {
'template': 'about.html'
})
)
一眼看上去似乎有点不可思议，不需要编写代码的视图！ 它和第八章中的例子完全一样：direct_to_template视图仅仅是直接从传递过来的额外参数获取信息并用于渲染视图。
因为通用视图都是标准的视图函数，我们可以在我们自己的视图中重用它。 例如，我们扩展 about例子，把映射的URL从 /about//修改到一个静态渲染 about/.html 。 我们首先修改URL配置以指向新的视图函数：
from django.conf.urls.defaults import *
from django.views.generic.simple import direct_to_template
**from mysite.books.views import about_pages**
urlpatterns = patterns('',
(r'^about/$', direct_to_template, {
'template': 'about.html'
}),
**(r'^about/(\w+)/$', about_pages),**
)
接下来，我们编写 about_pages 视图的代码：
from django.http import Http404
from django.template import TemplateDoesNotExist
from django.views.generic.simple import direct_to_template
def about_pages(request, page):
try:
return direct_to_template(request, template="about/%s.html" % page)
except TemplateDoesNotExist:
raise Http404()
在这里我们象使用其他函数一样使用 direct_to_template 。 因为它返回一个HttpResponse对象，我们只需要简单的返回它就好了。 这里唯一有点棘手的事情是要处理找不到模板的情况。 我们不希望一个不存在的模板导致一个服务端错误，所以我们捕获TemplateDoesNotExist异常并且返回404错误来作为替代。
这里有没有安全性问题？
眼尖的读者可能已经注意到一个可能的安全漏洞： 我们直接使用从客户端浏览器得到的数据构造模板名称(template="about/%s.html" % page )。乍看起来，这像是一个经典的 目录跨越（directory traversal） 攻击（详情请看第20章）。 事实真是这样吗？
完全不是。 是的，一个恶意的 page 值可以导致目录跨越，但是尽管 page 是 从请求的URL中获取的，但并不是所有的值都会被接受。 这就是URL配置的关键所在： 我们使用正则表达式 \w+ 来从URL里匹配 page ，而 \w 只接受字符和数字。 因此，任何恶意的字符 （例如在这里是点 . 和正斜线 / ）将在URL解析时被拒绝，根本不会传递给视图函数。

