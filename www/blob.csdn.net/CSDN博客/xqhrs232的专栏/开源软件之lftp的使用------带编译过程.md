# 开源软件之lftp的使用------带编译过程 - xqhrs232的专栏 - CSDN博客
2016年10月20日 13:59:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3751
原文地址::[http://www.2cto.com/os/201403/288641.html](http://www.2cto.com/os/201403/288641.html)
lftp 是一款非常著名的字符界面的文件传输工具。功能强大的下载工具，它支持访问文件的协议: ftp, ftps, http, https, hftp, fish.(其中ftps和https需要在编译的时候包含openssl库)。llftp的界面非常想一个shell: 有命令补全，历史记录，允许多个后台任务执行等功能，使用起来非常方便。它还有书签、排队、镜像、断点续传、多进程下载等功能。
LFTP is sophisticated ftp/http client, file transfer program supporting a number of network protocols. Like BASH, it has job control and uses readline library
 for input. It has bookmarks, built-in mirror, can transfer several files in parallel. It was designed with reliability in mind. LFTP is free software.
昨天需要同步不同服务器上的代码，但是使用默认的ftp工具不可以递归的下载文件夹，上网搜到了这个软件，感觉不错，所以写下自己的操作步骤，希望对大家有所帮助。
1、下载lftp安装包
我下载的版本是：lftp-4.0.1.tar.bz2，下载地址： [http://download.chinaunix.net/down.php?id=26738&ResourceID=5628&site=1](http://download.chinaunix.net/down.php?id=26738&ResourceID=5628&site=1)
tar.bz2后缀的安装包，解压使用“bzip2 -d lftp-4.0.1.tar.bz2”，生成一个“lftp-4.0.1.tar”tar包，然后释放里面的文件，操作步骤如下：
```java
[/opt/opthb/work/ymm/OpenSource]$bzip2 -d lftp-4.0.1.tar.bz2
[/opt/opthb/work/ymm/OpenSource]$tar -xzvf lftp-4.0.1.tar
[/opt/opthb/work/ymm/OpenSource]$ls
lftp-4.0.1/当然，使用tar的-j参数也是可以解压的，与bzip2的区别是，使用“bzip2”的方法解压时，源文件不再存在，而使用“tar -xjvf”，则会保留源文件，如下：
```
```java
[/opt/opthb/work/ymm/OpenSource]$ls
lftp-4.0.1.tar.bz2
```
下面是man手册中对于tar中中参数的解释：
```java
-j, --bzip2
       filter the archive through bzip2
我们可以看到，使用“-j”参数，使用的依旧是“bzip2”进行解压的。
2、安装lftp
修改configure文件执行权限，并生成makefile文件
使用"configure"命令检查并配置安装需要的系统环境，并生成安装配置文件。可以通过“--prefix 路径”来执行最终生成的库的路径。
#修改最终生成的库的路径为“/opt/opthb/work/ymm/usr/local”
```
```java
[/opt/opthb/work/ymm/OpenSource/lftp-4.0.1]$ ./configure --prefix=“/opt/opthb/work/ymm/usr/local”
```
生成所在环境的makefile之后，编译源文件，并生成可执行文件
命令如下：
```java
[/opt/opthb/work/ymm/OpenSource/lftp-4.0.1]$ make
使用“make install”安装编译过的文件
```
```java
[/opt/opthb/work/ymm/OpenSource/lftp-4.0.1]$ make install
```
安装成功之后，就会在指定的目录“/opt/opthb/work/ymm/usr/local”下面生成需要的可执行文件，如下：
```java
[/opt/opthb/work/ymm/OpenSource/lftp-4.0.1]$ cd /opt/opthb/work/ymm/usr/local
[/opt/opthb/work/ymm/usr/local]$ls
bin  etc  lib  share
[/opt/opthb/work/ymm/usr/local]$ls bin
help  lftp  lftpget
```
通过以上步骤，我们就可以是使用lftp了。
如果我们希望在其他路径也可以使用lftp的话，只需要把bin路径添加到环境变量PATH中就可以了，如下：
```java
[/opt/opthb/work/ymm/usr/local]$lftp
-bash: lftp: command not found
[/opt/opthb/work/ymm/usr/local]$export PATH=/opt/opthb/work/ymm/usr/local/bin:${PATH}                
[/opt/opthb/work/ymm/usr/local]$lftp
lftp :~>
```
3、登录远程服务器
登录远程服务器的方式和普通ftp登录一样，如下：
```java
[/opt/opthb/work/ymm/usr/local]$lftp
lftp :~> open 172.21.0.21
lftp 172.21.0.21:~> user billing_dx
Password: 
lftp billing_dx@172.21.0.21:~>
```
4、使用镜像进行文件的传输
通过help可以查看lftp所支持的所有命令的使用方法
```java
lftp billing_dx@172.21.0.21:~/work/ymm> help mirror
Usage: mirror [OPTS] [remote [local]]
Mirror specified remote directory to local directory
 -c, --continue         continue a mirror job if possible
 -e, --delete           delete files not present at remote site
     --delete-first     delete old files before transferring new ones
 -s, --allow-suid       set suid/sgid bits according to remote site
     --allow-chown      try to set owner and group on files
     --ignore-time      ignore time when deciding whether to download
 -n, --only-newer       download only newer files (-c won't work)
 -r, --no-recursion     don't go to subdirectories
 -p, --no-perms         don't set file permissions
     --no-umask         don't apply umask to file modes
 -R, --reverse          reverse mirror (put files)
 -L, --dereference      download symbolic links as files
 -N, --newer-than=SPEC  download only files newer than specified time
 -P, --parallel[=N]     download N files in parallel
 -i RX, --include RX    include matching files
 -x RX, --exclude RX    exclude matching files
                        RX is extended regular expression
 -v, --verbose[=N]      verbose operation
     --log=FILE         write lftp commands being executed to FILE
     --script=FILE      write lftp commands to FILE, but don't execute them
     --just-print, --dry-run    same as --script=-
When using -R, the first directory is local and the second is remote.
If the second directory is omitted, basename of first directory is used.
If both directories are omitted, current local and remote directories are used.
使用mirror命令可以进行进行文件夹的下载，配合着lcd命令（本地路径），就可以下载自己需要到文件到指定目录：
```
```java
lftp billing_dx@172.21.0.21:~> lcd /opt/opthb/work/ymm/test/
lcd ok, local cwd=/opt/opthb/work/ymm/test
mirror 文件夹，可以递归下载该文件夹到lcd指定的目录，如下：
```
```java
lftp billing_dx@172.21.0.21:~/work/ymm> mirror  DataGen/
Total: 3 directories, 15 files, 0 symlinks                                 
Modified: 1 file, 0 symlinks
195 bytes transferred
```
mirror -R上传文件夹（不可以单独的文件）
```java
lftp billing_dx@172.21.0.21:/tmp/temp> mirror -R ymm.txt	#上传文件出错
mirror: Access failed: /opt/opthb/work/ymm/test/ymm.txt: Not a directory
1 error detected
lftp billing_dx@172.21.0.21:/tmp/temp> mirror -R test		#上传目录正常
Total: 38 directories, 289 files, 0 symlinks                          
New: 289 files, 0 symlinks
3047762 bytes transferred in 2 seconds (1.89M/s)
To be removed: 0 directories, 1 file, 0 symlinks
```
使用断点续传功能和多线程处理的方法
mirror -c --parallel=线程数目 远程路径 本地路径
```java
lftp billing_dx@172.21.0.21:/tmp/temp> mirror -c --parallel=2 sub sub2
Total: 1 directory, 4 files, 0 symlinks
New: 4 files, 0 symlinks
如果本地路径不存在，会自动创建指定命名的目录。
使用上述方法，对于文件数目比较多的情况下，例如用户目录的迁移是很有效的。
而且，我发现通过ftp下载的文件，文件的可执行权限会被去掉，而通过lftp下载的文件，文件的可执行文件会被保留。
扩展阅读： http://download.chinaunix.net/download/0006000/5628.shtml lftp介绍和软件下载
http://blog.csdn.net/xiaotuni/article/details/2099609 tar.bz2文件解压
http://bbs.chinaunix.net/thread-1325666-1-1.html lftp下载整个文件夹
http://blog.csdn.net/helloworldtoo/article/details/7022291 lftp命令介绍
```
//==============================================================================================================================
备注::
编译的基本过程 
  1、下载，解压，创建安装的路径   
  2、./configure --prefix="/home/mysystem/lftp/"----------------这个路径为要安装的路径
  3、make
  4、make install 
   5、home/mysystem/lftp/目录下就有了
