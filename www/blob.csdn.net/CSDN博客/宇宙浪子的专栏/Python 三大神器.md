# Python 三大神器 - 宇宙浪子的专栏 - CSDN博客
2016年02月17日 16:28:09[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：4414
## 转自：[http://www.ziqiangxuetang.com/python/pip-virtualenv-fabric.html](http://www.ziqiangxuetang.com/python/pip-virtualenv-fabric.html)
## 1. pip 用来包管理
文档：[https://pip.pypa.io/en/latest/installing.html](https://pip.pypa.io/en/latest/installing.html)
```
```python
# 安装，可指定版本号
```
```python
(sudo) pip install Django
```
```python
=
```
```python
=
```
```python
1.6
```
```python
.
```
```python
8
```
```python
# 升级
```
```python
(sudo) pip install bpython
```
```python
-
```
```python
-
```
```python
upgrade
```
```python
# 一次安装多个
```
```python
(sudo) pip install BeautifulSoup4 fabric virtualenv
```
```python
# 从文本中安装，文本中为包名，一行一个，可以指定版本号
```
```python
(sudo) pip install –r requirements.txt
```
```python
# 删除
```
```python
(sudo) pip uninstall xlrd
```
```python
# 导出当前已经安装包
```
```python
pip freeze > requirements.txt
```
```
## 2. virtualenv 独立Python环境管理
文档： [http://virtualenvwrapper.readthedocs.org/en/latest/](http://virtualenvwrapper.readthedocs.org/en/latest/)
virtualenv 是一个创建Python独立环境的包，virtualenvwrapper 使得virtualenv变得更好用
```
```python
# 安装:
```
```python
(sudo) pip install virtualenv virtualenvwrapper
```
```python
# 修改.bash_profile，添加以下语句
```
```python
export WORKON_HOME
```
```python
=
```
```python
$HOME
```
```python
/
```
```python
.virtualenvs
```
```python
export PROJECT_HOME
```
```python
=
```
```python
$HOME
```
```python
/
```
```python
YunPan
```
```python
/
```
```python
workspace
```
```python
source
```
```python
/
```
```python
usr
```
```python
/
```
```python
local
```
```python
/
```
```python
bin
```
```python
/
```
```python
virtualenvwrapper.sh
```
```
mkvirtualenv ENV：创建运行环境ENV
rmvirtualenv ENV：删除运行环境ENV
mkproject mic：创建mic项目和运行环境mic
mktmpenv：创建临时运行环境
workon bsp: 工作在bsp运行环境
lsvirtualenv: 列出可用的运行环境
lssitepackages: 列出当前环境安装了的包
创建的环境是独立的，互不干扰，无需sudo权限即可使用 pip 来进行包的管理。
下面是使用演示图：
![virtualenv.png](http://www.ziqiangxuetang.com/media/uploads/images/virtualenv_20150128171929_814.png)
## 3. fabric 服务器管理和应用发布
官网：[http://www.fabfile.org/](http://www.fabfile.org/)
文档：[http://docs.fabfile.org/](http://docs.fabfile.org/)
fabric: application deployment or systems administration tasks
```
```python
#coding:utf-8
```
```python
from
```
```python
fabric.api
```
```python
import
```
```python
*
```
```python
# 服务器列表
```
```python
env.hosts
```
```python
=
```
```python
[
```
```python
'user@server1'
```
```python
,
```
```python
'user2@server2'
```
```python
]
```
```python
def
```
```python
ls_home():
```
```python
```
```python
with cd(
```
```python
'/home/bae/'
```
```python
):
```
```python
```
```python
run(
```
```python
'ls'
```
```python
)
```
```python
'''
```
```python
常用命令
```
```python
lcd(dir): 进入本机某目录
```
```python
local(cmd): 本机上执行命令
```
```python
cd(dir): 进入服务器某目录
```
```python
run(cmd):服务器上执行命令
```
```python
'''
```
```
把上面的文件保存成 fabfile.py在终端上进入该文件的目录，执行
```
```bash
fab 函数名
```
```bash
比如：
```
```bash
fab ls_home
```
```
更多使用方法请参见官方文档。
