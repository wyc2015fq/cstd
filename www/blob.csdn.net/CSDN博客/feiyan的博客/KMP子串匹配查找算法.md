# KMP子串匹配查找算法 - feiyan的博客 - CSDN博客












2018年02月10日 12:37:44[feiyanaffection](https://me.csdn.net/feiyanaffection)阅读数：306








### 1. 子串匹配的朴素解法

（1）将当前子串与目标串进行比较，每次移动一位， 若匹配失败，则向右再移动一位，直到将目标串查找完。 

（2）图解： 
![这里写图片描述](https://img-blog.csdn.net/20180210111951576?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpeWFuYWZmZWN0aW9u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（3）代码描述： 

返回目标串出现子串的第一个下标位置。
```
int sub_str_index(const char* s, const char* p)
{
    int ret = -1;
    int slen = strlen(s);
    int plen = strlen(p);
    int len = slen - plen;

    for(int i=0; (ret < 0) && (i <= len); i++)
    {
        bool equal = true;

        for(int j=0; equal && (j < plen); j++)
        {
            equal = equal && (s[i + j] == p[j]);
        }

        ret = (equal ? i : -1);
    }

    return ret;
}
```

（4）评价： 

这种方法固然能够实现功能，但是效率过于低下。

## 2. KMP子串查找算法

（1）伟大的发现： 
![这里写图片描述](https://img-blog.csdn.net/20180210113045434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpeWFuYWZmZWN0aW9u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180210112557351?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpeWFuYWZmZWN0aW9u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180210112919601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpeWFuYWZmZWN0aW9u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（2）什么是部分匹配表？ 

要想获得部分匹配表，必须先要求得部分匹配值，那什么是部分匹配值呢？往下看。 
**前缀：**除了最后一个字符以外，一个字符串的全部头部组合 
**后缀：**除了第一个字符以外，一个字符串的全部尾部组合 
**部分匹配值：**前缀和后缀中相同的串的最大长度 
**部分匹配表(Partial Matched Table)：**由部分匹配值组合成的与索引值对应的一张表 
![这里写图片描述](https://img-blog.csdn.net/20180210114012183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpeWFuYWZmZWN0aW9u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（3）实现关键：得到部分匹配值 
![这里写图片描述](https://img-blog.csdn.net/20180210114911900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpeWFuYWZmZWN0aW9u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

设前缀、后缀交集元素的最大长度为···ll(longest length)···, 

①当前要求的ll值，通过历史的ll值推导 

②当可选ll值为0时，直接比对首尾元素. 
![这里写图片描述](https://img-blog.csdn.net/20180210120608656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpeWFuYWZmZWN0aW9u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在上面的最后一个匹配中，重叠部分的长度就是当前的ll值，即为3。PMT[3]的含义是查找3个字符时的ll值，而3个字符时的ll值对应着下标为2的情形。 

（4）代码实现得到部分匹配值：
```cpp
int *make_pmt(const char* p)   // 时间复杂度O(m)
{
    int len = strlen(p);
    int* ret = static_cast<int*>(malloc(sizeof(int) * len));

    if(ret)
    {
        int ll = 0;

        ret[0] = 0;

        for(int i=1; i<len; i++)
        {
            while((ll > 0) && (p[ll] != p[i]))
            {
                ll = ret[ll - 1];
            }

            if(p[ll] == p[i])
            {
                ll++;
            }

            ret[i] = ll;
        }
    }

    return ret;
}
```

（5）使用部分匹配表（KMP算法）： 
![这里写图片描述](https://img-blog.csdn.net/20180210121936783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpeWFuYWZmZWN0aW9u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

KMP算法代码：
```cpp
int kmp(const char* s, const char* p)   // 时间复杂度 O(m) + O(n) ==> O(m + n)
{
    int ret = -1;
    int slen = strlen(s);
    int plen = strlen(p);
    int* pmt = make_pmt(p);

    if(pmt && (0 < plen) && (plen <= slen))
    {
        for(int i=0, j=0; i<slen; i++)
        {
            while((j > 0) && (s[i] != p[j]))
            {
                j = pmt[j - 1];
            }

            if(s[i] == p[j])
            {
                j++;
            }

            if(j == plen)
            {
                ret = i + 1 - plen;
                break;
            }
        }
    }

    free(pmt);

    return ret;
}
```

（6）核心：利用部分匹配值提高算法效率







