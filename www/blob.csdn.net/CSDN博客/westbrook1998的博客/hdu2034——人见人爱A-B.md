# hdu2034——人见人爱A-B - westbrook1998的博客 - CSDN博客





2018年07月18日 19:20:01[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：51








> 
参加过上个月月赛的同学一定还记得其中的一个最简单的题目，就是{A}+{B}，那个题目求的是两个集合的并集，今天我们这个A-B求的是两个集合的差，就是做集合的减法运算。（当然，大家都知道集合的定义，就是同一个集合中不会有两个相同的元素，这里还是提醒大家一下）  

  呵呵，很简单吧？ 

  Input 

  每组输入数据占1行,每行数据的开始是2个整数n(0<=n<=100)和m(0<=m<=100),分别表示集合A和集合B的元素个数，然后紧跟着n+m个元素，前面n个元素属于集合A，其余的属于集合B. 每个元素为不超出int范围的整数,元素之间有一个空格隔开.  

  如果n=0并且m=0表示输入的结束，不做处理。 

  Output 

  针对每组数据输出一行数据,表示A-B的结果,如果结果为空集合，则输出“NULL”,否则从小到大输出结果,为了简化问题，每个元素后面跟一个空格.  

  Sample Input 

  3 3 1 2 3 1 4 7 

  3 7 2 5 8 2 3 4 5 6 7 8  

  0 0 

  Sample Output 

  2 3  

  NULL
题目本身不难 用map 注意熟悉map的操作 然后注意输出格式

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
using namespace std;
const int MAXN=120;
map<int,int> ms;
vector<int> res;
int main(void){
    int n,m;
    while(~scanf("%d%d",&n,&m)){
        if(n==0 && m==0){
            break;
        }
        ms.clear();
        res.clear();
        int t;
        for(int i=0;i<n;i++){
            scanf("%d",&t);
            ms[t]=1;
        }
        for(int i=0;i<m;i++){
            scanf("%d",&t);
            if(ms.find(t)!=ms.end()){
                ms[t]=0;
            }
        }
        int cnt=0;
        map<int,int>::iterator it;
        for(it=ms.begin();it!=ms.end();it++){
            if(it->second==1){
                res.push_back(it->first);
            }
        }
        sort(res.begin(),res.end());
        for(int i=0;i<res.size();i++){
            printf("%d ",res[i]);
            cnt++;
        }
        if(cnt==0){
            printf("NULL");
        }
        printf("\n");
    }
    return 0;
}
```





