# Leetcode全回文问题 - 無名黑洞 - CSDN博客
2014年02月04日 10:12:30[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1416
**目录：**
**1、编号5 Longest Palindromic Substring2、编号9 Palindrome Number3、编号126 Valid Palindrome4、编号132 Palindrome Partitioning (DFS)5、编号133 Palindrome Partitioning II (DP)**
**1、编号5 Longest Palindromic Substring**
Given a string S, find the longest palindromic substring in S. You may assume that the maximum length of S is 1000, and there exists one unique longest palindromic substring.
寻找最大子回文串。回文类的基本题。O(n^2)的方法很容易写出，但过不了大数据。下面方法是在每个元素之间添加'#'的创新方法，把时间效率提高到O(n)
```cpp
class Solution {
public:
    string longestPalindrome(string s) {
        /*1.Initialization*/
        int len = s.length();
        int newLen = len*2+1;
        
        /*1.5 Establish s2 and add #*/
        string s2(newLen, 0); 
        s2[0] = '#';
        for(int i = 0; i < len; i++){
            s2[2*i+1] = s[i];
            s2[2*i+2] = '#';
        }
        
        /*2.Find radius of all characters*/
        int center_i = 0; 
        int high_i = 0;
        int *radius = new int[newLen+1];
        for(int i = 0; i < newLen; i++) radius[i] = 0;
        for(int i = 0; i < newLen; i++){
            /*Use previous data to current state*/
            if(high_i > i){
                int tmp = center_i*2 - i;
                if(tmp < 0) tmp = 0;
                radius[i] = min(radius[tmp], high_i - i);
            }
            /*Expand current state*/
            while(s2[i-1-radius[i]] == s2[i+1+radius[i]]) radius[i]++;
            /*Update range*/
            if(i+radius[i] > high_i){
                center_i = i;
                high_i = i + radius[i];
            }
            
        }
        
        /*3.Fnd the charactor which has max ridius*/
        int cen = 0;
        int rad = 0;
        for(int i = 0; i < newLen; i++){
            if(radius[i] > rad){
                cen = i;
                rad = radius[i];
            }
        }
        return RemoveSharp(s2.substr(cen-rad, 2*rad));
    }
    
    string RemoveSharp(string s){
        int len = s.length();
        string r;
        for(int i = 0; i < len; i++)
            if(s[i] != '#') r += s[i];
        return r;
    }
};
```
**2、编号9 Palindrome Number**
Determine whether an integer is a palindrome. Do this without extra space.
Some hints: 
Could negative integers be palindromes? (ie, -1)
If you are thinking of converting the integer to string, note the restriction of using extra space.
You could also try reversing an integer. However, if you have solved the problem "Reverse Integer", you know that the reversed integer might overflow. How would you handle such case?
There is a more generic way of solving this problem.
判断一个整型数字是否是回文。难度不大，但是要求不使用额外空间。所以就不一次性把数字转化，而是每次循环转换当前的最高位和最低位。
```cpp
class Solution {
public:
    bool isPalindrome(int x) {
        
        if( x < 0 ) return false;
        
        int count = 0;
        int y = x;
        while(y != 0){
            y = y / 10;
            count++;
        }
        
        if(count == 0 || count == 1) return true;
        
        while(count > 1){
            int l = RemoveLowest(&x);
            count--;
            int h = RemoveHighest(&x, count);
            count--;
            if(l!=h) return false;
        }
        
        return true;
    }
    
    int RemoveLowest(int *x){
        int r = (*x) % 10;
        *x = (*x) / 10;
        return r;
    }
    
    int RemoveHighest(int *x, int count){
        int denom = pow(10,count-1);
        
        int r = (*x) / denom;
        *x = (*x) - r * denom;
        return r;
    }
};
```
**3、编号126 Valid Palindrome**
Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.
For example,
"A man, a plan, a canal: Panama" is a palindrome.
"race a car" is not a palindrome.
Note:
Have you consider that the string might be empty? This is a good question to ask during an interview.
For the purpose of this problem, we define empty string as valid palindrome.
判断字符串是否是回文。很简单的函数。但注意此题要求忽略非字母和数字，另外要考虑大小写，所以要专门处理这两部分。
```cpp
class Solution {
public:
    bool isPalindrome(string s) {
        int begin = 0;
        int end = s.length()-1;
        
        while(begin < end){
            if(!CheckAlphanumeric(s[begin])) {
                begin++; continue;
            }
            if(!CheckAlphanumeric(s[end])) {
                end--; continue;
            }
            
            if(!CheckCase(s[begin], s[end])) return false;
            else{ begin++; end--; }
        }
        
        return true;
    }
    
    bool CheckAlphanumeric(char x){
        if(x >= '0' && x <='9') return true;
        if(x >= 'a' && x <= 'z') return true;
        if(x >= 'A' && x <= 'Z') return true;
        return false;
    }
    
    bool CheckCase(char x, char y){//!!!!!!!!!!!!!!
        if(tolower(x) == tolower(y)) return true;
        else return false;
    }
};
```
**4、编号132 Palindrome Partitioning**
Given a string s, partition s such that every substring of the partition is a palindrome.
Return all possible palindrome partitioning of s.
For example, given s = "aab",
Return
  [
    ["aa","b"],
    ["a","a","b"]
  ]
这种要求列出所有结果的题目就使用BFS或DFS做。这题用DFS以O(n^2)可以写出如下结果。这样的DFS方法可以解很多类似的问题。
```cpp
class Solution {
public:
    vector<vector<string>> partition(string s) {  
        vector<vector<string>> result;  
        vector<string> r;  
        DFS(s, 0, r, result);  
        return result;  
    } 
      
    void DFS(string &s, int level, vector<string>& r, vector<vector<string>> &result) {      
        if(level == s.size()) {  
            result.push_back(r);  
            return;  
        }  
        for(int i = level; i < s.size(); i++) {    /*O(n^2)*/
            if(isPalindrome(s, level, i)) {  
                r.push_back(s.substr(level, i + 1 - level));  //substr
                DFS(s, i+1, r, result);  
                r.pop_back();  
            }  
        }  
    }  
    
    bool isPalindrome(string &s, int start, int end) {  
        while(start < end) {  
            if(s[start] != s[end])  return false;  
            start++; 
            end--;  
        }  
        return true;  
    }  
};
```
**5、编号133 Palindrome Partitioning II**
Given a string s, partition s such that every substring of the partition is a palindrome.
Return the minimum cuts needed for a palindrome partitioning of s.
For example, given s = "aab",
Return 1 since the palindrome partitioning ["aa","b"] could be produced using 1 cut.
跟上题差不多，区别是要找到最小切割。如果按照上题思路列出所有答案然后找其中最小的，会超时。所以该用1维DP来解答。思路是建立一个数组dp，初始化为n, n-1, n-2,...,1。dp[i]表示在区间i~n-1之间虽小的cut数。另外建立一个二维辅助数组isPalindrome。isPalindrome[i][j]判断[i,j]是否是回文。如果区间[i, j]为回文的话，dp[i] = min(dp[i], dp[j+1] + 1)。
```cpp
class Solution {
public:
    int minCut(string s) {  
        int len = s.size();  
        
        /*Initialize*/
        int dp[len + 1];  
        for(int i = 0; i <= len; i++)   dp[i] = len-i;  
        
        bool isPalindrome[len][len];  
        for(int i = 0; i < len; i++)  
            for(int j = 0; j < len; j++)  
                isPalindrome[i][j] = false;  
                
        for(int i = len-1; i >= 0; i--)  
            for(int j = i; j < len; j++)  
                if(s[i] == s[j] && ((j - i) < 2 || isPalindrome[i+1][j-1] == true)){  
                    isPalindrome[i][j] = true;  
                    dp[i] = min(dp[i], dp[j+1] + 1);  
                }
        
        return dp[0]-1;  
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
