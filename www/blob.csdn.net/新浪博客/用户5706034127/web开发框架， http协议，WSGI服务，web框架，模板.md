# web开发框架， http协议，WSGI服务，web框架，模板_用户5706034127_新浪博客
||分类：[web](http://blog.sina.com.cn/s/articlelist_5706034127_7_1.html)|
### 知识点：http协议，WSGI服务，web框架，模板，的相互关系
### 一，HTTP请求
跟踪了新浪的首页，我们来总结一下HTTP请求的流程：
步骤1：浏览器首先向服务器发送HTTP请求，请求包括：
方法：GET还是POST，GET仅请求资源，POST会附带用户数据；
路径：/full/url/path；
域名：由Host头指定：Host: www.sina.com.cn
以及其他相关的Header；
如果是POST，那么请求还包括一个Body，包含用户数据。
步骤2：服务器向浏览器返回HTTP响应，响应包括：
响应代码：200表示成功，3xx表示重定向，4xx表示客户端发送的请求有错误，5xx表示服务器端处理时发生了错误；
响应类型：由Content-Type指定；
以及其他相关的Header；
通常服务器的HTTP响应会携带内容，也就是有一个Body，包含响应的内容，网页的HTML源码就在Body中。
步骤3：如果浏览器还需要继续向服务器请求其他资源，比如图片，就再次发出HTTP请求，重复步骤1、2。
Web采用的HTTP协议采用了非常简单的请求-响应模式，从而大大简化了开发。当我们编写一个页面时，我们只需要在HTTP请求中把HTML发送出去，不需要考虑如何附带图片、视频等，浏览器如果需要请求图片和视频，它会发送另一个HTTP请求，因此，一个HTTP请求只处理一个资源。
HTTP协议同时具备极强的扩展性，虽然浏览器请求的是`http://www.sina.com.cn/`的首页，但是新浪在HTML中可以链入其他服务器的资源，比如`![web开发框架， <wbr>http协议，WSGI服务，web框架，模板](http://i1.sinaimg.cn/home/2013/1008/U8455P30DT20131008135420.png)`，从而将请求压力分散到各个服务器上，并且，一个站点可以链接到其他站点，无数个站点互相链接起来，就形成了World
Wide Web，简称WWW。
HTML定义了页面的内容，CSS来控制页面元素的样式，而JavaScript负责页面的交互逻辑。
#### 二，WSGI接口
了解了HTTP协议和HTML文档，我们其实就明白了一个Web应用的本质就是：
- 
浏览器发送一个HTTP请求；
- 
服务器收到请求，生成一个HTML文档；
- 
服务器把HTML文档作为HTTP响应的Body发送给浏览器；
- 
浏览器收到HTTP响应，从HTTP Body取出HTML文档并显示。
所以，最简单的Web应用就是先把HTML用文件保存好，用一个现成的HTTP服务器软件，接收用户请求，从文件中读取HTML，返回。Apache、Nginx、Lighttpd等这些常见的静态服务器就是干这件事情的。
如果要动态生成HTML，就需要把上述步骤自己来实现。不过，接受HTTP请求、解析HTTP请求、发送HTTP响应都是苦力活，如果我们自己来写这些底层代码，还没开始写动态HTML呢，就得花个把月去读HTTP规范。
正确的做法是底层代码由专门的服务器软件实现，我们用Python专注于生成HTML文档。因为我们不希望接触到TCP连接、HTTP原始请求和响应格式，所以，需要一个统一的接口，让我们专心用Python编写Web业务。
这个接口就是WSGI：Web Server Gateway
Interface。
WSGI接口定义非常简单，它只要求Web开发者实现一个函数，就可以响应HTTP请求。我们来看一个最简单的Web版本的“Hello,
web!”：
```
def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    return '
```
# Hello, web!
'
上面的`application()`函数就是符合WSGI标准的一个HTTP处理函数，它接收两个参数：
- 
environ：一个包含所有HTTP请求信息的`dict`对象；
- 
start_response：一个发送HTTP响应的函数。
在`application()`函数中，调用：
```
start_response('200 OK', [('Content-Type', 'text/html')])
```
就发送了HTTP响应的Header，注意Header只能发送一次，也就是只能调用一次`start_response()`函数。`start_response()`函数接收两个参数，一个是HTTP响应码，一个是一组`list`表示的HTTP
Header，每个Header用一个包含两个`str`的`tuple`表示。
通常情况下，都应该把`Content-Type`头发送给浏览器。其他很多常用的HTTP
Header也应该发送。
然后，函数的返回值`'`
# ```Hello, web!```
'将作为HTTP响应的Body发送给浏览器。
有了WSGI，我们关心的就是如何从`environ`这个`dict`对象拿到HTTP请求信息，然后构造HTML，通过`start_response()`发送Header，最后返回Body。
整个`application()`函数本身没有涉及到任何解析HTTP的部分，也就是说，底层代码不需要我们自己编写，我们只负责在更高层次上考虑如何响应请求就可以了。
不过，等等，这个`application()`函数怎么调用？如果我们自己调用，两个参数`environ`和`start_response`我们没法提供，返回的`str`也没法发给浏览器。
所以`application()`函数必须由WSGI服务器来调用。有很多符合WSGI规范的服务器，我们可以挑选一个来用。但是现在，我们只想尽快测试一下我们编写的`application()`函数真的可以把HTML输出到浏览器，所以，要赶紧找一个最简单的WSGI服务器，把我们的Web应用程序跑起来。
好消息是Python内置了一个WSGI服务器，这个模块叫wsgiref，它是用纯Python编写的WSGI服务器的参考实现。所谓“参考实现”是指该实现完全符合WSGI标准，但是不考虑任何运行效率，仅供开发和测试使用。
### 运行WSGI服务
我们先编写`hello.py`，实现Web应用程序的WSGI处理函数：
```
# hello.py
def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    return '
```
# Hello, web!
'
然后，再编写一个`server.py`，负责启动WSGI服务器，加载`application()`函数：
```
# server.py
# 从wsgiref模块导入:
from wsgiref.simple_server import make_server
# 导入我们自己编写的application函数:
from hello import application
# 创建一个服务器，IP地址为空，端口是8000，处理函数是application:
httpd = make_server('', 8000, application)
print "Serving HTTP on port 8000..."
# 开始监听HTTP请求:
httpd.serve_forever()
```
确保以上两个文件在同一个目录下，然后在命令行输入
```
python
server.py
```
来启动WSGI服务器：
注意：如果`8000`端口已被其他程序占用，启动将失败，请修改成其他端口。
启动成功后，打开浏览器，输入`http://localhost:8000/`，就可以看到结果了：
在命令行可以看到wsgiref打印的log信息：
按`Ctrl+C`终止服务器。
如果你觉得这个Web应用太简单了，可以稍微改造一下，从`environ`里读取`PATH_INFO`，这样可以显示更加动态的内容：
```
# hello.py
def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    return '
```
# Hello, %s!
' % (environ['PATH_INFO'][1:] or'web')
你可以在地址栏输入用户名作为URL的一部分，将返回
```
Hello,
xxx!
```
：
三，web框架
#### 使用Web框架
了解了WSGI框架，我们发现：其实一个Web App，就是写一个WSGI的处理函数，针对每个HTTP请求进行响应。
但是如何处理HTTP请求不是问题，问题是如何处理100个不同的URL。
每一个URL可以对应GET和POST请求，当然还有PUT、DELETE等请求，但是我们通常只考虑最常见的GET和POST请求。
一个最简单的想法是从`environ`变量里取出HTTP请求的信息，然后逐个判断：
```
def application(environ, start_response):
    method = environ['REQUEST_METHOD']
    path = environ['PATH_INFO']
    if method=='GET' and path=='/':
        return handle_home(environ, start_response)
    if method=='POST' and path='/signin':
        return handle_signin(environ, start_response)
    ...
```
只是这么写下去代码是肯定没法维护了。
代码这么写没法维护的原因是因为WSGI提供的接口虽然比HTTP接口高级了不少，但和Web
App的处理逻辑比，还是比较低级，我们需要在WSGI接口之上能进一步抽象，让我们专注于用一个函数处理一个URL，至于URL到函数的映射，就交给Web框架来做。
由于用Python开发一个Web框架十分容易，所以Python有上百个开源的Web框架。这里我们先不讨论各种Web框架的优缺点，直接选择一个比较流行的Web框架——[Flask](http://flask.pocoo.org/)来使用。
用Flask编写Web App比WSGI接口简单（这不是废话么，要是比WSGI还复杂，用框架干嘛？），我们先用`easy_install`或者`pip`安装Flask：
```
$ easy_install flask
```
然后写一个`app.py`，处理3个URL，分别是：
- 
```
GET /
```
：首页，返回`Home`；
- 
```
GET /signin
```
：登录页，显示登录表单；
- 
```
POST /signin
```
：处理登录表单，显示登录结果。
注意噢，同一个URL`/signin`分别有GET和POST两种请求，映射到两个处理函数中。
Flask通过Python的[装饰器](http://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000/001386819879946007bbf6ad052463ab18034f0254bf355000)在内部自动地把URL和函数给关联起来，所以，我们写出来的代码就像这样：
```
from flask import Flask
from flask import request
app = Flask(__name__)
@app.route('/', methods=['GET', 'POST'])
def home():
    return '<<span class="title">h1>
```
Home</<span class="title">h1>'
@app.route('/signin', methods=['GET'])
def signin_form():
    return '''<<span class="title">formaction="/signin"method="post">
              <<span class="title">p><<span class="title">inputname="username"></<span class="title">p>
              <<span class="title">p><<span class="title">inputname="password"type="password"></<span class="title">p>
              <<span class="title">p><<span class="title">buttontype="submit">Sign In</<span class="title">button></<span class="title">p>
              </<span class="title">form>'''
@app.route('/signin', methods=['POST'])
def signin():
    # 需要从request对象读取表单内容：
    if request.form['username']=='admin' and request.form['password']=='password':
        return '<<span class="title">h3>Hello, admin!</<span class="title">h3>'
    return '<<span class="title">h3>Bad username or password.</<span class="title">h3>'
if __name__ == '__main__':
    app.run()
运行
```
python
app.py
```
，Flask自带的Server在端口`5000`上监听：
```
$ python app.py 
 * Running on http://127.0.0.1:5000/
```
打开浏览器，输入首页地址`http://localhost:5000/`：
除了Flask，常见的Python Web框架还有：
- 
[Django](https://www.djangoproject.com/)：全能型Web框架；
- 
[web.py](http://webpy.org/)：一个小巧的Web框架；
- 
[Bottle](http://bottlepy.org/)：和Flask类似的Web框架；
- 
[Tornado](http://www.tornadoweb.org/)：Facebook的开源异步Web框架。
当然了，因为开发Python的Web框架也不是什么难事，我们后面也会自己开发一个Web框架。
有了Web框架，我们在编写Web应用时，注意力就从WSGI处理函数转移到URL+对应的处理函数，这样，编写Web
App就更加简单了。
在编写URL处理函数时，除了配置URL外，从HTTP请求拿到用户数据也是非常重要的。Web框架都提供了自己的API来实现这些功能。Flask通过`request.form['name']`来获取表单的内容。
四，模板
#### 使用模板
Web框架把我们从WSGI中拯救出来了。现在，我们只需要不断地编写函数，带上URL，就可以继续Web App的开发了。
但是，Web
App不仅仅是处理逻辑，展示给用户的页面也非常重要。在函数中返回一个包含HTML的字符串，简单的页面还可以，但是，想想新浪首页的6000多行的HTML，你确信能在Python的字符串中正确地写出来么？反正我是做不到。
俗话说得好，不懂前端的Python工程师不是好的产品经理。有Web开发经验的同学都明白，Web
App最复杂的部分就在HTML页面。HTML不仅要正确，还要通过CSS美化，再加上复杂的JavaScript脚本来实现各种交互和动画效果。总之，生成HTML页面的难度很大。
由于在Python代码里拼字符串是不现实的，所以，模板技术出现了。
使用模板，我们需要预先准备一个HTML文档，这个HTML文档不是普通的HTML，而是嵌入了一些变量和指令，然后，根据我们传入的数据，替换后，得到最终的HTML，发送给用户：
这就是传说中的MVC：Model-View-Controller，中文名“模型-视图-控制器”。
Python处理URL的函数就是C：Controller，Controller负责业务逻辑，比如检查用户名是否存在，取出用户信息等等；
包含变量
```
{{
name
}}
```
的模板就是V：View，View负责显示逻辑，通过简单地替换一些变量，View最终输出的就是用户看到的HTML。
MVC中的Model在哪？Model是用来传给View的，这样View在替换变量的时候，就可以从Model中取出相应的数据。
上面的例子中，Model就是一个`dict`：
```
{ 'name': 'Michael' }
```
只是因为Python支持关键字参数，很多Web框架允许传入关键字参数，然后，在框架内部组装出一个`dict`作为Model。
现在，我们把上次直接输出字符串作为HTML的例子用高端大气上档次的MVC模式改写一下：
```
from flask import Flask, request, render_template
app = Flask(__name__)
@app.route('/', methods=['GET', 'POST'])
def home():
    return render_template('home.html')
@app.route('/signin', methods=['GET'])
def signin_form():
    return render_template('form.html')
@app.route('/signin', methods=['POST'])
def signin():
    username = request.form['username']
    password = request.form['password']
    if username=='admin' and password=='password':
        return render_template('signin-ok.html', username=username)
    return render_template('form.html', message='Bad username or password', username=username)
if __name__ == '__main__':
    app.run()
```
Flask通过`render_template()`函数来实现模板的渲染。和Web框架类似，Python的模板也有很多种。Flask默认支持的模板是[jinja2](http://jinja.pocoo.org/)，所以我们先直接安装jinja2：
```
$ easy_install jinja2
```
然后，开始编写jinja2模板：
通过MVC，我们在Python代码中处理M：Model和C：Controller，而V：View是通过模板处理的，这样，我们就成功地把Python代码和HTML代码最大限度地分离了。
使用模板的另一大好处是，模板改起来很方便，而且，改完保存后，刷新浏览器就能看到最新的效果，这对于调试HTML、CSS和JavaScript的前端工程师来说实在是太重要了。
在Jinja2模板中，我们用
```
{{
name
}}
```
表示一个需要替换的变量。很多时候，还需要循环、条件判断等指令语句，在Jinja2中，用
```
{%
... %}
```
表示指令。
比如循环输出页码：
```
```
{% for i in page_list %}
    <<SPAN class=title style="FONT-WEIGHT: normal; COLOR: rgb(0,0,128)">a href="/page/{{ i }}">
```
{{ i }}</<SPAN class=title style="FONT-WEIGHT: normal; COLOR: rgb(0,0,128)">a>
```
{% endfor %}
如果`page_list`是一个list：
```
[1,
2, 3, 4, 5]
```
，上面的模板将输出5个超链接。
