# Blue Jeans（POJ-3080） - Alex_McAvoy的博客 - CSDN博客





2019年01月14日 17:08:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：46
个人分类：[POJ																[字符串处理——单模式匹配(KMP)](https://blog.csdn.net/u011815404/article/category/8698890)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

The Genographic Project is a research partnership between IBM and The National Geographic Society that is analyzing DNA from hundreds of thousands of contributors to map how the Earth was populated. 

As an IBM researcher, you have been tasked with writing a program that will find commonalities amongst given snippets of DNA that can be correlated with individual survey information to identify new genetic markers. 

A DNA base sequence is noted by listing the nitrogen bases in the order in which they are found in the molecule. There are four bases: adenine (A), thymine (T), guanine (G), and cytosine (C). A 6-base DNA sequence could be represented as TAGACC. 

Given a set of DNA base sequences, determine the longest series of bases that occurs in all of the sequences.

# **Input**

Input to this problem will begin with a line containing a single integer n indicating the number of datasets. Each dataset consists of the following components:

A single positive integer m (2 <= m <= 10) indicating the number of base sequences in this dataset.

m lines each containing a single base sequence consisting of 60 bases.

# Output

For each dataset in the input, output the longest base subsequence common to all of the given base sequences. If the longest common subsequence is less than three bases in length, display the string "no significant commonalities" instead. If multiple subsequences of the same longest length exist, output only the subsequence that comes first in alphabetical order.

# Sample Input

**32GATACCAGATACCAGATACCAGATACCAGATACCAGATACCAGATACCAGATACCAGATAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA3GATACCAGATACCAGATACCAGATACCAGATACCAGATACCAGATACCAGATACCAGATAGATACTAGATACTAGATACTAGATACTAAAGGAAAGGGAAAAGGGGAAAAAGGGGGAAAAGATACCAGATACCAGATACCAGATACCAAAGGAAAGGGAAAAGGGGAAAAAGGGGGAAAA3CATCATCATCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCACATCATCATAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACATCATCATTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT**

# Sample Output

**no significant commonalitiesAGATACCATCATCAT**

————————————————————————————————————————————————

题意：给定长度为 60 的 m 个字符串，找它的最长公共子串，如果长度相同，输出字典序小的，如果找到的公共子串小于 3 ，就输出 no significant commonalities，如果找不到也输出 no significant commonalities

思路：KMP 模版题，但算一下时间复杂度 m 不大于 10，长度不大于 60，暴力枚举的话，复杂度大概是 60*60*10*（60+60），不会爆，果断暴力。

具体步骤如下
- 开一个二维数组来保存所有的字符串
- 以第一个字符串为母版，依次截取 n 个长度为 1 的子串，去检查其他的字符串是否存在该子串

	若有，则将这个子串复制到结果中；若没有，枚举下一个长度的字符串

	若结果与新的子串等长，比较两者字典序，将较小的复制到结果中
- 同样以第一个字符串为母版，依次截取 n-1 个长度为 2 的子串，重复步骤 2 的操作，更新结果
- 即每次截取长度 i ，个数 60-i 个子串，重复上述操作，不断更新结果，直到截取到长度为 n 的子串为止

在枚举过程中，可利用 substr() 函数来构造子串，用 find() 函数来判断是否存在子串

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 101
#define LL long long
using namespace std;
string str[N];
int main()
{
    int t;
    scanf("%d",&t);
    while(t--){
        int m;
        scanf("%d",&m);
        for(int i=1;i<=m;i++)
            cin>>str[i];

        string res="no significant commonalities";
        for(int len=3;len<=60;len++){//枚举长度
            for(int i=0;i<=60-len;i++){//枚举初始位置
                string temp=str[1].substr(i,len);//构造子串
                bool flag=true;
                for(int j=2;j<=m;j++){//枚举剩下的m-1个字符串
                    if(str[j].find(temp)==string::npos){//判断是否存在子串
                        flag=false;
                        break;
                    }
                }
                if(!flag)
                    continue;
                else{
                    if(res=="no significant commonalities")
                        res=temp;
                    else if(res.size()<temp.size())
                        res=temp;
                    else if(res.size()==temp.size())
                        res=min(res,temp);
                }
            }
        }
        cout<<res<<endl;

    }
    return 0;
}
```






