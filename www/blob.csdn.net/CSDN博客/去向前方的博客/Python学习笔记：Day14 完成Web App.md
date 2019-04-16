# Python学习笔记：Day14 完成Web App - 去向前方的博客 - CSDN博客





2018年07月09日 16:06:44[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：122标签：[Python																[webapp																[组件](https://so.csdn.net/so/search/s.do?q=组件&t=blog)




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

Python学习笔记：Day11 编写日志创建页[day11 编写目录](https://blog.csdn.net/lvsehaiyang1993/article/details/80935496)

Python学习笔记：Day12 编写日志列表页[day12 编写日志](https://blog.csdn.net/lvsehaiyang1993/article/details/80949143)

Python学习笔记：Day13 提升开发效率[这里写链接内容](https://blog.csdn.net/lvsehaiyang1993/article/details/80956748)
# 目录



- [前言](#前言)
- [目录](#目录)
- [完成webapp](#完成webapp)




# 完成webapp

在Web App框架和基本流程跑通后，剩下的工作全部是体力活了：在Debug开发模式下完成后端所有API、前端所有页面。我们需要做的事情包括：

把当前用户绑定到request上，并对URL/manage/进行拦截，检查当前用户是否是管理员身份：

```python
@asyncio.coroutine
def auth_factory(app, handler):
    @asyncio.coroutine
    def auth(request):
        logging.info('check user: %s %s' % (request.method, request.path))
        request.__user__ = None
        cookie_str = request.cookies.get(COOKIE_NAME)
        if cookie_str:
            user = yield from cookie2user(cookie_str)
            if user:
                logging.info('set current user: %s' % user.email)
                request.__user__ = user
        if request.path.startswith('/manage/') and (request.__user__ is None or not request.__user__.admin):
            return web.HTTPFound('/signin')
        return (yield from handler(request))
    return auth
```

后端API包括：

•获取日志：GET /api/blogs

•创建日志：POST /api/blogs

•修改日志：POST /api/blogs/:blog_id

•删除日志：POST /api/blogs/:blog_id/delete

•获取评论：GET /api/comments

•创建评论：POST /api/blogs/:blog_id/comments

•删除评论：POST /api/comments/:comment_id/delete

•创建新用户：POST /api/users

•获取用户：GET /api/users

管理页面包括：

•评论列表页：GET /manage/comments

•日志列表页：GET /manage/blogs

•创建日志页：GET /manage/blogs/create

•修改日志页：GET /manage/blogs/

•用户列表页：GET /manage/users

用户浏览页面包括：

•注册页：GET /register

•登录页：GET /signin

•注销页：GET /signout

•首页：GET /

•日志详情页：GET /blog/:blog_id

把所有的功能实现，我们第一个Web App就宣告完成！](https://so.csdn.net/so/search/s.do?q=webapp&t=blog)](https://so.csdn.net/so/search/s.do?q=Python&t=blog)




