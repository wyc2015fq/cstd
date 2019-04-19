# leetcode(10):easy4 - 刘炫320的博客 - CSDN博客
2018年03月17日 10:35:02[刘炫320](https://me.csdn.net/qq_35082030)阅读数：91标签：[数组](https://so.csdn.net/so/search/s.do?q=数组&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
这是我们100道题目中最后的easy题目了。
# 1.leetcode#437. Path Sum III
## 1.1问题描述
> 
You are given a binary tree in which each node contains an integer value. 
  Find the number of paths that sum to a given value. 
  The path does not need to start or end at the root or a leaf, but it must go downwards (traveling only from parent nodes to child nodes). 
  The tree has no more than 1,000 nodes and the values are in the range -1,000,000 to 1,000,000.
Example:
> 
root = [10,5,-3,3,2,null,11,3,-2,null,1], sum = 8 
        10 
       /  \ 
      5   -3 
     / \    \ 
    3   2   11 
   / \   \ 
  3  -2   1 
  Return 3. The paths that sum to 8 are: 
  1.  5 -> 3 
  2.  5 -> 2 -> 1 
  3. -3 -> 1
## 1.2思路
这道题最精彩之处就在于最后一句The tree has no more than 1,000 nodes and the values are in the range -1,000,000 to 1,000,000。使用二叉树转换为数组可以非常巧妙的解决此问题。
## 1.3代码
```java
int count;
    public int pathSum(TreeNode root, int sum) {
        count=0;
        int[] A=new int[1000];
        findSum(root,sum,A,0);
        return count;
    }
    void findSum(TreeNode root,int sum,int[] A,int i){
        if(root==null){
            return ;
        }
        A[i]=root.val;
        //统计符合个数
        count+=findSumFromArray(A,i,sum);
        //递归左孩子
        findSum(root.left, sum, A, i+1);
        //递归右孩子
        findSum(root.right, sum, A, i+1);
    }
    /***
     * 查找符合条件的
     * @param A
     * @param endIndex
     * @param sum
     * @return
     */
    int findSumFromArray(int[] A,int endIndex,int sum){
        int total=0;
        int sumCount=0;
        //从孩子节点求和
        for(int i=endIndex;i>=0;i--){
            total+=A[i];
            if(total==sum){
                sumCount++;
            }
        }
        return sumCount;
    }
```
# 2.leetcode#438. Find All Anagrams in a String
## 2.1问题描述
> 
Given a string s and a non-empty string p, find all the start indices of p’s anagrams in s. 
  Strings consists of lowercase English letters only and the length of both strings s and p will not be larger than 20,100. 
  The order of output does not matter.
Example
> 
Example 1: 
  Input: 
  s: “cbaebabacd” p: “abc” 
  Output: 
  [0, 6] 
  Explanation: 
  The substring with start index = 0 is “cba”, which is an anagram of “abc”. 
  The substring with start index = 6 is “bac”, which is an anagram of “abc”. 
  Example 2: 
  Input: 
  s: “abab” p: “ab” 
  Output: 
  [0, 1, 2] 
  Explanation: 
  The substring with start index = 0 is “ab”, which is an anagram of “ab”. 
  The substring with start index = 1 is “ba”, which is an anagram of “ab”. 
  The substring with start index = 2 is “ab”, which is an anagram of “ab”.
## 2.2思路
这道题就是使用一个滑动窗口，来统计窗口内是否符合该字符子串的字符分布。
## 2.3代码
```java
public List<Integer> findAnagrams(String s, String p) {
        List<Integer> list = new ArrayList<>();
         if (s == null || s.length() == 0 || p == null || p.length() == 0) return list;
         //表达出类似hash表的作用
         int[] hash = new int[256]; //character hash
         //统计p内的字符
         for (char c : p.toCharArray()) {
             hash[c]++;
         }
         //开始和结束节点，count是P中字符的个数
         int left = 0, right = 0, count = p.length();
         while (right < s.length()) {
             //如果它存在于表中，说明命中了一个字符
             if (hash[s.charAt(right)] >= 1) {
                 count--;
             }
             hash[s.charAt(right)]--;
             //右边移动
             right++;
             //如果确实都完全命中完毕
             if (count == 0) {
                 //记录目前的值
                 list.add(left);
             }
             //如果目前遍历到达p的长度了
             if (right - left == p.length() ) {
                 //如果left在hash表中，说明这个字符是有效的
                 if (hash[s.charAt(left)] >= 0) {
                     count++;
                 }     
                 hash[s.charAt(left)]++;
                 //左边的移动
                 left++;
             }
         }
         return list;      
    }
```
# 3.leetcode#448. Find All Numbers Disappeared in an Array
## 3.1问题描述
> 
Given an array of integers where 1 ≤ a[i] ≤ n (n = size of array), some elements appear twice and others appear once. 
  Find all the elements of [1, n] inclusive that do not appear in this array. 
  Could you do it without extra space and in O(n) runtime? You may assume the returned list does not count as extra space.
Example:
> 
Input: 
  [4,3,2,7,8,2,3,1] 
  Output: 
  [5,6]
## 3.2思路
这道题就是利用唯一性来进行解决，对于每一个数字都会有唯一的地址与之对应，只需要对目标地址标记，即可记录下有哪些字符的存在。这里采用对每个已经出现过的字符取负。
## 3.3代码
```java
public List<Integer> findDisappearedNumbers(int[] nums) {
        List<Integer> res = new ArrayList<Integer>();
        if(nums == null || nums.length == 0)
            return res;
        int n = nums.length;
        for(int i = 0;i < n;i++){
            //取绝对值是为了防止标记产生的副作用
            int val = Math.abs(nums[i]) - 1;
            if(nums[val] > 0)
                nums[val] = -nums[val];
        }
        //如果大于的话，说明该地址没有被见过
        for(int i = 0;i < n;i++){
            if(nums[i] > 0)
                res.add(i + 1);
        }
        return res;
    }
```
# 4.leetcode#581. Shortest Unsorted Continuous Subarray
## 4.1问题描述
> 
Given an integer array, you need to find one continuous subarray that if you only sort this subarray in ascending order, then the whole array will be sorted in ascending order, too. 
  You need to find the shortest such subarray and output its length.
Ecample
> 
Example 1: 
  Input: [2, 6, 4, 8, 10, 9, 15] 
  Output: 5 
  Explanation: You need to sort [6, 4, 8, 10, 9] in ascending order to make the whole array sorted in ascending order.
## 4.2思路
这里我们其实只需要找到第一个递减的位置和最后一个递增的位置。非常巧妙的是，可以反过来找，即找最后一个开始递增的数和第一个开始递减的数。一遍即可。
## 4.3代码
```java
public int findUnsortedSubarray(int[] nums) {
        int n = nums.length, beg = -1, end = -2, min = nums[n-1], max = nums[0];
        for (int i=1;i<n;i++) {
          max = Math.max(max, nums[i]);
          min = Math.min(min, nums[n-1-i]);
          //通过循环可以记录最后一个小于max的值
          if (nums[i] < max) end = i;
          //记录最前面一个大于min的值
          if (nums[n-1-i] > min) beg = n-1-i; 
        }
        return end - beg + 1;
    }
```
# 5.小结
本次主要介绍了数组的查找问题，都是一些非常巧妙的方法，但是适用性不是太强。
