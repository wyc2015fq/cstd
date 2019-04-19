# VirtualBox上安装64位系统 - =朝晖= - 博客园
# [VirtualBox上安装64位系统](https://www.cnblogs.com/dhcn/p/7105642.html)
[http://blog.csdn.net/mal327/article/details/6597263](http://blog.csdn.net/mal327/article/details/6597263)
原来VirtualBox安装64位的系统需要满足以下条件：
1.64位的cpu
2.安装的系统必须为64位
3.cpu允许硬件虚拟化
前面两个条件都满足，那就是第三个条件的问题了。
这里补充说明：
硬件虚拟化其实就是CPU的虚拟化技术。intel的叫VT-x，amd的叫AMD-V。支持虚拟技术的CPU带有特别优化过的指令集来控制虚拟过程，通过这些指令集，VMM(Virtual Machine Monitor，虚拟机监视器)会很容易提高性能，相比软件的虚拟实现方式会很大程度上提高性能。虚 拟化技术可提供基于芯片的功能，借助兼容VMM软件能够改进纯软件解决方案。由于虚拟化硬件可提供全新的架构，支持操作系统直接在上面运行，从而无需进行 二进制转换，减少了相关的性能开销，极大简化了VMM设计，进而使VMM能够按通用标准进行编写，性能更加强大。
如何判断是否支持硬件虚拟化呢？
1.在window下，利用一个小工具 securable.exe 来帮助我们测试硬件对虚拟化技术的支持程度。我们程序运行结果中可以看到物理机支持 64 位运算，支持硬件 DEP，支持虚拟化技术。这样的结果表明我们可以放心测试 XP MODE，如果“ Hardware Virtualization ”显示的结果是“ Locked OFF ”，则表明 CPU 支持虚拟化技术，但主板的 BIOS 却禁止了对虚拟化的支持，这种情况我们就需要更改 BIOS 设定或升级 BIOS。如果三项检测结果中有一项显示了“NO”，那就表明您的硬件不能满足 XP MODE 的部署条件，就不用继续测试了。
2.在Linux下，使用命令：grep -E '(vmx|svm)' /proc/cpuinfo。如果有vmx或svm内容输出，则说明CPU支持虚拟化技术。
重启进BIOS，找到virtualization的一项,我的BIOS中在Advanced-cpu setup-virtualization，现为Disabled状态，改为Enable，重启进入系统。
重新检测，发现已经支持硬件虚拟化。
可是安装还是出错，为什么呢？
原来新建虚拟电脑时，**要安装64位的ubuntu可是没有对应选择64位Ubuntu**，更改设置即可。
这里还要注意一点：要打开VirtualBox的vt-x/amd-v设置，这个其实会自动打开。
重新安装，大功告成了！

