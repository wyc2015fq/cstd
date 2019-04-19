# Tornado 文档学习：认证与安全 - 宇宙浪子的专栏 - CSDN博客
2017年11月16日 18:23:01[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：129标签：[python																[tornado](https://so.csdn.net/so/search/s.do?q=tornado&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python																[tornado](https://blog.csdn.net/bluehawksky/article/category/7102850)](https://blog.csdn.net/bluehawksky/article/category/2542577)
转自：
# Cookies 和安全 Cookies
你可以使用 `set_cookie` 方法设置用户浏览器的 cookies：
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
if not self.get_cookie("mycookie"):
```
- 
```python
self.set_cookie("mycookie", "myvalue")
```
- 
```python
self.write("Your cookie was not set yet!")
```
- 
```python
else:
```
- 
```python
self.write("Your cookie was set!")
```
Cookies 不太安全，很容易被客户端修改。如果你需要将 cookie 设置为例如识别当前登录用户的信息的话，你需要给你的 cookies 签名防止伪造。Tornado 支持签名 cookies，你可以使用 [set_secure_cookie](http://www.tornadoweb.org/en/stable/web.html#tornado.web.RequestHandler.set_secure_cookie) 和[get_secure_cookie](http://www.tornadoweb.org/en/stable/web.html#tornado.web.RequestHandler.get_secure_cookie) 方法。为了使用这些方法，你需要在创建应用时指定 `cookie_secret` 密钥。你可以用关键字参数的方式进行设置：
- 
```python
app = tornado.web.Application([
```
- 
```python
(r"/", MainHandler),
```
- 
```python
], cookie_secret="__TODO:__GENERATE_YOUR_OWN_RANDOM_VALUE_HERE__")
```
签名 cookie 除了包含编码过的 cookie 值，还有一个时间戳和一个 [HMAC](http://en.wikipedia.org/wiki/HMAC) 签名。如果 cookie 过期或者签名匹配失败，`get_secure_cookie` 会直接返回 `None` 表示
 cookie 并没有设定。安全的版本如下：
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
if not self.get_secure_cookie("mycookie"):
```
- 
```python
self.set_secure_cookie("mycookie", "myvalue")
```
- 
```python
self.write("Your cookie was not set yet!")
```
- 
```python
else:
```
- 
```python
self.write("Your cookie was not set!")
```
Tornado 的安全 cookie 可以保证完整性，但不能保护机密。也就是说，cookie 本身是不能被篡改的，但是会被用户看到。`cookie_secret` 是一个对称密钥，所以需要保护好——任何拥有这个密钥的人都可以生成符合签名的
 cookies。
默认情况下，Tornado 安全 cookies 30 天后过期。可以在调用 `set_secure_cookie` 时传递 `expire_days` 参数或者在调用 `get_secure_cookie` 时传递 `max_age_days` 参数修改。采用这两种分开的设置方式，在大多数情况下，更长的时间符合需求，但对于某些特殊场景（如更改账户信息），你可以使用一个较短的 `max_age_days`来读取
 cookies。
Tornado 也支持多个签名密钥，从而实现密钥轮转。`cookie_secret` 就必须是一个字典，其键是一个表示版本的整数，值则是相应的密钥。当前使用的签名密钥必须要设定为 `key_version`，而其它在字典中的密钥都可以用来签名验证，前提是在
 cookie 中设定了正确的密钥版本。为了实现 cookies 更新，可以使用`get_secure_cookie_key_version` 获取当前签名密钥版本。
# 用户认证
当前认证用户可以在请求 handler 中使用 `self.current_user` 访问，在模板中用 `current_user` 访问。默认情况下`current_user` 是 `None`。
为了实现用户认证，你需要在请求 handler 中重写 `get_current_handler()` 方法，从而基于 cookie 值验证当前用户身份。下面是用户使用指定的用户名登录，然后记录到
 cookie 中的示例：
- 
```python
class BaseHandler(tornado.web.RequestHandler):
```
- 
```python
def get_current_user(self):
```
- 
```python
return self.get_secure_cookie("user")
```
- 
```python
```
- 
```python
class MainHandler(BaseHandler):
```
- 
```python
def get(self):
```
- 
```python
if not self.current_user:
```
- 
```python
return self.redirect("/login")
```
- 
```python
return
```
- 
```python
name = tornado.escape.xhtml_escape(self.current_user)
```
- 
```python
self.write("Hello, " + name)
```
- 
```python
```
- 
```python
class LoginHandler(BaseHandler):
```
- 
```python
def get(self):
```
- 
```python
self.write('<html><body><form action="/login" method="post">'
```
- 
```python
'Name: <input type="text" name="name">'
```
- 
```python
'<input type="submit" value="Sign in">'
```
- 
```python
'</form></body></html>')
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
self.set_secure_cookie("user", self.get_argument("username"))
```
- 
```python
self.redirect("/")
```
- 
```python
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
], cookie_secret="__TODO:_GENERATE_YOUR_OWN_RANDOM_VALUE_HERE__")
```
你可以使用 [tornado.web.authenticated](http://www.tornadoweb.org/en/stable/web.html#tornado.web.authenticated) 装饰器要求用户登录。如果一个请求对应的方法使用了该装饰器，当用户没有登录时，会自动定向到 `login_url`（另外一个用户设置）。示例如下：
- 
```python
class MainHandler(BaseHandler):
```
- 
```python
@tornado.web.authenticated
```
- 
```python
def get(self):
```
- 
```python
name = tornado.escape.xhtml_escape(self.current_user)
```
- 
```python
self.write("Hello, " + name)
```
- 
```python
```
- 
```python
settings = {
```
- 
```python
"cookie_secret": "_GENERATE_YOUR_OWN_RANDOM_VALUE_HERE__",
```
- 
```python
"login_url": "/login"
```
- 
```python
}
```
- 
```python
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
(r"/login", LoginHandler)
```
- 
```python
], **settings)
```
如果在 `post()` 方法上使用了 `authenticated` 装饰器，并且用户并未登录，则服务器会发送一个
 403 响应。`@authenticated` 装饰器只是 
```
if
 not self.current_user: self.redirect()
```
 简写，并且对不是基于浏览器登录的方案可能不适合。
详细查看 [Tornado 博客应用](https://github.com/tornadoweb/tornado/tree/stable/demos/blog)，该项目展示了用户认证（并将用户数据存储在 MySQL 数据库）。
# 第三方认证
`tornado.auth` 模块实现了一些列非常流行的网站的认证和授权协议，包括：Google/Gmail, Facebook, Twitter
 和 FriendFeed。该模块包含了可以通过这些网站登录用户的方法，以及可以授权用户访问一些特权服务，如下载用户地址簿或者发布 Twitter 消息。
以下是一个使用 Google 认证的示例 handler，它将 Google 身份信息存储在 cookie 中用于后期访问：
- 
```python
class GoogleOAuth2LoginHandler(tornado.web.RequestHandler, tornado.auth.GoogleOAuth2Mixin):
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
if self.get_argument("code", False):
```
- 
```python
user = yield self.get_authenticated_user(redirect_uri="http://your.site.com/auth/google",
```
- 
```python
code=self.get_argument("code"))
```
- 
```python
# Save the user with `set_secure_cookie`
```
- 
```python
else:
```
- 
```python
yield self.authorize_redirect(
```
- 
```python
redirect_uri="http://your.site.com/auth/google",
```
- 
```python
client_id=self.settings['google_oauth']['key'],
```
- 
```python
scope=['profile', 'email'],
```
- 
```python
response_type='code',
```
- 
```python
extra_params={'approval_prompt': 'auto'}
```
- 
```python
)
```
详情参见 [tornado.auth](http://www.tornadoweb.org/en/stable/auth.html#module-tornado.auth) 模块文档。
# CSRF 保护
跨站请求伪造（Cross-site request forgery, CSRF）对于个性化网站是一个常见的问题。详情查看[Wikipedia](http://en.wikipedia.org/wiki/Cross-site_request_forgery) 的介绍。
通常采用的预防措施是在用户 cookie 中存放一个不可预测的值，并且在每次提交时都将那个值作为额外的参数传递。如果 cookie 和表单中的值不一致，则请求有可能是伪造的。
Tornado 内建了 XSRF 保护。只需要在设置中启用 `xsrf_cookies` 即可：
- 
```python
settings = {
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
], **settings)
```
如果启用了 `xsrf_cookies`，Tornado web 应用会给所有用户设置 `_xsrf` cookie，并会拒绝不含正确 `_xsrf` 值的`POST`, `PUT` 和 `DELETE` 请求。此外，需要给所有通过 `POST` 提交的表单
 HTML 中添加 XSRF 相关元素。你可以使用 `UIModule` 的 `xsrf_form_html()` 模块，它在所有的模板中都可以使用：
- `<form action="/new_message" method="post">`
- `    {% module xsrf_form_html() %}`
- `    <input type="text" name="message"/>`
- `    <input type="submit" value="post"/>`
- `</form>`
如果你使用 AJAX 提交，也需要 JavaScript 在每次请求中包含 `_xsrf` 值。下面是在 FriendFeed 使用 AJAX`POST` 请求时自动添加 `_xsrf` 值的示例：
- 
```java
function getCookie(name) {
```
- 
```java
var r = document.cookie.match("\\b" + name + "=([^;]*)\\b");
```
- 
```java
return r ? r[1] : undefined
```
- 
```java
}
```
- 
```java
```
- 
```java
jQuery.postJSON = function(url, args, callback) {
```
- 
```java
args._xsrf = getCookie("_xsrf");
```
- 
```java
$.ajax({url: url, data: $.param(args), dataType: "text", type: "POST", success: function(response) {
```
- 
```java
callback(eval("(" + response + ")"));
```
- 
```java
}});
```
- 
```java
};
```
对于 `PUT` 和 `DELETE` 请求（或者不使用编码参数的表单的 `POST` 请求），可以通过
 HTTP 头部 `X-XSRFToken`传递 XSRF token。通常当 `xsrf_form_html` 使用时，XSRF
 cookie 会被设置，但是在纯 JavaScript 应用中，你可能需要手动访问 `self.xsrf_token`（只需要读取该属性就可以设置
 cookie 了）。
如果需要自定义 XSRF 行为，你可以重写 `RequestHandler.check_xsrf_cookie()`。例如，如果你的 API
 不使用 cookies 进行认证，你可能会让 `check_xsrf_cookie()` 什么都不做从而关闭 XSRF 保护。然而，如果你需要同时支持
 cookie 和非 cookie 认证方式，非常重要的一点是无论当前请求是否含有 cookie，都应当启用 XSRF 保护。
# 参考
- [Tornado->User’s guide->Authentication and security](http://www.tornadoweb.org/en/stable/guide/security.html)
# 版权声明
- 本文由 [Christopher L](http://blog.chriscabin.com/) 发表，采用 [知识共享署名-非商业性使用-相同方式共享
 4.0 国际许可协议](http://creativecommons.org/licenses/by-nc-sa/4.0/) 进行许可。请确保你已了解许可协议，并在 转载 时声明。
- 本文固定链接：[http://blog.chriscabin.com/?p=1595.html](http://blog.chriscabin.com/?p=1595.html)。
