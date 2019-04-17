# virtualbox虚拟机ubuntu和宿主机xp文件件共享方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年12月05日 09:56:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2942








1.打开VirtualBox启动unbuntu虚拟机后：

  1）菜单：设备->安装增强功能，自动打开一个cd，自动运行直到安装完成后弹出cd；

   2）菜单：设备->分配数据空间->固定分配，设置一个xp下指定目录的共享文件控件，如d:\sharedata，命名为sharedata；

  重新启动VirtualBox和unbuntu虚拟机；

2.进入unbuntu虚拟机终端窗口，执行命令：

  1）sudo mkdir /mnt/shared，在ubuntu下建立目录/mnt/shared的共享文件夹；

  2 ）sudo mount -t  vboxsf sharedata /mnt/shared，挂载Virtualbox数据控件上命名为sharedata的xp共享文件夹在ubuntu的/mnt/shared文件夹；

3.互相访问：

  1）xp下，打开d:\sharedata即可读写；

  2）ubuntu下，打开/mnt/shared即可读写，终端命令cd /mnt/shared，然后ls就可以看到文件夹内的文件；



