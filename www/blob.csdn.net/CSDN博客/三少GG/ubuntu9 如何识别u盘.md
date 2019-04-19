# ubuntu9 如何识别u盘 - 三少GG - CSDN博客
2010年04月28日 17:56:00[三少GG](https://me.csdn.net/scut1135)阅读数：1686标签：[ubuntu																[vmware																[虚拟机																[service																[linux																[windows](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=vmware&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
# 求助：虚拟机装Ubuntu，linux无法识别U盘
我用vmare 安装了Ubuntu 9.04，向用u盘从windows向linux传些东西，但插上u盘后在终端里输入fdisk -l后一直没有u盘选项。
**答：**把虚拟机下面的USB设置成直接给虚拟机用。
我的具体方法：
1.在控制面板管理工具 打开服务-->启动 VMware USB Arbitration Service （默认貌似未启动）
2.在VMware中VM-->settings-->USB controller 全打勾
3.重启虚拟机！ （这个很重要,可以power off 后power on）
以后每次开启虚拟机时先启动服务VMware USB Arbitration Service ,就不用那么麻烦了。
PS：
 网上的大部分解决方案很奇怪，不可信。
请注意，我说的是U盘识别问题，不是挂载问题，rmmod echi_hcd这种方法就不要再说了
