# 使用API文档生成工具-sphinx - 宇宙浪子的专栏 - CSDN博客
2018年03月20日 18:46:46[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：237
Python第三方库sphinx可以自动化为restful API生成文档，使用步骤如下所示：
- 1. 安装
pip install sphinx
- 2. 创建文档目录
mkdir document
- 3. 配置
配置方式有两种：
- 可以使用sphinx-quickstart命令配置，基本上一路按回车就可以，最后会生成一下文件和目录：
![](https://img-blog.csdn.net/20180320185251541)
2. 使用sphinx-apidoc命令
可以用--help查看参数列表
例如：
sphinx-apidoc -F -l -f --ext-autodoc -o ./ -H projectname -A Author ../app
其中../app为API源码目录，根据自己情况设置，不过每执行一次都有重新修改conf.py的path路径
3.修改conf.py
将工程目录添加到模块搜索路径，生成文档时是需要。
```python
import os
import sys
sys.path.insert(0, os.path.abspath(r'..'))
```
这段代码默认是注释掉的，路径设置工程目录
- 4. 编译
直接执行make会给出可以make的列表
生成html文档，可以执行make epub 或 make html
生成文件在_build目录, 两种格式结构类似，但make html的格式有搜索功能，本人比较喜欢。
![](https://img-blog.csdn.net/201803201851457)

