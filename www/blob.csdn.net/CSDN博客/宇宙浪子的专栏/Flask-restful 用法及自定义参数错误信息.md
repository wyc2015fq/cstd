# Flask-restful 用法及自定义参数错误信息 - 宇宙浪子的专栏 - CSDN博客
2018年03月07日 10:57:48[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1530
转自：[https://segmentfault.com/a/1190000012281928](https://segmentfault.com/a/1190000012281928)
flask-restful 是一款比较好用的 flask 插件，它不仅自动为我们实现了数据的 json 化，还能对传入参数进行验证，优雅的替代了 form 表单。
代码结构：
```
app
 |_api
 |  |_ __init__.py
 |  |_ parser.py
 |  |_ view.py
 |
 |_ __init__.py
 |_ code.py
 |_ util.py
app.py
```
具体代码如下：
```
parser.py
from flask_restful.reqparse import RequestParser
parser = RequestParser()
parser.add_argument("id", type=int, location="args", required=True)
# get 请求为args， post请求为json
```
在 view 中的使用
view.py
```python
```
from flask_restful import Resource
from app.util import make_result
from .parser import parser
class Test(Resource):
    def get(self):
        req = parser.parse_args(strict=True)
        id = req.get("id")
        return make_result(data={"id": id})
```
```
还有 __init__.py 文件设置路由
__init__.py
```
from .view import Test
from flask_restful import Api
from flask import Blueprint
api = Blueprint("api", __name__) # 设置蓝图
resource = Api(api)
resource.add_resource(Test, "/") # 设置路由
```
util.py 为一个设置文件， 用来放我们自己定义的输出规范。
util.py
```
from flask import jsonify
from app.code import Code
def make_result(data=None, code=Code.SUCCESS):
    return jsonify({"code": code, "data": data, "msg": Code.msg[code]})
```
code.py 是我们自定义的错误码
code.py
```
class Code:
    SUCCESS = 1200
    NO_PARAM = 1300
    msg = {
        SUCCESS: "success",
        NO_PARAM: "no param"
    }
```
app.py 为启动文件
```
app.py
from flask import Flask
from app.api import api
app = Flask(__name__)
app.register_blueprint(api)
if __name__ == "__main__":
    app.run(debug=True)
```
当我们运行 python app.py 的时候，程序便启动了起来。
我们在浏览器中输入：localhost:5000/?id=123
即可看到：
```
{
"code": 1200,
"data":{
"id": 123
},
"msg": "success"
}
```
那当我们输入: localhost:5000/?id=api
得到了：
```
{
"message":{
"id": "invalid literal for int() with base 10: 'api'"
}
}
```
很明显，我们期望得到一个 int 型的 id，这里给了一个 str 类型的字符串，于是得到了 status_code 为 40 0的 response。
我们肯定是希望以我们自己的定义消息来输出，即：
```
{
"code": 1300,
"data": null,
"msg": "no param"
}
```
flask-restful 并没有提供相应的 api，经过查看源码，发现 flask-restful abort 的代码是改写的 flask 的 abort 代码，于是我们也改写 flask-restful 的 abort 代码。
flask abort 源码：
```
def abort(status, *args, **kwargs):
    return _aborter(status, *args, **kwargs)
```
非常简单。
flask-restful 的 abort 源码：
```python
```
def abort(http_status_code, **kwargs):
    try:
        original_flask_abort(http_status_code)
    except HTTPException as e:
        if len(kwargs):
            e.data = kwargs
        raise
```
```
也是非常的简单，这里的 original_flask_abort 即 flask 的 abort。
那现在改代码就很容易了，只要 http_status_code == 400，那么就是我们的参数没有接受正确，需要爆出格式化以后的错误。
在 app.py 中修改：
app.py
```
from flask import Flask, abort
import flask_restful
from app.api import api
from app.util import make_result
from app.code import Code
app = Flask(__name__)
app.register_blueprint(api)
def custom_abord(http_status_code, *args, **kwargs):
    # 只要http_status_code 为400， 报参数错误
    if http_status_code == 400:
        abort(make_result(code=Code.NO_PARAM))
    # 正常返回消息
    return abort(http_status_code)
# 把flask_restful中的abort方法改为我们自己定义的方法
flask_restful.abort = custom_abord
if __name__ == "__main__":
    app.run(debug=True)
```
现在再次运行 python app.py
浏览器输入：[http://localhost](http://localhost/):5000/?id=api
即可得到：
```
{
"code": 1300,
"data": null,
"msg": "no param"
}
```
输入：[http://localhost](http://localhost/):5000/?id=111 检测一下正常输出：
```
{
"code": 1200,
"data":{
"id": 111
},
"msg": "success"
}
```
