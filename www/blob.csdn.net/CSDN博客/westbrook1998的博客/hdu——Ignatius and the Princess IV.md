# hdu——Ignatius and the Princess IV - westbrook1998的博客 - CSDN博客





2018年05月17日 12:24:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
“OK, you are not too bad, em… But you can never pass the next test.” feng5166 says.  

  “I will tell you an odd number N, and then N integers. There will be a special integer among them, you have to tell me which integer is the special one after I tell you all the integers.” feng5166 says.  

  “But what is the characteristic of the special integer?” Ignatius asks.  

  “The integer will appear at least (N+1)/2 times. If you can’t find the right integer, I will kill the Princess, and you will be my dinner, too. Hahahaha…..” feng5166 says.  

  Can you find the pecial integer for Ignatius?  

  Input 

  The input contains several test cases. Each test case contains two lines. The first line consists of an odd integer N(1<=N<=999999) which indicate the number of the integers feng5166 will tell our hero. The second line contains the N integers. The input is terminated by the end of file.  

  Output 

  For each test case, you have to output only one line which contains the special number you have found.  

  Sample Input 

  5 

  1 3 2 3 3 

  11 

  1 1 1 1 1 5 5 5 5 5 5 

  7 

  1 1 1 1 1 1 1 

  Sample Output 

  3 

  5 

  1
给出奇数个数，求出现次超过一半的那个数，遍历一次然后用map添加和更新出现个数

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <map>
const int MAXN=1000050;
int t[MAXN];
using namespace std;
int main(void){
    int n;
    while(~scanf("%d",&n)){
        map<int,int> m; 
        for(int i=0;i<n;i++){
            scanf("%d",&t[i]);
        }
        for(int i=0;i<n;i++){
            if(m.count(t[i])==0){
                m.insert(make_pair(t[i],1));
            }
            else{
                if(m.find(t[i])->second==n/2){
                    printf("%d\n",t[i]);
                    break;
                }
                m.find(t[i])->second++;
            }
        }
    }
    return 0;
}
```





