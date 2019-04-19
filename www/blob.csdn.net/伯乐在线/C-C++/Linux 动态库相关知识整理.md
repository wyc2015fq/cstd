# Linux 动态库相关知识整理 - 文章 - 伯乐在线
原文出处： [左小田（@左小田）](http://zkt.name/linuxgong-xiang-ku-de-chuang-jian-yu-shi-yong/)
动态库和静态库在C/C++开发中很常见，相比静态库直接被编译到可执行程序，动态库运行时加载使得可执行程序的体积更小，更新动态库可以不用重新编译可执行程序等诸多好处。作者是一个Linux后台开发，这些知识经常用到，所以整理了一下这方面的知识。静态库相对简单，本文只关心Linux平台下的动态库。
### 创建动态库
这里我把一个短小却很有用的哈希函数编译成动态库做为示例，ELFhash用于对字符串做哈希，返回一个无符号整数。

C
```
//elfhash.h
#include 
unsigned long  ELFhash(const char* key);
//elfhash.c
#include "elfhash.h"
unsigned long ELFhash(const char* key)  
{
  unsigned long h = 0, g;
  while( *key ) {
    h = ( h > 24;
    h &= ~g;
}
  return h;
}
```
接下来使用gcc编译以上代码，并用ld将编译的目标文件链接成动态库

Shell
```
gcc -fPIC -c -Wall elfhash.c  
ld  -shared elfhash.o -o libelfhash.so
```
其中`-fPIC`意思是生成位置无关的代码(Position Independent Code)，适用于动态库，在多个进程中共享动态库的同一份代码。ld的`-shared`选项告诉链接器创建的是动态库。gcc也可以间接调用ld生成动态库

Shell
```
gcc -fPIC -shared -Wall -o libelfhash.so elfhash.c
```
### 使用动态库
动态库的使用方法有两种一种是隐式使用，第二种是显式使用。隐式使用的方法很简单。

C
```
#include "elfhash.h"
int main()  
{
    printf("%ldn", ElfHash("key-for-test"));
    return 0;
}
```
显式使用动态库需要借助以下几个函数

C
```
#include 
void *dlopen(const char *filename, int flag); //flag可以是RTLD_LAZY，执行共享库中的代码时解决未定义符号，RTLD_NOW则是dlopen返回前解决未定义符号。  
char *dlerror(void); //当发生错误时，返回错误信息  
void *dlsym(void *handle, const char *symbol); //获取符号  
int dlclose(void *handle); //关闭
```
应用上面几个函数，调用ELFhash实现跟隐式调用一样的功能

C
```
#include "elfhash.h"
#include 
#include 
int main() {  
    void *handle;
    unsigned long (*hash)(const char*);
    char *error;
    handle = dlopen ("./libelfhash.so", RTLD_LAZY);
    if (!handle) {
        fputs (dlerror(), stderr);
        exit(1);
    }
    hash = dlsym(handle, "ElfHash");
    if ((error = dlerror()) != NULL)  {
         fputs(error, stderr);
         exit(1);
    }
    printf ("%ldn", (*hash)("key-for-test"));
    dlclose(handle);
}
```
至此了解以上的知识就可以创建和使用动态库了。 实际应用中我们可能还是会遇到一些问题。
### 动态库的加载
动态库创建那一节，我演示如何隐式使用动态库，那么编译运行这段代码试一下。

C
```
gcc main.c -L./ -lelfhash  
./a.out //执行可执行程序
//以下是输出结果
./a.out: error while loading shared libraries: libelfhash.so: cannot open shared object file: No such file or directory
```
结果运行时报错，可执行程序找不到动态库。 网上有一些说法是编译时设置`-L`选项，但在Linux上面证明是不行的（SunOS上可行），这个选项只能在编译链接时有效， 可以让你使用`-l`如上面的`-lelfhash`。使用`readelf -d a.out`可以看到可执行文件依赖的动态库信息。

Shell
```
0x0000000000000001 (NEEDED)  Shared library: [libelfhash.so]
```
可以看到这里面并没有包含动态库的路径信息。查阅一下动态链接器的文档`man ld-linux.so`可以发现这样一句话(有的没有，版本问题)
> 
If a slash is found, then the dependency string is interpreted as a (relative or absolute) pathname, and the library is loaded using that pathname
这段话太长，我只截取一部分，大致就是说，当依赖中有`/`符号,那么会被解析成动态库加载的路径，隐式使用的例子换一种编译方法。

Shell
```
gcc main.c ./libelfhash.so  
./a.out
23621492 //输出正常
```
再用`readelf -d a.out`查看会发现，依赖信息中有了一个路径。

Shell
```
0x0000000000000001 (NEEDED)  Shared library: [./libelfhash.so]
```
这种方法虽然解决了问题，但是依赖中的路径是硬编码，不是很灵活。 动态链接器是如何查找的动态库的需要进一步查阅文档。关于查找的顺序有点长，这里就不直接引用了，大致是这样：
- (仅ELF文件) 使用可执行文件中DT_RPATH区域设置的属性,如果DT_RUNPATH被设置，那么忽略DT_RPATH（在我的Linux对应的是RPATH和RUNPATH）。
- 使用环境变量LD_LIBRARY_PATH，如果可执行文件中有set-user-id/set-group-id, 会被忽略。
- (仅ELF文件) 使用可执行文件中DT_RUNPATH区域设置的属性
- 从/etc/ld.so.cache缓存文件中查找
- 从默认路径/lib, /usr/lib文件目录中查找
我们需要设置RPATH或者RUNPATH,可以这样做

Shell
```
gcc main.c -Wl,-rpath,/home/xxx,--enable-new-dtags -L./  -lelfhash
```
这里的`-Wl`选项告诉链接器`ld`如果如何处理，接下来传递的`-rpath`(或者使用`-R`)告诉`ld`动态库的路径信息(注意`-Wl,`和后面选项之间不能有空格)。如果没有`--enable-new-dtags`那么只会设置RPATH，反之，RPATH和RUNPATH会同时被设置。使用`readelf -d a.out`查看结果:

Shell
```
0x000000000000000f (RPATH)  Library rpath: [/home/xxx]  
0x000000000000001d (RUNPATH)  Library runpath: [/home/xxx]
```
如果使用环境变量LD_LIBRARY_PATH，那么一般这样用 export

Shell
```
export LD_LIBRARY_PATH=/home/xxx;$LD_LIBRARY_PATH
```
RPATH和RUNPATH指定动态库的路径，用起来简单，但是也缺乏灵活性，LD*LIBRARY*PATH在临时测试的也是很有用的，但是在正式环境中，直接使用它也不是好的实践，因为环境变量跟用户的环境关系比较大。动态库不仅要考虑自己使用， 还有分发给别的用户使用的情况。
更通用的方法是使用`ldconfig`，有几种方法，先在`/etc/ld.so.conf.d/`目录下创建一个文件，然后把你的动态库路径写进去。或者将你的动态库放到`/lib,/lib64(64位),/usr/lib,/usr/lib64(64位)`然后运行`sudo ldconfig`重建`/etc/ld.so.cache`文件。
### 动态库版本
通常在使用第三方给的动态库的时候，都是带有版本（文件命名），可以在`/usr/lib64`下看到很多这样的动态库。现在我重新编译动态库，这次加上版本信息。

Shell
```
gcc -fPIC -shared -Wall -Wl,-soname,libelfhash.so.0  -o libelfhash.so.0.0.0 elfhash.c
```
每个动态库都有一个名字，如这里的*libelfhash.so.0.0.0*，叫`real name`，命名规则跟简单,通常是`libxxx.so.MAJOR.MINOR.VERSION`(有的时候VERSION会被省略),如果动态库在接口上的兼容性，比如删除了接口或者修改了接口参数，MAJOR增加，如果接口兼容，只是做了更新或者bug修复那么MINOR和VERSION增加。也就是说MAJOR相同的库接口都是兼容的，反之不兼容，如果使用不兼容的动态库需要重新编译可执行程序。
编译动态库时，通过给`ld`传递连接选项`-soname`可以指定一个`soname`, 如这里的*libelfhash.so.0* 只保留MAJOR,可执行程序运行加载动态库时，会加载这个指定名字的库。
动态库还有一个名字是`link name`,编译可执行程序时，传个链接器`ld`的动态库名字，通常是没有版本号以.so结尾的文件名。 一般作法是对soname创建软链。
按照这个规则来命名的动态库可以`ldconfig`识别，我们把*libelfhash.so.0.0.0*放到`/usr/lib64`文件夹中，执行以下指令

Shell
```
$sudo ldconfig -v | grep libelfhash.so
libelfhash.so.0 -> libelfhash.so.0.0.0
```
可以发现`ldconfig`根据*libelfhash.so.0.0.0*的信息，创建了一个soname指向real name的软链，当动态库更新(MINOR,VERSION增加)，拷贝新库到相应的位置，再执行`sudo ldconfig`会自动更新软链指向最新的动态库，动态库更新就完成了。
### 总结
OK，关于Linux动态库知识整理就到这里了，这些知识虽说都是些基础，少有涉及动态库内部的一些原理，但是却很常用。整理过程中我带着疑问去阅读了`ld`和`ld-linux.so`的文档，收获颇丰。同样，希望本文能帮你解释遇到的部分问题或疑惑。
