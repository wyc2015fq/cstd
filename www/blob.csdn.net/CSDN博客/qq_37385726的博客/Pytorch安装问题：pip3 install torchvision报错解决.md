# Pytorch安装问题：pip3 install torchvision报错解决 - qq_37385726的博客 - CSDN博客





2018年08月16日 16:44:59[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：3100








# **目录**



[问题描述](#%E9%97%AE%E9%A2%98%E6%8F%8F%E8%BF%B0)

[段子](#%E6%AE%B5%E5%AD%90)   （不想看的直接到解决即可，不过还是良心安利，挺搞笑的2333）

[解决](#%C2%A0%E8%A7%A3%E5%86%B3)

# 问题描述

> 
Collecting torchvision

Using cached torchvision-0.1.8-py2.py3-none-any.whl

Collecting torch (from torchvision)

Using cached torch-0.1.2.post1.tar.gz

Complete output from command python setup.py egg_info:

Traceback (most recent call last):

File "<string>", line 1, in <module>

File "/tmp/pip-build-hho2Nf/torch/setup.py", line 11, in <module>

raise RuntimeError(README)

RuntimeError: PyTorch does not currently provide packages for PyPI (see status at [#566](https://github.com/pytorch/pytorch/issues/566)).

Please follow the instructions at [http://pytorch.org/](http://pytorch.org/) to install with miniconda instead.

----------------------------------------

Command "python setup.py egg_info" failed with error code 1 in /tmp/pip-build-hho2Nf/torch/


问题我看到的是raise RuntimeError(README)

RuntimeError: PyTorch does not currently provide packages for PyPI

(see status at  https://github.com/pytorch/pytorch/issues/566).

于是，我去了[https://github.com/pytorch/pytorch/issues/566]()找答案



# 段子

在找答案的过程中，看到一个留言很搞笑，现粘贴过来笑一笑

![](https://img-blog.csdn.net/20180816164033371?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

大意就是，他也遇到了这个问题，等待了一个多月都没有解决，在这一个月了，他在Tensorflow上很舒适，弃了Pytorch 23333



#  解决

再往下的留言中有解决方式

> 
### 不使用pip3 install torchvision

### 而使用 pip install --no-deps torchvision


完美解决~



