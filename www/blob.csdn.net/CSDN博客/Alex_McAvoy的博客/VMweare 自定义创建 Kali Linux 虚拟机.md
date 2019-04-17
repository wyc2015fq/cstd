# VMweare 自定义创建 Kali Linux 虚拟机 - Alex_McAvoy的博客 - CSDN博客





2018年06月08日 00:01:13[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：348
个人分类：[Linux 学习笔记](https://blog.csdn.net/u011815404/article/category/7715914)









## 【准备工作】




工具：VMware Workstation Pro

来源：[https://www.vmware.com/cn.html](https://www.vmware.com/cn.html)

 镜像下载：[https://www.kali.org/downloads/](https://www.kali.org/downloads/)





## 【创建步骤】




### 1.创建新虚拟机

![](https://img-blog.csdn.net/20180206235028817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### 2.选取自定义安装

![](https://img-blog.csdn.net/20180206235136216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




### 3.选取兼容性

![](https://img-blog.csdn.net/20180206235202914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

说明：

 ①兼容性，是指所创建虚拟机，可被不同版本的VMware运行的情况，比如：兼容版本是Workstation 4.x，那么就需要4.x以上的VMware软件才能打开此虚拟机。

 ②兼容的版本越高，则可运行此虚拟机的VMware软件就越少，此虚拟机的限制也越少，性能越强；反之，兼容的版本越低，则可运行此虚拟机的VMware软件就越多，此虚拟机的限制也越多，性能越弱。

 ③通常情况下，如果只是在本机使用，选择兼容本机Workstation的版本即可；如果考虑到所创建的虚拟机，可能要在其它机器上打开，可以适当降低其兼容的版本。


### 4.选取安装来源
![](https://img-blog.csdn.net/20180207000532330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



此处选取稍后安装操作系统。



### 5.选取操作系统类型

![](https://img-blog.csdn.net/20180207000900058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


说明：

 ①对于Linux来说，它有很多发行版，各版本均是由内核、系统软件、应用软件三部分组成。

②内核是统一的，但具有不同的版本，而系统软件、应用软件，则有不同的类型，同样有不同的版本。内核、系统软件、应用软件之间的组合，就构成了一个发行版。

 ③下拉菜单中，我们可以看到各种各样的发行版。它们各有特点，我们根据自己的需要，下载相应镜像，安装所需要的版本。



### 6.选取虚拟机名称与位置



![](https://img-blog.csdn.net/20180207002140608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

说明：

 ①名字可以自己修改成自己喜欢的名字，它将用来标识Workstation所打开的虚拟机。

 ②保存路径，是指虚拟机文件的保存路径。我们所创建的虚拟机的所有信息，需要为这些文件指定存储路径。在这里，可选取除系统盘外的任何盘，如有需要，可以选择D盘（数据存储越接近内圈运行越快）。



### 7.配置虚拟机硬件

（1）配置CPU

![](https://img-blog.csdn.net/20180207003552718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20180207003604977?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


说明：
①总CPU核数=处理器数量(P) X 每个处理器的核心数量(C)

②总CPU核数，不要超过虚拟机硬件的最大支持数。

③总CPU核数，不要超过真实的物理主机的CPU核数。



（2）配置内存

![](https://img-blog.csdn.net/20180207004136265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

说明：虚拟机对内存的限制为4MB~64GB，设置的值，在这个限制范围内，同时不要超过真实的物理内存大小即可。

（3）配置网络（默认即可）



![](https://img-blog.csdn.net/20180207004450429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


（4）配置IO控制器类型（默认即可）

![](https://img-blog.csdn.net/20180207004616236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



说明：
 ①IO，就是计算机上的输入、输出设备，其中，I代表输入，O代表输出。

 ②常见的IO设备有，硬盘、光驱等。控制这些设备与计算机进行交互的功能单元，称为IO控制器，

（5）配置虚拟磁盘类型（默认即可）
![](https://img-blog.csdn.net/20180207004920384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


（6）创建一新的磁盘


![](https://img-blog.csdn.net/20180207005101918?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



（7）分配磁盘容量


![](https://img-blog.csdn.net/20180207005250189?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



说明：
 ①所谓的磁盘大小其实只是一个文件与真实硬盘无太大关联，推荐值以上可以随意安排。

 ②在这里，我们不勾选“选择是否立即分配空间”。因为一旦选上，即便虚拟机磁盘中没有存储任何信息，也会在生成20GB大小的文件。如果不选，那么，虚拟机磁盘存储多大的数据，就会分配相应大小的文件空间，进行存储。 

③磁盘文件的存储方式的选择在这里，我们选择第一种方式。

 如果选择“单个存储”，则虚拟机磁盘的所有数据，存储在1个Host文件之中；如果选择“分割存储”，则虚拟机磁盘存储的数据，将会保存在若干的Host文件之中。

分割存储的方式，在将虚拟机复制到其它机器时，速度更快，因为多个文件一起拷贝，可并行操作，但是，文件很多的话，与单个文件存储方式相比，虚拟机的IO性能会降低。


（8）设置虚拟机磁盘文件名称


![](https://img-blog.csdn.net/20180207005942029?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


说明：默认即可，其后缀是vmdk，即：virtualmachine disk的缩写。

（9）完成配置


![](https://img-blog.csdn.net/20180207010106096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTgxNTQwNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


说明： 至此，配置完毕，此页面中可以看到各种配置信息。对于这些配置，后续过程中，还可以进行修改。


### 8.配置ISO镜像



![](https://img-blog.csdn.net/20180207221647786)

说明： 虚拟机各项参数选择完成后，需要对系统设置进行编辑，由于之前没有指定操作系统，所以此处要配置ISO映像文件。选择自定义硬件，将下载好的镜像文件导入，点击“完成”就建好了一个新的虚拟机，接下来就要安装linux操作系统了。如果此处也不指定，在启动操作系统时会提示加入系统文件。



