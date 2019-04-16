# poj 1469 COURSES 二分图的最大匹配 匈牙利算法 - 紫芝的博客 - CSDN博客





2018年08月28日 13:16:31[紫芝](https://me.csdn.net/qq_40507857)阅读数：35








COURSES
|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 25820||**Accepted:** 9957|

Description

Consider a group of N students and P courses. Each student visits zero, one or more than one courses. Your task is to determine whether it is possible to form a committee of exactly P students that satisfies simultaneously the conditions: 
- 
- every student in the committee represents a different course (a student can represent a course if he/she visits that course) 
- each course has a representative in the committee 

Input

Your program should read sets of data from the std input. The first line of the input contains the number of the data sets. Each data set is presented in the following format: 


P N 

Count1 Student1 1 Student1 2 ... Student1 Count1 

Count2 Student2 1 Student2 2 ... Student2 Count2 

... 

CountP StudentP 1 StudentP 2 ... StudentP CountP 


The first line in each data set contains two positive integers separated by one blank: P (1 <= P <= 100) - the number of courses and N (1 <= N <= 300) - the number of students. The next P lines describe in sequence of the courses �from course 1 to course P, each line describing a course. The description of course i is a line that starts with an integer Count i (0 <= Count i <= N) representing the number of students visiting course i. Next, after a blank, you抣l find the Count i students, visiting the course, each two consecutive separated by one blank. Students are numbered with the positive integers from 1 to N. 

There are no blank lines between consecutive sets of data. Input data are correct. 

Output

The result of the program is on the standard output. For each input data set the program prints on a single line "YES" if it is possible to form a committee and "NO" otherwise. There should not be any leading blanks at the start of the line.

Sample Input

```
2
3 3
3 1 2 3
2 1 2
1 1
3 3
2 1 3
2 1 3
1 1
```

Sample Output

```
YES
NO
```

Source

[Southeastern Europe 2000](http://poj.org/searchproblem?field=source&key=Southeastern+Europe+2000)



```cpp
#include<cstdio>
#include<cstring>
using namespace std;
const int N=1001;
int n1,n2,k;
//n1,n2为二分图的顶点集，其中x∈n1,y∈n2
int map[N][N],vis[N],link[N];
//link记录n2中的点y在n1中所匹配的x点的编号

int find(int x)
{
    int i;
    for(i=1;i<=n2;i++)
    {
        //x->i有边，且节点i未被搜索
        if(map[x][i]&&!vis[i])
        {
            vis[i]=1;//标记节点已被搜索

            //如果i不属于前一个匹配M或者i匹配到的节点可以寻找到增广路
            if(link[i]==0||find(link[i])){
                link[i]=x;//更新
                return 1;//匹配成功
            }
        }
    }
    return 0;
}
int main()
{
    int T;
    scanf("%d",&T);
    while( T-- ){
        scanf("%d%d",&n1,&n2);
        memset(map,0,sizeof map);
        memset(link,0,sizeof link);
        for(int j=1;j<=n1;j++)
    {
        scanf("%d",&k);
        for(int i=0;i<k;i++)
    {
        int y;
        scanf("%d",&y);
        map[j][y]=1;
    }
}

    int s=0;//最大匹配数
    for(int i=1;i<=n1;i++)
    {
        memset(vis,0,sizeof(vis));
        if(find(i)) s++;
    }
    if(s==n1)//如果最大匹配数等于课程数
    printf("YES\n");
    else
    printf("NO\n");
}
    return 0;
}
```





