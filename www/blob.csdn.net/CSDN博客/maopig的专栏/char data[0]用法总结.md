# char data[0]用法总结 - maopig的专栏 - CSDN博客
2012年02月08日 21:29:40[maopig](https://me.csdn.net/maopig)阅读数：15604
struct MyData 
{
    int nLen;
    char data[0];
};         开始没有理解红色部分的内容，上网搜索下，发现用处很大，记录下来。
         在结构中，data是一个数组名；但该数组没有元素；该数组的真实地址紧随结构体MyData之后，而这个地址就是结构体后面数据的地址（如果给这个结构体分配的内容大于这个结构体实际大小，后面多余的部分就是这个data的内容）；这种声明方法可以巧妙的实现C语言里的数组扩展。
         实际用时采取这样：
         struct MyData *p = (struct MyData *)malloc(sizeof(struct MyData )+strlen(str))
         这样就可以通过p->data 来操作这个str。
         示例：
#include <iostream>
using namespace std;
struct MyData 
{
    int nLen;
    char data[0];
};
int main()
{
    int nLen = 10;
    char str[10] = "123456789";
    cout << "Size of MyData: " << sizeof(MyData) << endl;
    MyData *myData = (MyData*)malloc(sizeof(MyData) + 10);
    memcpy(myData->data,  str, 10);
    cout << "myData's Data is: " << myData->data << endl;
    free(myData);
    return 0;
}
         输出：
Size of MyData: 4
myData's Data is: 123456789         
由于数组没有元素，该数组在该结构体中分配占用空间，所以sizeof(struct Mydata) = 4。
         malloc申请的是14个字节的连续空间，它返回一个指针指向这14个字节，强制转换成struct INFO的时候，前面4个字节被认为是Mydata结构，后面的部分拷贝了“123456789”的内容。
在读程序中经常会看到这样的定义char data[0]，这是一个什么样的用法，有什么好处，在哪些地方用到？
本文的主要目的就是阐明这个定义的作用，以及适用范围，这需要对指针的概念和操作系统的内存模型有一个情形的认识。
首先看一段程序：
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct _Info
{
    int i;
    char data[0];
}Info;
int main(int argc, char* argv[])
{
    printf("%d/n",sizeof(Info));
    return 0;
}
程序的执行结果是：4。整数i就占了4个字节，这表明data没有占用空间。**data是一个数组名；该数组没有元素；该数组的真实地址紧随结构体Info之后；这种声明方法可以巧妙的实现C语言里的数组扩展。**
记住上面的结构体不同于：
typedef struct _Info
{
    int i;
    char* data;
}Info;
这个结构体占用8个字节的空间，因为**指针类型**要占用4个字节的空间。
再看一个例子：
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct _Info
{
    int i;
    char data[0];
}Info;
int main(int argc, char* argv[])
{
    char buf[10] = "123456789";
    void* p = NULL;
    printf("%d/n",sizeof(Info));
    Info* info = (Info*)malloc(sizeof(Info) + 10);
    p = (void*)info->data;
    printf("addr of info is %p. addr of data is %p ./n", info, p);
    strcpy((char*)p, buf);
    printf("%s/n", (char*)p);
    return 0;
}
程序的执行结果见下图：
可知，data的地址是紧随结构体之后的。
