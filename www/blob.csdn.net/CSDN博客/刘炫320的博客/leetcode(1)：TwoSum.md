# leetcode(1)：TwoSum - 刘炫320的博客 - CSDN博客
2018年03月03日 21:59:19[刘炫320](https://me.csdn.net/qq_35082030)阅读数：158
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
从今天开始我们开始踏上leetcode这片新的土地！从征服第一块领地开始！
# 1.leetcode#1. Two Sum
## 1.1题目描述
Given an array of integers, return indices of the two numbers such that they add up to a specific target.
You may assume that each input would have exactly one solution, and you may not use the same element twice.
Example:
> 
Given nums = [2, 7, 11, 15], target = 9, 
  Because nums[0] + nums[1] = 2 + 7 = 9, 
  return [0, 1].
## 1.2思路
这道题第一反应是两个循环走起的暴力破解。但是我们知道一般算法题是不允许用暴力破解的，万不得已才用。
但是可以优化这个暴力破解的过程，比如使用Java里的hashMap可以让搜索数组中存在某个值的时间复杂度为O（1）。
## 1.3代码
```java
/**
     * 使用一遍HashMap来解决
     * @param nums 数组
     * @param target 目标和
     * @return 两个数的下标
     */
    public int[] twoSum(int[] nums, int target) {
       Map<Integer, Integer> map = new HashMap<>();
       for (int i = 0; i < nums.length; i++) {
           //找到互补的数
           int complement = target - nums[i];
           //如果包含互补的数
           if (map.containsKey(complement)) {
               //返回结果
               return new int[] { map.get(complement), i };
           }
           //否则把该数及下标放入Map里
           map.put(nums[i], i);
       }
       throw new IllegalArgumentException("No two sum solution");
   }
```
但是这样我们只算学到了一点，那就是当可以遍历搜索数组时，我们可以使用HashMap来加速搜索。但是题目还是有后续的，请看后续题目。
# 2.leetcode#167. Two Sum II - Input array is sorted
## 2.1题目描述
Given an array of integers that is already sorted in ascending order, find two numbers such that they add up to a specific target number.
The function twoSum should return indices of the two numbers such that they add up to the target, where index1 must be less than index2. Please note that your returned answers (both index1 and index2) are not zero-based.
You may assume that each input would have exactly one solution and you may not use the same element twice.
Example:
> 
Input: numbers={2, 7, 11, 15}, target=9 
  Output: index1=1, index2=2
## 2.2思路
这道题目比上一道增加了一个约束条件，那就是数组是有序的了。虽然我们仍然可以使用HashMap偷懒来做，但是既然有序，对，我们提到有序搜索，就应该联想到二分法。这是很自然的想法。
## 2.3代码：
```java
public int[] twoSum2(int[] numbers, int target) {
         int i=0,j=numbers.length-1;
         while(i<j){
             int sum=numbers[i] + numbers[j];
                if(sum==target)return new int[]{i+1,j+1};
                //当数值大于目标值是，从右边减
                else if(sum>target)j--;
                //当数值小于目标值是，从左边加
                else i++;
             }
       throw new IllegalArgumentException("No two sum solution");
     }
```
## 2.4更进一步
当然，在百度第一页上的此题目的解法仅限于此了。但是我们应该能想到，既然左右可以二分，为什么不用二分法+分治呢？也就是说，上述的算法中存在可以优化的地方，那就是当不符合条件时，移动的距离太短，应该用二分法来替代自增自减操作。
## 2.5代码
```java
public static int[] twoSum(int[] numbers, int target) {
            if (numbers == null || numbers.length == 0) {
                return new int[2];
            }
            int start = 0;
            int end = numbers.length - 1;
            while (start < end) {
                if (numbers[start] + numbers[end] == target) {
                    return new int[]{start + 1, end + 1};
                } else if (numbers[start] + numbers[end] > target) {
                    //移动结尾到符合目标的第一个
                    end = largestSmallerOrLastEqual(numbers, start, end, target - numbers[start]);
                } else {
                    // 移动开头到符合目标的第一个
                    start = smallestLargerOrFirstEqual(numbers, start, end, target - numbers[end]);
                }
            }
            return new int[2];
        }
        private static int largestSmallerOrLastEqual(int[] numbers, int start, int end, int target) {
            int left = start;
            int right = end;
            //左边小于右边
            while (left <= right) {
                //左右中间点
                int mid = left + (right - left) / 2;
                //如果中间的大于目标
                if (numbers[mid] > target) {
                    //右边的等于mid-1
                    right = mid - 1;
                } else {
                    //左边等于目标+1；
                    left = mid + 1;
                }
            }
            return right;
        }
        private static int smallestLargerOrFirstEqual(int[] numbers, int start, int end, int target) {
            int left = start;
            int right = end;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (numbers[mid] < target) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            return left;
        }
```
这样我们又学到了一点，那就是，当出现搜索与约束不相符时，尽可能的大距离改变搜索范围，提高搜索效率。但是还有第二道衍生题，请看下一题。
# 3.leetcode#653. Two Sum IV - Input is a BST
## 3.1题目描述
Given a Binary Search Tree and a target number, return true if there exist two elements in the BST such that their sum is equal to the given target.
> 
Example 
  Example 1: 
  Input:  
      5 
     / \ 
    3   6 
   / \   \ 
  2   4   7 
  Target = 9 
  Output: True 
  Example 2: 
  Input:  
      5 
     / \ 
    3   6 
   / \   \ 
  2   4   7 
  Target = 28 
  Output: False
## 3.2思路
这道题的数据结构更加复杂，之前是数组或者有序数组还好，使用各种遍历来达成搜索的目的。但是一旦涉及到树搜索，那么自然要想到BFS和DFS，当然你依然可以使用HashMap来做,思路和之前的一样。
## 3.3代码
```java
public boolean findTarget(TreeNode root, int k) {
        Set < Integer > set = new HashSet();
        return find(root, k, set);
    }
    public boolean find(TreeNode root, int k, Set < Integer > set) {
        if (root == null)
            return false;
        if (set.contains(k - root.val))
            return true;
        set.add(root.val);
        return find(root.left, k, set) || find(root.right, k, set);
    }
```
## 3.4更近一步
当然如果我们熟悉树里的搜索的话，应该知道，达成此类问题一定要有两种搜索过程，第一种是递归实现大目标，第二种是递归实现子目标。这道题也是如此。我们其实也知道搜索思路如下：
- 当前节点是其中一个节点，然后再在树上找另一个节点。
- 当前节点不是目标中的一个节点，那么递归它的左子树和右子树即可。
## 3.5代码
```java
public boolean findTarget(TreeNode root, int k) {
        return dfs(root, root, k);
    }
    /**
     * 深度优先遍历
     * @param root 父节点
     * @param cur 当前节点
     * @param k 目标值
     * @return
     */
    private boolean dfs(TreeNode root, TreeNode cur, int k) {
        //如果当前值为空则返回false
        if(cur == null) return false;
        //否则搜索在树内是否存在剩余的一个值或者在左子树上递归，或者在右子树上递归
        return search(root, cur, k-cur.val) || dfs(root, cur.left, k) || dfs(root, cur.right, k);
    }
    /**
     * 搜索是否存在一个目标值
     * @param root 搜索节点
     * @param cur 当前节点
     * @param target 目标值
     * @return
     */
    private boolean search(TreeNode root, TreeNode cur, int target) {
        //如果父节点为空
        if(root == null) return false;
        //如果目标值找到了并且其值不是出发时的节点，则返回true
        if(target == root.val && root != cur) return true;
        //如果目标值大于搜索节点的值，则递归搜索节点的右子树
        else if (target > root.val) return search(root.right, cur, target);
        //如果目标值小于搜索节点的值，则搜索搜索节点的左子树
        else return search(root.left, cur, target);
    }
```
# 4小结
在Leetcode第一题的指引下，我们顺利的占领了第一块高地。本次学习主要有以下几点内容： 
1.当可以遍历搜索数组时，我们可以使用HashMap来加速搜索。 
2.HashMap的构建需要O(n)，如果数组有序的话，我们更愿意使用二分法来解决问题，其时间复杂度为O（logn）。 
3.当我们在搜索空间中搜索目标时，当出现错误时，应当尽可能的朝着正确方向更正。 
4.如果有树结构时，应当首先考虑拆分任务为子任务和递归任务，捋清楚搜索策略后，应该比较容易写出代码。
当然，正如汤姆马歇尔（机器学习之父）所说，我们可以把所有机器学习问题，看成是搜索问题。其实所有的算法，都可以看成是搜索问题，搜索空间内找到目标值的过程是大同小异的，其加速方法无怪乎是缩小搜索空间，加速更正速度等。
编程是如此，人生也是一样。向着星辰大海，出发吧！
