# 【算法题】字符串匹配算法KMP - xiaxzhou的博客 - CSDN博客





2017年09月06日 09:19:26[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：294








[详解见博客](http://blog.csdn.net/v_july_v/article/details/7041827)

KMP算法

```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace::std;

vector<int> GetNext(string str)
{
    vector<int> next(str.size());
    //next数组含义：如果str[i]不匹配，在str[i]之前：next[i]个后缀与 next[i]个前缀是相同的，(前缀和后缀是不能等于字符串本身的)
    //所以应该跳转到（str[next[i]]）处,这样可以将next[i]个前缀与后缀完全重合，继续比较
    //next[i]存的是 str[i] 的第一长的相同前缀后缀的长度，
    //next[next[i]]存的是 str[i] 的第二长的相同前缀后缀的长度...依次类推
    if (str.empty())
        return next;

    int j = 0;
    int k = -1;
    next[j] = k;
    //str[j]为后缀最后一位，str[k]为前缀最后一位
    //如果str[0]不匹配，str[0]前：有0个后缀与0个前缀相同，所以应该跳转到（0）处
    //但是str[0]==str[0]，所以next[0]存放-1，当调整到str[-1]时，意味着需要整体右移一位
    while (j<str.size()-1)
    {
        if (k == -1||str[j]==str[k])
        {
            ++k;//最长前缀后缀加1：k+1
            ++j;//存到next数组的j+1处
            next[j] = k;
        }
        else
        {
            k = next[k];
        }
    }

    //去除无效跳转,如果当前字符不匹配，且跳转后的字符和当前字符一样，则调整后肯定还是不匹配，需要继续跳转
    for (auto i = 0; i < next.size();++i)
    {
        int k = next[i];
        while (k != -1 && str[i] == str[k])
        {
            k = next[k];
        }
        next[i] = k;
    }
    return next;
}

int KMPSearch(string str, string pstr)
{
    vector<int> next = GetNext(pstr);

    int i = 0, j = 0;

    int slen = str.size();
    int plen = pstr.size();
    while (i < slen&&j < plen)
    {
        if (j == -1 || str[i] == pstr[j])
        {
            ++i;
            ++j;
        }
        else
        {
            j = next[j];
        }
    }
    if (j == pstr.size())
    {
        return i - j;
    }
    else
    {
        return -1;
    }
}

int main()
{
    string pstr = "abab";
    string str = "asdfafasfaababfasgdga";
    cout<<KMPSearch(str, pstr)<<endl;

    return 0;
}
```



