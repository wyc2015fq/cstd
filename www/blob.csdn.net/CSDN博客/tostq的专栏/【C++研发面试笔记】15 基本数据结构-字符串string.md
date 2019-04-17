# 【C++研发面试笔记】15. 基本数据结构-字符串string - tostq的专栏 - CSDN博客





2016年10月03日 19:12:28[tostq](https://me.csdn.net/tostq)阅读数：841
所属专栏：[C++研发面试笔记](https://blog.csdn.net/column/details/15031.html)









## 【C++研发面试笔记】15. 基本数据结构-字符串string

字符串是一类非常常见的数据结构，本篇主要介绍在STL中的string容器和一些标准库中常用的相关函数。

#### **15.1 STL的string**

`#include <string>`

在标准 C++ 中，字符串类 string 由 C++ STL 实现，提供丰富的字符串的处理功能。string 是一个基于字符的序列容器，具有vector向量容器一样的内部线性结构，字符逐一写入容器，最后以 null 字符结束。

#### 15.1.1 创建对象
- string()：默认的构造函数，用于创建一个不包含任何非空字符的 string 对象。
- `string(const string& s,  size_type pos = 0, size_type n=npos)`

拷贝构造函数，将 string 对象 s 的 [pos, pos+n) 中的字符拷贝到新创建的 string 对象空间中。
- string(const char*) 

将 char* 数组的字符拷贝到新创建的 string 对象空间中。

#### **15.1.2 字符的添加**
- `void push_back(char c)` 在string对象的字符串尾部添加一个c
- `string& append(const char* s)` 在string对象的字符串尾部添加字符串
- `string& append(const string& s)` 在string对象的字符串尾部添加字符串
- `iterator insert(iterator pos, const char& c)`在string对象的pos位置之前，插入一个字符c

string 对象具有上面几个常用的添加字符或字符串的函数，还提供了与这些添加函数相对应的，更方便的字符串 “+” 和 “+=” 操作。

#### **15.1.3 字符的遍历访问**
- string 字符串提供了数组操作符 “[]” ，用于访问指定索引位置处的字符。
- 一般的字符遍历则是使用前向迭代器和反向迭代器来进行。此时，通常要用 begin/rbegin 和 end/rend 函数找出遍历开始的首字符和末字符，然后通过迭代器的 “++” 和 “*” 操作，读取字符串中的字符。

#### **15.1.4 字符的删除**
- `iterator erase(iterator pos)` 删除字符串中position所指的字符
- `iterator erase(iterator first, iterator last)` 删除字符串中迭代器区间[first, last)上的所有字符
- `string& erase(size_type pos=0, size_type n=npos)` 删除字符串中从索引位置 pos 开始的n个字符。
- `void clear()` 删除字符串中的所有字符

#### **15.1.5 字符的替换**
- `string&  replace(size_type pos, size_type n, const char* s)` 将当前字符串从 pos索引位置开始的n个字符，替换为字符串s
- `string&  replace(size_type pos, size_type n, size_type n1, char c)`将当前字符串从pos索引位置开始的n个字符，替换为n1个字符c 
- `string&  replace(iterator first, iterator last, const char* s)`将当前字符串的[first, last)区间中的字符替换为字符串s

#### **15.1.6 字符的搜索**
- `size_type  find(const char* s, size_type pos=0)`在当前字符串pos索引位置开始，查找子串s，返回找到的位置索引， -1表示查找不到子串。
- `size_type  find(char c, size_type pos=0)` 在当前字符串的pos索引位置开始，查找字符串c的索引位置，返回-1表示查找不到字符c。

#### **15.1.7 字符串的比较**
- `int compare(const string& s)`   将当前字符串与字符串s比较，相等返回0，大于s 返回1，小于s返回-1
- `int compare(size_type pos, size_type n, const string& s)`将当前字符串从 pos 索引位置开始的 n 个字符构成的字符串与字符串 s 比较
- `int compare(const char* s)` 直接将当前字符串与字符串 s 比较。

#### **15.1.8 其他常用函数**
- `size_type  length()`    字符串的非空字符个数，即字符串的长度
- `size_type  size()`    返回字符串的长度
- `bool  empty()`   字符串是否为空，空则返回 1，否则返回 0
- `const  char* c_str()`    将string 对象转换为c字符数组。
- `string substr(int pos = 0,int n = npos)` 返回pos开始的n个字符组成的字符串
- `void swap(string &s2);`   交换当前字符串与s2的值

### 15.2 C标准的字符串操作函数

`#include <string.h> 和 #include <stdio.h>`
- **复制：**`char *strcpy(char* dest, const char *src);`

把从src地址开始且含有NULL结束符的字符串复制到以dest开始的地址空间 

src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。- **查找：**`char *strstr(char *str1, const char *str2);`

用于判断字符串str2是否是str1的子串。如果是，则该函数返回str2在str1中首次出现的地址。

### **15.3 C++ STL的函数**
- `string to_string (int val);`

将数字转换成字符串
- `int stoi (const string&  str, size_t* idx = 0, int base = 10);`

将字符串转换成整型，base指的是进制，10表示10进制，其他类似函数： 
`long stol (const string&  str, size_t* idx = 0, int base = 10);`
`unsigned long stoul (const string&  str, size_t* idx = 0, int`base = 10); 
`float stof (const string&  str, size_t* idx = 0);`
`double stod (const string&  str, size_t* idx = 0);`
> 
这篇博文是个人的学习笔记，内容许多来源于网络（包括CSDN、博客园及百度百科等），博主主要做了微不足道的整理工作。由于在做笔记的时候没有注明来源，所以如果有作者看到上述文字中有自己的原创内容，请私信本人修改或注明来源，非常感谢>_<






