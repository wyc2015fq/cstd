# python进阶(四) windows下虚拟环境使用 - _天枢 - 博客园
## [python进阶(四) windows下虚拟环境使用](https://www.cnblogs.com/yhleng/p/8359589.html)
2018-01-26 15:02 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8359589)
**虚拟环境作用:**
           1. 通常开发一个项目,会安装很多的第三方包,这时第三方包我们是安装在本机环境的.那么如果项目进行部署或移植的时候是不是要重新安装这些包????
           2.开发环境,同时在做两相项目,同时要用到Django这个包,但是一个项目要用1.8版本,一个要用1.9怎么办,虚拟环境因此诞生了..............
**下载并安装 虚拟环境virtualenv:**
虚拟环境严格来说也是一个第三方包,我们需要先进行下载:
```
pip install virtualenv
```
**virtualenv创建:**
**cmd命令进入要存储虚拟环境的文件夹**
```
virtualenv venv
```
执行以上创建命令,如果报以下错误:
```
AssertionError: Filename C:\Python27\Lib\os.py does not start with any of these prefixes: ['C:\\python27']
```
避免以上错误使用以下命令:python -m virtualenv <虚拟环境名称>
```
python -m virtualenv myenv
```
```
C:\Python27>python -m virtualenv myenv
New python executable in C:\Python27\myenv\Scripts\python.exe
Installing setuptools, pip, wheel...done.
```
创建虚拟环境成功!
** 进入虚拟环境**
    虚拟环境目录下的scripts/activate 来激活环境; 环境激活会在最前边显示虚拟环境名称
```
C:\Python27\myenv\Scripts>activate
**(myenv) C:\Python27\myenv\Scripts****>**
```
**退出虚拟环境**
**deactivate**
**删除虚拟环境**
    del myenv
    直接删除虚拟环境所在的文件夹venv就删除了我们创建的venv虚拟环境。
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,
