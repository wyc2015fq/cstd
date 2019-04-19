# Ubuntu下file not found + GRUB rescue肿么办？--由windows下分区调整引起的 - 三少GG - CSDN博客
2011年11月03日 10:02:52[三少GG](https://me.csdn.net/scut1135)阅读数：7870
grub 引导文件被你格式化了。就是grub坏了。糊里糊涂做了某个操作，导致ubuntu无法启动。
GRUB显示error: file not found，然后就停到了 GRUB rescue的提示符下。解决办法：1. [http://wenku.baidu.com/view/d797d43183c4bb4cf7ecd13e.html](http://wenku.baidu.com/view/d797d43183c4bb4cf7ecd13e.html)用Ubuntu的安装CD，也就是Live CD从光驱启动，打开一个终端窗口，在里面输入如下命令：sudo -i  （获得超级用户权限）mount /dev/sda7 /mnt  （也就是你的Ubuntu的“/”的挂接硬盘分区，比如sda7，根据你的具体安装情况确定，/mnt是你建立的一个用来挂接的目录）如果你单独划分了Ubuntu的boot分区，那么还需要做如下操作：mount /dev/sda6 /mnt/boot （假设你的boot分区是在sda6）挂载你其他的分区，如果有的话重建grub到sda的mbrgrub-install --root-directory=/mnt /dev/sda  （一定注意/mnt后面有个空格）然后，重启，就应该可以了
2. **[grub rescue救援模式的处理](http://apps.hi.baidu.com/share/detail/31688335)**
我的linux在调整分区后，出现了grub rescue>
这表示grub2的配置文件坏了，由于分区调整或分区UUID改变造成GRUB2不能正常启动，从而进入修复模式（grub rescue)也称救援模式。
在救援模式下只有很少的命令可以用：
set  ,  ls , insmod , root , prefix
(1)set  查看环境变量，这里可以查看启动路径和分区。
(2)ls   查看设备
(3)insmod  加载模块
(4)root  指定用于启动系统的分区,在救援模式下设置grub启动分区
(5)prefix 设定grub启动路径
一、分区乱了，我不知道boot目录在什么地方了
ls  #查看一下设备状态
如图：
![grub rescue救援模式的处理 - 汉斯的遗忘 - 汉斯的遗忘](http://hiphotos.baidu.com/jdhongyang/pic/item/276471066583a1807a894773.jpg)
grub rescue> ls (hd0,msdos3)
error: bad filename.
提示：错误的文件名，我在测试时发现必须是后面加一个/
grub rescue> ls (hd0,msdos3)/
./  ../  lost+found/
通过查看发现在(hd0,msdos1)/下有一个boot目录
二、设置grub的启动分区和路径
set root=(hd0,msdos1)  #设置grub启动分区
set prefix=(hd0,msdos1)/boot/grub/  #设置grub启动路径
查看一下设置情况：
grub rescue> set
prefix=(hd0,msdos1)/boot/grub
root=hd0,msdos1
三、加载基本模块
insmod /boot/grub/normal.mod  #加载基本模块
四、进入正常模式
normal  #进入正常模式，出现菜单，如果加载grub.cfg（错误的）可能出现问题，按shift可以出现菜单，之后按c键进入控制台
进入正常模式后就会出现grub>这样的提示符，在这里支持的命令就非常多了。
五、引导系统
set root=(hd0,msdos1)  #设置正常启动分区
linux /boot/vmlinuz ....  ro text root=/dev/sda1  #加载内核，进入控制台模式
initrd  /boot/initrd ....  #加载initrd.img
boot #引导
六、更新grub
安装：grub-install /dev/sda
更新：update-grub
3.  **[恢复grub时挂载错误导致开机grub rescue且仍然解决不能](http://forum.ubuntu.org.cn.sixxs.org/viewtopic.php?t=328748)** 按网上的资料继续……          1. ls 显示hd0 hd0,8 hd0,7 hd0,6 hd0,5 hd0,1
          2. ls (hd0,x)都显示unknow filesystem；ls (hd0,1)显示bad filename。改为ls (hd0,1)/后出现几行字符（里面包括boot目录，但无home目录），当时我认为这个就是hd0，1分区了
另：ls (hd0,1)/boot/出现内容，但ls (hd0,1)/boot/grub显示file not found（boot目录里并无grub子目录……）参考：[http://hi.baidu.com/szmneo/blog/item/80737b256314de398744f9f5.html](http://hi.baidu.com/szmneo/blog/item/80737b256314de398744f9f5.html)
Grub2 rescure
2009-11-27 13:26
**先说问题：**
今天手痒，想安装Kubuntu看看，而且想安装到U盘，结果安装完后U盘启动到一个闪烁到光标就卡了，拔掉U盘后就显示出来了Grub
 Rescure……唉
**解决办法：**
rescue 模式模式下的命令貌似没有几个，这句是废话。
首先：ls
查看硬盘分区情况，如果你知道grub安装在哪儿了（比如我的在（hd1，3）/grub/，这个要记住，后面/grub的地方都要改成你对应的），
那么就直接进行下一步。
如果不知道，那就ls每个分区，如:
 ls (hd1,3)/ ,   如果正确就会显示里面到内容，直到找到grub
第二：set
直接输入set可以查看root和prefix的配置，这步需要将它给成你找到到地方
例如我的如下：
**代码:**
 grub rescue>set root=(hd1,3)
 grub rescue>set prefix=(hd1,3)/grub //上面是设置的，
然后set查看下
 grub rescue>set
 grub rescue>root=hd1,3 
 grub
 rescue>prefix=(hd1,3)/grub
**第三：insmod** 这时继续如下输入就可以，会进入grub的菜单。 grub rescue>insmod /grub/normal.mod 
**第四：normal**
 直接输入normal进入普通模式 rescue：grub>normal 看到你期盼的系统菜单了吧，终于可以进入系统了，哈哈哈 
**第五：update** 进入系统后，首先 sudo update-grub
將選單修正，再
sudo
 grub-install /dev/sda
sda是你的启动磁盘～
OK，搞定一切了～＠！
参考1：
Grub2 中的运行模式，有了些许变化：
 Normal 也就是常规模式，包括了菜单界面与控制台，即是 Grub 传统版的默认模式。还有一个是 Rescue 模式，这个是受限模式，比如 Grub 没有能找到 root 设备，就会进入这种模式。
通常，我们可以在 normal 模式下输入 'rescue' 即可进入 rescue 模式；在 rescue 模式下输入 'normal' 即可进入 normal 模式。可是，通常后一种情况，需要我们做些一个相关设置才可以进入菜单：
set
 root=(hd0,1)                     - 指定根分区为第一个磁盘，里面包含了 grub.cfg
set prefex=/boot/grub             - 指定路径前缀，以构成菜单文件字符串 ' (hd0,1)/boot/grub/grub.cfg' ( 通常也不需要修改 )
然后输入 'normal' 就可以换回常规的菜单模式了。
参考2：http://danielliang.blogspot.com.sixxs.org/2009/10/grub2-rescue.html
昨天裝Ubuntu
 9.10時預留一個空間要作交換資料用,之後先回到windows把它切成主分割區並format成FAT32,結果重開機後Grub就說unknown filesystem接著顯示
> grub rescue> 
結果打help也沒用..本來就對grub沒啥研究，而且之後上網發現這次是grub2和舊版grub的用法又不太同-.-
最後總算找到 [https://help.ubuntu.com/community/Grub2](https://help.ubuntu.com/community/Grub2)
解決方法大概如下:
> grub
 rescue>set prefix=(hd0,3)/boot/grub root=hd0,3 
我安裝時就知道 / 是 /dev/sda3，對照(hd0,3)是沒問題的。
而現在應該是把後面空出來的也切成一塊，而使號碼往前移
> grub rescue>ls (hd0), (hd0,5), (hd0,4),
 (hd0,3), (hd0,2), ... 
看來很有可能，總之先試試
> grub rescue>set root=(hd0,2) grub rescue>set prefix=(hd0,2)/boot/grub grub rescue>insmod normal ........ rescue:grub>normal 
就回到選單啦！當然選單中的硬碟號碼一定也不對，就記得先修正再boot
進入系統後，就先
> sudo
 update-grub 
將選單修正，再
> sudo grub-install /dev/sda 
大功告成！如果沒做 grub-install,就有每次開機都練習 grub2 rescue mode的機會:P
...當然我沒有一次就那麼順利，這是把失敗的地方全部刪掉才有那麼簡捷的文件XD
而且網路上完全沒有這種範例我才打的..
+++++++++++++++++++++++++++本人实际解决方案：参考上文  2011.11.3主要依据内容：[http://wenku.baidu.com/view/c71a1812cc7931b765ce1563.html](http://wenku.baidu.com/view/c71a1812cc7931b765ce1563.html)<!-- @page { margin: 2cm } P { margin-bottom: 0.21cm } --> 
本人安装的是win7+ubuntu10.10,硬盘直接安装。因为win7下装了几个大软件，所以决定一键Ghost备份一下，结果重启之后，就进入了grub rescue界面:
error:unknown filesystem
grub rescue>
研究了半天，才解决这个问题。先说说是怎么回事。
安装ubuntu时，启动是用grub2进行启动。我的win7在C盘， Ubuntu空间是从D盘里分出来的。我们知道，每次系统启动时都是先进入grub，也就是先在ubuntu的启动目录里选择进入哪个系统，如果按分区来讲，grub2在(hd0,msdos7)也就是我的ubuntu所在的分区。那么启动时root应该设在(hd0,msdos7),可一键ghost会修改启动，它把root改在(hd0,msdos9),这是个windows分区，也就是NTFS分区，ubuntu是不能识别这种文件系统的，所以就有了error:unknown filesystem，**这种情况下自然不能启动，那么grub2就会启动grub rescue模式，就是修复模式。那么我们要做的就是把grub重新指向(hd0,msdos7)。**
下面是具体步骤：
①查看分区
因为每个人的分区不一样，所以我们要查看分区,用ls指令
grub rescue>ls
回车后，就会出现
(hd0) (hd0,msdos9) (hd0,msdos8) (hd0,msdos7) (hd0,msdos6) (hd0,msdos5) (hd0,msdos1)
grub rescue>
**注：我用的是grub2,对于grub用户，分区前没有msdos字样**
上面是我的分区，每个人的不一样。
grub rescue>set回车
prefix=(hd0,msdos9)/boot/grub
root=hd0,msdos9
从上面可以看出来现在我的系统是从(hd0,msdos9)里启动的。
那么怎么知道ubuntu在哪个分区呢？进入第二步
②寻找ubuntu所在分区
这一步我们要一个一个的试，
还是用ls指令
先试下在不在（hd0,msdos8）里边
grub rescue>**ls (hd0,msdos8)**/
回车会发现，不是，还是unknown filesystem
接着来
。。。。。。。。。
当我试到
(hd0,msdos7)的时候，可以看到一下子多了很多字，这些就是我ubuntu主文件夹下的文件夹和文件的名字。OK，找到分区了！
③修改启动分区
grub rescue>root=(hd0,msdos7)
grub rescue>prefix=/boot/grub //grub路径设置
grub rescue>set root=(hd0,msdos7)
grub rescue>set prefix=(hd0,msdos7)/boot/grubgrub rescue>insmod normal //启动normal启动
grub rescue>normal
依次敲入上面的指令，是不是看到熟悉的启动菜单了？选win7后，一键Ghost就开始备份系统了。别高兴太早，一切还都没开始呢。如果你不是因为一键Ghost问题进入grub rescue,可以直接跳到第⑤步
④再来一遍
当你等着一键Ghost备份完系统，重启后，你发现还是
error:unknown filesystem
grub rescue>
别着急，再把前三步进行一遍就可以了
⑤进入命令行启动Ubuntu
进入系统启动选项界面后，你发现，无论点那个还是进不去，这是因为你还没有真正的修改grub,这个要到ubuntu里修改
进入系统启动项界面后，按C进入命令行模式
grub >set root=hd0,msdos7
grub >set prefix=(hd0,msdos7)/boot/grub
grub >linux /vmlinuz-xxx-xxx root=/dev/sda7 //里边的xxxx可以按Tab键，如果有acpi问题,在最后加一句acpi=off
grub >initrd /initrd.img-xxx-xxx
grub >boot
Ubuntu正常启动了吧？
**⑥进入ubuntu修复grub**
大功就要高成了
进入Ubuntu后,修复grub
在终端里运行
sudo update-grub
重建grub到第一硬盘mbr
sudo grub-install /dev/sda
好啦，重启，一切搞定！
其余不错的两篇:1.  [http://zhidao.baidu.com/question/209639907.html](http://zhidao.baidu.com/question/209639907.html)2.  [http://os.51cto.com/art/201006/205349.htm](http://os.51cto.com/art/201006/205349.htm)+++++++++++++++++评论：[http://tieba.baidu.com/f?kz=1165692820](http://tieba.baidu.com/f?kz=1165692820)就当买个教训吧，以后别再用Windows下的分区工具了。
我现在已经抛弃MBR，改用GPT分区了，先分了10个主分区，一了百了。
n年前就吃过硬盘分区大师之类工具的亏，现在硬盘分区很少改动，一般分区都用Linux下的fdisk命令就可以了，很好使，lz可以去搜一下用法。++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
简要版本解决办法：
1. ls查看已有分区
2. ls (hd0,6)可以看到里面有GRUB目录
3. set命令查看当前的变量，发现都是hd0,7
4. set root=(hd0,6)
5. set prefix=(hd0,6)/grub
6. insmod normal
7. normal
每次开机都这么折腾一遍太麻烦，怎么办呢？
解决办法：
1. sudo update-grub
2. sudo grub-install /dev/sda
