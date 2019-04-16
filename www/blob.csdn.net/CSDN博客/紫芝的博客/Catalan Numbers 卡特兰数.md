# Catalan Numbers 卡特兰数 - 紫芝的博客 - CSDN博客





2018年09月23日 11:58:15[紫芝](https://me.csdn.net/qq_40507857)阅读数：62
个人分类：[组合数学](https://blog.csdn.net/qq_40507857/article/category/7984865)









卡特兰数源于**组合数学**，递推式为 **H【1】 = 1；H【n】 = H【n-1】*（4*n-2）/（n+1）{n>=2}**；

卡塔兰数的渐近增长为

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171005140941493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3VfdG9uZ3Rvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面给出几个求卡特兰数的公式，用h(n)表示卡特兰数的第n项，其中  **h(0)=1，h(1)=1 **

**公式一：**h(n)= h(0)*h(n-1)+h(1)*h(n-2) + ... + h(n-1)*h(0) (n>=2)

**公式二：**h(n)=h(n-1)*(4*n-2)/(n+1)

**公式三：**h(n)=C(2n,n)/(n+1) (n=0,1,2,...)

**公式四：**h(n)=c(2n,n)-c(2n,n-1)(n=0,1,2,...) 

其前几项为 :
`1, 2, 5, 14, 42, 132, 429, 1430, 4862, 16796, 58786, 208012, 742900, 2674440, 9694845, 35357670, 129644790, 477638700, 1767263190, 6564120420, 24466267020, 91482563640, 343059613650, 1289904147324, 4861946401452, ...`
主要应用：

1.括号化问题

矩阵链乘：![p=a_{1}*a_{2}*a_{3}*a_{4}*a_{5}*....*a_{n-1}*a_{n}](https://private.codecogs.com/gif.latex?p%3Da_%7B1%7D*a_%7B2%7D*a_%7B3%7D*a_%7B4%7D*a_%7B5%7D*....*a_%7Bn-1%7D*a_%7Bn%7D),根据乘法结合律，不改变顺序，只用括号表示对的乘积，有多少种括号化的方案？h（n）种

2.出栈次序问题：

一个栈（无穷大）的进栈次序为1，2，3，4，5，6.........n,有多少个出栈次序？

**HDU1023**

类似：

（1）有2n个人排成一行进入剧场，入场费5元，其中只有n个人有一张5元，另外n个人只有10元，剧院无其他钞票，问有多少种方法使得只有10元的人买票（售票处只有5元找零）？

将持5元者到达视作将5元入栈，持10元者到达视作栈中某5元出栈

**[HDU4165](http://acm.hdu.edu.cn/showproblem.php?pid=4165)**

（2）在圆上选择2n个点，将这些点成对连接起来，使得所得到的n条线段不相交的方法数？h（n）种

**POJ2084**

3.将多边形划分为三角形问题，将一个凸多边形区域分成三角形区域的方法数？

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171005151444802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3VfdG9uZ3Rvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

类似：

（1）一位律师在他住所以北n个街区和以东n个街区处工作，每天他走2n个街区上班，如果不穿越（但可以碰到）从家到办公室的对角线，那么有多少条可能的道路？**ans=2*h(n)**

**HDU2067**

![](https://gss1.bdstatic.com/9vo3dSag_xI4khGkpoWK1HF6hhy/baike/c0%3Dbaike80%2C5%2C5%2C80%2C26/sign=1cfe77909d0a304e462fa8a8b0a1cce3/4d086e061d950a7be3e94d5000d162d9f3d3c942.jpg)

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171005151358578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3VfdG9uZ3Rvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

4.给n个节点，能构成多少种形状不同的二叉树？

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20171005150439375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd3VfdG9uZ3Rvbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

先去掉一个点作为顶点，左边依次可以取0至N-1个，右边是N-1到0个，两两配对相乘，就是

h(0)*h(n-1)+h(2)*h(n-2)+......+h(n-1)*h(0)=h(n)

```java
import java.util.*;
import java.math.*;
public class Main {
	public static void main(String[] args) {
		BigInteger[] arr = new BigInteger[107];
		arr[1]=BigInteger.ONE;
		for(int i=2;i<=100;i++) {
			arr[i] = arr[i-1].multiply(BigInteger.valueOf(4*i-2)).divide(BigInteger.valueOf(i+1));
		}
		Scanner cin=new Scanner(System.in);
		while(cin.hasNext()) {
			int n=cin.nextInt();
			if(n==-1)
				break;
			 System.out.println(arr[n]);
		}
		cin.close();
	}
}
```

### HDU3723

A delta wave is a high amplitude brain wave in humans with a frequency of 1 – 4 hertz which can be recorded with an electroencephalogram (EEG) and is usually associated with slow-wave sleep (SWS). 

-- from Wikipedia 


The researchers have discovered a new kind of species called "otaku", whose brain waves are rather strange. The delta wave of an otaku's brain can be approximated by a polygonal line in the 2D coordinate system. The line is a route from point (0, 0) to (N, 0), and it is allowed to move only to the right (up, down or straight) at every step. And during the whole moving, it is not allowed to dip below the y = 0 axis. 


For example, there are the 9 kinds of delta waves for N = 4: 

![](https://odzkskevi.qnssl.com/e83af7a43bc0fc38c115a37fa757db58?v=1537756291)

Given N, you are requested to find out how many kinds of different delta waves of otaku.

**Input**

There are no more than 20 test cases. There is only one line for each case, containing an integer N (2 < N <= 10000) 

**Output**

Output one line for each test case. For the answer may be quite huge, you need only output the answer module 10 100.

Sample Input

3
4
Sample Output

4
9
### 题意：

在第一象限，从（0，0）到（n，0）点，每次y值的差值最大为1，要么加1（向上走），要么减1（向下走），要么不变（水平），问有多少种走法？

### 分析：

起点和终点在一条直线上，所以向上 i 次，就要向下 i 次，且（0<=i<n/2, i+i <=n），而上下的次序不固定，显然就是 i 个元素进栈出栈的次序，方案数为  ![h(i)= \frac{C(2n,n)}{n+1}](https://private.codecogs.com/gif.latex?h%28i%29%3D%20%5Cfrac%7BC%282n%2Cn%29%7D%7Bn&plus;1%7D)  (公式三）；

从n个元素中选出2*i个向上走或者向下走，n-2*i 个元素水平走，方案数为 ![C(n,2i)](https://private.codecogs.com/gif.latex?C%28n%2C2i%29) ;

向上0次，向下0次，水平直线走，方案数为 ![a[0]=1](https://private.codecogs.com/gif.latex?a%5B0%5D%3D1) ;

向上 i 次，向下 i 次，方案数 ![a[i]=h(i)*C(n,2i)=\frac{C(2i,i)}{i+1}*C(n,2i)=\frac{n!}{(i+1)*(n-2i)!*i!*i!}](https://private.codecogs.com/gif.latex?a%5Bi%5D%3Dh%28i%29*C%28n%2C2i%29%3D%5Cfrac%7BC%282i%2Ci%29%7D%7Bi&plus;1%7D*C%28n%2C2i%29%3D%5Cfrac%7Bn%21%7D%7B%28i&plus;1%29*%28n-2i%29%21*i%21*i%21%7D)

向上i+1次，向下i+1次，方案数 ![a[i+1]=\frac{n!}{(i+2)*(n-2i-2)!*(i+1)!*(i+1)!}](https://private.codecogs.com/gif.latex?a%5Bi&plus;1%5D%3D%5Cfrac%7Bn%21%7D%7B%28i&plus;2%29*%28n-2i-2%29%21*%28i&plus;1%29%21*%28i&plus;1%29%21%7D)

递推式：![a[i+1]=a[i]*\frac{(n-2i)*(n-2i-1)}{(i+2)*(i+1)}](https://private.codecogs.com/gif.latex?a%5Bi&plus;1%5D%3Da%5Bi%5D*%5Cfrac%7B%28n-2i%29*%28n-2i-1%29%7D%7B%28i&plus;2%29*%28i&plus;1%29%7D)



```java
import java.math.*;
import java.util.*;

public class Main{
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		
		while(cin.hasNext()) {
			BigInteger ans=BigInteger.ONE;
			BigInteger tmp=BigInteger.ONE;
			int n=cin.nextInt();
			for(int i=0;i<=n/2;i++) {
    tmp=tmp.multiply(BigInteger.valueOf(n-2*i)).multiply(BigInteger.valueOf(n-2*i-1)).divide(BigInteger.valueOf(i+2)).divide(BigInteger.valueOf(i+1));
				ans=ans.add(tmp);
			}
			ans=ans.mod(BigInteger.TEN.pow(100));//mod 10^100
			System.out.println(ans);
		}
		cin.close();
	}
}
```





