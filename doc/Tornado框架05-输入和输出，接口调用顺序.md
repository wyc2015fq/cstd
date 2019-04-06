# Tornado框架05-输入和输出，接口调用顺序

 

# 01-利用HTTP协议向服务器传参几种途径

1. 查询字符串（query string)，形如key1=value1&key2=value2；
2. 请求体（body）中发送的数据，比如表单数据、json、xml；
3. 提取uri的特定部分，如/blogs/2016/09/0001，可以在服务器端的路由中用正则表达式截取；
4. 在http报文的头（header）中增加自定义字段，如X-XSRFToken=xiaoke

# 02-获取查询字符串参数

> get_query_argument(name, default=_ARG_DEFAULT, strip=True)

从请求的查询字符串中返回指定参数name的值，如果出现多个同名参数，则返回最后一个的值。

default为设值未传name参数时返回的默认值，如若default也未设置，则会抛出tornado.web.MissingArgumentError异常。

strip表示是否过滤掉左右两边的空白字符，默认为过滤。

> get_query_arguments(name, strip=True)

从请求的查询字符串中返回指定参数name的值，注意返回的是list列表（即使对应name参数只有一个值）。若未找到name参数，则返回空列表[]。

strip同前，不再赘述。

# 03-获取请求体参数

> get_body_argument(name, default=_ARG_DEFAULT, strip=True)

从请求体中返回指定参数name的值，如果出现多个同名参数，则返回最后一个的值。

default与strip同前，不再赘述。

> get_body_arguments(name, strip=True)

从请求体中返回指定参数name的值，注意返回的是list列表（即使对应name参数只有一个值）。若未找到name参数，则返回空列表[]。

strip同前，不再赘述。

说明

**对于请求体中的数据要求为字符串，且格式为表单编码格式**（与url中的请求字符串格式相同），即key1=value1&key2=value2，HTTP报文头Header中的”Content-Type”为application/x-www-form-urlencoded  或 multipart/form-data。

对于请求体数据为json或xml的，无法通过这两个方法获取。

# 04-前两类方法的整合

> get_argument(name, default=_ARG_DEFAULT, strip=True)

从请求体和查询字符串中返回指定参数name的值，如果出现多个同名参数，则返回最后一个的值。

default与strip同前，不再赘述。

> get_arguments(name, strip=True)

从请求体和查询字符串中返回指定参数name的值，注意返回的是list列表（即使对应name参数只有一个值）。若未找到name参数，则返回空列表[]。

strip同前，不再赘述。

**说明** 

对于请求体中数据的要求同前。 这两个方法最常用。

注意：以上方法返回的都是unicode字符串

# 05-requestHandler.request请求信息

RequestHandler.request 对象存储了关于请求的相关信息，具体属性有：

- method HTTP的请求方式，如GET或POST;
- host 被请求的主机名；
- uri 请求的完整资源标示，包括路径和查询字符串；
- path 请求的路径部分；
- query 请求的查询字符串部分；
- version 使用的HTTP版本；
- headers 请求的协议头，是类字典型的对象，支持关键字索引的方式获取特定协议头信息，例如：request.headers[“Content-Type”]
- body 请求体数据；
- remote_ip 客户端的IP地址；
- files 用户上传的文件，为字典类型，型如：

```
{
  "form_filename1":[<tornado.httputil.HTTPFile>, <tornado.httputil.HTTPFile>],

  "form_filename2":[<tornado.httputil.HTTPFile>,],
  ...
}
```

tornado.httputil.HTTPFile是接收到的文件对象，它有三个属性：

- filename 文件的实际名字，与form_filename1不同，字典中的键名代表的是表单对应项的名字；
- body 文件的数据实体；
- content_type 文件的类型。

这三个对象属性可以像字典一样支持关键字索引，如request.files[“form_filename1”][0][“body”]。

# 06-正则提取uri

tornado中对于路由映射也支持正则提取uri，提取出来的参数会作为RequestHandler中对应请求方式的成员方法参数。 

若在正则表达式中定义了名字，则参数按名传递； 

若未定义名字，则参数按顺序传递。  
 提取出来的参数会作为对应请求方式的成员方法的参数。

```
define("port", default=8000, type=int, help="run server on the given port.")

class IndexHandler(RequestHandler):
    def get(self):
        self.write("hello qianfeng.")

class SubjectCityHandler(RequestHandler):
    def get(self, subject, city):
        self.write(("Subject: %s<br/>City: %s" % (subject, city)))

class SubjectDateHandler(RequestHandler):
    def get(self, date, subject):
        self.write(("Date: %s<br/>Subject: %s" % (date, subject)))

if __name__ == "__main__":
    tornado.options.parse_command_line()
    app = tornado.web.Application([
        (r"/", IndexHandler),
        (r"/sub-city/(.+)/([a-z]+)", SubjectCityHandler), # 无名方式
        (r"/sub-date/(?P<subject>.+)/(?P<date>\d+)", SubjectDateHandler), #　命名方式
    ])
    http_server = tornado.httpserver.HTTPServer(app)
    http_server.listen(options.port)
    tornado.ioloop.IOLoop.current().start()
```

建议：提取多个值时最好用命名方式。

# 07-write方法

write方法是写到缓冲区的，我们可以像写文件一样多次使用write方法不断追加响应内容，最终所有写到缓冲区的内容一起作为本次请求的响应输出。

利用write方法写json数据？

```
import json

class IndexHandler(RequestHandler):
    def get(self):
        stu = {
            "name":"zhangsan",
            "age":24,
            "gender":1,
        }
        stu_json = json.dumps(stu)
        self.write(stu_json)
```

实际上，我们可以不用自己手动去做json序列化，当write方法检测到我们传入的chunk参数是字典类型后，会自动帮我们转换为json字符串。

```
class IndexHandler(RequestHandler):
    def get(self):
        stu = {
            "name":"zhangsan",
            "age":24,
            "gender":1,
        }
        self.write(stu)
```

**两种方式有什么差异？**

对比一下两种方式的响应头header中Content-Type字段，自己手动序列化时为Content-Type:text/html; charset=UTF-8，

而采用write方法时为Content-Type:application/json; charset=UTF-8。

write方法除了帮我们将字典转换为json字符串之外  
 还帮我们将Content-Type设置为application/json; charset=UTF-8。

# 08-set_header(name, value)属性

利用set_header(name, value)方法，可以手动设置一个名为name、值为value的响应头header字段。  
 例如：

```
self.set_header("Content-Type", "application/json; charset=UTF-8")
```

# 09-set_default_headers()方法

该方法会在进入HTTP处理方法前先被调用，可以重写此方法来预先设置默认的headers。  
 注意：在HTTP处理方法中使用set_header()方法会覆盖掉在set_default_headers()方法中设置的同名header。

```
class IndexHandler(RequestHandler):

    def set_default_headers(self):
        print "执行了set_default_headers()"
        # 设置get与post方式的默认响应体格式为json
        self.set_header("Content-Type", "application/json; charset=UTF-8")
        # 设置一个名为name、值为python的header
        self.set_header("name", "python")

    def get(self):
        print "执行了get()"
        stu = {
            "name":"zhangsan",
            "age":24,
            "gender":1,
        }
        stu_json = json.dumps(stu)
        self.write(stu_json)
        self.set_header("name", "i love python") # 注意此处重写了header中的name字段,会覆盖掉在set_default_headers()方法中设置的同名header。
```

# 10-set_status(status_code, reason=None)

为响应设置状态码。

- status_code int类型，状态码
- reason string类型，描述状态码的词组，若为None，则会被自动填充为标准的状态码。
- 非标准状态码 self.set_status(210, “python error”) # 非标准状态码，必须设置了reason。否则报错

# 11-redirect(url)

告知浏览器重定向到url。

```
class LoginHandler(RequestHandler):
    """对应/login"""
    def get(self):
        self.write('<form method="post"><input type="submit" value="登陆"></form>')

    def post(self):
        self.redirect("/")
```

# 12-send_error(status_code=500, **kwargs)

抛出HTTP错误状态码status_code，默认为500，kwargs为可变命名参数。使用send_error抛出错误后tornado会调用write_error()方法进行处理，并返回给浏览器处理后的错误页面。

```
class IndexHandler(RequestHandler):
    def get(self):
        self.write("主页")
        self.send_error(404, content="出现404错误")
```

注意： 

1，默认的write_error()方法不会处理send_error抛出的kwargs参数，即上面的代码中content=”出现404错误”是没有意义的。要想处理这个错误信息，必须重写write_error()方法

2，使用send_error()方法后就不要再使用self.write()，再向输出缓冲区写内容了！

# 13-write_error(status_code, **kwargs)

用来处理send_error抛出的错误信息并返回给浏览器错误信息页面。可以重写此方法来定制自己的错误显示页面。

```
class IndexHandler(RequestHandler):
    def get(self):
        err_code = self.get_argument("code", None) # 注意返回的是unicode字符串，下同
        err_title = self.get_argument("title", "")
        err_content = self.get_argument("content", "")

        if err_code:
            self.send_error(err_code, title=err_title, content=err_content)
        else:
            self.write("主页")

    def write_error(self, status_code, **kwargs):
        self.write(u"<h1>出错了，程序员GG正在赶过来！</h1>")
        self.write(u"<p>错误名：%s</p>" % kwargs["title"])
        self.write(u"<p>错误详情：%s</p>" % kwargs["content"])
```

# 14-接口与调用顺序

```
class IndexHandler(RequestHandler):

    # 对应每个请求的处理类Handler在构造一个实例后首先执行initialize()方法。路由映射中的第三个字典型参数会作为该方法的命名参数传递，
    #作用：初始化参数（对象属性）
    def initialize(self):
        print "调用了initialize()"

    # 预处理，即在执行对应请求方式的HTTP方法（如get、post等）前先执行，注意：不论以何种HTTP方式请求，都会执行prepare()方法。
    def prepare(self):
        print "调用了prepare()"

    def set_default_headers(self):
        print "调用了set_default_headers()"

    def write_error(self, status_code, **kwargs):
        print "调用了write_error()"

    def get(self):
        print "调用了get()"

    def post(self):
        print "调用了post()"
        self.send_error(200)  # 注意此出抛出了错误

    # 在请求处理结束后调用，即在调用HTTP方法后调用。通常该方法用来进行资源清理释放或处理日志等。注意：请尽量不要在此方法中进行响应输出。
    def on_finish(self):
        print "调用了on_finish()"
```

在正常情况未抛出错误时，调用顺序为：

- set_defautl_headers()
- initialize()
- prepare()
- HTTP方法（get或post方法）
- on_finish()

在有错误抛出时，调用顺序为：

- set_default_headers()
- initialize()
- prepare()
- HTTP方法
- set_default_headers()
- write_error()
- on_finish()