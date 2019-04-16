# HDU1426 Sudoku Killer DFS - 紫芝的博客 - CSDN博客





2018年05月24日 20:36:58[紫芝](https://me.csdn.net/qq_40507857)阅读数：35
个人分类：[DFS](https://blog.csdn.net/qq_40507857/article/category/7566193)









# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1426)

# Sudoku Killer



****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 9634    Accepted Submission(s): 2887****





Problem Description

自从2006年3月10日至11日的首届数独世界锦标赛以后，数独这项游戏越来越受到人们的喜爱和重视。

据说，在2008北京奥运会上，会将数独列为一个单独的项目进行比赛，冠军将有可能获得的一份巨大的奖品———HDU免费七日游外加lcy亲笔签名以及同hdu acm team合影留念的机会。

所以全球人民前仆后继，为了奖品日夜训练茶饭不思。当然也包括初学者linle，不过他太笨了又没有多少耐性，只能做做最最基本的数独题，不过他还是想得到那些奖品，你能帮帮他吗？你只要把答案告诉他就可以，不用教他是怎么做的。


数独游戏的规则是这样的：在一个9x9的方格中，你需要把数字1-9填写到空格当中，并且使方格的每一行和每一列中都包含1-9这九个数字。同时还要保证，空格中用粗线划分成9个3x3的方格也同时包含1-9这九个数字。比如有这样一个题，大家可以仔细观察一下，在这里面每行、每列，以及每个3x3的方格都包含1-9这九个数字。


例题：
![](http://acm.hdu.edu.cn/data/images/C31-1001-1.jpg)


答案：
![](http://acm.hdu.edu.cn/data/images/C31-1001-2.jpg)





Input

本题包含多组测试，每组之间由一个空行隔开。每组测试会给你一个 9*9 的矩阵，同一行相邻的两个元素用一个空格分开。其中1-9代表该位置的已经填好的数，问号（?）表示需要你填的数。





Output

对于每组测试，请输出它的解，同一行相邻的两个数用一个空格分开。两组解之间要一个空行。

对于每组测试数据保证它有且只有一个解。





Sample Input

7 1 2 ? 6 ? 3 5 8
? 6 5 2 ? 7 1 ? 4
? ? 8 5 1 3 6 7 2
9 2 4 ? 5 6 ? 3 7
5 ? 6 ? ? ? 2 4 1
1 ? 3 7 2 ? 9 ? 5
? ? 1 9 7 5 4 8 6
6 ? 7 8 3 ? 5 1 9
8 5 9 ? 4 ? ? 2 3

Sample Output

7 1 2 4 6 9 3 5 8
3 6 5 2 8 7 1 9 4
4 9 8 5 1 3 6 7 2
9 2 4 1 5 6 8 3 7
5 7 6 3 9 8 2 4 1
1 8 3 7 2 4 9 6 5
2 3 1 9 7 5 4 8 6
6 4 7 8 3 2 5 1 9
8 5 9 6 4 1 7 2 3
Author

linle

Source

[ACM暑期集训队练习赛（三）](http://acm.hdu.edu.cn/search.php?field=problem&key=+ACM%CA%EE%C6%DA%BC%AF%D1%B5%B6%D3%C1%B7%CF%B0%C8%FC%A3%A8%C8%FD%A3%A9&source=1&searchmode=source)

Recommend

LL   |   We have carefully selected several similar problems for you:  [1258](http://acm.hdu.edu.cn/showproblem.php?pid=1258)[1045](http://acm.hdu.edu.cn/showproblem.php?pid=1045)[1016](http://acm.hdu.edu.cn/showproblem.php?pid=1016)[1010](http://acm.hdu.edu.cn/showproblem.php?pid=1010)[2614](http://acm.hdu.edu.cn/showproblem.php?pid=2614)

思路：

找到每个空位，从1到9尝试一遍，不冲突就把这个数字填上，然后填下一个空位

重点：

判断九宫格内的数字是否合法

```cpp
int tmpx=(x/3)*3,tmpy=(y/3)*3;  
    for(int i=tmpx;i<tmpx+3;++i){//判断九宫格  
        for(int j=tmpy;j<tmpy+3;++j){  
            if(sd[i][j]==k)return false;  
        }  
    }
```

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> P;
P node[90];
int sd[10][10],f=0,cnt;
bool jud(int x,int y,int k){
    for(int i=0;i<9;++i){//判断行和列
        if(sd[i][y]==k)return false;
        if(sd[x][i]==k)return false;
    }
    int tmpx=(x/3)*3,tmpy=(y/3)*3;
    for(int i=tmpx;i<tmpx+3;++i){//判断九宫格
        for(int j=tmpy;j<tmpy+3;++j){
            if(sd[i][j]==k)return false;
        }
    }
    return true;
}
void dfs(int n){
    if(f)return;
    if(n==cnt){
        f=1;
        for(int i=0;i<9;++i){
            for(int j=0;j<9;++j){
                printf("%d%c",sd[i][j],j==8?'\n':' ');
            }
        }
        return;
    }
    for(int i=1;i<=9;++i){
        if(jud(node[n].first,node[n].second,i)){
            sd[node[n].first][node[n].second]=i;
            dfs(n+1);
        }
        if(f)return;//找到结果，退出函数
    }
    sd[node[n].first][node[n].second]=0;//循环结束也没有解，说明此状态无解，退回0
}
int main(){
    char s[5];
    int i,j;
    while(~scanf("%s",s)){//scanf自动忽略回车，所以不用处理数据之间的空行
        cnt=0;
        sd[0][0]=(s[0]=='?'?0:s[0]-'0');//处理第一行第一位
        for(i=1;i<9;++i){
            scanf("%s",s);
            sd[0][i]=(s[0]=='?'?0:s[0]-'0');//第一行后面八位
        }
        for(i=1;i<9;++i){
            for(j=0;j<9;++j){
                scanf("%s",s);
                sd[i][j]=(s[0]=='?'?0:s[0]-'0');//后面八行
            }
        }
        for(int i=0;i<9;++i){//记录没有填的位置
            for(int j=0;j<9;++j){
                if(!sd[i][j])node[cnt].first=i,node[cnt++].second=j;
            }
        }
        if(f)putchar('\n');//两组解之间有空格
        f=0;
        dfs(0);
    }
}
```







