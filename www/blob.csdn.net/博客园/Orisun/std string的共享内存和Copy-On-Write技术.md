# std::string的共享内存和Copy-On-Write技术 - Orisun - 博客园







# [std::string的共享内存和Copy-On-Write技术](https://www.cnblogs.com/zhangchaoyang/articles/1914607.html)





简单来说，在复制一个对象时并不是真的在内存中把原来对象的数据复制一份到另外一个地址，而是在新对象的内存映射表中指向同原对象相同的位置，并且把那块内存的 Copy-On-Write 位设为 1。在对这个对象执行读操作的时候，内存数据没有变动，直接执行就可以。在写的时候，才真正将原始对象复制一份到新的地址，修改新对象的内存映射表到这个新的位置，然后往这里写。
这个技术需要跟虚拟内存和分页同时**使用**，其好处是在复制对象的时候因为并不是真的复制，而只是建了一个“指针”，因而大大提高性能。但这并不是一直成立的，前提是在复制新对象之后，进行的写操作只是在一小部分的内存分页上，大部分分页不会用到或者只是读取。不然会产生大量的分页错误，得不偿失。
但对于 fork 和 exec，这又是另外一回事。因为 fork 之后一般会跟一个 exec，将进程空间完全替换。因此在 fork 上采用 Copy-On-Write，会省去一次不必要的进程空间复制。

简单地说明一下string类内存分配的概念。通常，string类中必有一个私有成员，其是一个char*，用户记录从堆上分配内存的地址，其在构造时分配内存，在析构时释放内存。因为是从堆上分配内存，所以string类在维护这块内存上是格外小心的，string类在返回这块内存地址时，只返回const char*，也就是只读的，如果你要写，你只能通过string提供的方法进行数据的改写。

#include<iostream>
#include<string>
#include<cstdio>
using namespace std;

main()
{
       string str1 = "hello world";
       string str2 = str1;
       string str3 = str2;
 
       printf ("Sharing the memory:\n");
       printf ("\tstr1's address: %x\n", (unsigned int)str1.c_str() );
       printf ("\tstr2's address: %x\n", (unsigned int)str2.c_str() );
       printf ("\tstr3's address: %x\n", (unsigned int)str3.c_str() );
 
       str3[1]='a';
       str2[1]='w';
       str1[1]='q';
 
       printf ("After Copy-On-Write:\n");
       printf ("\tstr1's address: %x\n", (unsigned int)str1.c_str() );
       printf ("\tstr2's address: %x\n", (unsigned int)str2.c_str() );
       printf ("\tstr3's address: %x\n", (unsigned int)str3.c_str() );
 
       return 0;
}


Sharing the memory:
	　　str1's address: 83f9014
	　　str2's address: 83f9014
	　　str3's address: 83f9014
After Copy-On-Write:
	　　str1's address: 83f9014
	　　str2's address: 83f9054
	　　str3's address: 83f9034

开始时str1、str2、str3共享同一块内存，所以它们的首地址是一样的，这块内存标记为由str1、str2、str3共享（有点类似于链接文件）。

当更改str3时，它不能在原先的内存里直接更改，因为这里还存放着str2和str1，于是要单独给str3开辟新的内存。此时原先的内存标记为由str1、str2共享。

更改str2时同理，要单独为str2开辟新的内存，此时原先的内存标记为由str1独享。

更改str1时终于可以在原先的内存空间上直接更改了，因为没有人跟它共享空间了。












