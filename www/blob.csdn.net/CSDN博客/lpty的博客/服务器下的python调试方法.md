
# 服务器下的python调试方法 - lpty的博客 - CSDN博客

2017年07月07日 23:15:56[lpty](https://me.csdn.net/sinat_33741547)阅读数：1929



## 前言
由于开发环境与服务器部署环境不一致，有时候总会遇到一些奇怪的问题，这是如果可以进行单步调试，对于解决问题将有极大帮助。
## [安装方式](http://blog.csdn.net/sinat_33741547/article/details/74781591)
## 调试方法
### 单步调试
`python -m ipdb xxx.py`
### 断点
在需要打断点的地方加上下述代码
`from ipdb import set_trace
set_trace()`运行到目标位置中断程序，出现提示符，进入ipython环境
### 常用命令
n（下一步）
ENTER（重复上次命令）
c（继续）
s（进入子程序）
p 变量 （打印变量）
l（当前位置）
r（运行直到程序结束）
q（退出）

