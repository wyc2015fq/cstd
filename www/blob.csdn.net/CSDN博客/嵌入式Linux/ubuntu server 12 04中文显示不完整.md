
# ubuntu server 12.04中文显示不完整 - 嵌入式Linux - CSDN博客

2014年11月29日 13:37:39[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：1772


发现有台服务器ubuntu server 12.04在远程过去时文件名的中文现实没有问题，但ls的时候时间居然乱码。 搜索了一下，应该是locale的问题，可以这样处理：
1. 编辑/var/lib/locales/supported.d/local文件，内容改成如下：
> en_US.UTF-8 UTF-8

> zh_CN.UTF-8 UTF-8

> zh_CN.GBK GBK

> zh_CN GB2312
2. locale重新生成：
> sudo locale-gen
3. 修改缺省的编码为zh_CN.UTF-8
> nano /etc/default/locale
内容为：
> LANG=”zh_CN.UTF-8″

> LANGUAGE=”zh_CN:zh”

> LC_NUMERIC=”zh_CN.UTF-8″

> LC_TIME=”zh_CN.UTF-8″

> LC_MONETARY=”zh_CN.UTF-8″

> LC_PAPER=”zh_CN.UTF-8″

> LC_NAME=”zh_CN.UTF-8″

> LC_ADDRESS=”zh_CN.UTF-8″

> LC_TELEPHONE=”zh_CN.UTF-8″

> LC_MEASUREMENT=”zh_CN.UTF-8″

> LC_IDENTIFICATION=”zh_CN.UTF-8″
4. 退出，然后重新登陆进来应该就ok了。

我自己验证后发现，没有进行第三步和第四步也是可以正常显示了中文字符的。

