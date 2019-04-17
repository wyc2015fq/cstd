# 小侃#pragma - xiahouzuoxin - CSDN博客





2013年09月04日 21:49:48[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1921
个人分类：[ComputerLanuage](https://blog.csdn.net/xiahouzuoxin/article/category/1172320)









#pragma是一个编译器指令。




**================================================================**



```cpp
#pragma comment(lib, libname)
```
将libname的库添加到工程中，与VC++中Linker->Input->Additional
 dependencies的功能相同。使用#pragma comment后其它用户拿到源文件后就能直接编译，而不需要再进行工程中的库依赖设置。例子如下：




```cpp
#pragma comment(lib, "kernel32")
#pragma comment(lib, "user32")
```






**================================================================**




```cpp
#pragma code_seg( [ [ { push | pop}, ] [ identifier, ] ] [ "segment-name" [, "segment-class" ] )
```
指定函数存放在*.obj文件中的代码段，默认的代码段是.text


当code_seg()不跟参数默认将函数放在.text段；push与pop可选，push指将一个记录压入内部编译器栈，pop指将记录弹出内部编译器栈。例子如下：



```cpp
// pragma_directive_code_seg.cpp
void func1() {                  // stored in .text
}

#pragma code_seg(".my_data1")
void func2() {                  // stored in my_data1
}

#pragma code_seg(push, r1, ".my_data2")
void func3() {                  // stored in my_data2
}

#pragma code_seg(pop, r1)      // stored in my_data1
void func4() {
}

int main() {
}
```


================================================================



```cpp
#pragma data_seg( [ [ { push | pop }, ] [ identifier, ] ] [ "segment-name" [, "segment-class" ] )
```

使用例子如下：




```cpp
// pragma_directive_data_seg.cpp
int h = 1;                     // stored in .data
int i = 0;                     // stored in .bss
#pragma data_seg(".my_data1")
int j = 1;                     // stored in "my_data1"

#pragma data_seg(push, stack1, ".my_data2")   
int l = 2;                     // stored in "my_data2"

#pragma data_seg(pop, stack1)   // pop stack1 off the stack
int m = 3;                     // stored in "stack_data1"

int main() {
}
```






除了data_seg和code_seg之外，还有
#pragma bss_seg( [ [ { push | pop }, ] [ identifier, ] ] [ "segment-name" [, "segment-class" ] )#pragma const_seg( [ [ { push | pop}, ] [ identifier, ] ] [ "segment-name" [, "segment-class" ] )


等关于段的定义。

**================================================================**



```cpp
#pragma pack( [ show ] | [ push | pop ] [, identifier ] , n  )
```
编译器中提供了[#pragma](http://baike.baidu.com/view/1451188.htm) pack(n)来设定[变量](http://baike.baidu.com/view/296689.htm)以n[字节对齐](http://baike.baidu.com/view/1523557.htm)方式。n[字节对齐](http://baike.baidu.com/view/1523557.htm)就是说[变量](http://baike.baidu.com/view/296689.htm)存放的起始地址的[偏移量](http://baike.baidu.com/view/1254177.htm)有两种情况：第一、如果n大于等于该变量所占用的字节数，那么偏移量必须满足默认的对齐方式，第二、如果n小于该变量的类型所占用的字节数，那么偏移量为n的倍数，不用满足默认的对齐方式。结构的总大小也有个约束条件，分下面两种情况：如果n大于所有[成员变量](http://baike.baidu.com/view/684821.htm)类型所占用的字节数，那么结构的总大小必须为占用空间最大的变量占用的空间数的倍数；否则必须为n的倍数。n取值为1,2,4,8,16。




```cpp
#pragma pack(push) //保存对齐状态
#pragma pack(4)//设定为4字节对齐
struct test
{
char m1;
double m4;
int m3;
};
#pragma pack(pop)//恢复对齐状态
```
也可以使用__declspec(align(#))来对齐用户自定义数据（比如结构体），如下，




```cpp
__declspec(align(32)) struct Str1{
   int a, b, c, d, e;
};
```

**================================================================**




```cpp
#pragma message( messagestring )
```
编译时在标准输出控制台显示消息。





```cpp
#if _M_IX86 == 500
#pragma message( "Pentium processor build" )
#endif
```

```cpp
#pragma message( "Compiling " __FILE__ ) 
#pragma message( "Last modified on " __TIMESTAMP__ )
```

**================================================================**

```cpp
#pragma section( "section-name" [, attributes] )
```
在*.obj中创建一个新的段，段的属性（attributes）可以是：



read 可读

write 可写

execute 可执行

shared 进程共享

nopage

nocache

。。。等



```cpp
#pragma section("mysec",read,write)
int j = 0;
__declspec(allocate("mysec")) int i;
int i = 1;
```
其中j分配到了数据段，j分配到了mysec段。










