# Leetcode全排列问题 - 無名黑洞 - CSDN博客
2014年02月07日 01:00:41[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1206
**目录**
**1、编号30 Next Permutation2、编号44 Permutations3、编号45 Permutations II4、编号60 Permutation Sequence**
**1、编号30 Next Permutation**
Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.
If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).
The replacement must be in-place, do not allocate extra memory.
Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.
1,2,3 → 1,3,2
3,2,1 → 1,2,3
1,1,5 → 1,5,1
寻找当前数字组合的下一个排列。从后往前用排序做。
```cpp
void nextPermutation (vector<int> &num) {
    int i;
    int j = num.size()-1;
    
    for(i = num.size()-1; i > 0; i--){
        if(num[i-1] < num[i]) {
            while(num[i-1] >= num[j]) j--;
            swap(num[i-1], num[j]);
            sort(num.begin()+i, num.end());
            break;
        }
    }
    
    if(i==0) sort(num.begin(), num.end());
}
    
void swap(int &x, int &y){
    int tmp = x;
    x = y;
    y = tmp;
}
```
**2、编号44 Permutations**
Given a collection of numbers, return all possible permutations.
For example,
[1,2,3] have the following permutations:
[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], and [3,2,1].
三重循环。
```cpp
class Solution {
public:
    vector<vector<int> > permute(vector<int> &num) {                
        vector<vector<int>> result;  
        vector<int> initElem;
        
        sort(num.begin(), num.end());
        
        initElem.push_back(num[0]);
        result.push_back(initElem);
        for(int i = 1; i < num.size(); i++) result = Insert(result, num[i]);
        return result;                                              
    }                                                           
        
    vector<vector<int> > Insert(vector<vector<int>> result, int num){
        vector<vector<int>> r;
        for(int i = 0; i < result.size(); i++){
            for(int j = 0; j < result[i].size() + 1; j++){
                vector<int> tmp;
                tmp = result[i];
                tmp.insert(tmp.begin()+j, num);
                r.push_back(tmp);
            }
        }
        
        return r;
    }
};
```
**3、编号45 Permutations II**
Given a collection of numbers that might contain duplicates, return all possible unique permutations.
For example,
[1,1,2] have the following unique permutations:
[1,1,2], [1,2,1], and [2,1,1].
DFS
```cpp
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int> &num) {  
        vector<vector<int>> result;  
        vector<int> oneResult; 
        vector<bool> visited(num.size(), 0);  
        
        if(num.size() == 0) return result;  
        
        sort(num.begin(), num.end()); 
        
        GeneratePermute(num, 0, visited, oneResult, result);  
        return result;  
    }  
    void GeneratePermute(vector<int> & num, int level, vector<bool>& visited, 
                vector<int>& oneResult, vector<vector<int> >& result) {  
        if(level == num.size())  {  
            result.push_back(oneResult);  
            return;  
        }  
        for(int i = 0; i< num.size(); i++)  {  
            if(visited[i] == false){  
                if(i > 0 && num[i] == num[i-1] && visited[i-1] == false)  
                    continue;  
                    
                visited[i] = true;  
                oneResult.push_back(num[i]);  
                
                GeneratePermute(num, level+1, visited, oneResult, result);  
                oneResult.pop_back();  
                visited[i] = false;  
            }  
        }  
    }
```
**4、编号60 Permutation Sequence**
The set [1,2,3,…,n] contains a total of n! unique permutations.
By listing and labeling all of the permutations in order,
We get the following sequence (ie, for n = 3):
"123"
"132"
"213"
"231"
"312"
"321"
Given n and k, return the kth permutation sequence.
Note: Given n will be between 1 and 9 inclusive.
这一题纯粹就是找规律了。
```cpp
class Solution {
public:
    string getPermutation(int n, int k) {
        string resultString;
        vector<int> input;
        for(int i = 0; i < n; i++) input.push_back(i+1);
 
        if(input.size() == 0) return resultString;
        
        k--;/*!!!!!!!!*/
        
        while(input.size() > 1){
            int f = factorization(input.size()-1);
            int pos = k/f;
            resultString += input[pos] + '0';
            input.erase(input.begin() + pos);
            
            /*Update k*/
            k = k%f;
        }
        resultString += input[0] + '0';
        
        return resultString;
    }
    
    int factorization(int n){
        if(n == 0) return 1;
        int r = 1;
        for(int i = n; i > 0; i--) r *= i;
        return r;
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
