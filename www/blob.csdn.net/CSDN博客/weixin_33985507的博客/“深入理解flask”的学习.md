# “深入理解flask”的学习 - weixin_33985507的博客 - CSDN博客
2017年05月19日 14:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
pip基本操作
安装：pip install [包名]
删除：pip uninstall []
查找:pip search []
列表安装：pip install -r requirements.txt
导出列表：pip freeze > requirements.txt
**安装pip install flask==0.10.1**
### 这本书坑。忽略下面，没什么实际的技能学习
下面直接附代码
创建config.py文件
```python
class Config(object):
    """docstring for Config"""
    def __init__(self, arg):
        super(Config, self).__init__()
        self.arg = arg
        
    pass
class ProdConfig(object):
    """docstring for ProdConfig"""
    def __init__(self, arg):
        super(ProdConfig, self).__init__()
        self.arg = arg
    pass
class DevConfig(object):
    """docstring for DevConfig"""
    def __init__(self, arg):
        super(DevConfig, self).__init__()
        self.arg = arg
        
    pass
```
创建main.py文件
```python
from flask import Flask
from config import DevConfig
app  = Flask(__name__)
app.config.from_object(DevConfig)
@app.route('/')
def home():
    return 'hello'
if __name__ =='__main__':
    app.run()
```
运行main.py文件  浏览器访问127.0.0.1:500就会出现hello
**安装：pip install flask-script**
创建manage.py
```python
from flask.ext.script import Manager,Server
from main import app
manager = Manager(app)
manager.add_command('server',Server())
@manager.shell
def make_shell_context():
    return dect(app=app)
if __name__=='__main__':
    manager.run()
```
**运行python manage.py runserver **
还可以实用命令进入shell：
python manage.py shell
> 
# 第二章
这里新手会遇到坑，我做笔记的不讲解错误
安装pip install flask-sqlalcheny
前提安装了python的mysql库
修改config.py
```python
class DevConfig(object):
    debug= True
    SQLALCHEMY_DATABASE_URI= 'mysql+pymysql://用户名:密码@ip地址:端口/数据库名'
```
修改main.py
```python
#coding=utf-8
from flask import Flask
from config import DevConfig
from flask.ext.sqlalchemy import SQLAlchemy
app  = Flask(__name__)
app.config.from_object(DevConfig)
db = SQLAlchemy(app)
@app.route('/')
def home():
    return 'hello'
class User(db.Model):
    __tablename__ = 'user' #数据库中创建的表名
    id = db.Column(db.Integer(),primary_key=True)
    username = db.Column(db.String(255))
    password = db.Column(db.String(255))
    def __init__(self,username):
        self.username=username
    def __repr__(self):
        return "User '{}'".format(self.username)
#希望现实sql语句的在devconfig中加入SQLALCHEMY_ECHO=True
if __name__ =='__main__':
    app.run()
```
修改manage.py
```python
from flask.ext.script import Manager,Server
from main import app,db,User
manager = Manager(app)
manager.add_command('server',Server())
@manager.shell
def make_shell_context():
    return dict(app=app,db=db,User=User)
if __name__=='__main__':
    manager.run()
```
运行,并添加一个用户：
```python
python manage.py shell
db.create_all()
user = User(username='fake_name')
db.session.add(user)
db.session.commit()
```
修改main.py
```python
#coding=utf-8
from flask import Flask
from config import DevConfig
from flask.ext.sqlalchemy import SQLAlchemy
app  = Flask(__name__)
app.config.from_object(DevConfig)
db = SQLAlchemy(app)
@app.route('/')
def home():
    return 'hello'
class User(db.Model):
    __tablename__ ='user' #数据库中创建的表名
    id = db.Column(db.Integer(),primary_key=True)
    username = db.Column(db.String(255))
    password = db.Column(db.String(255))
    def __init__(self,username):
        self.username=username
    def __repr__(self):
        return "User '{}'".format(self.username)
#希望现实sql语句的在devconfig中加入SQLALCHEMY_ECHO=True
#文章
class Post(db.Model):
    id = db.Column(db.Integer,primary_key=True)
    title = db.Column(db.String(255))
    text = db.Column(db.Text())
    publish_date = db.Column(db.DateTime())
    user_id = db.Column(db.Integer()) #不喜欢用外链 这样比较自由  
    def __init__(self,title):
        self.title= title
    def __repr__(self):
        return "<post '{}'>".format(self.title)
＃标签
class Tag(db.Model):
    id = db.Column(db.Integer(),primary_key=True)
    title = db.Column(db.String(255))
    def __init__(self,title):
        self.title=title
    def __repr__(self):
        return "<Tag :{}>".format(self.title)
if __name__ =='__main__':
    app.run()
```
添加数据：
```python
python manger.py shell
user = User.query.get(1)
from main import Post
post = Post(title='title post',text ='',publish='2017-12-10')
post.user_id = user.id
db.session.add(post)
db.session.commit()
```
