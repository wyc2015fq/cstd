# 时序分析：串匹配—Brute-Force算法 - wishchinYang的专栏 - CSDN博客
2015年12月17日 16:48:10[wishchin](https://me.csdn.net/wishchin)阅读数：796
 在使用KMP算法之前，使用了BF算法用于串匹配：原文链接已无法查找.....![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)
       设有主串s和子串t，子串t的定位就是要在主串s中找到一个与子串t相等的子串。通常把主串s称为目标串，把子串t称为模式串，因此定位也称作模式匹配。模式匹配成功是指在目标串s中找到一个模式串t；不成功则指目标串s中不存在模式串t。
以下算法假设串采用顺序存储结构，即：
```cpp
#define MAXSIZE 50
typedef struct
{
        char data[MAXSIZE];
        int length;
    }SqString;
```
**Brute-Force算法**
    Brute-Force算法简称为BF算法，亦称为简单匹配算法，其基本思路是：从目标串s的第一个字符开始和模式串t中的第一个字符比较，若相等，则继续逐个比较后续的字符；否则从目标串s的第二个字符开始重新与模式串t的第一个字符进行比较。以此类推，若从模式串t的第i个字符开始，每个字符依次和目标串s中的对应字符相等，则匹配成功，该算法返回i；否则，匹配失败，算法返回-1 。
该算法C实现代码如下：
```cpp
int BFIndex(SqString *sp, SqString *tp)
{
    int i, j;
        if(sp->length >= tp->length)
        {
            for(i = 0; i < sp->length; i++)
            {
                for(j = 0; j < tp->length && (i+j) < sp->length && sp->data[i+j] == tp->data[j]; j++);
                if(j == tp->length) return(i);
            }
        }
        return(-1);
    }
```
**该算法的时间复杂度分析：**
       假设目标串s的长度为m，模式串t的长度为n。第一个for循环的语句频度为m，第二个for循环的语句频度为n，故该算法的时间复杂度为O(mn)，当然这是最坏的情况。该算法在最好情况下的时间复杂度为O(n)。
       该算法比较简单，易于理解，但效率不高，主要原因是：主串指针在若干字符序列比较相等后，若有一个字符比较不相等，需要回溯（主串指针的变化 i -> i+j -> i，回溯体现在i+j -> i这个过程，因为主串指针在第一个for循环每次执行i++时，都由i+j变为i，当然i+j >= i）。
例程：
```cpp
/*
    * file: Brute-Force.c
    * author: Jesse
    * date: 2011/08/07 13:15
    */
#include <stdio.h>
#define MAXSIZE 50
typedef struct
    {
        char data[MAXSIZE];
        int length;
    }SqString;
    int BFIndex(SqString *sp, SqString *tp)
    {
        int i, j;
        if(sp->length >= tp->length)
        {
            for(i = 0; i < sp->length; i++)
            {
                for(j = 0; j < tp->length && (i+j) < sp->length && sp->data[i+j] == tp->data[j]; j++);
                if(j == tp->length) return(i);
            }
        }
        return(-1);
    }
int main(void)
{
        SqString s, t;
        int index;
        printf("\n请输入目标串s和它的长度，以空格隔开，以回车键结束整个输入：\n");
        scanf("%s %d", s.data, &s.length);
        printf("请输入模式串t和它的长度，以空格隔开，以回车键结束整个输入：\n");
        scanf("%s %d", t.data, &t.length);
        index = BFIndex(&s, &t);
        if(-1 == index) printf("\n匹配失败！\n");
        else printf("\n匹配成功！ i = %d\n", index);
        return(0);
    }
```
