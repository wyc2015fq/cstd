# 字符串经典案例之一（字符串的copy） - 风雪夜归人 - CSDN博客
2017年08月31日 11:10:32[cdjccio](https://me.csdn.net/qq_34624951)阅读数：303
# 1、初级程序
```cpp
int main04()
{
    char a[] = "i am a student";
    char b[64];
    int i = 0;
    for(int i = 0; *(a+i) !='\0'; i++)
    {
        *(b+i) = *(a+i);
    }
    //'\0'并没有copy到b中
    b[i] = '\0';//重要
    printf("a:%s \n",a);
    printf("b:%s \n",b);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
# 2、接口的封装及优化
## 2.1 方法一
```cpp
//定义接口
//方法一
void  copy_str21(char *from, char *to)
{
    for(; *from != '\0'; from++, to++)
    {
        *to = *from;
    }
    *to = '\0';//重要 手工补‘\0’
    return;
}
```
## 2.2 方法二
```cpp
//方法二 优化
void copy_str22(char *from, char *to)
{
    //++的优先级更高
    for(;*from != '\0';)
          {
              *to++ = *from++;//相当于 先*to = *from 再from++ to**
          }
    *to = '\0';//重要 手工补‘\0’
    return;
}
```
## 2.3 方法三
```cpp
//方法三 优化
void copy_str23(char *from, char *to)
{
    while((*to = *from) != '\0')//字符copy后不为0
    {
        from ++;
        to ++;
    }
    //循环到最后‘\0’已经加到to里面，所以不需要手工加'\0'
}
```
## 2.4 方法四
```cpp
//方法四 优化
void copy_str24(char *from, char *to)
{
    while((*to++ = *from++) != '\0')
    {
        ;
    }
}
```
## **2.5 方法五**
```cpp
//方法五 优化
void copy_str25(char *from, char *to)
{
    while((*to++ = *from++))
    {
        ;
    }
}
```
## 2.5_、方法五的引申
```cpp
void copy_str25_error(char *from, char *to)
{
    while((*to++ = *from++))
    {
        ;
    }
    printf("from:%s \n",from);//此时from指向字符串的最后位置“\0”，所以打印不出字符串
}
```
## 2.6、方法六 辅助指针变量的重要性及错误检测
```cpp
//方法六 优化
//辅助指针变量  不轻易改变形参的值，用辅助变量把形参接过来
int copy_str26(char *from, char *to)
{
    char *tmpfrom = from;
    char *tmpto = to;
    if(tmpfrom == NULL || tmpto == NULL)
    {
        return -1;
    }
    while(*tmpto++ = *tmpfrom++);
    printf("from:%s \n",from);
}
```
## **main 函数一**
```cpp
int main()
{
    char *from = "abcdefg";
    char buf2[100];
//    copy_str21(from,buf2);
//    copy_str22(from,buf2);
//    copy_str23(from,buf2);
//    copy_str24(from,buf2);
    copy_str25(from,buf2);
    printf("from:%s \n",from);
    printf("buf2:%s \n",buf2);
    printf("Hello world!\n");
    system("pause");
    return 0;
}
```
## main函数二 初始化指针最好分配内存
```cpp
//错误案例
int main06()
{
    char *from = "abcdefg";
    char buf2[100];
    int ret = 0;
    //错误案例
//    {
//
//        //相当于*(0) = 'a' 不能修改特殊空间
//        char *myto = NULL;// 因此必须分配内存空间 没有内存就没有指针
//        copy_str21(from,myto);// error
//    }
    {
        char *myto = NULL;//必须分配内存
        ret = copy_str26(from,myto);
        if(ret != 0)
        {
            printf("func copy_str26 error:%d \n",ret);
            return ret;
        }
    }
    printf("Hello world!\n");
    system("pause");
    return ret;
}
```
## main函数三  赋值指针的重要性
```cpp
//引入辅助指针变量的重要性
int main()
{
    char *from = "abcdefg";
    char buf2[100];
    int ret = 0;
    printf("copy_str25_error begin!\n");
    //copy_str25_error(from,buf2);// 不能打印任何值
    copy_str26(from,buf2);
    printf("copy_str25_error end! \n");
    printf("Hello world!\n");
    system("pause");
    return ret;
}
```
