
# 在Ubuntu下安装jdk解压出现问题：.-jdk-6u30-linux-i586.bin: 113: .-install.sfx.3631: not found - 嵌入式Linux - CSDN博客

2013年09月11日 15:01:08[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：7048


在Ubuntu下安装jdk解压出现问题：./jdk-6u30-linux-i586.bin: 113: ./install.sfx.3631: not found。
Failed to extract the files.  Please refer to the Troubleshooting section of
the Installation Instructions on the download page for more information

网上搜索，发现是机器字长和jdk版本不匹配导致的，我的机子如果是64位的系统，然而选择了586的jdk。

之后到Oracle网站去下载了64位的jdk（jdk-6u43-linux-x64.bin），修改添加执行权限后，运行该文件，解压成功！

