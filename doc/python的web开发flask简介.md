# python的web开发flask简介

2018年11月22日 10:49:01

IE06

阅读数：35

更多

 								所属专栏： 																[python小tips](https://blog.csdn.net/column/details/31935.html) 																 							

 									



### 文章目录

- [1. 基本概念](https://blog.csdn.net/kittyzc/article/details/84326227#1__15)
- [1.1 初始化](https://blog.csdn.net/kittyzc/article/details/84326227#11__16)
- [1.2 路由和视图函数](https://blog.csdn.net/kittyzc/article/details/84326227#12__22)
- [1.3 上下文](https://blog.csdn.net/kittyzc/article/details/84326227#13__35)
- [1.4 响应和重定向](https://blog.csdn.net/kittyzc/article/details/84326227#14__86)
- [1.5 Flash消息传递](https://blog.csdn.net/kittyzc/article/details/84326227#15_Flash_101)
- [2. 模板](https://blog.csdn.net/kittyzc/article/details/84326227#2__113)
- [2.1 渲染过程](https://blog.csdn.net/kittyzc/article/details/84326227#21__119)
- [2.2 控制结构](https://blog.csdn.net/kittyzc/article/details/84326227#22__135)
- [2.3 bootstrap](https://blog.csdn.net/kittyzc/article/details/84326227#23_bootstrap_161)
- [3. 表单](https://blog.csdn.net/kittyzc/article/details/84326227#3__177)
- [3.1 启用CSRF保护](https://blog.csdn.net/kittyzc/article/details/84326227#31_CSRF_179)
- [3.2 创建表单类](https://blog.csdn.net/kittyzc/article/details/84326227#32__187)
- [4. 数据库](https://blog.csdn.net/kittyzc/article/details/84326227#4__222)
- [4.1 ORM模型](https://blog.csdn.net/kittyzc/article/details/84326227#41_ORM_224)
- [4.2 sqlite操作](https://blog.csdn.net/kittyzc/article/details/84326227#42_sqlite_288)

 flask是一个很简易的web框架，接下来我们将一步步建立一个完整的网站服务。

 使用如下代码安装flask



```python
pip install flask
1
```

启动服务的方法：

```python
export FLASK_APP=schedule.py
export FLASK_DEBUG=True
flask run --debugger --host=0.0.0.0
123
```

上面的代码是可调试的，如果要正式发布，则应该将debug相关内容删除。
 最后的–host部分使得服务可以远程访问。

# 1. 基本概念

# 1.1 初始化

构造函数的__name__变量决定程序的根目录。

```python
from flask import Flask
app = Flask(__name__)
12
```

# 1.2 路由和视图函数

客户端的URL需要转换为服务器的python函数，其映射关系称为路由，使用app.route修饰器可以将函数进行注册，被注册的函数称为视图函数。
 此外，也可以使用app.add_url_rule()生成映射。
 视图函数可以带参数，默认为string类型，也可以指定为int，float，path类型，使用<int: id>的方式。参考下面的例子：

```python
@app.route('/')
def index()
	return '<h1>Hello World!</h1>'

@app.route('/user/<name>')
def user(name):
	return '<h1>Hello, %s</h1>' % name
1234567
```

# 1.3 上下文

Flask使用上下文访问客户端返回的http请求对象。上下文分为以下四种：

| 变量        | 上下文 | 说明                                                         |
| ----------- | ------ | ------------------------------------------------------------ |
| current_app | 程序   | 激活程序的实例                                               |
| g           | 程序   | 处理请求时用作临时存储的对象                                 |
| request     | 请求   | 请求对象，封装了客户端发出的额HTTP请求中的内容               |
| session     | 请求   | 用户会话，存储需要记住的值的词典。session在关闭浏览器之后失效 |

无论是哪一个，在使用之前都要先引入：

```python
from flask import request,session
1
```

session的定义很简单，下面是例子，注意使用get()方法比较安全，对于不存在的键，返回默认值None。

```python
session['user'] = form.username.data
...
name = session.get('user')
123
```

request有post和get两种方式，下面是例子：
 模板代码：

```html
$.ajax({
    type:'POST',
    url:"/saveAction",
    contentType:'application/json; charset=UTF-8',
    data:JSON.stringify({'clng':lng,'clat':lat}),
    dataType:'json'
})
        
$.get('/getGeo',{lat:clat,lng:clng},function(data){
    var d = data;
})
1234567891011
```

后台python代码：

```python
import mzgeohash as geo
@app.route('/getGeo')
def getGeo():
    lat = float(request.args.get('lat'))
    lng = float(request.args.get('lng'))
    return  geo.encode((lng,lat))[:7]

@app.route('/saveAction', methods=['POST'])
def saveAction():
    d = json.loads(request.get_data())
    lat =  float(d['lat'])
    lng =  float(d['lng'])
    session['lat'] = lat
    session['lng'] = lng
    return 'success'
123456789101112131415
```

# 1.4 响应和重定向

可以使用make_response方法封装响应，可以接受作为html页面返回的字符串、装填吗和header字典。使用make_response还有一个好处是可以设置cookie：

```python
resp = make_response("set cookie OK")
resp.set_cookie("settime","python2",max_age=3600)
12
```

使用redirect可以执行重定向的响应，如下：

```python
from flask import redirect,url_for
@app.route('/')
def index():
	return redirect(url_for('login'))
1234
```

其中url_for接收视图函数名，返回对应的url。

# 1.5 Flash消息传递

Flash是只显示一次的数据。使用前需要import。
 使用方法很简单，下面是例子：

```python
flash('you changed your name')
1
```

然后在模板中使用get_flashed_messages方法获得消息。使用for循环是因为消息有可能会排队。

```python
{% for message in get_flashed_messages() %}
	{{message}}
{% endfor %}
123
```

# 2. 模板

模板指的是包含相应文本的文件，包含用占位变量表示的动态部分。Flask把静态文件放置在根目录下的static文件夹下。注意要使用中文的话，需要在python代码最前面加上：

```python
#coding=utf-8
1
```

然后在所有中文字符串前面加上u。

# 2.1 渲染过程

使用真实值替换变量，再返回最终相应字符串的过程称为**渲染**。
 Flask使用Jinja2引擎，代码中使用render_template渲染模板。渲染时可以传递参数，参考如下代码：

```python
from flask import render_template
...
@app.route('/user/<name>')
def user(name):
	return render_template('user.html',name = 'ie06')
12345
```

这样，模板中的name就会被替换为’ie06’。默认情况下，Flask在templates文件夹中寻找模板，变量用两个中括号表示，示例如下：

```html
<h1>Hello, {{name}}</h1>
1
```

可以为变量添加管道函数，例如
 {{name|capitalize}}，则name会变成’Ie06’

# 2.2 控制结构

使用{% %}添加控制结构。例如：

```html
{% if user%}
    Hello, {{user}}!
{% else %}
    Hello, Stranger!
{% endif %}   
12345
<ul>
	{% for comment in comments %}
		<li>{{comment}}</li>
	{% endfor %}
</ul>
12345
```

另外有一个**模板继承**的重要功能，首先定义个基础模板base.html，中间添加

```html
{% block body %}{% endblock %}
1
```

然后在衍生文件中使用

```html
{% extends "base.html" %}
{% block body %}...{% endblock %}
12
```

就可以将base.html中的内容加载到衍生文件中了。

# 2.3 bootstrap

bootstrap是Twitter开发的一个开源框架，python下需要安装，是一套非常好用的模板。

```python
pip install flask-bootstrap
1
```

使用如下方法引入bootstrap模板：

```python
from flask_bootstrap import Bootstrap
...
bootstrap = Bootstrap(app)
123
```

然后在模板中使用：

```html
{% extends "bootstrap/base.html" %}
1
```

[这里](http://getbootstrap.com/docs/4.1/examples/)有不少示例可供参考。

# 3. 表单

使用Flask-WTF创建表单。使用之前先用pip进行安装。

# 3.1 启用CSRF保护

设置一个密钥，然后程序使用密钥生成加密令牌，用于验证表单中数据的真伪：

```python
class Config(object):
    SECRET_KEY = 'a9087FFJFF9nnvc2@#$%FSD'
app = Flask(__name__)
app.config.from_object(Config)
1234
```

# 3.2 创建表单类

每个表单由一个表单类来表示，各种filed使用.data取出对应的值。
 第一次访问程序时，服务器接收到的是没有数据的GET请求，validate_on_submit返回False。填写好内容并提交表单后，服务器收到包含数据的POST请求，validate_on_submit返回True。
 参考下面的代码：

```python
from flask_wtf import FlaskForm
from wtforms import StringField,PasswordField,BooleanField,SubmitField
from wtforms.validators import DataRequired
class LoginForm(FlaskForm):
    #DataRequired，当你在当前表格没有输入而直接到下一个表格时会提示你输入
    username = StringField(unicode('用户', encoding='utf-8'),validators=[DataRequired(message='input username')])
    password = PasswordField(unicode('密码', encoding='utf-8'),validators=[DataRequired(message='input password')])
    remember_me = BooleanField(u'记住我')
    submit = SubmitField(u'登录')
@app.route('/',methods=['GET','POST'])
def login():
    form = LoginForm()
    result = u"请登录"
    if form.validate_on_submit(): 
    	result = u"欢迎你，"+ form.username.data
    	return redirect(url_for('mainpage',result = result))
    return render_template('login.html',title='登录',form=form, result = result)
1234567891011121314151617
```

然后在login.html中使用bootstrap模板进行渲染：

```html
{% extends "base.html" %}
{% import "bootstrap/wtf.html" as wtf %}
{% block title %}{{ title }}{% endblock %}
{% block content %}
<div class="alert alert-warning">{{result}}</div>
<div class="col-md-4">
    {{ wtf.quick_form(form) }}
</div>
{% endblock %}
123456789
```

# 4. 数据库

这里用最最最简单的sqlite，centos系统自带。

# 4.1 ORM模型

使用flask_sqlalchemy建立ORM模型，用对象化的方法操作数据库。使用之前需要安装flask_sqlalchemy。
 简单例子如下：

```python
from flask_sqlalchemy import SQLAlchemy
import sqlite3
import os
basedir = os.path.abspath(os.path.dirname(__file__))
class Config(object):
    SQLALCHEMY_DATABASE_URI = 'sqlite:///'+os.path.join(basedir,'static/data.sqlite3')
    SQLALCHEMY_TRACK_MODIFICATIONS=True
    SQLALCHEMY_COMMIT_ON_TEARDOWN=True

app = Flask(__name__)
app.config.from_object(Config)
db = SQLAlchemy(app)

class User(db.Model):
    __tablename__='user'
    id = db.Column(db.Integer, primary_key = True)
    phone = db.Column(db.String(64),nullable=False)
    pw = db.Column(db.String(64),nullable=False)
    def __repr__(self):
        return '<User %r>'%self.phone

@app.route('/',methods=['GET','POST'])
def login():
    form = LoginForm()
    result = u"请登录"
    if form.validate_on_submit():        
        try:
            user = User.query.filter_by(phone=form.username.data).first()
            if user is None:
                result = u'没有此用户'
            else:
                if user.pw == form.password.data:
                    session['user'] = form.username.data
                    return redirect(url_for('schedule'))
                else:
                    result = u'密码错误'
        except Exception as err:
            result = err
    return render_template('login.html',title='登录',form=form,result = result)
123456789101112131415161718192021222324252627282930313233343536373839
```

其中db对象表示创建了一个新的SQLAlchemy类的实例，使用这个对象可以创建数据库、创建表、对表进行数据增删改查的操作。
 既然是关系型数据库，那么表达关系是非常重要的内容，下面是上面例子的拓展：

```python
class User(db.Model):
    __tablename__='user'
    id = db.Column(db.Integer, primary_key = True)
    phone = db.Column(db.String(64),nullable=False)
    pw = db.Column(db.String(64),nullable=False)
    operations = db.relationship('Operation',backref='user',lazy='dynamci')
    def __repr__(self):
        return '<User %r>'%self.phone

class Operation(db.Model):
	__tablename__='operation'
	id = db.Column(db.Integer, primary_key = True)
	action = db.Column(db.Integer)
	time = db.Column(db.DateTime)
	user_id = db.Column(db.Integer,db.ForeignKey('user.id'))
123456789101112131415
```

在Operation类中，对字段添加ForeignKey的声明，就可以将两张表连接起来了；在User表中添加一个db.relationship的声明，就可以直接连接user对象（而不仅仅是user_id）。

# 4.2 sqlite操作

在创建了ORM模型后，我们可以直接使用它进行sqlite的操作。
 首先打开python shell，执行下面的语句：

```python
from schedule import db, User, Operation#这里导入放置ORM模型的py文件
db.create_all() #根据config中声明的地址创建数据库，并根据ORM的类创建表。
user1 = User(phone = '15000000000',pw = '123456')
user2 = User(phone = '18900000000',pw = '654321')
oper1_1 = Operation(action = 1, time = '2018-11-12 12:23:30',user = user1)
oper1_2 = Operation(action = 2, time = '2018-11-12 15:13:20',user = user1)
123456
```

通过数据库的会话管理对数据库进行操作

```python
db.session.add_all([user1,user2,oper1_1,oper1_2])
db.session.commit()
12
```

增、改都使用add/add_all方法，删除用delete方法，查询用query方法，例如：

```python
>>> User.query.all()
>>> User.query.filter_by(phone=form.username.data).first()
>>> user1.operations.filter_by(action = 1).count()
123
```

可以使用DDL语句直接在python中进行查询。直接输入sqlite3 + 数据库文件可以进入shell界面，使用SQL命令即可进行查询：
 ![在这里插入图片描述](https://img-blog.csdnimg.cn/20181122104703615.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)

.header on 显示表头
 .mode column 列显示方式
 .databases 显示数据库
 .tables 显示表名