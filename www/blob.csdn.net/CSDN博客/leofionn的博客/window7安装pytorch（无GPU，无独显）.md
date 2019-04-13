
# window7安装pytorch（无GPU，无独显） - leofionn的博客 - CSDN博客


2018年10月29日 08:34:53[leofionn](https://me.csdn.net/qq_36142114)阅读数：291


(1)  pip安装
pip install[http://download.pytorch.org/whl/cpu/torch-0.4.0-cp36-cp36m-win_amd64.whl](http://download.pytorch.org/whl/cpu/torch-0.4.0-cp36-cp36m-win_amd64.whl)
pip install torchvision
(2)这里还可以采用的方法是conda安装但是如果直接输入官网推荐的，我安装的时候给我的提示是找不到该包，采用的方法是
conda config --add channels[https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/win-64](https://mirrors.tuna.tsinghua.edu.cn/anaconda/pkgs/free/win-64)
conda config --set show_channel_urls yes
**完成后，敲入import torch无报错就成功了。**

