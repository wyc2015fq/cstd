# day5_结构体做函数参数 - 风雪夜归人 - CSDN博客
2017年09月11日 22:39:17[cdjccio](https://me.csdn.net/qq_34624951)阅读数：89
# 1、结构体做函数参数
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
typedef struct teacher
{
    char name[64];
    int age;
    int id;
}teacher;
void printteacher(teacher *array, int num)
{
    int i = 0;
    for(i = 0; i<num; i++)
    {
        printf("age：%d \n",array[i].age);
    }
}
void sortteacher(teacher *array, int num)
{
    int i, j;
    teacher tmp;
    for(i = 0; i<num; i++)
    {
        for(j = i + 1; j<num; j++)
        {
            if(array[i].age > array[j].age)
            {
                tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
}
teacher* createteacher(int num)
{
    teacher *tmp = NULL;
    tmp = (teacher *)malloc(sizeof(teacher) * num);
    if(NULL == tmp)
    {
        return NULL;
    }
    return tmp;
}
void Freeteacher(teacher *p)
{
    if(NULL != p)
    {
        free(p);
        p = NULL;
    }
}
int main0201()
{
    int i = 0;
    teacher Array[3];//在栈上分配内存
    for(i = 0; i<3; i++)
    {
        printf("输入年龄：\n");
        scanf("%d",&(Array[i].age));
    }
    //打印老师的年龄
    printteacher(Array, 3);
    sortteacher(Array,3);
    printf("排序后；\n");
    printteacher(Array, 3);
    system("pause");
    return 0;
}
int main0202()
{
    int i = 0;
//    teacher Array[3];//在栈上分配内存
    int num  = 3;
    teacher *pArray = NULL;
    pArray = createteacher(num);
    for(i = 0; i<num; i++)
    {
        printf("输入年龄：\n");
        scanf("%d",&(pArray[i].age));
    }
    //打印老师的年龄
    printteacher(pArray, num);
    sortteacher(pArray,num);
    printf("排序后；\n");
    printteacher(pArray, num);
    system("pause");
    return 0;
}
```
# 2、结构体套一级指针
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
typedef struct teacher
{
    char name[64];
    char *sname;
    int age;
    int id;
}teacher;
void printteacher(teacher *array, int num)
{
    int i = 0;
    for(i = 0; i<num; i++)
    {
        printf("age：%d \n",array[i].age);
    }
}
void sortteacher(teacher *array, int num)
{
    int i, j;
    teacher tmp;
    for(i = 0; i<num; i++)
    {
        for(j = i + 1; j<num; j++)
        {
            if(array[i].age > array[j].age)
            {
                tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
}
teacher* createteacher(int num)
{
    teacher *tmp = NULL;
    tmp = (teacher *)malloc(sizeof(teacher) * num);
    if(NULL == tmp)
    {
        return NULL;
    }
    return tmp;
}
int createteacher02(teacher **pt, int num)
{
    int i = 0;
    teacher *tmp = NULL;
    tmp = (teacher *)malloc(sizeof(teacher) * num);//teacher Array[3]
    if(NULL == tmp)
    {
        return -1;
    }
    memset(tmp,0,sizeof(teacher) * num);
    for(i = 0; i<num; i++)
    {
        tmp[i].sname = (char *)malloc(60);
    }
    *pt = tmp;//二级指针 形参间接的修改实参的值
    return 0;
}
void Freeteacher(teacher *p, int num)
{
    int i = 0;
    if(NULL == p)
    {
        return;
    }
    for(i = 0; i<num; i++)
    {
        if(NULL != p->sname)
        {
            free(p[i].sname );
        }
    }
    free(p);
}
int main0201()
{
    int i = 0;
    teacher Array[3];//在栈上分配内存
    for(i = 0; i<3; i++)
    {
        printf("输入年龄：\n");
        scanf("%d",&(Array[i].age));
    }
    //打印老师的年龄
    printteacher(Array, 3);
    sortteacher(Array,3);
    printf("排序后；\n");
    printteacher(Array, 3);
    system("pause");
    return 0;
}
int main0302()
{
    int i = 0;
//    teacher Array[3];//在栈上分配内存
    int num  = 3;
    teacher *pArray = NULL;
    createteacher02(&pArray, num);
    for(i = 0; i<num; i++)
    {
        printf("输入年龄：\n");
        scanf("%d",&(pArray[i].age));
        printf("输入姓名：\n");
        scanf("%s",pArray[i].name);//向指针所指向的内存空间写入数据
        printf("输入别名：\n");
        scanf("%s",pArray[i].sname);//sname没有分配空间
    }
    //打印老师的年龄
    printteacher(pArray, num);
    sortteacher(pArray,num);
    printf("排序后；\n");
    printteacher(pArray, num);
    Freeteacher(pArray, num);
    system("pause");
    return 0;
}
```
# 3、结构体套二级指针
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
typedef struct teacher
{
    char name[64];
    char *sname;
    char **stuname;// char student[10][30]
    int age;
    int id;
}teacher;
void printteacher(teacher *array, int num)
{
    int i = 0;
    for(i = 0; i<num; i++)
    {
        printf("age：%d \n",array[i].age);
    }
}
void sortteacher(teacher *array, int num)
{
    int i, j;
    teacher tmp;
    for(i = 0; i<num; i++)
    {
        for(j = i + 1; j<num; j++)
        {
            if(array[i].age > array[j].age)
            {
                tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
}
int createteacher02(teacher **pt, int num)
{
    int i = 0;
    int j = 0;
    teacher *tmp = NULL;
    char **p = NULL;
    tmp = (teacher *)malloc(sizeof(teacher) * num);//teacher Array[3]
    if(NULL == tmp)
    {
        return -1;
    }
    memset(tmp,0,sizeof(teacher) * num);
    for(i = 0; i<num; i++)
    {
        //malloc一级指针
        tmp[i].sname = (char *)malloc(60);
        //二级指针的第三种内存模型
        {
            //打造二维内存
            p = (char **)malloc(3 * sizeof(char *));
            for(j = 0; j<3; j++)
            {
                p[j] = (char *)malloc(120);
            }
            tmp[i].stuname = p;
        }
    }
    *pt = tmp;//二级指针 形参间接的修改实参的值
    return 0;
}
void Freeteacher(teacher *p, int num)
{
    int i = 0;
    int j = 0;
    if(NULL == p)
    {
        return;
    }
    for(i = 0; i<num; i++)
    {
        //释放一级指针
        if(NULL != p->sname)
        {
            free(p[i].sname );
        }
        //释放二级指针
        if(NULL != p[i].stuname)
        {
            char **myp = p[i].stuname;
            for(j = 0; j<3; j++)
            {
                if(NULL != myp[j])
                {
                    free(myp[j]);
                }
            }
            free(myp);
            p[i].stuname = NULL;
        }
    }
    free(p);
}
int main0201()
{
    int i = 0;
    teacher Array[3];//在栈上分配内存
    for(i = 0; i<3; i++)
    {
        printf("输入年龄：\n");
        scanf("%d",&(Array[i].age));
    }
    //打印老师的年龄
    printteacher(Array, 3);
    sortteacher(Array,3);
    printf("排序后；\n");
    printteacher(Array, 3);
    system("pause");
    return 0;
}
int main()
{
    int i = 0;
    int j = 0;
//    teacher Array[3];//在栈上分配内存
    int num  = 3;
    teacher *pArray = NULL;
    createteacher02(&pArray, num);
    for(i = 0; i<num; i++)
    {
        printf("输入年龄：\n");
        scanf("%d",&(pArray[i].age));
        printf("输入姓名：\n");
        scanf("%s",pArray[i].name);//向指针所指向的内存空间写入数据
        printf("输入别名：\n");
        scanf("%s",pArray[i].sname);//sname没有分配空间
        for(j = 0; j<3; j++)
        {
            printf("input student name:");
            scanf("%s",pArray[i].stuname[j]);
        }
    }
    //打印老师的年龄
    printteacher(pArray, num);
    sortteacher(pArray,num);
    printf("排序后；\n");
    printteacher(pArray, num);
    Freeteacher(pArray, num);
    system("pause");
    return 0;
}
```

