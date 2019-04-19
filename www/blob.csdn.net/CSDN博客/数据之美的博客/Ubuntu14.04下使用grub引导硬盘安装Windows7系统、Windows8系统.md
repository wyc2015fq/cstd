# Ubuntu14.04下使用grub引导硬盘安装Windows7系统、Windows8系统 - 数据之美的博客 - CSDN博客
2017年12月24日 15:17:19[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：263
个人分类：[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)

Ubuntu14.04下使用grub引导硬盘安装Windows7系统、Windows8系统,完全不需要U盘移动硬盘之类的东西，只需要从网上下个windows镜像就可以了。
在Window下安装Ubuntu系统比较容易，现我想在Ubuntu14.04下引导安装Windows系统，首先当然是各种百度+google，搜到了几篇文章，无奈这几篇文章都太旧了，照着捣鼓了两天头都大了。不过总算一步一步慢慢前进，最终搞定。现在总结一下，大致将这两天的工作分为三个大步骤：
准备工作：下载一个Windows镜像（最好是安装版，不要是Ghost版，后面我会解释为什么）。
一、Ubuntu下分区
1.参考 http://www.linuxidc.com/Linux/2010-01/23827p3.htm 这篇文章讲解的很清楚，我这里只说一点，Ubuntu下现在已经集成gparted工具了，不用再搞什么gparted镜像了。
直接命令:sudo gparted 就可以启动界面，如果没有使用其提示的命令安装即可。
我分出了/dev/sda4和/dev/sda5两个区并格式化为了ntfs格式给windows用，将这两个区分别挂载到了media/C和media/D下，实际上C和D就是两个文件夹。
2.关于如何开机自动把window的区挂载到Ubuntu
参考:http://blog.csdn.net/wzy_1988/article/details/8590836
就是添加要挂载的信息到/etc/fstab  文件中。
二、挂载iso镜像并拷贝里面的内容到D盘
其实挂载iso镜像就是解压，把里面的文件解压出来临时放到D盘（只是临时，等系统装完就可以把D盘下的这些东西删除了）下，因为我们要将window系统装到C盘。
第1步：
sudo mkdir /media/temp
sudo mount -o loop windows7.iso /media/D
#windows7.iso是你镜像的名称，media/D
 在Ubuntu下是一个文件夹，在window是下就是D盘。
第2步：
sudo
 cp -rv /media/temp/* /media/D
#如上面所说，这句命令就是将前面挂载（解压）的文件，全部拷贝到D盘下。
三、添加windows到引导界面
有两种方法。
1.可以修改grub脚本，位于/boot/grub/grub.cfg.
添加：
```
menuentry "Windows 7 (loader) (on /dev/sda5)" {
insmod part_gpt
insmod ntfs
set root='(hd0,gpt5)'
chainloader efi/boot/bootx64.efi
}
```
2.也可以进入grub界面后（重启电脑会看到这个节面），再按c进入grub命令行
```
insmod part_gpt
insmod ntfs
set root='(hd0,gpt5)'
chainloader efi/boot/bootx64.efi
```
boot
注意：这里是这整个过程的核心，最难搞的地方，倒数第二条指令我在其它文章里面看到的大多是chainloader +1 或者ntldr /bootmgr，我尝试了无数次都不行，后来看到到也有人是使用 chainloader (hd0,4)/efi/Microsoft/Boot/bootmgfw.efi，这样一条指令。直觉告诉我应该是最后这个指令，chainloader
 后面的路径应该是解压后镜像的目录，可是我下的是Ghost版根本没有那个目录，然后又去网上查了下，刚好看到关于windows8镜像目录的一个介绍，原来有efi这个目录，于是赶紧下了window8的iso镜像。（windows7肯定也有，不过我没再花时间去找window7的镜像，因为好累啊，反正刚好看到window8的下载地址就直接下了）。然后我去找了下efi目录，发现这个efi/boot/bootx64.efi文件比较像，上面说的那个efi/Microsoft/Boot/bootmgfw.efi，在那个目录下没有。所以还是略微有差异。
我猜测32win7、64win7、32win8、64win8应该会稍有不同，如果你是其他版本的windows自己猜测一下，应该有些规律。
然后就安装ok啦。哎，说起来好像也挺简单的，无奈自己花了好多时间，希望对大家有帮助。
PS：
1.hd0,gpt5，是指你将解压的镜像文件放到哪个区了，比如我是，dev/sda5，那我对应的地址就是（hd0，gpt5）.这两个表示方式具体的联系和区别我就不多讲了，自己不是很了解，免的误人子弟。
2，再强调一遍，chainloader这句才是关键啊（不同版本的地址可能不一样）
`chainloader efi/boot/bootx64.efi`
以下是参考资源虽然文章里面提到的chainloader +1 或者ntldr /bootmgr这样的指令对我没用帮助，但其它的内容还是挺正确的。（ntldr据说是vista上面用的，现在都不用这个了，不知真假）。
参考资源：
1.http://forum.ubuntu.org.cn/viewtopic.php?f=77&t=381226
2.http://forum.ubuntu.org.cn/viewtopic.php?f=77&t=327239
3.https://bbs.archlinux.org/viewtopic.php?pid=1321213
4.http://askubuntu.com/questions/369324/grub2-cannot-detect-windows-8
5.http://askubuntu.com/questions/22629/add-windows-7-to-boot-menu
