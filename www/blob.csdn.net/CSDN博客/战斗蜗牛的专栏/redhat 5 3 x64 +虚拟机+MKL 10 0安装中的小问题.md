# redhat 5.3 x64 +虚拟机+MKL 10.0安装中的小问题 - 战斗蜗牛的专栏 - CSDN博客





2017年11月04日 04:44:41[vbskj](https://me.csdn.net/vbskj)阅读数：129








在写论文过程中，需要把过去的老程序运行一遍，由于更换了win10，所以选择安装虚拟机vmware8.0+redhat5.5_x64的平台搭建方式。




系统安装完毕之后，通过虚拟机菜单安装vmare tools，以及设置共享目录。之后把mkl10.0的安装包拷贝到系统里进行安装。

1、切换到root用户，运行install.sh进行安装

2、要求输入license文件的绝对路径。输入后错误，发现原来是路径中有标点符号造成的，索性把license文件拷贝到桌面，直接load进去

3、这时出现了第二个错误，unable to find rpm tool, 之后提示安装失败

4、经过国外论坛查找，需要进入到install目录中直接运行install程序，而不是install.sh，同时给出命令参数 ./install --nonrpm

5、至此，MKL 10.0 安装成功！



