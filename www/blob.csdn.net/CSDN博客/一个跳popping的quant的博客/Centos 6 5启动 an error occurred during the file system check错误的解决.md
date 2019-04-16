# Centos 6.5启动 an error occurred during the file system check错误的解决 - 一个跳popping的quant的博客 - CSDN博客





2017年09月02日 16:47:21[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：771








今天在配置hadoop集群的时候，将一台虚拟机克隆了两台，给这两台虚拟机进行配置hosts和主机与ip地址映射之后，重启了一下系统，然后再次打开，就出现了an error occurred during the file system check提示，提示要么输入root密码进行修复，要么按ctrl+D跳过。一开始还以为自己修改错了什么东西，结果在打开第一台虚拟机时，发现也出现了这个问题，于是就百度了一下，原因是文件系统出现了错误。而具体的什么错误，现在还没弄明白，但是用了[http://blog.csdn.net/zs15yy/article/details/53606399](http://blog.csdn.net/zs15yy/article/details/53606399)的方法，就弄好了。

解决办法： 

输入密码进行修复 

执行指令：fsck -y 

运行完后reboot 就可以了
ps：linux命令：fsck

功能说明：检查文件系统并尝试修复错误。

语　　法：fsck [-aANPrRsTV][-t <文件系统类型>][文件系统…]

补充说明：当文件系统发生错误四化，可用fsck指令尝试加以修复。

参　　数： 

-a      自动修复文件系统，不询问任何问题。 

-A      依照/etc/fstab配置文件的内容，检查文件内所列的全部文件系统。 

-N      不执行指令，仅列出实际执行会进行的动作。 

-P      当搭配”-A”参数使用时，则会同时检查所有的文件系统。 

-r      采用互动模式，在执行修复时询问问题，让用户得以确认并决定处理方式。 

-R      当搭配”-A”参数使用时，则会略过/目录的文件系统不予检查。 

-s      依序执行检查作业，而非同时执行。 

-t<文件系统类型>      指定要检查的文件系统类型。 

-T      执行fsck指令时，不显示标题信息。 

-V      显示指令执行过程。
用fsck检查文件系统完整性 

文件系统很复杂，因此易于发生错误。可以用fsck 命令检查文件系统是否正确和有效。它可以根据指令修复找到的小错误，并将未修复错误报告用户。幸运的是，文件系统的代码非常有效，所以根本极少出现问题，并且问题通常原因是电源失败、硬件失败、或操作错误，例如没有正常关闭系统。 

借鉴[http://blog.csdn.net/johnstrive/article/details/25981973](http://blog.csdn.net/johnstrive/article/details/25981973)





