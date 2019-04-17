# hdu2196-树形dp - 安得广厦千万间的博客 - CSDN博客





2018年05月26日 00:29:43[独-](https://me.csdn.net/qq_41713256)阅读数：49








题目网址：http://hdu.hustoj.com/showproblem.php?pid=2196

题目大意：给你一个树形结构，每两个点有一定的距离，求每一个点能到达的最远距离。

首先要建立无向树，然后用两次dfs。

dp【n】【0】表示该节点往子节点能到达的最远距离

dp【n】【1】表示该节点往子节点能到达的第二远的距离


这两组数据要说明一下

![](https://img-blog.csdn.net/2018052600124155)


如上图所示 dp【p】【0】很显然等于8，但是dp【p】【1】并不等于7而是2；

也就是说dp【p】【0】和dp【p】【1】是往不同的方向走的，这样才能求出正确的dp【p】【2】。




dp【n】【2】表示该节点往父节点能到达的最远距离


一开始的时候觉得第二组数据没什么用，但是后来一想再求第三组数据的时候需要用到

还是同一幅图

![](https://img-blog.csdn.net/2018052600124155)


我们在计算dp【u】【2】时，它是从父节点p来的，这是我们就要分情况讨论了

首先dp【p】【0】如果不是往u节点方向的话，那么很显然dp【u】【2】=2+dp【p】【0】；

但是不幸的是dp【p】【0】是往u方向走的，所以我们只能拿第二远的距离，也就是dp【u】【2】=2+dp【p】【1】；

综上所述  

if dp【p】【0】==dp【u】【0】+dis【p，u】，那么 dp【u】【2】=dp【p】【1】+dis【p，u】；

else   dp【u】【2】=dp【p】【0】+dis【p，u】；

最后节点v能到达的最远距离为  MAX ( dp【v】【0】,dp【v】【2】);

AC代码：


#include<cstdio>
#include<iostream>
#include<algorithm>
#include<vector> 
#include<set>
#include<string>
#include<cstring>
#include<cmath>
**using namespace** std**;struct** node**{    int** to**,**cost**;};**
vector**<**node**>**edge**[**10010**]; int** dp**[**10010**][**3**];void** dfs1**(int** v**,int** p**){    int** one**=**0**,**two**=**0**;    for(int** i**=**0**;**i**<**edge**[**v**].**size**();**i**++)    {        int** u**=**edge**[**v**][**i**].**to**;        int** w**=**edge**[**v**][**i**].**cost**;        if(**u**==**p**) continue;**
        dfs1**(**u**,**v**);        if(**one**<**dp**[**u**][**0**]+**w**)        {**
            two**=**one**;**
            one**=**dp**[**u**][**0**]+**w**;        }        else if(**two**<**dp**[**u**][**0**]+**w**)        {**
            two**=**dp**[**u**][**0**]+**w**;        }    }**
    dp**[**v**][**0**]=**one**;** dp**[**v**][**1**]=**two**;}void** dfs2**(int** v**,int** p**){    int** t**=**0**;    for(int** i**=**0**;**i**<**edge**[**v**].**size**();**i**++)    {        int** u**=**edge**[**v**][**i**].**to**;        int** w**=**edge**[**v**][**i**].**cost**;        if(**u**==**p**) continue;        if(**dp**[**v**][**0**]==**dp**[**u**][**0**]+**w**)** t**=**dp**[**v**][**1**];        else** t**=**dp**[**v**][**0**];**
        dp**[**u**][**2**]=**max**(**t**,**dp**[**v**][**2**])+**w**;**
        dfs2**(**u**,**v**);    }}int main(){    int** n**,**x**,**y**,**i**;    while(**cin**>>**n**)    {        for(**i**=**0**;**i**<=**n**;**i**++)** edge**[**i**].**clear**();        for(**i**=**2**;**i**<=**n**;**i**++)        {**
            cin**>>**x**>>**y**;**
            now**.**to**=**x**;** now**.**cost**=**y**;**
            edge**[**i**].**push_back**(**now**);**
            now**.**to**=**i**;**
            edge**[**x**].**push_back**(**now**);        }**
        dfs1**(**1**,**0**);**
        dp**[**1**][**2**]=**0**;**
        dfs2**(**1**,**0**);        for(**i**=**1**;**i**<=**n**;**i**++)** printf**(**"%d\n"**,**max**(**dp**[**i**][**0**],**dp**[**i**][**2**]));    }    return** 0**;}**






