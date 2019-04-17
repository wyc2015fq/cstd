# Codeforces1016D——Vasya And The Matrix - westbrook1998的博客 - CSDN博客





2018年08月24日 18:15:30[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：39标签：[构造																[矩阵																[异或](https://so.csdn.net/so/search/s.do?q=异或&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
Now Vasya is taking an exam in mathematics. In order to get a good mark, Vasya needs to guess the matrix that the teacher has constructed! 

  Vasya knows that the matrix consists of n rows and m columns. For each row, he knows the xor (bitwise excluding or) of the elements in this row. The sequence a1, a2, …, an denotes the xor of elements in rows with indices 1, 2, …, n, respectively. Similarly, for each column, he knows the xor of the elements in this column. The sequence b1, b2, …, bm denotes the xor of elements in columns with indices 1, 2, …, m, respectively. 

  Help Vasya! Find a matrix satisfying the given constraints or tell him that there is no suitable matrix. 

  Input 

  The first line contains two numbers n and m (2 ≤ n, m ≤ 100) — the dimensions of the matrix. 

  The second line contains n numbers a1, a2, …, an (0 ≤ ai ≤ 109), where ai is the xor of all elements in row i. 

  The third line contains m numbers b1, b2, …, bm (0 ≤ bi ≤ 109), where bi is the xor of all elements in column i. 

  Output 

  If there is no matrix satisfying the given constraints in the first line, output “NO”. 

  Otherwise, on the first line output “YES”, and then n rows of m numbers in each ci1, ci2, … , cim (0 ≤ cij ≤ 2·109) — the description of the matrix. 

  If there are several suitable matrices, it is allowed to print any of them. 

  Examples 

  Input 

  2 3 

  2 9 

  5 3 13 

  Output 

  YES 

  3 4 5 

  6 7 8 

  Input 

  3 3 

  1 7 6 

  2 15 12 

  Output 

  NO
给定每一行每一列的异或值，构造一个满足条件的矩阵 

判断是否能构造这个想出来了，就是行的异或全部异或 列的异或再全部异或 就都等于全部数字的异或，判断这两个数字是否相等就好了

但是构造矩阵还是没能想出来，看了题解发现特别简单，就是最后一列和最后一行就放每一行每一列异或的那些值，其他全部为0，然后计算出右下角的一个就可以了

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;
int mat[105][105];
int np[105];
int mp[105];
int main(void){
    int n,m;
    int nx=0;
    int mx=0;
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++){
        scanf("%d",&np[i]);
        nx^=np[i];
    }
    for(int i=0;i<m;i++){
        scanf("%d",&mp[i]);
        mx^=mp[i];
    }
    if(nx!=mx){
        printf("NO\n");
    }
    else{
        mat[n-1][m-1]=mp[m-1];
        for(int i=0;i<n-1;i++){
            mat[i][m-1]=np[i];
            mat[n-1][m-1]^=np[i];
        }
        for(int i=0;i<m-1;i++){
            mat[n-1][i]=mp[i];
        }
        printf("YES\n");
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                printf("%d",mat[i][j]);
                if(j!=m-1){
                    printf(" ");
                }
            }
            printf("\n");
        }
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=矩阵&t=blog)](https://so.csdn.net/so/search/s.do?q=构造&t=blog)




