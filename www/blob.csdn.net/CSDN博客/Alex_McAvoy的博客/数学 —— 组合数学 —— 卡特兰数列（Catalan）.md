# 数学 —— 组合数学 —— 卡特兰数列（Catalan） - Alex_McAvoy的博客 - CSDN博客





2018年04月24日 22:42:58[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：246








# 【概述】

卡特兰数列是组合数学中一个常出现在各种计数问题中出现的数列，其前几项为 : 1, 1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786, 208012, ......

卡特兰数首先是由欧拉在计算对凸 n 边形的不同的对角三角形剖分的个数问题时得到的。 

问题：在一个凸 n 边形中，通过不相交于 n 边形内部的对角线，把 n 边形拆分成若干三角形，不同的拆分数目用 Hn 表示，Hn即为卡特兰数。

例如，五边形有如下五种拆分方案，故H5=5。

![](https://img-blog.csdn.net/20180802221052308?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180424221631800)

# 【公式】

1.递归公式 1

![f(n)=\sum_{i=0}^{n-1}f(i)*f(n-i-1)](https://private.codecogs.com/gif.latex?f%28n%29%3D%5Csum_%7Bi%3D0%7D%5E%7Bn-1%7Df%28i%29*f%28n-i-1%29)

2.递归公式 2

![f(n)=f(n-1)*(4*n-2)/(n+1)](https://private.codecogs.com/gif.latex?f%28n%29%3Df%28n-1%29*%284*n-2%29/%28n&plus;1%29)

3.组合公式 1

![f(n)=C_{2n}^n/(n+1)](https://private.codecogs.com/gif.latex?f%28n%29%3DC_%7B2n%7D%5En/%28n&plus;1%29)

4.组合公式 2

![f(n)=C_{2n}^n-C^{n-1}_{2n}](https://private.codecogs.com/gif.latex?f%28n%29%3DC_%7B2n%7D%5En-C%5E%7Bn-1%7D_%7B2n%7D)

# 【应用】
- 二叉树的计数：已知二叉树有 n 个结点，求能构成多少种不同的二叉树
- 括号化问题：一个合法的表达式由()包围，()可以嵌套和连接，如：(())()也是合法表达式，现给出 n 对括号，求可以组成的合法表达式的个数
- 划分问题：将一个凸 n+2 多边形区域分成三角形区域的方法数
- 出栈问题：一个栈的进栈序列为1,2,3,..n，求不同的出栈序列有多少种
- 路径问题：在 n*n 的方格地图中，从一个角到另外一个角，求不跨越对角线的路径数有多少种
- 握手问题：2n 个人均匀坐在一个圆桌边上，某个时刻所有人同时与另一个人握手，要求手之间不能交叉，求共有多少种握手方法

# 【算法实现】

```cpp
/*
	函数功能:计算Catalan的第n项  
	函数参数:n为项数  
	返回值:第n个Catalan数
*/
int Catalan(int n)  
{  
    if(n<=1)  
        return 1;  
  
    int *H=new int [n+1]; //保存临时结果  
    H[0]=H[1]=1;//H(0)和H(1)  
    
    for(int i=2;i<=n;i++)//依次计算H(2),H(3)...H(n)  
    {  
        H[i]=0;  
        for(int j=0;j<i;j++)//根据递归式计算H(i)=Hh(0)*H(i-1)+H(1)*H(i-2)+...+H(i-1)H(0)  
            H[i]+=(H[j]*H[i-1-j]);  
    }  
    
    int result=H[n];//保存结果  
    delete [] H;//释放空间  
    return result;
}
```



