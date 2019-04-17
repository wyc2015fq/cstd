# jupyter notebook添加kernel - 浅梦的博客 - CSDN博客





2017年01月15日 23:04:54[浅梦s](https://me.csdn.net/u012151283)阅读数：12691








# pip install 参数

安装指定源`pip install -i https://pypi.douban.com/simple package name`

升级`pip install --upgrade package`

# jupyter notebook 添加 kernel

参考资料： 
[Jupyter增加内核](http://www.cnblogs.com/Jeffiy/p/4861528.html)
[add python3 kernel to jupyter](http://stackoverflow.com/questions/28831854/how-do-i-add-python3-kernel-to-jupyter-ipython)
这里以在python2环境下的notebook添加python3 kernel为例， 

首先，确保python3环境下安装了ipykernel 
`pip3 install ipykernel`

然后在python3的python.exe所在目录下执行 `python -m ipykernel install`

可以添加命令 `--name kernelname` 为kernel指定名字
# 查看jupyter notebook kernel

使用命令`jupyter kernelspec list`可以查看当前的kernel

# 删除notebook kernel

使用命令`jupyter kernelspec remove kernelname`





