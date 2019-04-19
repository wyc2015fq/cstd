# 剑指offer——数值的整数次方 - 刘炫320的博客 - CSDN博客
2017年04月15日 19:11:58[刘炫320](https://me.csdn.net/qq_35082030)阅读数：358
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1.问题描述
> 
给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。
# 2.解决方法
# 2.1 level1
这道题，首先的第一反应就是使用最基础的，把指数运算换成连乘运算，然后通过编码实现，这个不难。
```java
public static double Power( double base, int exponent) {
        //如果为负数，则要先倒数，再连乘
        if(exponent<0){
            base=1/base;
            exponent=-1*exponent;
        }
        double res=1;
        for(int i=0;i<exponent;i++){
            res*=base;
        }
        return res;
    }
```
# 2.2 level2
但是如果仅仅是第一层，那么这个题目就没有什么上升空间了。当然我们要想更好的解决办法，那就是分治法。二分治是非常常见的分治方法。代码如下：
```java
public static double Power3(double base, int exponent){
            int n = Math.abs(exponent);
            double result = 0.0;
            if (n == 0)
                return 1.0;
            if (n == 1)
                return base;
            result = Power3(base, n >> 1);
            result *= result;
            if ((n & 1) == 1)
                // 如果指数n为奇数，则要再乘一次底数base            
                result *= base;
            if (exponent < 0) 
                // 如果指数为负数，则应该求result的倒数            
                result = 1 / result;
            return result;
    }
```
这个算法的精髓就在于使用位运算来解决偶数和奇数的二分之间的差异。
# 2.3 level3
当然，任何一个算法最好使用java自带的类库来实现，确实是很高明的方法，我们这题也有这样一个捷径可走。
```java
public static double Power(double base, int exponent){
        return Math.pow(base, exponent);
    }
```
# 3. 算法分析
## 3.1 就事论事
从时间复杂度的层面，level1算法为O（n），基本上在可接受的范围内。而level2算法为O（logn）这个算法就已经比较优秀了。而java自己的这个算法，应该是非常快速的。为了探究最终的代码实现，我还是又翻了一遍java的相关源码,最终找到了StrictMath.pow()方法，但是这个方法不是用java写的,即使在源码里，也只有这样一个声明：
```java
public static native double pow(double a, double b);
```
这个native修饰符，大家可以看[native修饰符](http://blog.csdn.net/wike163/article/details/6635321)。也就是说，StrictMath.pow是个本地方法，是靠底层代码实现。所以它应该是惊人的速度实现了这个运算，如何惊人的呢？
据我自己测试可以得知，level1和level2的差距大概是2个数量级，而level3本来应该是最快的，可是通过包装了以后反而处在了level1和level2之间了。后来我把这个Math.pow()放到了主函数中，才知道，这个速度在第一次调用的时候只需要400ns的时间，而此后调用，竟然不需要时间！
## 3.2 思维层面
我们也做了不少的算法了，主要有这么几个方面，首先要能做出来（Level1）,其次是做的漂亮。而这则需要我们对经典算法相当熟悉才行，例如分治法，减治法和动态规划法等。另外，如果题目没有要求，适当的使用库函数是一个非常不错的选择。
