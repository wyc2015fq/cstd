
# 最长公共子序列(LCS) 最长公共子串 - bitcarmanlee的博客 - CSDN博客


2019年04月02日 18:02:04[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：32



## 1.最长公共子序列
什么是子序列：
例如对于字符串"saabcd"，s,a,a是其一个子序列，s,a,b,d也是一个子序列。子序列不要求连续性。
最长公共子序列问题有最优子结构，这个问题可以分解称为更小的问题，因此整个问题就变简单了。同时，子问题的解释可以被重复使用的，也就是说更高级别的子问题会重用更小子问题的解。满足这两点以后，很容易就想到用动态规划来求解。
1.假设两个字符串s1, s2。当其中一个串的长度为0时，公共子序列的长度肯定为0。
2.假设s1的第i个字符与s2的第j个字符相等时，最长子序列等于s1的第i-1个字符与s2的第j-1个字符最长子序列长度+1。
3.假设s1的第i个字符与s2的第j个字符不相等时，最长子序列等于s1的第i个字符与s2的第j-1个字符最长子序列长度或s1的第i-1个字符与s2的第j个字符最长子序列长度中最大那一个。
如果用数学表达式描述如下。
$$
dp(i, j) = \begin{cases}
0, &amp; i=0 　||　j = 0 \\
dp(i-1, j-1) + 1,  &amp;i&gt;0　 \&amp;\&amp;　 j&gt;0 　\&amp;\&amp; 　s1[i] == s2[j] \\
max(dp(i-1, j), dp(i, j-1)), &amp;i&gt;0　 \&amp;\&amp;　 j&gt;0 　\&amp;\&amp; 　s1[i] != s2[j]
\end{cases}
$$
直接上代码
`public class LCS {
    public static final String s1 = "saabcd";
    public static final String s2 = "aaeefdhe";
    public static int[][] longestcommonsub() {
        int len1 = s1.length(), len2 = s2.length();
        int[][] dp = new int[len1+1][len2+1];
        for(int i=0; i<=len1; i++) {
            dp[i][0] = 0;
        }
        for(int i=0; i<=len2; i++) {
            dp[0][i] = 0;
        }
        for(int i=1; i<=len1; i++) {
            for(int j=1; j<=len2; j++) {
                if (s1.charAt(i-1) == s2.charAt(j-1)) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = Math.max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        return dp;
    }
    public static void printSeq(int[][] dp) {
        int i = s1.length(), j = s2.length();
        StringBuilder sb = new StringBuilder();
        while(i >= 1 && j >= 1) {
            if (s1.charAt(i-1) == s2.charAt(j-1)) {
                sb.append(s1.charAt(i-1));
                i--;
                j--;
            } else {
                if(dp[i][j-1] > dp[i-1][j]) {
                    // 说明相同的字符在行这边，下次遍历的时候应该是同一行，列向前退一格，所以j--
                    j--;
                } else {
                    i--;
                }
            }
        }
        sb.reverse();
        for(int k=0; k<sb.length(); k++) {
            System.out.println(sb.charAt(k));
        }
    }
    public static void main(String[] args) {
        int[][] dp = longestcommonsub();
        System.out.println("max length is: " + dp[dp.length-1][dp[0].length-1]);
        System.out.println("the character seq is: ");
        printSeq(dp);
    }
}`最后输出的结果为:
`max length is: 3
the character seq is: 
a
a
d`
## 2.最长公共子串
最长公共子串跟最长公共子序列的唯一区别在于，公共子串要求是连续的，子序列要求不一定连续。
具体的思路还是动态规划，不同点在于动态规划的迭代策略
$$
dp(i, j) = \begin{cases}
0, &amp; i=0 　||　j = 0 \\
dp(i-1, j-1) + 1,  &amp;i&gt;0　 \&amp;\&amp;　 j&gt;0 　\&amp;\&amp; 　s1[i] == s2[j] \\
0, &amp;i&gt;0　 \&amp;\&amp;　 j&gt;0 　\&amp;\&amp; 　s1[i] != s2[j]
\end{cases}
$$
`public class LCS2 {
    public static final String s1 = "aabcd";
    public static final String s2 = "abd";
    public static void lcs() {
        int len1 = s1.length(), len2 = s2.length();
        int[][] dp = new int[len1+1][len2+1];
        int maxnum = 0;
        // 子序列最后一个字符出现的位置
        int end = 0;
        for(int i=0; i<=len1; i++) {
            dp[i][0] = 0;
        }
        for(int j=0; j<=len2; j++) {
            dp[0][j] = 0;
        }
        for(int i=1; i<=len1; i++) {
            for(int j=1; j<=len2; j++) {
                if(s1.charAt(i-1) == s2.charAt(j-1)) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = 0;
                }
                if (dp[i][j] > maxnum) {
                    maxnum = dp[i][j];
                    // 注意i是dp数组的位置，映射到字符串中应该减1
                    end = i - 1;
                }
            }
        }
        System.out.println("result is: " + maxnum);
        System.out.println("the subseq is: " + s1.substring(end-maxnum+1, end+1));
    }
    public static void main(String[] args) {
        lcs();
    }
}`输出如下
`result is: 2
the subseq is: ab`

