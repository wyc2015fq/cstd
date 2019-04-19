# leetcode(16):最长公共子串/最长公共子序列 - 刘炫320的博客 - CSDN博客
2018年04月17日 13:33:31[刘炫320](https://me.csdn.net/qq_35082030)阅读数：4109
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)[算法思想培养指南](https://blog.csdn.net/column/details/36524.html)
最近BAT等一线大厂面试的热门题目，就是求最长公共子串子序列问题，Leetcode上面没有完全相同的题目，但是有类似的题目，让我们一起来看看。
# 1.leetcode#718. Maximum Length of Repeated Subarray
## 1.1题目描述
> 
Given two integer arrays A and B, return the maximum length of an subarray that appears in both arrays.
Example :
> 
Input: 
  A: [1,2,3,2,1] 
  B: [3,2,1,4,7] 
  Output: 3 
  Explanation:  
  The repeated subarray with maximum length is [3, 2, 1].
## 1.2思路
这其实是一道动态规划的题目，其实意思就是找到当前节点之前连续的公共子串长度问题。一旦想到这里，就知道其实状态转移就2种，要么上一个匹配成功，要么这一个匹配成功，其他情况都为0。如果我们用匹配矩阵来做，会非常好做。其思想就是，让行为第一个字符串，列为第二个字符串，那么Maching Metrix[row][col]则是第一个字符串第row位置和第二个字符串第col位置最大的匹配长度。只需要动笔画一下，即可完全理解此方式。
## 1.3代码
```java
public int findLength(int[] A, int[] B) {
        int max=0;
        int[][] samecount=new int[A.length][B.length];
        //初始化第一行第一列
        for(int i=0;i<A.length;i++){
            if(B[0]==A[i]){
                samecount[i][0]=1;
            }
        }
        for(int i=0;i<B.length;i++){
            if(B[i]==A[0]){
                samecount[0][i]=1;
            }
        }
        //初始化最后一行
        for(int row=1;row<A.length;row++){
            for(int col=1;col<B.length;col++){
                if(A[row]==B[col]){
                    //状态转移
                    samecount[row][col]=samecount[row-1][col-1]+1;
                    //全局比较
                    max=max>samecount[row][col]?max:samecount[row][col];
                }
            }
        }
        return max;
    }
```
## 1.4更进一步
上面的方式非常容易理解，时间复杂度是O(m*n)，空间复杂度也是O（m*n）。如果还想进一步压缩，其实也是可以的，可以做到时间复杂度是O(min(m,n))，空间复杂度是O(max(m,n))。具体方法就是，我们把二位矩阵改成一维矩阵，然后通过迭代的方式来不断的进行匹配，例如固定第二个字符串，然后使用第一个字符串的每一个字符都在第二个字符串中匹配，把匹配结果迭代到下一次的匹配中即可。
## 1.5代码
```java
public int findLength(int[] A, int[] B) {
        //时间复杂度小
        if(A.length<B.length){
            return findLength(B, A);
        }
        int max=0;
        int[] samecount=new int[A.length];
        //初始化第一行
        for(int i=0;i<A.length;i++){
            if(B[0]==A[i]){
                samecount[i]=1;
            }
        }
        //初始化最后一行
        for(int i=1;i<B.length;i++){
            int[] tempsamcount=new int[A.length];
            for(int j=0;j<A.length;j++){
                if(A[j]==B[i]){
                    if(j==0){
                        tempsamcount[j]=1;
                    }
                    else{
                        //状态转移
                        tempsamcount[j]=samecount[j-1]+1;
                    }
                    //全局比较
                    max=max>tempsamcount[j]?max:tempsamcount[j];
                }
            }
            //下一次迭代
            samecount=tempsamcount;
        }
        return max;
    }
```
至于空间复杂度为O（1）的算法，太难理解，因此我们这里不进行讲解。
# 2.leetcode#583. Delete Operation for Two Strings
## 2.1问题描述
> 
Given two words word1 and word2, find the minimum number of steps required to make word1 and word2 the same, where in each step you can delete one character in either string.
Example :
> 
Input: “sea”, “eat” 
  Output: 2 
  Explanation: You need one step to make “sea” to “ea” and another step to make “eat” to “ea”.
## 2.2思路
这道题原意思为，每次可以删除一个字符，然后问删除的最少次数，使得两个字符串相同。其实就是找到最长公共子序列即可。和最长公共子串一样，我们只需要找到其公共子序列的状态转移即可，只不过这里不需要连续，即不仅考虑它的上一状态中相匹配的那一种情况Matching Metrix[row-1][col-1]，还要考虑上一状态中，剩余的两种情况，即Matching Metrix[row-1][col]和Matching Metrix[row][col-1]。
## 2.3代码
```java
public static int minDistance(String word1, String word2) {
        if(word1==null||word1.length()==0){
            return word2.length();
        }
        else if(word2==null||word2.length()==0){
            return word1.length();
        }
        int max=0;
        //行为word1,列为word2;
        int[][] samecount=new int[word1.length()][word2.length()];
        //先把第一行处理掉
        for(int i=0;i<word1.length();i++){
            if(word1.charAt(i)==word2.charAt(0)){
                while(i<word1.length()){
                    samecount[i++][0]=1;
                }
                max=1;
            }
        }
        //再把第一列处理掉
        for(int i=0;i<word2.length();i++){
            if(word2.charAt(i)==word1.charAt(0)){
                while(i<word2.length()){
                    samecount[0][i++]=1;
                }
                max=1;
            }
        }
        //对后面的进行遍历
        for(int row=1;row<word1.length();row++){
            for(int col=1;col<word2.length();col++){
                //状态转移
                samecount[row][col]=Math.max(samecount[row-1][col], samecount[row][col-1]);
                //如果当前相等的话，再看是否比目前最长的还长
                if(word1.charAt(row)==word2.charAt(col)){
                    samecount[row][col]=Math.max(samecount[row][col], samecount[row-1][col-1]+1);
                }
                //与全局比较
                max=max>samecount[row][col]?max:samecount[row][col];
            }
        }
        return word1.length()+word2.length()-2*max;
    }
```
## 2.4更进一步
如同第一题一样，我们仍然可以缩减空间复杂度，方法相同，不过处理起来会有一些麻烦。主要在于初始值的问题。
## 2.5代码
```java
public static int minDistance(String word1, String word2) {
        if(word1==null||word1.length()==0){
            return word2.length();
        }
        else if(word2==null||word2.length()==0){
            return word1.length();
        }
        //假设word1长
        if(word1.length()<word2.length()){
            return minDistance(word2, word1);
        }
        int max=0;
        int[] samecount=new int[word1.length()];
        for(int i=0;i<samecount.length;i++){
            //只要有1个1，下面全部为1；
            if(word2.charAt(0)==word1.charAt(i)){
                while(i<samecount.length){
                    samecount[i++]=1;
                }
                max=1;
            }
        }
        for(int i=1;i<word2.length();i++){
            int[] tempsamecount=new int[word1.length()];
            for(int j=0;j<word1.length();j++){
                //如果是首位的话
                if(j==0){
                    //相等，则为1，否则为0
                    tempsamecount[j]=word1.charAt(j)==word2.charAt(i)?1:0;
                    //但是如果前面相等过了，则即使不相等也为1；
                    tempsamecount[j]=Math.max(tempsamecount[j], samecount[j]);
                }
                else{
                    //状态转移
                    //先看目前最长的子序列为多少
                    tempsamecount[j]=Math.max(tempsamecount[j-1], samecount[j]);
                    //如果相等当前相等的话，再看是否比目前最长的还长
                    if(word1.charAt(j)==word2.charAt(i)){
                        tempsamecount[j]=Math.max(tempsamecount[j], samecount[j-1]+1);
                    }
                }
                //与全局比较
                max=max>tempsamecount[j]?max:tempsamecount[j];
            }
            //进行下一次递归
            samecount=tempsamecount;
        }
        return word1.length()+word2.length()-2*max;
    }
```
# 3.小结
这一小节中，我们主要学习了最长公共子串和最长公共子序列的问题，把它转换为了一个匹配矩阵问题，自左上至右下进行搜索，完全考虑到了序列化的时间先后顺序。是一个非常通用的解法。
