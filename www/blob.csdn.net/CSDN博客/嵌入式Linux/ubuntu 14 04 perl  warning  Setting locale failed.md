
# ubuntu 14.04 perl: warning: Setting locale failed. - 嵌入式Linux - CSDN博客

2015年09月22日 09:35:05[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：4009


perl: warning: Setting locale failed.
perl: warning: Please check that your locale settings:
LANGUAGE = "en_HK:en",
LC_ALL = (unset),
LC_PAPER = "zh_CN.UTF-8",
LC_ADDRESS = "zh_CN.UTF-8",
LC_MONETARY = "zh_CN.UTF-8",
LC_NUMERIC = "zh_CN.UTF-8",
LC_TELEPHONE = "zh_CN.UTF-8",
LC_IDENTIFICATION = "zh_CN.UTF-8",
LC_MEASUREMENT = "zh_CN.UTF-8",
LC_TIME = "zh_CN.UTF-8",
LC_NAME = "zh_CN.UTF-8",
LANG = "en_HK.UTF-8"


修改~/.bashrc
在最底部添加　LC_ALL=C　所以可以这样运行
qifa@ts-server:~$ echo "export LC_ALL=C">>~/.bashrc
qifa@ts-server:~$ source ~/.bashrc



