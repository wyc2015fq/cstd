# N皇后问题(状态压缩实现) - 在思索中前行！ - CSDN博客





2017年02月17日 11:10:24[_Tham](https://me.csdn.net/txl16211)阅读数：1032








[**题目链接~~>**](http://acm.hdu.edu.cn/showproblem.php?pid=2553)

这题用 dfs（）N范围一大了过不了，需要打表，用状态压缩可以![奋斗](http://static.blog.csdn.net/xheditor/xheditor_emot/default/struggle.gif)状态压缩真是太强大了。

**状态压缩 1：**

       在状态压缩中，通常用 ( 1 << N ) - 1 来表示最大状态MAXST，用 A | B 来合并A和B的状态位，用 A & ~B 来清除A状态中所有的B中的状态位。

**一、DFS函数参数Col，MainDiag，ContDiag的表示意义：**

       当整形数Col，MainDiag，ContDiag的第X位为1时，表示因为之前摆放的皇后的纵向攻击，主对角线斜向攻击，副对角线斜向攻击而使得当前行摆放的皇后不能位于第X列。

**二、Col == ( 1 << N ) - 1 的表示意义：**

       ( 1 << N ) - 1 在二进制下就是N个1。这里Col中的1表示的是因为之前摆放的皇后的纵向攻击而使得当前行摆放的皇后不能位于的列，事实上也就是表示已经摆放了皇后的列。那么 Col == ( 1 << N ) - 1 也就表示N列每一列都摆放了皇后，也就是已经摆放了N个皇后，此时即获得一种皇后摆放方案。

**三、emptycol = ( ( 1 << N ) - 1 ) & ~( Col | MainDiag | ContDiag ) 的表示意义：**

       首先 Col | MainDiag | ContDiag 的第X位为1时，显然是表示在综合了之前摆放的皇后的所有攻击方式之后当前行摆放的皇后不能位于第X列。

       那么 emptycol = ( ( 1 << N ) - 1 ) & ~( Col | MainDiag | ContDiag ) 的第X位为1时，则是表示当前行摆放的皇后可以位于第X列。

**四、curcol = ( emptycol ) & ( ( ~emptycol ) + 1 ) 的表示意义：**

       emptycol & ~emptycol 的结果显然是0，但是~emptycol一个巧妙的+1之后却让结果极富意义。

       首先，2进制数字的+1进位（类比一下10进制）还是不难理解的：从最后一位开始，若当前位数字为1则将其进位为0并继续向前进位，若当前位数字为0则将其进位为1并停止进位。

       现假设emptycol中的第一个1在第First位，而其后First-1位都是0。

       那么~emptycol中的第一个0在第First位，而其后First-1位都是1。

       当~emptycol进行+1进位时，其后First-1位全部进位为0，其第First进位为1，而其First之前的所有位则不变。

       这时候再将emptycol与其进行&运算，显然答案的后First-1位为0，第First位为1，而First之前的所有位为0。

       也就是说，curcol = ( emptycol ) & ( ( ~emptycol ) + 1 ) 中仅有一个1，而且这个1的位置即为emptycol中最后一个1的位置！

       那么curcol的表示意义即为当前行摆放的皇后可以摆放的最后一列。

**五、 emptycol &= ~curcol 的表示意义：**

       清除emptycol的最后一个1，表示当前已将皇后摆放在可以摆放的最后一列，之后不必再对此情况进行DFS。

       因而while的循环结束条件是emptycol为0，也就是emptycol中没有1，表示已经没有需要DFS的情况了。

**六、DFS递归的函数参数 Col | curcol，( MainDiag | curcol ) >> 1，( ContDiag | curcol ) << 1 的表示意义：**

      1、Col中的1表示因为之前摆放的皇后的纵向攻击而使得当前行摆放的皇后不能位于的列，而下一行的皇后显然也是不能摆放在这些列的curcol中唯一的1表示当前行皇后摆放的列，那么下一行的皇后因为当前行皇后的纵向攻击而不能摆放在这一列。所以 Col | curcol 即下一行的皇后因为其之前摆放的皇后的纵向攻击而不能摆放的列。

      2、MainDiag中的1表示因为之前摆放的皇后的主对角线斜向攻击而使得当前行摆放的皇后不能位于的列。由于攻击是沿主对角线的，故而当前行影响的是第X列的话，那下一行影响的就是第X+1列。因此之前摆放的皇后的主对角线斜向攻击传递到下一行时则将变为 MainDiag >> 1。而因为当前行摆放状态为curcol的皇后的主对角线斜向攻击，下一行摆放的皇后将不能摆放在 curcol >> 1
 中唯一的1的所在位置对应的列。所以 MainDiag >>1 | curcol >> 1 也就是 ( MainDiag | curcol ) >> 1 即下一行的皇后因为其之前摆放的皇后的主对角线斜向攻击而不能摆放的列。

      3、( ContDiag | curcol ) << 1 的表示意义可类比第2点。 



```cpp
#include<stdio.h>  
int N,SchemeNum;  
void DFS(int Col,int MainDiag,int ContDiag)// Col 列放置皇后的情况，MainDiag 主对角线放置皇后的情况  
{                                          // ContDiag 次对角线放置皇后的情况  
    if(Col==(1<<N)-1) // 找到一种放置方案  
    {  
        ++SchemeNum;  
        return;  
    }  
    int emptycol=(((1<<N)-1)&~(Col|MainDiag|ContDiag)); //当前要放的行可以放的位置  
    while(emptycol)  
    {  
        int curcol=(emptycol)&((~emptycol)+1); //取第一位不为零的位  
        emptycol&=~curcol;                     //删除上一次放置的列  
        DFS(Col|curcol,(MainDiag|curcol)>>1,(ContDiag|curcol)<<1);// (ContDiag|curcol)<<1 最好&（1<<N）-1一下防止溢出  
    }  
}  
int main()  
{  
    while(scanf("%d",&N)==1&&N)  
    {  
       SchemeNum=0;  
       DFS(0,0,0);  
       printf("%d\n",SchemeNum);  
   }  
   return 0;  
}
```

**状态压缩（2）：**

     用Col，First和Second分别表示标记了列，主对角线和副对角线的值，初始时全为0，这样的话(Col | First | Second)就表示到当前列时已经标记了的（不能放皇后的）点，那我们对其取反~操作，


       也就是CanPut = ~（Col| First | Second），那得到的数就表示可以放的位置，所有1所在的位置也就是可以放的位置，那我们每次取出最低位的1(x & (-x))（这个我之前也不知道，网上查到的），那这样的话就得出了当前所有可以放的位置（不用像上面一个一个判断）（如果可以的话，尽可能自己推出递推关系，这里并不难）。


       这里要注意的一个问题就是取反（~）操作之后，由于它高于N的位置也被置为了1，也就是相当于放到了某一行的棋盘的外面去了，所以我设置了一个值High = (1<<N)-1，然后每次得到一个可以放的位置的值CanPut，就对其和High取与（&）运算，这样的话就可以去掉高位的1，得到的数都是High以内的。


       然后就是要处理已经处理了此行如何进行递归倒下一行的问题，假设在本行取出了一个CanPut的最低位LowBit，那么到下一行时，下一行的Col就是Col | LowBit， 当前行的First倒下一行时由于所有标记的点都往右移了一格，所以First>>1，再加上LowBit的右边一格也不能放，所以下一行主对角线已经标记了的就是（First>>1 | LowBit>>1），同理，副对角线标记了的就是（Second<<1 | LOwBit <<1）.

**代码：**

```cpp
#include <stdio.h>  
int N,ans,High;  
void DFS(int Col,int Fir,int Sec)  
{  
    if(Col == High){ans++;return;}//所有的列都已经被标记了，说明每一列都放了  
    int CanPut = ((~(Col | Fir | Sec)) & High);  
    while(CanPut)  
    {  
        int LowBit = CanPut & (-CanPut);//取出最低位  
        DFS((Col|LowBit), ((Fir|LowBit)>>1), (((Sec|LowBit) <<1) & High));  
        CanPut &= (~LowBit);//去掉最低位  
    }  
}  
  
int main()  
{  
    while(~scanf("%d", &N) && N)  
    {  
        High = (1<<N)-1;  ans = 0;  
        DFS(0,0,0);  
        printf("%d\n", ans);  
    }  
    return 0;  
}
```






