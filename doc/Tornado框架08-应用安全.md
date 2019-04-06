# Tornado框架08-应用安全

 

# 01-Cookie

## 普通cookie

**1.设置**

原型

```
self.set_cookie(name,value,domain=None,expires=None,path="/",expires_days=None,**kwargs)1
```

参数

| 属性         | 作用                                                         |
| ------------ | ------------------------------------------------------------ |
| name         | cookie的名称                                                 |
| value        | cookie值                                                     |
| domain       | 提交cookie时匹配的域名                                       |
| path         | 提交cookie时匹配的路径                                       |
| expires      | 设置cookie的有效期，可以是时间戳、事件元组、或者datetime类型对象。 |
| expires_days | 设置cookie的有效期天数，优先级低于expries                    |

实例

```
class SetpCookieHandler(RequestHandler):
    def get(self, *args, **kwargs):
        self.set_cookie("sunck","good")
        self.set_header("Set-Cookie", "kaige=nice; Path=/")
        self.write("ok")12345
```

**2.原理**

设置cookie实际上是通过该设置header的Set-Cookie来实现的

**3.获取**

原型：

```
self.get_cookie(name,default=None)1
```

作用

```
获取名为name的cookie的值，如果name不存在且设置了default属性，则返回default的值1
```

实例

```
class GetCookieHandler(RequestHandler):
    def get(self, *args, **kwargs):
        cookie = self.get_cookie("sunck")
        self.write(cookie)1234
```

**4.清除**

清除名为name，并同时匹配domain和path的cookie

```
self.clear_cookie(name,path="/",domain=None)1
```

清除同时匹配domino的和path的cookie

```
self.clear_all_cookies(path='/',domain=None)1
```

注意：

执行清除操作后，并不是立即删除浏览器中的cookie，而是给cookie值设置为空，并改变其有效期为失效。真正的删除是有浏览器去管理的

## 安全cookie

**1.概述**

Cookie是存储在客户端浏览器上，很容易被篡改。

Tornado提供了一种相对于安全的方案，对cookie进行简易加密(用签名的方法)，防止cookie被随意的篡改

**2.设置**

需要配置一个用来给Cookie进行混淆的秘钥

```
"cookie_secret":"HTqWqcJpRA6Js2kPEQVIy9CvgddvPk1RhmsQMGmqzC8="1
```

设置一个带签名和时间戳的cookie

```
self.set_secure_cookie(name,value,expires_days=30,version=None,**kwargs)1
```

实例

```
class SetsCookieHandler(RequestHandler):
    def get(self, *args, **kwargs):
        self.set_secure_cookie("liudehua","buru sunck handsome")
        self.write("OK")1234
```

**3.值**

```
"2|1:0|10:1508307760|8:liudehua|28:YnVydSBzdW5jayBoYW5kc29tZQ==|057d78aa76d44b47636b96b7925323238a14778b1216e78297613689e3a47ac0"1
```

安全cookie的版本，默认使用的第2版 
 默认为0 
 时间戳 
 cookie名 
 base64编码的cookie值 
 签名值，不带长度前缀

**4.获取**

```
self.get_secure_cookie(name=,value=None,max_age_days=31,min_version=None)1
```

功能：

```
如果cookie存在且验证通过，返回cookie的值，否则返回None1
```

参数：不同于expries_days，max_age_days是过滤安全cookie的时间戳

```
max_age_days1
```

实例

```
class GetsCookieHandler(RequestHandler):
    def get(self, *args, **kwargs):
        cookie = self.get_secure_cookie("liudehua")
        self.write(cookie)1234
```

# 02-XSRF

## 跨站请求伪造

当访问“搞事情”网站时，在我们不知道的情况下“计数器”网站的cookie被使用。以至于让服务器认为是“计数器”在访问

在这里使用的是GET方式模拟的跨站攻击，为了防范跨站攻击，任何会产生副作用的HTTP请求(点击添加购物车、编辑用户信息、修改密码等操作)，都是用POST请求。有保护POST请求的策略

```
class CookieNumHandler(RequestHandler):
    def get(self, *args, **kwargs):
        count = self.get_cookie("count", "0")
        count = str(int(count) + 1)
        self.set_cookie("count", count)
        self.render('cookienum.html', count=count)123456
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>搞事情</title>
</head>
<body>
    <img src="http://127.0.0.1:8000/cookienum" alt="">
    <h1>我在搞事情，去看看吧</h1>
</body>
</html>1234567891011
```

## XSRF保护

同源策略：所谓同源是指，域名，协议，端口相同。 不同源的客户端脚本(javascript、ActionScript)在没明确授权的情况下，不能读写对方的资源。 
 由于第三方站点没有访问cookie数据的权限（同源策略），所以我们可以要求每个请求包括一个特定的参数值作为令牌来匹配存储在cookie中的对应值，如果两者匹配，我们的应用认定请求有效。而第三方站点无法在请求中包含令牌cookie值，这就有效地防止了不可信网站发送未授权的请求。

## 开启XSRF

在配置中添加‘xsrf_cookie’:True

```
"xsrf_cookie":True,1
```

## 应用

模板中： 
 为浏览器设置的_xsrf的cookie（注意：该cookie会在浏览器关闭后失效）

为模板的表达式中添加了一个隐藏的输入域，name属性为_xsrf，value属性为名为_xsrf的cookie的值

```
<body>
    <form action="/postfile" method="post">
        {% module xsrf_form_html() %}
        姓名：<input type="text" name="username"/>
        <hr/>
        密码：<input type="password" name="passwd"/>
        <input type="submit" value="登陆"/>
    </form>
</body>123456789
```

非模板中

```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
    <script type="text/javascript" charset="utf-8" src="{{static_url('js/jquery.min.js')}}"></script>
</head>
<body>
    姓名：<input id="name" type="text" name="username"/>
    <hr/>
    密码：<input id="pass" type="password" name="passwd"/>
    <button onclick="login()">登陆</button>

    <script>
        function getCookie(name){
            var cook = document.cookie.match("\\b"+name+"=([^;]*\\b)")
            return cook ? cook[1] : undefined
        }
        function login(){
            $.post("/postfile", "_xsrf=" + getCookie("_xsrf")+"&username=sunck&passwd=123", function(data){
                alert("OK")
            })
        }
    </script>
</body>
</html>1234567891011121314151617181920212223242526
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
    <script type="text/javascript" charset="utf-8" src="{{static_url('js/jquery.min.js')}}"></script>
</head>
<body>
    姓名：<input id="name" type="text" name="username"/>
    <hr/>
    密码：<input id="pass" type="password" name="passwd"/>
    <button onclick="login()">登陆</button>

    <script>
        function getCookie(name){
            var cook = document.cookie.match("\\b"+name+"=([^;]*\\b)")
            return cook ? cook[1] : undefined
        }
        function login(){
            info = {
                "username":"kaige",
                "age":19
            }
            jsonInfo = JSON.stringify(info)
            $.ajax({
                url:"/postfile",
                method:"POST",
                data:jsonInfo,
                success:function(data){
                    alert("OK2")
                },
                headers:{
                    "X-XSRFToken":getCookie("_xsrf")
                }
            })
        }
    </script>
</body>
</html>123456789101112131415161718192021222324252627282930313233343536373839
```

需要在进入主页时就自动设置_xsrf的cookie，可以通过定义tornado.web.StaticFileHandler子类而实现

```
class StaticFileHandler(tornado.web.StaticFileHandler):
    def __init__(self, *args, **kwargs):
        super(StaticFileHandler,self).__init__(*args, **kwargs)
        self.xsrf_token1234
```

# 03-用户验证

在受到用户的请求后进行预处理验证用户的登陆状态，通过验证正常处理，否则强制用户跳转到登录界面

确保修饰的方法只有在验证合法才可以被调用

```
@tornado.web.authenticated装饰器1
```

验证用户的逻辑写在该方法中，如果返回True说明验证成功 
 如果返回False，说明验证失败。验证失败会重定向到配置文件中login_url所指定的路由上

```
get_current_user()方法1
"login_url":"/login"1
```

实例

```
class HomeHandler(RequestHandler):
    def get_current_user(self):
        flag = self.get_argument("flag", None)
        return flag
    @tornado.web.authenticated
    def get(self, *args, **kwargs):
        self.render('home.html')
class CartHandler(RequestHandler):
    def get_current_user(self):
        flag = self.get_argument("flag", None)
        return flag
    @tornado.web.authenticated
    def get(self, *args, **kwargs):
        self.render('cart.html')
class loginHandler(RequestHandler):
    def get(self, *args, **kwargs):
        next = self.get_argument("next", "/")
        url = "/login?next=" + next
        self.render('login.html', url=url)
    def post(self, *args, **kwargs):
        name = self.get_argument("username")
        pswd = self.get_argument("passwd")
        if name == "sunck" and pswd == "1234567890a":
            #登陆成功，哪里来哪里去
            next = self.get_argument("next", "/")
            self.redirect(next+"?flag=logined")
        else:
            #登陆失败，还嘚瑟啥
            next = self.get_argument("next", "/")
            self.redirect('/login?next='+next)
```