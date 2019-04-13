
# pip修改国内源 - 致林 - 博客园






# [pip修改国内源](https://www.cnblogs.com/bincoding/p/7944828.html)
Linux在~/.pip/pip.conf文件中添加或修改，
windows不存在该目录，在当前用户目录下创建pip.ini（例如 C:\Users\bin\pip\pip.ini）：
[global]
index-url = http://mirrors.aliyun.com/pypi/simple/
[install]
trusted-host=mirrors.aliyun.com临时使用：
可以在使用pip的时候加参数-i ，例如：
pip install django -i http://mirrors.aliyun.com/pypi/simple新版本pip需要https，可以使用：
豆瓣源： https://pypi.douban.com/simple
清华源： https://pypi.tuna.tsinghua.edu.cn/simple
参考：
http://mirrors.aliyun.com/help/pypi
https://pypi.python.org/mirrors





