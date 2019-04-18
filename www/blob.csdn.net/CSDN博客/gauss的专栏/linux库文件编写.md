# linux库文件编写 - gauss的专栏 - CSDN博客
2015年05月09日 12:32:00[gauss](https://me.csdn.net/mathlmx)阅读数：252
 linux库文件的编写
作者: laomai
地址: [http://blog.csdn.net/laomai](http://blog.csdn.net/laomai)
本文主要参考了如下资料
⑴hcj写的"Linux静态/动态链接库的创建和使用"
地址 [http://fanqiang.chinaunix.net/system/linux/2006-05-08/4126.shtml](http://fanqiang.chinaunix.net/system/linux/2006-05-08/4126.shtml)
⑵雨亦奇的文章"LINUX动态链接库高级应用"
地址[http://www.ccw.com.cn/htm/center/prog/02_3_13_3_2.asp](http://www.ccw.com.cn/htm/center/prog/02_3_13_3_2.asp))
在此一并致谢。
一、为什么要使用库文件
 我们在实际编程工作中肯定会遇到这种情况：有几个项目里有一些函数模块的功能相同，
实现代码也相同，也是我们所说的重复代码。比如，很多项目里都有一个用户验证的功能。
代码段如下：
   //UserLogin.h文件，提供函数声明
   int IsValidUser(char* username, int namelen);
   //UserLogin.c文件，实现对用户信息的验证
   int  IsValidUser(char* username, int namelen)
   {
     int IsValid = 0;
     /*下面是具体的处理代码，略去*/
     return IsValid
   }
   如果每个项目都保存着这两个UserLogin.h和UserLogin.c文件，会有以下几个
弊端:
   1、每个项目里都有重复的模块，造成代码重复。   
   2、代码的重用性不好，一旦IsValidUser的代码发生了变化，为了保持设计的一致性，
我们还要手工修改其他项目里的UserLogin.c文件，既费时又费力，还容易出错。   
库文件就是对公共代码的一种组织形式。 
    为了解决上面两个弊端，就提出了用库文件存放公共代码的解决方案，其要点就是
把公共的（也就是可以被多次复用的）目标代码从项目中分离出来，统一存放到库文件中，
项目要用到这些代码的时候，在编译或者运行的时候从库文件中取得目标代码即可。库文件
又分两种：静态库和动态库。
二、静态库与动态库
    如果程序是在编译时加载库文件的，就是使用了静态库。如果是在运行时加载目标代码，
就成为动态库。换句话说，如果是使用静态库，则静态库代码在编译时就拷贝到了程序的代码段，
程序的体积会膨胀。如果使用动态库，则程序中只保留库文件的名字和函数名，在运行时去查找
库文件和函数体，程序的体积基本变化不大。
    静态库的原则是“以空间换时间”，增加程序体积，减少运行时间;
    动态库则是“以时间换空间”，增加了运行时间，但减少了程序本身的体积。
    下面我们就以实际例子来看看如何使用这两种库.
三、静态库的编写和使用
1、概述
   静态库文件的扩展名一般为.a,其编写步骤很简单。
 ⑴编写函数代码
 ⑵编译生成各目标文件
 ⑶用ar文件对目标文件归档，生成静态库文件。
    注意归档文件名必须以lib打头。
   使用要点：
   ⑴在gcc 的-I参数后加上静态库头文件的路径。
   ⑵在gcc 的-L参数后加上库文件所在目录
   ⑶在gcc 的-l参数后加上库文件名，但是要去掉lib和.a扩展名。
     比如库文件名是libtest.a 那么参数就是 -l test
2、编写最简单的静态库文件
   编写如下两个文件，注意放在同一目录中 
    myalib.h   //静态库头文件
    myalib.c   //静态库实现文件
    //myalib.h  文件的内容
 void test();
 //myalib.c  文件的内容
 #inlcude <stdio.h>
 void test()
 {
  printf("test/n");
 }
3、制作库文件
 ⑴生成目标文件
  gcc -c myalib.c
  执行完后会生成一个myalib.o文件
 ⑵用ar命令归档,格式为ar -rc <生成的档案文件名> <.o文件名列表>
 再次提醒，归档文件名一定要以lib打头, .a结尾。
  ar -rc libtest.a myalib.o
  执行完后会生成一个libtest.a文件
4、使用库文件 
 ⑴编写一个测试程序main.c,内容为
 //main.c 测试静态库调用的程序
 #include "myalib.h"   //要把函数的头文件包含进来，否则编译时会报错
  int main(int argc,char* argv[])
 {
  test();
  return 0;  
 }
 ⑵编译目标文件，注意要把静态库头文件的路径加到-I参数里面
   gcc -I /root/exercise -o main.o -c main.c
  现在生成了一个main.o文件
 ⑶生成可执行文件，注意要把静态库文件的路径加到-L参数里面，
 把库文件名(去掉打头的lib和结尾的.a)加到-l参数后面。如下面所示
  gcc -o main -L/root/exercise   main.o -ltest
  此时就会生成一个名为main的可执行文件
  另外，注意- l参数好象应该加到输入文件名的后面，否则会报错。
  比如gcc -o main -L/root/exercise   -ltest main.o就会提示
  main.o(.text+0x11): In function `main':
  : undefined reference to `test'
  collect2: ld returned 1 exit status
  原因我还不清楚:-)
 ⑷执行可执行文件查看效果
  执行./main，  输出
  test
  说明执行成功。
四、动态库的编写
1、概述
   动态库一般以.so结尾，就是shared object的意思.
   其基本生成步骤为
   ⑴编写函数代码
   ⑵编译生成动态库文件,要加上 -shared 和 -fpic 选项 ，
     库文件名以lib开头， 以.so 结尾。
   使用方式分为两种: 隐式调用和显示调用  
    隐式调用类似于静态库的使用，但需修改动态链接库的配置文件/etc/ld.so.conf;
    显示调用则是在主程序里使用dlopen、dlsym、dlerror、dlclose等系统函数。
    具体的调用方式会在 "五、动态库的调用" 中详细说明.
2、编写最简单的动态库文件
   为了便于对照， 我们仍然采用静态库中的文件做例子.
    编写如下两个文件，注意放在同一目录中 
    myalib.h   //静态库头文件
    myalib.c   //静态库实现文件
    //myalib.h  文件的内容
 void test();
 //myalib.c  文件的内容
 #inlcude <stdio.h>
 void test()
 {
  printf("test/n");
 }
3、编译生成动态库 ,库文件名以lib开头， 以.so 结尾。
 gcc -fpic -shared -o libtest.so  myalib.c
 此时就生成一个libtest.so文件
五、动态库的隐式调用
  隐式调用的含义是代码里不出现库文件名，就是说这个代码和
  调用静态库的代码是类似的。
1、编写测试文件
 //main.c 测试动态库隐式调用的程序
 #include "myalib.h"   //要把函数的头文件包含进来，否则编译时会报错
  int main(int argc,char* argv[])
 {
  test();
  return 0;  
 }
2、 编译测试程序,与静态库类似,要把头文件的路径加到-I参数里面
  gcc -I /root/exercise -o main.o -c main.c
  现在生成了一个main.o文件
3、连接生成测试程序  
  gcc -o main -L/root/exercise   main.o -ltest
  现在生成了一个main文件
4、执行测试程序
  ./main 
  此时出现提示
    ./main: error while loading shared libraries: libtest.so: cannot open shared object file: No such file or directory。
    这个原因就是程序运行时并不知道动态库所在的路径，因此自然找不到。
    解决这个问题的办法有三种。见下节
六、使动态库被系统共享的三种办法
   (再次说明: 本节参考了计算机世界网雨亦奇的文章"LINUX动态链接库高级应用"
     地址[http://www.ccw.com.cn/htm/center/prog/02_3_13_3_2.asp](http://www.ccw.com.cn/htm/center/prog/02_3_13_3_2.asp))   
(1)拷贝动态链接库到系统共享目录下,或在系统共享目录下为该动态链接库
   建立连接(硬连接或符号连接均可,常用符号连接).这里说的系统共享目录,
   指的是LINUX动态链接库存放的目录,包括
   /lib,/usr/lib以及/etc/ld.so.conf文件内所列的一系列目录.
   实例：执行 
 # cp libtest.so /lib 
 # ldconfig 
  或: 
  # ln -s `pwd`/libtest.so /lib 
 # ldconfig 
 注意pwd前后有两个反引号`，其目的是取得pwd命令的输出，即当前目录.
 此时再执行main，即可成功.
(2)将动态链接库所在目录名追加到动态链接库配置文件/etc/ld.so.conf中. 
# pwd >> /etc/ld.so.conf 
# ldconfig 
 此时再执行main，即可成功.
(3)利用动态链接库管理命令ldconfig,强制其搜索指定目录,并更新缓存文件,便于动态装入. 
 # ldconfig `pwd` 
   此时再执行main，即可成功.
   要注意,第三种方法虽然有效,但效果是暂时的,供程序测试还可以,一旦再度运行ldconfig,
   则缓存文件内容可能改变,所需的动态链接库可能不被系统共享了.
   而且无论哪种办法，其实质都是用ldconfig命令把动态库文件
   所在路径加入到系统库列表中，(前两种永久，第三种临时)
七、动态库的显式调用
  显式调用的含义是代码出现库文件名，用户需要自己去
  打开和管理库文件。其要点为:
  ⑴把dlfcn.h系统头文件包含进来
  ⑵用dlopen函数打开库文件，并指定打开方式
 dllope的的第一个参数为共享库的名称，将会在下面位置查找指定的共享库。
  ①环境变量LD_LIBRARY_PATH列出的用分号间隔的所有目录。
  ②文件/etc/ld.so.cache中找到的库的列表，由ldconfig命令刷新。
  ③目录usr/lib。
  ④目录/lib。
  ⑤当前目录。
 第二个参数为打开共享库的方式。有两个取值
  ①RTLD_NOW：将共享库中的所有函数加载到内存
  ②RTLD_LAZY：会推后共享库中的函数的加载操作，直到调用dlsym()时方加载某函数
  ⑶用dlerror()函数测试是否打开成功，并进行错误处理;
  ⑷用dlsym获得函数地址,存放在一个函数指针中
  ⑸用获得的函数指针进行函数调用。
  ⑹程序结束时用dlclose关闭打开的动态库，防止资源泄露。
  ⑺用ldconfig工具把动态库的路径加到系统库列表中
1、编写测试文件
 //main.c 测试动态库显式调用的程序
 #include<dlfcn.h>      //用于动态库管理的系统头文件   
 #include "myalib.h"    //要把函数的头文件包含进来，否则编译时会报错
  int main(int argc,char* argv[])
 {
   //声明对应的函数的函数指针
  void (*pTest)();    
         //加载动态库
  void *pdlHandle = dlopen("libtest.so", RTLD_LAZY);  
  //错误处理
        if(pdlHandle == NULL )    {
         printf("Failed load library/n");
         return -1;
     }
     char* pszErr = dlerror();
     if(pszErr != NULL)
     {
         printf("%s/n", pszErr);
         return -1;
     }
     //获取函数的地址
     pTest = dlsym(pdlHandle, "test"); 
     pszErr = dlerror();
     if(pszErr != NULL)
     {
         printf("%s/n", pszErr);
         dlclose(pdlHandle);
         return -1;
     }
     //实现函数调用
     (*pTest)();
  //程序结束时关闭动态库
  dlclose(pdlHandle);
  return 0;  
 }  
2、编译测试文件 使用-ldl选项指明生成的对象模块需要使用共享库
 gcc -o main -ldl main.c
 执行完后就生成了一个main文件
3、执行测试程序
 执行 ./main
 输出
    test
    说明成功。
六、使用动态库时应注意的其他问题
1、无论是动态库的显式调用还是隐式调用，都需要用
ldconfig工具将动态库的路径加到系统库列表中，否则运行时会出错。   
2、可以用ldd命令检查程序都使用到哪些共享库
 ldd命令行用法如下: 
 ldd [--version] [-v|--verbose] [-d|--data-relocs] [-r|--function-relocs] [--help] FILE...
  各选项说明如下: 
  (1) --version : 此选项用于打印出ldd的版本号. 
  (2) -v 或 --verbose : 此选项指示ldd输出关于所依赖的动态链接库的尽可能详细的信息. 
  (3) -d 或 --data-relocs : 此选项执行重定位,并且显示不存在的函数. 
  (4) -r 或 --function-relocs : 此选项执行数据对象与函数的重定位,同时报告不存在的对象. 
  (5) --help : 此选项用于打印出ldd的帮助信息. 
  我们一般用-v选项.
  现在看几个实例
  ⑴用静态库连接时的结果  
 #ldd main
 libc.so.6 => /lib/tls/libc.so.6 (0xb74ad000)
 /lib/ld-linux.so.2 => /lib/ld-linux.so.2 (0xb75eb000)
 可见使用静态库时，由于库已经被编译成程序的一部分，因此ldd的输出中就只有用到的
 系统库。
 ⑵用动态库隐式连接时的结果 
  libtest.so => /root/exercise/libtest.so (0xb75e2000)
     libc.so.6 => /lib/tls/libc.so.6 (0xb74ab000)
     /lib/ld-linux.so.2 => /lib/ld-linux.so.2 (0xb75eb000)
     可见隐式使用动态库时，所有用到的动态库(包括系统和用户的)都会被显示出来。
 ⑶动态库显式连接时的结果
 ldd main
 libdl.so.2 => /lib/libdl.so.2 (0xb75e1000)
    libc.so.6 => /lib/tls/libc.so.6 (0xb74aa000)
    /lib/ld-linux.so.2 => /lib/ld-linux.so.2 (0xb75eb000)
 可见显式使用动态库时，程序中不再保存运行时打开动态库的信息，只保留用到的系统库的信息.   
 这个与使用静态库时的输出是类似的.
