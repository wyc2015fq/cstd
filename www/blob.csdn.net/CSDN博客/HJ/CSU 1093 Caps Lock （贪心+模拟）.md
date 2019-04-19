# CSU  1093 Caps Lock  （贪心+模拟） - HJ - CSDN博客
2017年04月28日 13:26:42[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：353
个人分类：[====ACM====																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
GBQC国的小明当上了打字员，现在他需要打出N篇只含字母的文章，小明习惯用Caps Lock键来转换大小写，因此每当从小写字母转换成大写字母的时候，他就会按一下Caps Lock键打开大写锁，每当从大写字母转换成小写字母的时候，他就会按一下Caps Lock键关闭大写锁。
在最初的时候大写锁是关闭的，小明的打文章的顺序可以是任意的，但对于任意一篇文章必须从文章开头至结尾逐一打完每个字母，中途不能更换其他文章。
现在小明想知道，如何安排打文章的顺序才能使按Caps Lock键的次数最少呢？
Input
输入包含多组测试数据。
对于每组测试数据，第一行包含一个整数N(1<=N<=10^3)，表示小明一共要打N篇文章，接下来一共有N行，每行均包含1-100个大写或小写字母，分别描述了这N篇文章。
Output
对于每组测试数据，用一行输出一个整数，表示小明打完这N篇文章至少按Caps Lock键多少次。
Sample Input
1 
ABCde
1 
fgHIJ
2 
ABCde 
fgHIJ
Sample Output
2 
1 
2
Hint
```
如果使用ctype.h下的函数请注意服务器上isupper()、islower()等函数尽管条件为假时会返回0，但条件为真时返回值不一定为1。
由于数据量较大，推荐使用scanf和printf。
```
解题思路： 
主要弄明白几点就行了： 
（1）统计好大写开始小写结尾的次数以及小写开始大写结尾的次数并且加以大小比较
（2）考虑一下全部为大写开始大写结尾的特殊情况
（3）考虑一下字符串只有一个字符的情况（实际上不需要考虑）
（4）最后在每个字符串内模拟一遍统计次数就行了
```
#include<stdio.h>
#include<string.h>
int main()
{
    int N;
    char a[105];
    while(scanf("%d",&N)!=EOF)
    {
        int count=0,k1=0,k2=0,j1=0,j2=0;
        getchar();
        while(N--)
        {
            scanf("%s",a);
            int len=0;      //记录字符串长度   
            for(int i=0;a[i]!='\0';i++)
            {
                if(a[i-1]>='a'&&a[i-1]<='z')
                  if(a[i]<='Z'&&a[i]>='A')
                    count++;
                if(a[i-1]>='A'&&a[i-1]<='Z')
                  if(a[i]<='z'&&a[i]>='a')
                    count++;
                len++; 
            }
            if(len>1)      //如果字符串大于1 
            {
                if(a[0]>='a')
                {
                    if(a[len-1]>='a')   
                       k1++;             //小写开始小写结尾的次数 
                    if(a[len-1]<='Z')
                       k2++;           //小写开始大写结尾的次数 
                }
                if(a[0]<='Z')
                {
                    if(a[len-1]>='a')   
                      j1++;           //大写开始小写结尾的次数 
                    if(a[len-1]<='Z')
                      j2++;           //大写开始大写结尾的次数 
                } 
            } 
        }
        int t=0;
        if(k2==0)      //不存在小写开始大写结尾的情况 
          if(j1==0&&j2!=0)     //不存在大写开始小写结尾的情况并且一直是大写开始大写结尾的情况 
            t=1;       //因为开始的时候是小写，如果一直是大写的话就需要累加1 
        if(j1>k2)      //大写开始小写结尾的次数大于小写开始大写结尾的次数 
          count=count+j1-k2;
        if(j1<k2)       //小写开始大写结尾的次数大于大写开始小写结尾的次数 
          count=count+k2-j1-1;    //因为刚开始的时候是小写，因此需要减1 
        printf("%d\n",count+t);
    } 
    return 0;
}
```
