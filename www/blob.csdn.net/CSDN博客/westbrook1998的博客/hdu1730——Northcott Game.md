# hdu1730——Northcott Game - westbrook1998的博客 - CSDN博客





2018年05月27日 13:42:26[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：73标签：[博弈																[Nim](https://so.csdn.net/so/search/s.do?q=Nim&t=blog)](https://so.csdn.net/so/search/s.do?q=博弈&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Tom和Jerry正在玩一种Northcott游戏，可是Tom老是输，因此他怀疑这个游戏是不是有某种必胜策略，郁闷的Tom现在向你求救了，你能帮帮他么？  

  游戏规则是这样的：  

  　　如图所示，游戏在一个n行m列（1 ≤ n ≤ 1000且2 ≤ m ≤ 100）的棋盘上进行，每行有一个黑子（黑方）和一个白子（白方）。执黑的一方先行，每次玩家可以移动己方的任何一枚棋子到同一行的任何一个空格上，当然这过程中不许越过该行的敌方棋子。双方轮流移动，直到某一方无法行动为止，移动最后一步的玩家获胜。Tom总是先下（黑方）。图1是某个初始局面，图二是Tom移动一个棋子后的局面（第一行的黑子左移两步）。  
![这里写图片描述](https://odzkskevi.qnssl.com/c0ecba7b2143c519b88fae09d70c62c0?v=1527223829)
![这里写图片描述](https://odzkskevi.qnssl.com/a7acf2cc99626355bfbe19843dba3a5b?v=1527223829)

  Input 

  　　输入数据有多组。每组数据第一行为两个整数n和m，由空格分开。接下来有n行，每行两个数Ti，Ji (1 ≤ Ti, Ji ≤ m)分别表示Tom和Jerry在该行棋子所处的列数。  

  　　 注意：各组测试数据之间有不定数量的空行。你必须处理到文件末。  

  Output 

  对于每组测试数据输出一行你的结果。如果当前局面下Tom有必胜策略则输出“I WIN!”，否则输出“BAD LUCK!”。 

  Sample Input 

  3 6 

  4 5 

  1 2 

  1 2 

  3 6 

  4 5 

  1 3 

  1 2 

  Sample Output 

  BAD LUCK! 

  I WIN!
Nim博弈的简单变形，把棋子间的空格看成石子，因为两方都是选择最优，所以只要有空格，先手的一方就会把棋子“贴住”后手，必胜

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
int main(void){
    int n,m;
    while(~scanf("%d%d",&n,&m)){
        int a,b;
        int res=0;
        for(int i=0;i<n;i++){
            scanf("%d%d",&a,&b);
            //把黑白棋之间的空格当成一个石子，所以这里要减一，和绝对值
            res=res^(abs(a-b)-1);
        }
        if(res>0){
            printf("I WIN!\n");
        }
        else{
            printf("BAD LUCK!\n");
        }
    }
    return 0;
}
```






