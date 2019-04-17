# 【PyTorch】PyTorch中使用指定的GPU - zkq_1986的博客 - CSDN博客





2018年11月14日 14:22:57[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：76








## [PyTorch中使用指定的GPU](https://www.cnblogs.com/darkknightzh/p/6836568.html)

转载出处：

[http://www.cnblogs.com/darkknightzh/p/6836568.html](http://www.cnblogs.com/darkknightzh/p/6836568.html)



PyTorch默认使用从0开始的GPU，如果GPU0正在运行程序，需要指定其他GPU。

有如下两种方法来指定需要使用的GPU。

1. 类似tensorflow指定GPU的方式，使用CUDA_VISIBLE_DEVICES。

1.1 直接终端中设定：

CUDA_VISIBLE_DEVICES=1 python my_script.py
1.2 python代码中设定：

import os
os.environ["CUDA_VISIBLE_DEVICES"] = "2"
见网址：[http://www.cnblogs.com/darkknightzh/p/6591923.html](http://www.cnblogs.com/darkknightzh/p/6591923.html)

2. 使用函数 set_device

import torch
torch.cuda.set_device(id)
该函数见 pytorch-master\torch\cuda\__init__.py。

不过官方建议使用CUDA_VISIBLE_DEVICES，不建议使用 set_device 函数。



