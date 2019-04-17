# hdu2147——kiki's game - westbrook1998的博客 - CSDN博客





2018年08月23日 21:52:27[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29








> 
Recently kiki has nothing to do. While she is bored, an idea appears in his mind, she just playes the checkerboard game.The size of the chesserboard is n*m.First of all, a coin is placed in the top right corner(1,m). Each time one people can move the coin into the left, the underneath or the left-underneath blank space.The person who can’t make a move will lose the game. kiki plays it with ZZ.The game always starts with kiki. If both play perfectly, who will win the game?  

  Input 

  Input contains multiple test cases. Each line contains two integer n, m (0
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
int main(void){
    int m,n;
    while(~scanf("%d%d",&n,&m)){
        if(m==0 && n==0){
            break;
        }
        if(m%2 && n%2){
            printf("What a pity!\n");
        }
        else{
            printf("Wonderful!\n");
        }
    }
    return 0;
}
```





