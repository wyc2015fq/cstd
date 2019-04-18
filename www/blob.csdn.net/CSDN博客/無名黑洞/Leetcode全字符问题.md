# Leetcode全字符问题 - 無名黑洞 - CSDN博客
2014年02月07日 08:09:26[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：3491
**目录**
**1、编号3 Longest Substring Without Repeating Characters2、编号6 ZigZag Conversion3、编号20 Valid Parentheses4、编号28 Implement strStr()5、编号31 Longest Valid Parentheses6、编号42 Multiply Strings7、编号57 Length of Last Word8、编号71 Simplify Path9、编号72 Edit Distance10、编号76 Minimum Window Substring11、编号91 Decode Ways12、编号98 Interleaving String13、编号116 Distinct Subsequences14、编号127 Word Ladder15、编号128 Word Ladder II16、编号140 Word Break17、编号141 Word Break II**
**1、编号3 Longest Substring Without Repeating Characters**
Given a string, find the length of the longest substring without repeating characters. For example, the longest substring without repeating letters for "abcabcbb" is "abc", which the length is 3. For "bbbbb" the longest substring is "b", with the length
 of 1.
一开始用二重循环O(n)的双指针解法。使用了长度为256的辅助数组记录是否碰到相同的字母。如果没找到相同字母，快指针向后走。如果找到了，慢指针走两步，快指针走一步。
```cpp
class Solution {
public:
    //For example, if the string is “abcdcedf”, what happens when you reach the second appearance of ‘c’?
    int lengthOfLongestSubstring(string s) {
        int i = 0, j = 0;
        int result = 0;
        bool exist[256] = { false };
        while(j < s.length()) {
            if(exist[s[j]]){
                result = max(result, j - i);
                while(s[i] != s[j]){
                    exist[s[i]] = false;
                    i++;
                }
                i++;
                j++;
            }else{
                exist[s[j]] = true;
                j++;
            }
        }
        result = max(result, (int)s.length() - i);
        return result;
    }
};
```
**2、编号6 ZigZag Conversion**
The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this: (you may want to display this pattern in a fixed font for better legibility) 
P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: "PAHNAPLSIIGYIR"
Write the code that will take a string and make this conversion given a number of rows: 
string convert(string text, int nRows);
convert("PAYPALISHIRING", 3) should return "PAHNAPLSIIGYIR". 
这题很无语了，找数学公式，把字符分为红色和绿色两类。indexRed = (n-1)*2*j+i, indexGreen = indexRed+2*(n-i-1)
```cpp
class Solution {
	public:
	string convert(string s, int nRows){
		/*Initialization*/
		if(nRows <= 1) return s;
		string result;
		if(s.size() ==0) return result;
        
		for(int i =0; i< nRows; i++)
			for(int j =0, indexRed = i; indexRed < s.size(); j++, indexRed = (nRows-1)*2*j+i){
				/*Red Principal*/
				result.append(1, s[indexRed]);
				/*Green*/
				if(i ==0 || i == nRows-1) continue;
				int indexGreen = indexRed + 2*(nRows - i - 1); /*Times 2 to go a v shape to get green index*/
				if(indexGreen < s.size())  result.append(1, s[indexGreen]);
			}
		return result;
	}
};
```
**3、编号20 Valid Parentheses**
Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
The brackets must close in the correct order, "()" and "()[]{}" are all valid but "(]" and "([)]" are not.
括号问题都不难解决，注意左括号和右括号分别处理。
```cpp
class Solution {
public:
    bool isValid(string s) {
        stack<char> sk;
        
        for(int i = 0; i < s.length(); i++){
            if(s[i] == '(' || s[i] == '[' || s[i] == '{') sk.push(s[i]);
            else{
                if(sk.empty()) return false;
                if(s[i] == ')' && sk.top() != '(') return false;
                else if(s[i] == ']' && sk.top() != '[') return false;
                else if(s[i] == '}' && sk.top() != '{') return false;
                sk.pop();
            }
        }
        if(sk.empty()) return true;
        else return false;
    }
};
```
**4、编号28 Implement strStr()**
Returns a pointer to the first occurrence of needle in haystack, or null if needle is not part of haystack.
haystack是目标字符串，needle是匹配字符串。
```cpp
class Solution {
public:
    char *strStr(char *haystack, char *needle) {
        if((*needle) == '\0') return haystack;
        
        char *hayLoop = haystack;
        int needleLength = strlen(needle);
        int index = 0;
        int inc = 1;
        
        while(*(hayLoop + needleLength - 1) != '\0'){
            if(hayLoop[index] == needle[index]){
                index++;
                /*Add this only to improve perfomance*/
                /*Refer KMP algorithm*/
                if(hayLoop[index] == needle[0] && inc==1) 
                    inc = index - 1;
            }else{
                hayLoop += (inc==0?1:inc);
                index = 0;
                inc = 1;
            }
            
            if(index == needleLength) return hayLoop;
        }
  
        return NULL;
    }
};
```
**5、编号31 Longest Valid Parentheses**
Given a string containing just the characters '(' and ')', find the length of the longest valid (well-formed) parentheses substring.
For "(()", the longest valid parentheses substring is "()", which has length = 2.
Another example is ")()())", where the longest valid parentheses substring is "()()", which has length = 4.
线性时间的话要用dp来做。
```cpp
class Solution {  
public:  
    int longestValidParentheses(string s) {  
        /*DP result O(N)*/
        /*初始dp[i]==i*/
        /*使用dp[i]==j，表示i到某个j结点能够匹配*/
        /*Initial*/
        int result = 0;  
        int dp[s.size()];  
        for(int i = 0; i < s.size(); i++)   dp[i] = i;  
        
        /*Fill dp*/
        for(int i = 1; i < s.size(); i++)  {  
            if(s[i] == '(')  continue;  //i is the next element. if next is '(' continue)
           
            int x = i - 1; //x is set to connect all nodes one element back
            
            if(dp[x] != x) x = dp[x] - 1; //if node x can not connect to node x, x is set back again  
            if(s[x] == '(') {   //if current is '(')
                dp[i] = x;  
                x--;  
                if(dp[x] != x)      x = dp[x] - 1;  
                if(dp[i] != x + 1)  dp[i] = x + 1;  
            }  
            if(i != dp[i] && ((i - dp[i] + 1) > result))  result = i - dp[i] + 1;  
        }  
        return result;  
    }
```
**6、编号42 Multiply Strings**
Given two numbers represented as strings, return multiplication of the numbers as a string.
Note: The numbers can be arbitrarily large and are non-negative.
字符串和数字转化问题。注意reverse的运用
```cpp
class Solution {
public:
    string multiply(string num1, string num2) {  
      if(num1.size() ==0 || num2.size() ==0) return 0;  
      string result(num1.size()+num2.size()+1, '0');  
      std::reverse(num1.begin(), num1.end());  
      std::reverse(num2.begin(), num2.end());  
      for(int i =0; i < num1.size(); i++)  {  
          int dig1 = num1[i] -'0';  
          int carry = 0;  
          for(int j = 0; j< num2.size(); j++)  {  
            int dig2 = num2[j] - '0';  
            int exist = result[i+j] -'0';          
            result[i+j] = (dig1*dig2+carry+ exist) % 10 +'0';    
            carry = (dig1*dig2+carry+exist)/10;  
          }  
          if(carry >0)  result[i+num2.size()] = carry + '0';  
      }  
      std::reverse(result.begin(), result.end());  
      int start = 0;  
      while(result[start] =='0' && start < result.size())  start++;  
      if(start == result.size()) return "0";  
      return result.substr(start, result.size()-start);  
    }  
};
```
**7、编号57 Length of Last Word**
Given a string s consists of upper/lower-case alphabets and empty space characters ' ', return the length of last word in the string.
If the last word does not exist, return 0.
Note: A word is defined as a character sequence consists of non-space characters only.
For example,  Given s = "Hello World", return 5.
注意最后一个字符有可能是空格。
```cpp
class Solution {
public:
    int lengthOfLastWord(const char *s) {
        if(*s == NULL) return 0;
        
        const char* p;
        p = s;
        bool inWord = false;
        int length = 0;
        
        while(*p != '\0'){
            if(*p != ' ') {
                if(!inWord) length = 0;
                inWord = true;
                length++;
            }else inWord = false;
            p++;
        }
        
        return length;
    }
};
```
**8、编号71 Simplify Path**
For example,
path = "/home/", => "/home"
path = "/a/./b/../../c/", => "/c"
click to show corner cases.
Corner Cases:
Did you consider the case where path = "/../"?
In this case, you should return "/".
Another corner case is the path might contain multiple slashes '/' together, such as "/home//foo/".
In this case, you should ignore redundant slashes and return "/home/foo".
一开始没看明白题。后来看了网上答案用栈来实现。
1. 等于“/”，跳过，直接开始寻找下一个element
2. 等于“.”，什么都不需要干，直接开始寻找下一个element
3. 等于“..”，弹出栈顶元素，寻找下一个element
4. 等于其他，插入当前elemnt为新的栈顶，寻找下一个element
最后，再根据栈的内容，重新拼path。这样可以避免处理连续多个“/”的问题。
```cpp
class Solution {
public:
        string simplifyPath(string path) {   
            vector<string> stack;   
            assert(path[0]=='/');   
            int i=0;   
            while(i< path.size())   
            {   
                 while(path[i] =='/' && i< path.size()) i++; //skip the begining '////'  
                 if(i == path.size())   
                      break;   
                 int start = i;   
                 while(path[i]!='/' && i< path.size()) i++; //decide the end boundary  
                 int end = i-1;   
                 string element = path.substr(start, end-start+1);   
                 if(element == "..")   
                 {   
                      if(stack.size() >0)   
                      stack.pop_back();   
                 }   
                 else if(element!=".")   
                 stack.push_back(element);      
            }   
            if(stack.size() ==0) return "/";   
            string simpPath;   
            for(int i =0; i<stack.size(); i++)   
            simpPath += "/" + stack[i];   
            return simpPath;   
       }   
};
```
**9、编号72 Edit Distance**
Given two words word1 and word2, find the minimum number of steps required to convert word1 to word2. (each operation is counted as 1 step.)
You have the following 3 operations permitted on a word:
a) Insert a character
b) Delete a character
c) Replace a character
应该是属于比较经典的DP题了。
```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int m = word1.length()+1;/*! Use more dimension than the input!*/
        int n = word2.length()+1;/*!*/
        vector<vector<int>> dp;
        for(int i = 0;i < m; i++){
            vector<int> tmp;
            for(int j = 0; j < n; j++) tmp.push_back(0);
            dp.push_back(tmp);
        }
        
        for(int i = 0;i < m; i++) dp[i][0] = i;
        for(int j = 0;j < n; j++) dp[0][j] = j;
        
        for(int i = 1;i < m; i++)
            for(int j = 1; j < n; j++)
                dp[i][j] = min(dp[i-1][j]+1, dp[i][j-1]+1, dp[i-1][j-1] + ((word1[i-1] == word2[j-1]) ? 0 : 1));
        
        return dp[m-1][n-1];
    }
    
    int min(int x, int y, int z){
        int tmp = min(x,y);
        return min(tmp, z);
    }
    
    int min(int x, int y){
        if(x < y) return x;
        else return y;
    }
};
```
**10、编号76 Minimum Window Substring**
Given a string S and a string T, find the minimum window in S which will contain all the characters in T in complexity O(n).
For example,
S = "ADOBECODEBANC"
T = "ABC"
Minimum window is "BANC".
Note: If there is no such window in S that covers all characters in T, return the emtpy string "".
If there are multiple such windows, you are guaranteed that there will always be only one unique minimum window in S.
这题用双指针解。快指针指到包含所有T中字符的时候，慢指针开始走，并保证慢指针走的时候仍然包含T中所有的字符。当慢指针走到不满足时，输出这时候的长度+1。然后快指针又开始走。这样循环到结尾就可以了。
```cpp
class Solution {
public:
    string minWindow(string S, string T) {
        if(T.length() == 0|| S.length() == 0) return "";
        
        vector<int> count1;
        vector<int> count2;
        /*Use count1 as counter, use count2 to recover count1*/
        for(int i  = 0; i < 256; i++){
            count1.push_back(0);
            count2.push_back(0);
        }
        for(int i  = 0; i < T.length(); i++){
            count1[T[i]]++;
            count2[T[i]]++;
        }
        /*If count is zero, find a solution*/
        int count = T.length();
        
        int start = 0;
        int minSize = INT_MAX;
        int minStart;
        
        for(int end = 0; end < S.length(); end++){
            count1[S[end]]--;
            if(count1[S[end]] >= 0) count--;
        
           /*Use every char as start, handle end situation*/
            if(count == 0){
                while(true){
                    if(count2[S[start]] > 0){
                        if(count1[S[start]] < 0) count1[S[start]]++;
                        else break;
                    }
                    start++;
                }
                if(minSize > (end - start + 1)){
                    minSize = end - start + 1;
                    minStart = start;
                }
            }
        }
        
        if(minSize == INT_MAX) return "";
        string result(S, minStart, minSize);
        return result;
    }
};
```
**11、编号91 Decode Ways**
A message containing letters from A-Z is being encoded to numbers using the following mapping:
'A' -> 1
'B' -> 2
...
'Z' -> 26
Given an encoded message containing digits, determine the total number of ways to decode it.
For example, Given encoded message "12", it could be decoded as "AB" (1 2) or "L" (12).
The number of ways decoding "12" is 2.
一开始以为是比较容易的一维dp。当读入一个数字时，要么它自己是一个code，要么它跟前一位数字组成一个code。如果是前者，就取一个值，如果是后者，就把两种情况的值加起来。但是做了以后发现0特别难处理。因为0没有对应的字符，所以只能跟前面一个字符组合，这样就彻底乱了。最后采用了从后往前dp的特殊方法。
```cpp
class Solution {
public:
    int numDecodings(string s) {
        if(s.length() == 0) return 0;
        
        vector<int> dp(s.length()+2, 1);
        
        for(int i = s.length()-1; i >= 0; i-- ){
            if(s[i] == '0') dp[i] = 0;
            else dp[i] = dp[i+1]; 
            
            if(i+1 < s.length() && (s[i] == '1' || (s[i] == '2' && s[i+1] <= '6')))
                dp[i] += dp[i+2];
        }
        return dp[0];
    }
    
    bool CheckValid(char num){
        if(num == '0') return false;
        return true;
    }
    
    bool CheckValid(char num1, char num2){
        if(num1 == '1' || (num1 == '2' && num2 >= '0' && num2 <= '6'))
            return true;
        return false;
    }
};
```
**12、编号98 Interleaving String**
Given s1, s2, s3, find whether s3 is formed by the interleaving of s1 and s2.
For example,
Given:
s1 = "aabcc",
s2 = "dbbca",
When s3 = "aadbbcbcac", return true.
When s3 = "aadbbbaccc", return false.
用二维dp来做。
```cpp
class Solution {
public:
    bool isInterleave(string s1, string s2, string s3) {
        int m = s1.length();
        int n = s2.length();
        if((m+n) != s3.length()) return false;
        
        bool **dp = new bool*[m+1];/*use m+1 and n+1 to denotes empty string*/
        for(int i = 0 ; i < m+1; i++) {
            dp[i] = new bool[n+1];
            for(int j = 0; j < n+1; j++)
                dp[i][j] = false;//!!!!!!!!! Initial!!!!!!!!
        }
        
        dp[0][0] = true;
        for(int i = 1; i < m+1; i++){
            if(s1[i-1] == s3[i-1]) dp[i][0] = dp[i-1][0];
            else dp[i][0] = false;
        }
        for(int j = 1; j < n+1; j++){
            if(s2[j-1] == s3[j-1]) dp[0][j] = dp[0][j-1];
            else dp[0][j] = false;
        }
        
        for(int i = 1; i < m+1; i++){
            for(int j = 1; j < n+1; j++){
                  char x = s1[i-1];//!!!!!
                  char y = s2[j-1];
                  char z = s3[i+j-1];
                  if(x == z)  dp[i][j] = dp[i-1][j] || dp[i][j];
                  if(y == z)  dp[i][j] = dp[i][j-1] || dp[i][j];//z//
            }
        }
    
        bool result = dp[m][n];
        for(int i = 0; i < m; i++) delete dp[i];
        delete dp;
        
        return result;
    }
};
```
**13、编号116 Distinct Subsequences**
Given a string S and a string T, count the number of distinct subsequences of T in S.
A subsequence of a string is a new string which is formed from the original string by deleting some (can be none) of the characters without disturbing the relative positions of the remaining characters. (ie, "ACE" is a subsequence of "ABCDE" while "AEC" is
 not).
Here is an example:
S = "rabbbit", T = "rabbit"
Return 3.
还是二维dp来做。列出例子如下。
/*
    r a b b b i t
  1 1 1 1 1 1 1 1
r 0 1 1 1 1 1 1 1
a 0 0 1 1 1 1 1 1
b 0 0 0 1 2 3 3 3
b 0 0 0 0 1 3 3 3
i 0 0 0 0 0 0 3 3
t 0 0 0 0 0 0 0 3  
*/
```cpp
class Solution {
public:
    /*2D DP read*/
    int numDistinct(string S, string T) {  
        int** dp;
        dp = new int*[T.length() + 1];
        for(int i = 0; i < T.length()+1; i++)
            dp[i] = new int[S.length() + 1];
        
        dp[0][0] = 1;  
        for (int i = 1; i <= T.length(); i++) dp[i][0] = 0;  
        for (int j = 1; j <= S.length(); j++) dp[0][j] = 1;  
        
        for (int i = 1; i <= T.length(); i++) {  
            for (int j = 1; j <= S.length(); j++) {  
                /*core*/
                dp[i][j] = dp[i][j - 1];  
                if (T[i - 1] == S[j - 1])  dp[i][j] += dp[i - 1][j - 1];  
            }  
        }  
        return dp[T.length()][S.length()];  
      
    }  
};
```
**14、编号127 Word Ladder**
Given two words (start and end), and a dictionary, find the length of shortest transformation sequence from start to end, such that:
Only one letter can be changed at a time
Each intermediate word must exist in the dictionary
For example,
Given:
start = "hit"
end = "cog"
dict = ["hot","dot","dog","lot","log"]
As one shortest transformation is "hit" -> "hot" -> "dot" -> "dog" -> "cog",
return its length 5.
Note:
Return 0 if there is no such transformation sequence.
All words have the same length.
All words contain only lowercase alphabetic characters.
这题要求最短的攀爬路径，应该使用BFS(Queue)而不是DFS(Stack, Recursive)
一般解法：使用两个队列(或一个pair类型的队列)，一个存单词，一个存level。
注意三类问题：第一类是求所有无重复元素的combination使用DFS；第二类是求最短距离的使用BFS；第三类主要是在图中判断有无联通路径，这时候看情况用BFS和DFS都可以
```cpp
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        if(wordList.empty()) return 0;
        
        unordered_set<string> uset;
        for(int i = 0; i < wordList.size(); i++) uset.insert(wordList[i]);
        
        queue<pair<string,int>> q;
        q.push(make_pair(beginWord,1));
   
        
        while(!q.empty()){
            pair<string,int> p = q.front();
            q.pop();
            
            string tmp = p.first;
            
            for(int i = 0; i < tmp.length(); i++){
                for(int c = 'a'; c <= 'z'; c++){
                    if(tmp[i] == c) continue;
                    tmp[i] = c;
                    
                    if(uset.find(tmp) != uset.end()){
                        uset.erase(tmp);
                        if(tmp.compare(endWord)==0) return p.second+1; //!
                        q.push(make_pair(tmp, p.second+1));
                    }
                    tmp = p.first;
                }
            }
            
        }
        
        return 0;
    }
};
```
**15、编号128 Word Ladder II**
Given two words (start and end), and a dictionary, find all shortest transformation sequence(s) from start to end, such that:
Only one letter can be changed at a time
Each intermediate word must exist in the dictionary
For example,
Given:
start = "hit"
end = "cog"
dict = ["hot","dot","dog","lot","log"]
Return
  [
    ["hit","hot","dot","dog","cog"],
    ["hit","hot","lot","log","cog"]
  ]
Note:
All words have the same length.
All words contain only lowercase alphabetic characters.
只是额外输出结果而已，这里使用了回溯的DFS方法来重构路径。会做上题的话，这题也是不难的。
```cpp
class Solution {
public:
    /*Read*/
    /*I use the BFS to search for the best ladder length, and record the parents of each word. When a new "previous to end" word is found, I simply get all solutions from this word recursively via DFS.*/
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string> &wordList) {
        vector<vector<string>> res;
        
        unordered_set<string> dict;
        for(int i = 0; i < wordList.size(); i++) dict.insert(wordList[i]);
        queue<pair<string, int>> q;
        q.push({beginWord, 1});
        
        unordered_map<string, vector<string>> path; /*用来装某个word对应的路线的上一个word*/
        path[beginWord].push_back(beginWord);
        
        unordered_map<string, int> umap_level; /*记录每个单词对应的level*/
        umap_level[beginWord] = 1;
        int FirstLength = -1; //当第一次发现一个有效结果的时候存其长度，以后碰到同样level的时候就不用找了，可以退出了
        while (!q.empty()) {
            pair<string,int> p = q.front();
            q.pop();
            if (FirstLength > 0 && p.second >= FirstLength) break;
            
            string tmp = p.first;
            for (int i = 0; i < tmp.size(); ++i)
                for (char c = 'a'; c <= 'z'; ++c) {
                    if (tmp[i] == c) continue;
                    tmp[i] = c;
                    
                    if (dict.find(tmp) != dict.end()){
                        if (tmp == endWord) {//找到一个结尾的单词了
                            FirstLength = p.second + 1;
                            vector<string> res1;
                            res1.push_back(endWord);
                            res1.push_back(p.first);
                            dfs(res, res1, path, p.first); /*Build paths*/
                        } else if (path.find(tmp) == path.end()) {//找到一个中间的单词
                            path[tmp].push_back(p.first);
                            umap_level[tmp] = p.second + 1;
                            q.push({tmp, p.second + 1});
                        } else if (umap_level[tmp] == p.second + 1)  path[tmp].push_back(p.first);
                    }
                    
                    tmp = p.first;
                }
        }
        return res;
    }
    /*Recursion*/
    void dfs(vector<vector<string>> &res, vector<string> &res1, 
             unordered_map<string, vector<string>> &path, 
             //vector<string> &p0,
             string s) {
        vector<string> a_path = path[s];
        
        if (a_path.size() == 1 && a_path[0] == s) {
            res.push_back(vector<string>(res1.rbegin(), res1.rend()));
            return;
        }
        
        for (int i = 0; i < a_path.size(); i++){
            res1.push_back(a_path[i]);
            dfs(res, res1, path, a_path[i]);
            res1.pop_back();
        }
    }
};
```
**16、编号140 Word Break**
Given a string s and a dictionary of words dict, determine if s can be segmented into a space-separated sequence of one or more dictionary words.
For example, given
s = "leetcode",
dict = ["leet", "code"].
Return true because "leetcode" can be segmented as "leet code".
这题有两个方法
方法1:
把字典装进hashset里面，然后拆分string看看分出来的结果在不在hashset里。
比如，把s根据0到i (i = max~min)拆成prefix和suffix，看看prefix在不在hashset里，在的话就继续递归suffix，不在的话就过。
这样可以得到正确结果，但是超时了。需要拿走一些不必要的计算。
新建一个notMatched的hashset，如果suffix没匹配上，就把它放进这里面。以后当要递归suffix之前查一下notMatched就好
```cpp
class Solution {
public:
    /*常规DFS方法*/
    bool recursive(string s, unordered_set<string> &uset, unordered_set<string> ¬Matched, int minLen, int maxLen){
        if(s.length() == 0) return true;
      
        for(int i = minLen; i <= min((int)s.length(), maxLen); i++){ //<= min(...)
            string prefix = s.substr(0, i);
            string suffix = s.substr(i);
            if(uset.find(prefix) != uset.end()){
                if(notMatched.find(suffix) != notMatched.end()) continue; //!
                if(recursive(suffix, uset, notMatched, minLen, maxLen)) return true;   
                notMatched.insert(suffix);  //!
            }
        }
        
        return false;
    }
    
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> uset;
        int maxLen = INT_MIN;
        int minLen = INT_MAX;
        for(int i = 0; i < wordDict.size(); i++){
            uset.insert(wordDict[i]);
            if((int)wordDict[i].length() > maxLen) maxLen = wordDict[i].length();//int
            if((int)wordDict[i].length() < minLen) minLen = wordDict[i].length();
        }
        unordered_set<string> notMatched;
        return recursive(s, uset, notMatched, minLen, maxLen);
    }
};
```
方法2: DP
dp[i]= true, if 存在0<k<i, 使得dp[k]为true并且，s[k+1,i]在dict里面
dp的问题是初始化不好办。所以在s前加一个符号，令dp[0]=true, 然后从1开始比对起。
```cpp
class Solution {
public:
    /*DP*/
    //possible[i] = true      if  S[0,i]在dictionary里面
    //            = true      if  possible[k] == true 并且 S[k+1,j]在dictionary里面， 0<k<i
    //            = false     if  no such k exist.
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> uset;
        for(int i = 0; i < wordDict.size(); i++) uset.insert(wordDict[i]);
        
        vector<bool> dp(s.length()+1, false);
        dp[0] = true;
        s = "#" + s;
        
        for(int i = 1; i < s.length(); i++){
            for(int k = 0; k < i; k++){
                if(dp[k] && uset.find(s.substr(k+1,i-k))!= uset.end()){
                    dp[i] = true;
                    break;
                }
            }
        }
        
        return dp[s.length()-1];
    }
};
```
**17、编号141 Word Break II**
Given a string s and a dictionary of words dict, add spaces in s to construct a sentence where each word is a valid dictionary word.
Return all such possible sentences.
For example, given
s = "catsanddog",
dict = ["cat", "cats", "and", "sand", "dog"].
A solution is ["cats and dog", "cat sand dog"].
这是上一题的进阶，使用一般方法加res和res1处理就好。注意细节。
```cpp
class Solution {
public:
    
    void recursive(string s, unordered_set<string> &uset, unordered_set<string> ¬Matched, int minLen, int maxLen, string res1, vector<string> &res){
        if(s.length() == 0) {
            res.push_back(res1.substr(0, res1.length()-1));
            return;
        }
        
        for(int i = minLen; i <= min((int)s.length(), maxLen); i++){ 
            string prefix = s.substr(0, i);
            string suffix = s.substr(i);
            if(uset.find(prefix) != uset.end()){
                if(notMatched.find(suffix) != notMatched.end()) continue; 
                int size0 = res.size();
                recursive(suffix, uset, notMatched, minLen, maxLen, res1 + prefix + " ", res);
                if(res.size() == size0) notMatched.insert(suffix);  
            }
        }
    }
    
    vector<string> wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> uset;
        int maxLen = INT_MIN;
        int minLen = INT_MAX;
        for(int i = 0; i < wordDict.size(); i++){
            uset.insert(wordDict[i]);
            if((int)wordDict[i].length() > maxLen) maxLen = wordDict[i].length();//int
            if((int)wordDict[i].length() < minLen) minLen = wordDict[i].length();
        }
        unordered_set<string> notMatched;
        
        vector<string> res;
        string res1;
        recursive(s, uset, notMatched, minLen, maxLen, res1, res);
        return res;
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/

