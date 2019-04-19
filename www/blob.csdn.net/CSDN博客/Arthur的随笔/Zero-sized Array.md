# Zero-sized Array - Arthur的随笔 - CSDN博客
2013年06月18日 23:29:36[largetalk](https://me.csdn.net/largetalk)阅读数：2288
# Zero-sized Array[](#zero-sized-array)
在memcached源码中看到的:
/** * Structure for storing items within memcached. */typedefstruct_stritem{....../* this odd type prevents type-punning issues when we do     * the little shuffle to save space when not using CAS. */union{uint64_tcas;charend;}data[];/* if it_flags & ITEM_CAS we have 8 bytes CAS *//* then null-terminated key *//* then " flags length\r\n" (no terminating null) *//* then data with terminating \r\n (no terminating null; it's binary!) */}item;
以前没有见过，而且用sizeof看，data字段完全不占空间，网上搜了下，做点笔记。
先看个例子,
#include <stdio.h>intmain(){intarr[];return;}
这段代码用gcc是可以编译通过的，只有在加入 -pedantic 边缘选项后会警告:
$ gcc -pedantic test.c -o t
test.c: In function ‘main’:
test.c:22:9: warning: ISO C forbids zero-size array ‘arr’ [-pedantic]
[gnu](http://gcc.gnu.org/onlinedocs/gcc/Zero-Length.html) 上开头就举了一个Zero Length Array的实用例子:
structline{intlength;charcontents[];};structline*thisline=(structline*)malloc(sizeof(structline)+this_length);thisline->length=this_length;
这个技巧非常有用，contents和结构体内存上相近，读取很快，而且节省一个指针(contents是不占内存的)
因此，上文的这段代码：
union{uint64_tcas;charend;}data[];
表示定义一个成员名叫data的zero-length array, 而data的类型是union, 以后可以通过data[i].cas 或 data[i].end来获得内存中数据。
