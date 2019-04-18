# flask学习笔记之blueprint - weixin_33985507的博客 - CSDN博客
2018年06月07日 14:10:53[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
### blueprint的作用
blueprint最主要的是解决路由的问题
### 创建blueprint的步骤
- 创建blueprint
- 注册blueprint
### 最简代码示例
`创建blueprint`
```bash
from flask import Blueprint
 
admin_bp = Blueprint('admin', __name__)
 
@admin_bp.route('/index')
def index(name):
    return '<h1>Hello, this is admin blueprint</h1>'
复制代码
```
`注册blueprint`
```bash
from flask import Flask
from admin.admin_module import admin_bp
 
app = Flask(__name__)
app.register_blueprint(admin_bp, url_prefix='/admin')
 
if __name__ == '__main__':
    app.run(debug=True)
复制代码
```
执行后访问路径`http://localhost:5000/admin/index`
