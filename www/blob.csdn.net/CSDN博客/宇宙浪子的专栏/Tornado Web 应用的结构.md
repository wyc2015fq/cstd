# Tornado Web 应用的结构 - 宇宙浪子的专栏 - CSDN博客
2017年11月16日 18:06:20[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：206标签：[python																[tornado](https://so.csdn.net/so/search/s.do?q=tornado&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python																[tornado](https://blog.csdn.net/bluehawksky/article/category/7102850)](https://blog.csdn.net/bluehawksky/article/category/2542577)
转自：[http://blog.chriscabin.com/coding-life/web-framework/tornado/1593.html](http://blog.chriscabin.com/coding-life/web-framework/tornado/1593.html)
# Tornado Web 应用的结构
通常来说，一个基于 Tornado 的 Web 应用会包含一个或多个继承自 `RequestHandler` 的子类，一个 用于将请求路由到回调的`Application` 对象以及一个用于启动服务器的 `main()` 函数。
最简单的 `hello, world` 版本如下：
- 
```python
import tornado.ioloop
```
- 
```python
import tornado.web
```
- 
```python
```
- 
```python
class MainHandler(tornado.web.RequestHandler):
```
- 
```python
def get(self):
```
- 
```python
self.write("Hello, world")
```
- 
```python
```
- 
```python
```
- 
```python
def make_app():
```
- 
```python
return tornado.web.Application([
```
- 
```python
(r"/", MainHandler),
```
- 
```python
])
```
- 
```python
```
- 
```python
```
- 
```python
if __name__ == "__main__":
```
- 
```python
app = make_app()
```
- 
```python
app.listen(8888)
```
- 
```python
tornado.ioloop.IOLoop.current().start()
```
# Application 对象
`Application` 对象负责全局配置，包括路由表配置（用于映射请求及对应的回调）。
路由表实际上是一个 `URLSpec` 对象（或元组）所构成的列表，每个都包含（至少）一个正则表达式和一个 handler 类。顺序很重要，第一个匹配的规则会被优先应用。如果正则表达式包含捕获组，这些组就是路径参数，这些参数会被传递到
 handler 的 HTTP 方法。如果将字典作为 `URLSpec` 的第三个元素，则它会提供初始化参数，并会被传递到 `RequestHandler.initialize`。最后，`URLSpec` 可能会有一个名称，从而使它能够在`RequestHandler.reverse_url` 中使用。
下面的例子中，根 URL `/` 会被映射到 `MainHanlder`；跟随数字的 `/story/` URL
 会被映射到 `StoryHandler`。数字（字符串格式）会被传递到 `StoryHandler.get` 中。
- 
```python
class MainHanlder(RequestHandler):
```
- 
```python
def get(self):
```
- 
```python
self.write('<a href="{}">link to stroy</a>'.format(self.reverse_url('story', '1')))
```
- 
```python
```
- 
```python
class StoryHandler(RequestHandler):
```
- 
```python
def initialize(self, db):
```
- 
```python
self.db = db
```
- 
```python
```
- 
```python
def get(self, story_id):
```
- 
```python
self.write("this is story {}".format(story_id))
```
- 
```python
```
- 
```python
app = Application([
```
- 
```python
url(r'/', MainHanlder),
```
- 
```python
url(r'/story/([0-9+])', StoryHandler, dict(db=db), name="story")
```
- 
```python
])
```
`Application` 构造器可以接受很多关键词参数，从而深度定制应用的行为表现，并且启用一些可选的功能；详情参见 [Application.settings](http://www.tornadoweb.org/en/stable/web.html#tornado.web.Application.settings)。
# 继承 `RequestHandler`
在 Tornado web 应用中，许多要做的工作都是在 `RequestHandler` 子类中完成的。在 handler 类中，最主要的入口都是按照
 HTTP 方法命名的：`get()`, `post()` 等。每个
 handler 可能会定义其中一个或多个方法来处理不同的 HTTP 请求。如上所述，这些方法会在对应的路由规则匹配时调用，相应调用参数则来自对应的捕获组。
在 handler 中，调用类似 `RequestHandler.render` 或者 `RequestHandler.write` 可以产生一个响应。`render` 会加载根据名称加载对应的 `Template` 并使用给定的参数渲染模板。`write` 则是用于非模板类输出；它会接受字符串、字节和字典值（字典是用
 JSON 编码过的）。
许多在 `RequestHandler` 中的方法都是被设计用于在子类中重写的，并且它们会在整个应用中使用。通常可以定义一个 `BaseHandler` 类，该类将 `write_error` 和 `get_current_user` 之类的方法重写，然后在应用中创建新的
 handler 时就继承你自己定义的 `BaseHandler` 而非 `RequestHandler`。
# 处理请求输入
请求 handler 可以使用 `self.request` 访问表示当前请求的对象。参见 [HTTPServerRequest](http://www.tornadoweb.org/en/stable/httputil.html#tornado.httputil.HTTPServerRequest) 获得完整的属性。
HTML 表单中的请求数据会自动解析，你可以使用类似 `get_query_argument` 和 `get_body_argument` 方法访问：
- 
```python
form_template = '''
```
- 
```python
<html><body><form action="/myform" method="POST">
```
- 
```python
<input type="text" name="message">
```
- 
```python
<input type="submit" value="Submit">
```
- 
```python
</form></body></html>
```
- 
```python
'''
```
- 
```python
class MyFormHandler(RequestHandler):
```
- 
```python
def get(self):
```
- 
```python
self.write(form_template)
```
- 
```python
```
- 
```python
def post(self):
```
- 
```python
self.set_header("Content-Type", "text/plain")
```
- 
```python
self.write("You wrote " + self.get_body_argument("message"))
```
由于 HTML 表单对于一个参数是单一值还是拥有一个元素的列表的编码是模糊的，所以 `RequestHandler` 提供了区分的方法从而让应用能够显式决定是否需要一个列表值。对于列表，可以使用 `get_query_arguments` 和`get_body_arguments` 获取。
通过表单上传的文件可以从 `self.request.files` 中访问，它会将名称（HTML 
```
<input
 type="file">
```
 元素的名称）映射到一文件列表。每个文件都是一个字典：
```
{"filename": ..., "content_type":
 ..., "body":...}
```
。只有使用了表单包装器上传的文件才能够在 `files` 对象中访问到（例如：Content-Type
 为 `multipart/form-data`）；如果没有使用这种格式，上传的原始数据可以从 `self.request.body` 中访问。默认情况下，上传的文件都会完全缓存在内存中；如果你需要处理内存也无法完全放得下的文件，请考虑 [stream_request_body](http://www.tornadoweb.org/en/stable/web.html#tornado.web.stream_request_body) 类装饰器。
在示例目录下，[file_receiver.py](https://github.com/tornadoweb/tornado/tree/master/demos/file_upload/) 展示了两种接收文件的方法。
鉴于 HTML 表单编码比较奇葩（如单复数参数定义模糊），Tornado 不会尝试对其它类型的输入统一参数。尤其是我们并不会解析 JSON 请求数据。对于希望使用 JSON 而非表单编码的应用，可以重写 `prepare` 方法来解析请求：
- 
```python
def prepare(self):
```
- 
```python
if self.request.headers['Content-Type'].startswith('application/json'):
```
- 
```python
self.json_args = json.loads(self.request.body)
```
- 
```python
else:
```
- 
```python
self.json_args = None
```
# 重载 `RequestHandler` 方法
除了 `get()`/`post()` 之类的方法，`RequestHandler` 中定义的一些其它方法也是用于在必要时重写的。对于每个请求，都会按照下面的顺序进行调用：
- 每个请求都会创建一个新的 `RequestHandler` 对象
- `initialize()` 会使用来自 `Application` 配置的初始化参数调用。`initialize` 通常应当将传递来的参数保存到成员变量中；它不应当产生任何输出或者调用类似 `send_error` 的方法
- `prepare()` 会被调用。这对于所有共享了同一个基类的子类非常有帮助，因为 `prepare` 会在任何
 HTTP 类型请求发生时调用。`prepare` 可能会输出；如果它调用了 `finish` 或者 `redirect` 等方法会导致请求处理直接结束
- 接下来任何一个 HTTP 方法会被调用：`get()`, `post()`, `put()` 等。如果
 URL 正则表达式中包含捕获组，他们都会被作为参赛传递到这个方法
- 当请求结束后，`on_finish()` 会被调用。对于同步 handler，该方法会在 `get()` 等方法返回后立即调用；对于异步模式，它会在调用 `finish()` 后触发
所有被设计用来重写的方法都在 [RequestHandler](http://www.tornadoweb.org/en/stable/web.html#tornado.web.RequestHandler) 文档中给出了。一些常见的重写方法包括：
- `write_error`：针对错误页面输出 HTML
- `on_connection_close`：在客户端断开连接时调用；应用可以选择检测这种情况并阻止后续处理。注意，并不能保证及时检测到关闭的连接。
- `get_current_user`：查看 [用户认证](http://www.tornadoweb.org/en/stable/guide/security.html#user-authentication)。
- `get_user_locale`：返回当前用户对应的 `locale` 对象。
- `set_default_headers`：可能用于对响应设置额外的头部（例如自定义的 `Server` 头部）。
# 错误处理
如果 handler 抛出了异常，Tornado 会调用 `RequestHandler.write_error` 产生一个错误页面。`tornado.web.HTTPError` 可用于产生一个特殊的状态码；所有其它异常都会返回
 500 状态码。
默认的错误页面会包含在调试模式下的堆栈追踪和一个单行的错误描述（如：”500: Internal Server Error”）。可以重写 `RequestHandler.write_error` 来产生自定义的错误页。该方法通常可以通过 `write` 和`render` 方法产生输出。如果错误是由于异常导致的，则会将一个 `exc_info` 元素当做关键字参数传递过来（注意该异常并不能保证就是 `sys.exc_info` 中的异常，所以 `write_error` 必须使用 `traceback.format_exception` 而非`traceback.format_exc`）。
当然也可以从常规的 handler 方法中生成一个错误页面，而非使用 `write_error` 方法，具体做法是调用`set_status`，写入响应，然后返回。当简单返回不方便时，可以通过抛出 `tornado.web.Finish` 异常来终止
 handler 而不用调用 `write_error` 方法。
对于 404 错误，使用 `default_handler_class`。这种 hanlder 应当重写 `prepare` 方法而非具体的 `get()` 等方法，这样可以对任何
 HTTP 方法起作用。它应当产生上述的错误页面：可以抛出一个 `HTTPError(404)` 异常并重写`write_error`，或者简单地调用 `self.set_status(404)` 并在 `prepare()` 中直接产生响应。
# 重定向
Tornado 中有两种主要的方法可以实现重定向：`RequeustHandler.redirect` 和 `RedirectHandler`。
你可以在 `RequestHandler` 中使用 `self.redirect()` 重定向。还有一个额外的参数 `permanent` 可以指定重定向是否是永久的。`permanent` 默认值是 `False`，这会产生 
```
302
 Found
```
 HTTP 响应状态，非常适合在用户成功 POST 后重定向的场景。如果 `permenent` 是 `True`，就会产生 
```
301
 Moved Permenently
```
 HTTP 响应，这种场景适用于将页面对应的 URL 重定向到 SEO 友好的 URL 形式。
`RedirectHandler` 允许你在 `Application` 的路由表中直接配置重定向。例如，配置一个静态的重定向：
- 
```python
app = tornado.web.Application([
```
- 
```python
url(r"/app", tornado.web.RedirectHandler,
```
- 
```python
dict(url="http://itunes.apple.com/my-app-id")),
```
- 
```python
])
```
`RedirectHanlder` 也支持正则替换。以下规则会将所有 `/pictures/` 开头的
 URL 重定向到 `/photos`：
- 
```python
app = tornado.web.Application([
```
- 
```python
url(r"/photos/(.*)", MyPhotoHandler),
```
- 
```python
url(r"/picutures/(.*)", tornado.web.RedirectHandler, dict(url=r"/photos/{0}")),
```
- 
```python
])
```
不同于 `RequestHandler.redirect`，`RedirectHandler` 默认使用永久重定向。因为路由表在运行时不会改变，所以假定为永久重定向了；而在
 handler 中定义的重定向可能是其它逻辑产生的结果并且可能会改变。若需要临时重定向，给 `RequestHandler` 添加 `permanent=False` 初始化参数。
# 异步 handler
Tornado handler 默认是同步模式：当 `get()`/`post()` 方法返回后，就认为一次请求结束了，并且响应也发送出去了。由于在处理某个请求期间其它请求都被阻塞了，任何长时间允许的
 handler 都应当是异步的，这样可以使用非阻塞的方式调用耗时操作。这个话题详情参见 [异步和非阻塞 I/O](http://www.tornadoweb.org/en/stable/guide/async.html)；本节重点探讨 `RequestHandler` 子类中使用异步技术。
让 handler 异步化最简单的方式是使用 `coroutine` 装饰器。这允许你使用 `yield` 关键字执行非阻塞
 I/O，并且在协程没有返回前响应都不会发送。详情参见 [协程](http://www.tornadoweb.org/en/stable/guide/coroutines.html) 一节。
在某些情况下，协程可能会不如回调好用。这时我们可以使用 `tornado.web.asynchronous` 装饰器。当使用该装饰器后，响应并不会自动发送；相反，请求会一直保存直至 `RequestHanlder.finish` 被回调函数调用了。这取决于应用来确保该方法是被调用还是让用户浏览器简单挂起。
以下是使用 Tornado 内建的 `AsyncHTTPClient` 来调用 `FriendFeedAPI` 的示例：
- 
```python
class MainHandler(tornado.web.RequestHandler):
```
- 
```python
@tornado.web.asynchronous
```
- 
```python
def get(self):
```
- 
```python
http = tornado.httpclient.AsyncHTTPClient()
```
- 
```python
http.fetch("http://friendfeed-api.com/v2/feed/bret", callback=self.on_response)
```
- 
```python
```
- 
```python
def on_response(self, response):
```
- 
```python
if response.error:
```
- 
```python
raise tornado.web.HTTPError(500)
```
- 
```python
json = tornado.escape.json_decode(response.body)
```
- 
```python
self.write("Fetched" + str(len(json['entries'])) + 'entries from the FriendFeed API')
```
- 
```python
self.finish()
```
当 `get()` 返回后，请求并未结束。当 HTTP 客户端最终调用 `on_response()` 时，请求依然存在，只有调用了`self.finish()` 后响应才被发送到客户端。
作为对比，以下是使用协程的方式：
- 
```python
class MainHandler(tornado.web.RequestHandler):
```
- 
```python
@tornado.gen.coroutine
```
- 
```python
def get(self):
```
- 
```python
http = tornado.httpclient.AsyncHTTPClient()
```
- 
```python
response = yield http.fetch("http://frinedfeed-api.com/v2/feed/bret")
```
- 
```python
json = tornado.escape.json_decode(response.body)
```
- 
```python
self.write("Fetched" + str(len(json['entries'])) + 'entries from the FriendFeed API')
```
对于更加高级的异步示例，参见[聊天室应用](https://github.com/tornadoweb/tornado/tree/stable/demos/chat)，它使用长轮询的方式实现了一个 AJAX 聊天室。使用长轮询的用户可能想要重写 `on_connection_close()` 方法，从而在客户端断开连接后进行清理（记得要看那个方法的文档，了解一些缺陷）。
# 参考
- [Tornado->User’s guides->Structure of a tornado web application](http://www.tornadoweb.org/en/stable/guide/structure.html#)
# 版权声明
- 本文由 [Christopher L](http://blog.chriscabin.com/) 发表，采用 [知识共享署名-非商业性使用-相同方式共享
 4.0 国际许可协议](http://creativecommons.org/licenses/by-nc-sa/4.0/) 进行许可。请确保你已了解许可协议，并在 转载 时声明。
- 本文固定链接：[http://blog.chriscabin.com/?p=1593](http://blog.chriscabin.com/?p=1593)。
