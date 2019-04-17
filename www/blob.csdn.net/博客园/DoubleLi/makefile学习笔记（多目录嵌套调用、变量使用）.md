# makefile学习笔记（多目录嵌套调用、变量使用） - DoubleLi - 博客园






[http://blog.csdn.net/leexiang_han/article/details/9274229](http://blog.csdn.net/leexiang_han/article/details/9274229)




学习了几天的makefile的嵌套调用编写也有一些心得,先声明,我也是初学者写文章只是为了更好的交流,也希望避免其他刚入Linux编程大门的童鞋少走弯路。以下都是经过我多次测试并跑成功的，如有偏差还请见谅与指正。

我有两个练习例子：

①先来说说第一个例子：生成一个可执行文件edit

top dir（根目录）下有源文件main.c、makefile（主）、文件夹sub1、文件夹sub2；子目录sub1里有源文件sub1.c和makefile（次1）；子目录sub2里有源文件sub2.c和makefile（次2）。

main.c里有主函数，需要调用sub1.c里的排序函数（按升序排）和sub2.c里的排序函数（按降序排）。要求通过根目录下的makefile嵌套调用子目录下的makefile来编译，最后链接生成可执行文件edit。

好了，我就不贴三个源文件的C代码了，大家自己写用来测试，我就直接贴makefile的编写了。

首先是子目录sub1下的makefile：

sub1.o : sub1.c

              gcc-c sub1.c

接下来是子目录sub2下的makefile：

sub2.o : sub2.c

       gcc -csub2.c

都很简单对吧，就一条正常的编译命令，最后是根目录下的makefile：

VPATH=./sub1 ./sub2

obj=main.o sub1.o sub2.o



edit : $(obj)

       gcc -oedit main.o ./sub1/sub1.o ./sub2/sub2.o



main.o : main.c

       gcc -cmain.c

sub1.o : sub1.c

       $(MAKE) -C./sub1

sub2.o : sub2.c

       $(MAKE) -C./sub2



.PHONY : clean

clean :

       rm editmain.o -fr

       rm./sub1/*.o -fr

       rm./sub2/*.o -fr



②下面就说说我的第二个练习例子：生成两个可执行文件edit1、edit2

top dir（根目录）下有源文件lib.c、makefile（主）、文件夹main1、文件夹main2；子目录main1里有源文件main1.c和makefile（次1）；子目录main2里有源文件main2.c和makefile（次2）。

main1.c和main2.c里都有主函数，需要调用lib.c里的求和函数，只是所传递的参数值不同。要求通过根目录下的makefile嵌套调用子目录下的makefile来编译，最后链接生成可执行文件edit1和edit2，最后执行edit1和edit2应该能得到两个不同的值（也就是lib.c所求得的和）。

首先是子目录main1的makefile：

main1.o : main1.c

       gcc -cmain1.c



接下来是子目录main2的makefile：

main2.o : main2.c

       gcc -cmain2.c



最后是根目录下的makefile：

VPATH = ./main1 ./main2

obj1 = main1.o lib.o

obj2 = main2.o lib.o



all : edit1 edit2



edit1 : $(obj1)

       gcc -o./main1/edit1 ./main1/main1.o lib.o

main1.o : main1.c

       $(MAKE) -C./main1

lib.o : lib.c

       gcc -clib.c



edit2 : $(obj2)

       gcc -o ./main2/edit2./main2/main2.o lib.o

main2.o : main2.c

       $(MAKE) -C./main2



.PHONY : clean

clean :

       rm *.o -fr

       rm./main1/*.o ./main1/edit1 -fr

       rm./main2/*.o ./main2/edit2 –fr



难道你认为这次学习就结束了吗？？当然不！上面这些makefile的确能满足嵌套调用的基本需求，但是你不觉得./sub1/sub1.c，./sub2/sub.2这些路径的表达很烦琐吗？万一这是个大的工程项目，突然要往某个子目录下增加一个源文件，那么修改这些makefile还不弄死你啊，可怕的不是累而是容易遗漏某一处而导致错误，所以对于立志成为一个好的makefile的编写员的人这些还远远不够！下面就让我们来对以上的makefile进行一次改头换面吧！领略一下变量的风采！

①    第一个例子的根目录下的makefile修改如下，其余两个makefile不变：

cur-dir:=$(shell pwd)

s1=$(cur-dir)/sub1

s2=$(cur-dir)/sub2



obj=main.o sub1.o sub2.o



edit : $(obj)

       gcc -oedit main.o $(s1)/sub1.o $(s2)/sub2.o



main.o : main.c

       gcc -cmain.c

sub1.o : $(s1)/sub1.c

       $(MAKE) -C$(s1)

sub2.o : $(s2)/sub2.c

       $(MAKE) -C$(s2)



.PHONY : clean

clean :

       rm editmain.o -fr

       rm $(s1)/*.o-fr

       rm$(s2)/*.o -fr



②    第二个例子的根目录下的makefile修改如下，其余两个makefile不变：

cur-dir:=$(shell pwd)

s1=$(cur-dir)/main1

s2=$(cur-dir)/main2



obj1 = main1.o lib.o

obj2 = main2.o lib.o



all : edit1 edit2



edit1 : $(obj1)

       gcc -o$(s1)/edit1 $(s1)/main1.o lib.o

main1.o : $(s1)/main1.c

       $(MAKE) -C$(s1)

lib.o : lib.c

       gcc -clib.c



edit2 : $(obj2)

       gcc -o$(s2)/edit2 $(s2)/main2.o lib.o

main2.o : $(s2)/main2.c

       $(MAKE) -C$(s2)



.PHONY : clean

clean :

       rm *.o -fr

       rm$(s1)/*.o $(s1)/edit1 -fr

       rm$(s2)/*.o $(s2)/edit2 -fr










