# POJ 3279(Fliptile)题解 - 在思索中前行！ - CSDN博客





2017年06月23日 17:47:24[_Tham](https://me.csdn.net/txl16211)阅读数：436










## 【题意】



给定长宽的黑白棋棋盘摆满棋子，每次操作可以反转一个位置和其上下左右共五个位置的棋子的颜色，求要使用最少翻转次数将所有棋子反转为黑色所需翻转的是哪些棋子。

## 【题目分析】

这题刚开始被放到搜索的分类下了..然而这和搜索有什么关系..没经验所以想了各种和搜索沾边的方法，结果没想出解法，直到看了网上的题解，压根不是搜索。


具体解法：首先根据题目，每次操作都会影响到周围的“棋子”，而要使得每个1都被反转为0，那么我们就应当每次都反转1下方的棋子以改变1为0.

那么，当我们处理过1到n-1行的时候，前n-1行就都已经是0了，最后我们只需要检查最后一行是不是全部为0就可以检查这次的出操作是否正确了。如果正确且最小，那就存起来。最后输出，万事大吉。

当然，因为我们要改变第x行的1为0需要反转的是x+1行的位置。而这个整个规则是我们验证一组操作是否能达到目的所用的，那么我们需要在验证前先确定操作（没操作怎么验证..）。

于是根据规则内容可知，只需要能确认第一行的翻转情况，就能够推出下面所有的翻转情况并验证是否正确。于是需要做的就是枚举第一行的情况了。

## 【算法流程】

整个代码分了四部分，处理输入部分没啥可说的，接下来就是doWork干活部分了..

需要干的活就是枚举第一行的所有情况然后对于每次枚举都计算验证是否符合要求以及反转所需的次数。其中，第一行的状态数量可以使用左移运算优化（效率比pow高），于是总共枚举的数量就有1<<col次。另外，因为这使得一行的状态是由一个数字保存的，所以依然使用位运算取得是否翻转的状态。

将枚举好的一次首行状态存好后就可以交给calc计算和验证了。验证就是通过上述翻转规则操作。其中get(x,y)为取得某个位置是否为1的状态的方法（过程）。

最后检查结果就好。下面是代码。




```cpp
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <queue>
#define each(i,n) (int i=1;i<=(n);++i)
#define INF 0x3f3f3f3f

using namespace std;

int row,col;
int arr[20][20];
int flip[20][20],ans[20][20];
int dir[5][2] = {
    0,0, 0,1, 0,-1, -1,0, 1,0 
};

int get(int x,int y) {
    int c = arr[x+1][y+1];
    for(int i = 0;i<5;i++) {
        int dx = x + dir[i][0];
        int dy = y + dir[i][1];
        if(dx >=0 && dx<row && dy>=0 && dy<col) {
            c+=flip[dx][dy];
        }
    }
    return c&1; // with flip state, if odd return 1
}

int calc() {
    for each(i,row-1) {
        for(int j = 0;j<col;j++) {
            if(get(i-1,j)) ++flip[i][j];
        }
    }
    for(int i=0;i<col;i++) { // check last line
        if(get(row-1,i)) return 0;
    }
    int cnt = 0;
    for (int i=0;i<row;i++) { //统计翻转的次数  
        for (int j=0;j<col;j++) {
            cnt += flip[i][j];  
        }
    }
    return cnt;
}


void doWork() {
    int cnt = INF;
    for(int i=0;i<(1<<col);i++) { //from 0000 to 1111
        memset(flip,0,sizeof(flip));
        for(int j=0;j<col;j++) {
            flip[0][col-j-1] = (i>>j)&1; //get pos state form binary number
        }
        int num = calc();
        if (num<cnt && num!=0) {
            cnt = num;
            memcpy(ans,flip,sizeof(flip));
        }
    }
    if (cnt==INF) printf("IMPOSSIBLE\n");
    else {
        for (int i=0;i<row;i++) {  
            printf("%d",ans[i][0]);  
            for each(j,col-1) {
                printf(" %d",ans[i][j]);
            }
            printf("\n");  
        }
    }
}

int main() {
    
    while(~scanf("%d%d",&row,&col)) {
        memset(arr,0,sizeof(arr));
        for each(i,row) {
            for each(j,col) {
                scanf("%d",&arr[i][j]);
            }
        }
        
        doWork();
    }
    
}
```






