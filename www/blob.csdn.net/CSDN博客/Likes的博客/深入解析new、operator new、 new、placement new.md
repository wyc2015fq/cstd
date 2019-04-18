# 深入解析new、operator new、::new、placement new - Likes的博客 - CSDN博客
2018年08月02日 10:10:56[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：335
## 1. new分为两步
**a、内存分配**
       　　　　调用相应的operator new( )分配内存 
**b、构造函数**
        　　　　调用相应的构造函数　　
　　实验代码：
```cpp
class T{  
    public:  
        T(){  
            cout << "构造函数。" << endl;  
        }  
        ~T(){  
            cout << "析构函数。" << endl;  
        }  
        void * operator new(size_t sz){  
            T * t = (T*)malloc(sizeof(T));  //operator new就是简单的分配内存即可
            cout << "内存分配。" << endl;  
            return t;  
        }  
        void operator delete(void *p){  
            free(p);  
            cout << "内存释放。" << endl;  
            return;  
        }  
    };  
    int main()  
    {  
        T * t = new T(); // 先 内存分配 ，再 构造函数  
        delete t; // 先 析构函数， 再 内存释放  
        return 0;  
    }
```
　　输出： 
![这里写图片描述](https://img-blog.csdn.net/20180802100646565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NvbmdjaHV3YW5nMTg2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 2、new的内部实现机制
　　我们都知道new分为两步完成，但是new是怎样完成这两步的呢？ 
开始我以为是在new内部封装了一个两个函数，一个用来分配内存，另一个用来调用相应的构造函数。
　　但事实上并非如此，根本就没有一个“隐藏”的new函数，编译器会自动将new改写成一个内存分配函数，一个构造函数。 
这里其实还有一个更本质的问题需要说明：new的本质是关键字，编译器在背后的操作可能与我们的想象大不相同。能够重载的是函数，是运算符，关键字是不能重载的（有的博客中说operator new可以重载，new不能重载就是这个原因，更本质的原因可以概括为不能重载关键字），这也是为什么运算符重载前面必须要有operator的原因。+(int a)，new(int a)都是错的。 
简而言之，new在编译器的操作下会变成两部分代码，一部分分配内存，一部分调用构造函数。
　　这其实是可以做实验验证的：  
　　就在上述代码中加入new T并打上断点，再F10一步一步地调试，发现先进入类中的operator new，执行完后并没有到new T的下一行，而是依然停留在new这一行，继续F10进入构造函数。就是说new T这一行代码停留了两次。这意味着编译器确实将new T这一行代码翻译成了两次函数调用。
　　第二种验证方式就是直接使用反汇编查看代码（推荐）： 
![这里写图片描述](https://img-blog.csdn.net/20180802100715188?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NvbmdjaHV3YW5nMTg2OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
　　可以看到的确有两个call，第一个call T::operator new( )。第二个call T::T( )。注意：真正的汇编中没有具体的函数名，只有相应的地址。显示函数名是VS的一个比较便利的功能。
## 3、::new与new
　　在全局命名空间中有一个自带的、隐藏的operator new专门用来分配内存。默认情况下编译器会将new这个关键字翻译成这个operator new和相应的构造函数。
　　但在有的情况下，用户自己会在类中重载operator new，这种情况下，编译器默认会使用类中重载的operator new（本质上因为编译器会从命名空间由内而外查找自己想要的函数，选用第一个）。
　　如果我们想要继续使用默认的operator new，就应该写成::new 字面意思就是调用最外层命名空间中的operator new
　值得一提的是最外层的（也是默认的）operator new也是可以被重载的。通过这种方式我们可以改变所有new的部分行为。
## 4、placement new
　　new分为两步走，先分配内存，后调用构造函数。我们自然而然可以想到，可不可以保持一块内存，反复构造析构，这样可以省略中间的多次分配内存。由于malloc内存会导致系统调用（malloc()之后，内核发生了什么），这可以节省大量的系统开销。C++标准也是这么想的，所以他们提供了placement new。 
    使用示例：
```cpp
char* ptr = new char[sizeof(T)]; // allocate memory  
    T* tptr = new(ptr) T;            // construct in allocated storage ("place")  
    tptr->~T();                      // destruct  
    delete[] ptr;                    // deallocate memory
```
