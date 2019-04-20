# C语言day2(一级指针到多级指针、指针的本质) - 风雪夜归人 - CSDN博客
2017年08月27日 22:19:21[cdjccio](https://me.csdn.net/qq_34624951)阅读数：112
个人分类：[C语言](https://blog.csdn.net/qq_34624951/article/category/7043669)
# 0、指针的注意点
1、指针变量和它指向的内存块变量是两码事。
2、指针指向某个变量，就是把某个变量地址否给指针。
3、*p间接赋值成立的3个条件：
a)2个变量（通常一个实参，一个形参）
b) 建立关系，实参取地址赋给形参指针
c)*p形参去间接修改实参的值
# 1、野指针产生的原因
```cpp
//野指针产生的原因
//指针变量和它所指向的内存空间变量是两个不同的概念
//释放了指针所指的内存空间，但是指针变量本身没有重置成NULL
//造成释放的时候 指针指向野指针
//避免方法：定义指针的时候，初始化成NULL，释放指针所指的内存空间以后，把指针重置成NULL。
int main02()
{
    char *p1 = NULL;
    p1 = (char *)malloc(100);
    if(p1== NULL)
    {
        return;
    }
    strcpy(p1,"11112222");
    printf("p1:%s\n",p1);
    if(p1 != NULL)
    {
        free(p1);//释放p1所指向的内存空间，但是p1本身的内存空间依然保存着动态分配内存空间的首地址，即野指针
        p1 = NULL;//
    }
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 2、向null copy数据
```cpp
//向null copy数据
int main03()
{
    char *p1 = NULL;
    strcpy(p1,"abcdefg");//指针指向的空间不能写入
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 3、不断地修改指针的值相当于不断改变指针的指向
```cpp
//不断地修改指针的值 相当于不断改变指针的指向
int main04()
{
    int buf[128];//可以在栈上分配内存
    int i;
    int j = 0;
    char *p2 = NULL;
    char *p1 = NULL;
    p1 = &buf[0];//不断地修改p1的值 相当于不断改变p1的指向
    p1 = &buf[1];
    p1 = &buf[2];
    for(i = 0; i<10; i++)
    {
        p1 = buf[i];
    }
    p2 = (char *)malloc(100);
    strcpy(p2,"abcdefgh11223344");
    for(i = 0; i<10; i++)
    {
        p1 = p2+i;
        printf("%c ",*p1);
    }
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 4、1级指针的技术推演
```cpp
int getFileLen(int *p)
{
    *p = 50;
    return p;
}
//return只能返回一个结果
int getFileLen2()
{
    int a = 100;
    return a;
}
//不能修改a的值
//形参的属性
int getFileLen3(int b)
{
    b = 200;
    return b;
}
int main05()
{
    int a = 10;
    int *p = NULL;
    //修改a的值
    a = 20;//直接修改
    p = &a;
    *p = 30;//p的值是a的地址，*像是一把钥匙，通过内存块，知道一块内存空间，间接修改了a的值
    printf("a:%d\n",a);
    {
        *p = 40;
        printf("a:%d\n",a);
    }
    getFileLen(&a);
    printf("getFileLen后 a:%d \n",a);
    getFileLen3(a);
    printf("getFileLen3后 a:%d \n",a);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 5、从1级指针到2级指针
```cpp
void getMem(char **p2)
 {
    *p2 = 400;//间接赋值 p2是p1的地址
//    printf("p1:%d\n",p1);
}
void getMem2(char *p2)
 {
    p2 = 800;//间接赋值 p2是p1的地址
//    printf("p1:%d\n",p1);
}
int main()
{
    char *p1 = NULL;
    char **p2 = NULL;
    p1 = 0x11;
    p2 = 0x22;
    //直接修改p1的值
    p1 = 0x111;
    //间接修改p1的值
    p2 = &p1;
    *p2 = 100;//间接赋值 p2是p1的地址
    printf("p1:%d\n",p1);
    {
        *p2 = 200;//间接赋值 p2是p1的地址
        printf("p1:%d\n",p1);//p1 = 200
    }
    getMem(&p1);
    printf("p1:%d\n",p1);//400
    getMem2(p1);//不能修改P1的值
    printf("p1:%d\n",p1);//400
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 6、指针做函数参数--C语言的精髓
> 
Note：（1）函数调用时，形参传给实参，用实参取地址，传给形参，在被调用函数里面用*p，来改变实参，把运算结果传出来。
（2）函数调用时,用n指针（形参）改变n-1指针（实参）的值：
改变0级指针（int iNum = 1）的值有2种方式
改变1级指针（eg char *p = 0x1111 ）的值，有2种方式
改变2级指针的（eg char **pp1 = 0x1111 ）的值，有2种方式
```cpp
//重点：指针做函数参数
//接口的封装和设计
//不同的函数可以操作同一块内存空间
int getMem3(char **myp1,int *mylen1,char **myp2,int *mylen2)
 {
    int ret = 0;
    char *tmp1 = NULL;
    char *tmp2 = NULL;
    tmp1 = (char *)malloc(100);
    strcpy(tmp1,"11223344");
    //间接赋值
    *mylen1 = strlen(tmp1);// 1级指针
    printf("*mylen1=%d \n",*mylen1);// 8
    *myp1 = tmp1;// 2级指针的间接赋值
    tmp2 = (char *)malloc(200);
    strcpy(tmp2,"aabbccddee");
    *mylen2 = strlen(tmp2);//1级指针的间接赋值
    *myp2 = tmp2;// 2级指针的间接赋值
    return ret;
}
int main()
{
    int ret = 0;
    char *p1 = NULL;
    int len1 = 0;
    char *p2 = NULL;
    int len2 = 0;
    ret = getMem3(&p1,&len1,&p2,&len2);
    if(ret != 0)
    {
        printf("func getMem3() error:%d \n",ret);
    }
    printf("p1:%s \n",p1);// 11223344
    printf("p2:%s \n",p2);// aabbccddee
    printf("len1:%d \n",len1);// 8
    printf("len2:%d \n",len2);// 10
    if(p1 != NULL)
    {
        free(p1);
        p1 = NULL;
    }
     if(p2 != NULL)
    {
        free(p2);
        p2 = NULL;
    }
    printf("Hello world!\n");
    system("pause");
    return ret;
}
```
# 7、间接赋值的应用场景
```cpp
int main08()
{
    // 1 2 3 这3个条件写在一个函数里
    // 12写在一起 3单独写在另一个函数里-->函数调用
    // 1   23写在一起  c++
    char from[128];
    char to[128] = {0};
    char *p1 = from;
    char *p2 = to;
    strcpy(from,"112233445566");
    while(*p1 != '\0')
    {
        *p2 = *p1;
        p2 ++;
        p1 ++;
    }
    printf("from=%s\n",from);// 112233445566
    printf("to=%s\n",to);// 112233445566
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 8、指针的输入和输出特性
```cpp
//指针的输入和输出特性
//修改1级指针用2级指针
int getMem41(char **myp1/*out*/,int *mylen1/*out*/,char **myp2/*out*/,int *mylen2/*out*/)
 {
    int ret = 0;
    char *tmp1 = NULL;
    char *tmp2 = NULL;
    tmp1 = (char *)malloc(100);
    strcpy(tmp1,"11223344");
    //间接赋值
    *mylen1 = strlen(tmp1);// 1级指针
    printf("*mylen1=%d \n",*mylen1);// 8
    *myp1 = tmp1;// 2级指针的间接赋值
    tmp2 = (char *)malloc(200);
    strcpy(tmp2,"aabbccddee");
    *mylen2 = strlen(tmp2);//1级指针的间接赋值
    *myp2 = tmp2;// 2级指针的间接赋值
    return ret;
}
char *getMem42(int num)
{
    char *tmp1 = NULL;
    tmp1 = (char *)malloc(num);
    strcpy(tmp1,"abcdffg");
    return tmp1;
}
int main09()
{
    int ret = 0;
    char *p1 = NULL;
    int len1 = 0;
    char *p2 = NULL;
    int len2 = 0;
    ret = getMem41(&p1,&len1,&p2,&len2);
    if(ret != 0)
    {
        printf("func getMem41() error:%d \n",ret);
    }
    printf("p1:%s \n",p1);// 11223344
    printf("p2:%s \n",p2);// aabbccddee
    printf("len1:%d \n",len1);// 8
    printf("len2:%d \n",len2);// 10
    if(p1 != NULL)
    {
        free(p1);
        p1 = NULL;
    }
     if(p2 != NULL)
    {
        free(p2);
        p2 = NULL;
    }
    p1 = getMem42(100);
    printf("p1:%s \n",p1);
    if(p1 != NULL)
    {
        free(p1);
        p1 = NULL;
    }
    printf("Hello world!\n");
    system("pause");
    return ret;
}
```
# 9、详解多级指针与指针类型的关系
推论①：指针变量的步长只与‘指针变量的值’的类型有关（指针的值的类型 == 指针指向数据的类型），指针类型跟指针的值有关，指针是占据4个字节大小的内存空间，但是指针的类型却是各不相同的指针的类型决定了指针指向的内存空间从哪个位置开始，从哪个位置结束。
普通变量名是一段内存空间的标识，普通变量名代表的是一段内存空间，
对于复杂变量（例如指针）:要知道某个指针的步长，必须确定指针的类型
**指针的值永远是一个十六进制数字，但是这个十六进制数字指向的内存空间却是不相同的**
int a[5]; char *arr[4]={"aa","ss","dd","ee"};对于这2个一维数组而言
a是一个指针，a指针的值是a[0]这个int型变量的地址，所以a的类型是int *；
arr是一个指针,arr指针的值是arr[0]这个char *型变量的地址，所以arr的类型是char **
有人会想a的类型是int *，那么a的步长应该是sizeof(int *) 是4；其实不对，a的步长应该是a指向的内存空间的大小
a指向的内存空间是int型的变量a[0]，a[0]的大小是4个字节，因此a的步长是sizeof(int) 是4 虽然都是4 但是意义不一样
再来讨论arr，由以上推论可得，arr的步长应该是arr指向内存空间的大小，arr指向内存空间是一个字符串的指针，
字符串的指针的大小是 sizeof(char *) = 4 个字节，所以arr的步长是4个字节
举例int a[5]={0},讨论a和&a的步长
a指向的是一个int型变量（上面已经详述）,a的步长是sizeof(int) = 4；
&a这指针指向的是一个int型的一维数组，这个int型的一维数组的大小是sizeof(int)*5 = 20;
所以&a的步长是20
V推论②：多级指针是人为为‘指针本身’做的一种区分
二级指针是指针本身类型的一种人为自定类别，但通常意义上指针的类型没有任何关系
一级指针与二级指针的区别在于，一级指针的值是某种类型的数据的地址，而二级指针的值是一个指针的地址
指针的步长只跟指针的类型有关系，跟指针是几级指针没关系
```cpp
#include<stdio.h>
#include<stdlib.h>
void main(){
    int a[5] = { 1, 2, 3, 2, 1 };
    printf("%x\n", &a);                    // 打印 76fc54
    printf("%x\n", &a + 1);                // 打印 76fc68   得出结论：&a的步长是20（特别提醒：这是十六进制）
    printf("\n-----------------\n");
    char *myarr[] = {"aaaaa","bb","c","dede"};
    printf("%x\n", myarr);                 // 打印  81fe88
    printf("%x\n", myarr + 1);             // 打印  81fe8c  得出结论：myarr的步长是4
    system("pause");
}
```
