
# VS不生成.vhost.exe .pdb文件方法 - 追求卓越,做到专业 - CSDN博客


2015年11月18日 11:27:07[Waldenz](https://me.csdn.net/enter89)阅读数：1216


使用Visual Studio编译工程时，默认设置下，即使选择了「Release」时也会生成扩展名为「.vshost.exe」和「.pdb」的文件。
![](https://jpghlq.blu.livefilestore.com/y2pMj_4-RNGrZFMo9N-WUjHLXZiGB8p19JY6yccOw6rPBkjUWss23pY3Q38L3XGbXVwnswk7EOjOfUezVqOGi1AN4oad2JsHEtb_qU8TDs3VHc/files-in-release-folder.jpg)

### 一、先解释一下各个文件的作用：
.pdb文件：
程序数据库文件（Program Database File）。默认设置下，Debug的PDB是full，保存着调试和项目状态信息、有断言、堆栈检查等代码，可以对程序的调试配置进行增量链接。
而Release的默认设置对PDB指定为pdb-only，保存着程序出了什么错误，以及错误在哪行。
.vshost.exe文件：
宿主进程文件（VS host process），是Visual Studio 2005中的一项功能，主要是为了提高调试性能。 release时最好删除。
.vshost.exe.manifest 文件：
是一个以.manifest为后缀的XML文件，用于组织和描述隔离应用程序及并行组件，用于COM类、接口及库的绑定和激活，而这些信息，以往都是存储在注册表中的。
Manifests也制定了组成程序集的文件及Windows类，release 时也最好删除。
### 二、不生成这些文件的方法：
不生成.vshost.exe文件的方法
如图，打开工程属性的debug标签页，将构成选为「Release」，去掉「Visual Studio Hosting Process有效」的勾选后保存。
![](https://jpghlq.blu.livefilestore.com/y2pQlA5s5BeLtJkY6fL6hYHnFBZCMrDN7WRrppj5_WzCb9dELqd_XbJzXH2Vd5wh9_EKoS5BACIRAF3ZDgjHOpBHA-0OXrJoxtuX1MdxHYG6RA/config-debug.png)
不生成.pdb文件的方法
如图，打开工程属性的build标签页，将构成选为「Release」后打开「详细设定」。将 「输出」中的「Debug信息」从「pdb only」变为「none」后保存。
![](https://jpghlq.blu.livefilestore.com/y2pAXeGxErhE-zemx7REq-wmz8irXok8av8FI9pdJi-tXR3u5h1SeFJZkpyttTqkEkIDO8QdXTojVvtLrAiXcMEbJXoZlZ3epnEZHmEyNA4xuA/config-build.png)
![](https://jpghlq.blu.livefilestore.com/y2p4HS83vgVZd7WNut6Z5fHjip_T5ixfr01I7uJgXWFkXTupL2qYSw6CA3_nU6hOmOPoSxynylF2SILLhv6CL-jVkNSpTrwnBjUDLjuCiaX1NI/config-build-advanced.png)


