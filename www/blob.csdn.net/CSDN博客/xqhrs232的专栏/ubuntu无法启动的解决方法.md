# ubuntu无法启动的解决方法 - xqhrs232的专栏 - CSDN博客
2016年03月24日 00:00:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：9905
原文地址：[http://blog.csdn.net/mayqlzu/article/details/5878117](http://blog.csdn.net/mayqlzu/article/details/5878117)
相关文章
1、grub2 无法修复 Ubuntu启动不了 完美解决----[http://wenku.baidu.com/link?url=4cD5DxjWxK9aXk3Rglgb22AfZWIi5T1Q3_k5RaHM-X2kvL1S5zW6R_VUjo_uTpU3afNAl3dLzkGXjM_pERZsgyV56AbmF4sBgTjGK2iSh-e](http://wenku.baidu.com/link?url=4cD5DxjWxK9aXk3Rglgb22AfZWIi5T1Q3_k5RaHM-X2kvL1S5zW6R_VUjo_uTpU3afNAl3dLzkGXjM_pERZsgyV56AbmF4sBgTjGK2iSh-e)
2、ubuntu 14.04 更新后无法启动----[http://bbs.csdn.net/topics/390876759](http://bbs.csdn.net/topics/390876759)
3、引导异常导致无法正常开机----[http://tieba.baidu.com/p/2910461207](http://tieba.baidu.com/p/2910461207)
4、ubuntu12.04 无法启动----[http://zhidao.baidu.com/link?url=ZfZgRPMxkxVzsvXymIXDAw7hpQd7uVNBzcweTUegoWv9qWUcG22fd1wpt8Ng64SwwvFxQhWm492BjmQ8VamhW_](http://zhidao.baidu.com/link?url=ZfZgRPMxkxVzsvXymIXDAw7hpQd7uVNBzcweTUegoWv9qWUcG22fd1wpt8Ng64SwwvFxQhWm492BjmQ8VamhW_)
我经常遇到这样的事情：关闭ubuntu系统的时候死机了，只能按电源键了，再次启动的时候就起不来了，起不来的现象还分两种，解决办法也不一样，如下：
情况1
现象：具体记不清了，反正是开始检查磁盘，然后报错：unrecoverable error什么的，然后就停在那儿了好像。
解决办法：
For those of you with Wubi problems, here's how to mount your
 root.disk, and recover your files using a LiveCD:
- 
Boot your computer, enter your BIOS by pressing the key it tells you to press at the first splash screen you come to at startup, put the CD/DVD drive first in the boot sequence, insert your LiveCD, then save the changes, and exit your BIOS.
- 
Your computer should now boot from the CD.
- 
In the Ubuntu setup program, select the preferred language you use, then select the "Try Ubuntu with No Change To My Computer" option.
- 
Once at the Ubuntu desktop, in a Live session, open up Applications->Accessories>Terminal.
- 
Now run:
![](http://neosmart.net/forums/images/chestnut/misc/code.gif)
sudo fdisk -l
sudo mkdir /win
sudo mount /dev/sdxy /win
sudo mkdir /vdisk
sudo mount -o loop /win/ubuntu/disks/root.disk /vdisk
	
where the "x" and the "y" in "sdxy" is replaced with the correct HDD letter (starting at  **a**   of course), and the correct partition number (starting at 1), of the Windows partition your Wubi install is on. The first command will have given
 you the location.
Once running those commands, open up Places>Computer>Filesystem>vdisk, and you should find the contents of your root.disk in there. Now you can backup your data to external media, and reinstall Ubuntu with Wubi if you like.
You can also try running the 
![](http://neosmart.net/forums/images/chestnut/misc/code.gif)
sudo fsck /win/ubuntu/disks/root.disk
	
command to fix any filesystem errors that may be interfering with the boot, if that's the problem.
Hope it helps.

===========================================================================
情况2
现象：先闪过一个什么提示界面，太快了，看不清楚，然后就是进入grub> 提示符，不动了。
解决办法：
### [WUBI - Ubuntu won't boot; missing root.disk](http://creekcodes.blogspot.com/2009/01/wubi-ubuntu-wont-boot-missing-rootdisk.html)
Did this ever happen to you? It sucks right? You simply wished 'you should have installed natively (the old way)'.
So your Ubuntu won't boot because root.disk is missing, then you check out**`c:/ubuntu/disks`**, it's not there. (And probably,  **`disks`**  directory is not even accessible or is missing too.)  `o_O`
So what happened? Your  [NTFS filesytem](http://en.wikipedia.org/wiki/NTFS)   got corrupted because on many reasons -- primarily, you might had a power interruption.
Is it lost then? I'm not sure but let's hope it didn't. You can try to recover**`root.disk`**  by following this steps:
1. Run  [command prompt](http://en.wikipedia.org/wiki/Windows_command_line)   as Administrator -- needless to say, '**`Enter your password`**'.
2. Run  [Check Disk](http://en.wikipedia.org/wiki/Chkdsk)   by typing:  **`chkdsk c: /f`**  (change  **`C:`**  to your local disk
 drive)（我的注释：我的ubuntu是装在e:/ubuntu的，所以，我需要检查e盘，我用的命令是 chkdsk e: /r，我用的是/r参数，不是/f参数，/r也是从网上看来的，试过的可以的，/f没试过，估计也可以吧）
**NOTE:**   There's a big change that a message will show.
（我的注释：系统可能提示说要不要卸载这个卷，选否，它又问要不要下次启动的时候检查，选择是。重启，进入检查阶段后，可能遇到百分之多少的时候好像死掉了，其实没有，要有耐心多等会儿）*`Chkdsk cannot run because the volume is in use by another process. Would you like to schedule this volume to be checked the next time the system restarts? (Y/N)`*
Just type  **`Y`**  and restart you PC immediately.
3. After it finished checking (and possibly,  *repairing* ) your disk, make sure to [show
 your hidden files](http://www.microsoft.com/windowsxp/using/helpandsupport/learnmore/tips/hiddenfiles.mspx) .
4. Look for your (onced missing)  **`root.disk`**  under  **`c:/found.000`**  directory and put it back to  **`c:/ubuntu/disks/`**.
**NOTE:**   Your  **`swap.disk`**  could also be inside  **`found.000`**.
（我的注释：当你想打开found.000的时候，系统可能提示说不能查看，你可以以管理员身份运行cmd.exe，然后用cd命令进入found.000，用move命令将里面的boot, disk, swap三个东西剪切到e:/ubuntu/disks/下，我是装载e盘的，你的可能在其他盘，如果没有disks目录就先手动建一个）
Now, try to reboot your Windows and choose Ubuntu then hope that the recovered file is intact and not corrupted.  `;)`
================================================================================
情况3
现象：我修改了/etc下的某个文件，重启的时候，在过了选择window还是ubuntu的界面之后就黑屏了，硬盘灯闪了一会儿也灭了，按什么都没反应，短按（太短不行，太长就直接关机了，要适中）电源键会短暂出现ubuntu登录界面，就是一行ubuntu的字，下面几个点的那个界面，然后关机。
原因：就是因为改了/etc的某个文件啊，/etc这个目录是比较重要的，不能随便改，即使要改也要做好备份，便于恢复，要是忘了当时怎么改的，那就惨了。
解决办法：
参照情况1，从cd启动ubuntu，把“那个ubuntu的”root.disk挂在到现在的这个ubuntu系统下，例如挂载到/vdisk，然后就能看到“那个ubuntu”的系统盘了，把当时的改动还原过来，重启就好了。
参考材料：
1 [http://neosmart.net/forums/showthread.php?t=5004](http://neosmart.net/forums/showthread.php?t=5004)
2 [http://creekcodes.blogspot.com/2009/01/wubi-ubuntu-wont-boot-missing-rootdisk.html](http://creekcodes.blogspot.com/2009/01/wubi-ubuntu-wont-boot-missing-rootdisk.html)
# ubuntu 14.04 更新后无法启动
