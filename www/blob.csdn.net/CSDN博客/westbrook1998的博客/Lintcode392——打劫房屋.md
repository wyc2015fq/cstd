# Lintcode392——打劫房屋 - westbrook1998的博客 - CSDN博客





2018年02月19日 19:21:14[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：103








## 题目：

> 
假设你是一个专业的窃贼，准备沿着一条街打劫房屋。每个房子都存放着特定金额的钱。你面临的唯一约束条件是：相邻的房子装着相互联系的防盗系统，且 当相邻的两个房子同一天被打劫时，该系统会自动报警。 

  给定一个非负整数列表，表示每个房子中存放的钱， 算一算，如果今晚去打劫，你最多可以得到多少钱 在不触动报警装置的情况下。  

样例 

  给定 [3, 8, 4], 返回 8. 

  动态规划  
## 题解：（O（n）空间复杂度）

```
public class Solution {
    /*
     * @param A: An array of non-negative integers
     * @return: The maximum amount of money you can rob tonight
     */
    public long houseRobber(int[] A) {
        // write your code here
        //状态转移方程
        //dp[i]=max(dp[i-1],dp[i-2]+A[i])
        int len=A.length;
        if(0==len){
            return 0;
        }
        if(1==len){
            return A[0];
        }
        if(2==len){
            return Math.max(A[0],A[1]);
        }
        long[] dp=new long[len];         //用于数值很大的测试数据
        dp[0]=A[0];
        dp[1]=A[1];
        for(int i=2;i<len;i++){
            dp[i]=(long)Math.max(dp[i-1],dp[i-2]+A[i]);
        }
        return dp[len-1];
    }   
}
```

## 题解：O（1）空间复杂度：

主要就是将dp数组改成三个dp变量    

```
public class Solution {
    /*
     * @param A: An array of non-negative integers
     * @return: The maximum amount of money you can rob tonight
     */
    public long houseRobber(int[] A) {
        // write your code here
        //状态转移方程
        //dp[i]=max(dp[i-1],dp[i-2]+A[i])
        int len=A.length;
        if(0==len){
            return 0;
        }
        if(1==len){
            return A[0];
        }
        if(2==len){
            return Math.max(A[0],A[1]);
        }
        long dp_1,dp_2,dp_3=0;        //用于数值很大的测试数据
        dp_1=A[0];
        dp_2=A[1];
        for(int i=2;i<len;i++){
            dp_3=(long)Math.max(dp_2,dp_1+A[i]);
            dp_1=dp_2;
            dp_2=dp_3;
        }
        return dp_3;
    }   
}
```





