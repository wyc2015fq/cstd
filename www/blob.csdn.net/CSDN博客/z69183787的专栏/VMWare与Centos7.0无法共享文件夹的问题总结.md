# VMWare与Centos7.0无法共享文件夹的问题总结 - z69183787的专栏 - CSDN博客
2018年02月07日 18:34:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：327
[http://blog.csdn.net/joson_yuan/article/details/71805269?utm_source=itdadao&utm_medium=referral](http://blog.csdn.net/joson_yuan/article/details/71805269?utm_source=itdadao&utm_medium=referral)
[http://blog.csdn.net/wear_/article/details/39896775](http://blog.csdn.net/wear_/article/details/39896775)
[http://blog.csdn.net/cindy_cheng/article/details/50456977](http://blog.csdn.net/cindy_cheng/article/details/50456977)
[http://blog.csdn.net/kelvindream/article/details/53503148](http://blog.csdn.net/kelvindream/article/details/53503148)
我使用的是win10系统，由于需要使用linux来执行神经网络的代码，遂在自己的pc上安装了vmware 12，并添加了centos 7.0。当我想使用vm-tools提供的共享文件夹功能来在centos中访问host中的文件时，发现在centos中根本找不到共享文件夹。
那么问题来了，找不到文件夹该怎么做？
1.检查是否在设置中添加了共享目录，如图中右下角红框所示：
![](https://img-blog.csdn.net/20170513161012691?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSm9zb25feXVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果没有添加，则需要先添加共享文件夹，添加前需保证centos处于关闭状态，否则无法编辑。等待添加完成后，开启centos，并查看能否找到共享目录，如果能找到，恭喜你，否则进入第二步。
2、更新vm-tools
vm-tools的更新方式如下图：
![](https://img-blog.csdn.net/20170513161123552?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSm9zb25feXVhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击菜单栏中的虚拟机选项，再点击更新VMWare Tools，按照提示，先卸载上一版本，再重新安装。
在安装过程中，我遇到了一个提示为：
# Enter the path to the kernel header files for the 3.18.0-kali1-686-pae kerne?
这个意思是说找不到kernel头文件，这其中的原因很可能是你的kernel的版本没有更新，在网上查找了很多的资料来解决，花费了大力气才搞好。
先按Ctrl+Z终止，再输入：
yum -y install kernel-devel-$(uname -r)
升级Kernel-devel，再次安装VMWare Tools便不会出现刚才的提示。安装好之后重启centos，可直接使用reboot命令。重启后如果出现了共享文件夹，那么恭喜你，如果没有，那么进入下一步。
3、挂在文件夹
最后，需要将文件夹挂载到centos下在命令行中输入如下命令：
mount -t vmhgfs .host:/ /mnt/hgfs
如果没有权限则需要切换到root用户
如果报错 ERROR: can not mount filesystem: No such device，则输入如下命令：
sudo
 vmhgfs-fuse .host:/ /mnt/hgfs
注意：挂载的文件夹必须是空文件夹，否则这里会报错。
执行后可到/mnt/hgfs/目录中查看，此时发现共享文件夹已存在，可以访问，恭喜你。如果还是没有，重启centos。
