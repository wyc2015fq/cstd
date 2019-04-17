# Lintcode——845. Greatest Common Divisor - westbrook1998的博客 - CSDN博客





2018年04月04日 21:55:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：88标签：[Lintcode																[算法																[最大公因数](https://so.csdn.net/so/search/s.do?q=最大公因数&t=blog)
个人分类：[Lintcode](https://blog.csdn.net/westbrook1998/article/category/7459920)





> 
求两个数的最大公因数


代码：

```java
public class Solution {
    /**
     * @param a: the given number
     * @param b: another number
     * @return: the greatest common divisor of two numbers
     */
    public int gcd(int a, int b) {
        // write your code here
        if(b==0){
            return a;
        }
        return gcd(b,a%b);
    }
}
```

水题，但是基础算法要好好掌握](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=Lintcode&t=blog)




