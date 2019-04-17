# vim config - V__KING__的专栏 - CSDN博客





2013年12月25日 20:13:17[v__king__](https://me.csdn.net/V__KING__)阅读数：1456









ubuntu系统装好之后，vi编辑工具默认情况下使用起来特别不好使，我们可以通过以下步骤改善



第一步:下载vim文本编辑工具,并且安装




sudo apt-get install  vim




第二步:配置vim

1.下载jeffy-vim-read.tar.gz配置脚本,这个可以在网上搜索下载

2.解压jeffy-vim-read.tar.gz




tar -zxvf jeffy-vim-read.tar.gz




3.解压完成 后，进入jeffy-vim-read目录




cd jeffy-vim-read




4.执行install.sh,会出现以下界面




./install.sh






等待几分钟，会自动从网络上下载一些插件来配置vim，特别方便




5.最后安装一下ctags插件，就完全配置好了




sudo apt-get install ctags




6.测试下,通过vi 打开一个C语言的.c文件，如下







最后附上 jeffy-vim-read.tar.gz的脚本，方便大家下载

 jeffy-vim-read.zip 








