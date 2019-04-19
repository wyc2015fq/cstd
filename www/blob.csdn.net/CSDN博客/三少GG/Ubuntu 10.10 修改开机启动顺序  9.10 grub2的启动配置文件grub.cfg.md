# Ubuntu 10.10 修改开机启动顺序 | 9.10 grub2的启动配置文件grub.cfg - 三少GG - CSDN博客
2010年12月17日 13:48:00[三少GG](https://me.csdn.net/scut1135)阅读数：3285标签：[ubuntu																[linux																[search																[debian																[windows																[microsoft](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=debian&t=blog)](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
【2012.2.21】
刚安装了双系统，分别是windows7(先)和Ubuntu10.04(后)(非wubi安装), 默认是Linux启动,通过下面的操作可把windows7改为默认启动系统
　　启动Ubuntu后，在终端中输入：
　　先备份 sudo cp /boot/grub/grub.cfg /boot/grub/grub.cfg.backup
　　修改启动列表：**sudo gedit /boot/grub/grub.cfg **(注意不是sudo gedit /boot/grub/menu.lst,貌似9.10就改为了grub.cfg)
　　然后找到
　　default 0
　　然后往下看，**找你想默认启动的操作系统!修改default 后面的数字**，0对应第一个，1对应第二个，依次往下!以下是我的grub.cfg
　　默认是Ubuntu!把0改为4就可以了!
**　　(升级内核之后数字酌情修改)**
上面的自己修改default后面的数字。
　　如果你觉得这样不方便到话，我建议你把win7的那一项剪切到Ubuntu前面 ，这样既美观又方便(不用修改default后面的数字)。  
类似如下：
......
　### END /etc/grub.d/05_debian_theme ###
　　### BEGIN /etc/grub.d/30_os-prober ###
　　menuentry "Windows 7 (loader) (on /dev/sda1)" {
　　insmod ntfs
　　set root='(hd0,1)'
　　search --no-floppy --fs-uuid --set 6e40b6ff40b6ccd9
　　chainloader +1
　　}
　　### END /etc/grub.d/30_os-prober ###
　　### BEGIN /etc/grub.d/10_linux ###
　　menuentry 'Ubuntu，Linux 2.6.32-21-generic' --class ubuntu --class gnu-linux --class gnu --class os  ......
每次更新内核，你都要重新修改这个启动项.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://www.linuxidc.com/Linux/2009-12/23181.htm](http://www.linuxidc.com/Linux/2009-12/23181.htm)    important!!!
升级到Ubuntu 9.10后，就要接触grub2了，它和以前的版本有了很多的不同。这里是一个基础教程。
### 一、grub2的启动配置文件grub.cfg
grub2的启动配置文件grub.cfg是/boot/grub/grub.cfg，而不是以前的memu.lst。
如果你是多系统，有ubuntu和windows，那么用下面的命令，可以使grub2得到所以可以启动的系统。
sudo update-grub
实际就是让系统自己生成合适的grub.cfg文件。
这个grub.cfg文件是只读属性，so如果你要修改它，需要权限。
sudo chmod +w /boot/grub/grub.cfg
然后可以编辑grub.cfg了。
sudo gedit /boot/grub/grub.cfg
### 二、默认系统和等待时间
#### 1、set default=0
这是说从第一项启动。每个启动项都是以menuentry开始，menuentry后面“xxx”是启动项名称，然后｛xxx｝是启动代码。
### BEGIN /etc/grub.d/10_linux ###
menuentry “Ubuntu, Linux 2.6.31-14-generic” {
recordfail=1
if [ -n ${have_grubenv} ]; then save_env recordfail; fi
set quiet=1
insmod ext2
set root=(hd0,6)
search –no-floppy –fs-uuid –set 040508ff-fec7-4c66-ba64-a09f8abe8059
linux/boot/vmlinuz-2.6.31-14-generic root=UUID=040508ff-fec7-4c66-ba64-a09f8abe8059 ro   quiet splash
initrd/boot/initrd.img-2.6.31-14-generic
}
menuentry “Ubuntu, Linux 2.6.31-14-generic (recovery mode)” {
recordfail=1
if [ -n ${have_grubenv} ]; then save_env recordfail; fi
insmod ext2
set root=(hd0,6)
search –no-floppy –fs-uuid –set 040508ff-fec7-4c66-ba64-a09f8abe8059
linux/boot/vmlinuz-2.6.31-14-generic root=UUID=040508ff-fec7-4c66-ba64-a09f8abe8059 ro single
initrd/boot/initrd.img-2.6.31-14-generic
}
### END /etc/grub.d/10_linux ###
### BEGIN /etc/grub.d/20_memtest86+ ###
menuentry “Memory test (memtest86+)” {
linux16/boot/memtest86+.bin
}
menuentry “Memory test (memtest86+, serial console 115200)” {
linux16/boot/memtest86+.bin console=ttyS0,115200n8
}
### END /etc/grub.d/20_memtest86+ ###
### BEGIN /etc/grub.d/30_os-prober ###
menuentry “Microsoft Windows XP Home Edition (on /dev/sda1)” {
insmod ntfs
set root=(hd0,1)
search –no-floppy –fs-uuid –set 5c108a1c1089fd70
drivemap -s (hd0) ${root}
chainloader +1
}
if keystatus; then
if keystatus –shift; then
set timeout=-1
else
set timeout=10
fi
else
if sleep$verbose –interruptible 3 ; then
set timeout=10
fi
fi
### END /etc/grub.d/30_os-prober ###
> 
### BEGIN /etc/grub.d/10_linux ###
menuentry “Ubuntu, Linux 2.6.31-14-generic” {
recordfail=1
if [ -n ${have_grubenv} ]; then save_env recordfail; fi
set quiet=1
insmod ext2
set root=(hd0,6)
search –no-floppy –fs-uuid –set 040508ff-fec7-4c66-ba64-a09f8abe8059
linux/boot/vmlinuz-2.6.31-14-generic root=UUID=040508ff-fec7-4c66-ba64-a09f8abe8059 ro   quiet splash
initrd/boot/initrd.img-2.6.31-14-generic
}
menuentry “Ubuntu, Linux 2.6.31-14-generic (recovery mode)” {
recordfail=1
if [ -n ${have_grubenv} ]; then save_env recordfail; fi
insmod ext2
set root=(hd0,6)
search –no-floppy –fs-uuid –set 040508ff-fec7-4c66-ba64-a09f8abe8059
linux/boot/vmlinuz-2.6.31-14-generic root=UUID=040508ff-fec7-4c66-ba64-a09f8abe8059 ro single
initrd/boot/initrd.img-2.6.31-14-generic
}
### END /etc/grub.d/10_linux ###
### BEGIN /etc/grub.d/20_memtest86+ ###
menuentry “Memory test (memtest86+)” {
linux16/boot/memtest86+.bin
}
menuentry “Memory test (memtest86+, serial console 115200)” {
linux16/boot/memtest86+.bin console=ttyS0,115200n8
}
### END /etc/grub.d/20_memtest86+ ###
### BEGIN /etc/grub.d/30_os-prober ###
menuentry “Microsoft Windows XP Home Edition (on /dev/sda1)” {
insmod ntfs
set root=(hd0,1)
search –no-floppy –fs-uuid –set 5c108a1c1089fd70
drivemap -s (hd0) ${root}
chainloader +1
}
if keystatus; then
if keystatus –shift; then
set timeout=-1
else
set timeout=10
fi
else
if sleep$verbose –interruptible 3 ; then
set timeout=10
fi
fi
### END /etc/grub.d/30_os-prober ###
我的第五个启动项是menuentry “Microsoft Windows XP Home Edition (on /dev/sda1)”，我要它默认是以xp启动，所以修改为set
 default=”4″。(hd0,1)表示第一块硬盘的第一个分区，我的Linux在(hd0,6)。
当然，你也可以把你需要默认启动的系统放到最前面，那么就还是set default=0
#### 2、set timeout=10
if [ ${recordfail} = 1 ]; then
set timeout=-1
else
set timeout=10
fi
这里停留在grub选择系统界面的时间就是10秒，即set timeout=10。
有时候我并不想等待这么久，我直接启动xp，需要ubuntu时才按几下Shift键进入grub选择系统界面。可以添加如下代码：
if keystatus; then
if keystatus –shift; then
set timeout=-1
else
set timeout=10
fi
else
if sleep$verbose –interruptible 3 ; then
set timeout=10
fi
fi
### 三、其他详细配置说明
[请点击此处](http://forum.ubuntu.org.cn/viewtopic.php?f=139&t=225669)
### 四、我的grub.cfg文件
安装了xp和ubuntu，默认启动xp，在电脑启动grub时，按Shift键进入系统选择界面。
> 
#
# DO NOT EDIT THIS FILE
#
# It is automatically generated by /usr/sbin/grub-mkconfig using templates
# from /etc/grub.d and settings from /etc/default/grub
#
### BEGIN /etc/grub.d/00_header ###
if [ -s /boot/grub/grubenv ]; then
have_grubenv=true
load_env
fi
set default=”4″
if [ ${prev_saved_entry} ]; then
saved_entry=${prev_saved_entry}
save_env saved_entry
prev_saved_entry=
save_env prev_saved_entry
fi
insmod ext2
set root=(hd0,6)
search –no-floppy –fs-uuid –set 040508ff-fec7-4c66-ba64-a09f8abe8059
if loadfont /usr/share/grub/unicode.pf2 ; then
set gfxmode=640×480
insmod gfxterm
insmod vbe
if terminal_output gfxterm ; then true ; else
# For backward compatibility with versions of terminal.mod that don’t
# understand terminal_output
terminal gfxterm
fi
fi
if [ ${recordfail} = 1 ]; then
set timeout=-1
else
set timeout=10
fi
### END /etc/grub.d/00_header ###
### BEGIN /etc/grub.d/05_debian_theme ###
set menu_color_normal=white/black
set menu_color_highlight=black/white
### END /etc/grub.d/05_debian_theme ###
### BEGIN /etc/grub.d/10_linux ###
menuentry “Ubuntu, Linux 2.6.31-14-generic” {
recordfail=1
if [ -n ${have_grubenv} ]; then save_env recordfail; fi
set quiet=1
insmod ext2
set root=(hd0,6)
search –no-floppy –fs-uuid –set 040508ff-fec7-4c66-ba64-a09f8abe8059
linux/boot/vmlinuz-2.6.31-14-generic root=UUID=040508ff-fec7-4c66-ba64-a09f8abe8059 ro   quiet splash
initrd/boot/initrd.img-2.6.31-14-generic
}
menuentry “Ubuntu, Linux 2.6.31-14-generic (recovery mode)” {
recordfail=1
if [ -n ${have_grubenv} ]; then save_env recordfail; fi
insmod ext2
set root=(hd0,6)
search –no-floppy –fs-uuid –set 040508ff-fec7-4c66-ba64-a09f8abe8059
linux/boot/vmlinuz-2.6.31-14-generic root=UUID=040508ff-fec7-4c66-ba64-a09f8abe8059 ro single
initrd/boot/initrd.img-2.6.31-14-generic
}
### END /etc/grub.d/10_linux ###
### BEGIN /etc/grub.d/20_memtest86+ ###
menuentry “Memory test (memtest86+)” {
linux16/boot/memtest86+.bin
}
menuentry “Memory test (memtest86+, serial console 115200)” {
linux16/boot/memtest86+.bin console=ttyS0,115200n8
}
### END /etc/grub.d/20_memtest86+ ###
### BEGIN /etc/grub.d/30_os-prober ###
menuentry “Microsoft Windows XP Home Edition (on /dev/sda1)” {
insmod ntfs
set root=(hd0,1)
search –no-floppy –fs-uuid –set 5c108a1c1089fd70
drivemap -s (hd0) ${root}
chainloader +1
}
if keystatus; then
if keystatus –shift; then
set timeout=-1
else
set timeout=10
fi
else
if sleep$verbose –interruptible 3 ; then
set timeout=10
fi
fi
### END /etc/grub.d/30_os-prober ###
### BEGIN /etc/grub.d/40_custom ###
# This file provides an easy way to add custom menu entries.  Simply type the
# menu entries you want to add after this comment.  Be careful not to change
# the ‘exec tail’ line above.
### END /etc/grub.d/40_custom ###
一、grub.cfg详解(红色为说明)
说明：grub.cfg默认为只读，需要个性化配置文件的，建议不要直接修改grub.cfg，请参考链接的pdf文档和google文档。
set default=0
#默认为0
insmod jpeg
#添加jpg支持，如要使用png或tga文件做背景，加上 insmod png或insmod tga
insmod ext2
#除了用作启动的分区外，其他分区格式可在menu底下再添加
set root=(hd0,7)
#设定root分区
search --no-floppy --fs-uuid --set f255285a-5ad4-4eb8-93f5-4f767190d3b3
#设定uuid=****的分区为root，和上句重复，可删除
# 以下为终端配置
if loadfont /usr/share/grub/unicode.pf2 ; then
#设置终端字体，unicode.pf2支持中文显示
set gfxmode=640x480
#设置分辨率，默认为 640x480，可用800x600，1024x768，建议跟你想设定的图片大小一致
insmod gfxterm
#插入模块 gfxterm，支持中文显 示，它还支持 24 位图像
insmod vbe
#插入 vbe 模块，GRUB 2 引入很多模块的东西，要使用它，需要在这里加入
if terminal_output gfxterm ; then true ; else
# For backward compatibility with versions of terminal.mod that don't
# understand terminal_output
terminal gfxterm
#设置 GRUB 2 终端为 gfxterm
fi
fi
set timeout=10
background_image (hd0,7)/boot/images/1.jpg
#设置背景图片
### END /etc/grub.d/00_header ###
### BEGIN /etc/grub.d/05_debian_theme ###
set menu_color_normal=white/black
set menu_color_highlight=cyan/black
#这两行为 Debian 下的菜单颜色设置，如果默认的话，你会发现背景完全被蓝色挡住了，你需要修改 blue 为 black，这样背景就会出现
### END /etc/grub.d/05_debian_theme ###
# 10_linux 为自动添加的当前root分区linux引导项
### BEGIN /etc/grub.d/10_linux ###
#菜单项，要包括 menuentry 双引号" " 和大括号 { }才完整，否则不显示菜单
menuentry "[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2), Linux 2.6.31-9-386" {
insmod ext2
set root=(hd0,7)
search --no-floppy --fs-uuid --set f255285a-5ad4-4eb8-93f5-4f767190d3b3
#这句与set root=(hd0,7)重复，可删除
linux /boot/vmlinuz-2.6.31-9-386 root=UUID=f255285a-5ad4-4eb8-93f5-4f767190d3b3 ro quite splash
#不喜欢看到一长串的， roo=UUID=***可用root=/dev/sda7代替
initrd /boot/initrd.img-2.6.31-9-386
}
### END /etc/grub.d/10_linux ###
### BEGIN /etc/grub.d/20_memtest86+ ###
menuentry "Memory test (memtest86+)" {
linux16 /boot/memtest86+.bin
}
### END /etc/grub.d/20_memtest86+ ###
# 自动添加存在于其他分区的系统引导项
### BEGIN /etc/grub.d/30_os-prober ###
#windows 启动菜单
menuentry "Windows Vista (loader) (on /dev/sda1)" {
insmod ntfs
#windows格式为ntfs，或为fat32改为 insmod fat 
set root=(hd0,1)
search --no-floppy --fs-uuid --set ece067d2e067a196
#可删除
#grub2比较先进的地方就是如果发现windows启动是通过ntldr 引导的，定为2000/xp/2003，会在这加上 drivemap -s (hd0) ${root} ，作用相当于grub的map，可正常启动非第一硬盘的xp/2003系统。
chainloader +1 
}
# 查找到其他分区上的linux系统并自动添加 
menuentry "[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) karmic (development branch) (9.10) (on /dev/sda3)" {
insmod ext2
set root=(hd0,3)
search --no-floppy --fs-uuid --set 4d893970-0685-44ed-86b3-1de45b2db84a
linux /boot/vmlinuz-2.6.31-9-generic root=/dev/sda3
initrd /boot/initrd.img-2.6.31-9-generic
}
#若存在macos会自动在这里添加。
### END /etc/grub.d/30_os-prober ###
# 以下为手动添加的菜单项
### BEGIN /etc/grub.d/40_custom ###
menuentry "CDLinux"{
set root=(hd0,8)
linux /CDlinux/bzImage root=/dev/ram0 vga=791 CDL_LANG=zh_CN.UTF-8
initrd /CDlinux/initrd
}
### END /etc/grub.d/40_custom ###
# 手动添加时，硬盘编号从0开始(hd0)，主分区编号从1开始(hd0,1)，逻辑分区从5开始(hd0,5)
二、grub2终端部分命令介绍
在出现选择菜单时，按C进入终端命令行模式，按E进入当前菜单项编辑模式（和grub一样），编辑中按Ctrl + C退出，按Ctrl + X 以编辑内容启动。
1. help
查看命令用法，显示所有可用命令
help search 
search 命令用法
2. ls
列出当前的所有设备。如 (hd0) (hd0,1) (hd0,5) (hd1) (hd1,1) (hd1,2) .......
ls -l
详细列出当前的所有设备。对于分区，会显示其label及uuid。
ls /
列出当前设为root的分区下的文件
ls (hd1,1)/
列出(hd1,1)分区下文件
3. search
search -f /ntldr
列出根目录里包含ntldr文件的分区，返回为分区号
search -l LINUX
搜索label是LINUX的分区。
search --set -f /ntldr
搜索根目录包含ntldr文件的分区并设为root，注意如果多外分区含有ntldr文件，set 失去作用。
4. loopback
loopback命令可用于建立回放设备，如
loopback lo0 (hd1,1)/abc.iso
可以使用lo0设备来访问abc.iso里的内容，比如说，可以从abc.iso里的软盘映像中启动
loopback lo0 (hd1,1)/aa.iso
linux (lo0)/memdisk
initrd (lo0)/abc.img
要删除某一回放设备，可以使用-d参数：
loopback -d lo0
5. set
使用set可以设置变量的值
set root=
set timeout=
需要调用变量的值时，使用${AA}，如set root=(hd1,1)
则${root}=(hd1,1)
6. pager
分页显示。
set pager=1
满页时暂停，按space继续
set pager=0
取消分页
7. linux
linux取代grub中的kernel
三、
单linux系统或
硬盘安装时iso放在C盘，umount /isodevice引起的误认为单系统
不能出现菜单项的几种处理方法。
1. 开机自检后时按几下shift键，可调出菜单项
2. sudo update-grub
重建grub.cfg，会发现新的系统而改写grub.cfg，一般能出现菜单项。
3.如第二种方法不能解决，直接修改grub.cfg
把在### BEGIN /etc/grub.d/30_os-prober　中的这一段
if keystatus; then
if keystatus --shift; then
set timeout=-1
else
set timeout=0
fi
else
if sleep$verbose --interruptible 3 ; then
set timeout=0
fi
fi
删除或修改三处set timeout=<大于0>
si 4.辛苦修改的grub.cfg在upate-grub后被打回原形，怎么办？
其实，grub不用修改grub.cfg，只要把个性化配置写入 /etc/default/grub 和 /etc/gurb.d下的脚本文件，以后不管升级内核或者是升级grub所执行的update-grub，所创建的grub.cfg都会如你所愿。
先看下 /etc/default/grub，一般可能要修改的是这两处：
GRUB_DEFAULT=0 -----> 设定默认启动项，按需要修改为1, 2, ...
GRUB_TIMEOUT=10 ------->设定倒计时时间，默认10秒，一般改为3-5秒就够了。
再看下 /etc/grub.d文件夹下的什么东东：
00_header 05_debian_theme 10_linux 20_memtest86+ 30_os-prober 40_custom README
前面这五个脚本对应grub.cfg上的各个部分，怎样修改这些脚本文件才能保证不会破坏update-grub的运行，又能让grub.cfg符合你的意愿呢？其实很简单，只要找到
cat << EOF
********
********
********
EOF
这类语句，EOF中间的文本会直接写入grub.cfg中相应位置，所以个性化的语句添加在这地方就可以了。
00_header 可供修改的有两地方
cat << EOF
load_env
set default=${GRUB_DEFAULT} <----- 这里根据 /etc/default/grub中default变量设定，不用修改
if [ /${prev_saved_entry} ]; then
saved_entry=/${prev_saved_entry}
save_env saved_entry
prev_saved_entry=
save_env prev_saved_entry
fi
insmod jpeg <---------这地方加入要加载的模块，如图片支持，或系统格式支持 insmod ntfs 和 insmod fat 等
insmod png <---------
EOF
另一个在最后
cat << EOF
set timeout=${GRUB_TIMEOUT} <------- timeout 根据上面 /etc/default/grub中的timeout 变量设定，这里不用改
background_image (hd0,7)/boot/images/012.jpg <----------这里背景图片的绝对路径
EOF
05_debian_theme 修改背景颜色
cat << EOF
set menu_color_normal=white/black <--------
set menu_color_highlight=yellow/black <------- 前面字体颜色可自定，后面背景一定要设为black，不然背景色会挡住背景图片。
EOF
10_linux 这部分是自动搜索当前系统，建立当前系统的启动菜单，包括系统头，内核等信息，不要随便修改，要使用个性菜单名，比如中文菜单，可修改这里，其余地方不要做修改：
cat << EOF
menuentry "启动 $1" { <---------在 双引号 " " 和 变量 $1 这间加入个性文字，或直接把 变量$1 改为固定字符比如[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) Karmic
EOF
if [ "x$3" = "xquiet" ]; then
cat << EOF
set quiet=1
EOF
20_memtest86+ 添加内存测试启动项，可做修改的地方：
cat << EOF
menuentry "Memory test (memtest86+)" { <------可以修改为中文菜单如 menuentry "内存测试"｛ ，双引号必须是英文字符
linux16 $MEMTESTPATH
}
# 下面第二个菜单项可删除
menuentry "Memory test (memtest86+)"
linux16 $MEMTESTPATH *********
}
EOF
30_os-prober 查找其他分区中存在的系统并建立菜单项，依次为 windows >> linux >> macos，这里不必修改。
或根据自己的系统修改
menuentry "${LONGNAME} (on ${DEVICE})" <-------把引号中的变量改为固定，如 windows xp ,vista 或 linux ,[Fedora](http://www.linuxidc.com/topicnews.aspx?tid=5) 之类的菜单项。
40_custom 自定义启动项，按菜单标准格式添加即可，update-grub后会自动添加到grub.cfg上，如：
menuentry "启动 Veket"{
set root=(hd0,8)
linux /veket/vmlinuz root=/dev/ram0 PMEDIA=hd
initrd /veket/initrd.gz
}
menuentry "启动 CDLinux"{
set root=(hd0,8)
linux /CDlinux/bzImage root=/dev/ram0 vga=791 CDL_LANG=zh_CN.UTF-8
initrd /CDlinux/initrd
}
menuentry "启动 Grub4Dos"{
set root=(hd0,8)
linux /grub.exe
}
把各项修改后保存，然后
sudo update-grub
cat /boot/grub/grub.cfg
看看配置文件是不是和你想的一样了。
不一样？？？　再改，再 sudo update-grub
五、grub2几种修复方法
1. 双系统重装windows造成grub2被改写的修复
方法一 grub4dos0.4.4
在Windows启动项上加上grub4dos启动（不多说了，看置顶贴），重启选择进入grub，在命令行下输入(/boot单独分区的去掉 /boot)
代码:
     grub>find --set-root /boot/grub/core.img
     grub>kernel /boot/grub/core.img
     grub>boot
进入grub2菜单，进入系统后再执行
代码:
    sudo grub-install /dev/sd？
方法二 进入Livecd 后修复（感谢billbear）
引用:
sudo -i
mount 你的根分区 /mnt
mount 你的/boot 分区 /mnt/boot #如果有的话
#挂载你其他的分区，如果有的话
# 重建grub到sda的mbr
grub-install --root-directory=/mnt /dev/sda
2. 由于root分区uuid改变造成的不能正常启动，只能进入grub rescue模式的修复
代码:
grub rescue>set
grub rescue>prefix=(hd？,？)/grub
grub rescue>root=hd？,？
grub rescue>set root=hd？,？
grub rescue>set prefix=(hd？,？)/boot/grub
grub rescue>set
grub rescue>root=hd？,？
grub rescue>prefix=(hd？,？)/boot/grub
grub rescue>insmod /boot/grub/normal.mod
grub rescue>normal
这时就可以调出 /boot/grub/grub.cfg，修改相应uuid，
改到命令行下
grub>insmod /boot/grub/linux.mod
grub>set root=hd？,？
grub>linux /boot/vmlinuz-*** root=/dev/sd？？
grub>initrd /boot/initrg.img-****
进入系统
hd？,？ 是grub文件所在分区 sda？ 是/分区。
3. grub模块和配置文件grub.cfg受损无法启动时修复
Livcd启动进入试用
引用:
sudo -i
mount 你的根分区 /mnt
mount 你的/boot 分区 /mnt/boot #如果有的话
# 挂载你其他的分区，如果有的话
# 重建grub到sda的mbr
grub-install --root-directory=/mnt /dev/sda
# 重建grub.cfg
mount --bind /proc /mnt/proc
mount --bind /dev /mnt/dev
mount --bind /sys /mnt/sys
chroot /mnt update-grub
umount /mnt/sys
umount /mnt/dev
umount /mnt/proc
六、 一些补充说明
1. chainloader
grub2将支持 chainloader /file 的用法。
目前支持的文件只有 grub2 的 boot.img 和 grub4dos 的grldr 和 grub.exe。希望正式版能支持 ntldr bootmgr peldr 等文件。
2. drivemap
drivemap 兼容 grub 的 map，主要用于只能从 (hd0) 引导启动的系统如 win2000 xp 2003，可以象 map 用法一样如:
menuentry "Windows XP" {
insmod ntfs
drivemap (hd0) (hd1)
drivemap (hd1) (hd0)
set root=(hd1,1)
chainloader +1
}
实际上 drivemap 有了更方便的用法：
menuentry "Windows XP" {
insmod ntfs
set root=(hd1,1)
drivemap -s (hd0) ${root}
chainloader +1
}
3. grub2 引导软盘img镜像启动
比如要加载(hd1,1)根目录下的 a.img 镜像，先把 memdisk 从 memdisk.gz 中解压出来，用法是:
linux (hd1,1)/memdisk #镜像文件超过2.88M要加上 c=* h=* s=*
initrd (hd1,1)/a.img
boot
现在有bug，加载memdisk会自动重启，beta2还没修正。
目前只学习到这些，如有新内容以后再补充。
希望各位XDJM纠错和补充，以方便准备使用grub2的朋友。
