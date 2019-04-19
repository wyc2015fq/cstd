# dd命令详解 - xqhrs232的专栏 - CSDN博客
2011年10月15日 11:13:33[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：663标签：[file																[磁盘																[oracle																[permissions																[脚本																[cookies](https://so.csdn.net/so/search/s.do?q=cookies&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=permissions&t=blog)](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/liumang_D/article/details/3899462](http://blog.csdn.net/liumang_D/article/details/3899462)
一、dd命令的解释。
dd：用指定大小的块拷贝一个文件，并在拷贝的同时进行指定的转换。
注意：指定数字的地方若以下列字符结尾则乘以相应的数字：b=512；c=1；k=1024；w=2
参数：
1. if=文件名：输入文件名，缺省为标准输入。即指定源文件。< if=input file >
2. of=文件名：输出文件名，缺省为标准输出。即指定目的文件。< of=output file >
3. ibs=bytes：一次读入bytes个字节，即指定一个块大小为bytes个字节。
    obs=bytes：一次输出bytes个字节，即指定一个块大小为bytes个字节。
    bs=bytes：同时设置读入/输出的块大小为bytes个字节。
4. cbs=bytes：一次转换bytes个字节，即指定转换缓冲区大小。
5. skip=blocks：从输入文件开头跳过blocks个块后再开始复制。
6. seek=blocks：从输出文件开头跳过blocks个块后再开始复制。
注意：通常只用当输出文件是磁盘或磁带时才有效，即备份到磁盘或磁带时才有效。
7. count=blocks：仅拷贝blocks个块，块大小等于ibs指定的字节数。
8. conv=conversion：用指定的参数转换文件。
    ascii：转换ebcdic为ascii
     ebcdic：转换ascii为ebcdic
    ibm：转换ascii为alternate ebcdic
    block：把每一行转换为长度为cbs，不足部分用空格填充
    unblock：使每一行的长度都为cbs，不足部分用空格填充
    lcase：把大写字符转换为小写字符
    ucase：把小写字符转换为大写字符
    swab：交换输入的每对字节
     noerror：出错时不停止
     notrunc：不截短输出文件
    sync：将每个输入块填充到ibs个字节，不足部分用空（NUL）字符补齐。
二、dd应用实例。
1.将本地的/dev/hdb整盘备份到/dev/hdd
   dd if=/dev/hdb of=/dev/hdd
2.将/dev/hdb全盘数据备份到指定路径的image文件
   dd if=/dev/hdb of=/root/image
3.将备份文件恢复到指定盘
   dd if=/root/image of=/dev/hdb
4.备份/dev/hdb全盘数据，并利用gzip工具进行压缩，保存到指定路径
    dd if=/dev/hdb | gzip > /root/image.gz
5.将压缩的备份文件恢复到指定盘
   gzip -dc /root/image.gz | dd of=/dev/hdb
6.备份磁盘开始的512个字节大小的MBR信息到指定文件
   dd if=/dev/hda of=/root/image count=1 bs=512
   count=1指仅拷贝一个块；bs=512指块大小为512个字节。
   恢复：dd if=/root/image of=/dev/hda
7.备份软盘
   dd if=/dev/fd0 of=disk.img count=1 bs=1440k (即块大小为1.44M)
8.拷贝内存内容到硬盘
   dd if=/dev/mem of=/root/mem.bin bs=1024 (指定块大小为1k)  
9.拷贝光盘内容到指定文件夹，并保存为cd.iso文件
   dd if=/dev/cdrom(hdc) of=/root/cd.iso
10.增加swap分区文件大小
第一步：创建一个大小为256M的文件：
dd if=/dev/zero of=/swapfile bs=1024 count=262144
第二步：把这个文件变成swap文件：
mkswap /swapfile
第三步：启用这个swap文件：
swapon /swapfile
第四步：编辑/etc/fstab文件，使在每次开机时自动加载swap文件：
/swapfile    swap    swap    default   0 0
11.销毁磁盘数据
     dd if=/dev/urandom of=/dev/hda1
注意：利用随机的数据填充硬盘，在某些必要的场合可以用来销毁数据。
12.测试硬盘的读写速度
     dd if=/dev/zero bs=1024 count=1000000 of=/root/1Gb.file
      dd if=/root/1Gb.file bs=64k | dd of=/dev/null
通过以上两个命令输出的命令执行时间，可以计算出硬盘的读、写速度。
13.确定硬盘的最佳块大小：
     dd if=/dev/zero bs=1024 count=1000000 of=/root/1Gb.file
     dd if=/dev/zero bs=2048 count=500000 of=/root/1Gb.file
     dd if=/dev/zero bs=4096 count=250000 of=/root/1Gb.file
     dd if=/dev/zero bs=8192 count=125000 of=/root/1Gb.file
通过比较以上命令输出中所显示的命令执行时间，即可确定系统最佳的块大小。
14.修复硬盘：
     dd if=/dev/sda of=/dev/sda 或dd if=/dev/hda of=/dev/hda
当硬盘较长时间(一年以上)放置不使用后，磁盘上会产生magnetic flux point，当磁头读到这些区域时会遇到困难，并可能导致I/O错误。当这种情况影响到硬盘的第一个扇区时，可能导致硬盘报废。上边的命令有可能使这些数据起死回生。并且这个过程是安全、高效的。
**/dev/null和/dev/zero的区别**
/dev/null，外号叫无底洞，你可以向它输出任何数据，它通吃，并且不会撑着！
/dev/zero,是一个输入设备，你可你用它来初始化文件。
/dev/null，外号叫无底洞，你可以向它输出任何数据，它通吃，并且不会撑着！
/dev/zero,是一个输入设备，你可你用它来初始化文件。
/dev/null------它是空设备，也称为位桶（bit bucket）。任何写入它的输出都会被抛弃。如果不想让消息以标准输出显示或写入文件，那么可以将消息重定向到位桶。
/dev/zero------该设备无穷尽地提供0，可以使用任何你需要的数目——设备提供的要多的多。他可以用于向设备或文件写入字符串0。
oracle@localhost oracle]$if=/dev/zero of=./test.txt bs=1k count=1
oracle@localhost oracle]$ ls -l
total 4
-rw-r--r--    1 oracle   dba          1024 Jul 15 16:56 test.txt
eg,
    find / -name access_log  2>/dev/null
使用/dev/null 
把/dev/null看作"黑洞". 它非常等价于一个只写文件. 所有写入它的内容都会永远丢失. 而尝试从它那儿读取内容则什么也读不到. 然而, /dev/null对命令行和脚本都非常的有用.
禁止标准输出.    1 cat $filename >/dev/null
   2 # 文件内容丢失，而不会输出到标准输出.
禁止标准错误 (来自例子 12-3).    1 rm $badname 2>/dev/null
   2 #           这样错误信息[标准错误]就被丢到太平洋去了.
禁止标准输出和标准错误的输出.    1 cat $filename 2>/dev/null >/dev/null
   2 # 如果"$filename"不存在，将不会有任何错误信息提示.
   3 # 如果"$filename"存在, 文件的内容不会打印到标准输出.
   4 # 因此Therefore, 上面的代码根本不会输出任何信息.
   5 #
   6 #  当只想测试命令的退出码而不想有任何输出时非常有用。
   7 #
   8 #
   9 # cat $filename &>/dev/null
  10 #     也可以, 由 Baris Cicek 指出.
Deleting contents of a file, but preserving the file itself, with all attendant permissions (from Example 2-1 and Example 2-3):    1 cat /dev/null > /var/log/messages
   2 #  : > /var/log/messages   有同样的效果, 但不会产生新的进程.（因为:是内建的）
   3 
   4 cat /dev/null > /var/log/wtmp
自动清空日志文件的内容 (特别适合处理这些由商业Web站点发送的讨厌的"cookies"):
--------------------------------------------------------------------------------
例子 28-1. 隐藏cookie而不再使用
   1 if [ -f ~/.netscape/cookies ]  # 如果存在则删除.
   2 then
   3   rm -f ~/.netscape/cookies
   4 fi
   5 
   6 ln -s /dev/null ~/.netscape/cookies
   7 # 现在所有的cookies都会丢入黑洞而不会保存在磁盘上了.
--------------------------------------------------------------------------------
使用/dev/zero 
像/dev/null一样, /dev/zero也是一个伪文件, 但它实际上产生连续不断的null的流（二进制的零流，而不是ASCII型的）. 写入它的输出会丢失不见, 而从/dev/zero读出一连串的null也比较困难, 虽然这也能通过od或一个十六进制编辑器来做到. /dev/zero主要的用处是用来创建一个指定长度用于初始化的空文件，就像临时交换文件.
--------------------------------------------------------------------------------
例子 28-2. 用/dev/zero创建一个交换临时文件
   1 #!/bin/bash
   2 # 创建一个交换文件.
   3 
   4 ROOT_UID=0         # Root 用户的 $UID 是 0.
   5 E_WRONG_USER=65    # 不是 root?
   6 
   7 FILE=/swap
   8 BLOCKSIZE=1024
   9 MINBLOCKS=40
  10 SUCCESS=0
  11 
  12 
  13 # 这个脚本必须用root来运行.
  14 if [ "$UID" -ne "$ROOT_UID" ]
  15 then
  16   echo; echo "You must be root to run this script."; echo
  17   exit $E_WRONG_USER
  18 fi  
  19   
  20 
  21 blocks=${1:-$MINBLOCKS}          #  如果命令行没有指定，
  22                                  #+ 则设置为默认的40块.
  23 # 上面这句等同如：
  24 # --------------------------------------------------
  25 # if [ -n "$1" ]
  26 # then
  27 #   blocks=$1
  28 # else
  29 #   blocks=$MINBLOCKS
  30 # fi
  31 # --------------------------------------------------
  32 
  33 
  34 if [ "$blocks" -lt $MINBLOCKS ]
  35 then
  36   blocks=$MINBLOCKS              # 最少要有 40 个块长.
  37 fi  
  38 
  39 
  40 echo "Creating swap file of size $blocks blocks (KB)."
  41 dd if=/dev/zero of=$FILE bs=$BLOCKSIZE count=$blocks  # 把零写入文件.
  42 
  43 mkswap $FILE $blocks             # 将此文件建为交换文件（或称交换分区）.
  44 swapon $FILE                     # 激活交换文件.
  45 
  46 echo "Swap file created and activated."
  47 
  48 exit $SUCCESS
--------------------------------------------------------------------------------
关于 /dev/zero 的另一个应用是为特定的目的而用零去填充一个指定大小的文件, 如挂载一个文件系统到环回设备 （loopback device） (参考例子 13-8) 或"安全地" 删除一个文件(参考例子 12-55).
--------------------------------------------------------------------------------
例子 28-3. 创建ramdisk
   1 #!/bin/bash
   2 # ramdisk.sh
   3 
   4 #  "ramdisk"是系统RAM内存的一段，
   5 #+ 它可以被当成是一个文件系统来操作.
   6 #  它的优点是存取速度非常快 (包括读和写).
   7 #  缺点: 易失性, 当计算机重启或关机时会丢失数据.
   8 #+       会减少系统可用的RAM.
   9 #
  10 #  那么ramdisk有什么作用呢?
  11 #  保存一个较大的数据集在ramdisk, 比如一张表或字典,
  12 #+ 这样可以加速数据查询, 因为在内存里查找比在磁盘里查找快得多.
  13 
  14 
  15 E_NON_ROOT_USER=70             # 必须用root来运行.
  16 ROOTUSER_NAME=root
  17 
  18 MOUNTPT=/mnt/ramdisk
  19 SIZE=2000                      # 2K 个块 (可以合适的做修改)
  20 BLOCKSIZE=1024                 # 每块有1K (1024 byte) 的大小
  21 DEVICE=/dev/ram0               # 第一个 ram 设备
  22 
  23 username=`id -nu`
  24 if [ "$username" != "$ROOTUSER_NAME" ]
  25 then
  26   echo "Must be root to run /"`basename $0`/"."
  27   exit $E_NON_ROOT_USER
  28 fi
  29 
  30 if [ ! -d "$MOUNTPT" ]         #  测试挂载点是否已经存在了,
  31 then                           #+ 如果这个脚本已经运行了好几次了就不会再建这个目录了
  32   mkdir $MOUNTPT               #+ 因为前面已经建立了.
  33 fi
  34 
  35 dd if=/dev/zero of=$DEVICE count=$SIZE bs=$BLOCKSIZE  # 把RAM设备的内容用零填充.
  36                                                       # 为何需要这么做?
  37 mke2fs $DEVICE                 # 在RAM设备上创建一个ext2文件系统.
  38 mount $DEVICE $MOUNTPT         # 挂载设备.
  39 chmod 777 $MOUNTPT             # 使普通用户也可以存取这个ramdisk.
  40                                # 但是, 只能由root来缷载它.
  41 
  42 echo "/"$MOUNTPT/" now available for use."
  43 # 现在 ramdisk 即使普通用户也可以用来存取文件了.
  44 
  45 #  注意, ramdisk是易失的, 所以当计算机系统重启或关机时ramdisk里的内容会消失.
  46 #
  47 #  拷贝所有你想保存文件到一个常规的磁盘目录下.
  48 
  49 # 重启之后, 运行这个脚本再次建立起一个 ramdisk.
  50 # 仅重新加载 /mnt/ramdisk 而没有其他的步骤将不会正确工作.
  51 
  52 #  如果加以改进, 这个脚本可以放在 /etc/rc.d/rc.local,
  53 #+ 以使系统启动时能自动设立一个ramdisk.
  54 #  这样很合适速度要求高的数据库服务器.
  55 
  56 exit 0
这样，一些诸如一些错误信息就不会显示出来。
