# 使用国内镜像源来加速python pypi包的安装 - =朝晖= - 博客园
# [使用国内镜像源来加速python pypi包的安装](https://www.cnblogs.com/dhcn/p/7106366.html)
zt from:[http://www.leadnt.com/2013/08/使用国内镜像源来加速python-pypi包的安装/](http://www.leadnt.com/2013/08/%E4%BD%BF%E7%94%A8%E5%9B%BD%E5%86%85%E9%95%9C%E5%83%8F%E6%BA%90%E6%9D%A5%E5%8A%A0%E9%80%9Fpython-pypi%E5%8C%85%E7%9A%84%E5%AE%89%E8%A3%85/)
pipy国内镜像目前有：
http://pypi.sdutlinux.org/ 山东理工大学
http://pypi.douban.com/ 豆瓣
http://e.pypi.[Python](http://lib.csdn.net/base/python).org/ 清华大学
http://pypi.hustunique.com/ 华中理工大学
http://pypi.mirrors.ustc.edu.cn 中国科学技术大学
对于pip这种在线安装的方式来说，很方便，但网络不稳定的话很要命。使用国内镜像相对好一些，
如果想手动指定源，可以在pip后面跟-i 来指定源，比如用豆瓣的源来安装web2py：
```
pip install web2py -i http://pipy.douban.com/simple
```
注意后面要有/simple目录。
要配制成默认的话，需要创建或修改配置文件（[Linux](http://lib.csdn.net/base/linux)的文件在~/.pip/pip.conf，windows在%HOMEPATH%\pip\pip.ini），修改内容为：
```
index-url = http://pypi.douban.com/simple
```
这样在使用pip来安装时，会默认调用该镜像。
更多配置参数见：[http://www.pip-installer.org/en/latest/configuration.html](http://www.pip-installer.org/en/latest/configuration.html)
pipy不完全源服务器的状态，可以参考：[http://www.pypi-mirrors.org/](http://www.pypi-mirrors.org/)

