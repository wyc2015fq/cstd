# 【Scipy】在Windows上配置Scipy环境 - CD's Coding - CSDN博客





2016年12月01日 23:05:49[糖果天王](https://me.csdn.net/okcd00)阅读数：1936








配环境真乃万事开头难的典范……

Scipy是我配了快一万年的东西……

常用的像是Sklearn都需要包含这个包

（唔，确实对于大多数人来说很简单，但是吾辈是真的经历了千辛万苦，甚至还使用Anocoda来临时用以跑程序）

今天，终于成功了QAQ




事前准备：

pip install wheel

WHEEL，用来安装whl文件的程序




http://www.lfd.uci.edu/~gohlke/pythonlibs/#numpy


NUMPY+MKL，请务必注意适合自己的版本，如Python2.7


- numpy‑1.11.2+mkl‑cp27‑cp27m‑win32.whl
- numpy‑1.11.2+mkl‑cp27‑cp27m‑win_amd64.whl
- numpy‑1.12.0b1+mkl‑cp27‑cp27m‑win32.whl
- numpy‑1.12.0b1+mkl‑cp27‑cp27m‑win_amd64.whl






http://www.lfd.uci.edu/~gohlke/pythonlibs/#scipy


SCIPY，请务必注意适合自己的版本，如Python2.7，并且请在NUMPY安装完毕后安装


- scipy‑0.18.1‑cp27‑cp27m‑win32.whl
- scipy‑0.18.1‑cp27‑cp27m‑win_amd64.whl









whl文件的安装方式：


`wheel install xxxxx.whl`




