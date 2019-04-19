# Linux下不能加载动态库问题 - sxf_123456的博客 - CSDN博客
2018年03月04日 20:29:11[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：71
当出现下边异常情况
./test: error while loading shared libraries: libmfs_open.so: cannot open shared object file: No such file or directory
若动态库的路径在(/usr/cluster/.share/lib)
解决办法：
方法一、在/etc/ld.so.conf文件中添加路径，vi /etc/ld.so.conf 
添加下边内容
include ld.so.conf.d/*.conf
/usr/cluster/.share/lib
方法二、在终端输入：export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/cluster/.share/lib
方法三、修改/etc/profile文件
export MPI_HOME=/usr/cluster
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MPI_HOME/.share/lib
在终端执行source /etc/profile 使配置文件生效
