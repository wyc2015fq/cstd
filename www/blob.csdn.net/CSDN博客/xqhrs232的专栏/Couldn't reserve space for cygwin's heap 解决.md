# Couldn't reserve space for cygwin's heap"解决 - xqhrs232的专栏 - CSDN博客
2017年03月25日 23:14:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3859
原文地址::[http://blog.csdn.net/tony2005/article/details/6213802](http://blog.csdn.net/tony2005/article/details/6213802)
相关文章
1、
## Couldn't reserve space for cygwin's heap, 错误解决
 ----[http://blog.sina.com.cn/s/blog_55870a7f0100kz44.html](http://blog.sina.com.cn/s/blog_55870a7f0100kz44.html)
2、[Couldn't reserve space for cygwin's heap, Win32 error 487](http://blog.csdn.net/hnmsky/article/details/4836016)----[http://blog.csdn.net/hnmsky/article/details/4836016](http://blog.csdn.net/hnmsky/article/details/4836016)
3、[Rebase.exe 的使用](http://blog.csdn.net/chenwanming507507507/article/details/6070882)----[http://blog.csdn.net/chenwanming507507507/article/details/6070882](http://blog.csdn.net/chenwanming507507507/article/details/6070882)
4、[Couldn't reserve space for cygwin's heap, Win32 error 0](http://blog.csdn.net/ba_jie/article/details/43056089)----[http://blog.csdn.net/ba_jie/article/details/43056089](http://blog.csdn.net/ba_jie/article/details/43056089)
5、[Git
 Extensions: Win32 error 487: Couldn't reserve space for cygwin's heap, Win32 error 0](http://stackoverflow.com/questions/18502999/git-extensions-win32-error-487-couldnt-reserve-space-for-cygwins-heap-win32)----[http://stackoverflow.com/questions/18502999/git-extensions-win32-error-487-couldnt-reserve-space-for-cygwins-heap-win32](http://stackoverflow.com/questions/18502999/git-extensions-win32-error-487-couldnt-reserve-space-for-cygwins-heap-win32)
在安装了WinAVR以后，本来工作好好的，可是我为了安装调试程序，就把WinAVR卸载了，谁知道重新安装以后打开PN，怎么样也make不了，输出的出错信息如下：
> "make.exe" clean 
AllocationBase 0x0, BaseAddress 0x71590000, RegionSize 0x160000, State 0x10000 
C:/WinAVR/utils/bin/sh.exe: *** Couldn't reserve space for cygwin's heap, Win32 error 487 
-------- begin -------- 
Cleaning project: 
rm -f main.hex 
AllocationBase 0x0, BaseAddress 0x71590000, RegionSize 0x160000, State 0x10000 
C:/WinAVR/utils/bin/rm.exe: *** Couldn't reserve space for cygwin's heap, Win32 error 487 
make.exe: *** [clean_list] Error 1 
> Process Exit Code: 2
刚开始我以为是版本的问题，因为我第一次安装的时候遇到过这个问题，当时把程序删除掉重新安装了一下就解决了，所以我就按照这个思路多次的卸载与安装，但这次怎么都不成功。
实在没办法了，用Baidu搜了一下，可是结果很让我失望，中文网页里面竟然没有相关的内容，有的也只是简单的几条，心都凉了。还是google牛，把"Couldn't reserve space for cygwin's heap”输进去后网页一大堆，可惜我的英文还没有到火候，虽然基础不错，可是看到这么多的英文地址头都大了，无奈之下，先找最相似的，找了不少，可是都没用，大多数网页分为下面几类：
1、说的最多的是cygwin1.dll的版本问题，并且用词也不肯定，都是什么可能、或许，曾经听说过这样的词汇，可事实上，这一点我早就想到了，也试了n久，根本解决不了问题。
It sounds like you have another copy of the Cygwin DLL somewhere on your system, probably from a different version.
I have also had this issue.  I was able to correct the problem by the 
following steps:
1. Make sure that there is only one copy of cygwin1.dll on your computer
2. Clean up your PATH environment variable.
The order of the paths does matter.  The path closest to the front will 
have priority, especially when you have multiple cygwin1.dll's.
For example:
c:/gnuarm/bin;c:/cygwin/bin;c:/cygwin/usr/X11R6/bin;c:/winavr/bin/;
is different than
c:/winavr/bin/;c:/gnuarm/bin;c:/cygwin/bin;c:/cygwin/usr/X11R6/bin;
2、也有个别说是WinAVR版本的问题，解决方案就是卸载再安装，多试几次就OK了，这简直是碰运气，我也试过，都快把我弄崩溃了。
I did have the same problem beginning of this week. 
Formerly I always worked with WIN98 and it worked ok but now with WinXP.
I am not sure what it was but I think it had something to do with Turtoise CVS because it also uses Cygwin.
This is what I did to make it work again :
Uninstall WINAVR (build 20040404) 
Install WINAVR (build 20040404) >> Still did not work 
Uninstall Turtoise CVS >> WINAVR still did not work 
Uninstall WINAVR (build 20040404) 
Install WINAVR (build 20030424) >> Works (expect Progrs. Notepad but ok)! 
Install Turtoise CVS >> Keeps working 
Uninstall WINAVR (build 20030424) 
Install WINAVR (build 20040404) >> Everything works including PN)
Hope this works for you too
3、还有人说是电脑运行的其他的一些常驻内存的程序引起的，像杀毒软件什么的，关了的话就可以，我测试过，对我没什么用。
i found the problem. It seemed to be a problem with the available memory in the started shell. So i closed all "memory
resident" programs. The critical program was my firewall "Securepoint". After closing Securepoint every thing worked well.
I just found out the following after finally getting it back working.
1) The re-install didn't seem to help any 
2) Disabling Panda antivirus completely allowed it to run properly again 
3) After re-enabling anti-virus features one at a time, isolated the specific one that was causing the problem: "Blocking of
unknown threats / Enable behavioral analysis"
So with all antivirus features enabled but that one it works fine...
4、也有人提出说是Cygwin内存分配的问题，说内存分配的不够大，要修改注册表，我也照着做了，也没解决。可能对有的朋友有用，
原文如下：
By default no Cygwin program can allocate more than 384 MB of memory (program+data). You should not need to change this
default in most circumstances. However, if you need to use more real or virtual memory in your machine you may add an entry
in the either the HKEY_LOCAL_MACHINE (to change the limit for all users) or HKEY_CURRENT_USER (for just the current user)
section of the registry.
Add the DWORD value heap_chunk_in_mb and set it to the desired memory limit in decimal MB. It is preferred to do this in
Cygwin using the regtool program included in the Cygwin package. (For more information about regtool or the other Cygwin
utilities, see the section called “Cygwin Utilities” or use each the --help option of each util.) You should always be
careful when using regtool since damaging your system registry can result in an unusable system. This example sets memory
limit to 1024 MB:
regtool -i set /HKLM/Software/Cygnus/ Solutions/Cygwin/heap_chunk_in_mb 1024
regtool -v list /HKLM/Software/Cygnus/ Solutions/Cygwin
Exit all running Cygwin processes and restart them. Memory can be allocated up to the size of the system swap space minus any
the size of any running processes. The system swap should be at least as large as the physically installed RAM and can be
modified under the System category of the Control Panel.
5、也有人说跟环境变量、虚拟内存的大小有关，我试了也没用。
6、我最后的解决方式：rebase -b 0x76000000 c:/winavr/utils/bin/msys-1.0.dll，然后问题就解决了，主要参考的网站：
[http://www.avrfreaks.net/index.php?module=PNphpBB2&file=viewtopic&t=37848&highlight](http://www.avrfreaks.net/index.php?module=PNphpBB2&file=viewtopic&t=37848&highlight)=
My name is hyungon.kim (Korea) 
I got the same error message. 
-Couldn't reserve space for cygwin's heap, Win32 error 
487
I trying to several times to solve this problem. 
I searching and searching Internet many times. 
but I couldn't find a solution.
In 5~6 hours, finally I found it a solution.
solution--------------------------------------
first, You must have rebase.exe 
If you search Internet, you find easily, and downloading.
second, rebase -b 0x76000000 /winavr/utils/bin/msys-1.0.dll
0x76000000(examlple) -> you can change this address value 
properly, maybe Winavr is compiled well.

