
# VirtualBox 扩展C盘 - 致林 - 博客园






# [VirtualBox 扩展C盘](https://www.cnblogs.com/bincoding/p/5718746.html)
**1. 关闭虚拟机，找到ovi文件**
![](//images2015.cnblogs.com/blog/771778/201608/771778-20160801143151372-1878128394.png)
**2.执行命令：**
C:\Program Files\Oracle\VirtualBox>VBoxManage.exe modifyhd "C:\Users\claudchen\VirtualBox VMs\server2012\server2012.vdi"--resize 51200
//执行完毕后进入系统查看Disk management,如果可以extend disk就直接扩展
//执行完毕后如果不能识别新添加的磁盘，可能需要删除系统快照
**3.如果步骤2没用，删除原虚拟机，****选择保留内容**
![](https://images2015.cnblogs.com/blog/771778/201608/771778-20160802084056106-933841660.png)
**4.新建虚拟机，选择已有虚拟磁盘**
![](https://images2015.cnblogs.com/blog/771778/201608/771778-20160802084211465-676443668.png)
**5.进入磁盘管理，扩展c盘**
![](//images2015.cnblogs.com/blog/771778/201608/771778-20160801110813122-1331873097.png)
warn:扩容后可能会丢失部分内容，如Sharepoint的site collection。





