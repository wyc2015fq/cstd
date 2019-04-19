# 非正常关闭vi编辑器时会生成一个.swp文件 - xqhrs232的专栏 - CSDN博客
2016年09月23日 09:40:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：450
原文地址::[http://www.cnblogs.com/DMDD/p/5052048.html](http://www.cnblogs.com/DMDD/p/5052048.html)
相关文章
1、[关于vi不正常退出产生的swp文件](http://blog.csdn.net/zwhfyy/article/details/37648879)----[http://blog.csdn.net/zwhfyy/article/details/37648879](http://blog.csdn.net/zwhfyy/article/details/37648879)
非正常关闭vi编辑器时会生成一个.swp文件
关于swp文件
使用vi，经常可以看到swp这个文件,那这个文件是怎么产生的呢，当你打开一个文件，vi就会生成这么一个.(filename)swp文件以备不测（不测下面讨论），如果你正常退出，那么这个这个swp文件将会自动删除。下面说不测。
不测分为：1当你用多个程序编辑同一个文件时。
         2非常规退出时。
第一种情况的话，为了避免同一个文件产生两个不同的版本（vim中的原话），还是建议选择readonly为好。
第二种情况的话，你可以用vim -r filename恢复，然后再把swp文件删除（这个时候要确保你的swp文件没有用处了，要不然你会伤心的）
swp文件的来历，当你强行关闭vi时，比如电源突然断掉或者你使用了Ctrl+ZZ，vi自动生成一个.swp文件，下次你再编辑时，就会出现一些提示。
你可以使用
> 
vi -r {your file name}
来恢复文件，然后用下面的命令删除swp文件，不然每一次编辑时总是有这个提示。
> 
rm .{your file name}.swp
在网上搜到了一个类似的提示，不同的linux提示可能不一样
> 
“.xorg.conf.swp” already exists!
[O]pen Read-Only, (E)dit anyway, (R)ecover, (Q)uit:
当然可以用R键恢复。
vi编辑器要正常退出可以使用Shift-ZZ 。
[SuSE] 【已解决】vi 命令 生成 .swp 文件相关问题 
-> 例：使用vi a.dat时 会生成一个 .a.dat.swp 文件
       然后再开个终端 vi a.dat 时会提示：
      【“Swap file ".文件名.swp" already exists![O]pen Read-Only, (E)dit anyway, (R)ecover, (Q)uit:】  
-> vi 版本：
   vi -vierson
     VIM - Vi IMproved 6.3 (2004 June 7, compiled Oct 20 2004 23:12:36)
->  问： (vi 一个文件时怎么样可以不让它产生.swp文件？
          或不让他提示【“Swap file ".文件名.swp" already exists![O]pen Read-Only, (E)dit anyway, (R)ecover, (Q)uit:】)
处理方法： 打开 /etc/vimrc 并添加 set noswapfile 后 OK
