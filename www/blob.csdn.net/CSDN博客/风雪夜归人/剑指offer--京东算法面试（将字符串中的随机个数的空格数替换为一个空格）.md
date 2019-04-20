# 剑指offer--京东算法面试（将字符串中的随机个数的空格数替换为一个空格） - 风雪夜归人 - CSDN博客
2018年11月13日 21:56:48[cdjccio](https://me.csdn.net/qq_34624951)阅读数：69
个人分类：[剑指offer](https://blog.csdn.net/qq_34624951/article/category/7623846)
```cpp
#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;
/**
\brief: 将I       have    a family! 替换为 I have a family!.
\input: src：源字符串
\       strLen：源字符串的长度
\output:desc：目标字符串
*/
void ReserveOneEmpty(char *desc, char *src, int srcLen)
{
    char *s_desc = desc;
    char *s_src = src;
    if (desc == nullptr || src == nullptr || srcLen <= 0)
    {
        return;
    }
    // 处理开始为空格的字符串情况
    while (*s_src == ' ')
    {
        s_src ++;
    }
    while (*s_src != '\0')
    {
        if (*s_src != ' ')
        {
            *(s_desc ++) = *(s_src ++);
        }
        else
        {
            while (*s_src == ' ')
            {
                s_src ++;
            }
            *(s_desc ++) = ' ';
            *(s_desc ++) = *(s_src ++);
        }
    }
    *s_desc = '\0';
}
int main()
{
    char *src = "  I       have    a  family!";
    int srcLen = strlen(src);
    char *desc = (char *)malloc(sizeof(char) * srcLen + 1);
    ReserveOneEmpty(desc, src, srcLen);
    cout << src << endl;
    cout << desc << endl;
    return 0;
}
```
编译结果：
![](https://img-blog.csdnimg.cn/20181114094228488.png)
