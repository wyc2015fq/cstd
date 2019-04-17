# 如果问你 WSGI 是什么？0岁Web后端开发必看 - zhusongziye的博客 - CSDN博客





2018年11月07日 21:17:28[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：39标签：[如果问你 WSGI 是什么？0岁Web后端开发必看](https://so.csdn.net/so/search/s.do?q=如果问你 WSGI 是什么？0岁Web后端开发必看&t=blog)
个人分类：[Web开发](https://blog.csdn.net/zhusongziye/article/category/7131153)









写了几年的 Python Web 程序，说不出 WSGI 是什么？ 说不出 Web 程序是如何跑起来的？ 说不出一个Web核心框架应该包含哪些东西？ 这估计是工资涨不上去的原因之一。读完这篇文章，帮你了解 WSGI 是什么。

### 背景

Python Web 开发中，服务端程序可以分为两个部分，一是服务器程序，二是应用程序。前者负责把客户端请求接收，整理，后者负责具体的逻辑处理。为了方便应用程序的开发，我们把常用的功能封装起来，成为各种Web开发框架，例如 Django, Flask, Tornado。不同的框架有不同的开发方式，但是无论如何，开发出的应用程序都要和服务器程序配合，才能为用户提供服务。这样，服务器程序就需要为不同的框架提供不同的支持。这样混乱的局面无论对于服务器还是框架，都是不好的。对服务器来说，需要支持各种不同框架，对框架来说，只有支持它的服务器才能被开发出的应用使用。

这时候，标准化就变得尤为重要。我们可以设立一个标准，只要服务器程序支持这个标准，框架也支持这个标准，那么他们就可以配合使用。一旦标准确定，双方各自实现。这样，服务器可以支持更多支持标准的框架，框架也可以使用更多支持标准的服务器。

![](https://img-blog.csdnimg.cn/20181107211625334.png)

（帮助理解，画了个图）



Python Web开发中，这个标准就是 The Web Server Gateway Interface, 即 WSGI. 这个标准在PEP 333中描述，后来，为了支持 Python 3.x, 并且修正一些问题，新的版本在PEP 3333中描述。

### WSGI 是什么

WSGI 是服务器程序与应用程序的一个约定，它规定了双方各自需要实现什么接口，提供什么功能，以便二者能够配合使用。

WSGI 不能规定的太复杂，否则对已有的服务器来说，实现起来会困难，不利于WSGI的普及。同时WSGI也不能规定的太多，例如cookie处理就没有在WSGI中规定，这是为了给框架最大的灵活性。要知道WSGI最终的目的是为了方便服务器与应用程序配合使用，而不是成为一个Web框架的标准。

另一方面，WSGI需要使得middleware（是中间件么？）易于实现。middleware处于服务器程序与应用程序之间，对服务器程序来说，它相当于应用程序，对应用程序来说，它相当于服务器程序。这样，对用户请求的处理，可以变成多个 middleware 叠加在一起，每个middleware实现不同的功能。请求从服务器来的时候，依次通过middleware，响应从应用程序返回的时候，反向通过层层middleware。我们可以方便地添加，替换middleware，以便对用户请求作出不同的处理。

WSGI主要是对应用程序与服务器端的一些规定，所以，它的主要内容就分为两个部分。

### 应用程序

WSGI规定：
`1. 应用程序需要是一个可调用的对象`
在Python中:
- 
可以是函数

- 
可以是一个实例，它的类实现了**call**方法

- 
可以是一个类，这时候，用这个类生成实例的过程就相当于调用这个类




同时，WSGI规定：
`2. 可调用对象接收两个参数`
这样，如果这个对象是函数的话，它看起来要是这个样子：

```
# callable function
def application(environ, start_response):
    pass
```

如果这个对象是一个类的话，它看起来是这个样子：

```
# callable class
class Application:
    def __init__(self, environ, start_response):
        pass
```

如果这个对象是一个类的实例，那么，这个类看起来是这个样子：

```
# callable object
class ApplicationObj:
    def __call__(self, environ, start_response):
        pass
```

最后，WSGI还规定:
`3.可调用对象要返回一个值，这个值是可迭代的。`
这样的话，前面的三个例子就变成：

```
HELLO_WORLD = b"Hello world!\n"


# callable function
def application(environ, start_response):
    return [HELLO_WORLD]


# callable class
class Application:
    def __init__(self, environ, start_response):
        pass

    def __iter__(self):
        yield HELLO_WORLD


# callable object
class ApplicationObj:
    def __call__(self, environ, start_response):
        return [HELLO_WORLD]
```

你可能会说，不是啊，我们平时写的web程序不是这样啊。 比如如果使用web.py框架的话，一个典型的应用可能是这样的:

```
class hello:
    def GET(self):
        return 'Hello, world!'
```

这是由于框架已经把WSGI中规定的一些东西封装起来了，我们平时用框架时，看不到这些东西，只需要直接实现我们的逻辑，再返回一个值就好了。其它的东西框架帮我们做好了。这也是框架的价值所在，把常用的东西封装起来，让使用者只需要关注最重要的东西。

当然，WSGI关于应用程序的规定不只这些，但是现在，我们只需要知道这些就足够了。下面，再介绍服务器程序。

### 服务器程序

服务器程序会在每次客户端的请求传来时，调用我们写好的应用程序，并将处理好的结果返回给客户端。

WSGI规定：
`4.服务器程序需要调用应用程序`
服务器程序看起来大概是这个样子的：

```
def run(application):
    environ = {}

    def start_response(status, response_headers, exc_info=None):
        pass

    result = application(environ, start_response)

    def write(data):
        pass

    for data in result:
        write(data)
```

这里可以看出服务器程序是如何与应用程序配合完成用户请求的。

WSGI规定了应用程序需要一个可调用对象，有两个参数，返回一个可迭代对象。在服务器 程序中，针对这几个规定，做了以下几件事：
- 
把应用程序需要的两个参数设置好

- 
调用应用程序

- 
迭代访问应用程序的返回结果，并将其传回客户端


你可以从中发现，应用程序需要的两个参数，一个是一个dict对象，一个是函数。它们到底有什么用呢？这都不是我们现在应该关心的，现在只需要知道，服务器程序大概做了什么事情就好了，后面，我们会深入讨论这些细节。

### middleware

另外，有些功能可能介于服务器程序和应用程序之间，例如，服务器拿到了客户端请求的URL, 不同的URL需要交由不同的函数处理，这个功能叫做 URL Routing，这个功能就可以放在二者中间实现，这个中间层就是 middleware。

middleware对服务器程序和应用是透明的，也就是说，服务器程序以为它就是应用程序，而应用程序以为它就是服务器。这就告诉我们，middleware需要把自己伪装成一个服务器，接受应用程序，调用它，同时middleware还需要把自己伪装成一个应用程序，传给服务器程序。

其实无论是服务器程序，middleware 还是应用程序，都在服务端，为客户端提供服务，之所以把他们抽象成不同层，就是为了控制复杂度，使得每一次都不太复杂，各司其职。

![](https://img-blog.csdnimg.cn/20181107211654395.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

下面，我们看看middleware大概是什么样子的。

```
# URL Routing middleware
def urlrouting(url_app_mapping):
    def midware_app(environ, start_response):
        url = environ['PATH_INFO']
        app = url_app_mapping[url]

        result = app(environ, start_response)

        return result

    return midware_app
```

函数 midware_app就是一个简单的middleware：对服务器而言，它是一个应用程序，是一个可调用对象， 有两个参数，返回一个可调用对象。对应用程序而言，它是一个服务器，为应用程序提供了参数，并且调用了应用程序。

另外，这里的urlrouting函数，相当于一个函数生成器，你给它不同的 url-app 映射关系，它会生成相应的具有 url routing功能的 middleware。

如果你仅仅想简单了解一下WSGI是什么，相信到这里，你差不多明白了，下面会介绍WSGI的细节，这些细节来自 PEP3333。

Python之禅注：

其实在 Python 中，已经内置了一个实现 WSGI 的服务器程序

```
from wsgiref.simple_server import make_server

def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    return [b'Hello, World']

if __name__ == '__main__':
    server = make_server('', 8888, application)
    print("server running on port 8888")
    server.serve_forever()
```

这个Web程序包含了两部分，application是应用程序部分，make_server会创建一个服务器程序，服务器程序启动时，监听8888端口，浏览器访问 localhost:8888 时，会显示Hello, World。 这里的 application可以用你的 Django 程序来代替。



作者：minixalpha

原文：https://blog.csdn.net/on_1y/article/details/18803563



