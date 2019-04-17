# sqlite3移植到arm linux - DoubleLi - 博客园






1,环境：

软件：linux：2.6.38

硬件：6410

交叉编译工具：arm-linux-gcc

 也适用于其他linux平台。

2,步骤：

1》下载sqlite3源码包：

[http://www.sqlite.org/sqlite-amalgamation-3.6.1.tar.gz](http://www.sqlite.org/sqlite-amalgamation-3.6.1.tar.gz)



2》复制到工作目录/tmp

cp sqlite-amalgamation-3.6.1.tar.gz /tmp



3》解压生成源码目录

tar xvzf   sqlite-amalgamation-3.6.1.tar.gz



4》配置

进入源码目录

./configure --prefix=/nfs/sqlite --target=arm-linux --host=arm-linux LD=arm-linux-ld

注：/nfs是我的板子挂载的根文件系统的目录，

arm-linux是交叉编译器的前缀，例如你的交叉编译器是arm-linux-gcc,去掉-gcc剩下的部分。host和LD都需要跟着变。



5》编译

make



6》安装

make install

完成后/nfs/sqlite目录下会有bin,include,lib三个目录。



7》复制生成的头文件和库到板子

cp /nfs/sqlite/bin/* /nfs/usr/bin

cp /nfs/sqlite/lib/* /nfs/lib



8》编写测试文件：test.c

略。



9》编译

arm-linux-gcc -I/nfs/sqlite/include -L/nfs/sqlite/lib -lsqlite3 test.c -o test



10》如果不想在编译的时候加-L和-I选项，就需要把库和可执行文件拷到交叉编译环境的lib和bin目录下，这样在编译的时候会自动找到。

具体命令略。



 11》另外需要注意的是，如果你用的是boa打开数据库文件，需要把数据库文件拷到boa配置的根目录下的某个目录，然后在调用sqlite3_open函数时指定该目录。









