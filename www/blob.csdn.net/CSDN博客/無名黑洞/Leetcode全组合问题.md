# Leetcode全组合问题 - 無名黑洞 - CSDN博客
2014年02月05日 23:36:51[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1363
**目录**
**1、编号17 Letter Combinations Of Phone Number（DFS）2、编号21 Generate Parentheses（DFS）3、编号77 Combinations（DFS）4、编号94 Restore IP Addresses（DFS）**
**1、编号17 Letter Combinations Of Phone Number**
Given a digit string, return all possible letter combinations that the number could represent. 
A mapping of digit to letters (just like on the telephone buttons) is given below.
Input:Digit string "23"
Output: ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].
Note: Although the above answer is in lexicographical order, your answer could be in any order you want. 
组合问题里面比较简单的类型，因为键盘上只有9个数字，可以分成四种情况。用DFS解。
```cpp
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        vector<string> r;
        ProcessStringDigits(r, digits, 0, "");
        return r;
    }
    
    void ProcessStringDigits(vector<string> &r, string digits, int index, string c){
        if(index == digits.size()){/*End of this*/
            r.push_back(c);
            return;
        }else{/*keep processing*/
            if(digits[index] == '7') {
                int num = (digits[index]- '0' - 2) * 3;
                for(int i = 0; i < 4; i++) ProcessStringDigits(r, digits, index + 1, c + (char)((int)'a' + num + i));
            }else if(digits[index] == '8') {
                int num = (digits[index]- '0' - 2) * 3 + 1;
                for(int i = 0; i < 3; i++) ProcessStringDigits(r, digits, index + 1, c + (char)((int)'a' + num + i));
            }else if(digits[index] == '9'){
                int num = (digits[index]- '0' - 2) * 3 + 1;
                for(int i = 0; i < 4; i++) ProcessStringDigits(r, digits, index + 1, c + (char)((int)'a' + num + i));
            }else{
                int num = (digits[index]- '0' - 2) * 3;
                for(int i = 0; i < 3; i++) ProcessStringDigits(r, digits, index + 1, c + (char)((int)'a' + num + i));
            }
        }
        
    }
};
```
**2、编号21 Generate Parentheses**
Given n pairs of parentheses, write a function to generate all combinations of well-formed parentheses.
For example, given n = 3, a solution set is:
"((()))", "(()())", "(())()", "()(())", "()()()"
首先确定输入是什么。如果有 n组括号，那么就是n个左括号和n个右括号。用两个变量来追踪它，当左括号和右括号为零的时候循环结束。当每次加入新符号的时候也只有两种情况。使用DFS来循环这两种情况就可以了。另外要注意的是，任何时候，右括号的数量不可以超过左括号，所以用一个额外的变量来记录结果里面有多少个还没配对的左括号。
```cpp
class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> result;
        result.push_back("(");
        ParenthesisLoop(result, n-1, n, 0, 1);
        return result;
    }
    
    void ParenthesisLoop(vector<string> &result, int l, int r, 
        int mainIndex, int leftLeft){
        /*End Condition: r==0*/
        if(l == 0 || r== 0){
            for(int i = 0; i < leftLeft; i++) result[mainIndex] += ")";
            return;
        }
        
        /*Loop Condition: r>0 && l>0*/
        if(leftLeft > 0){ /*can add both side parenthesis*/
            result.push_back(result[mainIndex] + ")");
            ParenthesisLoop(result, l, r-1, result.size()-1, leftLeft-1);
            
            result[mainIndex] += "(";
            ParenthesisLoop(result, l-1, r, mainIndex, leftLeft+1);
        }else{/*leftLeft == 0, can only add left parenthesis*/
            result[mainIndex] += "(";
            ParenthesisLoop(result, l-1, r, mainIndex, leftLeft+1);
        }
    }
};
```
****3、编号77 Combinations****
Given two integers n and k, return all possible combinations of k numbers out of 1 ... n.
For example,
If n = 4 and k = 2, a solution is:
[
  [2,4],
  [3,4],
  [2,3],
  [1,2],
  [1,3],
  [1,4],
]
DFS解决组合题的经典案例。可以背下来的那种。
```cpp
class Solution {
public:
    vector<vector<int> > combine(int n, int k) {
        vector<vector<int>> result;
        vector<int> oneResult;
        Insert(result, oneResult, 0, k, n);
        return result;
    }
    
    void Insert(vector<vector<int>> &result, vector<int> &oneResult, int level, int k, int n){
        if(oneResult.size() == k){//use oneResult.size() instead of level
            result.push_back(oneResult);
            return;
        }
        
        for(int i = level; i < n; i++){
            oneResult.push_back(i+1);
            Insert(result, oneResult, i+1, k, n);
            oneResult.pop_back();
        }
    }
};
```
**4、编号94 Restore IP Addresses**
Given a string containing only digits, restore it by returning all possible valid IP address combinations.
For example: Given "25525511135",
return ["255.255.11.135", "255.255.111.35"]. (Order does not matter)
跟普通组合题相比，有两个注意点。一是有效元素判定。数字必须是0～255之间。并且01、055这种数字是不行的。第二是因为入栈的是string，就不用push和pop了。
```cpp
class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> result;
        string oneResult = "";
        Insert(s, 0, 0, oneResult, result);
        return result;
    }
    
    void Insert(string s, int level, int num, string oneResult, vector<string> &result){
        if(num == 4){
            if(level == s.length()){
                oneResult.resize(oneResult.length()-1);
                result.push_back(oneResult);
            }
            return;
        }
        
        string tmp = "";
        for(int i = level; i < level+3; i++){
            if(i >= s.length()) return;
            tmp += s[i];
            if(CheckValid(tmp)){
                Insert(s, i+1, num+1, oneResult + tmp + '.', result);
            }else return;
        }
    }
    
    bool CheckValid(string x){
        int mul = 1;
        int r = 0;
        for(int i = x.length()-1; i >= 0; i-- ){
            r += (x[i]-'0') * mul;
            mul = mul*10;
        }
        
        if(r <= 255 && r > 0 && x[0] != '0') return true;//!!!!!!!!!!! x[0] != '0'
        
        if(r == 0 && x.length() == 1) return true;
        
        return false;
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
