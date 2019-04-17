# 074_解决VMWare与Hyper-V的冲突 - 小灰笔记 - CSDN博客





2019年03月09日 13:54:37[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：31








         如果使用Hyper-V安装了虚拟机之后，VM虚拟机启动的时候就会出现兼容性问题。这个即使是删除Hyper-V中的虚拟机也不会有任何改变，其实正确的解决方式是直接关闭Hyper-V的功能。自然，这是你若想用VM的时候，毕竟两者看上去只能够二选一。

         关闭的话，控制面板进入 程序的开关配置直接勾掉Hyper-V，进而重启即可。

![](https://img-blog.csdnimg.cn/20190309135434688.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         具体的修改就在上面的截图中了。



