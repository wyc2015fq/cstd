# Tornado 文档学习：模板与 UI - 宇宙浪子的专栏 - CSDN博客
2017年11月16日 18:08:19[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：219
转自：[http://blog.chriscabin.com/coding-life/web-framework/tornado/1594.html](http://blog.chriscabin.com/coding-life/web-framework/tornado/1594.html)
# 引言
Tornado 内建了一个简单、快速并且灵活的目标语言。本节会描述模板语言详情，同时也涉及一些其它相关问题，如国际化。
Tornado 也可以使用其它 Python 模板语言，虽然并没有将这些系统和 `RequestHandler.render` 的相关细节介绍。只需要将模板选人成字符串然后传递给 `RequestHandler.write` 即可。
# 配置模板
默认情况下，Tornado 会在引用模板文件的 `.py` 文件相同目录下查找模板。若想自定义模板目录，可以在`Application` 配置中设置 `template_path`（或者如果你定义的不同
 handler 有不同的模板路径，可以重写`RequestHandler.get_template_path` ）
为了加载来自非文件系统的模板，可以继承 `tornado.template.BaseLoader`，然后在应用设置中传递`template_loader` 参数为自定义的 `TemplateLoader`。
默认情况下，编译后的模板会被缓存；若想关闭缓存功能，并希望模板的修改能及时起作用，请在应用设置中传递参数 `compiled_template_cache=False` 或 `debug=True`。
# 模板语法
一个 Tornado 模板就是一个 HTML 文件，其中包含 Python 风格的控制语句和表达式：
- `<html>`
- `   <head>`
- `      <title>{{ title }}</title>`
- `   </head>`
- `   <body>`
- `     <ul>`
- `       {% for item in items %}`
- `         <li>{{ escape(item) }}</li>`
- `       {% end %}`
- `     </ul>`
- `   </body>`
- ` </html>`
如果你将该模板保存为 `template.html` 文件，并将其放在你的 Python 文件所在目录，你可以使用下面的方式渲染模板：
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
items = ["Item1", "Item2"]
```
- 
```python
self.render("template.html", title="My title", items=items)
```
Tornado 模板支持多种控制语句和表达式。控制语句使用 `{%` 和 `%}` 包围，例如 
```
{%
 if len(items) > 1 %}
```
。表达式则使用 `{{` 和 `}}` 包围，例如 
```
{{
 items[0] }}
```
。
控制语句或多或少会和 Python 语句比较像。我们提供了 `if`, `for`, `while` 和 `try`，它们都使用 
```
{%
 end %}
```
 结束。我们也提供了模板继承机制，可以通过 `extends` 和 `block` 语句实现，详细信息可以参见 [tornado.template](http://www.tornadoweb.org/en/stable/template.html#module-tornado.template) 文档描述。
表达式可以是任意 Python 表达式，包括函数调用。模板代码会在一个包含下面对象和函数的命名空间中执行（注意下面的列表这对于 `RequestHandler.render` 和 `render_string` 是适用的。如果在 `RequestHandler` 外面直接使用 `tornado.template` 的话，下面很多入口都是无效的）。
- `escape`：`tornado.escape.xhtml_escape` 别名
- `xhtml_escape`：`tornado.escape.xhtml_escape` 别名
- `url_escape`：`tornado.escape.url_escape` 别名
- `json_encode`：`tornado.escape.json_encode` 别名
- `squeeze`：`tornado.escape.squeeze` 别名
- `linkify`：`tornado.escape.linkify` 别名
- `datetime`：Python 的 `datetime` 模块
- `handler`：当前的 `RequestHandler` 对象
- `request`：`handler.request` 别名
- `current_user`：`handler.current_user` 别名
- `locale`：`handler.locale` 别名
- `_`：`handler.locale.translate` 别名
- `static_url`：`handler.static_url` 别名
- `xsrf_form_html`：`handler.xsrf_form_html` 别名
- `reverse_url`：`Application.reverse_url` 别名
- 所有来自 `ui_methods` 的接口以及对应的应用配置
- 任何传递到 `render` 或者 `render_string` 中的关键字参数
当你在构建真实的应用时，就可能想要使用 Tornado 模板的所有特性，尤其是模板继承。请阅读[tornado.template](http://www.tornadoweb.org/en/stable/template.html#module-tornado.template) 文档学习所有特性（一些功能，包括 `UIModules` 是在 `tornado.web` 模块中实现的）。
Tornado 模板会在背后被翻译成 Python 代码。你在模板中使用的表达式会被逐字地复制到一个用于表示你的模板的函数中。我们不会尝试阻止任何编写在模板中的代码；和其它严格的模板系统相比，Tornado 模板系统更加灵活。因此，如果你在模板表达式中编写了随意的代码，最终可可能会在执行模板时产生随机的 Python 错误。
所有的模板输出默认都会使用 `tornado.escape.xhtml_escape` 函数转义。可以全局修改该行为，只需要给`Application` 或者 `tornado.template.Loader` 构造函数传递 `autoescape=None` 即可。对于单个模板文件，我们可以用 
```
{%
 autoescape None %}
```
。对于单个表达式，可以使用 `{% raw ... %}` 代替 `{{...}}` 。此外，可以在这些地方使用某种可用的转义函数而非 `None`。
需要注意的是，虽然 Tornado 的自动转义对于避免 XSS 攻击有所帮助，但并非所有情况下都是满足要求的。对于出现在某些地方（如 JavaScript 或 CSS）的表达式，还是需要额外转义的。此外，我们需要注意应总是使用双引号；此外，HTML 属性中也可能包含不可信的内容，在使用 `xhtml_escape` 需要注意，或者对于属性也必须使用额外的转义函数（参见 [http://wonko.com/post/html-escaping](http://wonko.com/post/html-escaping)）。
# 国际化
当前用户（无论是否登录）的地域总是可以在请求 handler 中访问 `self.locale` 和模板中的 `locale` 中获取。可以使用 `locale.name` 获取名称，你可以使用 `Locale.translate` 方法翻译字符串。模板中提供了全局的函数 `_()` 用于翻译字符。翻译函数有两种形式：
- 
```python
_("Translate this string")
```
会将字符串直接根据当前地域翻译出来，另外一种形式：
- 
```python
_("A person liked this", "%(num)d people liked this",
```
- 
```python
len(people)) % {"num": len(people)}
```
它会根据第三个参数值的单复数形式进行翻译。上述例子中，当 `len(people) == 1` 时，会返回 “A person liked
 this”，否则返回第二个字符串。
翻译中最常用的模式就是对变量使用 Python 的命名占位符，因为占位符可以在翻译时移动。
以下是一个国际化模板示例：
- 
```python
<html>
```
- 
```python
<head>
```
- 
```python
<title>FriendFeed - {{ _("Sign in") }}</title>
```
- 
```python
</head>
```
- 
```python
<body>
```
- 
```python
<form action="{{ request.path }}" method="post">
```
- 
```python
<div>{{ _("Username") }} <input type="text" name="username"/></div>
```
- 
```python
<div>{{ _("Password") }} <input type="password" name="password"/></div>
```
- 
```python
<div><input type="submit" value="{{ _("Sign in") }}"/></div>
```
- 
```python
{% module xsrf_form_html() %}
```
- 
```python
</form>
```
- 
```python
</body>
```
- 
```python
</html>
```
默认情况下，我们会使用用户浏览器发送的头部 `Accept-Language` 检测用户地域.如果没有找到合适的`Accept-Language` 值，就使用 `en_US`。如果你允许用户在偏好设置中改变 `locale`，你可以重写`RequestHandler.get_user_locale` 来改变默认的地域选择：
- 
```python
class BaseHandler(RequestHandler):
```
- 
```python
def get_current_user(self):
```
- 
```python
user_id = self.get_secure_cookie("user")
```
- 
```python
if not user_id:
```
- 
```python
return None
```
- 
```python
```
- 
```python
return self.backend.get_user_by_id(user_id)
```
- 
```python
```
- 
```python
def get_user_locale(self):
```
- 
```python
if "locale" not in self.current_user.prefs:
```
- 
```python
# 使用 Accept-Language 值
```
- 
```python
return None
```
- 
```python
return self.current_user.prefs["locale"]
```
- 
```python
```
如果 `get_user_locale` 返回 `None`，我们会使用 `Accept-Language` 头部值。
`tornado.locale` 木块支持两种格式的翻译文件：使用 `gettext` 和相关工具创建的 `.mo` 格式，简单的 `.csv` 格式。应用通常会在启动时调用 [tornado.locale.load_translations](http://www.tornadoweb.org/en/stable/locale.html#tornado.locale.load_translations) 或者[tornado.locale.load_gettext_translations](http://www.tornadoweb.org/en/stable/locale.html#tornado.locale.load_translations) 加载翻译文件。
你可以在应用中调用 `tornado.locale.get_supported_locales()` 获得支持的地域。可以根据支持的地域为用户选择最接近的那个。例如，如果用户的地域是 `es_GT`，并且 `es` 地域是支持的，则请求的 `self.locale` 就是 `es`。如果没有匹配，则使用 `en_US`。
# UI 模块
Tornado 支持使用 UI 模块，方便为应用提供标准、可重用的组件。UI 模块就像很特殊的函数调用，从而在页面上渲染组件，这些组件可以打包成独立的包，拥有自己的 CSS 和 JavaScript。
例如，当你在实现博客时，你希望让博客入口同时出现在主页和每篇博客中，你可以创建一个 `Entry` 模块，从而在相应页面上渲染它们。首先，为你的
 UI 模块创建一个 Python 模块，如 `uimodules.py`：
- 
```python
class Entry(tornado.web.UIModules):
```
- 
```python
def render(self, entry, show_comments=False):
```
- 
```python
return self.render_string(
```
- 
```python
"module-entry.html", entry=entry,
```
- 
```python
show_comments=show_comments
```
- 
```python
)
```
在应用中设置 `ui_modules` 让 Tornado 使用 `uimodules.py`：
- 
```python
from . import uimodules
```
- 
```python
```
- 
```python
class HomeHandler(RequestHanlder):
```
- 
```python
def get(self):
```
- 
```python
entries = self.db.query("SELECT * FROM entries ORDER BY date DESC")
```
- 
```python
self.render("entry.html", entries=entries)
```
- 
```python
```
- 
```python
class EntryHandler(RequestHandler):
```
- 
```python
def get(self, entry_id):
```
- 
```python
entry = self.db.get("SELECT * FROM entries WHERE id = %s", entry_id)
```
- 
```python
if not entry:
```
- 
```python
raise HTTPError(404)
```
- 
```python
self.render("render.html", entry=entry)
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
"ui_modules": uimodules
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
app = tornado.web.Application([
```
- 
```python
(r"/", HomeHandler),
```
- 
```python
(r"/entry/([0-9+])", EntryHandler)
```
- 
```python
], **settings)
```
在模板中，你可以使用 `{% module %}` 调用一个模块。例如，你可以在 `home.html` 调用 `Entry` 模块：
- 
```python
{% for entry in entries %}
```
- 
```python
{% module Entry(entry) %}
```
- 
```python
{% end %}
```
也可以在 `entry.html`：
- 
```python
{% module Entry(entry, show_comments=True) %}
```
模块可以包含自定义的 CSS 和 JavaScript，可通过重写 `embedded_css`, `embedded_javascript`,`javascript_files`, `css_files` 方法实现：
- 
```python
class Entry(tornado.web.UIModule):
```
- 
```python
def embedded_css(self):
```
- 
```python
return ".entry { margin-bottom: 1em; }"
```
无论模块在页面中使用多少次，模块的 CSS 和 JavaScript 都只会被包含一次。CSS 总会包含在页面的`<head>` 中，而 JavaScript
 则总会被包含在页面 `</body>` 标签刚结束的位置之前。
当无需额外的 Python 代码时，模板文件自身可被作为一个模块。例如，前面介绍的例子可以被重写并放到`module-entry.html`：
- `{{ set_resources(embedded_css=".entry { margin-bottom: 1em; }") }}`
- `<!-- more template html... -->`
这个模板模块可以用下面的方式调用：
- `{% module Template("module-entry.html", show_comments=True) %}`
`set_resources` 函数只用在通过 
```
{%
 module Template(...) %}
```
 调用时才起作用。和 `{% include ... %}` 不同的是，模板模块拥有来自它们所包含的模板的独特命名空间——它们只能看见全局模板命名空间和它们自己的关键字参数。
# 参考
- [Tornado->User’s guide->Template and UI](http://www.tornadoweb.org/en/stable/guide/templates.html)
# 版权声明
- 本文由 [Christopher L](http://blog.chriscabin.com/) 发表，采用 [知识共享署名-非商业性使用-相同方式共享
 4.0 国际许可协议](http://creativecommons.org/licenses/by-nc-sa/4.0/) 进行许可。请确保你已了解许可协议，并在 转载 时声明。
- 本文固定链接：[http://blog.chriscabin.com/?p=1594](http://blog.chriscabin.com/?p=1594)。
