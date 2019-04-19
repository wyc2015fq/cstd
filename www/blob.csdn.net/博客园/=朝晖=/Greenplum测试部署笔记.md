# Greenplum测试部署笔记 - =朝晖= - 博客园
# [Greenplum测试部署笔记](https://www.cnblogs.com/dhcn/p/7823100.html)
      按照官方Readme文档在Ubunut16.04上成功编译安装Greenplum最新代码(now：2017-11-12 21:40)
      按照文档安装的过程中主要出现两个问题：
      1、Root用户安装会卡在：make create-demo-cluster这一步,原因就是Root用户不行。
      2、Ok，adduser了一个新用户安装，中间第一次出现的问题是src里面某个目录不能操作导致失败，不能操作的目录是：
```
mkdir: cannot create directory ‘~/gpdb_src/gpAux/gpdemo/datadirs’: Permission denied
```
       解决的方法也很简单，无非是上一级目录直接chmod 777.
     继续出现的问题是：
```
/usr/local/gpdb/bin/postgres: error while loading shared libraries: libgpopt.so.3: cannot open shared object file: No such file or directory
```
     解决的方法也很简单，这个文件实际是在/usr/local/lib里面，把这个目录加进/usr/local/gpdb/greenplum_path.sh文件的LD_LIBRARY_PATH配置中。
     ok,Demo cluster启动成功。
     阿里云按量按小时买的大数据服务，部署起来略有麻烦哦。
     /usr/local/gpdb/bin目录里面的脚本执行需要一些环境变量，除了上面的/usr/local/gpdb/greenplum_path.sh这个文件需要source以外,如果想用下面gpstop指令重启服务器的话，还需要source ~/gpdb_src/gpAux/gpdemo/gpdemo-env.sh ：
```
source /usr/local/gpdb/greenplum_path.sh
source ~/gpdb_src/gpAux/gpdemo/gpdemo-env.sh 
/usr/local/gpdb/bin/gpstop -u
```

