# linux系统下c/cpp编译链接时系统的默认搜索路径 - happyhorizon的算法天空 - CSDN博客
2018年03月15日 20:30:03[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：119
C/C++程序在linux下被编译和连接时，GCC/G++会查找系统默认的include和link的路径，以及自己在编译命令中指定的路径。自己指定的路径就不说了，这里说明一下系统自动搜索的路径。
## include头文件路径
```
除了默认的/usr/include, /usr/local/include等include路径外，还可以通过设置环境变量来添加系统include的路径：
```
```bash
# C
    export C_INCLUDE_PATH=XXXX:$C_INCLUDE_PATH
    # CPP
    export CPLUS_INCLUDE_PATH=XXX:$CPLUS_INCLUDE_PATH
```
以上修改可以直接命令行输入（一次性），可以在/etc/profile中完成（对所有用户生效），也可以在用户home目录下的.bashrc或.bash_profile中添加（针对某个用户生效），修改完后重新登录即生效。
## link链接库文件路径
链接库文件在连接（静态库和共享库）和运行（仅限于使用共享库的程序）时被使用，其搜索路径是在系统中进行设置的（也可以在编译命令中通过 -l  -L 来指定，这里讲的是使用系统默认搜索路径）。
一般 Linux 系统把 /lib  /usr/lib  /usr/local/lib 作为默认的库搜索路径，所以使用这几个目录中的链接库文件可直接被搜索到（不需要专门指定链接库路径）。对于默认搜索路径之外的库，则需要将其所在路径添加到gcc/g++的搜索路径之中。
链接库文件的搜索路径指定有两种方式：修改环境变量或者修改/etc/so.ld.conf : 
    1）在环境变量中添加 
    动态链接库搜索路径： 
`export LD_LIBRARY_PATH=XXX:$LD_LIBRARY_PATH`
    静态链接库搜索路径： 
`export LIBRARY_PATH=XXX:$LIBRARY_PATH`
    以上修改可以直接命令行输入（一次性），可以在/etc/profile中完成（对所有用户生效），也可以在用户home目录下的.bashrc或.bash_profile中添加（针对某个用户生效）,修改完后重新登录即生效。 
    2）在/etc/ld.so.conf 中添加指定的链接库搜索路径（需要root权限），然后运行 /sbin/ldconfig，以达到刷新 /etc/ld.so.cache的效果。 
      以上两种方式均可以达到指定链接库搜索路径的效果。
