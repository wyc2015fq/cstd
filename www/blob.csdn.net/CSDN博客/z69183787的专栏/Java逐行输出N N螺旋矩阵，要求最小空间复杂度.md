# Java逐行输出N*N螺旋矩阵，要求最小空间复杂度 - z69183787的专栏 - CSDN博客
2015年08月24日 15:53:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1405
今天去参加了一个笔试，有一个笔试题是逐行输出一个N*N螺旋矩阵，要求最小空间复杂度，比如4*4，则如下：
```java
1  2  3  4
12 13 14  5
11 16 15  6
10  9  8  7
```
[?](http://my.oschina.net/u/1382972/blog/221115#)
当时想了半天，最后终于想到个思路，不过也用了太长时间，没时间写具体代码了。而且笔试也是手写代码到纸上，这样代码没法实时改和看结果，而且水平有限，所以很讨厌这样的写代码方式。  回来路上想既然思路都有了，在电脑上应该很快能写出来，结果折腾了3个多小时才写好，感觉自己真是弱爆啊。。。对数字太不敏感，尤其是for循环是0还是1开头，到底包不包括边界，一复杂点儿就晕。。。
自己的思路是这样的：每圈的长度是可以算出来的，比如上面4*4的矩阵，最外圈长度是12，内圈的长度是4，所以可可以根据第i*j个元素处在那一圈计算其值大小。
最终代码如下：
```java
public class Matrix {
 
    public static void main(String[] args) {
        new Matrix().printMatrix(7);
    }
    public static String form = null;
    public void printMatrix(int dim) {
        form = "%"+(String.valueOf(dim*dim).length()+1) + "d";
        int max = (dim%2==0)?(dim/2):(dim/2+1);
        for(int i=1; i<=max; i++) {
            for(int j=1;j<i;j++) {
                print(getSize(j, dim)-(i-j)+1);
            }
            printTopLine(i, dim);
            for(int j=i-1;j>0;j--) {
                print(getSize(j-1, dim)+(dim-2*(j-1))+i-j);
            }
            System.out.println();
        }
        for(int i=max+1; i<=dim; i++) {
            for(int j=1;j<=dim-i;j++) {
                print(getSize(j, dim)-(i-j)+1);
            }
            printBottemLine(dim-i+1, dim);
            for(int j=dim-i;j>0;j--) {
                print(getSize(j-1, dim)+(dim-2*(j-1))+i-j);
            }
            System.out.println();
        }
         
    }
 
    public void print(int i) {
        System.out.format(form, i);
    }
    public int getSize(int level, int dim) {
        int size =0;
        for(int i=1;i<=level;i++) {
            size+=4*(dim-2*(i-1))-4;
        }
        return size;
    }
    public void printTopLine(int level, int dim) {
        int size =getSize(level-1, dim);
        for(int i=0; i<dim-2*(level-1);i++) {
            print(size+1+i);
        }
    }
    public void printBottemLine(int level, int dim) {
        int size =getSize(level, dim);
        int first = size+1;
        for(int i=0; i<dim-2*(level-1);i++) {
            print(first-(dim-2*(level-1))+1-i);
        }
    }
}
```
 输出：
```java
1  2  3  4  5  6  7
24 25 26 27 28 29  8
23 40 41 42 43 30  9
22 39 48 49 44 31 10
21 38 47 46 45 32 11
20 37 36 35 34 33 12
19 18 17 16 15 14 13
```
但感觉代码还是太复杂了，看着就晕，不知道谁有更简单的方法啊，网上简单搜了一些，没找到。。。
