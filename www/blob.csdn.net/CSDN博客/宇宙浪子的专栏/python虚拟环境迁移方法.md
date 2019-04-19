# python虚拟环境迁移方法 - 宇宙浪子的专栏 - CSDN博客
2019年02月25日 17:59:14[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：158
# **场景**
- 生产环境是断网，无法联网安装依赖包。
- 第三方依赖库太多，每次联网安装依赖包太耗费时间或网络资源。
-  需要更改已有虚拟环境路径。
# 方法一
如果是有网环境：
-  source env/bin/activate   # 先激活本地虚拟环境
- pip freeze > requirements.txt 
- pip install -r requirements.txt
# 方法二
-  source venv/bivn/activate   # 先激活本地虚拟环境
- pip freeze > requirements.txt 
- 在开发环境先下载好离线包：pip download -d your_offline_packages -r requirements.txt
- 打包your_offline_packages，并copy到目标环境
- 在目标环境创建新的virtualenv环境并激活:
	- virtualenv -p python_path venv
- source venv/bivn/activate
- 批量安装离线安装包
	- 
pip install --no-index --find-links=your_offline_packages -r requirements.txt
# 方法三
- 打包已经创建好的venv，并拷贝到目标环境解压
- venv/bin/下修改activate文件中参数
```
VIRTUAL_ENV="your new venv path"
export VIRTUAL_ENV
```
- source venv/bin/activate
