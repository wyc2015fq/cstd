
# Python 学习入门（28）—— 服务器实例 - 阳光岛主 - CSDN博客

2013年12月17日 00:01:32[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：7776个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



在新的Python 3.x中，BaseHTTPServer, SimpleHTTPServer, CGIHTTPServer整合到http.server包，SocketServer改名为socketserver。
任何一台装有操作系统(推荐Linux)和Python的计算机都可以作为HTTP服务器使用，来架设你的网站。在这里引入更高级的Python包，以写出更成熟的Python服务器。

**1. 支持POST的socket服务器**
我们首先改写原文中的HTTP服务器，从而让该服务器支持更加丰富的HTTP请求。相对于原程序，这里增添了表格以及对应"POST"方法的操作。如果你已经读过用socket写一个Python服务器，会发现这里只是增加很少的一点内容。
原始程序:

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
import socket
# Address
HOST = ''
PORT = 8700
text_content = '''
HTTP/1.x 200 OK  
Content-Type: text/html
<head>
    <title>hello ithomer</title>
</head>
<html>
    <p>hello, Python Server</p>
    <img src="/home/homer/1_sunboy_2050.jpg"/>
    <form name="input" action="/" method="post">
        First name:<input type="text" name="firstname"><br>
        <input type="submit" value="Submit">
    </form> 
</html>
'''
f = open('/home/homer/1_sunboy_2050.jpg', 'rb')
pic_content = '''
HTTP/1.x 200 OK  
Content-Type: image/jpg
'''
pic_content = pic_content + f.read()
# Configure socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
# Serve forever
while True:
    s.listen(3)
    conn, addr = s.accept()                    
    request = conn.recv(1024)       # 1024 is the receiving buffer size
    method = request.split(' ')[0]
    src = request.split(' ')[1]
    print 'Connected by', addr
    print 'Request is:', request
    # if GET method request
    if method == 'GET':
        if src == '/test.jpg':      # if ULR is /test.jpg
            content = pic_content
        else: 
            content = text_content
        conn.sendall(content)       # send message
    # if POST method request
    if method == 'POST':
        form = request.split('\r\n')
        idx = form.index('')        # Find the empty line
        entry = form[idx:]          # Main content of the request
        value = entry[-1].split('=')[-1]
        conn.sendall(text_content + '\n <p>' + value + '</p>')
        ######
        # More operations, such as put the form into database
        # ...
        ######
    # close connection
    conn.close()
```
运行上面Python服务器，向上一篇文章那样，使用一个浏览器作为客户端。
我们看到了新增的表格以及提交(submit)按钮。在表格中输入aa并提交表格，我们的Python服务器给出上面的结果。
**2. 使用SocketServer架设服务器**
我们首先使用SocketServer包来简化我们架设服务器的过程。在上面使用socket的过程中，我们先设置了socket的类型，然后依次调用bind(),listen(),accept()，并使用while循环来让服务器不断的接受请求。上面的这些步骤可以通过SocketServer包来简化。
SocketServer:

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
@author: homer
@see: ithomer.net
'''
# use TCPServer
import SocketServer
HOST = ''
PORT = 8800
text_content = '''
HTTP/1.x 200 OK  
Content-Type: text/html
<head>
    <title>hello ithomer</title>
</head>
<html>
    <p>hello, Python Server</p>
    <img src="/home/homer/1_sunboy_2050.jpg"/>
    <form name="input" action="/" method="post">
        First name:<input type="text" name="firstname"><br>
        <input type="submit" value="Submit">
    </form> 
</html>
'''
f = open('/home/homer/1_sunboy_2050.jpg', 'rb')
pic_content = '''
HTTP/1.x 200 OK  
Content-Type: image/jpg
'''
pic_content = pic_content + f.read()
# This class defines response to each request
class MyTCPHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        # self.request is the TCP socket connected to the client
        request = self.request.recv(1024)
        print 'Connected by', self.client_address[0]
        print 'Request is', request
        method = request.split(' ')[0]
        src = request.split(' ')[1]
        if method == 'GET':
            if src == '/test.jpg':
                content = pic_content
            else: content = text_content
            self.request.sendall(content)
        if method == 'POST':
            form = request.split('\r\n')
            idx = form.index('')  # Find the empty line
            entry = form[idx:]  # Main content of the request
            value = entry[-1].split('=')[-1]
            self.request.sendall(text_content + '\n <p>' + value + '</p>')
            ######
            # More operations, such as put the form into database
            # ...
            ######

# Create the server
server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)
# Start the server, and work forever
server.serve_forever()
```
我们建立了一个TCPServer对象来创建一个TCP socket服务器，并同时设置IP地址和端口。然后使用server_forever()方法来让服务器不断工作(就像原始程序中的while循环一样)。我们传递给TCPServer一个MyTCPHandler类，用对socket作出操作。注意，MyTCPHandler继承自BaseRequestHandler，我们通过改写handler()方法来个性化我们的操作。在handler()中，可以通过self.request来引用socket (正如我们在handler()中对socket进行recv()和sendall()操作)，还可以使用self.address来引用socket的客户端地址。

**3. SimpleHTTPServer: 使用静态文件来回应请求**
在经过了SocketServer的改造之后，我们的handler()，也就是对请求进行处理的部分，依然是乱糟糟的一团。这对于大型服务器来说可能是个问题。
为什么呢？ 对于一个HTTP请求(request)来说，它的起始行包含两个重要信息：请求方法和URL。之前，我们都用if结构来区分不同的请求方法和URL，并针对不同的情况来进行不同的操作:
请求方法(request method)       URL                操作
GET                           /                  发送text_content
GET                           /text.jpg          发送pic_content
POST                          /                  分析request主体中包含的value(实际上是我们填入表格的内容); 发送text_content和value
根据请求方法和URL的不同，一个大型的HTTP服务器可能需要应付成千上万种不同的请求。如果针对每个请求都在程序中写出不同的操作的话，需要大量的时间和精力，同时为运营和维护带来很大的困难。我们需要有更标准化，也更简便的方式来处理这些请求。在Python中，我们可以使用SimpleHTTPServer包和CGIHTTPServer包来减小以上的负担。其中，SimpleHTTPServer可以用于处理GET方法和HEAD方法的请求。它读取request中的URL地址，并在当前目录中找到对应的静态文件，并将文件的内容发送给客户端。
对应于我们的情况，就是将text_content放置在index.html中，而不用读取text.jpg文件。当一个HTTP请求到来时，其URL指向某个文件，SimpleHTTPServer会读取这个文件，并分析文件类型，自动生成response，回复客户端。如果URL指向某个文件夹，SimpleHTTPServer会读取该文件夹下的index.html或者index.hml文件。
首先，我们在当前目录下生成如下index.html文件:

```python
<head>
    <title>hello ithomer</title>
</head>
<html>
    <p>hello, Python Server</p>
    <img src="/home/homer/1_sunboy_2050.jpg"/>
    <form name="input" action="/" method="post">
        First name:<input type="text" name="firstname"><br>
        <input type="submit" value="Submit">
    </form> 
</html>
```
然后，改写我们的Python服务器程序。实际上，我们只是更换了TCPServer的Handler：使用SimpleHTTPServer包中唯一的类SimpleHTTPRequestHandler，而不是我们之前自己定义的MyTCPHandler
SimpleHTTPServer:
\#Simple HTTPsERVER
importSocketServerimportSimpleHTTPServer
HOST=''PORT= 8000\#Create the server, SimpleHTTPRequestHander is pre-defined handler in SimpleHTTPServer packageserver =SocketServer.TCPServer((HOST, PORT), SimpleHTTPServer.SimpleHTTPRequestHandler)\#Start the serverserver.serve_forever()
这里的程序还不能等效于之前的程序，因为不能处理POST请求。我们会在后面使用CGI来弥补这个缺陷。但要点是，我们的Python服务器程序已经变得非常简单。我们将内容存放于静态文件，并根据URL指向的静态文件为客户端提供内容，从而让内容和Python服务器相分离。这样的话，我们每次更新内容的时候就可以只修改静态文件，而不用停止整个Python服务器。
我们也应该注意到使用这些改进付出的代价。比如说，对于原始程序来说，request中的URL只具有指导意义，我们可以任意规定相应的操作。而在 SimpleHTTPServer的改进中，response固化成为：读取URL对应文件并将其内容呈现给客户。这大大限制了我们的自由 度。即使在后面我们使用CGI增大了自由度，但相对于原始程序，我们依然是增加了自己的限制。有时候，程序的便捷与程序的自由度相抵触，程序员需要在两者之间取舍。对于一个小的项目来说，我们可以跟随已经 制定的标准(比如这里的SimpleHTTPServer，或者使用一个框架)，使用这些新的标准可以让开发变得很便捷。然而对于一个大型的项目来说，我们往往需要争取回自己的自由度，修订成为项目需要的标准。
**4. CGIHTTPServer：使用静态文件或者CGI来回应请求**
CGIHTTPServer包中的CGIHTTPRequestHandler类继承自SimpleHTTPRequestHandler类，所以可以用来代替上面的例子，来提供静态文件的服务。此外，CGIHTTPRequestHandler类还可以用来运行CGI脚本。
首先，我们先看看什么是CGI(Common Gateway Interface)。CGI是服务器和应用脚本之间的一套接口标准，目的是让服务器程序运行脚本程序，将程序的输出作为response发送给客户。通常来说，支持CGI的服务器程在接收到客户的request之后，根据request中的URL，运行对应的脚本文件。服务器会将HTTP request信息以及socket信息输入给脚本文件，也负责收集脚本的输出，并组装成为合法的HTTP response。利用CGI，我们可以充分发挥服务器的可编程性，动态的生成response，而不必局限于静态文件。
服务器和CGI脚本之间通过CGI标准作为接口。这样就可以让服务器与不同语言写的CGI脚本相配合，比如说使用Apache服务器与Perl写的CGI脚本，或者Python服务器与shell写的CGI脚本。
到这里为止，我们都在使用TCPServer来构建服务器。为了使用CGI，我们需要使用BaseHTTPServer包中的HTTPServer类来构建服务器。其实HTTPServer是TCPServer的子类，其使用方法也与TCPServer相同。它只是增加了server_name和server_port两个属性。但不凑巧的是，我们的CGIHTTPRequestHandler需要调用这两个属性...
Python服务器的改动很简单。
CGIHTTPServer:
\#A messy HTTP server based on TCP socket
importBaseHTTPServerimportCGIHTTPServer
HOST=''PORT= 8000\#Create the server, CGIHTTPRequestHandler is pre-defined handlerserver =BaseHTTPServer.HTTPServer((HOST, PORT), CGIHTTPServer.CGIHTTPRequestHandler)\#Start the serverserver.serve_forever()
CGIHTTPRequestHandler默认当前目录下的cgi-bin和ht-bin文件夹中的文件为CGI脚本，而存放于其他地方的文件被认为是静态文件。因此，我们需要修改一下index.html，将其中form元素指向的action改为cgi-bin/post.py。
<head>
<title>WOW</title></head><html><p>Wow, Python Server</p><IMGsrc="test.jpg"/><formname="input"action="cgi-bin/post.py"method="post">First name:<inputtype="text"name="firstname"><br><inputtype="submit"value="Submit"></form></html>
我们创建一个cgi-bin的文件夹，并在cgi-bin中放入如下post.py文件，也就是我们的CGI脚本：
\#!/usr/bin/env python
\#Written by Vameiimportcgi
form=cgi.FieldStorage()\#Output to stdout, CGIHttpServer will take this as response to the clientprint"Content-Type: text/html"\#HTML is followingprint\#blank line, end of headersprint"<p>Hello world!</p>"\# Start of contentprint"<p>"+repr(form['firstname']) +"</p>"
(post.py需要有执行权限，见评论区)
第一行必须要有，以便告诉Python服务器，脚本所使用的语言 (我们这里的CGI是Python，当然也可以是别的语言，比如bash)。 cgi包用于提取request中提交的表格信息(我们暂时不深入cgi包)。脚本只负责将所有的结果输出到标准输出(使用print)。而CGIHTTPRequestHandler会收集这些输出，并组装成为response传送给客户端。
如果一个请求是POST方法，那么它的URL必须指向一个CGI脚本(也就是在cgi-bin或者ht-bin中的文件)。CGIHTTPRequestHandler继承自SimpleHTTPRequestHandler，所以也可以处理GET方法和HEAD方法的请求。此时，如果URL指向CGI脚本时，服务器将脚本的运行结果传送到客户端；当此时URL指向静态文件时，服务器将文件的内容传送到客户端。
我们可以让CGI脚本执行数据库操作，比如将接收到的数据放入到数据库中，以及更丰富的程序操作。CGI脚本提供了LAMP架构中PHP的作用 (我们的Python服务器相当于LAMP中的Apache)。

**5. 总结**
我们使用了Python标准库中的一些高级包简化了Python服务器，并将静态内容、CGI应用和服务器分割，降低三者之间的耦合，从而可以更加便捷的构建和维护Python服务器。但是，我们并没有通过这些高级包得到额外的功能。所有这些高级包可以做到的，也完全可以利用低级的socket和Python编程实现，而socket许多可以实现的，则不能在这些高级包中实现。高级包带来的是便捷和规范，代价是一定的自由度。
到现在为止，我们已经粗略的介绍了Python中构建HTTP服务器的常用工具。希望你享受在自己的电脑上架设服务器的过程。

**参考推荐：**
[Python服务器进化](http://www.cnblogs.com/vamei/archive/2012/10/31/2747885.html)



