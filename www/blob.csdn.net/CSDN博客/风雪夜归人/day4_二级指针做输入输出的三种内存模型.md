# day4_二级指针做输入输出的三种内存模型 - 风雪夜归人 - CSDN博客
2017年09月03日 21:30:00[cdjccio](https://me.csdn.net/qq_34624951)阅读数：129
## 一、第一种内存模型
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
//二级指针做输入的第一种内存模型
int main41()
{
    int i = 0;
    int j = 0;
    int num = 0;
    char *tmp = NULL;
    //数组的每一个元素是指针  myArray是指针数组
    char *myArray[] = {"aaaa","bbbb","cccc","1111"};
    num = sizeof(myArray)/sizeof(myArray[0]);
    printf("排序之前：\n");
    for(i = 0; i<num; i++)
    {
//        printf("%s \n",myArray[i]);
        printf("%s \n",*(myArray + i));
    }
    //排序
    //改变的是指针的指向 没有改变指针所指向的内存空间
    for(i = 0; i<num; i++)
    {
        for(j = 0; j<num; j++)
        {
            if(strcmp(myArray[i],myArray[j]) > 0)
            {
                tmp = myArray[i]; //交换的是数组元素 即指针的值
                myArray[i] = myArray[j];
                myArray[j] = tmp;
            }
        }
    }
    printf("排序之后：\n");
    for(i = 0; i<num; i++)
    {
        printf("%s \n",*(myArray + i));
    }
    system("pause");
    return 0;
}
//函数封装
void printmyArray11(char **myArray,int num)
{
    int i = 0;
     for(i = 0; i<num; i++)
    {
        //printf("%s \n",myArray[i]);
        printf("%s \n",*(myArray + i));
    }
}
//排序
    //改变的是指针的指向 没有改变指针所指向的内存空间
void sortmyArray(char **myArray,int num)
{
    int i = 0;
    int j = 0;
    char *tmp = NULL;
    for(i = 0; i<num; i++)
    {
        for(j = 0; j<num; j++)
        {
            if(strcmp(myArray[i],myArray[j]) > 0)
            {
                tmp = myArray[i]; //交换的是数组元素 即指针的值
                myArray[i] = myArray[j];
                myArray[j] = tmp;
            }
        }
    }
}
int main()
{
    int num = 0;
    //数组的每一个元素是指针  指针数组
    char *myArray[] = {"aaaa","bbbb","cccc","1111"};
    num = sizeof(myArray)/sizeof(myArray[0]);
    printf("排序之前：\n");
    printmyArray11(myArray,num);
    //排序
    sortmyArray(myArray,num);
    printf("排序之后：\n");
    printmyArray11(myArray,num);
    system("pause");
    return 0;
}
```
# ![](https://img-blog.csdn.net/20170903213442777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 二、第二种内存模型
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
//二级指针做输入的第2种内存模型
//打印 排序
//封装成函数
int main51()
{
    int i = 0;
    int j = 0;
    int num = 4;
    char mybuf[30];
    char tmpbuf[30];
    char myArray[10][30] = {"aaaa","bbbb","cccc","2222"};
    //编译器只关心有10行30列    myarray+1跳30个字节  多维数组名的本质
    //打印
    printf("排序之前：\n");
    for(i = 0; i<num; i++)
    {
        printf("%s \n",myArray[i]);
    }
    //排序
    for(i = 0; i<num; i++)
    {
        for(j = i+1; j<num; j++)
        {
            if(strcmp(myArray[i],myArray[j]) > 0)
            {
                strcpy(tmpbuf,myArray[i]);//交换的是内存块
                strcpy(myArray[i],myArray[j]);
                strcpy(myArray[j],tmpbuf);
            }
        }
    }
    printf("排序之后：\n");
    for(i = 0; i<num; i++)
    {
        printf("%s \n",myArray[i]);
    }
    system("pause");
    return 0;
}
//函数封装
//打印
void printmyArray51_error(char **myArray,int num)
{
    int i = 0;
     for(i = 0; i<num; i++)
    {
        //问题的本质是：第二种myArray + i和第一种myArray + i不一样
        //指针的步长不一样 即指针所指向的内存空间数据类型不一样
        printf("%s \n",*(myArray + i));
    }
}
void printmyArray51(char myArray[10][30],int num)
{
    int i = 0;
     for(i = 0; i<num; i++)
    {
        printf("%s \n",*(myArray + i));
    }
}
//交换的是内存块
void sortmyArray02(char myArray[10][30],int num)
{
    int i = 0;
    int j = 0;
    char tmpbuf[30];
    for(i = 0; i<num; i++)
    {
        for(j = i+1; j<num; j++)
        {
            if(strcmp(myArray[i],myArray[j]) > 0)
            {
                strcpy(tmpbuf,myArray[i]);//交换的是内存块
                strcpy(myArray[i],myArray[j]);
                strcpy(myArray[j],tmpbuf);
            }
        }
    }
}
int main()
{
    int num = 4;
    char myArray[10][30] = {"aaaa","bbbb","cccc","2222"};
    printf("排序之前：\n");
    printmyArray51(myArray,num);
    //排序
    sortmyArray02(myArray,num);
    printf("排序之后：\n");
    printmyArray51(myArray,num);
    system("pause");
    return 0;
}
```
# 三、第三种内存模型
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
//二级指针做输入的第3种内存模型
//
int main61()
{
    char **p2 = NULL;
    int i = 0;
    int j = 0;
    int num = 5;
    char *tmp = NULL;
    p2 = (char **)malloc(sizeof(char *)*num);
    for(i = 0; i<num; i++)
    {
        p2[i] = (char *)malloc(sizeof(char)*100);
        sprintf(p2[i],"%d%d%d",i+1,i+1,i+1);
    }
    printf("排序之前：\n");
    for(i = 0; i<num; i++)
    {
        printf("%s \n",p2[i]);
    }
    //排序 交换的是指针
    for(i = 0; i<num; i++)
    {
        for(j = i+1; j<num; j++)
        {
            if(strcmp(p2[i],p2[j]) < 0)
            {
                tmp = p2[i];
                p2[i] = p2[j];
                p2[j] = tmp;
            }
        }
    }
    printf("排序之后：\n");
    for(i = 0; i<num; i++)
    {
        printf("%s \n",p2[i]);
    }
    //释放内存
    for(i = 0; i<num; i++)
    {
        if(p2 != NULL)
        {
            free(p2[i]);
            p2[i] = NULL;
        }
    }
    if(p2[i] != NULL)
    {
        free(p2[i]);
    }
    system("pause");
    return 0;
}
//函数封装
//分配内存
char **getMen61(int num)
{
    char **p2 = NULL;
    int i = 0;
    p2 = (char **)malloc(sizeof(char *)*num);
    if(p2 == NULL)
    {
        return;
    }
    for(i = 0; i<num; i++)
    {
        p2[i] = (char *)malloc(sizeof(char)*100);
        sprintf(p2[i],"%d%d%d",i+1,i+1,i+1);
    }
    return p2;
}
//打印
void printmyArray61(char **myArray,int num)
{
    int i = 0;
     for(i = 0; i<num; i++)
    {
        printf("%s \n",*(myArray + i));
    }
}
//排序
void sortmyArray61(char **myArray,int num)
{
    int i = 0;
    int j = 0;
    char *tmp = NULL;
    for(i = 0; i<num; i++)
    {
        for(j = 0; j<num; j++)
        {
            if(strcmp(myArray[i],myArray[j]) > 0)
            {
                tmp = myArray[i]; //交换的是数组元素 即指针的值
                myArray[i] = myArray[j];
                myArray[j] = tmp;
            }
        }
    }
}
//释放内存
void getMen61_free(char **p2,int num)
{
    int i = 0;
    for(i = 0; i<num; i++)
    {
        if(p2 != NULL)
        {
            free(p2[i]);
            p2[i] = NULL;
        }
    }
    if(p2[i] != NULL)
```
    {
        free(p2[i]);
    }
}
int main()
{
    char **p2 = NULL;
    int num = 5;
    char tmpbuf[100];
    //分配内存
    p2 = getMen61(num);
    printf("排序之前：\n");
    printmyArray61(p2,num);
    //排序 交换的是内存
    sortmyArray61(p2,num);
    printf("排序之后：\n");
    printmyArray61(p2,num);
    //释放内存
    getMen61_free(p2,num);//p2是一个野指针
    system("pause");
    return 0;
}
```cpp
```
