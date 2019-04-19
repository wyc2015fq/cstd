# Linux动态库lib和inlcude 加载方法 - fanyun的博客 - CSDN博客
2019年02月23日 22:10:12[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：29
       Linux动态库的默认搜索路径是/lib和/usr/lib。动态库被创建后，一般都复制到这两个目录中。当程序执行时需要某动态库，并且该动态库还未加载到内存中，则系统会自动到这两个默认搜索路径中去查找相应的动态库文件，然后加载该文件到内存中，这样程序就可以使用该动态库中的函数，以及该动态库的其它资源了。在Linux 中，动态库的搜索路径除了默认的搜索路径外，还可以通过以下三种方法来指定。
　　方法一：在配置文件/etc/ld.so.conf中指定动态库搜索路径。
　　可以通过编辑配置文件/etc/ld.so.conf来指定动态库的搜索路径，该文件中每行为一个动态库搜索路径。每次编辑完该文件后，都必须运行命令ldconfig使修改后的配置生效。我们通过例1来说明该方法。
　　例1：
　　我们通过以下命令用源程序pos_conf.c(见程序1)来创建动态库 libpos.so，详细创建过程请参考文[1]。
　　# gcc -c pos_conf.c
　　# gcc -shared -fPCI -o libpos.so pos_conf.o
　　#
　　#include
　　void pos()
　　{
　　printf("/root/test/conf/lib\n");
　　}
　　程序1: pos_conf.c
　　接着通过以下命令编译main.c(见程序2)生成目标程序pos。
　　# gcc -o pos main.c -L. -lpos
　　void pos();
　　int main()
　　{
　　pos();
　　return 0;
　　}
　　程序2: main.c
　　然后把库文件移动到目录/root/test/conf/lib中。
　　# mkdir -p /root/test/conf/lib
　　# mv libpos.so /root/test/conf/lib
　　最后编辑配置文件/etc/ld.so.conf，在该文件中追加一行"/root/test/conf/lib"。
　　运行程序pos试试。
　　# ./pos
　　./pos: error while loading shared libraries: libpos.so: cannot openshared object file: No such file or directory
　　出错了，系统未找到动态库libpos.so。找找原因，原来在编辑完配置文件/etc/ld.so.conf后，没有运行命令ldconfig，所以刚才的修改还未生效。我们运行ldconfig后再试试。
　　# ldconfig
　　# ./pos
　　/root/test/conf/lib
程序pos运行成功，并且打印出正确结果。
方法二：通过环境变量LD_LIBRARY_PATH指定动态库搜索路径。
　　通过设定环境变量LD_LIBRARY_PATH也可以指定动态库搜索路径。当通过该环境变量指定多个动态库搜索路径时，路径之间用冒号"："分隔。下面通过例2来说明本方法。
　　例2：
　　我们通过以下命令用源程序pos_env.c(见程序3)来创建动态库libpos.so。
　　# gcc -c pos_env.c
　　# gcc -shared -fPCI -o libpos.so pos_env.o
　　#include
　　void pos()
　　{
　　printf("/root/test/env/lib\n");
　　}
　　程序3: pos_env.c
　　测试用的可执行文件pos可以使用例1中的得到的目标程序pos，不需要再次编译。因为pos_conf.c中的函数pos和pos_env.c中的函数pos 函数原型一致，且动态库名相同，这就好比修改动态库pos后重新创建该库一样。这也是使用动态库的优点之一。
　　然后把动态库libpos.so移动到目录/root/test/conf/lib中。
　　# mkdir -p /root/test/env/lib
　　# mv libpos.so /root/test/env/lib
　　我们可以使用export来设置该环境变量，在设置该环境变量后所有的命令中，该环境变量都有效。
　　例如：
　　# export LD_LIBRARY_PATH=/root/test/env/lib
　　但本文为了举例方便，使用另一种设置环境变量的方法，既在命令前加环境变量设置，该环境变量只对该命令有效，当该命令执行完成后，该环境变量就无效了。如下述命令：
　　# LD_LIBRARY_PATH=/root/test/env/lib ./pos
　　/root/test/env/lib
　　程序pos运行成功，并且打印的结果是"/root/test/env/lib"，正是程序pos_env.c中的函数pos的运行结果。因此程序pos搜索到的动态库是/root/test/env/lib/libpos.so。
　　方法三：在编译目标代码时指定该程序的动态库搜索路径。
　　还可以在编译目标代码时指定程序的动态库搜索路径。这是通过gcc 的参数"-Wl,-rpath,"指定(如例3所示)。当指定多个动态库搜索路径时，路径之间用冒号"："分隔。
