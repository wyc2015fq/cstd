# 笔记︱利用python + flask制作一个简易本地restful API - 素质云笔记/Recorder... - CSDN博客





2017年09月26日 10:43:14[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3254








原版官网：[http://flask-restful.readthedocs.io/en/latest/](http://flask-restful.readthedocs.io/en/latest/)

中文官网：[http://www.pythondoc.com/Flask-RESTful/quickstart.html](http://www.pythondoc.com/Flask-RESTful/quickstart.html)

python3下载：
```
pip install flask-restful
```

其中，好像py3.6不能这么调用：

```
from flask.ext.restful import reqparse, abort, Api, Resource
```

需要这样：

```
from flask_restful import reqparse, abort, Api, Resource
```

.

# 一、案例解析

由一个完整案例解析：

```
from flask import Flask
from flask.ext.restful import reqparse, abort, Api, Resource

app = Flask(__name__)
api = Api(app)

TODOS = {
    'todo1': {'task': 'build an API'},
    'todo2': {'task': '?????'},
    'todo3': {'task': 'profit!'},
}


def abort_if_todo_doesnt_exist(todo_id):
    if todo_id not in TODOS:
        abort(404, message="Todo {} doesn't exist".format(todo_id))

parser = reqparse.RequestParser()
parser.add_argument('task', type=str)


# Todo
#   show a single todo item and lets you delete them
class Todo(Resource):
    def get(self, todo_id):
        abort_if_todo_doesnt_exist(todo_id)
        return TODOS[todo_id]

    def delete(self, todo_id):
        abort_if_todo_doesnt_exist(todo_id)
        del TODOS[todo_id]
        return '', 204

    def put(self, todo_id):
        args = parser.parse_args()
        task = {'task': args['task']}
        TODOS[todo_id] = task
        return task, 201


# TodoList
#   shows a list of all todos, and lets you POST to add new tasks
class TodoList(Resource):
    def get(self):
        return TODOS

    def post(self):
        args = parser.parse_args()
        todo_id = int(max(TODOS.keys()).lstrip('todo')) + 1
        todo_id = 'todo%i' % todo_id
        TODOS[todo_id] = {'task': args['task']}
        return TODOS[todo_id], 201

##
## Actually setup the Api resource routing here
##
api.add_resource(TodoList, '/todos')
api.add_resource(Todo, '/todos/<todo_id>')


if __name__ == '__main__':
    app.run(debug=True)
```

## 1、rescouce资源部分

### 1.1 TODOS ，任务列表

相当于数据库，如果是大型API调用，后台需要数据库支持才行，其中的todo1是子项任务；

### 1.2 api = Api(app)

Flask-RESTful 提供的最主要的基础就是资源(resources)，可以通过.add_resource添加资源resource，

```
api.add_resource(TodoList, '/todos')
	api.add_resource(Todo, '/todos/<todo_id>')
```

把内容TodoList列表，赋值给/todos，把Todo子项赋值给’/todos/<todo_id>’

该操作可以让子项分离之后用requests 来调用。

### 1.3 新加任务：post

```
def post(self):
	args = parser.parse_args()
	todo_id = int(max(TODOS.keys()).lstrip('todo')) + 1
	todo_id = 'todo%i' % todo_id
	TODOS[todo_id] = {'task': args['task']}
	return TODOS[todo_id], 201
```

有新加的任务，todo的编号+1，同时存储到TODOS

### 1.4 class Todo(Resource):

设置todo任务，使其可以用其他内容调取子项任务，这里有get（得到任务）/delete（删除任务）/put（更新任务）

与TodoList是平行资源，分管一个链接字段，’/todos/<todo_id>’

### 1.5 class TodoList(Resource):

项目列表，get（得到任务）/post（上传任务），分管’/todos’

### 1.6 判断字段是否存在

可以作为安全组进行密码验证.

```
def api_key_audit(api_keys):
     if api_key not in api_keys:
         abort(404, message="api key {} doesn't exist".format(api_key))
```

### 1.7 debug选项的开启

```
debug的作用是，如果你修改了app.py文件，并且保存之后，就会立即重新加载一遍app.py
```

一般来说，正式环境下不能打开，不然改了一下文件，就会重启

### 1.8 app.run()函数

```
run(self, host=None, port=None, debug=None, **options)
```

默认的host = 127.0.0.1,port = 5000

## 2、网页内容

abort_if_todo_doesnt_exist函数，报错机制，如果没有该任务号，则会显示404错误；

参数解析reqparse：

验证表单数据仍然很痛苦。就通过命令上传参数，Flask-RESTful 内置了支持验证请求数据，
```
from flask.ext.restful import reqparse

parser = reqparse.RequestParser()
parser.add_argument('rate', type=int, help='Rate to charge for this resource')
args = parser.parse_args()
```

与 argparse 模块不同，reqparse.RequestParser.parse_args() 返回一个 Python 字典而不是一个自定义的数据结构。

使用 reqparse 模块同样可以自由地提供聪明的错误信息。如果参数没有通过验证，Flask-RESTful 将会以一个 400 错误请求以及高亮的错误信息回应。

主要应用在：class Todo中的put（更新参数）、class TodoList中的post（上传参数）

.
# 二、如何使用

这边在官网中提供了两种方式：一种是curl/一种是利用py的requests

## 1、curl的方式

```
# 增加一个新的任务
$ curl http://localhost:5000/todos -d "task=something new" -X POST -v    

#删除一个任务
$ curl http://localhost:5000/todos/todo2 -X DELETE -v     

#更新一个任务
$ curl http://localhost:5000/todos/todo3 -d "task=something different" -X PUT -v

#获取一个单独的任务
$ curl http://localhost:5000/todos/todo3
```

PUT ：更新

POST ：新增

.
## 2、py的requests方式

requests非常强大，本案例所需的有put, get ,post

```
from requests import put, get ,post
# 更新子任务，todo2
put('http://localhost:5000/todo2', data={'data': 'Change my brakepads'}).json()
{u'todo2': u'Change my brakepads'}

# 拿到子项任务，todo2
get('http://localhost:5000/todo2').json()
{u'todo2': u'Change my brakepads'}

# 看到所有的任务
get('http://localhost:5000/todos').json()

# 新加任务
post('http://localhost:5000/todos', data={'task': 'http://mpic.tiankong.com/5a3/23f/5a323f17880785b60cd4895de11a3569/G70-254989.jpg'}).json()
```

额外的requests还有以下的请求方式：

```
r = requests.get("http://httpbin.org/get")
r = requests.post("http://httpbin.org/post")
r = requests.put("http://httpbin.org/put")
r = requests.delete("http://httpbin.org/delete")
r = requests.head("http://httpbin.org/get")
r = requests.options("http://httpbin.org/get")
```

响应方式有，可以通过多种方式读取，比如（参考来源：[Requests 库的使用](http://wiki.jikexueyuan.com/project/explore-python/HTTP/Requests.html)）：
- 普通响应，使用 r.text 获取，读取 unicode 形式的响应
- JSON 响应，使用 r.json() 获取
- 二进制响应，使用 r.content 获取，获取二进制数据，比如用返回的二进制数据创建一张图片
- 原始响应，使用 r.raw 获取，获取来自服务器的原始套接字响应，这可以通过访问响应对象的 raw 属性来实现，但要确保在初始请求中设置了 stream=True

.

延伸一：直接在post中返回结果
```
def post(self,api_key):
        api_key_audit(api_key)                                  # 验证上传
        args = parser.parse_args()
#        todo_id = int(max(TODOS.keys()).lstrip('todo')) + 1  #lstrip函数是 去掉坐起开始的'todo'字样
        todo_id = 'todo%i' % len(traverseDirByListdir('../data'))
        save_post_data(todo_id,{todo_id: args['task']})
        return {todo_id: predict(args['task'])}, 201

api.add_resource(TodoList, '/todos/<api_key>')
```

以上是直接调用的一部分内容

```
# 两种调用方式
post('http://localhost:5000/todos/1', data={'task':url}).json()
! curl http://localhost:5000/todos/1 -d "task=url" -X POST -v
```

1为需要验证的Key。

.

## 报错一：

```
can't find '__main__' module in ''
```

把debug关掉`app.run(debug=False, ...)`。

.

## 延伸二：设置nginx

（1）先找到配置文件/etc/nginx/nginx.conf的位置

```
nginx -t
```

（2）配置nginx.conf文件

修改的时候，新加一个：

location /similar {

proxy_pass [http://127.0.0.1:6666/Msimilar;](http://127.0.0.1:6666/Msimilar;)

}

即可。
（3）重启Nginx即可使用

nginx -s reload 重新启动nginx

## 延伸四：如何回传整张图片

之前以为把图片压缩成字节型，回传就会没有问题，但是仍然报错：

```
TypeError: a bytes-like object is required, not 'str'
```

那么怎么样的才算是合理的？

借鉴百度API的写法，用base64编码。

```
import base64
    # 读入
    def get_file_content_base64(filePath):
        with open(filePath,"rb") as f:
            base64_data = base64.b64encode(f.read())
            return base64_data.decode('utf-8')
# 保存
bytespic = get_file_content_base64(filePath)
        with open(name, 'wb') as file:
            file.write(base64.b64decode(bytespic))
```









