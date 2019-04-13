
# 剑指offer——数值的整数次方 - manduner的博客 - CSDN博客


2019年03月14日 22:30:13[Manduner_TJU](https://me.csdn.net/manduner)阅读数：22


# 1，问题描述
给定一个double类型的浮点数base和int类型的整数exponent。求base的exponent次方。
# 2，解题思路
利用快速幂的解法，快速幂公式如下：
![](https://img-blog.csdnimg.cn/20190314222222591.png)
该方法体现的就是递归的思想，源码如下：
# 3，源码
```python
public class Solution {
    public double Power(double base, int exponent) {
        double result = 0.0;
        //1,先讨论exponent的特殊情况：
            //exponent==0,exponent==1,exponent==-1
        //2，再针对exponent >1 和 exponent < -1的情况进行分析
            //其中，exponent<-1时，base的exponent次方就等于(1.0/base)^(-exponent)
        if(exponent==0){
            result = 1.0;
        }else if(exponent==1){
            result = base;
        }else if(exponent==-1){
            result = 1.0/base;
        }else if(exponent > 1){
            result = powerWithExponent(base,exponent);
        }else if(exponent < -1){
            base = 1.0/base;
            result = powerWithExponent(base,-exponent);
        }
        return result;
    }
    
    public double powerWithExponent(double base, int exponent){
        if(exponent%2==0){
            base = Power(base,exponent/2);
            base *= base;
        }else{
            double temp = base;
            base = Power(base,(exponent-1)/2);
            base = base*base*temp;
        }
        return base;
    }
  }
```


