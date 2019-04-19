# leetcode题库：5.最长回文子串Longest Palindrome string - Koma Hub - CSDN博客
2018年04月23日 11:21:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：59
个人分类：[Algorithm																[C/C++																[LeetCode](https://blog.csdn.net/Rong_Toa/article/category/7589453)](https://blog.csdn.net/Rong_Toa/article/category/7156199)](https://blog.csdn.net/Rong_Toa/article/category/7221428)
***题目描述：***
/** 题目地址：[https://leetcode-cn.com/problems/longest-palindromic-substring/description/](https://leetcode-cn.com/problems/longest-palindromic-substring/description/)
 * 题目：最长回文子串Longest Palindrome string
 * 给定一个字符串 s，找到 s 中最长的回文子串。你可以假设 s 的最大长度为1000。
 * 示例 1：输入: "babad"  输出: "bab"  注意: "aba"也是一个有效答案。
 * 示例 2：输入: "cbbd"  输出: "bb"
 */
***我的代码：***
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/**
 * 题目：最长回文子串Longest Palindrome string
 * 给定一个字符串 s，找到 s 中最长的回文子串。你可以假设 s 的最大长度为1000。
 * 示例 1：输入: "babad"  输出: "bab"  注意: "aba"也是一个有效答案。
 * 示例 2：输入: "cbbd"  输出: "bb"
 */
void get(char *s, int index, int *begin, int *end)
{
    int offset=0, len = strlen(s);
    *begin = index;
    *end = index;
    if(index==1 || index==len-1)
        return;
    while(*begin > 0)
    {
        if(s[*begin] == s[(*begin)-1])
            (*begin) --;
        else 
            break;
    }
    while(*end < len-1)
    {
        if(s[*end] == s[(*end) +1])
            (*end) ++;
        else 
            break;
    }
    while(*begin > 0 && *end < len-1)
    {
        (*begin) --;
        (*end) ++;
        if(s[*begin] == s[*end])
        {
            continue;
        }
        else 
        {
            (*begin) ++;
            (*end) --;
            return;
        }
    };
}
char* longestPalindrome(char* s) 
/* 题干给出的函数 */
{
    char *ls;
    int len = strlen(s);
    int i, begin=0, end=0;
    int count=0, cb, ce;
    char flag = s[0];
    for(i=0;i<len;i++)
    {
        if(flag == s[i])
            continue;
        else
            flag = s[i];
        get(s,i,&begin,&end);
        //printf("%d,%d\n",begin,end);
        if(count<end-begin+1)
        {
            count = end-begin+1;
            cb = begin;
            ce = end;
        }
    }
    //printf("%d,%d\n",cb,ce);
    ls = malloc(sizeof(char)*(ce-cb+1));
    for(i=cb;i<=ce;i++)
    {
        ls[i-cb] = s[i];
    }
    ls[i-cb] = '\0';
    return ls;
}
int main(int argc, char **argv)
{
    char *str;
    if(argc > 1){
        str = argv[1];
    }
    else 
    {   
        str = "abcb6";
    }
    
    printf("String: %s\n",str);
    printf("The Longest Palindrome:%s\n",longestPalindrome(str));
    return 1;
}
```
***测试输出结果：***
```
D:\test>gcc leetcode5.c
D:\test>a.exe
String: abcb6
The Longest Palindrome:bcb
D:\test>a.exe abcba
String: abcba
The Longest Palindrome:abcba
D:\test>a.exe abcbaaaaaaaa
String: abcbaaaaaaaa
The Longest Palindrome:aaaaaaaa
D:\test>a.exe abcbaaaaaaaabcb
String: abcbaaaaaaaabcb
The Longest Palindrome:bcbaaaaaaaabcb
D:\test>a.exe abcbaaabaaaa
String: abcbaaabaaaa
The Longest Palindrome:aaabaaa
```
