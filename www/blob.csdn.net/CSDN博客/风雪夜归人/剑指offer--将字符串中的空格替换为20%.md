# 剑指offer--将字符串中的空格替换为20% - 风雪夜归人 - CSDN博客
2018年08月01日 22:54:48[cdjccio](https://me.csdn.net/qq_34624951)阅读数：35
```
#include <iostream>
#include <windows.h>
#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <deque>
#include <set>
#include <algorithm>
#include <functional>
using namespace std;
// 将字符串中的空格替换成20%
// 从后往前替换，遇到空格就替换为20%，同时指向替换后字符串的指针向前移动3个位置，
// 指向替换前字符串的指针向前移动1个位置
// 此方法的好处是每个字符只需要移动一次位置
void ReplaceBlabk(char *str, int len)
{
    char *p1 = NULL;
    int originlen = 0;
    int newlen = 0;
    int blank = 0;
    p1 = str;
    while(*p1 != '\0')
    {
        if(*p1 == ' ')
        {
            ++ originlen;
            ++ blank;
        }
        else
        {
            ++ originlen;
        }
        p1 ++;
    }
    newlen = originlen + 2*blank;
    // 判断是否为修改后的字符串预留了足够的内存
    if(newlen > len)
    {
        return;
    }
    int indexoforigin = originlen;
    int indexofnew = newlen;
    //while(indexoforigin <= indexofnew && indexoforigin >= 0)
    while(indexoforigin != indexofnew) // 当移动到第一个空格之前一个字符时，不再复制，节省运行时间
    {
        if(str[indexoforigin] == ' ')
        {
            str[indexofnew --] = '%';
            str[indexofnew --] = '0';
            str[indexofnew --] = '2';
            indexoforigin --;
        }
        else
        {
            str[indexofnew --] = str[indexoforigin --];
        }
    }
}
int main()
{
    char str[100] = "we are happy.";
    ReplaceBlabk(str, 100);
    cout << str << endl;
    //cout << "Hello world!" << endl;
    system("pause");
    return 0;
}
```
