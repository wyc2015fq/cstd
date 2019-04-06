# Tornado框架06-模板

 

# 01-静态文件

## static_path引用文件

配置静态文件的路径，告诉tornado从文件系统中的一个特定的位置提取静态文件 
 使用：

```
"static_path": os.path.join(BASE_DIRS, "static")1
```

作用： 
 引入其他文件

```
<!--<link rel="stylesheet" href="/static/css/home.css" />-->
<link rel="stylesheet" href="{{static_url('css/home.css')}}" />12
```

请求：

```
http://127.0.0.1:9808/static/html/index.html1
```

## StaticFileHandler

使用原因

```
http://127.0.0.1:9808/static/html/index.html这种请求方式不友好1
```

本质

```
tornado预置的用来提供静态资源文件的handler1
```

作用：自由映射静态文件

使用

```
(r'/(.*)$', tornado.web.StaticFileHandler,{"path":os.path.join(config.BASE_DIRS, "static/html")})
(r'/(.*)$', tornado.web.StaticFileHandler, {"path": os.path.join(config.BASE_DIRS, "static/html"),"default_filename":"index.html"})12
```

注意：最好在所有的路由下面使用，否则可能导致其他路由不可用

> 1.<http://127.0.0.1/static/html/index.html>
>
> 2.<http://127.0.0.1/>
>
> 3.<http://127.0.0.1/view/index.html>

# 02-模板

## 路径与渲染

使用模板，需要仿照静态文件路径设置一样，向web.Application类的构造函数传递一个名为template_path的参数来告诉Tornado从文件系统的一个特定位置提供模板文件，如：

```
app = tornado.web.Application(
    [(r'/', IndexHandler)],
    static_path=os.path.join(os.path.dirname(__file__), "statics"),
    template_path=os.path.join(os.path.dirname(__file__), "templates"),
)12345
```

在这里，我们设置了一个当前应用目录下名为templates的子目录作为template_path的参数。在handler中使用的模板将在此目录中寻找。 
 现在我们将静态文件目录statics/html中的index.html复制一份到templates目录中，此时文件目录结构为：

```
├── statics
│   ├── css
│   │   ├── index.css
│   │   ├── main.css
│   │   └── reset.css
│   ├── html
│   │   └── index.html
│   ├── images
│   │   ├── home01.jpg
│   │   ├── home02.jpg
│   │   ├── home03.jpg
│   │   └── landlord01.jpg
│   ├── js
│   │   ├── index.js
│   │   └── jquery.min.js
│   └── plugins
│       ├── bootstrap
│       │   └─...
│       └── font-awesome
│           └─...
├── templates
│   └── index.html
└── test.py1234567891011121314151617181920212223
```

在handler中使用render()方法来渲染模板并返回给客户端。

```
class IndexHandler(RequestHandler):
    def get(self):
        self.render("index.html") # 渲染主页模板，并返回给客户端。
1234
```

## 模板语法

## 变量与表达式

在tornado的模板中使用{{}}作为变量或表达式的占位符，使用render渲染后占位符{{}}会被替换为相应的结果值。 
 我们将index.html中的一条房源信息记录

```
class HomeHandler(RequestHandler):
    def get(self, *args, **kwargs):
        temp = 18
        self.render('home.html', age=temp)


class HomeHandler(RequestHandler):
    def get(self, *args, **kwargs):
        temp = 18
        per = {
            "name":"sun",
            "height":175
        }
        stu = {
            "id":123456,
            "weight":50
        }
        self.render('home.html', age=temp, per=per, **stu)      123456789101112131415161718
```

改为模板：

```
<h1>年龄：{{age}}</h1> 
<h1>年龄：{{age + 10}}</h1>

<h1>{{per["name"]}}</h1>
<h1>{{per["height"]}}</h1>

<h1>{{id}}</h1>
<h1>{{weight}}</h1>12345678
```

{{}}不仅可以包含变量，还可以是表达式，如：

## 控制语句

可以在Tornado模板中使用Python条件和循环语句。控制语句以{\%和\%}包围，并以类似下面的形式被使用：

```
{% if page is None %}
或
{% if len(entries) == 3 %}123
```

控制语句的大部分就像对应的Python语句一样工作，支持if、for、while，注意end:

```
{% if ... %} ... {% elif ... %} ... {% else ... %} ... {% end %}
{% for ... in ... %} ... {% end %}
{% while ... %} ... {% end %}123
```

## 函数

> static_url() 
>    Tornado模板模块提供了一个叫作static_url的函数来生成静态文件目录下文件的URL。读取配置中的static_path值，并将参数拼接到该路径下返回一个新路径如下面的示例代码：

```
<link rel="stylesheet" href="{{ static_url("style.css") }}">1
```

这个对static_url的调用生成了URL的值，并渲染输出类似下面的代码：

```
<link rel="stylesheet" href="/static/style.css?v=ab12">1
```

优点： 
 \-  static_url函数创建了一个基于文件内容的hash值，并将其添加到URL末尾（查询字符串的参数v）。这个hash值确保浏览器总是加载一个文件的最新版而不是之前的缓存版本。无论是在你应用的开发阶段，还是在部署到生产环境使用时，都非常有用，因为你的用户不必再为了看到你的静态内容而清除浏览器缓存了。  
  \- 另一个好处是你可以改变你应用URL的结构，而不需要改变模板中的代码。例如，可以通过设置static_url_prefix来更改Tornado的默认静态路径前缀/static。如果使用static_url而不是硬编码的话，代码不需要改变。

## 转义

写入的js程序并没有运行，而是显示出来了： 
 我们查看页面源代码，发现<、>、”等被转换为对应的html字符 
 这是因为tornado中默认开启了模板自动转义功能，防止网站受到恶意攻击。

```
class Transferred(RequestHandler):
    def get(self, *args, **kwargs):
        str = "<h1>sun is a good man</h1>"
        # str = "<script>alert('sunck good')</script>"
        self.render('transferred.html', str=str)12345
```

模板：将str当成一个普通字符串来显现

```
<body>
    {{str}}
</body>123
```

我们可以通过raw语句来输出不被转义的原始格式，如：

```
关闭当前标签中的自动转义
{% raw text %}12
```

注意：在Firefox浏览器中会直接弹出alert窗口，而在Chrome浏览器中，需要set_header(“X-XSS-Protection”, 0) 
 若要关闭自动转义，一种方法是在Application构造函数中传递

> autoescape=None，

另一种方法是在每页模板中修改自动转义行为，添加如下语句：

```
关闭当前页面的自动转义
{% autoescape None %}12
```

> escape() 
>    已经关闭了自动转义，但是在某个位置需要开启自动转义，如：

```
{{ escape(text) }}1
```

## 自定义函数

在模板中还可以使用一个自己编写的函数，只需要将函数名作为模板的参数传递即可，就像其他变量一样。 
 我们修改后端如下： 
 index.py

```
def mySum(a, b):
    return a + b
self.render('home.html', age=temp, per=per, flag=False, stus=stus, mySum=mySum, **stu)123
```

模板

```
<h1>使用自定义函数:{{mySum(5,8)}}</h1>1
```

## 块

我们可以使用块来复用模板，块语法如下： 
 父模板

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>{{title}}</title>
</head>
<body>
    {% block main %}

    {% end %}
</body>
</html>123456789101112
```

而子模板index.html使用extends来使用父模板base.html，如下：

```
{% extends "base.html" %}

{% block main %}
<h1>sunck is a handsome man</h1>
{% end %}
```