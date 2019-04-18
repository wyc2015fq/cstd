# HDU 5284 wyh2000 and a string problem（字符串，水） - xcw0754 - 博客园
# [HDU 5284 wyh2000 and a string problem（字符串，水）](https://www.cnblogs.com/xcw0754/p/4657745.html)
题意：比如给你一个串，要求判断wyh是不是它的子序列，那么你只需要找一个w，找一个y，再找一个h，使得w在y前面，y在h前面即可。有一天小学生拿着一个串问他“wyh是不是这个串的子序列?”.但是wyh2000有重度近视眼，如果字符串中有一段连续的v(至少两个)，那么他会把它看成一个w。例如，字符串vvv会被看成w，字符串vvwvvv会被看成www，字符串vwvv会被看成vww。请问wyh2000会怎么回答这个问题?输出yes和no。
思路：其实就是将两个以上的v换成w，再来看是否能有'wyh'子串的存在。可以写更技巧些，但是为了快，就随便写了。还好过了。
```
//#include <bits/stdc++.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#define LL long long
#define pii pair<int,int>
#define INF 0x7f7f7f7f
using namespace std;
const int N=4000000;
char s[N];
char ans[N];
int main()
{
    //freopen("input.txt", "r", stdin);
    int t;
    cin>>t;
    while(t--)
    {
        scanf("%s",s);
        int len=strlen(s);
        int p=0;
        ans[++p]=s[0];
        for(int i=1; i<len; i++)//替换掉
        {
            if(ans[p]=='v'&&s[i]=='v')
            {
                ans[p]='w';
                while(s[i]=='v')    i++;
                i--;
            }
            else    ans[++p]=s[i];
        }
        ans[++p]='\0';
        len=p;
        int i=1;
        bool w=false, y=false, h=false;
        while(i<len&&ans[i]!='w')   i++;//找3个字符出来
        if(i<len&&ans[i]=='w')  w=1;
        while(i<len&&ans[i]!='y')   i++;
        if(i<len&&ans[i]=='y')  y=1;
        while(i<len&&ans[i]!='h')   i++;
        if(i<len&&ans[i]=='h')  h=1;
        if(w&&y&&h)    puts("Yes");
        else puts("No");
    }
    return 0;
}
AC代码
```

