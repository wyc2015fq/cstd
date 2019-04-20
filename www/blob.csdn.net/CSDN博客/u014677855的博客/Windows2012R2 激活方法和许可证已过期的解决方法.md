# Windows2012R2 激活方法和许可证已过期的解决方法 - u014677855的博客 - CSDN博客
2019年01月30日 09:47:12[xiaobigben](https://me.csdn.net/u014677855)阅读数：3670
个人分类：[系统激活](https://blog.csdn.net/u014677855/article/category/8368791)

### Windows2012R2 激活方法和许可证已过期的解决方法
- [激活Windows2012的方法](#Windows2012_6)
- [许可证已过期的解决方法](#_24)
## 激活Windows2012的方法
虚拟机上面安装的Windows2012系统显示Windows未激活，试了好几种方法激活它，连续输了很多密钥都显示无效。最终终于找到了一种简单方便的方法激活了它。
- **激活工具下载**
下载地址：[激活工具](https://pan.baidu.com/s/1ZVUzGZcJ4HXibrDsFtqG0A)
提取码：tzt2
- **激活方法**
解压文件，打开2012R2激活工具，如下图。
![解压后的激活工具](https://img-blog.csdnimg.cn/2018111616400032.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=,size_16,color_FFFFFF,t_70)
打开windows server 2012 r2激活工具，运行KMSpico_setup.exe程序，安装KMSpico激活软件；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116164521964.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=,size_16,color_FFFFFF,t_70)
KMSpico激活软件安装完成；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116164535307.jpg)
直接点击红色的按钮进行激活即可；
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116164551908.jpg)
查看系统，显示已经激活。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116164829209.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=,size_16,color_FFFFFF,t_70)
## 许可证已过期的解决方法
之前安装的系统显示许可证已过期，系统每隔一个小时就自动关机。让人特别恼火。也是找了好久终于找到简单可行的方法。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190130093528767.png)
操作步骤：
- 
**获取可升级版本**
打开cmd，命令提示符，输入：
dism /online /Get-TargetEditions
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019013009355163.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=,size_16,color_FFFFFF,t_70)
- 
**根据查到的可升级版本，找一个对应的产品密钥，并更新**
dism /online /set-edition:ServerDatacenter /productkey:**-**-*****-********** /accepteula其中的****代表对象版本的密钥![在这里插入图片描述](https://img-blog.csdnimg.cn/20190130093627120.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=,size_16,color_FFFFFF,t_70)3.** 更新完成后，提示是否重启![在这里插入图片描述](https://img-blog.csdnimg.cn/20190130094152662.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=,size_16,color_FFFFFF,t_70)4  重启之后，解决问题![在这里插入图片描述](https://img-blog.csdnimg.cn/20190130094551326.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=,size_16,color_FFFFFF,t_70)**
