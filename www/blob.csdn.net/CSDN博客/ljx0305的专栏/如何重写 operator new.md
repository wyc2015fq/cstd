# 如何重写 operator new  - ljx0305的专栏 - CSDN博客
2008年07月27日 22:44:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1231标签：[delete																[编译器																[class																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)
满足规则：
1、处理 0 大小的内存分配请求
2、具有一个无穷循环，用 while(true)
3、正确申请到内存，返回void*指针
4、不能申请到内存，调用 new_handler 处理，使用new_handler set_new_handler(void (*)())可以改变
5、如果new_handler==NULL,throw std::bad_alloc
全局的operator new 
void* operator new(size_t size) throw(std::bad_alloc)
类特定的 operator new,3种
1)  static void* operator new (size_t size) throw(std::bad_alloc)
2) friend void operator new (size_t size) throw (std::bad_alloc)
3) void * operator new (size_t size) throw(std::bad_alloc) //member function
placement new 则 有多余的 parameter ,size 由编译器自动赋值，其他的参数用户赋值，以下面的形式调用
new (<paraList>) Type;
operator delete
void operator delete(void* rawMemory) throw();    //normal signature at global scope
void operator delete(void* rawMemory,size_t size) throw();    //at class scope, need size_t parameter
引用:http://www.cnblogs.com/luxiaoliang/archive/2007/12/31/1021509.html
