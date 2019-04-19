# typedef - LC900730的博客 - CSDN博客
2017年05月07日 10:21:14[lc900730](https://me.csdn.net/LC900730)阅读数：395标签：[typedef](https://so.csdn.net/so/search/s.do?q=typedef&t=blog)
个人分类：[c语言](https://blog.csdn.net/LC900730/article/category/6901419)
## typedef：为已经存在的类型取别名
typedef ：编译时候处理，为已有类型取别名。不能用于定义变量，仅仅适用于类型 
define：预编译时候处理，简单字符替换
#### typedef作用
- 为特定类型指定了一个同义字:如typedef int Num[100];(NUM是int [100]同义词)
typedef void (*go)();    //go是void (*)()同义词
- 只是为已经存在的类型指定了一个新类型名，而没有创造新的类型
- 用typedef声明数组类型、指针类型、结构体类型、共用体类型、枚举类型等，使得编程更加方便
- 当不同的源文件中用到同一类型的数据时，常用typedef声明一些数据类型，可以把所有的typedef名称声明单独放在一个头文件中，然后在需要用到它们的文件中用#include指令把它们包含在文件中。这样编程者就不需要在各文件中自己定义typedef名称了
- 使用typedef名称可以有利于程序中的通用与移植。有时程序会依赖于硬件特性，用typedef就便于移植
#### typedef定义步骤：
- ①按照定义变量方法先写出定义体：如int i；
- ②将变量名换成新类型名 int integer
- ③最前面加typedef  typedef int integer
- ④用新类型名定义变量函数指针 integer i，j
类型定义可嵌套：（结构体指针用法中常见）
```cpp
typedef struct info {
        char name[20];
        int num;
    }INFO;
    void main() {
        typedef INFO * pinfo;     //结构体指针的别名，也可以在上述INFO后面，*pinfo
        pinfo p1;
        p1 = (pinfo) malloc(sizeof(INFO));
        p1->num = 100;
        strcpy(p1->name, "yinc");
        printf("%s\t%d\n", p1->name, p1->num);
    }
```
