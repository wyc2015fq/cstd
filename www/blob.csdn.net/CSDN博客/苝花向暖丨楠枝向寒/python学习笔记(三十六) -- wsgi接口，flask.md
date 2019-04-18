# python学习笔记(三十六) -- wsgi接口，flask - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月15日 23:45:03[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：238
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[前言](#%E5%89%8D%E8%A8%80)

[WSGI](#WSGI)

[Web框架 -- flask](#Web%E6%A1%86%E6%9E%B6%20--%20flask)

# 前言

客户端也就是浏览器，输入地址如 www.baidu.com ，然后dns会找到这个域名对应的ip地址，从而向该ip地址的服务器发送请求，服务器接收到请求，根据请求的内容，将页面信息再返回给浏览器，浏览器再解析内容最终暂时给用户。

如果我们想独自完成整个流程的处理是非常麻烦的，比如 当你输入一个 域名，dns是如何解析的，解析完又是如何把请求数据封装的，封装完又是如何在网络间进行传输的，传输完服务器那边又是如何接收的，接收完又是怎么解析的，解析完我们还需要根据不同的url进行不同的处理，再查询数据库，最终再返回给前端，前端再把信息以一定得样式显示给用户，所以说为了让后端开发程序员把精力更多放在业务相关的逻辑上面，出现了服务器，以及web框架。

# WSGI

服务器 就是 为程序员 解决了 接收浏览器请求、解析请求 以及将 服务端处理完的信息返回的结果 再发送给客户端，当然这里面涉及的问题非常多，例如高并发问题。wsgi 接口就是一个python内置的服务器。

下面我们使用WSGI来接收一个客户端发送过来的请求，然后再返回一个消息给客户端

hello.py

```python
# encoding=utf8

def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    print(environ)
    body = '<h1>Hello,%s!</h1>' % (environ['PATH_INFO'][1:] or 'web')
    # 返回的数据必须是byte类型
    return [body.encode('utf-8')]
```

参数1:  wsgi将浏览器发送过来的所有请求信息放在了environ变量中

参数2:  start_response函数，通过这个函数可以将返回信息发送给浏览器，这个函数接收两个参数。

第一个参数是General中的status code状态码

![](https://img-blog.csdn.net/2018091517281014?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

第二个参数是一个list，这个list中的每一个元素是一个tuple，每一个tuple又包含两个 str，每一个tuple代表response header中的一行信息。

![](https://img-blog.csdn.net/20180915172547472?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

server.py

通过上面的解释，我们能够猜测到这个application函数肯定不是我们自己去调用的，因为它的参数包含了请求信息，而请求信息是浏览器发送过来的，所以肯定是服务器帮我们调用的。

```python
# encoding=utf8

from wsgiref.simple_server import make_server
from hello import application

# 创建一个wsgi服务器对象, 同时绑定ip地址，端口号，以及处理http请求的函数
httpd = make_server('', 8000, application)  #  ip地址如果写空，则会绑定本机上所有的ip，包括虚拟ip
print('Server Http on 8000')
# 开始监听该ip地址以及该端口所收到的请求
httpd.serve_forever()
```

由于我上面 make_server中的第一个参数传的是一个空字符，所以我可以通过下面任何一个IPv4地址请求到我的wsgi服务器 

![](https://img-blog.csdn.net/20180915174402214?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Web框架 -- flask

web框架 ， 由于服务器需要根据不同的url做出不同的逻辑处理，但是 wsgi接收到的浏览器发送过来的请求信息全部都放在了application函数的environ中，也就是说你想对不同的url做出不同的处理还必须经过这个application函数，也就是我们必须在application函数体内从environ变量中取出url的路径，然后再判断分别去调用不同的函数去处理该请求，这是一件非常麻烦的事，并且不利于后期的维护。

```python
def application(environ, start_response):
    method = environ['REQUEST_METHOD']
    path = environ['PATH_INFO']
    if method=='GET' and path=='/':
        return handle_home(environ, start_response)
    if method=='POST' and path='/signin':
        return handle_signin(environ, start_response)
    ...
```

所以出现了web框架，比较常用是flask。

使用前先安装
`pip install flask`
使用如下

```python
# encoding=utf8

from flask import Flask
from flask import request

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])   # ip+端口 就代表 / ，即根目录，这个请求就会被该函数处理
def home():
    return '<h1>Home</h1>'  # 返回内容就是字符串，不用想wsgi一样转换成byte

@app.route('/signin', methods=['GET'])   #  url中输入 ip+端口/signin 就会被这个函数处理 ，注意通过url输入地址访问的均是get请求
def signin_form():

    #  action 代表提交的地址，method代表提交的方式
    # input中的name属性代表将该文本框中值传递到后端，后端接收该值的变量名
    # form标签是表单，当点击提交按钮时 form标签围住的区域会被传递给服务器，
    # 服务器接收时，会用input框中name属性的值作为变量名，然后用这个变量去接收对应的值
    return '''<form action="/signin" method="post">     
              <p><input name="username"></p>    
              <p><input name="password" type="password"></p> 
              <p><button type="submit">Sign In</p>    
              </form>     
            '''

@app.route('/signin', methods=['POST'])   #  这个请求是通过点击登陆按钮，然后触发表单提交事件，表单提交设置的是POST函数
def signin():
    # request中存放了请求信息，从request对象读取表单内容：
    if request.form['username'] == 'admin' and request.form['password'] == "123456":
        return '<h3>Hello, admin!</h3>'
    return '<h3>Bad username or password.</h3>'

if __name__ == '__main__':
    app.run()   #  通过run函数启动服务器
```

效果

![](https://img-blog.csdn.net/20180915231505496?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

登录页面 。密码 ： 123456 

![](https://img-blog.csdn.net/20180915231641214?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

效果 

![](https://img-blog.csdn.net/20180915231713745?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

登录页面。 密码 1111

![](https://img-blog.csdn.net/20180915231822333?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

效果

![](https://img-blog.csdn.net/20180915231802854?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

通过上面的示例，大家肯定能看出来，return 后面直接跟的是 前端页面的代码，显然这样非常不合适，这就叫前后端耦合。其实通常前端程序员将页面的模板写好，我们输入url向后端发送一条请求，后端将页面返回但是不是通过上面的方式直接在return后面写前端代码，而是直接返回一个别人写好的前端页面，页面中显示数据的位置，被el表达式，或者一些框架预先占好位置，然后通过一些前端框架实现当页面展示到浏览器时，自动再向后端发送请求来获取数据，后端返回数据，浏览器再将数据放到指定的位置。

如下是 前端通过vue框架 在table中占位置

![](https://img-blog.csdn.net/20180915233540209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如下 是前端 通过ajax，在页面加载完毕后向后端发送请求获取数据

![](https://img-blog.csdn.net/20180915233645293?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

再通过vue框架，将返回的数据放入页面中展示给用户

![](https://img-blog.csdn.net/20180915233855812?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180915233942459?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面是 java 将信息返回给前端的代码

![](https://img-blog.csdn.net/20180915234240114?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 通过对比可以明显看出，我们上述python 的使用方式存在很大的问题。

下一篇文章我将展示如何用python实现上述操作。

