# Python学习笔记：Day11 编写日志创建页 - 去向前方的博客 - CSDN博客





2018年07月06日 08:45:56[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：94








# 前言

最近在学习深度学习，已经跑出了几个模型，但Pyhton的基础不够扎实，因此，开始补习Python了，大家都推荐廖雪峰的课程，因此，开始了学习，但光学有没有用，还要和大家讨论一下，因此，写下这些帖子，廖雪峰的课程连接在这里：[廖雪峰](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000)

Python的相关介绍，以及它的历史故事和运行机制，可以参见这篇：[python介绍](https://blog.csdn.net/lvsehaiyang1993/article/details/80644237)

Python的安装可以参见这篇：[Python安装](https://blog.csdn.net/lvsehaiyang1993/article/details/80644321)

Python的运行模式以及输入输出可以参见这篇：[Python IO](https://blog.csdn.net/lvsehaiyang1993/article/details/80644827)

Python的基础概念介绍，可以参见这篇：[Python 基础](https://blog.csdn.net/lvsehaiyang1993/article/details/80647010)

Python字符串和编码的介绍，可以参见这篇：[Python字符串与编码](https://blog.csdn.net/lvsehaiyang1993/article/details/80648947)

Python基本数据结构：list和tuple介绍，可以参见这篇：[Python list和tuple](https://blog.csdn.net/lvsehaiyang1993/article/details/80649384)

Python控制语句介绍：ifelse，可以参见这篇：[Python 条件判断](https://blog.csdn.net/lvsehaiyang1993/article/details/80649552)

Python控制语句介绍：循环实现，可以参见这篇：[Python循环语句](https://blog.csdn.net/lvsehaiyang1993/article/details/80651808)

Python数据结构：dict和set介绍[Python数据结构dict和set](https://blog.csdn.net/lvsehaiyang1993/article/details/80652307)

Python函数相关：[Python函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80654173)

Python高阶特性：[Python高级特性](https://blog.csdn.net/lvsehaiyang1993/article/details/80661973)

Python高阶函数：[Python高阶函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80665892)

Python匿名函数：[Python匿名函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80667629)

Python装饰器：[Python装饰器](https://blog.csdn.net/lvsehaiyang1993/article/details/80667177)

Python偏函数：[Python偏函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80673522)

Python模块：[Python模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80673944)

Python面向对象编程（1）：[Python面向对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80674239)

Python面向对象编程（2）：[Python面向对象（2）](https://blog.csdn.net/lvsehaiyang1993/article/details/80675404)

Python面向对象编程（3）：[Python面向对象（3）](https://blog.csdn.net/lvsehaiyang1993/article/details/80675873)

Python面向对象编程（4）：[Pyhton面向对象（4）](https://blog.csdn.net/lvsehaiyang1993/article/details/80676298)

Python面向对象高级编程（上）：[Python面向对象高级编程（上）](https://blog.csdn.net/lvsehaiyang1993/article/details/80688026)

Python面向对象高级编程（中上）：[Python面向对象高级编程（中上）](https://blog.csdn.net/lvsehaiyang1993/article/details/80689908)

Python面向对象高级编程（中下）：[Python面向对象高级编程（中下）](https://blog.csdn.net/lvsehaiyang1993/article/details/80691702)

Python面向对象高级编程（完）：[Python面向对象高级编程（完）](https://blog.csdn.net/lvsehaiyang1993/article/details/80692002)

Python错误调试（起）：[Python调试：起](https://blog.csdn.net/lvsehaiyang1993/article/details/80694444)

Python错误调试（承）：[Python调试：承](https://blog.csdn.net/lvsehaiyang1993/article/details/80694819)

Python错误调试（转）：[Python调试：转](https://blog.csdn.net/lvsehaiyang1993/article/details/80695280)

Python错误调试（合）：[python调试：合](https://blog.csdn.net/lvsehaiyang1993/article/details/80697743)

Python文件IO编程：[Python文件IO](https://blog.csdn.net/lvsehaiyang1993/article/details/80700751)

Python文件IO编程2：[Python文件IO2](https://blog.csdn.net/lvsehaiyang1993/article/details/80707102)

Python文件IO编程3：[PYthon文件IO3](https://blog.csdn.net/lvsehaiyang1993/article/details/80707554)

Python进程和线程（起）：[Python进程和线程起](https://blog.csdn.net/lvsehaiyang1993/article/details/80709180)

Python进程和线程（承）：[Python进程和线程承](https://blog.csdn.net/lvsehaiyang1993/article/details/80709260)

Python进程和线程（转）：[Python进程和线程转](https://blog.csdn.net/lvsehaiyang1993/article/details/80709353)

Python进程和线程（合）：[Python进程和线程合](https://blog.csdn.net/lvsehaiyang1993/article/details/80709405)

Python正则表达式：[Python正则表达式](https://blog.csdn.net/lvsehaiyang1993/article/details/80711331)

Python学习笔记:常用内建模块1：[Python学习笔记：常用内建模块1](https://blog.csdn.net/lvsehaiyang1993/article/details/80712912)

Python学习笔记:常用内建模块2：[Python学习笔记：常用内建模块2](https://blog.csdn.net/lvsehaiyang1993/article/details/80713218)

Python学习笔记:常用内建模块3：[Python学习笔记：常用内建模块3](https://blog.csdn.net/lvsehaiyang1993/article/details/80713747)

Python学习笔记:常用内建模块4：[Python学习笔记:   常用内建模块4](https://blog.csdn.net/lvsehaiyang1993/article/details/80716665)

Python学习笔记:常用内建模块5：[Python学习笔记:   常用内建模块5](https://blog.csdn.net/lvsehaiyang1993/article/details/80716912)

Python学习笔记:常用内建模块6:[Python学习笔记：常用内建模块6](https://blog.csdn.net/lvsehaiyang1993/article/details/80717102)

Python学习笔记:第三方模块1：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726046)

Python学习笔记:第三方模块2：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726301)

Python学习笔记:第三方模块3：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726378)

Pytho学习笔记：网络编程：[Python网络编程](https://blog.csdn.net/lvsehaiyang1993/article/details/80729939)

Python学习笔记：电子邮件：[Python电子邮件1](https://blog.csdn.net/lvsehaiyang1993/article/details/80731086)

Python学习笔记：SMTP服务器：[PythonSMTP服务器](https://blog.csdn.net/lvsehaiyang1993/article/details/80731573)

Python学习笔记：POP3服务器：[PythonPOP3服务器](https://blog.csdn.net/lvsehaiyang1993/article/details/80731697)

Python学习笔记：Python数据库 [Python数据库1](https://blog.csdn.net/lvsehaiyang1993/article/details/80742388)

Python学习笔记：Python数据库2[Python数据库2](https://blog.csdn.net/lvsehaiyang1993/article/details/80747424)

Python学习笔记：web开发1[Python学习笔记：web开发1](https://blog.csdn.net/lvsehaiyang1993/article/details/80765955)

Python学习笔记：web开发2[Python学习笔记: web开发2](https://blog.csdn.net/lvsehaiyang1993/article/details/80778507)

Python学习笔记:  web开发3[Python学习笔记: web开发3](https://blog.csdn.net/lvsehaiyang1993/article/details/80779521)

Python学习笔记：异步IO（1）[Python学习笔记：异步IO（1）](https://blog.csdn.net/lvsehaiyang1993/article/details/80789370)

Python学习笔记：异步IO（2）[Python学习笔记：异步IO（2）](https://blog.csdn.net/lvsehaiyang1993/article/details/80789574)

Python学习笔记：异步IO（3）[Python学习笔记：异步IO（3）](https://blog.csdn.net/lvsehaiyang1993/article/details/80795526)

Python学习笔记：Day 1-2开发[Python学习笔记：Day1-2开发](https://blog.csdn.net/lvsehaiyang1993/article/details/80807565)

Python学习笔记：Day 3 ORM[Python学习笔记：Day3 ORM](https://blog.csdn.net/lvsehaiyang1993/article/details/80842307)

Python学习笔记：Day 4 Model[Python学习笔记：Day4Model](https://blog.csdn.net/lvsehaiyang1993/article/details/80845475)

Python学习笔记：Day 5 web框架[PYTHON学习笔记：DAy5](https://blog.csdn.net/lvsehaiyang1993/article/details/80866237)

Python学习笔记：Day 6 配置文件[Python学习笔记：Day 6 配置文件](https://blog.csdn.net/lvsehaiyang1993/article/details/80875144)

Python学习笔记：Day 7 编写MVC[Python学习笔记：Day 7编写MVC](https://blog.csdn.net/lvsehaiyang1993/article/details/80880125)

Python学习笔记：Day 8 构建前端[Python学习笔记:Day 8 构建前端](https://blog.csdn.net/lvsehaiyang1993/article/details/80899296)

Python学习笔记：Day 9 编写API[Python 9编写API](https://blog.csdn.net/lvsehaiyang1993/article/details/80914717)

Python学习笔记：Day 10 用户注册和登陆[Python 10用户注册和登陆](https://blog.csdn.net/lvsehaiyang1993/article/details/80921837)
# 目录



- [前言](#前言)
- [目录](#目录)
- [编写日志创建页](#编写日志创建页)




# 编写日志创建页

在Web开发中，后端代码写起来其实是相当容易的。

例如，我们编写一个REST API，用于创建一个Blog：

```python
@post('/api/blogs')
def api_create_blog(request, *, name, summary, content):
    check_admin(request)
    if not name or not name.strip():
        raise APIValueError('name', 'name cannot be empty.')
    if not summary or not summary.strip():
        raise APIValueError('summary', 'summary cannot be empty.')
    if not content or not content.strip():
        raise APIValueError('content', 'content cannot be empty.')
    blog = Blog(user_id=request.__user__.id, user_name=request.__user__.name, user_image=request.__user__.image, name=name.strip(), summary=summary.strip(), content=content.strip())
    yield from blog.save()
    return blog
```

编写后端Python代码不但很简单，而且非常容易测试，上面的API：api_create_blog()本身只是一个普通函数。

Web开发真正困难的地方在于编写前端页面。前端页面需要混合HTML、CSS和JavaScript，如果对这三者没有深入地掌握，编写的前端页面将很快难以维护。

更大的问题在于，前端页面通常是动态页面，也就是说，前端页面往往是由后端代码生成的。

生成前端页面最早的方式是拼接字符串：

```
s = '<html><head><title>'
    + title
    + '</title></head><body>'
    + body
    + '</body></html>'
```

显然这种方式完全不具备可维护性。所以有第二种模板方式：

```
<html>
<head>
    <title>{{ title }}</title>
</head>
<body>
    {{ body }}
</body>
</html>
```

ASP、JSP、PHP等都是用这种模板方式生成前端页面。

如果在页面上大量使用JavaScript（事实上大部分页面都会），模板方式仍然会导致JavaScript代码与后端代码绑得非常紧密，以至于难以维护。其根本原因在于负责显示的HTML DOM模型与负责数据和交互的JavaScript代码没有分割清楚。

要编写可维护的前端代码绝非易事。和后端结合的MVC模式已经无法满足复杂页面逻辑的需要了，所以，新的MVVM：Model View ViewModel模式应运而生。

MVVM最早由微软提出来，它借鉴了桌面应用程序的MVC思想，在前端页面中，把Model用纯JavaScript对象表示：

```xml
<script>
    var blog = {
        name: 'hello',
        summary: 'this is summary',
        content: 'this is content...'
    };
</script>

View是纯HTML：

<form action="/api/blogs" method="post">
    <input name="name">
    <input name="summary">
    <textarea name="content"></textarea>
    <button type="submit">OK</button>
</form>
```

由于Model表示数据，View负责显示，两者做到了最大限度的分离。

把Model和View关联起来的就是ViewModel。ViewModel负责把Model的数据同步到View显示出来，还负责把View的修改同步回Model。

ViewModel如何编写？需要用JavaScript编写一个通用的ViewModel，这样，就可以复用整个MVVM模型了。

好消息是已有许多成熟的MVVM框架，例如AngularJS，KnockoutJS等。我们选择Vue这个简单易用的MVVM框架来实现创建Blog的页面templates/manage_blog_edit.html：

```xml
{% extends '__base__.html' %}

{% block title %}编辑日志{% endblock %}

{% block beforehead %}

<script>
var
    ID = '{{ id }}',
    action = '{{ action }}';
function initVM(blog) {
    var vm = new Vue({
        el: '#vm',
        data: blog,
        methods: {
            submit: function (event) {
                event.preventDefault();
                var $form = $('#vm').find('form');
                $form.postJSON(action, this.$data, function (err, r) {
                    if (err) {
                        $form.showFormError(err);
                    }
                    else {
                        return location.assign('/api/blogs/' + r.id);
                    }
                });
            }
        }
    });
    $('#vm').show();
}
$(function () {
    if (ID) {
        getJSON('/api/blogs/' + ID, function (err, blog) {
            if (err) {
                return fatal(err);
            }
            $('#loading').hide();
            initVM(blog);
        });
    }
    else {
        $('#loading').hide();
        initVM({
            name: '',
            summary: '',
            content: ''
        });
    }
});
</script>

{% endblock %}

{% block content %}


    <div class="uk-width-1-1 uk-margin-bottom">
        <div class="uk-panel uk-panel-box">
            <ul class="uk-breadcrumb">
                <li><a href="/manage/comments">评论</a></li>
                <li><a href="/manage/blogs">日志</a></li>
                <li><a href="/manage/users">用户</a></li>
            </ul>
        </div>
    </div>

    <div id="error" class="uk-width-1-1">
    </div>

    <div id="loading" class="uk-width-1-1 uk-text-center">
        <span><i class="uk-icon-spinner uk-icon-medium uk-icon-spin"></i> 正在加载...</span>
    </div>

    <div id="vm" class="uk-width-2-3">
        <form v-on="submit: submit" class="uk-form uk-form-stacked">
            <div class="uk-alert uk-alert-danger uk-hidden"></div>
            <div class="uk-form-row">
                <label class="uk-form-label">标题:</label>
                <div class="uk-form-controls">
                    <input v-model="name" name="name" type="text" placeholder="标题" class="uk-width-1-1">
                </div>
            </div>
            <div class="uk-form-row">
                <label class="uk-form-label">摘要:</label>
                <div class="uk-form-controls">
                    <textarea v-model="summary" rows="4" name="summary" placeholder="摘要" class="uk-width-1-1" style="resize:none;"></textarea>
                </div>
            </div>
            <div class="uk-form-row">
                <label class="uk-form-label">内容:</label>
                <div class="uk-form-controls">
                    <textarea v-model="content" rows="16" name="content" placeholder="内容" class="uk-width-1-1" style="resize:none;"></textarea>
                </div>
            </div>
            <div class="uk-form-row">
                <button type="submit" class="uk-button uk-button-primary"><i class="uk-icon-save"></i> 保存</button>
                <a href="/manage/blogs" class="uk-button"><i class="uk-icon-times"></i> 取消</a>
            </div>
        </form>
    </div>

{% endblock %}
```

初始化Vue时，我们指定3个参数：

el：根据选择器查找绑定的View，这里是#vm，就是id为vm的DOM，对应的是一个
标签；



data：JavaScript对象表示的Model，我们初始化为{ name: ”, summary: ”, content: ”}；

methods：View可以触发的JavaScript函数，submit就是提交表单时触发的函数。

接下来，我们在标签中，用几个简单的v-model，就可以让Vue把Model和View关联起来：




Form表单通过把提交表单的事件关联到submit方法。

需要特别注意的是，在MVVM中，Model和View是双向绑定的。如果我们在Form中修改了文本框的值，可以在Model中立刻拿到新的值。试试在表单中输入文本，然后在Chrome浏览器中打开JavaScript控制台，可以通过vm.name访问单个属性，或者通过vm.$data访问整个Model：

![这里写图片描述Sva 

![这里写图片描述]([https://img-blog.csdn.net/20180706084304168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70](https://img-blog.csdn.net/20180706084304168?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

) 
![这里写图片描述](https://img-blog.csdn.net/20180706084335722?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)ript逻辑中修改了Model，这个修改会立刻反映到View上。试试在JavaScript控制台输入vm.name = ‘MVVM简介’，可以看到文本框的内容自动被同步了：
双向绑定是MVVM框架最大的作用。借助于MVVM，我们把复杂的显示逻辑交给框架完成。由于后端编写了独立的REST API，所以，前端用AJAX提交表单非常容易，前后端分离得非常彻底。






