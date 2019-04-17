# hdu1202——The calculation of GPA - westbrook1998的博客 - CSDN博客





2018年07月29日 19:44:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：49标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
每学期的期末，大家都会忙于计算自己的平均成绩，这个成绩对于评奖学金是直接有关的。国外大学都是计算GPA(grade point average) 又称GPR(grade point ratio)，即成绩点数与学分的加权平均值来代表一个学生的成绩的。那么如何来计算GPA呢？  

  一般大学采用之计分法  

  A90 - 100 4 点  

  B80 - 89 3 点  

  C70 - 79 2 点  

  D60 - 69 1 点  

  E0 - 59 0 点  

  例如：某位学生修习三门课，其课目、学分及成绩分别为：  

  英文：三学分、92 分；化学：五学分、80 分；数学：二学分、60分，则GPA的算法如下：  

  科目 学分 分数 点数 分数×点数 

  英文  3    92    4     12  

  化学  5    80    3     15  

  数学  2    60    1      2  

  合计  10   29  

  29/10=2.9  

  2.9即为某生的GPA  

  下面有请你写一个用于计算GPA的程序。  

  Input 

  包含多组数据，每组数据的第一行有一个数N，接下来N行每行表示一门成绩。每行有两个实型的数 s,p，s表示这门课的学分,p表示该学生的成绩（百分制）。如果p=-1则说明该学生这门课缺考，是不应该计算在内的。  

  Output 

  对每组数据输出一行，表示该学生的GPA，保留两位小数。如果GPA不存在，输出-1。  

  Sample Input 

  3 

  3 92 

  5 80 

  2 60 

  Sample Output 

  2.90
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
int main(void){
    int n;
    while(~scanf("%d",&n)){
        double p,m;
        double ans=0;
        double r=0;
        for(int i=0;i<n;i++){
            scanf("%lf%lf",&p,&m);
            if(m!=-1){
                r+=p;
                if(m>=90){
                    ans+=4*p;
                }
                else if(m>=80){
                    ans+=3*p;
                }
                else if(m>=70){
                    ans+=2*p;
                }
                else if(m>=60){
                    ans+=1*p;
                }
            }
        }
        //printf("%lf %d\n",ans,r);
        if(r==0){
            printf("-1\n");
        }
        else{
            printf("%.2lf\n",ans/r);
        }
    }
    return 0;
}
```





