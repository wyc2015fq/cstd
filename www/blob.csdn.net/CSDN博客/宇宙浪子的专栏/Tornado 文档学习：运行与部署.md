# Tornado 文档学习：运行与部署 - 宇宙浪子的专栏 - CSDN博客
2017年11月16日 18:21:06[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：337
转自：[http://blog.chriscabin.com/coding-life/web-framework/tornado/1596.html](http://blog.chriscabin.com/coding-life/web-framework/tornado/1596.html)
# 引言
由于 Tornado 提供了自己的 HTTPServer，运行与部署它会与其它 Python web 框架编写的应用有所不同。和配置一个 WSGI 容器查找你的应用不同的是，Tornado 应用中可以编写一个 `main()` 函数直接启动服务器：
- 
```python
def main():
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
IOLoop.current().start()
```
- 
```python
```
- 
```python
if __name__ == '__main__':
```
- 
```python
main()
```
配置你的操作系统或者进程管理器运行该程序从而启动服务器。此外，需要注意的是增加每个进程可打开的文件数量（避免 “Too many open files” 错误）。要想提高限制（比如设置为 50,000），你可以使用 `ulimit`命令，修改 `/etc/security/limits.conf` 或者在
 supervisord 配置中设置 `minfds`。
# 进程与端口
由于 Python GIL 的问题，有必要运行多个进程充分利用多核服务器。典型的做法是每个 CPU 都运行一个进程。
Tornado 内建了启动多进程的模式，只需要做少许改动即可：
- 
```python
def main():
```
- 
```python
app = make_app()
```
- 
```python
server = tornado.httpserver.HTTPServer(app)
```
- 
```python
server.bind(8888)
```
- 
```python
server.start(0)  # 为每个 CPU fork 出一个进程
```
- 
```python
IOLoop.current().start()
```
虽然上述做法有些缺陷，但它的确是一次启动多个进程并且共享端口的最简单的方法了。首先，每个子进程都有自己的 IOLoop，所以在 fork 前不要直接或间接触碰全局的 IOLoop 是非常重要的。第二，该模式下很难做到零宕机时间更新。最后，由于多个进程共享端口，所以分别监控它们会变得更加困难。
对于更加复杂的部署方案，推荐单独启动多个进程，并且让每个进程监听不同的端口。[supervisord](http://www.supervisord.org/) 的进程组特性就非常适合管理这种情况。当每个进程都使用不同的端口后，就需要一个负载均衡器（HAProxy 或者 nginx）来对外提供单一的访问地址。
# 在负载均衡器后面运行
当在类似 nginx 的负载均衡器下运行时，推荐传递 `xheaders=True` 参数到 `HTTPServer` 构造器。这样
 Tornado 可以通过 `X-Real-IP` 类似的头部获取用户的 IP 地址，从而避免将所有的流量都打到负载均衡器上。
以下是一个 nginx 配置骨架，和在 FriendFeed 使用的类似。它假设 nginx 和 Tornado 服务器运行在相同的机器上，并且 4 个 Tornado 服务器运行在 8000~8003 端口：
- `user nginx;`
- `worker_processes 1;`
- ``
- `error_log /var/log/nginx/error.log;`
- `pid /var/run/nginx.pid;`
- ``
- `events {`
- `    worker_connections 1024;`
- `    use epoll;`
- `}`
- ``
- `http {`
- `    # Enumerate all the Tornado servers here`
- `    upstream frontends {`
- `        server 127.0.0.1:8000;`
- `        server 127.0.0.1:8001;`
- `        server 127.0.0.1:8002;`
- `        server 127.0.0.1:8003;`
- `    }`
- ``
- `    include /etc/nginx/mime.types;`
- `    default_type application/octet-stream;`
- ``
- `    access_log /var/log/nginx/access.log;`
- ``
- `    keepalive_timeout 65;`
- `    proxy_read_timeout 200;`
- `    sendfile on;`
- `    tcp_nopush on;`
- `    tcp_nodelay on;`
- `    gzip on;`
- `    gzip_min_length 1000;`
- `    gzip_proxied any;`
- `    gzip_types text/plain text/html text/css text/xml`
- `               application/x-javascript application/xml`
- `               application/atom+xml text/javascript;`
- ``
- `    # Only retry if there was a communication error, not a timeout`
- `    # on the Tornado server (to avoid propagating "queries of death"`
- `    # to all frontends)`
- `    proxy_next_upstream error;`
- ``
- `    server {`
- `        listen 80;`
- ``
- `        # Allow file uploads`
- `        client_max_body_size 50M;`
- ``
- `        location ^~ /static/ {`
- `            root /var/www;`
- `            if ($query_string) {`
- `                expires max;`
- `            }`
- `        }`
- `        location = /favicon.ico {`
- `            rewrite (.*) /static/favicon.ico;`
- `        }`
- `        location = /robots.txt {`
- `            rewrite (.*) /static/robots.txt;`
- `        }`
- ``
- `        location / {`
- `            proxy_pass_header Server;`
- `            proxy_set_header Host $http_host;`
- `            proxy_redirect off;`
- `            proxy_set_header X-Real-IP $remote_addr;`
- `            proxy_set_header X-Scheme $scheme;`
- `            proxy_pass http://frontends;`
- `        }`
- `    }`
- `}`
# 静态文件和激进文件缓存
你可以在 Tornado 中配置 `static_path` 为静态文件提供访问服务：
- 
```python
settings = {
```
- 
```python
"static_path": os.path.join(os.path.dirname(__file__), "static"),
```
- 
```python
"cookie_secret": "__TODO:_GENERATE_YOUR_OWN_RANDOM_VALUE_HERE__",
```
- 
```python
"login_url": "/login",
```
- 
```python
"xsrf_cookies": True,
```
- 
```python
}
```
- 
```python
application = tornado.web.Application([
```
- 
```python
(r"/", MainHandler),
```
- 
```python
(r"/login", LoginHandler),
```
- 
```python
(r"/(apple-touch-icon\.png)", tornado.web.StaticFileHandler,
```
- 
```python
dict(path=settings['static_path'])),
```
- 
```python
], **settings)
```
上述设置会让所有前缀为 `/static/` 的请求访问 `static` 目录，例如 `http://localhost:8888/static/foo.png` 会在特殊的静态文件目录中提供 `foo.png` 的访问服务。同时，`/robots.txt` 和 `/favicon.ico` 也会由静态文件目录自动提供访问服务（即便其没有 `/static/` 前缀）。
在上面的配置中，我们使用 `StaticFileHandler` 显式地配置 Tornado 直接从根访问 `apple-touch-icon.png`，虽然它在物理层面是存储在静态文件目录的。正则表达式的捕获组会告诉 `StaticFileHandler` 请求的文件名。你也可以用同样的方式在网站的根下提供对 `sitemap.xml` 的访问。当然，你也可以在
 HTML 中使用合适的 `<link />`标签来避免仿造一个根 `app-touch-icon.png` 文件。
为了提高性能，通常可以使用浏览器缓存静态资源，这样就不必发送大量的 `If-Modified-Since` 或者 `Etag` 请求从而导致页面渲染中阻塞问题的发生。Tornado
 使用静态内容版本的方式提供了缓存支持。
为了使用这个特性，可以在 HTML 中使用 `static_url` 代替静态文件的 URL：
- `<html>`
- `   <head>`
- `      <title>FriendFeed - {{ _("Home") }}</title>`
- `   </head>`
- `   <body>`
- `     <div><img src="{{ static_url("images/logo.png") }}"/></div>`
- `   </body>`
- ` </html>`
`static_url()` 函数会把相对路径翻译成资源的绝对路径，如 `/static/images/logo.png?v=aae54`。参数 `v` 是`logo.pn` 文件的哈希值，它的存在会让
 Tornado 服务器向浏览器发送缓存头部，从而让浏览器将内容缓存下来。
由于参数 `v` 和文件内容有关，一旦你更新了文件并重启服务器后，它会发送一个新的 `v` 值，这样浏览器会自动访问新的文件。如果文件内容没有改变，浏览器会继续使用本地缓存的文件，从而大幅度提高渲染的效率。
在生产环境中，你可能想要用更好的文件服务器如 nginx 提供静态文件服务。你可以配置几乎所有的 Web 服务器识别 `static_url()` 发送的版本标签，从而设置相应的缓存头部。以下是我们在
 FriendFeed 使用 nginx 的配置：
- `location /static/ {`
- `    root /var/friendfeed/static;`
- `    if ($query_string) {`
- `        expires max;`
- `    }`
- ` }`
# 调试模式和自动重载
如果在 `Application` 初始化时传递 `debug=True`，应用汇运行在调试/开发模式。在该模式下，便于开发的一些特性将会启用（以下这下也可以使独立的标志；如果两个都设置了，则独立的标志优先级更高）：
- `autoreload=True`：应用汇监控源文件的变化，并在任何更改发生时重新加载。这样可以减少手动重启服务器的次数。然而，某些错误（如语法错误等）仍然会导致在调试模式下无法恢复。
- `compiled_template_cache=False`：模板不会缓存。
- `static_hash_cache=False`：静态文件哈希（用于 `static_url` 函数）不会被缓存。
- `serve_traceback=True`：当发生在 `RequestHandler` 中的异常未捕获时，会产生一个错误页面，包含详细的堆栈追踪信息。
自动重载模式不能在多进程模式的 `HTTPServer` 下使用。如果需要使用该模式的话，就不能给 `HTTPServer.start`传递除了
 1 以外的参数（或者调用 `tornado.process.fork_process`）。
调试模式下的自动重载功能可以由 `tornado.autoreload` 这个独立模块提供。组合使用这两个特性可以提供额外的鲁棒性：在 App
 中设置 `autoreload=True`，从而在运行时检测变化，然后可以使用 
```
python
 -m tornado.autoreload myserver.py
```
 捕获启动时发生的语法错误等。
重载会导致 Python 解释器命令行参数丢失（如 `-u`），因为它使用 `sys.executable` 和 `sys.argv` 重新执行了
 Python。此外，修改这些参数也会导致重载失败。
在某些平台上（包括 Windows 和 Mac OSX 10.6 之前的版本），不能原地更新处理；因此，一旦代码变更被检测到后，旧的服务器仍然存在，但也启动了一个新的服务器。这对某些 IDE 来说非常困惑。
# WSGI 和 GAE
Tornado 可以在没有 WSGI 容器的情况下独立运行。然而，在某些环境下（如 Google App Engine），只允许 WSGI 应用允许，应用不能运行自己的服务器。对于这种情况，Tornado 提供了一个阉割模式，该模式提供了针对 WSGI 环境下有限的 Tornado 功能，并且不支持异步操作。在 WSGI 模式下，诸如协程、`@asynchronous` 装饰器，`AsyncHTTPClient` 和 `auth` 模块以及 `WebSockets` 都无法使用。
你可以使用 `tornado.wsgi.WSGIAdapter` 将一个 Tornado 应用转换成 WSGI 应用。示例如下：
- 
```python
import tornado.web
```
- 
```python
import tornado.wsgi
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
tornado_app = tornado.web.Application([
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
app = tornado.wsgi.WSGIAdapter(tornado_app)
```
完整的功能可以查看 [appengine 示例应用](https://github.com/tornadoweb/tornado/tree/stable/demos/appengine)。
# 参考
- [Running and deploying](http://www.tornadoweb.org/en/stable/guide/running.html)
# 版权声明
- 本文由 [Christopher L](http://blog.chriscabin.com/) 发表，采用 [知识共享署名-非商业性使用-相同方式共享
 4.0 国际许可协议](http://creativecommons.org/licenses/by-nc-sa/4.0/) 进行许可。请确保你已了解许可协议，并在 转载 时声明。
- 本文固定链接：[http://blog.chriscabin.com/?p=1596](http://blog.chriscabin.com/?p=1596)。
