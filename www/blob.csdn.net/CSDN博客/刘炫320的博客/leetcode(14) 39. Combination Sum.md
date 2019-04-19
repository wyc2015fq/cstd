# leetcode(14):39. Combination Sum - 刘炫320的博客 - CSDN博客
2018年04月09日 11:43:09[刘炫320](https://me.csdn.net/qq_35082030)阅读数：103
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1.leetcode39. Combination Sum
## 1.1问题描述
> 
Given a set of candidate numbers (C) (without duplicates) and a target number (T), find all unique combinations in C where the candidate numbers sums to T. 
  给一组候选数字（C）（没有重复）并给一个目标数字（T），找出 C 中所有唯一的组合使得它们的和为 T。 
  The same repeated number may be chosen from C unlimited number of times. 
  可以从 C 无限次数中选择相同的数字。 
  Note: 
  All numbers (including target) will be positive integers. 
  所有数字（包括目标）都是正整数。 
  The solution set must not contain duplicate combinations. 
  解集合中没有相同组合。 
  For example, given candidate set [2, 3, 6, 7] and target 7,  
  A solution set is:  
  [ 
    [7], 
    [2, 2, 3] 
  ]
## 1.2思路
这其实就是一个完全背包问题。做完全背包问题只需要注意这么几点，第一，数组中数是 从大到小排序。第二，在进行尝试的时候，要先能放多少放多少。第三，注意回溯的条件及解的保存。另外，这次是需要记忆路径的，因此需要使用记忆化搜索的形式。
## 1.3代码
```java
class Solution {
     public List<List<Integer>> combinationSum(int[] candidates, int target) {
        List<List<Integer>> result=new ArrayList<>();
        //进行排序
         Arrays.sort(candidates);
        int end=0;
        while(end<candidates.length&&candidates[end]<=target){
            end++;
        }
        end--;
        int candidatecount=end+1;
         //重新创建候选集合，并且是自大到小排序
        int[] newcandidate=new int[candidatecount];
        for(int i=0;i<=end;i++){
            newcandidate[i]=candidates[end-i];
        }
        int[] path=new int[candidatecount];
         //进行求解
        combinationSum(path, newcandidate, 0, target, result);
        return result;
    }
    public void combinationSum(int[] path,int[] candidates,int curr,int target,List<List<Integer>> result){
        //如果符合条件了。
        if(target==0){
            List<Integer> tempresult=new ArrayList<>();
            for(int i=0;i<path.length;i++){
                if(path[i]>0){
                    int count=path[i],num=candidates[i];
                    while(count-->0){
                        tempresult.add(num);
                    }
                }
            }
            result.add(tempresult);
            return;
        }
        //如果不满足条件了
        if(curr==path.length){
            return;
        }
        //添加当前值的数目，完全背包问题一定要从大到小排序
        for(int i=target/candidates[curr];i>=0;i--){
            path[curr]=i;
            combinationSum(path, candidates, curr+1, target-candidates[curr]*i, result);
        }
    }
}
```
## 1.4进一步思考
观察后可以发现其中最慢的部分要数给出路径结果的部分，如果可以实时记录下路径，就好很多。 
但是这就是另一个思路了。并不是真正传统上的完全背包问题的普通解法。
## 1.5代码
```java
public List<List<Integer>> combinationSum(int[] nums, int target) {
    List<List<Integer>> list = new ArrayList<>();
    Arrays.sort(nums);
    backtrack(list, new ArrayList<>(), nums, target, 0);
    return list;
}
private void backtrack(List<List<Integer>> list, List<Integer> tempList, int [] nums, int remain, int start){
    //如果不符合条件了
    if(remain < 0) return;
    //如果符合条件的话
    else if(remain == 0) list.add(new ArrayList<>(tempList));
    else{ 
        for(int i = start; i < nums.length; i++){
            tempList.add(nums[i]);
            //可以重复添加该元素，因此添加第i个而不是i+1；
            backtrack(list, tempList, nums, remain - nums[i], i); 
            //再删除最后一个元素
            tempList.remove(tempList.size() - 1);
        }
    }
}
```
