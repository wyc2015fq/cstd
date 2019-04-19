# Django开发环境静态文件访问配置 - =朝晖= - 博客园
# [Django开发环境静态文件访问配置](https://www.cnblogs.com/dhcn/p/7116101.html)
settings主要配置项目：
```
STATIC_ROOT = 'D:\Dev\jpDev\czxg\assets' #这个地方是所在系统标准目录文法配置  
STATIC_URL = '/static/' 这个是一个URL前缀，主要是和URLConf里面的相对应  
STATICFILES_DIRS = (  
    # Put strings here, like "/home/html/static" or "C:/www/django/static".  
    # Always use forward slashes, even on Windows.  
    # Don't forget to use absolute paths, not relative paths.  
    "D:/Dev/assets",#注意这个地方虽然是目录，但是所有的斜杠必须是前向，所以Windows下它和STATIC_ROOT 的内容略有区别  
)
```
URLConf里面加的那一项是
```
(r'^static/(?P<path>.*)$','django.views.static.serve',{'document_root':settings.STATIC_ROOT}),
```
想在这类静态目录都是根据__file__的目录动态设置

