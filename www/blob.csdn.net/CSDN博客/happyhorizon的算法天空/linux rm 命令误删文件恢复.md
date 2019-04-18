# linux rm 命令误删文件恢复 - happyhorizon的算法天空 - CSDN博客
2019年01月26日 10:44:55[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：61
不小心用rm命令删错了文件,该怎么办?
# 查看分区和文件格式
## 误删的文件在哪里
首先, 用rm命令误删了文件,并不是不可以恢复. 首先需要查看一下误删文件所在的分区和文件格式
```
$df -T
文件系统       类型          1K-块       已用       可用 已用% 挂载点
/dev/sda3      ext4      171639696  128461364   34436444   79% /home
/dev/sdb       ext4      961303584  268191752  644257320   30% /home/dl/BrainLab
```
## 如何卸载误删文件所在的分区
首先:
**卸载被误删除文件所在的分区，或以只读方式mount**
误删文件后第一件事是:马上卸载被误删除文件所在的分区，或者以只读的方式来挂载该分区。
这是因为文件被删除后，文件中的数据还存在磁盘上，除非存放这些数据的数据块又被操作系统分配出去了。我们这一步就是尽量降低数据块中数据被覆盖的风险，以提高恢复数据成功的比率。
如果被误删的文件在根分区，那么你最好重启计算机，进入单用户模式，以只读的方式挂载根分区，然后再进行恢复。
进入单用户模式后，根分区还是以读写方式mount的，用下面的命令，把挂载方式由读写(rw)改为只读(ro)：
```
mount -o ro,remount /
```
如果被删除的文件不是根分区，也可以用unmount的方式将该分区卸载。假设文件在分区/dev/sda3中，该分区挂载到/home，那么我们用下面的命令来卸载：
```
umount /dev/sda3
```
或者命令：
```
umount /home
```
这里, 我误删的文件在"/home/dl/BrainLab"目录，可以知道分区是/dev/sdb, 文件格式是ext4.
# 误删恢复
## ext4误删恢复
安装extundelete
```
sudo apt-get install extundelete
```
这里强调一下，不要把代码下载到要恢复的分区中，要恢复文件的分区不要做任何的数据读写操作！
例如/dev/sdb是一块开机自动挂载的新硬盘, 可以考虑先unmout这个盘.
执行:
```
sudo extundelete /dev/sdb --restore-all
```
建议恢复所有的被删除的文件，因为extundelete有可能会把恢复的文件改名。在程序结束后，会生成目录RECOVERED_FILES
```
cd RECOVERED_FILES
```
到这个目录下就可以找到想要恢复的文件, 如果找不到, 有可能extundelete将恢复的文件重命名了，例如文件中newClass这个类, 那么可以用grep命令查找:
```
grep -rHn "newClass" *
```
根据输出内容就可以找到需要的文件，拷贝出来并重命名。
## ext3\ext2 误删恢复
对于ext3格式的分区, 可以安装ext3grep工具 :
```
sudo apt-get install ext3grep
```
对于ext2格式的分区,可以利用debugfs工具.具体方法见参考.
# 参考:
[https://blog.csdn.net/fjb2080/article/details/5604103](https://blog.csdn.net/fjb2080/article/details/5604103)
恢复ext3分区上误删的文件:
[https://blog.easwy.com/archives/undelete-directories-files-on-ext3-filesystem-via-ext3grep/](https://blog.easwy.com/archives/undelete-directories-files-on-ext3-filesystem-via-ext3grep/)
恢复ext2分区上误删的文件:
[http://www.th7.cn/system/lin/201605/165053.shtml](http://www.th7.cn/system/lin/201605/165053.shtml)
