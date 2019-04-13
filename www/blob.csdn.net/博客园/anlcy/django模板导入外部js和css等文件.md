
# django模板导入外部js和css等文件 - anlcy - 博客园






# [django模板导入外部js和css等文件](https://www.cnblogs.com/camilla/p/7459058.html)
**1.新建文件夹templates（存放模板文件），新建文件夹media（存放js、css、images文件夹），并把两个文件夹放到了项目的根目录下**
**2.设定模板路径**
设置模板路径比较简单，只要在setting.py里面的TEMPLATE_DIRS选项里面添加：
importos.path
TEMPLATE_DIRS =(
    os.path.join(os.path.dirname(__file__),'../templates').replace('\\','/'),   
)
这步的意思就是，在网站项目的根目录下，有一个templates文件夹，里面放置所有的模板
**3.设定CSS/JS/IMAGES等路径**
1）同样，在setting.py里面，指定根目录下面的media路径：
STATIC_PATH= os.path.join(os.path.dirname(__file__),'../media').replace('\\','/')
2）在urls.py里面设定以下语句(注意：假如在blog应用中新建了一个urls.py文件，那么应该添加到blog目录下的urls.py文件)：，将请求CSS/IMAGES/JS的URL转到该地方：
from django.confimportsettings
(r'^site_media/(?P<path>.*)$','django.views.static.serve',{'document_root':settings.STATIC_PATH}),如：
(r'^css/(?P<path>.*)$', 'django.views.static.serve',{'document_root': '/var/www/django-demo/css'}),
(r'^js/(?P</path><path>.*)$','django.views.static.serve', {'document_root': '/var/www/django-demo/js'}),
(r'^images/(?P</path><path>.*)$','django.views.static.serve', {'document_root': '/var/www/django-demo/images'}),

**4.最后，在模板中，已经可用以下方法访问CSS/IMAGES/JS等**
<link href="/site_media/style/style.css" rel="stylesheet" type="text/css" />
<img src=’/site_media/images/a.jpg’>
<script src=’/site_media/js/b.js’> </script>
## 遇到的问题
**问题1：**
django 1.10 urls.py配置static静态文件的链接
url(r'^static/(?P<path>.*)$', 'django.views.static.serve', {'document_root': settings.STATIC_ROOT}, name='static'),
报错：raise TypeError('view must be a callable or a list/tuple in the case of include().')
**解决方法：**
改成下面的 格式
from django.viewsimportstaticfrom 工程目录importsettings
url(r'^static/(?P<path>.*)$', static.serve, {'document_root':settings.STATIC_ROOT}, name='static'),

原因：django 1.10 url的格式变了，需要引入view对象，然后调用view对象中的方法，不能直接用字符串的形式了

**问题2：**
访问静态文件时，报下面的错误
django.core.exceptions.ImproperlyConfigured: The STATICFILES_DIRS setting should not contain the STATIC_ROOT setting
**解决方法：**
修改settings.py文件，将STATICFILES_DIRS中包含的STATIC_ROOT的路径删除掉
比如我的STATICFILES_DIRS原来包含了../webManage/static/，而我实际上是用这个地址作为我的静态资源文件地址
STATICFILES_DIRS =(\#动态的获取static文件的路径，注释掉STATIC_ROOT的路径\#os.path.join(os.path.dirname(__file__), '../webManage/static/').replace('\\', '/'),
    os.path.join(os.path.dirname(__file__),'../static/').replace('\\','/'),
    os.path.join(os.path.dirname(__file__),'../static/js/').replace('\\','/'),
)





