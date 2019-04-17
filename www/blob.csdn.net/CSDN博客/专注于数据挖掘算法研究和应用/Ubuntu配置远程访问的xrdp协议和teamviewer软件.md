# Ubuntu配置远程访问的xrdp协议和teamviewer软件 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年11月29日 15:50:55[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4892标签：[ubuntu																[service																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)





 先启用远程设置：System->Preferences->Remote Desktop

1.ubuntu安装xrdp协议：

  1)安装：sudo apt-get install xrdp

  2)修改配置：进入/etc/xrdp 修改xrdp.ini 和 sesman.ini，一般保持默认就行；

  3)重启xrdp服务：

    命令：sudo service xrdp start 来启动

          sudo service xrdp stop 停止

          sudo /etc/init.d xrdp restart

3.ubuntu安装teamviewer的linux版：

  1)下载地址：[http://www.teamviewer.com/zhCN/download/index.aspx](http://www.teamviewer.com/zhCN/download/index.aspx)

  2)下载teamviewer_linux.deb；

  3)命令：sudo dpkg -i teamviewer_linux.deb；](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)




