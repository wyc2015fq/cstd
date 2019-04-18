# pip安装jupyter - qq229873466的博客 - CSDN博客

2017年05月10日 19:55:57[qq229873466](https://me.csdn.net/qq229873466)阅读数：687


1.修改更新源。

~/.pip/pip.conf（没有文件夹/文件就创建）

```
[global]
index-url = https://pypi.tuna.tsinghua.edu.cn/simple

[install]
trusted-host=mirrors.aliyun.com
```

2.python2 只支持ipython5.X，下载[ipython5.X](https://pypi.python.org/pypi/ipython/5.0.0#downloads)

3.安装jupyter

```
python -m pip install --upgrade --force pip
sudo pip install setuptools==33.1.1
sudo pip install jupyter
```







