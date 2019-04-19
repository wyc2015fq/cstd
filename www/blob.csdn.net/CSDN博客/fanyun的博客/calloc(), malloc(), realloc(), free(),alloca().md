# calloc(), malloc(), realloc(), free(),alloca() - fanyun的博客 - CSDN博客
2016年10月30日 20:45:52[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：299标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

内存区域可以分为栈、堆、静态存储区和常量存储区，局部变量，函数形参，临时变量都是在栈上获得内存的，它们获取的方式都是由编译器自动执行的。
利用指针，我们可以像汇编语言一样处理内存地址，C 标准函数库提供了许多函数来实现对堆上内存管理，其中包括：malloc函数，free函数，calloc函数和realloc函数。使用这些函数需要包含头文件stdlib.h。
　　四个函数之间的有区别，也有联系，我们应该学会把握这种关系，从而编出精炼而高效的程序。
在说明它们具体含义之前，先简单从字面上加以认识，前3个函数有个共同的特点，就是都带有字符”alloc”，就是”allocate”，”分配”的意思，也就是给对象分配足够的内存，” calloc()”是”分配内存给多个对象”，” malloc()”是”分配内存给一个对象”，”realloc()”是”重新分配内存”之意。”free()”就比较简单了，”释放”的意思，就是把之前所分配的内存空间给释放出来。
void *calloc(size_t nobj, size_t size);
　　分配足够的内存给nobj个大小为size的对象组成的数组, 并返回指向所分配区域的第一个字节的指针;
　　若内存不够,则返回NULL. 对于分配到的内存块初始化为0，无需要使用memset初始化。有经验的程序员更喜欢使用calloc()，因为这样的话新分配内存的内容就不会有什么问题，调用calloc()肯定会清0。
　　char *p = (char *) calloc(100,sizeof(char));
void *malloc(size_t size);
　　分配足够的内存给大小为size的对象, 并返回指向所分配区域的第一个字节的指针;
　　若内存不够,则返回NULL. 不对分配的空间进行初始化.
　　char *p = (char *)malloc(sizeof(char));
void *realloc(void *p, size_t size);
　　将p所指向的对象的大小改为size个字节.
　　如果新分配的内存比原内存大, 那么原内存的内容保持不变, 增加的空间不进行初始化.
　　如果新分配的内存比原内存小, 那么新内存保持原内存的内容, 增加的空间不进行初始化.
　　返回指向新分配空间的指针; 若内存不够,则返回NULL, 原p指向的内存区不变.
　　char *p = (char *)malloc(sizeof(char));
　　p= (char *)realloc(p, 256);
对realloc的详细语法说明：
　　void *realloc( void *memblock, size_t size );
　　1、如果memblock = 0, size ！= 0，表示为空指针分配空间，同malloc(memblock,
 size) 
　　2、如果memblock ！= 0, size = 0，表示将已分配的空间释放，同free(memblock) 
　　3、如果memblock ！= 0, size != 0，表示将已分配的空间重新分配， 
　　　　1)、如果size < memblock已分配内存大小，则将memblock的后部分切除，返回指针和memblock相等。 
　　　　2)、如果size > memblock已分配内存大小， 
　　　　　　(1)、并且memblock大小和其后的自由内存总和大于等于size，则分配size大小的内存，返回指针和memblock相等。 
　　　　　　(2)、如果memblock大小和其后的自由内存总和小于size，则在其他地方分配size大小的内存，返回指针和memblock不相等。如果其他地方也没有足够空间分配内存，则返回指针NULL。
void free(void *p);
　　释放p所指向的内存空间; 当p为NULL时, 不起作用.P必先调用calloc, malloc或realloc.
值得注意的有以下5点：
（1）通过malloc函数得到的堆内存必须使用memset函数来初始化 
　　malloc函数分配得到的内存空间是未初始化的。因此，一般在使用该内存空间时，要调用另一个函数memset来将其初始化为全0，memset函数的声明如下：void * memset (void * p,int c,int n) ;
　　该函数可以将指定的内存空间按字节单位置为指定的字符c，其中，p为要清零的内存空间的首地址，c为要设定的值，n为被操作的内存空间的字节长度。如果要用memset清0，变量c实参要为0。
　　malloc函数和memset函数的操作语句一般如下：
　　int * p=NULL;
　　p=(int*)malloc(sizeof(int));
　　if(p==NULL)
    　　printf(“Can’t get memory!\n”);
　　memset(p,0,siezeof(int));
（2）使用malloc函数分配的堆空间在程序结束之前必须释放 
　　从堆上获得的内存空间在程序结束以后，系统不会将其自动释放，需要程序员来自己管理。一个程序结束时，必须保证所有从堆上获得的内存空间已被安全释放，否则，会导致内存泄露。
　　我们可以使用free()函数来释放内存空间，但是，free函数只是释放指针指向的内容，而该指针仍然指向原来指向的地方，此时，指针为野指针，如果此时操作该指针会导致不可预期的错误。安全做法是：在使用free函数释放指针指向的空间之后，将指针的值置为NULL。
（3）calloc函数的分配的内存也需要自行释放
　　calloc函数的功能与malloc函数的功能相似，都是从堆分配内存,它与malloc函数的一个显著不同时是，calloc函数得到的内存空间是经过初始化的，其内容全为0。　　　　　　　　　　     calloc函数适合为数组申请空间，可以将size设置为数组元素的空间长度，将n设置为数组的容量。
（4）如果要使用realloc函数分配的内存，必须使用memset函数对其内存初始化
　　realloc函数的功能比malloc函数和calloc函数的功能更为丰富，可以实现内存分配和内存释放的功能。realloc 可以对给定的指针所指的空间进行扩大或者缩小，无论是扩张或是缩小，原有内存的中内容将保持不变。当然，对于缩小，则被缩小的那一部分的内容会丢失。realloc 并不保证调整后的内存空间和原来的内存空间保持同一内存地址。相反，realloc
 返回的指针很可能指向一个新的地址。
　　所以，在代码中，我们必须将realloc返回的值，重新赋值给 p :
　　p = (int *) realloc(p, sizeof(int) *15);
　　甚至，你可以传一个空指针（0）给 realloc ，则此时realloc 作用完全相当于malloc。
　　int* p = (int *)realloc (0,sizeof(int) * 10);   //分配一个全新的内存空间，
　　这一行，作用完全等同于：
　　int* p = (int *)malloc(sizeof(int) * 10);
（5）关于alloca()函数
　　还有一个函数也值得一提，这就是alloca()。其调用序列与malloc相同，但是它是在当前函数的栈帧上分配存储空间，而不是在堆中。其优点是：当 函数返回时，自动释放它所使用的栈帧，所以不必再为释放空间而费心。其缺点是：某些系统在函数已被调用后不能增加栈帧长度，于是也就不能支持alloca 函数。尽管如此，很多软件包还是使用alloca函数，也有很多系统支持它。
分类: [C++](http://www.cnblogs.com/zqrferrari/category/245550.html)
