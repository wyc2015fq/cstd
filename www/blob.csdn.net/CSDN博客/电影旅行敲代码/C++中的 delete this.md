# C++中的"delete this" - 电影旅行敲代码 - CSDN博客
2015年11月06日 19:36:00[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：1359
# 引子
- C++中delete能否用于栈对象
- C++中”delete this”会出现什么问题
- C++中”delete this”的注意事项
## C++中delete能否用于栈对象
在大家的印象中delete只能用于new出动态内存对象，我们使用了g++/Clang++/MSVC来测试delete栈对象的情况。
测试代码如下：
```cpp
int main()
{
    int num = 10;
    int* p = #
    delete p;
    cout << num << endl;
    return 0;
}
```
使用g++/Clang++/MSVC进行编译都是没有问题的（肯定啊，从词法语法甚至到语义分析都没有问题）。只是在运行结果时稍有不同，g++编译运行会输出10。Clang++和MSVC编译运行时会直接down掉。经过实验室delete栈对象是未定义的行为。
## C++中”delete this”会出现什么问题
经过以上得知delete栈内存是未定义的行为，但是delete动态内存上的”this”指针呢？我们同样在3个编译平台上进行测试。测试源码如下：
```cpp
class A
{
public:
    int num;
    A() { num = 10; }
    void del() 
    { 
        void* ptr = this;
        delete this; 
        // 继续执行成员函数
        print();
    }
    void print() { cout << num << endl; }
};
int main()
{
    A* a = new A();
    a->del();
    return 0;
}
```
在g++上编译运行输出结果随机，而在Clang++上编译运行输出结果”10”，在Visual Studio上编译运行输出结果恒为”-17891602”。说明delete this操作编译通过，但是**是未定义的行为**，有的编译平台在delete this后会修改原有位置的数据，有的编译平台只是将相关内存位置设为”delete”，其实这是和编译平台所实现的运行时库相关。
**由于是未定义行为，所以delete this还是比较危险的行为，当类定义了虚函数或者有其他线程使用了这一块内存时，会导致严重的危害甚至程序崩溃**。
这里有必要说明一下不同的编译平台在delete操作后对原有内存的行为。
> - **0xABABABAB** : Used by Microsoft’s HeapAlloc() to mark “no man’s land” guard bytes after allocated heap memory
- **0xABADCAFE** : A startup to this value to initialize all free memory to catch errant pointers
- **0xBAADF00D** : Used by Microsoft’s LocalAlloc(LMEM_FIXED) to mark uninitialised allocated heap memory
- **0xBADCAB1E** : Error Code returned to the Microsoft eVC debugger when connection is severed to the debugger
- **0xBEEFCACE** : Used by Microsoft .NET as a magic number in resource files
- **0xCCCCCCCC** : Used by Microsoft’s C++ debugging runtime library to mark uninitialised stack memory
- **0xCDCDCDCD** : Used by Microsoft’s C++ debugging runtime library to mark uninitialised heap memory
- **0xDEADDEAD** : A Microsoft Windows STOP Error code used when the user manually initiates the crash.
- **0xFDFDFDFD** : Used by Microsoft’s C++ debugging heap to mark “no man’s land” guard bytes before and after allocated heap memory
- **0xFEEEFEEE** : Used by Microsoft’s HeapFree() to mark freed heap memory. —[[Visual Studio调试过程中相关字节含义]](http://stackoverflow.com/questions/127386/in-visual-studio-c-what-are-the-memory-allocation-representations)
在Visual Studio上delete后会将原有内存设置为”0xfeeefeee”，具体结果如下图所示： 
删除前this处内存内容为10，其中后面”0xfdfdfdfd”是Visual Studio在DEBUG模式下在堆上的*守卫字节*。 
![删除前](https://img-blog.csdn.net/20151106201538207)
> 
The **0xfeeefeee** bit pattern indicates that this memory is part of a deallocated memory allocation (free() or delete). If you are seeing the 0xfeeefeee bit pattern it means that you are reading memory that has been deallocated by free() or delete.—[[0xfeeefeee]](http://www.softwareverify.com/memory-bit-patterns.php)
The **0xfdfdfdfd** bit pattern is the guard pattern around memory allocations allocated with the “non-aligned” (default) allocators.Memory allocated with malloc(), realloc(), new and new [] are provided with a guard block before and after the memory allocation. When this happens with an non-aligned (default) memory allocator, the bit pattern used in the guard block is 0xfdfdfdfd.—[[0xfdfdfdfd]](http://www.softwareverify.com/memory-bit-patterns.php)
The **0xabababab** bit pattern is the bit pattern for the guard block after memory allocated using HeapAlloc(), LocalAlloc(LMEM_FIXED), GlobalAlloc(GMEM_FIXED) or CoTaskMemAlloc().If you are seeing the 0xabababab bit pattern it means that you are reading memory after a memory block that has been allocated by HeapAlloc(), LocalAlloc(LMEM_FIXED), GlobalAlloc(GMEM_FIXED) or CoTaskMemAlloc().—[[0xabababab]](http://www.softwareverify.com/memory-bit-patterns.php)
delete this后，原有内容设置为”0xfeeefeee”表示为已释放的内存。
![这里写图片描述](https://img-blog.csdn.net/20151106201553844)
在g++下删除前和删除后，this值如下图所示：
![g++](https://img-blog.csdn.net/20151106203318507)
g++也使用了类似于Visual Studio的模式对堆上分配的内存有“守卫字节”，只是g++使用的“守卫字节”是”0xabababab”和Visual Studio的守卫字节相同，表示空闲内存的字节为”0xfeeefeee”，表示已释放的字节是”0x007000c0”。
其实这些字节有一个专门的名字”**Magic debug values**“，维基百科上有详细的说明，我就不当搬运工了。
> 
Magic debug values are specific values written to memory during allocation or deallocation, so that it will later be possible to tell whether or not they have become corrupted, and to make it obvious when values taken from uninitialized memory are being used.—[[Magic debug values]](https://en.wikipedia.org/wiki/Magic_number_(programming))
## C++中”delete this”的注意事项
- 你必须100%绝对确保”this”对象是通过**new**分配的（不是通过new[]，不是placement new，不是栈上的局部对象，不是全局对象，不是另一个对象的数据成员；仅仅只是通过原始的new运算符）
- 你必须100%绝对确保调用”delete this”操作的成员函数是最后调用的成员函数
- 你必须100%绝对确保在当前函数中”delete this”后，调用的其他成员函数不会读取”this”对象。
- 你必须100%确保再也不会使用”this”指针。即使你使用this指针和其他指针比较，例如nullptr，打印this指针，转换this指针等等。
