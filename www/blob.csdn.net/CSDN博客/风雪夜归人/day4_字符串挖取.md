# day4_字符串挖取 - 风雪夜归人 - CSDN博客
2017年09月04日 22:12:51[cdjccio](https://me.csdn.net/qq_34624951)阅读数：104
# 题目：
有一个字符串符合以下特征（”abcdef,acccd,eeee,aaaa,e3eeeee,sssss,";）,要求写一个函数（接口），输出以下结果
1）  以逗号分割字符串，形成二维数组，并把结果传出；
2）  把二维数组行数运算结果也传出。
strchr(“aa,aa”,’,’ );
请自己定义一个接口（函数）。
要求1：能正确表达功能的要求，定义出接口（函数）；
要求2：正确实现接口（函数），并实现功能；
要求3：编写正确的测试用例。
# 分析：
特征1：在字符串中搜索子串
特征2：需要挖取子串
# 办法：
**定义两个指针同时指向字符串的起始位置，然后用函数strchr()搜索字符串，同时使用strncpy()函数将挖取的子串放在数组中。**
```cpp
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
int splitString(const char *buf1,char c,char buf2[10][30],int *count)
{
    char *p1 = buf1;
    char *p2 = buf1;
    int i = 0;
    int length = 0;
    if(buf1 == NULL)
    {
        printf("func splitString() error!");
        return -1;
    }
    while(*buf1 != '\0')
    {
        p2 = strchr(p2,c);
        if(p2 != NULL)
        {
            if(p2 - p1 > 0)
            {
                strncpy(buf2[i],p1,p2-p1);
                buf2[i][p2-p1] = '\0';
                i++;
                p1 = p2 = p2 + 1;
            }
        }
        else
        {
            break;
        }
    }
    *count = i;
    return 0;
}
int main()
{
    int ret = 0;
    char *p1 = "aaaa,bbbb,cccc,dddd,edfg,f34d,";
    char cTmp  = ',';
    char myArray[10][30];
    int ncount = 0;
    int i = 0;
    ret = splitString(p1,cTmp,myArray,&ncount);
    if(ret != 0)
    {
        printf("func splitString() error！");
        return ret;
    }
    for(i = 0; i<ncount; i++)
    {
        printf("%s \n",myArray[i]);
    }
    printf("count= %d \n",ncount);
    system("pause");
    return 0;
}
```
