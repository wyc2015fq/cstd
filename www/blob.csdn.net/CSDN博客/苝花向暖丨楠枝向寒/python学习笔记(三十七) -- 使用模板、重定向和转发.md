# python学习笔记(三十七) -- 使用模板、重定向和转发 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月16日 13:56:18[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：99
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



**Table of Contents**

[前言](#%E5%89%8D%E8%A8%80)

[使用模板](#%E4%BD%BF%E7%94%A8%E6%A8%A1%E6%9D%BF)

[重定向和转发](#%E9%87%8D%E5%AE%9A%E5%90%91%E5%92%8C%E8%BD%AC%E5%8F%91)

[题外话](#%E9%A2%98%E5%A4%96%E8%AF%9D)

# 前言

上篇文章也说过了，在python中写前端代码肯定是不合适的，一是前后端耦合，二是在后端中写前端也没有代码提示是非常痛苦的。

所以一般情况，页面都是预先写好的，页面中有一些类似{{name}}的占位符，我们从后端传递到前端，就可以将这些占位符替换成我们想要的值。这整个过程就被称为 MVC，M代表从后端传递到前端的值，V代表写好的页面模板，C代表代表逻辑处理即接收前端的请求再做处理的那部分。

# 使用模板

**MVC中的M 和 C如下**

```python
from flask import Flask, request, render_template  # 通过render_template将页面返回给用户

app = Flask(__name__)

@app.route('/', methods=['GET', 'POST'])
def home():
    page_list = [1, 2, 3, 4, 5]
    return render_template('home.html', page_list=page_list)

@app.route('/signin', methods=['GET'])
def signin_form():
    return render_template('form.html')

@app.route('/signin', methods=['POST'])
def signin():
    username = request.form['username']
    password = request.form['password']
    if username == 'admin' and password == 'password':
        # 密码正确就转发到登录成功页面,并给前端username
        return render_template('signin-ok.html', username=username)
    # 否则转发到form.html 传给前端 username 和 message
    return render_template('form.html', message='Bad username or password', username=username)

if __name__ == '__main__':
    app.run()
```

**MVC中的 V如下**

**hello.html**

```
<!DOCTYPE html>
<html>
<head>
  <title>Home</title>
</head>
<body>
  <h1 style="font-style:italic">Home</h1>

  下面是jinja2中的for循环使用测试<br>
    {% for i in page_list %}
        <a href="/page/{{ i }}">{{ i }}</a>
    {% endfor %}
</body>
</html>
```

**form.html**

```
<!DOCTYPE html>
<html>
<head>
  <title>Please Sign In</title>
</head>
<body>
  {% if message %}  <!--jinjia2 中的 if语句，如果message不为空 -->
  <p style="color:red">{{ message }}</p>  <!-- message是后端传递过来的值，字体为红色 -->
  {% endif %}  <!--jinjia2 中的 if结束标志 -->
  <form action="/signin" method="post">
    <legend>Please sign in:</legend>
    <p><input name="username" placeholder="Username" value="{{ username }}"></p>
    <p><input name="password" placeholder="Password" type="password"></p>
    <p><button type="submit">Sign In</button></p>
  </form>
</body>
</html>
```

**signin-ok.html**

```
<!DOCTYPE html>
<html>
<head>
  <title>Welcome, {{ username }}</title>   <!-- 后端会传递过来一个值，前端会显示 -->
</head>
<body>
  <p>Welcome, {{ username }}!</p>
</body>
</html>
```

效果如下

![](https://img-blog.csdn.net/20180916131807351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

输入错误密码 

![](https://img-blog.csdn.net/20180916131852398?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180916132428438?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

输入正确密码

![](https://img-blog.csdn.net/2018091613255741?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180916132524771?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 重定向和转发

**这里我说一下什么叫转发和重定向**

**转发过程** 客户首先发送一个请求到服务器端，服务器端发现匹配的app.route中的url，并指定该函数去执行，当这个函数执行完之后，它要调用render_template()方法，把请求转发给指定的signin-ok.html,整个流程都是在服务器端完成的，而且是在同一个请求里面完成的，因此该函数和html共享的是同一个request，在render_template里面放的所有参数，在signin-ok.html中都能取出来。整个过程是一个请求，一个响应。

**重定向使用如下：**

```python
from flask import Flask,redirect,url_for    #  redirect负责重定向，url_for获取重定向地址
```

```python
return redirect(url_for('signin_form'))　# url_for参数是函数名，它会根据函数名返回app.route中的url
```

**重定向过程** 客户发送一个请求到服务器，服务器匹配app.route中的url，然后该函数执行处理请求，这都和请求转发一样，函数处理完之后调用了redirect()这个方法，这个方法是response的方法，所以，当这个函数处理完之后，看到redirect()方法，立即向客户端返回这个响应，响应行告诉客户端你必须要再发送一个请求，请求的地址就是url_for('signin_form')的返回值，紧接着客户端受到这个请求后再次匹配app.route中的url，然后再给客户端返回结果,这里两个请求互不干扰，相互独立。

重定向和转发体现在浏览器的区别就是 url 会不会改变，比如你 输入 [http://127.0.0.1:5000/](http://127.0.0.1:5000/) ,服务器接收到了请求，在函数内部给你转发到了登录页面，这时浏览器显示的是登录页面但是地址还是 [http://127.0.0.1:5000/](http://127.0.0.1:5000/)，如果是重定向的话url地址就会被改变，因为上面也说了，重定向是告诉浏览器你需要重新发送一次请求，同时告诉浏览器请求的url。

## 题外话

除了Jinja2，常见的模板还有：
- 
[Mako](http://www.makotemplates.org/)：用`<% ... %>`和`${xxx}`的一个模板；

- 
[Cheetah](http://www.cheetahtemplate.org/)：也是用`<% ... %>`和`${xxx}`的一个模板；

- 
[Django](https://www.djangoproject.com/)：Django是一站式框架，内置一个用`{% ... %}`和`{{ xxx }}`的模板。


