# C语言中结构体 - LC900730的博客 - CSDN博客
2017年05月05日 21:17:29[lc900730](https://me.csdn.net/LC900730)阅读数：226标签：[c语言																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=c语言&t=blog)
个人分类：[c语言](https://blog.csdn.net/LC900730/article/category/6901419)
# struct
## 1.结构体定义的时候不能赋初值；
```
struct ours{
    int num=100;   //编译报错
    int num;
    char str[100];
}
```
## 2.结构体中字符串赋值方式
```
void main(){
    struct ours o1={100,"hello china"};
    struct ours o2=01;    //结构体整体直接赋值的时候，即使字符串也可以直接赋值；
    o1.str=o2.str;        //错误，字符串不能直接赋值
    //字符串拷贝的方式
    sprintf(o1.str,02.str);
    strcpy(o1.str,o2.str);
}
```
## 3.结构体中定义结构体
#### 3.1：第一种情况：在结构体中再次定义一个结构体，但是没有创建结构体的实例，再次定义结构体内部变量，会被当做母结构体成员变量
```
struct tianchao
{
    int data;
    char name[100];
    struct beijing {
        char str[100];
        int num;
    };
};
void main() {
    struct tianchao t1;
    t1.beijing;//报错，此时beijing只是一个类型，没有实体
    t1.data=100;
    t1.num=99;
    strcpy(t1.str,"goA");      
    printf("%s\n",t1.str);
}
```
#### 3.2：结构体内部定义的结构体，创建结构体变量，那么该变量会直接作为一个成员；
```cpp
struct tianchao
{
    int data;
    char name[100];
    struct beijing {
        char str[100];
        int num;
    }b1;                       //内部定义的第一种方式
    struct beijing b2;         //内部定义的第二种方式
};
void main() {
    struct tianchao t1;
    t1.data = 1000;
    t1.b1.num = 99;
    sprintf(t1.name, "goA");
    sprintf(t1.b2.str, "goB2");
    sprintf(t1.b1.str, "goB1");
    printf("%s\n", t1.b2.str);     //goB2
    getchar();
}
```
## 4.匿名结构体
```
struct {
    char name[100];
    char phone[100];
    int num;
}
```
## 5.结构体数组
#### 5.1结构体第一种定义方式
```
struct dangdang {
    char email[30];
    char name[30];
    char addr[100];
    int num;
    int bignum;
    char tel[20];
    char phone[20];
    double RMB;
};
void main(){
    struct dangdang dd[2];   //结构体数组第一种定义方式
}
```
#### 5.2结构体定义第二种形式
```
struct dangdang {
    char email[30];
    char name[30];
    char addr[100];
    int num;
    int bignum;
    char tel[20];
    char phone[20];
    double RMB;
}dd[2];     //第二种方式
```
#### 5.3结构体定义第三种形式
```
struct {
    char email[30];
    char name[30];
    char addr[100];
    int num;
    int bignum;
    char tel[20];
    char phone[20];
    double RMB;
}dd[2];     //第三种方式（匿名）
```
## 6.结构体数组初始化
```
结构体数组在内存中是连续存储的，挨个排列
<h4>6.1顺序初始化</h4>
```
```
struct dangdang {
    char email[30];
    char name[30];
    char addr[100];
    int num;
    int bignum;
    char tel[20];
    char phone[20];
    double RMB;
}dd[3]={
    {1,1.0,"1.0"},
    { 2,2.0,"2.0"},
    { 3,3.0,"3.0"}
},dd2[2]={
    {1,1.0,"1.0"},
    {2,2.0,"2.0"},
};     //整体初始化
struct dangdang {
    char email[30];
    char name[30];
    char addr[100];
    int num;
    int bignum;
    char tel[20];
    char phone[20];
    double RMB;
}dd[3]={
    1,1.0,"1.0",
    2,2.0,"2.0",
    3,3.0,"3.0"   //顺序初始化，省略一层大括号，但是必须一一对应匹配，否则报错
}
```
## 7.指针与结构体
```
struct mydata {
    int num;
    char str[100];
};
void main(){
    struct mydata md;
    struct mydata *p;
    md.num = 99;
    sprintf(md.str, "gogogo");
    p = &md;   //存放的是结构体变量在内存的起始地址，类型决定解析
    //printf("%d\n", sizeof(p));
    printf("%s\n", p->str);    //<====>(*p).str
    system("pause");
}
```
## 8.结构体指针作为函数参数
用结构体变量的成员作参数—–值传递（新建一个变量接收传递的值） 
用指向结构体变量或者数组指针做参数—-地址传递 
用结构体变量作参数—-多值传递
## 9.结构体字节对齐问题
结构体变量所占据的内存单元个数应该大于等于其内部的所有数据成员占据内存单元数目的和
```
出于效率考虑，C语言引入了字节对齐机制，一般有如下3条通用准则；
1.结构体变量大小能够被其最宽基本类型成员大小所整除；
2.结构体每个成员相对于结构体首地址的偏移量（offset）都是最宽基本成员大小的整数倍；如有需要编译器会在成员之间加上填充字节（internal  adding）;
3.结构体总大小为结构体最宽基本类型成员大小的整数倍；如有需要编译器会在最末尾一个成员之间加上填充字节（trailing padding）；
```
基本类型是指char short int float double等这样的内置数据类型，“数据宽度是指”其sizeof的大小。诸如结构体、共用体和数组等都不是基本数据类型
```
struct info{
    int num;
}
    sizeof(struct info);//4
struct info{
    char c;
    int num;
}
    sizeof(struct info);//4
struct info{
    char c;    //1, 必须能被2整除，因此此时是2
    short sh;  //2   
    char ch[9];   //9     此时是10
}
    sizeof(struct info);//14
```
