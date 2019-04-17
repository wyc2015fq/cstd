# poj2488——A Knight's Journey - westbrook1998的博客 - CSDN博客





2018年08月12日 19:41:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29标签：[dfs																[搜索](https://so.csdn.net/so/search/s.do?q=搜索&t=blog)](https://so.csdn.net/so/search/s.do?q=dfs&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Background  

  The knight is getting bored of seeing the same black and white squares again and again and has decided to make a journey  

  around the world. Whenever a knight moves, it is two squares in one direction and one square perpendicular to this. The world of a knight is the chessboard he is living on. Our knight lives on a chessboard that has a smaller area than a regular 8 * 8 board, but it is still rectangular. Can you help this adventurous knight to make travel plans?  

  Problem  

  Find a path such that the knight visits every square once. The knight can start and end on any square of the board. 

  Input 

  The input begins with a positive integer n in the first line. The following lines contain n test cases. Each test case consists of a single line with two positive integers p and q, such that 1 <= p * q <= 26. This represents a p * q chessboard, where p describes how many different square numbers 1, … , p exist, q describes how many different square letters exist. These are the first q letters of the Latin alphabet: A, … 

  Output 

  The output for every scenario begins with a line containing “Scenario #i:”, where i is the number of the scenario starting at 1. Then print a single line containing the lexicographically first path that visits all squares of the chessboard with knight moves followed by an empty line. The path should be given on a single line by concatenating the names of the visited squares. Each square name consists of a capital letter followed by a number.  

  If no such path exist, you should output impossible on a single line. 

  Sample Input 

  3 

  1 1 

  2 3 

  4 3 

  Sample Output 

  Scenario #1: 

  A1 

  Scenario #2: 

  impossible 

  Scenario #3: 

  A1B3C1A2B4C2A3B1C3A4B2C4
这题我还是很佩服我自己的，竟然能码了这么多代码 

题意是给一个棋盘，每个位置都有标号，然后问马能不能走满全部，马走日。 

因为棋盘很小，我的思路就是枚举起点，进行dfs，这里的枚举按字典序来，这样只要一搜到路径，就能立马输出，所以这里dfs里注意return要两次，不然在第一次return之后还会对其他方向再进行搜索，所以这里还要判断然后return
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
int t,p,q;
int mp[9][9];
int ti;
bool vis[9][9];
int res_i[30];
int res_j[30];
int flag;
bool check(int i,int j){
    if(i<1 || i>q || j<1 || j>p || vis[i][j]){
        return false;
    }
    return true;
}
bool judge(int ti,int tj){
    for(int i=1;i<=q;i++){
        for(int j=1;j<=p;j++){
            if(!vis[i][j] && (ti!=i || tj!=j)){
                return false;
            }
        }
    }
    return true;
}
void dfs(int i,int j,int cnt){
    //printf("%d %d %d\n",i,j,cnt);
    if(judge(i,j)){
        for(int i=0;i<q*p;i++){
            printf("%c%d",res_i[i]-1+'A',res_j[i]);
        }
        flag=1;
        printf("\n");
        return;
    }
    else{
        if(vis[i][j]){
            return;
        }
        else{
            vis[i][j]=true;
            //printf("%d %d %d\n",i,j,check(i-2,j-1));
            if(check(i-2,j-1)){
                res_i[cnt]=i-2;
                res_j[cnt++]=j-1;
                //vis[i-2][j-1]=true;
                //printf("1\n");
                dfs(i-2,j-1,cnt);
                vis[i-2][j-1]=false;
                cnt--;
                if(flag){
                    return;
                }
            }
            if(check(i-2,j+1)){
                res_i[cnt]=i-2;
                res_j[cnt++]=j+1;
                //vis[i-2][j+1]=true;
                //printf("2\n");
                dfs(i-2,j+1,cnt);
                vis[i-2][j+1]=false;
                cnt--;
                if(flag){
                    return;
                }
            }
            if(check(i-1,j-2)){
                res_i[cnt]=i-1;
                res_j[cnt++]=j-2;
                //vis[i-1][j-2]=true;
                //printf("3\n");
                dfs(i-1,j-2,cnt);
                vis[i-1][j-2]=false;
                cnt--;
                if(flag){
                    return;
                }
            }
            if(check(i-1,j+2)){
                res_i[cnt]=i-1;
                res_j[cnt++]=j+2;
                //vis[i-1][j+2]=true;
                //printf("4\n");
                dfs(i-1,j+2,cnt);
                vis[i-1][j+2]=false;
                cnt--;
                if(flag){
                    return;
                }
            }
            if(check(i+1,j-2)){
                res_i[cnt]=i+1;
                res_j[cnt++]=j-2;
                //vis[i+1][j-2]=true;
                //printf("5\n");
                dfs(i+1,j-2,cnt);
                vis[i+1][j-2]=false;
                cnt--;
                if(flag){
                    return;
                }
            }
            if(check(i+1,j+2)){
                res_i[cnt]=i+1;
                res_j[cnt++]=j+2;
                //vis[i+1][j+2]=true;
                //printf("6\n");
                dfs(i+1,j+2,cnt);
                vis[i+1][j+2]=false;
                cnt--;
                if(flag){
                    return;
                }
            }
            if(check(i+2,j-1)){
                res_i[cnt]=i+2;
                res_j[cnt++]=j-1;
                //vis[i+2][j-1]=true;
                //printf("7\n");
                dfs(i+2,j-1,cnt);
                vis[i+2][j-1]=false;
                cnt--;
                if(flag){
                    return;
                }
            }
            if(check(i+2,j+1)){
                res_i[cnt]=i+2;
                res_j[cnt++]=j+1;
                //vis[i+2][j+1]=true;
                //printf("8\n");
                dfs(i+2,j+1,cnt);
                vis[i+2][j+1]=false;
                cnt--;
                if(flag){
                    return;
                }
            }
        }
    }
}
int main(void){
    //freopen("out.out","w",stdout);
    int c=1;
    scanf("%d",&t);
    while(t--){
        flag=0;
        scanf("%d%d",&p,&q);
        if(ti!=0){
            printf("\nScenario #%d:\n",c++);
            ti++;
        }
        else{
            printf("Scenario #%d:\n",c++);
            ti++;
        }
        //枚举起点
        for(int i=1;i<=q;i++){
            for(int j=1;j<=p;j++){
                res_i[0]=i;
                res_j[0]=j;
                //printf("分界线--------\n");
                dfs(i,j,1);
                vis[i][j]=false;
                if(flag){
                    break;
                }
            }
            if(flag){
                break;
            }
        }
        if(!flag){
            printf("impossible\n");
        }
    }
    return 0;
}
```







