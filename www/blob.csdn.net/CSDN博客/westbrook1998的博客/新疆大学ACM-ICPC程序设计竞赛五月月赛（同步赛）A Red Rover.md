# 新疆大学ACM-ICPC程序设计竞赛五月月赛（同步赛）A	Red Rover - westbrook1998的博客 - CSDN博客





2018年05月01日 21:17:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：111








暴力搜索匹配字符串…我居然都做不出来TAT 
![这里写图片描述](https://uploadfiles.nowcoder.com/images/20180428/304251_1524893444448_DDAA442B9766FECDF6D84C1A7F255673)

就是给一个字符串，可以把其中的一部分子字符串替换成M，然后求替换后的字符串长度加上这个替换模块的长度的最小值
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <string>
#include <iostream>
using namespace std;
int main(void){
    string s,ts,ns;
    int num,ans,t;
    cin >> s;
    int n=s.size();
    //初始化，一个都不用替换
    ans=n;
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            int len=j-i;
            //两个for循环获取子字符串
            ts=s.substr(i,len);
            //匹配字符串的个数
            num=0;
            for(int k=0;k<=n-len;k++){
                ns=s.substr(k,len);
                if(ts==ns){
                    num++;
                    //因为循环已经有一个k++了
                    k+=len-1;
                }
            }
            //原长度减去匹配模块（len）被代替成M（1）的个数（num），再加上匹配模块
            t=n-num*(len-1)+len;
            //找出最小值
            if(ans>t){
                ans=t;
            }
        }
    }
    printf("%d\n",ans);
    return 0;
}
```




