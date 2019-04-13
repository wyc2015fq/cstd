
# windows7重装后恢复Ubuntu开机启动项，Ubuntu所在的硬盘uuid改变没法进入Ubuntu系统，解决方法 - forever1dreamsxx--NLP - CSDN博客


置顶2012年11月06日 09:26:24[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：1698


**第一种情况：**
当重装windows7后，其他盘符没有发生改变，这种情况最简单，方法如下：
恢复方法：
1.把安装ubuntu->winboot文件夹下wubidr和wubidr.mbr两个文件拷到C盘根目录下
2.以管理员权限打开附件中的“运行”
在命令行中运行c:/windows/system32>bcdedit /create /d "Ubuntu" /application bootsector
成功后（如果不是以管理员权限运行可能会被拒绝）得到一串号码id 如下：
{b8779cc7-c9b2-11de-908b-c8405850ad4a}
3.把{}中的id复制下来，如：b8779cc7-c9b2-11de-908b-c8405850ad4a
4.再输入下列命令  （注意：自己替换下面{}中的id为第2步得到的id）
bcdedit /set {b8779cc7-c9b2-11de-908b-c8405850ad4a} device partition=c:
bcdedit /set {b8779cc7-c9b2-11de-908b-c8405850ad4a} path \wubildr.mbr
bcdedit /set {b8779cc7-c9b2-11de-908b-c8405850ad4a} description "Ubuntu Linux"
bcdedit /displayorder {b8779cc7-c9b2-11de-908b-c8405850ad4a} /addlast
如果一切成功的话，重启之后便可以出现Ubuntu Linux的选项了。
然后就可以进入Ubuntu系统。
**第二种情况：**
假如你的windows7系统在C盘，你的Ubuntu系统文件夹在D盘，你重装了windows7后，Ubuntu所在的D盘也被格式化，你将备份的Ubuntu文件件重新拷回到D盘，然后用第一种方法恢复开机启动项，然后进入系统，会很蛋疼，进不了系统说：no  such device 然后是一串数字字母，就是uuid错了，因为，你格式化了D盘所以其uuid发生了改变。
解方法如下：
1、首先你按照第一种情况添加开机启动项；
2、进入Ubuntu开机那个界面，不要继续，直接按c，这时会出现一个输入命令的终端 窗口；
3、输入  ls ，这时候会显示：(这里面记不到了)  (loop0),(hd0,msdos8),(hd0,msdos7),(hd0,msdos6),(hd0,msdos5),(hd0,msdos1)，(hd0,msdos1)是主分区，表示C盘，5678是逻辑分区。一般情况下主分区是1234（我们一般只有一个），然后逻辑分区从5开始，后面的一次类推（这个我一前unix书看过没懂，现在懂了），所以D盘就是(hd0,msdos5)；
4、因为我的在D盘，所以就输入命令：  ls -l (hd0,msdos5)，查看D盘的uuid，然后拿笔记下来；
5、按  esc，此时又回到了Ubuntu的开机界面，然后按 E ，此时进入编辑模式；
6、然后将你刚才第4步抄下的uuid替换你现在所在编辑界面里面的uuid，还有如果你的Ubuntu系统不是在D盘，还要改对应（hd0,msdos?)，？是你在哪个盘就改几；
7、然后Ctrl  X，恭喜你已经可以进入了系统，不过还没有完，你以后每次进入都要重复步骤456，所以下面继续；
8、进入ubuntu系统后要改系统里面的的uuid文件，在 /boot/grub grub.cfg文件里面，此文件只读，且改变需要管理员权限；
9、所以先改权限 sudo chmod 666 /boot/grub grub.cfg；
10、然后 cd  /boot/grub 然后 vi grub.cfg，把里面的uuid全部换成你刚才抄下的uuid，保存退出；
11、改回grub.cfg的权限 sudo chmod 644 /boot/grub grub.cfg；
然后重启就可以直接进入Ubuntu系统，修改成功。
如果你有什么疑问，可以关注我的sina微博：forever1dream--NLP，我可以具体给你说；
**第三种情况：**
如果你将你配置好的Ubuntu系统拷贝到其他电脑上去使用，uuid发生了改变，应该和第二种情况一样（已测试）。
昨晚搞了好久终于OK了。。。。。

