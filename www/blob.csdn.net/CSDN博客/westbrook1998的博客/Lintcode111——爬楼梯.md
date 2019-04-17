# Lintcode111——爬楼梯 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:20:02[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：65








## 题目：

> 
假设你正在爬楼梯，需要n步你才能到达顶部。但每次你只能爬一步或者两步，你能有多少种不同的方法爬到楼顶部？  

样例 

  比如n=3，1+1+1=1+2=2+1=3，共有3种不同的方法 

  返回 3 

  经典的爬楼梯题目，简单的动态规划，从最后开始推    
## 题解：

```java
public class Solution {
    /**
     * @param n: An integer
     * @return: An integer
     */
    public int climbStairs(int n) {
        // write your code here
        int[] s=new int[n];
        if(0==n){
            return 0;
        }
        if(1==n){
            return 1;
        }
        if(2==n){
            return 2;
        }
        s[0]=1;
        s[1]=2;
        for(int i=2;i<n;i++){
            s[i]=s[i-1]+s[i-2];
        }
        return s[n-1];
    }
}
```





