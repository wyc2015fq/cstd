
# defconfig、 .config - 嵌入式Linux - CSDN博客

2019年02月21日 10:28:28[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：65个人分类：[Makefile																](https://blog.csdn.net/weiqifa0/article/category/8686212)



最近在裁剪内核慢慢关注这个问题
# defconfig、 .config
defconfig 一般在arch/arm64/configs/目录下，是一个简单的内核配置，是没有展开的。
.config一般是用来执行make menuconfig的基础配置
从.config到defcong不是简单的复制操作，而是make arch=arm64 defcong
通过menuconfig生成的.confg也不是直接拷贝成defcong，而是使用make savedefconfig
The`.config`file is not simply copied from your`defconfig`file. The motivation for storing`defconfig`in such a format is next: in`defconfig`we can only specify options with non-default values (i.e. options we changed for our board). This way we can keep it small and clear. Every new kernel version brings a bunch of new options, and this way we don't need to update our`defconfig`file each time the kernel releases. Also, it should be mentioned that kernel build system keeps very specific order of options in`defconfig`file, so it's better to avoid to modify it by hand. Instead you should use`make savedefconfig`rule.
## Simplified explanation
When`.config`file is being generated, kernel build system goes through all`Kconfig`files (from all subdirs), checking all options in those`Kconfig`files:
if option is mentioned indefconfig, build system puts that option to.configwith value chosen indefconfig
if option isn't mentioned indefconfig, build system puts that option to.configusing its default value, specified in correspondingKconfig
Check[scripts/kconfig/Makefile](http://lxr.free-electrons.com/source/scripts/kconfig/Makefile#L98)and[scripts/kconfig/conf.c](http://lxr.free-electrons.com/source/scripts/kconfig/conf.c#L665)files to see how it's actually done.
具体查看
[https://stackoverflow.com/questions/41885015/what-exactly-does-linux-kernels-make-defconfig-do](https://stackoverflow.com/questions/41885015/what-exactly-does-linux-kernels-make-defconfig-do)

