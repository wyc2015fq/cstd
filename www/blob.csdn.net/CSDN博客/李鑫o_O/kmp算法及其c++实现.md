# kmp算法及其c++实现 - 李鑫o_O - CSDN博客





2016年03月16日 15:12:12[hustlx](https://me.csdn.net/HUSTLX)阅读数：511








算法详解请看http://www.cnblogs.com/c-cloud/p/3224788.html

下面是我用c++实现的版本：



```cpp
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;
vector<int> makeNext(string P)
{
    int q, k;//q:模版字符串下标；k:最大前后缀长度
    int m = P.size();//模版字符串长度
    vector<int> next(m, 0);
    for (q = 1, k = 0; q < m; ++q)//for循环，从第二个字符开始，依次计算每一个字符对应的next值
    {
        while (k > 0 && P[q] != P[k])//递归的求出P[0]···P[q]的最大的相同的前后缀长度k
            k = next[k - 1];          //不理解没关系看下面的分析，这个while循环是整段代码的精髓所在，确实不好理解  
        if (P[q] == P[k])//如果相等，那么最大相同前后缀长度加1
        {
            k++;
        }
        next[q] = k;
    }
    return next;
}
void kmp(string T,string P)
{
    int n = T.size();
    int m = P.size();
    vector<int> next=makeNext(P);
    for (int i = 0, q = 0; i < n; ++i)
    {
        while (q > 0 && P[q] != T[i])
            q = next[q - 1];
        if (P[q] == T[i])
        {
            q++;
        }
        if (q == m)
        {
            printf("Pattern occurs with shift:%d\n", (i - m + 1));
            q = next[q-1]; //look for next match
        }        
    }
}

int main()
{
    string T = "ababxbababcdabdfdsabcdabdssabcdabd";
    string P = "abcdabd";
    cout << "T:" << T << std::endl;
    cout << "P:" << P << std::endl;
    kmp(T, P);
    return 0;
}
```







