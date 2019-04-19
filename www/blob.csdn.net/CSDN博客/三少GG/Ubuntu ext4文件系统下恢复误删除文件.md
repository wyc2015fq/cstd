# Ubuntu ext4文件系统下恢复误删除文件 - 三少GG - CSDN博客
2012年07月27日 10:43:25[三少GG](https://me.csdn.net/scut1135)阅读数：6598
### [linux ext3 ext4文件系统下 恢复 rm －rf 误删除文件](http://qyiyunso.blog.163.com/blog/static/350776862010826115537113/)
今天一时手快 rm -rf .* ，删除了一个重要邮件目录，幸好通过extundelete恢复了。
记下操作流程：
1.准备工作
主要通过 [extundelete
 0.2.0](https://sourceforge.net/project/platformdownload.php?group_id=260221)完成恢复
需要依赖两个包
> 
sudo aptitude install e2fsprogs e2fslibs-dev
下载页面[http://extundelete.sourceforge.net/](http://extundelete.sourceforge.net/)
[](http://extundelete.sourceforge.net/)快速下载： [download
 extundelete](https://sourceforge.net/project/platformdownload.php?group_id=260221)
tar -xjf extundelete-0.2.0.tar.bz2
cd extundelete-0.2.0
编译三部曲
> 
./configure
make
sudo make install
**2.如果分区已经挂载，先umount，并且在将要恢复的地方查看空间大小**
> 
df -h ./
文件系统               1K-块        已用     可用 已用% 挂载点
/dev/sda3             18129292   2651744  14541764  16% /
/dev/sda1               194442     12197    172206   7% /boot
...
**要 umount  /home!!!**
如果你要恢复的是根目录或者系统目录就只有拿硬盘挂到另外一台电脑上操作了。
3.恢复制定分区下所有删除文件，默认恢复到执行命令的目录下，文件夹名字为RECOVERED_FILES，使用详情请参考：extundelete --help 运行的结果
恢复/dev/sda3里一个被误删除的文件
extundelete /dev/sda3 --restore-file /an/important/file
**恢复/dev/sda3里一个被误删除的文件夹**
**extundelete /dev/sda3 --restore-directory /an/important**
**eg: grant/workspaces/...**
恢复/dev/sda3里所有被删除的东西
extundelete /dev/sda3 --restore-all
> 
sudo extundelete /dev/sda8 –restore-all
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
$ extundelete --help
Usage: extundelete [options] [--] device-file
Options:
  --version, -[vV]       Print version and exit successfully.
  --help,                Print this help and exit successfully.
  --superblock           Print contents of superblock in addition to the rest.
                         If no action is specified then this option is implied.
  --journal              Show content of journal.
  --after dtime          Only process entries deleted on or after 'dtime'.
  --before dtime         Only process entries deleted before 'dtime'.
Actions:
  --inode ino            Show info on inode 'ino'.
  --block blk            Show info on block 'blk'.
  --restore-inode ino[,ino,...]
                         Restore the file(s) with known inode number 'ino'.
                         The restored files are created in ./RESTORED_FILES
                         with their inode number as extension (ie, file.12345).
  --restore-file 'path'  Will restore file 'path'. 'path' is relative to root
                         of the partition and does not start with a '/' (it
                         must be one of the paths returned by --dump-names).
                         The restored file is created in the current
                         directory as 'RECOVERED_FILES/path'.
  --restore-files 'path' Will restore files which are listed in the file 'path'.
                         Each filename should be in the same format as an option
                         to --restore-file, and there should be one per line.
  --restore-all          Attempts to restore everything.
  -j journal             Reads an external journal from the named file.
  -b blocknumber         Uses the backup superblock at blocknumber when opening
                         the file system.
  -B blocksize           Uses blocksize as the block size when opening the file
                         system.  The number should be the number of bytes.
[root@yznvm1 yzn]# extundelete **/dev/sda3**--restore-file /home/yzn/helloworld/Makefile.am        
WARNING: Extended attributes are not restored.
WARNING: EXT3_FEATURE_INCOMPAT_RECOVER is set.
The partition should be unmounted to undelete any files without further data loss.
If the partition is not currently mounted, this message indicates 
it was improperly unmounted, and you should run fsck before continuing.
If you decide to continue, extundelete may overwrite some of the deleted
files and make recovering those files impossible.  You should unmount the
file system and check it with fsck before using extundelete.
Would you like to continue? (y/n) 
y
Loading filesystem metadata ... 143 groups loaded.
Loading journal descriptors ... 29535 descriptors loaded.
Writing output to directory RECOVERED_FILES/
Restored inode 4223596 to file RECOVERED_FILES/home/yzn/helloworld/Makefile.am
ok,这样就恢复成功了。
当然文件不能保证一定恢复成功，我的另外一个文件恢复出来是乱码，还有一个文件报错找不到inode.
