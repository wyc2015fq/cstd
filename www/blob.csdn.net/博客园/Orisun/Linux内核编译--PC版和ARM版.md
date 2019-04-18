# Linux内核编译--PC版和ARM版 - Orisun - 博客园







# [Linux内核编译--PC版和ARM版](https://www.cnblogs.com/zhangchaoyang/articles/1915253.html)





首先请区别一下Linux内核的编译和内核模块的编译。

顶层Makefile文件（在源代码根目录下的Makefile文件）负责产生内核映像vmlinuz和模块。顶层Makefile根据内核配置，递归编译内核代码下所有子目录里的文件，最终建立内核文件映像。每个子目录下有个Makefile文件，根据上级目录的makefile配置编译指定的代码文件。这些Makefile使用.config文件配置的数据构建各种文件列表，最终生成目标文件或者内嵌模块。

编译配置好之后,进入Linux源文件所在的文件夹

使用make bzImage命令完成内核编译,此时会生成vmlinuz文件

make modules编译内核需要的模块

make modules_install安装模块到/lib/modules目录

make install安装内核文件到/boot目录

安装过程会自动修改GRUB启动菜单




# PC更换Linux内核版本

1.去www.kernel.org上下载linux-2.6.37.tar.bz2。解压。

2.建立符号链接

#ln -s
/home/orisun/Download/linux-2.6.37 /usr/src/linux-2.6.37

3.内核编译配置

#cd
/usr/src/linux-2.6.37

#make
mrproper <--删除以前的.o文件，第一次运行其实这步可以省略

#make
menuconfig

#make
dep	   <--建立依赖关系，第一次运行其实这步可以省略

#make
clean    <--删除没用的文件

4.编译

#make
bzImage		<--编译内核

#make
modules		<--编译模块

#make
modules_install		<--安装模块

#make
install		<--建立initrd（加载LKM用的程序）

5.更改GRUB

#new-kernel-pkg
–install –mkinitrd –depmod 2.6.37  	<--写入grub.conf

6.重启，选择运行新的内核

#shutdown
-r now



# 编译开发板的Linux内核

1.首先获取交叉编译工具链arm-linux-gcc

2.下载解压linux-2.6.30.4.tar.bz2


3.参见《Linux移植.pdf》,step3和step4中所说的修改源文件不需要做了，在天嵌提供的linux-2.6.30.4中这些文件已经改过来了

4.在linux源文件下执行make ARCH=arm CROSS_COMPILE=arm-linux- menuconfig,按照附录5配置linux2.6.30.4编译选项，“Device Drivers"下的选项参照step8.2配置就可以了。配置完毕后注意保存，参见5.5

5.make zImage编译内核

如果没有事先安装交叉编译工具链本步会提示找不到命令arm-linux-gcc

编译内核文件后会生成vmlinuz等文件

6.把镜像文件存到指定位置

step7.2中所述的arch/arm/boot/Makefile文件不用改了，已经改过来了，只改内核源文件根目录下的Makefile文件就可以了















