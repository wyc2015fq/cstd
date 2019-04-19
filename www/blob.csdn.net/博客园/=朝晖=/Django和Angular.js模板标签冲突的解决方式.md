# Django和Angular.js模板标签冲突的解决方式 - =朝晖= - 博客园
# [Django和Angular.js模板标签冲突的解决方式](https://www.cnblogs.com/dhcn/p/7105533.html)
参考文章：[http://yanhua365.lofter.com/post/b417f_1f0361](http://yanhua365.lofter.com/post/b417f_1f0361)
[http://stackoverflow.com/questions/8302928/angularjs-with-django-conflicting-template-tags](http://stackoverflow.com/questions/8302928/angularjs-with-django-conflicting-template-tags)
[http://blog.boxelderweb.com/2012/11/16/providing-django-template-variables-as-constants-to-angularjs/](http://blog.boxelderweb.com/2012/11/16/providing-django-template-variables-as-constants-to-angularjs/)
　　说在前面的话，目前新版的Django 模板里面有个`verbatim`[¶](https://docs.djangoproject.com/en/1.11/ref/templates/builtins/#verbatim)标签，是做这个事情的正途。
       这个{{}}标签冲突的问题，在最新版的Django 1.5中提出了模板内建的解决方案 [verbatim](https://docs.djangoproject.com/en/1.5/ref/templates/builtins/#verbatim) 标签：[https://docs.djangoproject.com/en/1.5/ref/templates/builtins/#verbatim](https://docs.djangoproject.com/en/1.5/ref/templates/builtins/#verbatim) 这个标签内的内容可以避免被服务器端模板解析。使用1.4及之前版本的同学可以考虑用系统内建标签templatetag输出特殊字符串，或者用下面这个方案，就是少写几个字符。特殊定制了一下。至于客户端模板配置的解决方案还是看上面参考文章吧。
       主要参考第一篇的文章中间那个，用服务器端标签输出客户端所需的标签的解决方案，因为模板里到处是{{"}}"}}这样的文本容易让人糊涂，所以把输出封装成了Django
的包含标签解决，其中后双大括号的输出还用一个变量，否则输出成歧义。废话补多说了，上代码：
```
#coding:utf-8  
''''' 
Created on 2013-7-9 
 
@author: hulda 
'''  
from django import template  
  
register = template.Library()  
 
@register.inclusion_tag('dlb.djhtml')  
def dlb():  
    ''''' 
        double left brace:双左大括号，主要是为解决Django服务器端模板标签和AngularJS客户端模板标签冲突的问题 
    '''  
    return {}  
 
@register.inclusion_tag('drb.djhtml')  
def drb():  
    ''''' 
        double right brace:双右大括号，主要是为解决Django服务器端模板标签和AngularJS客户端模板标签冲突的问题 
    '''  
      
    return {"drb":"}}"}
```
  总结：坦率地讲这种服务器端解决方式，相比于客户端设置可能在性能上会略有缺憾，不过Web站点在性能上产生问题的时候，团队优化能力估计应该也不是问题了。

