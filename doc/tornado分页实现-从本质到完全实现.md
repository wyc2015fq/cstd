# tornado分页实现-从本质到完全实现

 																				2017年08月25日 22:12:26 					[狮子xxxl](https://me.csdn.net/iiiiher) 						阅读数：1622 						 																															

 									

​                   					 							 					                   					 					版权声明：亦余心之所善兮,虽九死其犹未悔.					https://blog.csdn.net/iiiiher/article/details/77587368				

[分页程序git代码](https://github.com/lannyMa/tornado_fengye)

## 初始化tornado目录

![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiw8ab0oj8j209008naaa)

## 构建项目–实现post提交,get展示数据

实现类似 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiw8ayy5xij20eh0gb3z8)

代码逻辑 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiw8fway2ej21c90tbws8)

完整代码

```
start.py

#!/usr/bin/env python
# coding=utf-8
import time
import tornado.ioloop
import tornado.web


# 业务逻辑处理模块


# 配置选项模块
from controllers import home


# 静态文件和模板文件的配置
settings = {
    'template_path': 'templates',
    'static_path': 'statics',
}

# 路由模块
## 动态路由系统
application = tornado.web.Application([
    (r"/index/(?P<page>\d+)/(?P<nid>\d+)", home.IndexHandler),
],
    **settings
)

## wsgi模块
if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
1234567891011121314151617181920212223242526272829303132333435
controllers/home.py

#!/usr/bin/env python
# coding=utf-8
import tornado.web

LIST_INFO = [
    {"username": "maotai", "email": "123456"},
]


class IndexHandler(tornado.web.RequestHandler):
    # def get(self, *args, **kwargs):
    def get(self,page,nid):
        # print(page,nid)
        self.render("home/index.html", list_info=LIST_INFO)

    def post(self, *args, **kwargs):
        username = self.get_argument("username", "")
        email = self.get_argument("email", "")

        tmp = {"username": username, "email": email}
        LIST_INFO.append(tmp)
        self.redirect("/index/1/1")
12345678910111213141516171819202122232425
home/index.html

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>index</title>
</head>
<body>
<h1>添加数据</h1>
<form action="/index/1/1" method="post">
    <input type="text" name="username">
    <input type="email" name="email">
    <input type="submit" typeof="提交">
</form>
<h1>显示数据</h1>
<table border="1">
    <thead>
        <tr>
            <th>用户名</th>
            <th>邮箱</th>
        </tr>
    </thead>
    <tbody>
        {% for line in list_info %}
            <tr>
                <td>{{ line['username'] }}</td>
                <td>{{ line['email'] }}</td>
            </tr>
        {% end %}
    </tbody>
</table>
</body>
</html>12345678910111213141516171819202122232425262728293031323334
```

实现url // 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiw8jbjubcj21gg0lw11o)

## 实现分页-每页显示5条数据

效果: 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiw95odbmog20ei0dzhdt)

代码逻辑: 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiw99azshwj21790v64cr)

完整代码:

```
#!/usr/bin/env python
# coding=utf-8
import time
import tornado.ioloop
import tornado.web


# 业务逻辑处理模块


# 配置选项模块
from controllers import home


# 静态文件和模板文件的配置
settings = {
    'template_path': 'templates',
    'static_path': 'statics',
}

# 路由模块
## 动态路由系统
application = tornado.web.Application([
    (r"/index/(?P<page>\d*)", home.IndexHandler),
],
    **settings
)

## wsgi模块
if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
123456789101112131415161718192021222324252627282930313233
#!/usr/bin/env python
# coding=utf-8
import tornado.web

LIST_INFO = [
    {"username": "maotai", "email": "123456"},
]


class IndexHandler(tornado.web.RequestHandler):
    # def get(self, *args, **kwargs):
    def get(self, page):
        # 每页显示5条数据
        # LIST_INFO[0:5]
        # LIST_INFO[5:10]
        page = int(page)
        print(page)
        start = (page - 1) * 5
        end = page * 5
        current_list = LIST_INFO[start:end]
        self.render("home/index.html", list_info=current_list)

    def post(self, *args, **kwargs):
        username = self.get_argument("username", "")
        email = self.get_argument("email", "")

        tmp = {"username": username, "email": email}
        LIST_INFO.append(tmp)
        self.redirect("/index/1")
123456789101112131415161718192021222324252627282930
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>index</title>
</head>
<body>
<h1>添加数据</h1>
<form action="/index/1" method="post">
    <input type="text" name="username">
    <input type="email" name="email">
    <input type="submit" typeof="提交">
</form>
<h1>显示数据</h1>
<table border="1">
    <thead>
        <tr>
            <th>用户名</th>
            <th>邮箱</th>
        </tr>
    </thead>
    <tbody>
        {% for line in list_info %}
            <tr>
                <td>{{ line['username'] }}</td>
                <td>{{ line['email'] }}</td>
            </tr>
        {% end %}
    </tbody>
</table>
</body>
</html>1234567891011121314151617181920212223242526272829303132
```

## 处理page异常

- page为空 如page=
- page转换int不成功, 如page=xfdsfd213
- page<=0情况 
   思路: 
   使用try except来操作

```
class IndexHandler(tornado.web.RequestHandler):
    # def get(self, *args, **kwargs):
    def get(self, page):
        # 每页显示5条数据
        # LIST_INFO[0:5]
        # LIST_INFO[5:10]

        # c        
        # if not page:
        #     page = 1
        try:
            page = int(page)
        except:
            page = 1
        if page <= 0:
            page = 1

        print(page)
        start = (page - 1) * 5
        end = page * 5
        current_list = LIST_INFO[start:end]
        self.render("home/index.html", list_info=current_list)

    def post(self, *args, **kwargs):
        username = self.get_argument("username", "")
        email = self.get_argument("email", "")

        tmp = {"username": username, "email": email}
        LIST_INFO.append(tmp)
        self.redirect("/index/1")
12345678910111213141516171819202122232425262728293031
```

## 实现提交数据后,不总是跳转到index/1,而是和get时候index/x一样

实现效果 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiwa6i5p28g20ei0dzhdt)

代码逻辑 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiw9zx7tl6j20wu12wwq4)

完整代码:

```
#!/usr/bin/env python
# coding=utf-8
import tornado.web

LIST_INFO = [
    {"username": "maotai", "email": "123456"},
]


class IndexHandler(tornado.web.RequestHandler):
    # def get(self, *args, **kwargs):
    def get(self, page):
        try:
            page = int(page)
        except:
            page = 1
        if page <= 0:
            page = 1

        print(page)
        start = (page - 1) * 5
        end = page * 5
        current_list = LIST_INFO[start:end]
        self.render("home/index.html", list_info=current_list,current_page=page)

    def post(self,page):
        username = self.get_argument("username", "")
        email = self.get_argument("email", "")

        tmp = {"username": username, "email": email}
        LIST_INFO.append(tmp)
        self.redirect("/index/"+page)
123456789101112131415161718192021222324252627282930313233
#!/usr/bin/env python
# coding=utf-8
import time
import tornado.ioloop
import tornado.web


# 业务逻辑处理模块


# 配置选项模块
from controllers import home


# 静态文件和模板文件的配置
settings = {
    'template_path': 'templates',
    'static_path': 'statics',
}

# 路由模块
## 动态路由系统
application = tornado.web.Application([
    (r"/index/(?P<page>\d*)", home.IndexHandler),
],
    **settings
)

## wsgi模块
if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()
123456789101112131415161718192021222324252627282930313233
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>index</title>
</head>
<body>
<h1>添加数据</h1>
<form action="/index/{{ current_page }}" method="post">
    <input type="text" name="username">
    <input type="email" name="email">
    <input type="submit" typeof="提交">
</form>
<h1>显示数据</h1>
<table border="1">
    <thead>
        <tr>
            <th>用户名</th>
            <th>邮箱</th>
        </tr>
    </thead>
    <tbody>
        {% for line in list_info %}
            <tr>
                <td>{{ line['username'] }}</td>
                <td>{{ line['email'] }}</td>
            </tr>
        {% end %}
    </tbody>
</table>
</body>
</html>1234567891011121314151617181920212223242526272829303132
```

## xss跨站

效果: 提交js代码.每刷新一次都会执行一次. 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiwanekl72g20na0ck0w2)

实现:让代码按照原格式执行,前面加raw

```
    <tbody>
        {% for line in list_info %}
            <tr>
                <!--<td>{{ line['username'] }}</td>-->
                <td>{% raw line['username'] %}</td>
                <td>{{ line['email'] }}</td>
            </tr>
        {% end %}
    </tbody>123456789
```

## 初步实现点分页链接

效果: 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiwb46ui5eg20dm0ckaaf)

核心代码

```
<table border="1">
    <thead>
        <tr>
            <th>用户名</th>
            <th>邮箱</th>
        </tr>
    </thead>
    <tbody>
        {% for line in list_info %}
            <tr>
                <!--<td>{{ line['username'] }}</td>-->
                <td>{% raw line['username'] %}</td>
                <td>{{ line['email'] }}</td>
            </tr>
        {% end %}
    </tbody>
</table>
<div class = "pager">
    <a href="/index/1">1</a>
    <a href="/index/2">2</a>
    <a href="/index/3">3</a>
</div>12345678910111213141516171819202122
```

## 从后端传值过来

实现效果 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiwbbb2mf5j211e16948w)

```
    def get(self, page):
        try:
            page = int(page)
        except:
            page = 1
        if page <= 0:
            page = 1


        start = (page - 1) * 5
        end = page * 5
        current_list = LIST_INFO[start:end]

        all_pager,c = divmod(len(LIST_INFO),5)
        if c>0:
            all_pager+=1

        str_page = """
        <a href="/index/1">1</a>
        <a href="/index/2">2</a>
        <a href="/index/3">3</a>
        <a href="/index/4">4</a>
        """
        self.render("home/index.html", list_info=current_list,current_page=page,str_page=str_page)123456789101112131415161718192021222324
```

## 实现分页链接根据后端数据长度自动生成

效果图: 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiwbjhne1mj20iz0o8go7)

核心代码:

```
LIST_INFO = [
    {"username": "maotai", "email": "123456"},
]
for line in range(300):
    tmp={"username": "maotai", "email": "123456"}
    LIST_INFO.append(tmp)
1234567
    def get(self, page):
        try:
            page = int(page)
        except:
            page = 1
        if page <= 0:
            page = 1


        start = (page - 1) * 5
        end = page * 5
        current_list = LIST_INFO[start:end]

        all_pager,c = divmod(len(LIST_INFO),5)
        if c>0:
            all_pager+=1

        # str_page = """
        # <a href="/index/1">1</a>
        # <a href="/index/2">2</a>
        # <a href="/index/3">3</a>
        # <a href="/index/4">4</a>
        # """
        list_page=[]
        for p in range(all_pager):
            tmp = '<a href="/index/%s">%s</a>'%(p+1,p+1)
            list_page.append(tmp)
        str_page="".join(list_page)
        self.render("home/index.html", list_info=current_list,current_page=page,str_page=str_page)
123456789101112131415161718192021222324252627282930
<div class = "pager">
    <!--{{str_page}}-->
    {% raw str_page %}
</div>1234
```

## 实现当前页分页链接高亮

![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiwbp9snoyj20oq0hmdhh)

```
    def get(self, page):
        try:
            page = int(page)
        except:
            page = 1
        if page <= 0:
            page = 1


        start = (page - 1) * 5
        end = page * 5
        current_list = LIST_INFO[start:end]

        all_pager,c = divmod(len(LIST_INFO),5)
        if c>0:
            all_pager+=1

        # str_page = """
        # <a href="/index/1">1</a>
        # <a href="/index/2">2</a>
        # <a href="/index/3">3</a>
        # <a href="/index/4">4</a>
        # """
        list_page=[]
        for p in range(all_pager):
            if p+1 == page:
                tmp = '<a class="active" href="/index/%s">%s</a>' % (p + 1, p + 1)
            else:
                tmp = '<a href="/index/%s">%s</a>'%(p+1,p+1)
            list_page.append(tmp)
        str_page="".join(list_page)
        self.render("home/index.html", list_info=current_list,current_page=page,str_page=str_page)
123456789101112131415161718192021222324252627282930313233
```

前端:

```
        .pager a.active{
            background-color: brown;
            color: white;
        }1234
```

完整的code:

![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiwbr19e1tj207c07cwen)

```
#!/usr/bin/env python
# coding=utf-8
import tornado.web

LIST_INFO = [
    {"username": "maotai", "email": "123456"},
]
for line in range(300):
    tmp={"username": "maotai", "email": "123456"}
    LIST_INFO.append(tmp)

class IndexHandler(tornado.web.RequestHandler):
    # def get(self, *args, **kwargs):
    def get(self, page):
        try:
            page = int(page)
        except:
            page = 1
        if page <= 0:
            page = 1


        start = (page - 1) * 5
        end = page * 5
        current_list = LIST_INFO[start:end]

        all_pager,c = divmod(len(LIST_INFO),5)
        if c>0:
            all_pager+=1

        # str_page = """
        # <a href="/index/1">1</a>
        # <a href="/index/2">2</a>
        # <a href="/index/3">3</a>
        # <a href="/index/4">4</a>
        # """
        list_page=[]
        for p in range(all_pager):
            if p+1 == page: #这里为什么p+1,因为遍历的是从0到all_pager
                tmp = '<a class="active" href="/index/%s">%s</a>' % (p + 1, p + 1)
            else:
                tmp = '<a href="/index/%s">%s</a>'%(p+1,p+1)
            list_page.append(tmp)
        str_page="".join(list_page)
        self.render("home/index.html", list_info=current_list,current_page=page,str_page=str_page)

    def post(self,page):
        username = self.get_argument("username", "")
        email = self.get_argument("email", "")

        tmp = {"username": username, "email": email}
        LIST_INFO.append(tmp)
        self.redirect("/index/"+page)
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>index</title>
    <style>
        .pager a{
            display: inline-block;
            padding:5px;
            margin: 3px;
            background-color: cadetblue;
        }
        .pager a.active{
            background-color: brown;
            color: white;
        }
    </style>
</head>
<body>
<h1>添加数据</h1>
<form action="/index/{{ current_page }}" method="post">
    <input type="text" name="username">
    <input type="email" name="email">
    <input type="submit" typeof="提交">
</form>
<h1>显示数据</h1>
<table border="1">
    <thead>
        <tr>
            <th>用户名</th>
            <th>邮箱</th>
        </tr>
    </thead>
    <tbody>
        {% for line in list_info %}
            <tr>
                <td>{{ line['username'] }}</td>
                <!--<td>{% raw line['username'] %}</td>-->
                <td>{{ line['email'] }}</td>
            </tr>
        {% end %}
    </tbody>
</table>
<div class = "pager">
    <!--{{str_page}}-->
    {% raw str_page %}
</div>
</body>
</html>12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849
```

## 实现显示11项,当前页+-5

![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fiwdb4lgvig20e40gqdwq)

分析:

```
当总页数<11:
    1 11
当总页数>11:
    当前页<6:
        1 11
    当前页>6:
        当前页+5>总页数
            总页数-11 总页数
        当前页+5<总页数:
            s=page-5
            t=page+51234567891011
```

核心实现代码:

```
class IndexHandler(tornado.web.RequestHandler):
    # def get(self, *args, **kwargs):
    def get(self, page):
        try:
            page = int(page)
        except:
            page = 1
        if page <= 0:
            page = 1

        start = (page - 1) * 5
        end = page * 5
        current_list = LIST_INFO[start:end]

        all_pager, c = divmod(len(LIST_INFO), 5)
        if c > 0:
            all_pager += 1

        # str_page = """
        # <a href="/index/1">1</a>
        # <a href="/index/2">2</a>
        # <a href="/index/3">3</a>
        # <a href="/index/4">4</a>
        # """
        list_page = []
        if all_pager < 11:
            s = 1
            t = 11
        else:
            if page <= 6:
                s = 1
                t = 11
            elif page > 6:
                if page + 5 > all_pager:
                    s = all_pager - 11
                    t = all_pager
                else:
                    s = page - 5
                    t = page + 5

        for p in range(s, t + 1):
            if p == page:  # 这里为什么p,而不是p+1? 因为range(s, t + 1),不是从0开始的.
                tmp = '<a class="active" href="/index/%s">%s</a>' % (p, p)
            else:
                tmp = '<a href="/index/%s">%s</a>' % (p, p)
            list_page.append(tmp)
        str_page = "".join(list_page)
        self.render("home/index.html", list_info=current_list, current_page=page, str_page=str_page)
12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849
```

## 封装分页class

思路 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fixepb1athj21bs1rwqnh) 
 代码:

```
#!/usr/bin/env python
# coding=utf-8
import tornado.web

LIST_INFO = [
    {"username": "maotai", "email": "123456"},
]
for line in range(300):
    tmp = {"username": "maotai", "email": "123456"}
    LIST_INFO.append(tmp)


class Pagination:
    def __init__(self, current_page, all_item):
        all_pager, c = divmod(len(all_item), 5)
        if c > 0:
            all_pager += 1
        try:
            current_page = int(current_page)
        except:
            current_page = 1
        if current_page <= 0:
            current_page = 1

        self.current_page = current_page
        self.all_pager = all_pager

    @property
    def start(self):
        return (self.current_page - 1) * 5

    @property
    def end(self):
        return self.current_page * 5

    def page_str(self, baseurl):
        list_page = []
        if self.all_pager < 11:
            s = 1
            t = 11
        else:
            if self.current_page <= 6:
                s = 1
                t = 11
            elif self.current_page > 6:
                if self.current_page + 5 > self.all_pager:
                    s = self.all_pager - 11
                    t = self.all_pager
                else:
                    s = self.current_page - 5
                    t = self.current_page + 5

        for p in range(s, t + 1):
            if p == self.current_page:
                tmp = '<a class="active" href="%s%s">%s</a>' % (baseurl, p, p)
            else:
                tmp = '<a href="%s%s">%s</a>' % (baseurl, p, p)
            list_page.append(tmp)
        return "".join(list_page)

    def current_list_info(self):
        current_list = LIST_INFO[self.start:self.end]
        return current_list


class IndexHandler(tornado.web.RequestHandler):
    # def get(self, *args, **kwargs):
    def get(self, page):
        ## 实例化分页类
        # 传入当前页 和 所有数据
        # 获得current_list 即每页5条数据列表
        page_obj = Pagination(page, LIST_INFO)
        current_list = page_obj.current_list_info()
        str_page = page_obj.page_str("/index/")
        self.render("home/index.html", list_info=current_list, current_page=page, str_page=str_page)

    def post(self, page):
        username = self.get_argument("username", "")
        email = self.get_argument("email", "")

        tmp = {"username": username, "email": email}
        LIST_INFO.append(tmp)
        self.redirect("/index/" + page)
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384
```

## 实现上一页 下一页 跳转到

效果: 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fixfxokdu0g20qk0gqn8i)

原理: 
 ![img](http://ww1.sinaimg.cn/large/9e792b8fgy1fixfzymb0hj20h50aqdgh) 
 利用js的方法href实现跳转.

完整的code:

```
#!/usr/bin/env python
# coding=utf-8
import tornado.web

LIST_INFO = [
    {"username": "maotai", "email": "123456"},
]
for line in range(300):
    tmp = {"username": "maotai", "email": "123456"}
    LIST_INFO.append(tmp)


class Pagination:
    def __init__(self, current_page, all_item):
        all_pager, c = divmod(len(all_item), 5)
        if c > 0:
            all_pager += 1
        try:
            current_page = int(current_page)
        except:
            current_page = 1
        if current_page <= 0:
            current_page = 1

        self.current_page = current_page
        self.all_pager = all_pager
        self.all_item=all_item

    @property
    def start(self):
        return (self.current_page - 1) * 5

    @property
    def end(self):
        return self.current_page * 5

    def current_list_info(self):
        current_list = LIST_INFO[self.start:self.end]
        return current_list

    def page_str(self, baseurl):
        list_page = []

        # 上一页  --上一页 页码 下一页 必须按照顺序添加到list
        if self.current_page > 1:
            last_page = '<a href="%s%s">上一页</a>' % (baseurl, self.current_page - 1)
        else:
            last_page = '<a href="%s%s">上一页</a>' % (baseurl, self.current_page)
        list_page.append(last_page)

        # 页码页
        if self.all_pager < 11:
            s = 1
            t = 11
        else:
            if self.current_page <= 6:
                s = 1
                t = 11
            elif self.current_page > 6:
                if self.current_page + 5 > self.all_pager:
                    s = self.all_pager - 11
                    t = self.all_pager
                else:
                    s = self.current_page - 5
                    t = self.current_page + 5

        for p in range(s, t + 1):

            if p == self.current_page:
                tmp = '<a class="active" href="%s%s">%s</a>' % (baseurl, p, p)
            else:
                tmp = '<a href="%s%s">%s</a>' % (baseurl, p, p)
            list_page.append(tmp)

        # if self.current_page>1:
        #     syyp = self.current_page - 1
        # else:
        #     syyp = self.current_page
        #
        # if self.current_page > len(self.all_item):
        #     xyyp = len(self.all_item)
        # else:
        #     xyyp = self.current_page
        #
        # syy = '<a href="%s%s">上一页</a>' % (baseurl, syyp)
        # xyy = '<a href="%s%s">下一页</a>' % (baseurl, xyyp)
        # list_page.insert(0,syy)
        # list_page.append(xyy)



        # 下一页
        if self.current_page< self.all_pager:
            next_page = '<a href="%s%s">下一页</a>' % (baseurl, self.current_page+1)
        else:
            next_page = '<a href="%s%s">下一页</a>' % (baseurl, self.current_page)
        list_page.append(next_page)

        # 跳转到
        ## 获取本次点击前框里输入的数据, 拼接 baseurl + 数字 就可以实现跳转
        jump = """<input type="text" /><a onclick="Jump('%s',this);">GO</a>"""%(baseurl,)
        script="""<script>
            function Jump(baseUrl,ths){
                var val = ths.previousElementSibling.value;
                if(val.trim().length>0){
                    location.href = baseUrl+val;
                }
            }
            </script>
        """
        list_page.append(jump)
        list_page.append(script)
        return "".join(list_page)


class IndexHandler(tornado.web.RequestHandler):
    # def get(self, *args, **kwargs):
    def get(self, page):
        page_obj = Pagination(page, LIST_INFO)
        current_list = page_obj.current_list_info()
        str_page = page_obj.page_str("/index/")
        self.render("home/index.html", list_info=current_list, current_page=page, str_page=str_page)

    def post(self, page):
        username = self.get_argument("username", "")
        email = self.get_argument("email", "")

        tmp = {"username": username, "email": email}
        LIST_INFO.append(tmp)
        self.redirect("/index/" + page)
```