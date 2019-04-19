# linux 下常用命令 - sxf_123456的博客 - CSDN博客
2017年09月28日 21:09:25[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：139标签：[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux command](https://blog.csdn.net/sxf_123456/article/category/6748440)
                
linux 下显示一个文件中的某几行(中间几行)
1、从100行开始，显示200行；即显示100-299行
cat filename | tail -n +100 | head -n 200
2、显示100行到300行
cat filename | head -n 300 | tail -n 100
3、显示最后100行
tail -n 100
4、显示从1000行开始显示，显示1000行以后的
tail -n +1000
5、显示前面1000行
head -n 1000 
6、用sed命令
查看文件的第5行到第10行内容
sed -n '5,10p' filename
7、mkdir 命令
-p 目录不存在时，则创建
mkdir -p /usr/test/t
8、rm 命令
删除任何.log文件，删除前逐一确认
rm -i *.log
删除子目录及子目录的所有文档删除
rm -rf test
删除以-f开头的文件
rm -- -f*
9、当parent子目录被删除后它也成为空目录的话，一并删除
rmdir -p /parent/child/child1
10、文件重命名
将文件test.log重命名为test1.txt
mv test.log test1.txt
将文件log1.txt,log2.txt,log3.txt移动到根目录test3中
mv log1.txt log2.txt log3.txt /test3
将文件file1改名为file2，如果file2已经存在，则询问是否覆盖
mv -i log1.xt log2.txt
将源文件复制到目标文件，或将多个源文件复制到目标目录
复制a.txt到test目录下，保持原文件时间，如果原文件存在则提示是否覆盖
cp -ai a.txt test
为a.txt建议一个链接
cp -s a.txt link_a.txt
11、head命令
显示文件前20行内容
head 1.log -n 20
显示文件前20个字节
head -c 20 1.log
显示1.log的最后10行
head -n -10 1.log
12、which 命令
linux 下查找某个文件，可以使用以下命令
which 查看可执行的文件的位置
whereis 查看文件的位置
locate 配合数据库查看文件的位置
find  查找文件
13、find命令
-size n:[c] 查找文件长度为n块文件，带有c时表示文件字节大小
-amin n 查找系统最后N分钟访问的文件
-ctime n 查找系统最后n*24小时被改变文件状态的文件
-cmin  n 查看系统中最后N分钟被改变的文件
-mmin n 查找系统中最后N分钟被改变文件数据的文件
-mtime n 查找系统中最后n*24小时被改变文件数据的文件
查找48小时内修改过的文件
find -atime -2
在当前目录查找以.log结尾的文件。
find ./ -name '*.log'
查找/opt目录下权限为777的文件
find /opt -perm 777
查找大于1k的文件
find -size +1000c
find -size 1000c 查找等于1000字符的文件
14、tar命令
-c 建立新的压缩文件
-f 指定压缩文件
-r 添加文件到已经压缩文件包中
-x 从压缩包中抽取文件
-t 显示压缩文件中的内容
-z 支持gzip压缩
-j 支持bzip2压缩
-Z 支持compress压缩文件
-v 显示操作过程
将文件全包打包成tar包
tar -cvf log.tar 1.log 2.log 
将/etc下的所有文件及目录打包到指定目录，并使用gz压缩
tar -zcvf /tmp/etc.tar.gz /etc
查看刚打包的文件内容
tar -ztvf /tmp/etc.tar.gz
要压缩打包/home,/etc但不要/home/dmtsai
tar --exclude /home/dmtsai -zcvf myfile.tar.gz /home/* /etc
chown命令
-c 显示更改的部分信息
-R 处理指定目录及子目录
改变拥有者和群组并显示改变信息
chown -c mail:mail 1.log
改变文件群组
chown -c :mail 1.log
改变文件夹及子文件目录属主及属组为mail
chown -cR mail: test/
15、df命令
-a 显示全部文件系统列表
-h 以方便阅读的方式显示信息
-i 显示inode信息
-k 区块为1024字节
-l 只显示本地磁盘
-T 列出文件系统类型
显示磁盘使用情况
df -l
以易读的方式列出所有文件系统及其类型
df -haT
16、du命令
du命令也是查看使用空间，但是与df命令不同的是du 命令是对文件和目录磁盘使用空间的查看
-a 显示目录中所有文件大小
-k 以KB为单位显示文件大小
-m 以MB为单位显示文件大小
-g 以GB为单位显示文件大小
-h 以易读的方式显示文件大小
以易读的方式显示文件夹及子文件夹大小
du -h scf/
以易读方式显示文件夹内所有文件大小
du -ha scf/
显示几个文件或目录各自用磁盘空间的大小，还统计它们的总和
du -hc test/ scf/
输出当前目录下各个子目录所使用的空间
du -hc --max-depth=1 scf/
17、ln命令
-b 删除，覆盖以前建立的链接
-s 软链接
-v 显示详细处理过程
给文件创建软链接，并显示操作信息
ln -sv 1.txt link_1.txt
给文件创建硬链接，并显示操作信息
ln -v 1.txt link_1.txt
给目录创建软链接
ln -sv /opt/soft/test/test3 /opt/soft/test/test5
18、date 命令
-d<字符串> 显示字符串所指的日期与时间，字符串前后必须加上双引号
-s<字符串> 根据字符串来设置日期与时间。字符串前后必须加上双引号
-u 显示GMT
%H 小时(00-23)
%I 小时(00-12)
%M 分钟(以00-59)
%s 总秒数，起算时间为1970-01-01 00:00:00 UTC。
19、top命令
显示当前系统正在执行的进程的相关信息，包括进程ID、内存占用率、CPU占用率
-c 显示完整的进程命令
-s 保密模式
-p 指定进程显示
-n 循环显示次数
20、kill 命令
先使用p查找进程pro1,然后用kill杀掉
kill -9 $(ps -ef | grep pro1)
21、free命令
-b 以Byte显示内存使用情况
-k 以kb显示内存使用情况
-m 以mb为单位显示内存使用情况
-g 以gb为单位显示内存使用情况
-s 持续显示内存 <秒>
-t 显示内存使用总和
(1) 显示内存使用情况
free
free -k
free -m
(2) 以总和的形式显示内存的使用情况
free -t
(3) 周期性查询内存使用情况
free -s 10
