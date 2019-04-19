# leetcode(18):回溯问题 - 刘炫320的博客 - CSDN博客
2018年04月20日 18:55:17[刘炫320](https://me.csdn.net/qq_35082030)阅读数：200
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)[算法思想培养指南](https://blog.csdn.net/column/details/36524.html)
回溯有一系列问题，例如求一些排列和组合的问题。这里有大神的统一思路，我们来汉化一把。
# 1.leetcode:78.Subsets
## 1.1问题描述
> 
Given a set of distinct integers, nums, return all possible subsets (the power set).
Example
> 
Input: nums = [1,2,3] 
  Output: 
  [ 
    [3], 
    [1], 
    [2], 
    [1,2,3], 
    [1,3], 
    [2,3], 
    [1,2], 
    [] 
  ]
## 1.2代码
```java
public List<List<Integer>> subsets(int[] nums) {
    List<List<Integer>> list = new ArrayList<>();
    //先进行排序，这步骤不是必要的，但是是通用的，尤其是对于组合类的题目
    Arrays.sort(nums);
    //
    backtrack(list, new ArrayList<>(), nums, 0);
    //返回结果
    return list;
}
private void backtrack(List<List<Integer>> list , List<Integer> tempList, int [] nums, int start){
    //把之前的结果先保存到最终结果中。
    list.add(new ArrayList<>(tempList));
    for(int i = start; i < nums.length; i++){
        //添加第i个数字到暂时的节点中
        tempList.add(nums[i]);
        //把暂时的结果进行下一次递归，并把开始的元素往前加1；
        backtrack(list, tempList, nums, i + 1);
        //去掉当前节点再递归。
        tempList.remove(tempList.size() - 1);
    }
}
```
# 2.leetcode:90. Subsets II
## 2.1题目描述
> 
Given a collection of integers that might contain duplicates, nums, return all possible subsets (the power set).
Example
> 
Input: [1,2,2] 
  Output: 
  [ 
    [2], 
    [1], 
    [1,2,2], 
    [2,2], 
    [1,2], 
    [] 
  ]
## 2.2代码
```java
public List<List<Integer>> subsetsWithDup(int[] nums) {
    List<List<Integer>> list = new ArrayList<>();
    //排序这时候是必须的
    Arrays.sort(nums);
    backtrack(list, new ArrayList<>(), nums, 0);
    return list;
}
private void backtrack(List<List<Integer>> list, List<Integer> tempList, int [] nums, int start){
    list.add(new ArrayList<>(tempList));
    for(int i = start; i < nums.length; i++){
        //如果有重复的的话，就跳过。
        if(i > start && nums[i] == nums[i-1]) continue; 
        //添加当前数
        tempList.add(nums[i]);
        //继续下一个递归
        backtrack(list, tempList, nums, i + 1);
        //移除最后一个数，再进行循环。
        tempList.remove(tempList.size() - 1);
    }
}
```
# 3.leetcode:46. Permutations
## 3.1问题描述
> 
Given a collection of distinct integers, return all possible permutations.
Example
> 
Input: [1,2,3] 
  Output: 
  [ 
    [1,2,3], 
    [1,3,2], 
    [2,1,3], 
    [2,3,1], 
    [3,1,2], 
    [3,2,1] 
  ]
## 3.2代码
```java
public List<List<Integer>> permute(int[] nums) {
       List<List<Integer>> list=new ArrayList <>();
       Arrays.sort(nums);
    }
private void backtrack(List<List<Integer>> list,List<Integer> templist, int[] nums){
    //如果达到了需要的长度
    if(templist.size()==nums.length){
        //就把当前序列加入到最终结果中。
        list.add(new ArrayList<>(templist));
    }
    else{
        for(int i=0;i<nums.length;i++){
            //如果包含过值了，就继续。
            if(templist.contains(nums[i])){
                continue;
            }
            //添加当前值到结果中
            templist.add(nums[i]);
            //继续添加
            backtrack(list,templist,nums);
            //删除最后一个值继续递归
            templist.remove(templist.size()-1);
        }
    }
}
```
# 4.leetcode:47. Permutations II
## 4.1问题描述
> 
Given a collection of numbers that might contain duplicates, return all possible unique permutations.
Example
> 
Input: [1,1,2] 
  Output: 
  [ 
    [1,1,2], 
    [1,2,1], 
    [2,1,1] 
  ]
## 4.2代码
```java
public List<List<Integer>> permuteUnique(int[] nums) {
    List<List<Integer>> list = new ArrayList<>();
    Arrays.sort(nums);
    backtrack(list, new ArrayList<>(), nums, new boolean[nums.length]);
    return list;
}
private void backtrack(List<List<Integer>> list, List<Integer> tempList, int [] nums, boolean [] used){
    if(tempList.size() == nums.length){
        list.add(new ArrayList<>(tempList));
    } else{
        for(int i = 0; i < nums.length; i++){
        //如果第i位置用过了，或者如果和上一个一样，但是上一个没放进去过的话。
            if(used[i] || i > 0 && nums[i] == nums[i-1] && !used[i - 1]) continue;
            //把当前值置为1；
            used[i] = true; 
            //把当前值加入到节点中
            tempList.add(nums[i]);
            //进行递归
            backtrack(list, tempList, nums, used);
            //再把当前值从局部结果中去除。
            used[i] = false; 
            //移除当前值
            tempList.remove(tempList.size() - 1);
        }
    }
}
```
# 5.leetcode:39. Combination Sum
## 5.1问题描述
> 
Given a set of candidate numbers (candidates) (without duplicates) and a target number (target), find all unique combinations in candidates where the candidate numbers sums to target. 
  The same repeated number may be chosen from candidates unlimited number of times.
Example
> 
Input: candidates = [2,3,6,7], target = 7, 
  A solution set is: 
  [ 
    [7], 
    [2,2,3] 
  ]
## 5.2代码
```java
public List<List<Integer>> combinationSum(int[] nums, int target) {
    List<List<Integer>> list = new ArrayList<>();
    //进行必要的排序
    Arrays.sort(nums);
    backtrack(list, new ArrayList<>(), nums, target, 0);
    return list;
}
private void backtrack(List<List<Integer>> list, List<Integer> tempList, int [] nums, int remain, int start){
    //如果已经多了的话，就没有结果。
    if(remain < 0) return;
    //如果确实符合条件了，就加入到解中。
    else if(remain == 0) list.add(new ArrayList<>(tempList));
    else{ 
        //从起始位置
        for(int i = start; i < nums.length; i++){
            //把当前加入到解中
            tempList.add(nums[i]);
            //进行递归，还可以放入相同的数字。
            backtrack(list, tempList, nums, remain - nums[i], i); 
            //把最后一个移除
            tempList.remove(tempList.size() - 1);
        }
    }
}
```
# 6.leetcode:40. Combination Sum II
## 6.1问题描述
> 
Given a collection of candidate numbers (candidates) and a target number (target), find all unique combinations in candidates where the candidate numbers sums to target. 
  Each number in candidates may only be used once in the combination.
Example 1:
> 
Input: candidates = [10,1,2,7,6,1,5], target = 8, 
  A solution set is: 
  [ 
    [1, 7], 
    [1, 2, 5], 
    [2, 6], 
    [1, 1, 6] 
  ]
## 6.2代码
```java
public List<List<Integer>> combinationSum2(int[] nums, int target) {
    List<List<Integer>> list = new ArrayList<>();
    //排序
    Arrays.sort(nums);
    backtrack(list, new ArrayList<>(), nums, target, 0);
    return list;
}
private void backtrack(List<List<Integer>> list, List<Integer> tempList, int [] nums, int remain, int start){
    //如果已经超了
    if(remain < 0) return;
    //如果可以的话
    else if(remain == 0) list.add(new ArrayList<>(tempList));
    else{
        for(int i = start; i < nums.length; i++){
            //如果有重复的就要跳过。
            if(i > start && nums[i] == nums[i-1]) continue; 
            //把结果加入进去
            tempList.add(nums[i]);
            //进行迭代
            backtrack(list, tempList, nums, remain - nums[i], i + 1);
            //去掉当前值
            tempList.remove(tempList.size() - 1); 
        }
    }
}
```
# 7leetcode:131. Palindrome Partitioning
## 7.1问题描述
> 
Given a string s, partition s such that every substring of the partition is a palindrome. 
  Return all possible palindrome partitioning of s.
Example
> 
given s = “aab”, 
  Return 
  [ 
    [“aa”,”b”], 
    [“a”,”a”,”b”] 
  ]
## 7.2代码
```java
public List<List<String>> partition(String s) {
   List<List<String>> list = new ArrayList<>();
   backtrack(list, new ArrayList<>(), s, 0);
   return list;
}
public void backtrack(List<List<String>> list, List<String> tempList, String s, int start){
   if(start == s.length())
      list.add(new ArrayList<>(tempList));
   else{
      for(int i = start; i < s.length(); i++){
         //如果是回文数
         if(isPalindrome(s, start, i)){
             //添加到结果中
            tempList.add(s.substring(start, i + 1));
            //进行递归
            backtrack(list, tempList, s, i + 1);
            //移除最后一个值
            tempList.remove(tempList.size() - 1);
         }
      }
   }
}
//判断是否是回文数
public boolean isPalindrome(String s, int low, int high){
   while(low < high)
      if(s.charAt(low++) != s.charAt(high--)) return false;
   return true;
}
```
# 8.小结
通过上面7个例子，我们可以总结以下通用流程： 
1.backtrack至少包含4个参数，包括最终的结果，临时的结果，原数组和当前次序，还有其他的参数都是约束条件，包括剩余空间、目标和等。 
2.在递归时，先判断是否满足条件，如果满足约束条件了，就加入到最终结果中。否则的话，还是从start开始先加入临时结果，再进行回溯，然后再去除当前值。 
3.在进行遍历的时候，要看一些约束条件，比如，是否需要重复的数字，是否需要使用。
