# POJ 1696 Space Ant（极角排序） - HJ - CSDN博客
2017年01月14日 20:14:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：303
Description 
The most exciting space discovery occurred at the end of the 20th century. In 1999, scientists traced down an ant-like creature in the planet Y1999 and called it M11. It has only one eye on the left side of its head and just three feet all on the right side of its body and suffers from three walking limitations: 
```
It can not turn right due to its special body structure. 
It leaves a red path while walking. 
It hates to pass over a previously red colored path, and never does that.
```
The pictures transmitted by the Discovery space ship depicts that plants in the Y1999 grow in special points on the planet. Analysis of several thousands of the pictures have resulted in discovering a magic coordinate system governing the grow points of the plants. In this coordinate system with x and y axes, no two plants share the same x or y.  
An M11 needs to eat exactly one plant in each day to stay alive. When it eats one plant, it remains there for the rest of the day with no move. Next day, it looks for another plant to go there and eat it. If it can not reach any other plant it dies by the end of the day. Notice that it can reach a plant in any distance.  
The problem is to find a path for an M11 to let it live longest.  
Input is a set of (x, y) coordinates of plants. Suppose A with the coordinates (xA, yA) is the plant with the least y-coordinate. M11 starts from point (0,yA) heading towards plant A. Notice that the solution path should not cross itself and all of the turns should be counter-clockwise. Also note that the solution may visit more than two plants located on a same straight line. 
Input 
The first line of the input is M, the number of test cases to be solved (1 <= M <= 10). For each test case, the first line is N, the number of plants in that test case (1 <= N <= 50), followed by N lines for each plant data. Each plant data consists of three integers: the first number is the unique plant index (1..N), followed by two positive integers x and y representing the coordinates of the plant. Plants are sorted by the increasing order on their indices in the input file. Suppose that the values of coordinates are at most 100.
Output 
Output should have one separate line for the solution of each test case. A solution is the number of plants on the solution path, followed by the indices of visiting plants in the path in the order of their visits.
Sample Input
2 
10 
1 4 5 
2 9 8 
3 5 9 
4 1 7 
5 3 2 
6 6 3 
7 10 10 
8 8 1 
9 2 4 
10 7 6 
14 
1 6 11 
2 11 9 
3 8 7 
4 12 8 
5 9 20 
6 3 2 
7 1 6 
8 2 13 
9 15 1 
10 14 17 
11 13 19 
12 5 18 
13 7 3 
14 10 16
Sample Output
10 8 7 3 4 9 5 6 2 1 10 
14 9 10 11 5 12 8 7 6 13 4 14 1 3 2
Source 
Tehran 1999
极角排序。
最后输出的点肯定是n个的e
题目大意： 
    找出一条最长的非右拐的路径
解法： 
    寻找逆时针螺旋线最多能连几个点，每次都连最外面的点即可。极角排序。换做max_element可提高效率，但这个题规模太小了，没必要。综合复杂度(n^2)log(n)。
请结合图片理解此过程： 
![这里写图片描述](https://img-blog.csdn.net/20130726230743312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhlbmduYW5sZWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cmath>
using namespace std;
int pos;
struct node{
    int num,x,y;
}s[55],res[55];
int multi(node a,node p1,node p2)
{
    return (p1.x-a.x)*(p2.y-a.y)-(p1.y-a.y)*(p2.x-a.x);
}
int d(node p1,node p2)
{
    return sqrt((p2.x-p1.x)*(p2.x-p1.x)+(p2.y-p1.y)*(p2.y-p1.y));
}
bool cmp(node a,node b)                               //极角排序 
{
    int tmp=multi(s[pos],a,b);
    if(tmp>0)
      return true;
    else if(tmp==0&&(d(s[pos],a)<d(s[pos],b)))
      return true;
    return false;
}
int main()
{
    int n,m;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%d",&m);
        for(int i=0;i<m;i++)
        {
            scanf("%d%d%d",&s[i].num,&s[i].x,&s[i].y);
            if(s[i].y<s[0].y)                            //找到纵坐标最小的点 
              swap(s[i],s[0]);
        }
        int j=pos=0;
        sort(s,s+m,cmp);
        res[j++]=s[pos++];
        for(int i=2;i<m;i++)
        {
            sort(s+pos,s+m,cmp);                       //每次查找每次排序，以当前pos值所指向的点作为初点 
            res[j++]=s[pos++];
        }
        res[j++]=s[pos++];
        printf("%d",j);
        for(int i=0;i<j;i++)
          printf(" %d",res[i].num);
        printf("\n"); 
    }
    return 0;
}
```
