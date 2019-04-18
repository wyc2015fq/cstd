# UVA 575 Skew Binary （水） - xcw0754 - 博客园
# [UVA 575 Skew Binary （水）](https://www.cnblogs.com/xcw0754/p/4607459.html)
题意：根据这种进制的算法，例如![\begin{displaymath}10120_{skew} = 1 \times (2^5 - 1) + 0 \times (2^4-1) + 1 \tim...
...2 \times (2^2-1) + 0 \times (2^1-1)
= 31 + 0 + 7 + 6 + 0 = 44.
\end{displaymath}](http://7xjob4.com1.z0.glb.clouddn.com/14c74f51f067e79e21d6b0ac22e4abca)，给你一个左式，要求推出右式。（其实右式就是一个十进制数，根据这种进位的方法来转成特殊进制的数。）
思路：观察转换特点，有点类似于二进制，但是其在后面还减一了。比如25-1、24-1 ...21-1。如果我们不减1会怎样？如上式，从左边看起，就会多加了（25-1）*1+1*（23-1）+2*（22-1）。这就是说，我们可以先将 10120 从右往左逐个乘以21  22  23  24  25 再减去10120即可得出结果。
　　当然也可以按照上边式子老实计算。
```
#include <bits/stdc++.h>
#define LL long long
using namespace std;
int main()
{
    //freopen("input.txt", "r", stdin);
    char s[50];
    while(~scanf("%s",s))
    {
        LL ans=0, fun=2;
        int len=strlen(s);
        if(len==1&&s[0]=='0') break;
        reverse(s,s+len);
        for(int i=0; i<len; i++,fun+=fun)
            ans+=(s[i]-'0')*fun;
        for(int i=0; i<len; i++)
            ans-=s[i]-'0';
        cout<<ans<<endl;
    }
    return 0;
}
View Code
```

