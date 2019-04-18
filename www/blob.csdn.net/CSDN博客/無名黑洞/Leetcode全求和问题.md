# Leetcode全求和问题 - 無名黑洞 - CSDN博客
2014年02月04日 10:29:10[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：12298
**目录**
**1、编号1 Two Sum (排序，双指针)2、编号14 3Sum (排序，双指针)3、编号16 3Sum Closest (排序，双指针)4、编号18 4Sum Total (排序，双指针)5、编号38 Combination Sum (DFS)6、编号39 Combination Sum II (DFS)7、编号53 Maximum Subarray (DP)8、编号62 Minimum Path Sum (DP)9、编号65 Add Binary10、编号121 Triangle (DP)**
**1、编号1 Two Sum**
Given an array of integers, find two numbers such that they add up to a specific target number.
The function twoSum should return indices of the two numbers such that they add up to the target, where index1 must be less than index2. Please note that your returned answers (both index1 and index2) are not zero-based.
You may assume that each input would have exactly one solution.
Input: numbers={2, 7, 11, 15}, target=9
Output: index1=1, index2=2
这是Leetcode第一题！Welcome to the world of Leetcode! 但是第一题也不简单，因为时间复杂度O(n^2)会超时，所以不能用简单的两重循环求解。如果先把输入排序(O(nlogn))的话就可以通过大数据。核心算法是用两个指针向中间走。另外该题要求输出答案数据的序号。
```cpp
class Solution {
public:
    /*Below is the 2 sum algorithm that is O(NlogN) + O(N)*/
    /*Alternative: hash从左往右扫描一遍，然后将数及坐标，存到map中。然后再扫描一遍即可。时间复杂度O(n)*/
    vector<int> twoSum(vector<int> &numbers, int target) {
        vector<int> numbersCopy;
        for(int i = 0; i < numbers.size(); i++) numbersCopy.push_back(numbers[i]);
        
        sort(numbersCopy.begin(), numbersCopy.end()); //O(NlogN)
        vector<int> returnNumbers = twoSumAlgorithm(numbersCopy, target);//O(N)
        
        vector<int> returnIndexes;
        for(int j = 0; j < returnNumbers.size(); j++)
            for(int i = 0; i < numbers.size(); i++)//O(N)
                if(numbers[i] == returnNumbers[j]) returnIndexes.push_back(i + 1);
                
        if(returnIndexes[0] > returnIndexes[1]){
            returnIndexes[0] = returnIndexes[0]^returnIndexes[1];
            returnIndexes[1] = returnIndexes[0]^returnIndexes[1];
            returnIndexes[0] = returnIndexes[0]^returnIndexes[1];
        }
        return returnIndexes;
    }
    
    /*Core algorithm is linear*/
    vector<int> twoSumAlgorithm(vector<int> &numbers, int target) {
        int len = numbers.size();
        vector<int> r;
        int i = 0; int j = len - 1;
        while(i < j){
            int x = numbers[i] + numbers[j];
            if(x == target){ 
                r.push_back(numbers[i]);
                r.push_back(numbers[j]);
                i++; j--;
            }else if(x > target) j--;
            else i++;
        }
        return r;
    }
};
```
**2、编号14 3Sum**
Given an array S of n integers, are there elements a, b, c in S such that a + b + c = 0? Find all unique triplets in the array which gives the sum of zero.
Note:
Elements in a triplet (a,b,c) must be in non-descending order. (ie, a ≤ b ≤ c)
The solution set must not contain duplicate triplets.
For example, given array S = {-1 0 1 2 -1 -4},
    A solution set is:
    (-1, 0, 1)
    (-1, -1, 2)
该题要求a + b + c = 0, 把-c看成上题的target的话，可以直接使用上一题的方法。
```cpp
class Solution {
public:
    vector<vector<int> > threeSum(vector<int> &num) {  
        vector<vector<int> >  result;  
        sort(num.begin(), num.end());  
        
        /*Call 2 sum*/
        for(int i = 0; i < num.size(); i++){  
            if(i > 0 && num[i] == num[i-1]) continue;  
            twoSum(num, result, i);
        } 
        return result;  
    }
    
    /*2 sum algorithm. different from the other one is that the other one return index not value*/
    void twoSum(vector<int> &num, vector<vector<int>> &res, int targetIndex) {
	    /*starting from targetIndex + 1 is because the numbers before have been used*/
            int i = targetIndex + 1;   
            int j = num.size()-1;  
            
            while(i < j){  
                if(num[targetIndex] + num[i] + num[j] < 0)   i++;  
                else if(num[targetIndex] + num[i] + num[j] > 0)   j--;  
                else{  
                    vector<int> v;  
                    v.push_back(num[targetIndex]);  
                    v.push_back(num[i]);  
                    v.push_back(num[j]);  
                    res.push_back(v);  
                    i++; j--;  
                    while(i < num.size() && num[i]==num[i - 1]) i++;  
                    while(j >= 0 && num[j] == num[j + 1]) j--;  
                }  
            }
            
    }
};
```
**3、编号16 3Sum Closest**
Given an array S of n integers, find three integers in S such that the sum is closest to a given number, target. Return the sum of the three integers. You may assume that each input would have exactly one solution.
For example, given array S = {-1 2 1 -4}, and target = 1.
The sum that is closest to the target is 2. (-1 + 2 + 1 = 2).
跟上面两题一个方法...
```cpp
class Solution {
public:
    int threeSumClosest(vector<int> &num, int target) {
        int result = 0;
        int remainder = INT_MAX
        
        sort(num.begin(), num.end());
        
        for(int i = 0; i < num.size(); i++)
            twoSum(num, i, result, remainder, target);
        
        return result;
    }
    
    void twoSum(vector<int> num, int targetIndex, int& result, int &remainder, int target){
        int i = targetIndex + 1; 
        int j = num.size() - 1; 
        
        while(i < j){
            int thisRemainder = num[targetIndex] + num[i] + num[j] - target;
            if(abs(thisRemainder) <= remainder) {
                remainder = abs(thisRemainder);
                result = num[targetIndex] + num[i] + num[j];
            }
            if(thisRemainder > 0) j--; 
            else i++;
            
        }
        
    }
};
```
**4、编号18 4Sum Total**
Given an array S of n integers, are there elements a, b, c, and d in S such that a + b + c + d = target? Find all unique quadruplets in the array which gives the sum of target.
Note:
Elements in a quadruplet (a,b,c,d) must be in non-descending order. (ie, a ≤ b ≤ c ≤ d)
The solution set must not contain duplicate quadruplets.
For example, given array S = {1 0 -1 0 -2 2}, and target = 0.
    A solution set is:
    (-1,  0, 0, 1)
    (-2, -1, 1, 2)
    (-2,  0, 0, 2)
跟3sum相比只是多了一重循环而已。
```cpp
class Solution {
public:
    vector<vector<int> > fourSum(vector<int> &num, int target) {
        vector<vector<int> >  result;  
         
        if(num.size() < 4) return result;
        sort(num.begin(), num.end()); 
        
        for(int i = 0; i < num.size(); i++){  
            if(i > 0 && num[i] == num[i-1]) continue;  
            for(int j = i+1; j < num.size(); j++){
                if(j > i+1 && num[j] == num[j-1]) continue;  
                twoSum(num, result, i, j, target);
            }
        } 
        return result;  
        
    }
    
    void twoSum(vector<int> &num, vector<vector<int>> &res, int targetIndex, int targetIndex2, int target) {
            int i = targetIndex2 + 1; 
            int j = num.size()-1;  
            
            while(i < j){  
                if(num[targetIndex] + num[targetIndex2] + num[i] + num[j] < target)   i++;  
                else if(num[targetIndex] + num[targetIndex2] + num[i] + num[j] > target)   j--;  
                else{  
                    vector<int> v;  
                    v.push_back(num[targetIndex]);  
                    v.push_back(num[targetIndex2]);  
                    v.push_back(num[i]);  
                    v.push_back(num[j]);  
                    res.push_back(v);  
                    i++; j--;  
                    while(i < num.size() && num[i]==num[i - 1]) i++;  
                    while(j >= 0 && num[j] == num[j + 1]) j--;  
                }  
            }
            
    }
    
    
    
};
```
**5、编号38 Combination Sum**
Given a set of candidate numbers (C) and a target number (T), find all unique combinations in C where the candidate numbers sums to T. 
The same repeated number may be chosen from C unlimited number of times. 
Note:
All numbers (including target) will be positive integers.
Elements in a combination (a1, a2, … , ak) must be in non-descending order. (ie, a1 ≤ a2 ≤ … ≤ ak).
The solution set must not contain duplicate combinations.
For example, given candidate set 2,3,6,7 and target 7, 
A solution set is: 
[7] 
[2, 2, 3] 
组合问题一般用DFS来做。与编号132（Palindrome Partitioning）类似。
```cpp
class Solution {
public:
    vector<vector<int> > combinationSum(vector<int> &candidates, int target) {
        int sum = 0;
        vector<int> currentResult;
        vector<vector<int> > result;
        sort(candidates.begin(), candidates.end());
        Recursive(candidates, target, 0, currentResult, result, sum);
        return result;
    }
    
    
    void Recursive(vector<int> &candidates, int target, int level, vector<int> ¤tResult, vector<vector<int>> &result, int &sum ){
        if(target < sum) return;
        if(target == sum){
            result.push_back(currentResult);
            return;
        }
        for(int i = level; i < candidates.size(); i++){
            sum += candidates[i];
            currentResult.push_back(candidates[i]);
            
            Recursive(candidates, target, i, currentResult, result, sum);
            
            sum -= candidates[i];
            currentResult.pop_back();
            
        }
        
    }
};
```
**6、编号39 Combination Sum II**
Given a collection of candidate numbers (C) and a target number (T), find all unique combinations in C where the candidate numbers sums to T.
Each number in C may only be used once in the combination.
Note:
All numbers (including target) will be positive integers.
Elements in a combination (a1, a2, … , ak) must be in non-descending order. (ie, a1 ≤ a2 ≤ … ≤ ak).
The solution set must not contain duplicate combinations.
For example, given candidate set 10,1,2,7,6,1,5 and target 8, 
A solution set is: 
[1, 7] 
[1, 2, 5] 
[2, 6] 
[1, 1, 6] 
跟上题差不多，但是每个数字只能用一次。所以在递归的时候，下一次递归的开始要用i+1而不是i。另外为了避免重复，在递归函数最后还要加一句。
```cpp
class Solution {
public:
    vector<vector<int> > combinationSum2(vector<int> &num, int target) {
        int sum = 0;
        vector<int> currentResult;
        vector<vector<int> > result;
        sort(num.begin(), num.end());
        Recursive(num, target, 0, currentResult, result, sum);
        return result;
    }
    
    void Recursive(vector<int> &candidates, int target, int level, vector<int> ¤tResult, vector<vector<int>> &result, int &sum ){
        if(target < sum) return;
        if(target == sum){
            result.push_back(currentResult);
            return;
        }
        for(int i = level; i < candidates.size(); i++){
            sum += candidates[i];
            currentResult.push_back(candidates[i]);
            
            Recursive(candidates, target, i+1, currentResult, result, sum);/*Difference*/
            sum -= candidates[i];
            currentResult.pop_back();
            
            while((i+1) < candidates.size() && candidates[i] == candidates[i+1]) i++; /*Difference*/
        }
    }
};
```
**7、编号53 Maximum Subarray**
Find the contiguous subarray within an array (containing at least one number) which has the largest sum.
For example, given the array[−2,1,−3,4,−1,2,1,−5,4],
the contiguous subarray [4,−1,2,1] has the largest sum = 6.
1维DP可解。。。下面的解法很巧妙，只用常数空间，比DP更优。思路是只要部分和是正数就收入。假如部分和为零就更新为零。
```cpp
class Solution {
public:
    int maxSubArray(int A[], int n) {  
      int sum = 0;  /*Store current elements*/
      int max = INT_MIN;
      for(int i = 0; i < n ; i++)  {  
          sum += A[i];        
          if(sum > max) max = sum;  
          if(sum < 0) sum = 0;  
      }  
      return max;  
    } 
};
```
**8、编号62 Minimum Path Sum**
Given a m x n grid filled with non-negative numbers, find a path from top left to bottom right which minimizes the sum of all numbers along its path.
Note: You can only move either down or right at any point in time.
2维DP求解。先用输入更新所有的dp数组。然后更新第一排和第一列。之后dp[i][j] += min(dp[i-1][j], dp[i][j-1])
```cpp
class Solution {
public:
    int minPathSum(vector<vector<int> > &grid) {
        int m = grid.size();
        if(m == 0) return 0;
        int n = grid[0].size();
        if(n == 0) return 0;
        
        if(m == 1 && n == 1) return grid[0][0];
        
        vector<vector<int>> dp;
        for(int i = 0; i < m; i++) {
            vector<int> tmp;
            for(int j = 0; j < n; j++)tmp.push_back(grid[i][j]);
            dp.push_back(tmp);
        }
        
        for(int i = 1; i < m; i++) dp[i][0] += dp[i-1][0];
        for(int j = 1; j < n; j++) dp[0][j] += dp[0][j-1];
        
        for(int i = 1; i < m; i++) 
            for(int j = 1; j < n; j++)
                dp[i][j] += min(dp[i-1][j], dp[i][j-1]);
        return dp[m-1][n-1];
    }
};
```
**9、编号65 Add Binary**
Given two binary strings, return their sum (also a binary string).
For example,
a = "11"
b = "1"
Return "100".
思路比较简单，直接一位一位地加。下面的方法是把输入倒个，然后从前往后加，最后得到的结果就是正的。
```cpp
class Solution {
public:
    string addBinary(string a, string b) {  
      string result;  
      int maxSize = max(a.size(), b.size());  
      std::reverse(a.begin(), a.end());  
      std::reverse(b.begin(), b.end());  
      int carry = 0;  
      for(int i =0; i < maxSize;i++){  
        int ai = i < a.size() ? a[i] - '0' : 0;  
        int bi = i < b.size() ? b[i] - '0' : 0;  
        int val = (ai + bi + carry) % 2;  
        carry = (ai + bi + carry) / 2;  
        result.insert(result.begin(), val + '0');  
      }
      if(carry == 1) result.insert(result.begin(), '1');  
      return result;  
    } 
};
```
**10、编号121 Triangle**
Given a triangle, find the minimum path sum from top to bottom. Each step you may move to adjacent numbers on the row below.
For example, given the following triangle
[
     [2],
    [3,4],
   [6,5,7],
  [4,1,8,3]
]
The minimum path sum from top to bottom is 11 (i.e., 2 + 3 + 5 + 1 = 11).
Note:
Bonus point if you are able to do this using only O(n) extra space, where n is the total number of rows in the triangle.
跟上面编号62一样做2维DP就可以了。。。
```cpp
class Solution {
public:
    int minimumTotal(vector<vector<int> > &triangle) {
        vector<vector<int> > dp;
        int h = triangle.size();
        if(h == 0) return 0;
        
        for(int i = 0; i < h; i++){
            vector<int> tmp;
            for(int j = 0; j <= i; j++) tmp.push_back(0);
            dp.push_back(tmp);
        }
        dp[0][0] = triangle[0][0];
        
        for(int i = 1; i < h; i++){
            for(int j = 0; j <= i; j++){
                if(j == 0) dp[i][j] = dp[i-1][0] + triangle[i][j];
                else if(j == i) dp[i][j] = dp[i-1][j-1] + triangle[i][j];
                else dp[i][j] = min(dp[i-1][j-1], dp[i-1][j]) + triangle[i][j];
            }
        }
        
        int min = INT_MAX;
        for(int i = 0; i < h; i++)
            if(dp[dp.size()-1][i] < min) min = dp[dp.size()-1][i];
            
        return min;
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
