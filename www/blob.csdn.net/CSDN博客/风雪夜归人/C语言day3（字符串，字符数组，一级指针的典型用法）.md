# C语言day3（字符串，字符数组，一级指针的典型用法） - 风雪夜归人 - CSDN博客
2017年08月30日 08:51:46[cdjccio](https://me.csdn.net/qq_34624951)阅读数：203
# 1、1级指针的典型用法
**数组**
**字符串**
```cpp
//1级指针的典型用法
//数组 int a[10]
//字符串
//1、C语言的字符串是以0结尾的字符串
//2、在C语言中没有字符串类型 通过字符数组来模拟字符串
//3、字符串的内存分配  堆 栈 全局区
//字符数组的初始化
int main01()
{
    //1、制定长度
    char buf2[100] = {'a','b','c','d'};//其余置0
//    char buf3[2] = {'a','b','c','d'};//初始化的个数大于内存
    //2、不指定长度
    char buf1[] = {'a','b','c','d'};//buf1是一个数组 不是一个以0结尾的字符串
    printf("buf2:%s\n",buf2);// abcd
    printf("buf2[88]:%d\n",buf2[88]);// 0
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 2、怎样用字符串初始化字符数组
**strlen() 字符串长度 不包括‘0’**
**sizeof() 内存块的大小**
```cpp
//怎样用字符串初始化字符数组
//strlen() 长度 不包括0
//sizeof() 内存块的大小
int main02()
{
    char buf3[] = "abcd"; // buf3作为字符数组 应该是5个字节 作为字符串 应该是4个字节
    int len = strlen(buf3);
    int size = 0;
    printf("buf3字符的长度：%d\n",len);// 4
    //buf3 作为数组 数组是一种数据类型 本质（固定大小的内存空间）
    size = sizeof(buf3);
    printf("buf3数组所占内存空间的大小：%d\n",size);// 5
    {
        char buf4[128] = "abcd";
        printf("buf4[100]:%d \n",buf4[100]);// 0
    }
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 3、通过数组下标和指针修改内存
```cpp
//通过数组下标和指针修改内存
int main()
{
    int i = 0;
    char *p = NULL;
    int size1 = 0;
    int size2 = 0;
    char buf5[128] = "abcdefg";
    size1 = strlen(buf5);
    size2 = sizeof(buf5);
    printf("buf5字符的长度：%d\n",size1);// 7
    printf("buf5数组所占内存空间的大小：%d\n",size2);// 128
    for(i = 0; i<strlen(buf5); i++)
    {
        printf("%c ",buf5[i]);//a b c d e f g 
    }
    printf("\n");
    p = buf5;//buf代表数组的首元素的地址
    for(i = 0;i<strlen(buf5); i++)
    {
        p = p + i;
//        printf("%c ",*(p+i));
        printf("%c ",*p);//a b d g why
    }
    printf("\n");
    //[]和*的推导过程  本质相同
    //buf5[i] = buf5[0+i] ==>*(buf5+i)
    for(i = 0;i<strlen(buf5); i++)
    {
        printf("%c ",*(buf5+i));//a b c d e f g 
    }
    printf("\n");
    //指针和内存首地址的区别
    {
        //buf5 = buf5 + 1; // error
        //buf5 = 0x11  // error
        //buf5是一个指针 只读的常量  buf5是一个常量指针
        //why？
        //析构内存的时候，保证内存空间安全释放
    }
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
