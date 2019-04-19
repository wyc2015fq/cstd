# lintcode&九章算法——Lintcode No.77. 最长公共子序列 ？ 待解决 - wjheha的博客 - CSDN博客
2017年12月16日 14:42:24[wjheha](https://me.csdn.net/wjheha)阅读数：109
来源：公众号fourierhai 
[http://mp.weixin.qq.com/s/aXQ8NQTQugsLDV2CVJ41gA](http://mp.weixin.qq.com/s/aXQ8NQTQugsLDV2CVJ41gA)
***No.77. 最长公共子序列***
给出两个字符串，找到最长公共子序列(LCS)，返回LCS的长度。 
您在真实的面试中是否遇到过这个题？ Yes 
***说明***
最长公共子序列的定义：
- 最长公共子序列问题是在一组序列（通常2个）中找到最长公共子序列（注意：不同于子串，LCS不需要是连续的子串）。该问题是典型的计算机科学问题，是文件差异比较程序的基础，在生物信息学中也有所应用。
- [https://en.wikipedia.org/wiki/Longest_common_subsequence_problem](https://en.wikipedia.org/wiki/Longest_common_subsequence_problem)
***样例***
给出”ABCD” 和 “EDCA”，这个LCS是 “A” (或 D或C)，返回1 
给出 “ABCD” 和 “EACB”，这个LCS是”AC”返回 2
***解题思路***
动态规划 经典题目，还好今天去听了算法课，讲了好几道关于动态规划的题目。 
其实这道题目要求的只是最长公共子序列的长度，这只是第一步。第二步是返回最长公共子序列。 
参考博客[http://blog.chinaunix.net/uid-26548237-id-3374211.html](http://blog.chinaunix.net/uid-26548237-id-3374211.html)
利用二维数组记录信息： 
若字符串A和字符串B长度分别为m和n 
创建1个二维数组L[m.n]； 
初始化L数组内容为0 
m和n分别从0开始，m++，n++循环： 
如果str1[m] == str2[n]，则L[m,n] = L[m - 1, n -1] + 1； 
如果str1[m] != str2[n]，则L[m,n] = max{L[m,n - 1]，L[m - 1, n]} 
最后从L[m,n]中的数字一定是最大的，且这个数字就是最长公共子序列的长度 
从数组L中找出一个最长的公共子序列
***代码实现***
```
public class Solution {
    /*
     * @param A: A string
     * @param B: A string
     * @return: The length of longest common subsequence of A and B
     */
    public int longestCommonSubsequence(String A, String B) {
        // write your code here
        int n = A.length();
        int m = B.length();
        int C[][] = new int[n + 1;][m + 1];
        for(int i = 1; i <= n; i++){
            for(int j = 1; j <= m; j++){
                C[i][j] = Math.max(C[i - 1][j], C[i][j - 1]);
                if(A.charAt(i - 1) == B.charAt(j - 1)){
                    C[i][j] = C[i - 1][j - 1] + 1;
                }
            }
        }
        return C[n][m]; 
    }
}
```
***评测结果***
时间大概是在3000ms以内。
