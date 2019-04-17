# VMPlayer Ubuntu 16.04 Copy and Paste with Host 主机与宿机之间的复制粘贴 - Grandyang - 博客园







# [VMPlayer Ubuntu 16.04 Copy and Paste with Host 主机与宿机之间的复制粘贴](https://www.cnblogs.com/grandyang/p/8309184.html)







使用Ubuntu的虚拟机时如果不能主机之间进行复制粘粘，会非常非常的不方便，所以我们需要安装vmware tools，使用如下的代码（注意第二句一定要有，不然还是不能复制粘贴）：



```
sudo apt-get install open-vm-tools
sudo apt-get install open-vm-tools-desktop
sudo reboot
```














