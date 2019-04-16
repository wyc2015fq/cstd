# KMP字符串搜索算法 - 紫芝的博客 - CSDN博客





2018年08月30日 11:17:54[紫芝](https://me.csdn.net/qq_40507857)阅读数：152








**假设现在我们面临这样一个问题：有一个文本串S，和一个模式串P，现在要查找P在S中的位置，怎么查找呢？**

**Ps:文本串S可以类比为一篇文章，P为其中某个单词，查找这个单词在文章中哪里出现过。**

# 方法1：暴力匹配

如果用暴力匹配的思路，并假设现在文本串S匹配到 i 位置，模式串P匹配到 j 位置，则有：

如果当前字符匹配成功（即S[i] == P[j]），则i++，j++，继续匹配下一个字符；

如果失配（即S[i]! = P[j]），令i = i - (j - 1)，j = 0。相当于每次匹配失败时，i 回溯，j 被置为0。

# 方法2：kmp算法

假设现在文本串S匹配到 i 位置，模式串P匹配到 j 位置

如果j = -1，或者当前字符匹配成功（即S[i] == P[j]），都令i++，j++，继续匹配下一个字符；

如果j != -1，且当前字符匹配失败（即S[i] != P[j]），则令 i 不变，j = next[j]。此举意味着失配时，模式串P相对于文本串S向右移动了j - next [j] 位。

换言之，当匹配失败时，模式串向右移动的位数为：失配字符所在位置 - 失配字符对应的next 值（next 数组的求解会在下文的详细阐述），即移动的实际位数为：j - next[j]，且此值大于等于1。

### 【next 数组】的含义：

代表当前字符之前的字符串中，有多大长度的相同前缀后缀。例如如果 next [j] = k ，代表j 之前的字符串中有最大长度为 k 的相同前缀后缀。意味着在某个字符失配时，该字符对应的next 值会告诉你下一步匹配中，模式串应该跳到哪个位置（跳到next [j] 的位置），如果next [j] 等于0或-1，则跳到模式串的开头字符，若next [j] = k 且 k > 0，代表下次匹配跳到j 之前的某个字符，而不是跳到开头，且具体跳过了k 个字符。

![](https://img-blog.csdn.net/20180830103346503?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180830103501249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

按暴力匹配是这样子滴：然而可以更快一些，直接将D和E比较 T_T

![](https://img-blog.csdn.net/20180830103701406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

当匹配失败时，j要移动的下一个位置k。存在着这样的性质：最前面的k个字符和j之前的最后k个字符是一样的。

用数学公式来表示是这样的

**P[0 ~ k-1] == P[j-k ~ j-1]**

![](https://img-blog.csdn.net/20180830104029349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```cpp
void cal_next(char *str,int *next,int len)
{
    next[0]=-1;//-1表示不存在相同的最大前缀和最大后缀
    int k=-1;
    for(int q=1;q<len;q++)
    {
        //如果下一个不同，k就变成next[k]
        while(k>-1&&str[k+1]!=str[q])
            k=next[k];//向前回溯
        //无论k取何值，next[k]都是小于k的
        if(str[k+1]==str[q])    ++k;
        next[q]=k;
    }
}
```

有了next数组，我们就可以来进行字符串的快速匹配啦

```cpp
int KMP(char *str,int slen,char *ptr,int plen)
{
    int *next=new int[plen];
    cal_next(ptr,next,plen);//计算next数组
    int j=-1;
    for(int i=0;i<slen;i++)
    {
        while(j>-1&&ptr[j+1]!=str[i]){
        //ptr和str不匹配，且j>-1,表示ptr和str有部分匹配
        j=next[j];//往前回溯
        }
        if(ptr[j+1]==str[i])
            j=j+1;
        if(j==plen-1)
        //j移动到ptr的末端
        return i-plen+1;
    }
    return -1;
}
```





