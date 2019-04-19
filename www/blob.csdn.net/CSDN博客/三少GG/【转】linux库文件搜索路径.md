# 【转】linux库文件搜索路径 - 三少GG - CSDN博客
2010年09月20日 20:22:00[三少GG](https://me.csdn.net/scut1135)阅读数：855
              众所周知，Linux动态库的默认搜索路径是/lib和/usr/lib。动态库被创建后，一般都复制到这两个目录中。当程序执行时需要某动态库，并且该动 态库还未加载到内存中，则系统会自动到这两个默认搜索路径中去查找相应的动态库文件，然后加载该文件到内存中，这样程序就可以使用该动态库中的函数，以及 该动态库的其它资源了。在Linux 中，动态库的搜索路径除了默认的搜索路径外，还可以通过以下三种方法来指定。
方法一：在配置文件/etc/ld.so.conf中指定动态库搜索路径。 
可以通过编辑配置文件/etc/ld.so.conf来指定动态库的搜索路径，该文件中每行为一个动态库搜索路径。每次编辑完该文件后，都必须运行命令ldconfig使修改后的配置生效。我们通过例1来说明该方法。
例1： 
我们通过以下命令用源程序pos_conf.c（见程序1）来创建动态库 libpos.so，详细创建过程请参考文[1]。
# gcc -c pos_conf.c
         # gcc -shared -fPCI -o libpos.so pos_conf.o
         #
#include 
          void pos()
          {
                 printf("/root/test/conf/lib/n");
          }
           程序1: pos_conf.c
接着通过以下命令编译main.c（见程序2）生成目标程序pos。 
# gcc -o pos main.c -L. -lpos
          #
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
          #
最后编辑配置文件/etc/ld.so.conf，在该文件中追加一行"/root/test/conf/lib"。 
运行程序pos试试。 
# ./pos
          ./pos: error while loading shared libraries: libpos.so: cannot open shared object file: No such file or directory
          #
出错了，系统未找到动态库libpos.so。找找原因，原来在编辑完配置文件/etc/ld.so.conf后，没有运行命令ldconfig，所以刚才的修改还未生效。我们运行ldconfig后再试试。
# ldconfig
          # ./pos
           /root/test/conf/lib 
          #
程序pos运行成功，并且打印出正确结果。 
方法二：通过环境变量LD_LIBRARY_PATH指定动态库搜索路径。 
通过设定环境变量LD_LIBRARY_PATH也可以指定动态库搜索路径。当通过该环境变量指定多个动态库搜索路径时，路径之间用冒号"："分隔。下面通过例2来说明本方法。
例2： 
我们通过以下命令用源程序pos_env.c（见程序3）来创建动态库libpos.so。 
# gcc -c pos_env.c
          # gcc -shared -fPCI -o libpos.so pos_env.o
          #
#include 
         void pos()
         {
               printf("/root/test/env/lib/n");
         }
         程序3: pos_env.c
测试用的可执行文件pos可以使用例1中的得到的目标程序pos，不需要再次编译。因为pos_conf.c中的函数pos和pos_env.c中的函数pos 函数原型一致，且动态库名相同，这就好比修改动态库pos后重新创建该库一样。这也是使用动态库的优点之一。
然后把动态库libpos.so移动到目录/root/test/conf/lib中。 
# mkdir -p /root/test/env/lib
          # mv libpos.so /root/test/env/lib
          #
我们可以使用export来设置该环境变量，在设置该环境变量后所有的命令中，该环境变量都有效。 
例如： 
# export LD_LIBRARY_PATH=/root/test/env/lib
          #
但本文为了举例方便，使用另一种设置环境变量的方法，既在命令前加环境变量设置，该环境变量只对该命令有效，当该命令执行完成后，该环境变量就无效了。如下述命令：
# LD_LIBRARY_PATH=/root/test/env/lib ./pos
         /root/test/env/lib
         #
程序pos运行成功，并且打印的结果是"/root/test/env/lib"，正是程序pos_env.c中的函数pos的运行结果。因此程序pos搜索到的动态库是/root/test/env/lib/libpos.so。
方法三：在编译目标代码时指定该程序的动态库搜索路径。 
还可以在编译目标代码时指定程序的动态库搜索路径。这是通过gcc 的参数"-Wl,-rpath,"指定（如例3所示）。当指定多个动态库搜索路径时，路径之间用冒号"："分隔。
例3： 
我们通过以下命令用源程序pos.c（见程序4）来创建动态库libpos.so。 
# gcc -c pos.c
          # gcc -shared -fPCI -o libpos.so pos.o
          #
#include 
         void pos()
         {
               printf(".//n");
         }
         程序4: pos.c
因为我们需要在编译目标代码时指定可执行文件的动态库搜索路径，所以需要用gcc命令重新编译源程序main.c(见程序2)来生成可执行文件pos。
# gcc -o pos main.c -L. -lpos -Wl,-rpath,./
         #
再运行程序pos试试。
# ./pos
          ./
          #
程序pos运行成功，输出的结果正是pos.c中的函数pos的运行结果。因此程序pos搜索到的动态库是./libpos.so。 
以上介绍了三种指定动态库搜索路径的方法，加上默认的动态库搜索路径/lib和/usr/lib，共五种动态库的搜索路径，那么它们搜索的先后顺序是什么呢？ 
在 介绍上述三种方法时，分别创建了动态库./libpos.so、 /root/test/env/lib/libpos.so和/root/test/conf/lib/libpos.so。我们再用源程序 pos_lib.c（见程序5）来创建动态库/lib/libpos.so，用源程序pos_usrlib.c（见程序6）来创建动态库 /usr/lib/libpos.so。
#include 
          void pos()
          {
                  printf("/lib/n");
          }
           程序5: pos_lib.c
#include 
         void pos()
         {
                printf("/usr/lib/n");
         }
         程序6: pos_usrlib.c
这 样我们得到五个动态库libpos.so，这些动态库的名字相同，且都包含相同函数原型的公用函数pos。但存储的位置不同和公用函数pos 打印的结果不同。每个动态库中的公用函数pos都输出该动态库所存放的位置。这样我们可以通过执行例3中的可执行文件pos得到的结果不同获知其搜索到了 哪个动态库，从而获得第1个动态库搜索顺序，然后删除该动态库，再执行程序pos，获得第2个动态库搜索路径，再删除第2个被搜索到的动态库，如此往复， 将可得到Linux搜索动态库的先后顺序。程序pos执行的输出结果和搜索到的动态库的对应关系如表1所示：
程序pos输出结果
使用的动态库
对应的动态库搜索路径指定方式
./
./libpos.so
编译目标代码时指定的动态库搜索路径
/root/test/env/lib
/root/test/env/lib/libpos.so
环境变量LD_LIBRARY_PATH指定的动态库搜索路径
/root/test/conf/lib
/root/test/conf/lib/libpos.so
配置文件/etc/ld.so.conf中指定的动态库搜索路径
/lib
/lib/libpos.so
默认的动态库搜索路径/lib
/usr/lib
/usr/lib/libpos.so
默认的动态库搜索路径/usr/lib表1: 程序pos输出结果和动态库的对应关系 
创建各个动态库，并放置在相应的目录中。测试环境就准备好了。执行程序pos，并在该命令行中设置环境变量LD_LIBRARY_PATH。 
# LD_LIBRARY_PATH=/root/test/env/lib ./pos
          ./
          #
根据程序pos的输出结果可知，最先搜索的是编译目标代码时指定的动态库搜索路径。然后我们把动态库./libpos.so删除了，再运行上述命令试试。
# rm libpos.so
         rm: remove regular file `libpos.so'? y
         # LD_LIBRARY_PATH=/root/test/env/lib ./pos
         /root/test/env/lib
         #
根据程序pos的输出结果可知，第2个动态库搜索的路径是环境变量LD_LIBRARY_PATH指定的。我们再把/root/test/env/lib/libpos.so删除，运行上述命令。
# rm /root/test/env/lib/libpos.so
         rm: remove regular file `/root/test/env/lib/libpos.so'? y
         # LD_LIBRARY_PATH=/root/test/env/lib ./pos
         /root/test/conf/lib
         #
第3个动态库的搜索路径是配置文件/etc/ld.so.conf指定的路径。删除动态库/root/test/conf/lib/libpos.so后再运行上述命令。
# rm /root/test/conf/lib/libpos.so
         rm: remove regular file `/root/test/conf/lib/libpos.so'? y
         # LD_LIBRARY_PATH=/root/test/env/lib ./pos
         /lib
         #
第4个动态库的搜索路径是默认搜索路径/lib。我们再删除动态库/lib/libpos.so，运行上述命令。
# rm /lib/libpos.so
         rm: remove regular file `/lib/libpos.so'? y
         # LD_LIBRARY_PATH=/root/test/env/lib ./pos
         /usr/lib
         #
最后的动态库搜索路径是默认搜索路径/usr/lib。 
综合以上结果可知，动态库的搜索路径搜索的先后顺序是： 
1.编译目标代码时指定的动态库搜索路径； 
2.环境变量LD_LIBRARY_PATH指定的动态库搜索路径； 
3.配置文件/etc/ld.so.conf中指定的动态库搜索路径； 
4.默认的动态库搜索路径/lib； 
5.默认的动态库搜索路径/usr/lib。 
在上述1、2、3指定动态库搜索路径时，都可指定多个动态库搜索路径，其搜索的先后顺序是按指定路径的先后顺序搜索的。对此本文不再举例说明，有兴趣的读者可以参照本文的方法验证。
**本文来自ChinaUnix博客，如果查看原文请点：**[http://blog.chinaunix.net/u1/43190/showart_425624.html](http://blog.chinaunix.net/u1/43190/showart_425624.html)
