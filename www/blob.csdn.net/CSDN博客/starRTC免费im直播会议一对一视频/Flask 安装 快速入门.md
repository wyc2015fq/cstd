# Flask 安装 快速入门 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月27日 14:18:51[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：138
$
pip install flask
Flask自带的Server在端口5000上监听：
ython app.py
flask通过request.form['name']来获取表单的内容。
外部可见服务器
app.debug=True
如果启用了调试支持，在代码修改的时候服务器能够自动加载
app.run**(**host='0.0.0.0'**) ， 如果不能访问看下是不是放在docker里面了，端口没有映射之类**

Virtualenv 能够允许多个不同版本的
 Python 安装，每一个服务于各自的项目。 
pip install virtualenv
一旦成功安装 virtualenv，运行 shell 创建自己的环境。我通常会创建一个项目文件夹，其下创建 *venv* 文件夹:
$ mkdir myproject$ cd myproject$ virtualenv venv
现在，只要你想要在某个项目上工作，只要激活相应的环境。
$ . venv/bin/activate
现在你只需要键入以下的命令来激活你的 virtualenv 中的 Flask:
$ pip install Flask
确保你的应用不叫做 *flask.py*，
 因为这会与 Flask 本身冲突。
一个最小的应用看起来像这样:
**from**flask**import**Flaskapp=Flask**(**__name__**)**@app.route**(**'/'**)****def**hello_world**():****return**'Hello World!'**if**__name__=='__main__'**:**app.run**()**

[http://www.pythondoc.com/flask/quickstart.html#quickstart](http://www.pythondoc.com/flask/quickstart.html#quickstart)
if __name__ =='__main__': 确保服务器只会在该脚本被
 Python 解释器直接执行的时候才会运行，而不是作为模块导入的时候。
**url参数传递**
@app.route**(**'/user/<username>'**)**
@app.route**(**'/post/<int:post_id>'**)****def**show_post**(**post_id**):**
**int表示**接受整数
/post/4

@app.route**(**'/login'**)****def**login**():****pass**
**print**url_for**(**'login'**) // **/login**print**url_for**(**'login'**,**next='/'**) // **/login?next=/
**好处：**能够处理特殊字符和 Unicode
 转义，
HTTP方法
@app.route**(**'/login'**,**methods=**[**'GET'**,**'POST'**])****def**login**():****if**request.method=='POST'**:**do_the_login**()****else****:**show_the_login_form**()**
渲染模板
使用方法 [**render_template()**](http://www.pythondoc.com/flask/api.html#flask.render_template) 来渲染模版

接收数据
**from**flask**import**request
当前请求的方法可以用 **method** 属性来访问。你可以用 **form** 属性来访问表单数据
@app.route**(**'/login'**,**methods=**[**'POST'**,**'GET'**])****def**login**():**error=None**if**request.method=='POST'**:****if**valid_login**(**request.form**[**'username'**],**request.form**[**'password'**]):****return**log_the_user_in**(**request.form**[**'username'**])****else****:**error='Invalid username/password'*# if the request method was GET or the credentials were invalid***return**render_template**(**'login.html'**,**error=error**)**
如果在 *form* 属性中不存在上述键值会发生些什么？在这种情况下会触发一个特别的 [**KeyError**](http://docs.python.org/dev/library/exceptions.html#KeyError)。
你可以用 **args** 属性来接收在
 URL ( ?key=value ) 中提交的参数:
searchword=request.args.get**(**'key'**,**''**)**
日志
app.logger.debug**(**'A
 value for debugging'**)**
[http://www.pythondoc.com/flask/tutorial/index.html#tutorial](http://www.pythondoc.com/flask/tutorial/index.html#tutorial)
