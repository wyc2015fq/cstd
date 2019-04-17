# 冲刺NOIP复习，算法知识点总结 - 在思索中前行！ - CSDN博客





2015年11月03日 17:05:09[_Tham](https://me.csdn.net/txl16211)阅读数：4343







**前言**

       离NOIP还有一个星期，匆忙的把整理的算法补充完善，看着当时的整理觉得那时还年少。第二页贴了几张从贴吧里找来的图片，看着就很热血的。当年来学这个竞赛就是为了兴趣，感受计算机之美的。经过时迁，计划赶不上变化，现在尚处于迷茫之中，也很难说当时做的决定是对是错。然而我一直坚信迷茫的时候选择难走的路会看见更好的风景。

      这篇文章简单的说了一下NOIP考试中会常用的算法，可能难度掌握的不是太好，有一部分内容不是NOIP考查范围，然而随着难度的增加，看一些更高级的算法也没有坏处。还有一些非常非常基础的比如链表啊什么的就直接没有写上（别问我为什么整理了那么多的排序算法）。

      最后祝大家在NOIP中取得理想的成绩！



## **搜索**
**DFS  （框架）**





```cpp
procedure dfs(x);
  var
  begin
     if 达到目标状态 then 输出结果并退出过程;
     if 满足剪枝条件 then exit;
     for i:=1 to 搜索宽度 do 
         begin
             备份现场；（注意如果现场使用了全局变量，则需要使用局部变量备份）
         dfs(参数+增量)；
         恢复现场；
end;
```

优化


（1）   最优化剪枝：求最优值时，当前的状态无论如何不可能比最优值更优，则退出，可与展望结合剪枝

（2）   可行性剪枝：提前判断该状态是否能得到可行解，如不能则退出

（3）   记忆化搜索：对于已经搜索过的状态直接退出

（4）   改变搜索顺序：对于看起来希望更大的决策先进行搜索

（5）   优化搜索策略

（6）   预处理找到大体搜索翻译

（7）   改写成IDA*算法

（8）   卡时（注意现在联赛中禁止使用meml掐时）





**BFS （**框架）




```cpp
初始化；把初始布局存入
    设首指针head=0；   尾指针tail:=1；
    repeat    
        inc(head)，取出队列首记录为当前被扩展结点；   
        for  i：=1  to   规则数 do    {r是规则编号}
        begin
            if  新空格位置合法  then
            begin
                if  新布局与队列中原有记录不重复
                     tail增1，并把新布局存入队尾；
              if  达到目标  then  输出并退出；
            end；
        end；
    until  head>=tail； {队列空}
```

优化

判重的优化：hash，二叉排序树

双向广搜或启发式搜索

改写成A*算法

二分优化



## 排序

冒泡排序：（框架）



```cpp
var a:array[1..100] of longint;t,n,i,j:longint;
procedure sort;
begin
  for i:=1 to n-1 do{与每个数都进行比较}
   for j:=1 to n-i do
    if a[j]>a[j+1] then
    begin
    t:=a[j];
    a[j]:=a[j+1];
    a[j+1]:=t;
    end;
end;
```


选择排序：（框架）





```cpp
var a:array[1..100] of longint;t,n,i,j:longint;
procedure sort;
begin
  for i:=1 to n-1 do
   for j:=1+i to n do{大数沉小数浮}
    if a[j]>a[i] then
    begin
    t:=a[j];
    a[j]:=a[i];
    a[i]:=t;
    end;
end;
```


插入排序：（框架）





```cpp
var a:array[0..100] of longint;n,i,j,t:longint;
procedure sort;
begin
   for i:=2 to n do
     for j:=1 to (i-1) do
     begin
       if  (a[i]<a[j]) then
       begin
          t:=a[j];
          a[j]:=a[i];
          a[i]:=t;
       end;
     end;
end;
```


桶排：（框架）





```cpp
var a,b:array[0..100] of longint;r,i,j,t,k,n:longint;
procedure sort;
begin
  for i:=0 to 100 do b[i]:=0;{为B数组清零，小桶内容清零}
  for i:=1 to n do b[a[i]]:=b[a[i]]+1;
{桶的序号就是那个要排序的东西；出现一次，桶里得旗数加一}
  for i:=0 to 100 do{扫描所有的桶}
    begin
      if b[i]<>0 then{桶里有旗}
      for j:=1 to b[i] do write(i,' ');{桶的序号就是那个数}
end;
end;
```


快速排序：（框架）





```cpp
var a:array[1..100] of longint;
    n,i,h,g:longint;
procedure kp(l,r:longint);{变量不能与全局变量相同，否则会被抹去}
  var b,m,i,j,t:longint;
begin
    i:=l;
    j:=r;
m:=a[(l+r) div 2];{基准数最好从中间取}
    repeat
      while a[j]>m do dec(j);
      while a[i]<m do inc(i);{两侧的哨兵移动}
      if i<=j then
{哨兵未碰面}{“=”利用repeat循环的性质，使repeat循环得以结束}
      begin
        t:=a[j];
        a[j]:=a[i
        a[i]:=t;{交换两个哨兵的值}
        inc(j);
        dec(j);{哨兵继续运动}
      end;
    until i>j;
    if j>l then kp(l,j);
    if i<r then kp(i,r);{都是循环不结束后进行的动作}
end;
begin
  read(n);
  for i:=1 to n do read(a[i]);
  kp(1,n); {“一”位置与“N”位置}
  for i:=1 to n-1 do write(a[i],' ');
  write(a[n]);{防止多输出空格使程序结果出错}
end.
```


堆排序：（框架）





```cpp
var a:array[1..100] of longint;
    n,i,b:longint;
procedure jianshu(i:longint);
  begin
while ((a[i]>a[i*2])or(a[i]>a[i*2+1]))and(i<=n div 2) do
{当父亲数大于子女数时并且他有孩子时进行}
      begin
        if a[i*2]<=a[i*2+1]{左儿子小于右儿子}
        then
          begin
            b:=a[i*2]; a[i*2]:=a[i];a[i]:=b;{左右儿子的值互换}
            jianshu(i*2);{继续为左儿子建树}
          end
        else
          begin
            b:=a[i*2+1];a[i*2+1]:=a[i];a[i]:=b;
            jianshu(i*2+1);{上同，不过是为右儿子建树}
          end;
      end;
  end;
procedure tiao;
  begin
    while n<>0 do
      begin
        write(a[1]);
        a[1]:=a[n];
        n:=n-1;
        for i:=(n div 2) downto 1 do
         jianshu(i);
      end;
  end;
begin
  read(n);
  for i:=1 to n do
    read(a[i]);
  for i:=(n div 2) downto 1 do
    jianshu(i);
  tiao;
end.
```




## 数学定理

**中国剩余定理**

若有一些两两互质的整数m1, m2,… mn，则对任意的整数：a1,a2,...an，以下联立同余方程组对模数m1, m2,… mn 有公解：



**康托展开**


  a[i]为当前未出现的元素中是排在第几个（从0开始）

  把一个整数X展开成如下形式：

  X=a[n]*(n-1)!+a[n-1]*(n-2)!+...+a[i]*(i-1)!+...+a[2]*1!+a[1]*0!

  其中a[i]为当前未出现的元素中是排在第几个（从0开始），并且0<=a[i]<i(1<=i<=n)
**错排通项**


  考虑一个有n个元素的排列，若一个排列中所有的元素都不在自己原来的位置上，那么这样的排列就称为原排列的一个错排。

  f[1]=0;f[2]=1;

  f[n] =(n-1)(f[n-2) + f[n-1])

  f[n]:=n![1-1/1!+1/2!-1/3!……+(-1)^n*1/n!]

  f[n] = (n!/e+0.5)，其中e是自然对数的底，[x]为x的整数部分。

**费马大定理**


    费马大定理，又被称为“费马最后的定理”，由法国数学家费马提出。它断言当整数n >2时，关于x, y, z的方程 xn + yn = zn 没有正整数解。
    被提出后，经历多人猜想辩证，历经三百多年的历史，最终在1995年被英国数学家安德鲁·怀尔斯证明。


**费马小定理**

    假如a是一个整数，p是一个素数，那么 ap ≡a (mod p)。

    如果a不是p的倍数，这个定理也可以写成ap-1 ≡1 (mod p)。----这个更加常用


**逆元**

    由费马小定理：假如p是质数，且gcd(a,p)=1，那么ap-1≡1（mod p）

    逆元：如果ab≡1（mod p），那么在模p意义下，a、b互为逆元。

    所以，假如p是质数，且gcd(a,p)=1，那么a的逆元是ap-2

    逆元的作用：在模意义下进行除法。乘a的逆元等同于除以a。


**欧拉函数**

    在数论中，对正整数n，欧拉函数是小于或等于n的正整数中与n互质的数的数目。此函数以其首名研究者欧拉命名，它又称为φ函数、欧拉商数等。

    若m,a为正整数，且m,a互素，(gcd(a,m) = 1)，则aφ(m)≡1，其中为φ(m)欧拉函数，mod m为同余关系。

    欧拉定理实际上是费马小定理的推广。


**Stirling数**

    第一类s(p,k)的一个的组合学解释是：将p个物体排成k个非空循环排列的方法数。

    s(p,k)的递推公式： s(p,k)=(p-1)*s(p-1,k)+s(p-1,k-1) ,1<=k<=p-1

    边界条件：s(p,0)=0 ,p>=1  s(p,p)=1  ,p>=0

    递推关系的说明：

    考虑第p个物品，p可以单独构成一个非空循环排列，这样前p-1种物品构成k-1个非空循环排列，方法数为s(p-1,k-1)；

    也可以前p-1种物品构成k个非空循环排列，而第p个物品插入第i个物品的左边，这有(p-1)*s(p-1,k)种方法。

    第二类S(p,k)的一个组合学解释是：将p个物体划分成k个非空的不可辨别的（可以理解为盒子没有编号）集合的方法数。

    k!S(p,k)是把p个人分进k间有差别(如：被标有房号）的房间(无空房）的方法数。

    S(p,k)的递推公式是：S(p,k)=k*S(p-1,k)+S(p-1,k-1) ,1<= k<=p-1

    边界条件：S(p,p)=1 ,p>=0    S(p,0)=0 ,p>=1

    递推关系的说明：

    考虑第p个物品，p可以单独构成一个非空集合，此时前p-1个物品构成k-1个非空的不可辨别的集合，方法数为S(p-1,k-1)；

    也可以前p-1种物品构成k个非空的不可辨别的集合，第p个物品放入任意一个中，这样有k*S(p-1,k)种方法。

    PS：第一类斯特林数和第二类斯特林数有相同的初始条件，但递推关系不同。




## 数论
**GCD&LCM**





```cpp
//GCD 
function gcd(a,b:longint):longint;
begin
if b=0 then gcd:=a
else gcd:=gcd (b,a mod b);
end ;
 
//LCM
function lcm(a,b:longint):longint;
begin
if a<b then swap(a,b);
lcm:=a;
while lcm mod b>0 do inc(lcm,a);
end;
```

**素数**





```cpp
//单个判断
function prime (n: longint): boolean;
var i longint;
begin
for i:=2 to trunc(sqrt(n)) do
if n mod i=0 then exit(false)
exit(true); 
end;

//筛法打表
procedure main;
var i,j:longint;
begin
    fillchar(f,sizeof(f),true);
    f[0]:=false;f[1]:=false;
    for i:=2 to trunc(sqrt(maxn)) do
    if f[i] then
           begin
            j:=2*i;
            while j<=    maxn do
            begin
                    f[j]:=false;
                    inc(j,i);
                    end;
        end;
end;
```
**快速幂**

```cpp
//{a^b mod n}
function f(a,b,n:int64):int64;
var t,y:int64;
begin
    t:=1;
    y:=a;
    while b<>0 do
        begin
            if(b and 1)=1 then t:=t*y mod n;
            y:=y*y mod n;
{这里用了一个很强大的技巧,y*y即求出了a^(2^(i-1))不知道这是什么的看原理}
            b:=b shr 1;{去掉已经处理过的一位}
        end;
    exit(t);
end;
```
**模运算法则**




(A+B) mod C = (A mod C + B mod C) mod C

(A-B) mod C = (A mod C - B mod C) mod C

(A * B) mod C = (A mod C) * (B mod C) mod C

(A / B) mod C = ???

结合律

((a+b) mod p + c)mod p = (a + (b+c) mod p) mod p

((a*b) mod p * c)mod p = (a * (b*c) mod p) mod p

分配律 

((a +b)mod p × c) mod p = ((a × c) mod p + (b × c) mod p) mod p


**组合和全排列**

排列A(n,m)（m是上标）=n!/（n-m）!=nx(n-1)x...xm

组合C(n,m)=n!/m!（n-m）!=[nx(n-1)x...xm]/m!


**阶乘**




```cpp
//{a^b mod n}
function f(a,b,n:int64):int64;
var t,y:int64;
begin
    t:=1;
    y:=a;
    while b<>0 do
        begin
            if(b and 1)=1 then t:=t*y mod n;
            y:=y*y mod n;
{这里用了一个很强大的技巧,y*y即求出了a^(2^(i-1))不知道这是什么的看原理}
            b:=b shr 1;{去掉已经处理过的一位}
        end;
    exit(t);
end;
```

**约瑟夫环问题**

递推公式，设有n个人（0,...,n-1），数m，则第i轮出局的人为f(i)=(f(i-1)+m-1)%(n-i+1),f(0)=0;

例：有M个猴子围成一圈，每个有一个编号，编号从1到M。打算从中选出一个大王。经过协商，决定选大王的规则如下：从第一个开始，每隔N个，数到的猴子出圈，最后剩下来的就是大王。要求：从键盘输入M，N，编程计算哪一个编号的猴子成为大王。





```cpp
var a:array[1..1000] of longint;
    m,n,i,j,sum:longint;
    weizhi:longint=0;
begin
  readln(m,n);
  for i:=1 to m do a[i]:=i;
  sum:=m;
  repeat
   weizhi:=weizhi+1;
   if weizhi>m then weizhi:=weizhi-m;
   if a[weizhi]<>0 then
    begin
     j:=j+1;
     if j=n then
      begin
       a[weizhi]:=0;
       j:=0;
       sum:=sum-1;
      end;
    end;
  until sum=1;
  for i:=1 to m do if a[i]<>0 then write(a[i]);
end.
```
**Catalan数**


**h(n)= h(0)*h(n-1)+h(1)*h(n-2) + ... + h(n-1)h(0) (n>=2)**

```cpp
h[0]:=1;
    h[1]:=1;
    for i:=2 to n do
        begin
            j:=i-1;
            k:=0;
            while k<>i do
                begin
                    h[i]:=h[i]+h[k]*h[j];
                    dec(j);
                    inc(k);
                end;
        end;
```


1、矩阵连乘： P=a1×a2×a3×……×an，依据乘法结合律，不改变其顺序，只用括号表示成对的乘积，试问有几种括号化的方案？(h(n-1)种)

2、一个栈(无穷大)的进栈序列为1，2，3，…，n，有多少个不同的出栈序列。

3、对于一个二进制的01串，共n+m位，满足n个1，m个0，且0<=n-m<=1，该串还满足从左向右1的个数永远大于0的个数。问共有多少种排列方式。

4、在一个凸多边形中，通过若干条互不相交的对角线，把这个多边形划分成了若干个三角形。求不同划分的方案数。

5、给定N个节点，能构成多少种不同的二叉树？






**扩展欧几里德算法**


若存在一组解x0,y0，满足b*x0+(a mod b)*y0=d则取x=y0，y=x0-(a div b)*y0，有ax+by=d这样,我们可以用类似辗转相除的迭代法求解。




```cpp
function extended-gcd(a, b:longint; var x, y:integer);
var x1, y1 :integer;
begin
     if b=0 then begin
         extended-gcd:=a;
         x:=1; y:=0
     end else begin
         extended-gcd:=extended-gcd(b, a mod b, x1, y1);
         x:=y1;
         y:=x1-(a div b)*y1;
     end;
  end;
```

**对自然数因子的计算**


怎样求一个数有多少个因数？

对于一个已知的自然数，要求出它有多少个因数，可用下列方法：

首先将这个已知数分解质因数，将此数化成几个质数幂的连乘形式，然后把这些质数的指数分别加一，再相乘，求出来的积就是我们要的结果。

　　例如：求360有多少个因数。

　　因为360分解质因数可表示为：360=2^3×3^2×5，２、３、５的指数分别是３、２、１，这样３６０的因数个数可这样计算出：（３＋１）（２＋１）（１＋１）＝２４个。




怎样求有n个因数的最小自然数？

同样拥有n个（n为确定的数）因数的自然数可以有多个不同的数，如何求出这些数中的最小数？

　　这是与上一个问题相反的要求，是上一题的逆运算。

　　比如求有２４个因数的最小数是多少？

　　根据上一问题解决过程的启示，可以这样做，先将２４分解因式，把２４表示成几个数连乘积的形式，再把这几个数各减去１，作为质数２、３、５、７......的指数，求出这些带指数的数连乘积，试算出最小数即可。具体做法是：

　　因为：24=4×6，  24=3×8， 24=4×3×2，

　　现在分别以这三种表示法试求出目标数x：

　　（１）、２４＝４×６，４－１＝３，６－１＝５

　　              Ｘ＝2^5×3^3＝８６４

       （２）、２４＝３×８，３－１＝２，８－１＝７

　　　          Ｘ＝2^7×3^2＝１１５２

　　（３）２４＝４×３×２，4-1=3, 3-1=2, 2-1=1

　　　          Ｘ＝2^3×3^2×5＝３６０

　　 综合(1)、(2)、(3)可知３６０是有２４个因数的最小数。

**矩阵乘法**


矩阵乘法有下列三要素：

（1）可乘原则：前列数 = 后行数

（2）乘积阶数：前行数 × 后列数

（3）Cij乘积元素：= 前矩阵的第i行元素与后矩阵的第j列对应元素的乘积之和

矩阵乘法求斐波那契数列的原理：

f(n) 是第n项的值。

f(1)= 1; f(2) =1;

f(n)= f(n-1) + （n-2)

分两步推导：

**位运算**


Pascal和C中的位运算符号

下面的a和b都是整数类型，则：

C语言  |  Pascal语言

——-+————-

a & b  |  a and b

a | b  |  a or b

a ^ b  |  a xor b

  ~a   |   not a

a << b |  a shl b

a >> b |  a shr b

注意C中的逻辑运算和位运算符号是不同的。520|1314=1834，但520||1314=1，因为逻辑运算时520和1314都相当于True。同样的，!a和~a也是有区别的。

注意：在数学中TRUE对应一；FALSE对应0
**常用位运算**
=== 1. and运算 ===

and运算通常用于二进制取位操作，例如一个数 and 1的结果就是取二进制的最末位。这可以用来判断一个整数的奇偶，二进制的最末位为0表示该数为偶数，最末位为1表示该数为奇数.

X and –X返回的值是X第一个1出现的位数

=== 2. or运算 ===

or运算通常用于二进制特定位上的无条件赋值，例如一个数or 1的结果就是把二进制最末位强行变成1。如果需要把二进制元元元元最末位变成0，对这个数or 1之后再减一就可以了，其实际意义就是把这个数强行变成最接近的偶数。


=== 3. xor运算 ===

xor运算通常用于对二进制的特定一位进行取反操作，因为异或可以这样定义：0和1异或0都不变，异或1则取反。

xor运算的逆运算是它本身，也就是说两次异或同一个数最后结果不变，即(a xor b) xor b = a。


=== 4. not运算 ===

not运算的定义是把内存中的0和1全部取反。使用not运算时要格外小心，你需要注意整数类型有没有符号。如果not的对象是无符号整数（不能表示负数），那么得到的值就是它与该类型上界的差，因为无符号类型的数是用$0000到$FFFF依次表示的。


=== 5. shl运算 ===

a shl b就表示把a转为二进制后左移b位（在后面添b个0）。例如100的二进制为1100100，而110010000转成十进制是400，那么100 shl 2 = 400。可以看出，a shl b的值实际上就是a乘以2的b次方，因为在二进制数后添一个0就相当于该数乘以2。



=== 6. shr运算 ===

和shl相似，a shr b表示二进制右移b位（去掉末b位），相当于a除以2的b次方（取整）。我们也经常用shr 1来代替div 2，比如二分查找、堆的插入操作等等。想办法用shr代替除法运算可以使程序效率大大提高。最大公约数的二进制算法用除以2操作来代替慢得出奇的mod运算，效率可以提高60%。


常用位运算详细示例


    功能                 |                      示例            |    位运算

———————-+———————————+——————–

去掉最后一位           | (101101->10110)           | x shr 1

在最后加一个0         | (101101->1011010)       | x shl 1

在最后加一个1         | (101101->1011011)       | x shl 1+1

把最后一位变成1      | (101100->101101)         | x or 1

把最后一位变成0      | (101101->101100)         | x or 1-1

最后一位取反           | (101101->101100)          | x xor 1

把右数第k位变成1    | (101001->101101,k=3)      | x or (1 shl (k-1))

把右数第k位变成0    | (101101->101001,k=3)      | x and not (1 shl (k-1))

右数第k位取反          | (101001->101101,k=3)      | x xor (1 shl (k-1))

取末三位                   | (1101101->101)                 | x and 7

取末k位                     | (1101101->1101,k=5)       | x and (1 shl k-1)

取右数第k位              | (1101101->1,k=4)              | x shr (k-1) and 1

把末k位变成1            | (101001->101111,k=4)      | x or (1 shl k-1)

末k位取反                  | (101001->100110,k=4)      | x xor (1 shl k-1)

把右边连续的1变成0  | (100101111->100100000)    | x and (x+1)

把右起第一个0变成1  | (100101111->100111111)    | x or (x+1)

把右边连续的0变成1  | (11011000->11011111)        | x or (x-1)

取右边连续的1           | (100101111->1111)              | (x xor (x+1)) shr 1

去掉右起第一个1的左边 | (100101000->1000)           | x and (x xor (x-1))

**补码**

计算机用$0000到$7FFF依次表示0到32767的数，剩下的$8000到$FFFF依次表示-32768到-1的数。32位有符号整数的储存方式也是类似的。稍加注意你会发现，二进制的第一位是用来表示正负号的，0表示正，1表示负。这里有一个问题：0本来既不是正数，也不是负数，但它占用了$0000的位置，因此有符号的整数类型范围中正数个数比负数少一个。对一个有符号的数进行not运算后，最高位的变化将导致正负颠倒，并且数的绝对值会差1。也就是说，not a实际上等于-a-1。这种整数储存方式叫做“补码”。





## 动态规划

PS：鉴于网上有大量的专门讲述DP的文章，这里不多做累赘的说明。总而言之，DP作为NOIP必考算法，熟练的掌握是极其必要的。
**特征**


1、最优子结构

      如果问题的一个最优解中包含了子问题的最优解，则该问题具有最优子结构。也称最优化原理

      最优子结构也可以理解为“整体最优则局部最优”。反之不一定成立。

2、重叠子问题

      在解决整个问题时，要先解决其子问题，要解决这些子问题，又要先解决他们的子子问题 ……。而这些子子问题又不是相互独立的，有很多是重复的，这些重复的子子问题称为重叠子问题。

      动态规划算法正是利用了这种子问题的重叠性质，对每一个子问题只解一次，而后将其解保存在一个表中，以后再遇到这些相同问题时直接查表就可以，而不需要再重复计算，每次查表的时间为常数。

3.无后效性原则

已经求得的状态，不受未求状态的影响。
**步骤**

1、找出最优解的性质，并刻画其结构特征；

   2、递归地定义最优值（写出动态规划方程）；

   3、以自底向上的方式计算出最优值；记忆化搜索（树型）、递推

   4、根据计算最优值时得到的信息，构造一个最优解。
关键

状态转移方程的构造是动态规划过程中最重要的一步,也是最难的一步.对于大多数的动态规划,寻找状态转移方程有一条十分高效的通道,就是寻找变化中的不变量（已经求得的值）.定量处理的过程也就是决策实施的过程.
**格式**
动态规划的一般倒推格式为：




```cpp
f[Un]=初始值；
for k←n-1 downto 1  do          //枚举阶段
   for U取遍所有状态 do            //枚举状态
      for X取遍所有决策 do         //枚举决策
         f[Uk]=opt{f[Uk+1]+L[Uk,Xk]}； 
 
L[Uk,Xk]：状态Uk通过策略Xk到达状态Uk+1的费用
输出：f[U1]:目标
```

动态规划的一般顺推格式为：





```cpp
f[U1]=初始值；
for k←2 to n  do              //枚举每一个阶段
   for U取遍所有状态 do
      for X取遍所有决策 do
         f[Uk]=opt{f[Uk-1]+L[Uk-1,Xk-1]}； 
 
L[Uk-1,Xk-1]：
状态Uk-1通过策略Xk-1到达状态Uk  的费用
输出：f[Un]:目标
```
**推荐参考资料**

刘永辉：《通过金矿模型介绍动态规划》

dd_engi:《背包九讲》

ncepu：《动态规划_从新手到专家》

董的博客：《背包问题应用》

知乎问题：什么是动态规划？动态规划的意义是什么？

推荐习题

PS:以CODEVS为主要OJ，POJ上的DP题目早有人整理

背包型动态规划：1014,1068

序列型动态规划：1044,1576,3027

区间型动态规划：1048,1154,1166

棋盘性动态规划：1010,1169,1219,1220,

划分型动态规划：1017,1039,1040

树型动态规划：1163,1380






## 图论算法

**回路问题**

概念补充：奇点就是从这个点出发的线有奇数条的点。
Euler回路

定义：若图G中存在这样一条路径，使得它恰通过G中每条边一次,则称该路径为欧拉路径。若该路径是一个圈，则称为欧拉(Euler)回路。
充要条件：图连通且无奇点
Hamilton回路
定义：经过图的每个顶点且仅一次的算法
充要条件：图连通且奇点数为0个或2个
一笔画问题
如何判断一个图形是否可以一笔不重地画出
　　■⒈凡是由偶点组成的连通图，一定可以一笔画成。画时可以把任一偶点为起点，最后一定能以这个点为终点画完此图。
　　■⒉凡是只有两个奇点的连通图（其余都为偶点），一定可以一笔画成。画时必须把一个奇点为起点，另一个奇点终点。
　　■⒊其他情况的图都不能一笔画出。(奇点数除以二便可算出此图需几笔画成。)
Einstein学起了画画，
此人比较懒～～，他希望用最少的笔画画出一张画。。。给定一个无向图，包含 n 个顶点(编号1~n)，m 条边，求最少用多少笔可以画出图中所有的边
解法：注意此题可能图不是联通的，所以需要用并查集预处理后完成





```cpp
var n,m,i,u,v,sum1,sum2,mid:longint;
    b,f:array[0..1010] of longint;
 
procedure intt;
begin
    assign(input,'draw.in');
    assign(output,'draw.out');
    reset(input);
    rewrite(output);
end;
 
procedure outt;
begin
    close(input);
    close(output);
end;
 
procedure sort(l,r: longint);
      var
         i,j,x,y: longint;
      begin
         i:=l;
         j:=r;
         x:=f[(l+r) div 2];
         repeat
           while f[i]<x do
            inc(i);
           while x<f[j] do
            dec(j);
           if not(i>j) then
             begin
                y:=f[i];
                f[i]:=f[j];
                f[j]:=y;
                y:=b[i];
                b[i]:=b[j];
                b[j]:=y;
                inc(i);
                j:=j-1;
             end;
         until i>j;
         if l<j then
           sort(l,j);
         if i<r then
           sort(i,r);
      end;
 
function root(x:longint):Longint;
begin
    if f[x]=x then exit(x) else root:=root(f[x]);
    f[x]:=root;
    exit(root);
end;
 
begin
    intt;
    readln(n,m);
    for i:=1 to n do f[i]:=i;
    for i:=1 to m do
        begin
            read(u,v);
            if root(u)<>root(v) then
                f[root(u)]:=root(v);
            inc(b[u]);
            inc(b[v]);
        end;
    for i:=1 to n do mid:=root(i);
    sort(1,n);
    v:=n;
    while v>1 do
        begin
            while f[v-1]<>f[v] do dec(v);
            inc(sum2);
            mid:=f[v];
            while f[v]=mid do
                begin
                    if b[v] mod 2=1 then inc(sum1);
                    dec(v);
                end;
            if sum1>0 then sum1:=sum1-2;
            sum2:=sum2+sum1 div 2;
            sum1:=0;
        end;
    writeln((sum1 div 2)+sum2);
    outt;
end.
```
**图的遍历**


DFS

遍历算法（递归过程）：

 1)从某一初始出发点i开始访问： 输出该点编号；并对该点作被访问标志（以免被重复访问）。

 2)再从i的其中一个未被访问的邻接点j作为初始点出发继续深搜。（按序号从小到大的顺序访问）

 3)当i的所有邻接点都被访问完，则退回到i的父结点的另一个邻接点k再继续深搜。

 直到全部结点访问完毕。

```cpp
var a:array[1..100,1..100]of longint;p:array[1..100] of boolean;m,n,j:longint;
procedure init;
var i,x,y:longint;
begin
 readln(n);readln(m);
 for i:=1 to m do
  begin
   read(x,y);a[x,y]:=1;a[y,x]:=1;
  end;
end;
procedure dfs(k:longint);
var i:longint;
begin
  write(k,' ');p[k]:=true;
  for i:=1 to m do
  if ((p[i]=false)and(a[k,i]=1)) then dfs(i);{k（i）是具体的点}
end;
begin
  fillchar(p,sizeof(p),false);
  init;
  dfs(1);
end.
```


BFS

按层次遍历：

 从图中某结点i出发，在访问了i之后依次访问i的各个未曾访问的邻接点（按序号由小到大的顺序），

 然后分别从这些邻接点出发按广度优先搜索的顺序遍历图，直至图中所有可被访问的结点都被访问到。





```cpp
var a:array[1..100] of longint;
    closed:longint=1;
    open:longint=0;
    t:array [1..100,1..100] of longint;
    p:array [1..100]of boolean;
    j,n,m,g:longint;
procedure add(x:longint);
begin
 inc(closed);
 a[closed]:=x;
end;
function del:longint;
begin
  inc(open);
  del:=a[open];
end;
procedure init;
var i,x,y:longint;
begin
 readln(n);
 readln(m);
 for i:=1 to m do
  begin
   read(x,y);
   t[x,y]:=1;
   t[y,x]:=1;
  end;
end;
procedure bfs(j:longint);
var i,k:longint;
begin
  a[1]:=(j);
  write(j,' ');
  p[j]:=true;
  while open<closed do
  begin
  inc(open);
  k:=a[open];
  for i:=1 to n do
  if ((p[i]=false)and(t[k,i]=1)) then
   begin
     write(i,' ');
     p[i]:=true;
     inc(closed);
     a[closed]:=i;
  end;
  end;
end;
begin
  for g:=1 to m do
  begin
  p[g]:=false;
  a[g]:=0;
  end;
  init;
  bfs(1);
end.
```

**最短路径**

dijkstra算法

算法思想

设给定源点为Vs，S为已求得最短路径的终点集，开始时令S={Vs} 。当求得第一条最短路径(Vs ，Vi)后，S为{Vs，Vi} 。根据以下结论可求下一条最短路径。

设下一条最短路径终点为Vj ，则Vj只有：

◆  源点到终点有直接的弧<Vs，Vj>；

◆ 从Vs 出发到Vj 的这条最短路径所经过的所有中间顶点必定在S中。即只有这条最短路径的最后一条弧才是从S内某个顶点连接到S外的顶点Vj 。

    若定义一个数组dist[n]，其每个dist[i]分量保存从Vs 出发中间只经过集合S中的顶点而到达Vi的所有路径中长度最小的路径长度值，则下一条最短路径的终点Vj必定是不在S中且值最小的顶点，即：

    dist[i]=Min{ dist[k]| Vk∈V-S }

    利用上述公式就可以依次找出下一条最短路径。

示例程序

算法思想





```cpp
var a:array[1..100,1..100]of integer;//邻接矩阵
    flag:array[1..100] of boolean;//已经找到最短路径的节点标志
    path:array[1..100]of integer;
    w,x,n,i,j,min,minn,k:integer;
begin
readln(n,k);
for i:=1 to n do//读取邻接矩阵,无路径写-1
    begin
    for j:=1 to n do 
begin
read(a[i,j]);
If a[i,j]=-1 then a[I,j]:=maxint;
End;
    readln;
    end;
fillchar(flag,sizeof(flag),false);//标明所有节点都未找到最短路径
flag[k]:=true;                       //源节点除外
fillword(path,sizeof(path) div 2,k);
path[k]:=0;
minn:=k;//标记最小的点
for x:=2 to n do
    begin
    min:=32767;//标记要找下一个最短路径点的距离
    for i:=1 to n do//找下一点点
        if (a[k,i]<min) and (flag[i]=false) then
            begin
            min:=a[k,i];
            minn:=i;
            end;
    flag[minn]:=true;//标记下一个点的找到
    for j:=1 to n do //更新最短路径
        if (j<>minn) and (a[k,minn]+a[minn,j]<a[k,j]) and (flag[j]=false) then
        begin 
        a[k,j]:=a[k,minn]+a[minn,j];
        path[j]:=minn;
        end;
    end;
for    i:=1 to n do write(a[k,i],' ');//输出源点到各个点的距离
writeln;
for    i:=1 to n do write(path[i],' ');//输出源点到各个点的距离
end.
```

**floyd算法**


在原路径里增加一个新结点，如果产生的新路径比原路径更小，则用新路径值代替原路径的值。这样依次产生n个矩阵（n为网络结点数）

用公式表示就是，对于K=1,2,3…n,第k个矩阵

运算过程中K从1开始，而 i，j 则分别从1到n取遍所有值，然后k加1，直到k等于n时停止。

示例程序

算法思想





```cpp
var st,en,f:integer;
    k,n,i,j,x:integer;
    a:array[1..10,1..10] of integer;//邻接矩阵
    path:array[1..10,1..10] of integer;//path[a,b]为A点到B点的最短路径要走的第一个点
begin
    readln(n);//读取节点数
    for i:=1 to n do//读取邻接矩阵
        begin
            for j:=1 to n do
                begin
                    read(k);
                    if k<>0 then a[i,j]:=k
                    else a[i,j]:=maxint;
                    path[i,j]:=j;
                end;
            readln;
        end;
for x:=1 to n do//I,J点之间加入X点，判断是否更短，并更新。
    for i:=1 to n do
    for j:=1 to n do
    if a[i,j]>a[i,x]+a[x,j] then
        begin
            a[i,j]:=a[i,x]+a[x,j];
            path[i,j]:=path[i,x];
        end;
    readln(st,en);//读取开始结束点
    writeln(a[st,en]);//写出开始结束长度
    f:=st;
    while f<> en do//写出路径
        begin
            write(f);
            write('-->');
            f:=path[f,en];
        end;
    writeln(en);
end.
```

**spfa算法**


2.1.  SPFA算法需要什么

　    SPFA需要用到一个先进先出的队列Q。

　    SPFA需要对图中的所有顶点做一个标示，标示其是否在队列Q中。

2.2.  SPFA是怎样执行的

　　2.2.1   SPFA的初始化

          　　SPFA的初始化和Dijkstra类似。

          　　先把所有顶点的路径估计值初始化为代价最大值。比如：maxint。

          　　所有顶点都标记为不在队列中。

          　　起始顶点放入队列Q中。

          　　起始顶点标记在队列中。

          　　起始顶点的最短路径估计值置为最小值，比如0。

　          　然后下面是一个循环

　　2.2.2 SPFA循环

         　　循环结束的条件是队列Q为空。第一次进入循环的时候，只有起始顶点一个元素。

         　　每次循环，弹出队列头部的一个顶点。

         　　对这个顶点的所有出边进行松弛。如果松弛成功，就是出边终点上对应的那个顶点的路径代价值被改变了，且这个被松弛的顶点不在队列Q中，就把这个被松弛的顶点入队Q。注意，这里顶点入队的条件有2：1.松弛成功。2.且不在队列Q中。

         　　当队列Q没有了元素。算法结束

示例程序

算法思想





```cpp
const
    maxp=10000;{最大结点数}
    maxv=100000;{最大边数}
var
    p,c,s,t:longint;{p,结点数;c,边数;s:起点;t:终点}
    a,b:array[1..maxp,0..maxp]of longint;
    {a[x,y]存x,y之间边的权;b[x,c]存与x相连的第c个边的另一个结点y}
    d:array[1..maxp]of integer;{队列}
    v:array[1..maxp]of boolean;{是否入队的标记}
//    rn:array[1..maxp]of integer;{各个点的入队次数}
    dist:array[1..maxp]of longint;{到起点的最短路}
    head,tail:longint;{队首/队尾指针}
procedure init;
var
    i,x,y,z:longint;
begin
  read(p,c);
  for i:=1 to c do
    begin
     readln(x,y,z);
     {x,y:一条边的两个结点;z:这条边的权值}
     inc(b[x,0]);b[x,b[x,0]]:=y;a[x,y]:=z;
     {b[x,0]：以x为一个结点的边的条数,b[x,a]:x点连接的第A个节点}
     inc(b[y,0]);b[y,b[y,0]]:=x;a[y,x]:=z;{无向图的操作，取舍}
   end;
  readln(s,t);
end;
procedure spfa(s:longint);
var i,j,now,sum:longint;
begin
  fillchar(d,sizeof(d),0);
  fillchar(v,sizeof(v),false);
  for j:=1 to p do dist[j]:=maxlongint;
  dist[s]:=0;
  v[s]:=true;
  d[1]:=s;
// rn[s]:=1;
  {队列的初始状态,s为起点}
  head:=1;tail:=1;
  while head<=tail do{队列不空，注意：此处head>tial未空}
   begin
    now:=d[head];
    for i:=1 to b[now,0]do{now指向的所有节点}
    if dist[b[now,i]]>dist[now]+a[now,b[now,i]]then
    {如果最短路径大于now节点的最短路径和now节点到该节点的路径之和}
    begin
     dist[b[now,i]]:=dist[now]+a[now,b[now,i]];{修改最短路}
     if not v[b[now,i]]then{扩展结点入队——如果此节点尚未入队，则入队，且表示}
     begin
      inc(tail);
      d[tail]:=b[now,i];
      v[b[now,i]]:=true;
//    inc(rn[b[now,i]]);
//    if rn[b[now,i]]>c then begin writeln('fu huan');halt; end;
       end;
   end;
   v[now]:=false;{释放结点，一定要释放掉，因为这节点有可能下次用来松弛其它节点}
   inc(head);
 end;
end;
procedure print;
begin
  writeln(dist[t]);
end;
begin
  init;
  spfa(s);
  print;
end.
```
**最小生成树**

prim算法

假设N=(V，E)是连通网，TE是N上最小生成树中边的集合。

1、算法实现：算法从U={u0}(u0∈V)，TE={}开始，重复执行下述操作：

在所有u∈U，v∈V-U的边(u，v)中找一条代价最小的边(u0 ,v0),将其并入集合TE，同时将v0并入U集合。

当U=V则结束，此时TE中必有n-1条边，则T=(V，{TE})为N的最小生成树。时间复杂度为0（n2）

2、算法思想：普里姆算法构造最小生成树的过程是从一个顶点U={u0}作初态，不断贪心寻找与U中顶点相邻且代价最小的边的另一个顶点，扩充到U集合直至U=V为止。

优化方法

     我们很容易就可以发现prim算法的关键：每次如何从生成树T到T外的所有边中，找出一条最小边。例如，在第k次前，生成树T中已有k个顶点和（k-1）条边，此时，T到T外得所有边数为k*（n-k）,当然，包括没有边的两顶点，我们记权值为“无穷大”的边在内，从如此多的边中查找最短边，时间复杂度为O(k(n-k))，显然无法满足我们的期望。

      我们来看O(n-k)的方法：假定在进行第k次前已经保留着从T中到T外的每一个顶点（共n-k个）的各一条最短边，在进行第k次时，首先从这（n-k）条最短边中，找出一条最最短边（它就是从T到T外的最短边），假设为（vi,vj），此步需要进行（n-k）次比较；然后把边（vi,vj）和顶点vj并入T中的边集TE和顶点集U中，此时，T外只有n-(k+1)个顶点，对于其中的每个顶点vt，若（vj,vt）边上的权值小于原来保存的从T中到vt的最短边的权值，则用（v,vt）修改之，否则，保持原最小边不变。这样就把第k次后T中到T外的每一个顶点vt的各一条最短边都保留下来了，为第(k+1)次最好了准备。这样Prim的总时间复杂度为O(n^2)。

示例程序（优化后）

算法思想

```cpp
const
  max=1000;
var
  map:array[1..MXN,1..MXN] of longint;
  cost:array[1..MXN] of longint;
  visit:array[1..MXN] of boolean;
  i,j,n,m,x,y,v:longint;
function prim():longint;
  var
   i,j,min,mini,ans:longint;
  begin
   ans:=0;
   for i:=1 to n do begin visit[i]:=false;cost[i]:=maxlongint;end;
//visit[i]是i点是否被访问的标志，cost[i]是到i点的最小权边。
   for i:=2 to n do
     if map[1,i]<>0 then cost[i]:=map[1,i];visit[1]:=true;
for i:=1 to n-1 do
    begin
     min:=maxlongint;
     for j:=1 to n do
        if not visit[j] and (cost[j]<min) then 
           begin min:=cost[j];mini:=j;end;
     visit[mini]:=true;inc(ans,min);
     for j:=1 to n do
      if not visit[j] and (map[mini,j]>0) and (map[mini,j]<cost[j]) then cost[j]:=map[mini,j];
//更新圈内圈外存储的最短距离
    end;
   exit(ans);
  end;
begin
  readln(n,m);
  for i:=1 to m do
   begin
    readln(x,y,v);
    if (map[x,y]=0) or (map[x,y]>v) then
     begin
      map[x,y]:=v;map[y,x]:=v;
     end;
   end;
  writeln(prim());
end.
```

Kruskal算法


       先构造一个只含 n 个顶点、而边集为空的子图，把子图中各个顶点看成各棵树上的根结点，之后，从网的边集 E 中选取一条权值最小的边，若该条边的两个顶点分属不同的树，则将其加入子图，即把两棵树合成一棵树，反之，若该条边的两个顶点已落在同一棵树上，则不可取，而应该取下一条权值最小的边再试之。依次类推，直到森林中只有一棵树，也即子图中含有 n-1 条边为止。

　   时间复杂度为为O(e^2), 使用并查集优化后复杂度为 O（eloge），与网中的边数有关，适用于求边稀疏的网的最小生成树。

       设图G的度为，G=(V,E)，设该图的最小生成树为T=(V,TE)，设置边的集合TE的初始状态为空集。将图G中的边按权值从小到大排好序，然后从小的依次开始选取，若选取得边使生成树T不形成回路，则把它并入TE中，保留作为T的一条边；若选取得边使生成树形成回路，则舍弃；如此继续进行，直到使TE中包含n-1条边为止。

实现克鲁斯卡尔(Kruskal)算法时，要解决以下两个问题：

1.选择代价最小的边(堆排序，或简单选择排序)；

2.判定边所关联的两个顶点是否在同一个连通分量中（集合）

算法的关键与优化：

       kruskal算法实现过程中的关键和难点在于：如何判断欲加入的一条边是否与生成树中已经保留的边形成回路？我们可以将顶点划分到不同的集合中，每个集合的顶点表示一个无回路的连通分量。初始时，我们把n个顶点划分到n个集合中，每个集合只有一个顶点，表明顶点之间互不相通。当选取一条边时，若它的两个顶点分属两个不同的集合，则表明此边连通了两个不同的连通分量，因每个连通分量无回路，所以连同后得到的连通分量仍无回路。因此这条边应该保留，且合并成一个连通分量，若选取得边的两个顶点属于同一个连通分量，则应舍弃，因为一个无回路的连通分量内加入一条新边必然产生回路。

示例程序





```cpp
const MXN=1000;
 type
  rqmap=record
   s,t,v:longint;
  end;
 var
  map:array[1..MXN*MXN] of rqmap;
  father:array[1..MXN] of longint;
  n,m,i,ingraph,ans:longint;
procedure qsort(b,e:longint);//排序
  var
   i,j,x:longint;
   t:rqmap;
  begin
   i:=b;j:=e;x:=map[(i+j)>>1].v;
   while (i<=j) do
    begin
     while (map[i].v<x) do inc(i);
     while (map[j].v>x) do dec(j);
     if (i<=j) then begin t:=map[i];map[i]:=map[j];map[j]:=t;inc(i);dec(j);end;
    end;
   if i<e then qsort(i,e);
   if j>b then qsort(b,j);
  end;
 function find(x:longint):longint; 
  begin
   if (father[x]=x) then exit(x);
   father[x]:=find(father[x]);//路径压缩
   exit(father[x]);
  end;
procedure union(a,b:longint); //并查集
  begin
   father[find(a)]:=find(father[b]);
  end;
 begin
readln(n,m);
  for i:=1 to n do father[i]:=i;
  for i:=1 to m do readln(map[i].s,map[i].t,map[i].v);
  qsort(1,m);ans:=0;ingraph:=1;i:=0;
  while (ingraph<n) do
   begin
    inc(i);
    if find(map[i].s)<>find(map[i].t) then
     begin
      inc(ingraph);inc(ans,map[i].v);union(map[i].s,map[i].t);
     end;
   end;
  writeln(ans);
end.
```
topology排序

整体思想：扫描纵列，如果纵列上没有任何后继节点，则找到，把这一列的横排都填为零

```cpp
var a:array[1..100,1..100] of longint;
    s:array[1..100] of longint;
    ans:array[1..100] of longint;
    i,j,n,kg,x,w:longint;
begin
  readln(n);
  fillchar(a,sizeof(a),0);
  fillchar(s,sizeof(s),0);
  for i:=1 to n do
   for j:=1 to n do
    begin
     read(a[i,j]);
    end;
  i:=1;
  x:=1;
  for j:=1 to n do
   begin
    kg:=0;
    for w:=1 to n do
     if a[w,j]=1 then inc(kg);
    if kg=0 then
     begin
      ans[x]:=j;
      inc(x);
      for w:=1 to n do a[j,w]:=0;
     end;
   end;
  for i:=1 to n do
   write(ans[i],' ');
end.
```
关键路径

利用拓扑排序

```cpp
const maxx=50;
var a:array[1..maxx,1..maxx]of longint;//邻接矩阵
    p:array[1..maxx] of longint;//拓扑路径
    b:array[1..maxx] of longint;//b[i]表示第i个点到起点的距离
    c:array[1..maxx] of longint;//c[i]表示在关键路径时i点的前驱
    pd:array[1..maxx] of boolean;
    n,m,i,j,k:longint;
procedure change;
var i,j:longint;
begin
  fillchar(a,sizeof(p),0);
  fillchar(b,sizeof(b),0);
  fillchar(c,sizeof(c),0);
  fillchar(p,sizeof(p),0);
  fillchar(pd,sizeof(pd),false);
end;
procedure tp(k:longint);
var i,j:longint;
begin
    if k=n+1 then exit;
    for i:=1 to n do
    begin
        if (b[i]=0) and (pd[i]) then
        begin
            p[k]:=i;//拓扑序列
            pd[i]:=false;
            for j:=1 to n do if a[i,j]<>0 then dec(b[j]);//b[j] 用来存入度
            tp(k+1);
        end;
    end;
end;
procedure main;
var i,j:longint;
begin
  b[p[1]]:=0;
  c[p[1]]:=0;
  for i:=2 to n do
   for j:=1 to n do
    if b[p[j]]+a[p[j],p[i]]>b[p[i]] then
     begin
      b[p[i]]:=b[p[i]]+a[p[j],p[i]];
      c[p[i]]:=p[j];
     end;
end;
procedure output;
var i:longint;
begin
  for i:=1 to n-1 do
   write(c[i],'-->');
  writeln(c[n]);
  writeln(b[n]);
end;
begin
  change;
  readln(n);
  for i:=1 to n do
   for j:=1 to n do
    read(a[i,j]);
  tp(1);
  main;
  output;
end.
```
按照概念



（1）关键路径：AOE网中，从事件i到j的路径中，加权长度最大者称为i到j的关键路径（Critical Path），记为cp(i,j)。特别地，始点0到终点n的关键路径cp(0,n)是整个AOE的关键路径。



（2）事件最早/晚发生时间

         etv:事件vi的最早发生时间为,从始点到vi的最长（加权）路径长度，即cp(0,i)

         ltv:事件vi的最晚发生时间为：在不拖延整个工期的条件下，vi的可能的最晚发生时间。即ltv(i) = etv(n) - cp(i, n)

        要从右往左推

（3）活动最早/晚开始时间

         活动ak=<vi, vj>的最早开始时间ete(k)：等于事件vi的最早发生时间，即ete(k) = etv(i) = cp(0, i)

         活动ak=<vi, vj>的最晚开始时间lte(k)定义为：在不拖延整个工期的条件下，该活动的允许的最迟开始时间，

即lte(k) = ltv(j)–len(i, j)，这里，ltv(j)是事件j的允许的最晚发生时间，len(i, j)是ak的权。

         活动ak的最大可利用时间：定义为lte(k)-ete(k)


4．关键路径：是活动的集合

若活动ak的最大可利用时间等于0（即(lte(k)=ete(k)），则称ak 为关键活动，否则为非关键活动。

显然，关键活动的延期，会使整个工程延期。但非关键活动不然，只要它的延期量不超过它的最大可利用时间，就不会影响整个工期。





```cpp
const maxx=100;
var a:array[1..maxx,1..maxx] of longint;
    et,eet:array[1..maxx] of longint;
    //eet表示活动最早可以开始的时间,et表示活动最迟应该开始的时间
    n,i,j,k,x,y,w:longint;
    min,max:longint;
begin
  fillchar(a,sizeof(a),char(-1));
  fillchar(et,sizeof(et),0);
  fillchar(eet,sizeof(eet),0);
  readln(n);
  readln(x,y,w);
  while x<>0 do
   begin
     a[x,y]:=w;
     readln(x,y,w);
   end;
  eet[1]:=0;
  for i:=2 to n do
   begin
    max:=0;
    for j:=1 to n do
     if a[j,i]<>-1 then
      if a[j,i]+eet[j]>max then
        max:=a[j,i]+eet[j];
    eet[i]:=max;
   end;
  et[n]:=eet[n];
  for i:=n-1 downto 1 do//最迟开始时间需要倒着推
   begin
    min:=maxint;
    for j:=1 to n do
        if et[j]-a[i,j]<min then//事件vi的最晚发生时间为：在不拖延整个工期的条件下，vi的可能的最晚发生时间。即ltv(i) = etv(n) - cp(i, n)
       min:=et[j]-a[i,j];
   if a[i,j]<>-1 then
    et[i]:=min;
   end;
  for i:=1 to n-1 do
   if et[i]=eet[i] then
   //如果最早开始时间和最晚开始时间相同即认为是关键事件
     write(i,'-->');
  writeln(n);
  writeln(max);
end.
//默认为节点按顺序输入，即序号前面的结点会影响序号后面结点的活动
```

**次短路**


删边：每次删最短路上的一条边，用Dijkstra+堆求单源最短路径，则每次求最短路径时间复杂度为O(N*log(N+M) + M)，所以总的时间复杂度为O(N*M*log(N+M) + M^2)。

**次小生成树**


{O（n^3）}


先求MST 并记录选择的边

依次删掉这些边（n-1次）做MST求出新的生成树的值

则这些值中最小的即为the Second MST

{O(n^2)}

在求MST的同时，对于新加入的节点，max[i][j]记录已加入的节点i到新加入节点j的路径上边权最大的值。

求完后mst记录最小生成树的权值和，枚举每一条不在树中的边u~v，必然与MST形成一个圈，max[u][v]中存的就是圈中除了新加入的边外最大的边了。那么加入u~v后，生成树的权值min=mst+g[u][v]-max[u][v];

最小的min就是次小生成树





**二分图最大匹配—匈牙利算法**



```cpp
var
  g:array[1..maxn,1..maxm]of boolean;  //图，g[i,j]:节点i到节点j是否有边
  y:array[1..maxm]of boolean;          //访问标记，y[i]:节点i是否已被访问过
  link:array[1..maxm]of longint;       //与节点i匹配的点，link[i]=0 表示i没有被匹配
 
function find(v:longint):boolean; //从v出发找匹配
var 
  i:longint;
begin
  for i:=1 to m do                         //枚举与v相连的点i
    if g[v,i] and (not y[i]) then          //如果i与v相连且还未被访问过
    begin
      y[i]:=true;                          //标记i已被访问过
      if (link[i]=0)or find(link[i]) then  
//如果i无匹配或原来的匹配点link[i]可以另找一个匹配
      begin
        link[i]:=v;                        //让v-i与i匹配
        find:=true;                        //匹配成功并返回
        exit;
      end;
  end;
  find:=false;                            //匹配不成功
end;
 
begin
  //read the graph into array g[][]
  for i:=1 to n do
  begin
    fillchar(y,sizeof(y),0);
    if find(i) then inc(ans);
  end;
  //now then ans stores the max match
end.
```




## 博弈

**取对称状态**

Alice and Bob decide to play a funny game. At the beginning of the game they pick n(1 <= n <= 106) coins in a circle, as Figure 1 shows. A move consists in removing one or two adjacent coins, leaving all other coins untouched. At
 least one coin must be removed. Players alternate moves with Alice starting. The player that removes the last coin wins. (The last player to move wins. If you can't move, you lose.) 
如果n<=2那么先手的会获得胜利，当n>=3时，先手的走了一步以后，后手的可以把这个一个大图分成两个完全相同的小图，每步都是如此，则在n步以后，先手的总会无棋可取，后手的获得胜利。





```cpp
var t,n,i:longint;
begin
    while true do
        begin
            readln(n);
            if n=0 then halt;
            if n<=2 then writeln('Alice')
            else writeln('Bob');
        end;
end.
```

**取NIM值**

Here is a simple game. In this game, there are several piles of matches and two players. The two player play in turn. In each turn, one can choose a pile and take away arbitrary number of matches from the pile (Of course the number of matches, which is taken
 away, cannot be zero and cannot be larger than the number of matches in the chosen pile). If after a player’s turn, there is no match left, the player is the winner. Suppose that the two players are all very clear. Your job is to tell whether the player who
 plays first can win the game or not.

　　随机博弈指的是这样的一个博弈游戏，目前有任意堆石子，每堆石子个数也是任意的，双方轮流从中取出石子，规则如下：1)每一步应取走至少一枚石子；每一步只能从某一堆中取走部分或全部石子；2)如果谁取到最后一枚石子就胜。也就是尼姆博弈（Nimm Game）。必败局面：也叫奇异局势。无论做出何出操作，最终结果都是输的局面。必败局面经过2次操作后，可以达到另一个必败局面。必胜局面：经过1次操作后可以达到必败局面。即当前局面不是必败局面就是必胜局面，而必胜局面可以一步转变成必败局面。

　　最终状态：

　（1）最后剩下一堆石子；（必胜局面）

　（2）剩下两堆，每堆一个；（必败局面）

   （3）当石子剩下两堆，其中一堆只剩下1颗，另一堆剩下多于n颗石子时，当前取的人只需将多于1颗的那一堆取出n-1颗，则局面变为刚才提到的必败局面。（必胜局面）

判断当前局势是否为必胜（必败）局势：

　　　1）把所有堆的石子数目用二进制数表示出来，当全部这些数按位异或结果为0时当前局面为必败局面，否则为必胜局面；

　　　2）在必胜局面下，因为所有数按位异或的结果

               是大于零的，那么通过一次取，将这个（大于其它所有数按位异或的结果的）数下降到其它所有数按位异或的结果，这时局面就变为必败局面了。


定理：一组自然数中必然存在一个数，它大于等于其它所有数按位异或的结果。

证明：原命题等价于，设a1＾a2＾... ＾an=p，p≠0时，必存在k，使得ak＾p<ak< span="">（当p=0时，对于任意的k，有ak＾p=ak）。

设p的最高位是第q位，则至少存在一个k，使得ak的第q位也是1，而ak＾p的第q位为0，所以ak^p<ak

补缀一点，（a＾b）＾b=a＾（b＾b）=a＾0=a，所以ak＾p相当于“其它所有数按位异或的结果”。





```cpp
var x,y,n:int64;i:longint;
procedure main;
begin
    y:=0;
    read(n);
    for i:=1 to n do
        begin
            read(x);
            y:=y xor x;
        end;
    if y=0 then writeln('No')
    else writeln('Yes');
end;
begin
    while not seekeof do
        main;
end.
```




## 分治



二分答案、二分查找

## **回溯**

八皇后问题，汉汉诺问题

**高精度**

加，减，乘，除，阶乘

**高级数据结构**


二叉树



```cpp
//二叉树的先序遍历
type note=record
     father,lch,rch:longint;end;
var a:array[1..100] of note;n,j,t,m:longint;s:longint;
procedure duru;
var i:longint;f,l,r:longint;
begin
 read(n);
 for i:=1 to n do
  begin
  readln(f,l,r);
  a[f].lch:=l;{f的左孩子为l}
  a[f].rch:=r;{f的右孩子为r}
  if l<>0 then a[l].father:=f;{如果有左孩子那么左孩子的父亲是f}
  if r<>0 then a[r].father:=f;{如果有右孩子那么右孩子的父亲是f}
  end;
end;{输入的过程，第一个是父亲，后两个为左右孩子，0表示空}
function root:longint;
var i:longint;
begin
 for i:=1 to n do{i表示结点}
  if a[i].father=0 then
begin
 root:=i;exit;{如果这个结点的父亲为零，则这个结点就是根}
end;
end;{找根的程序}
procedure xiangen(t:longint);
begin
  if t<>0 then
   begin
    write(t,' ');{先根}
    xiangen(a[t].lch);{再对左子树先根遍历，左孩子当根}
    xiangen(a[t].rch);{再对右子树先根遍历，后孩子当根}
  end;
end;{进行先序遍历并输出的程序}
begin
 duru;m:=root;
 writeln(m);xiangen(m);
end.
 
 
//二叉树的中序遍历
procedure zhonggen(t:longint);
begin
  if t<>0 then
   begin
    write(t,' ');
    zhonggen(a[t].lch);
    zhonggen(a[t].rch);
  end;
end;
 
 
//二叉树的后序遍历
procedure hougen(t:longint);
begin
  if t<>0 then
   begin
    hougen(a[t].lch);
    hougen(a[t].rch);
    write(t,' ')
  end;
end；
 
 
//已知二叉树的前序遍历与中序遍历求后序遍历
var s1,s2:string;
procedure work(s1,s2:string);
var b,a:longint;{b,a的值在不断改变，必须放在局部变量里}
begin
  if s1<>'' then
   begin
    b:=length(s1);
    a:=pos(s1[1],s2);
    work(copy(s1,2,a-1),copy(s2,1,a-1));
    work(copy(s1,a+1,b-a),copy(s2,a+1,b-a));
    write(s1[1]);
   end;
end;
begin
  readln(s1);readln(s2);
  work(s1,s2);
end.
```




并查集



```cpp
{已按秩优化}
a:array[1..max] of longint;//a[i]表示i的前驱
function find(i:longint):longint;//非递归算法找i的根
var j,k,t:longint;
begin
       j:=i;
       while a[j]<>0 do j:=a[j]; // 顺着i向上找根
       find:=j;
       k:=i; //从i开始对子树结点进行路径压缩
       while a[k]<>0 do begin t:=k;k:=a[k];a[t]:=j;end;
end; 
function root(i:longint):longint;//递归算法找i的根
var j,k,t:longint;
begin
       if a[i]=0 then exit(i); //若i为根，返回本身结点序号
       root:=root(a[i]); //否则继续向上找
       a[i]:=root;
end;
procedure union(x,y:longint);//合并
begin
  if root[x]=root[y] then
   else f[y]:=x;
end;
```


树状数组





```cpp
{求2^k的代码：}
function  lowbit(t:longint):longint;
begin
    lowbit:=t and(t xor (t-1));
end;
 
{当想要查询一个SUM(n)(求A[1]..A[n]的和)，可以依据如下算法即可：
step1:令sum = 0，转第二步；
step2:假如n <= 0，算法结束，返回sum值，否则sum = sum + Cn，转第三步；
step3:令n = n–lowbit(n)，转第二步。}
function getsum(k:integer):integer;
var t:integer;
begin
    t:=0;
    while k>0 do
        begin
            t:=t+c[k];     
            k:=k-lowbit(k);
        end;
    getsum:=t;
end;
 
{修改算法如下（给某个结点i加上x）：
step1: 当i > n时，算法结束，否则转第二步；
step2: Ci = Ci + x， i = i + lowbit(i)转第一步。
i = i +lowbit(i)这个过程实际上也只是一个把末尾1补为0的过程。}
procedure add(i:longint;x:longint);
begin
    while i<=n do
        begin
            c[i]:=C[i]+x;
            i:=i+lowbit(i);
        end;
end;
```


线段树





```cpp
type xtree=record
    a,b,left,right,cover:longint;
    end;
//a,b是区间左右端点，当a=b表示一个叶子节点；left,right表示左右儿子，cover表示是否被覆盖
 
var tree:array[1..1000] of xtree;
    c,d,number,tot:longint;
    //tot为一共有多少个结点
 
procedure make(a,b:longint);
var now:longint;//now必须为局部变量,dfs中枚举变量一样的道理
begin
    inc(tot);
    now:=tot;
    tree[now].a:=a;
    tree[now].b:=b;
    tree[now].cover:=0;
    if a+1<b then
        begin
            tree[now].left:=tot+1;
            make(a,(a+b)div 2);
            tree[now].right:=tot+1;
            //若now为全局变量，建左子树会修改now的值，导致此处建树不正确
            make((a+b) div 2,b);
        end;
end;
//建树过程
 
procedure insert(num:longint);
begin
    if (c<=tree[num].a)and(tree[num].b<=d) then
        inc(tree[num].cover)
    //若当前区间被[c,d]覆盖，则cover+1
    else
        begin
            if c<(tree[num].a+tree[num].b)div 2 then insert(tree[num].left);
            if d>(tree[num].a+tree[num].b)div 2 then insert(tree[num].right);
        end;
    //否则将区间[c,d]插到左右子树中
end;
//插入线段[c,d](c,d为全局变量)到线段树第num个节点区间
 
procedure delete(num:longint);
begin
    if (c<=tree[num].a)and(tree[num].b<=d) then
        dec(tree[num].cover)
    //若当前区间被[c,d]覆盖，则cover-1
    else
        begin
            if c<(tree[num].a+tree[num].b)div 2 then delete(tree[num].left);
            if d>(tree[num].a+tree[num].b)div 2 then delete(tree[num].right);
        end;
    //否则将区间[c,d]在左右子树中删除
end;
//在线段树第num个节点区间中删除线段[c,d]
 
procedure count(num:longint);
begin
    if tree[num].cover>0 then
        number:=number+(tree[num].b-tree[num].a)
    //若当前区间被完全覆盖，则累加到number全局变量中
    else
        begin
            if tree[num].left>0 then count(tree[num].left);
            if tree[num].right>0 then count(tree[num].right);
        end;
     //否则，若为部分覆盖，则累加左右子树的测度
end;
//计算整个线段树的测度(被覆盖区间的总长度)
 
begin
    readln(c,d);
    make(c,d);
    readln(c,d);
    insert(1);//插入线段[c,d]；
    readln(c,d);
    delete(1);//删除线段[c,d]
    count(1);//计算线段树的测度
    writeln(number);
end.
```


二叉搜索树





```cpp
{treap示范代码：
标准的代码缩进风格，优美的算法实现。
经典标程，完全掌握后水平会提高很多
不改变bst的性质(在bst所有子树中均满足：左子树的所有节点<=根<=右子树的所有节点)
通过旋转操作，使根的hr最小(即所有的hr构成堆的关系)}
var
  //l[i],r[i],v[i]:i号结点的左儿子、右儿子，关键值
  //hr[i]:i号节点的优先值(treap所有子树中,根的hr必须是最小的)
  //s[i]:i号节点为根的子树节点总数
  l,r,hr,s,v:array[0..2000000]of longint;
  n,root,m:longint;
   
procedure init;//初始化
begin
  readln(n);
  m:=0;
  //randomize;  //考试要求程序每次运行结果必须一致,慎用。确实要用：randomize 100;
  fillchar(s,sizeof(s),0);
  fillchar(l,sizeof(l),0);
  fillchar(r,sizeof(r),0);
  root:=0;
end;
 
//旋转是平衡二叉树的精髓，它不会改变bst的性质(左子树<=根<=右子树)
//左旋使树的左子树深度+1,右子树深度-1
//右旋使树的右子树深度+1,左子树深度-1
procedure l_rotate(var x:longint);inline;//左旋以x为根的子树（注意var参数及意义）
var
  y:longint;
begin
  y:=r[x];    //保存x的右儿子到y中
  r[x]:=l[y]; //将y的左儿子作为x的右儿子
  l[y]:=x;    //x作为y的左儿子
  s[y]:=s[x]; //维护旋转后的子树大小
  s[x]:=s[l[x]]+s[r[x]]+1;
  x:=y;   //y为根
end;
 
procedure r_rotate(var x:longint);inline;//右旋以x为根的子树
var
  y:longint;
begin
  y:=l[x];
  l[x]:=r[y];
  r[y]:=x;
  s[y]:=s[x];
  s[x]:=s[l[x]]+s[r[x]]+1;
  x:=y;
end;
 
//插入（递归，if key<=root,则插入到左子树，否则到右子树，直到尽头再新建节点）
procedure insert(var k,key:longint);inline;
begin
  if k=0 then//已到尽头，新建节点并写入key及随机值hr
    begin
      inc(m);
      v[m]:=key;
      s[m]:=1;
      hr[m]:=random(maxlongint);
      k:=m;//修改k,使父节点指向当前节点(修改前从父节点指向0)
      exit;
    end;
  inc(s[k]);
  if key<=v[k] then//若key<=根则插入到左子树，否则到右子树
    begin
      insert(l[k],key);//若l[k]=0,到下层则新建节点并修改l[k]=m
      if hr[l[k]]>hr[k] then  //旋转
        r_rotate(k);
      exit;
    end;
  if key>v[k] then
    begin
      insert(r[k],key);
      if hr[r[k]]>hr[k] then
        l_rotate(k);
      exit;
    end;
end;
 
{删除:在k号节点为根的子树中删除key
基本方法：由于是静态结构，为了提高效率，并没真正删除
若找到则删除，若没找到，则删除查找尽头的节点
主程序中需判断返回值，若不等于key,重新插入key即可
找到后的处理：
    若为叶节点，直接删除，否则，将要删除的节点左子树的最右节点（思考：为什么？）代替它
}
function delete(var k:longint;key:longint):longint;inline;
begin
  dec(s[k]);//维护节点总数
  //如果找到，或已到尽头
  if (key=v[k])or(l[k]=0)and(key<=v[k])or(r[k]=0)and(key>v[k]) then
    begin
      delete:=v[k];//返回要删除的节点（不一定=key）
      if (l[k]=0)or(r[k]=0) then //若左右子树只有一个，则让儿子代替根即可
        begin
          k:=l[k]+r[k];//用儿子替换当前要删除的节点
          exit;
        end;
      v[k]:=delete(l[k],key+1);//k左右子树都有，则用左子树的最右节点替换k
      exit;
    end;
  if key<=v[k] then//若k<=v[k],则在左子树中删，否则，在右子树中删
    exit(delete(l[k],key));
  if key>v[k] then
    exit(delete(r[k],key));
end;
 
function find(var k,key:longint):boolean;inline;//查找
begin
   if k=0 then//递归边界
      exit(false);
 
   if key>v[k] then
      find:=find(r[k],key)
   else
      find:=(v[k]=key)or find(l[k],key);
end;
 
//key的排名(key排在第几，按从小到大的顺序)
function rank(var t,key:longint):longint;inline;
begin
   if t=0 then
      exit(1);
   if key<=v[t] then
      rank:=rank(l[t],key)
   else
      rank:=s[l[t]]+1+rank(r[t],key);
end;
 
function select(var t:longint;k:longint):longint;inline;//选择排在第k位的数
begin
   if k=s[l[t]]+1 then//若找到第k位的节点，则返回节点key值
      exit(v[t]);
 
   if k<=s[l[t]] then//递归
      select:=select(l[t],k)
   else
      select:=select(r[t],k-1-s[l[t]]);
end;
 
function pred(var t,key:longint):longint;inline;//找key的前趋
begin
   if t=0 then
      exit(key);
   if key<=v[t] then//key<=根，原问题等价于在左子树中找key
      pred:=pred(l[t],key)
   else begin  //key>根，原问题等价于在右子树中找key，但右儿子返回时，要判断你是不是key的前趋
      pred:=pred(r[t],key);//右子树的返回值
      if pred=key then   //如果右子树的返回值=key说明在右子树中没找到key的前趋
         pred:=v[t];     //右子树没有key的前趋，那你就是了。
   end;
end;
 
function succ(var t,key:longint):longint;inline;//找key的后继
begin
   if t=0 then
      exit(key);
   if v[t]<=key then
      succ:=succ(r[t],key)
   else begin
      succ:=succ(l[t],key);
      if succ=key then
         succ:=v[t];
   end;
end;
 
procedure order;inline;//操作解释和执行
var
  a,b:longint;
begin
  readln(a,b);
  case a of
     1:insert(root,b);
     2:delete(root,b);
     3:writeln(find(root,b));
     4:writeln(rank(root,b));
     5:writeln(select(root,b));
     6:writeln(pred(root,b));
     7:writeln(succ(root,b));
   end;
end;
 
procedure main;inline;//主模块
var
  i:longint;
begin
  init;
  for i:=1 to n do
    order;
end;
 
begin//主程序
  main;
end.
```

**进制转换**
## **后记**

本文未涉及的内容包括但不限于：
哈希（HASH）
强连通分量
离散化
尺取法
网络流
计算几何（包括凸包）
2-SAT
LCA
倍增法
KMP算法
后缀数组
Grundy数
trie树
AC自动机
FFT快速傅里叶变换
斯坦纳树
单纯形法
矩阵
三分搜索
α-β剪枝
离散对数
微积分
树链剖分
负进制转换
群论
…



不过没有关系，这些基本都属于NOIP涉及不到的范围，学无止境，把握好当下的才是关键！
参考文献

 百度百科
《数学》第三册 苏州大学出版社
《动态规划学案》 山东省实验中学 王乃广
《PASCAL省选模板》island
《NOIP2012最终征途》亟隐 Slack
《自然数的因数》桃花岛主
《二分图的最大匹配、完美匹配和匈牙利算法》pi9nc

**  推荐文章&书籍&博客&网站**

  刘汝佳《算法竞赛入门经典1&2》
  啊哈磊《啊哈！算法》
  Thomas H.Cormen等 《算法导论》
  编程之美小组《编程之美》
  秋叶拓哉等《挑战程序设计竞赛》
  Charles Petzold 《CODE》
  顾森《思考的乐趣》
 《NOI导刊》（一本杂志，已经停办，不过可以找来以前的看看）
 《骗分导论》
 《人品导论》
  MATRIX67
  HZWER
  枫伶忆《大话二进制，八进制，十进制，十六进制之间的转换》
  免费程序员图书下载（http://it-ebooks.info/）
  快速查询数列(http://oeis.org/)
  在线学习：计蒜客
  在线学习：萌码
  使用技巧：PDF自由转换(http://smallpdf.com/cn)

PS：其实在竞赛之路上帮助最大的还是大牛神犇们的博客，从中可以学到很多的东西。我也养成了写博客的习惯，做完题写写博客加加注释，整理思路的同时还能助人为乐。写博客推荐去博客园或是CSDN社区，这是专门面向程序猿的博客，支持自己设置板式，代码高亮。因为有群体限制，所以交流起来也更方便。







