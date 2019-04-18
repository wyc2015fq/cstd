# Leetcode全匹配问题 - 無名黑洞 - CSDN博客
2014年02月07日 00:22:38[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：2004
**目录**
**1、编号11 Regular Expression Matching2、编号29 Substring with Concatenation of All Words3、编号43 Wildcard Matching4、编号79 Word Search**
**1、编号11 Regular Expression Matching**
Implement regular expression matching with support for '.' and '*'.
'.' Matches any single character.
'*' Matches zero or more of the preceding element.
The matching should cover the entire input string (not partial).
The function prototype should be:
bool isMatch(const char *s, const char *p)
Some examples:
isMatch("aa","a") → false
isMatch("aa","aa") → true
isMatch("aaa","aa") → false
isMatch("aa", "a*") → true
isMatch("aa", ".*") → true
isMatch("ab", ".*") → true
isMatch("aab", "c*a*b") → true
这类题，看清题意比较重要。给字符串s和p，问s和p在什么情况下能匹配上（p包含s可以，s包含p不行）。.表示任何单字符，*表示任意数量的后面的字符。使用递归来做。首先处理不带*的特殊情况，然后处理*。
```cpp
class Solution {
public:
    bool isMatch(const char *s, const char *p) {
         /*if p(search) is empty, then if s(text) is also empty, return true, otherwise return false*/
        if (*p == '\0') return *s == '\0';     
    
        /*Begin processing, p is not NULL, s can be NULL*/
        if (*(p+1) != '*') {                                                       /*if no star is involved*/
            if (*p == *s || (*p == '.' && (*s) != '\0')) return isMatch(s+1, p+1); /*YES matched, go next, and sync next*/
            return false;                                                          /*NOT matched, return FALSE*/
        }
        else {                                                /*p has star involved*/
            while (*p == *s || (*p == '.' && (*s) != '\0')) {
                if (isMatch(s, p+2)) return true;             /*YES matched, go next. if next true, then ture, else nothing*/
                s++;                                          /*if star not matched, not return false, go next text instead*/
            }
            return isMatch(s, p+2);                           /*NOT matched, skip this star*/
        }
    }
};
```
**2、编号29 Substring with Concatenation of All Words**
You are given a string, S, and a list of words, L, that are all of the same length. Find all starting indices of substring(s) in S that is a concatenation of each word in L exactly once and without any intervening characters.
For example, given:
S: "barfoothefoobarman"
L: ["foo", "bar"]
You should return the indices: [0,9].
(order does not matter).
比较难的题目。使用双map法。第一个map words存储字典L里所有的单词。另一个map用来记录S中找到的匹配上的单词的情况。假如S中某个单词不在words中，或出现的频率大于Words中的频率，则不匹配。
```cpp
class Solution {  
public:  
    vector<int> findSubstring(string S, vector<string> &L) {  
        /*Initial Map*/
        map<string, int> words;  
        map<string, int> currentStr;  
        for(int i = 0; i < L.size(); i++)  words[L[i]]++; /*store all words in L and the concurrency*/
        
        /*Initial Result*/
        int N = L.size(); /*Words number*/ 
        int M = L[0].size();  /*All words in L are the same length*/
        vector<int> result;  
        if(N <= 0)   return result;  
        
        /*Find all substrings in S (not in L, thats brilliant!) and match*/
        /*map::find is also a good use here*/
        for(int i = 0; i <= (int)S.size() - N * M; i++){  
            currentStr.clear();  
            int j = 0;  
            for(j = 0; j < N; j++)  {  
                string sub = S.substr(i + j * M, M);  /*Use substr to find string between i+j*M and i+j*M+M*/
                if(words.find(sub) == words.end()) break;  /*Not find, cus sub is not in words*/
                currentStr[sub]++;  
                if(currentStr[sub] > words[sub]) break;  /*Fail, cus sub in current string is more then words*/
            }  
            if(j == N)  result.push_back(i);  /*All j go through, so find*/
        }
        
        return result;  
    }  
};
```
**3、编号43 Wildcard Matching**
Implement wildcard pattern matching with support for '?' and '*'.
'?' Matches any single character.
'*' Matches any sequence of characters (including the empty sequence).
The matching should cover the entire input string (not partial).
The function prototype should be:
bool isMatch(const char *s, const char *p)
Some examples:
isMatch("aa","a") → false
isMatch("aa","aa") → true
isMatch("aaa","aa") → false
isMatch("aa", "*") → true
isMatch("aa", "a*") → true
isMatch("ab", "?*") → true
isMatch("aab", "c*a*b") → false
编号11的变种。这里的*号可以表示任何字符串。
```cpp
class Solution {
public:
    bool isMatch(const char *s, const char *p) {  
        const char* starAddress=NULL;  
        const char* ss = s;   
        while (*s){  
            if ((*p=='?')||(*p==*s)){s++;p++;}  
            else if (*p=='*'){starAddress=p; p++; ss=s;}  
            else if (starAddress){ p = starAddress+1; ss++; s=ss;}  
            else return false;  
        }  
        while (*p=='*'){p++;}  
        return !*p;  
    }  
};
```
**4、编号79 Word Search**
Given a 2D board and a word, find if the word exists in the grid.
The word can be constructed from letters of sequentially adjacent cell, where "adjacent" cells are those horizontally or vertically neighboring. The same letter cell may not be used more than once.
For example, Given board =
[
  ["ABCE"],
  ["SFCS"],
  ["ADEE"]
]
word = "ABCCED", -> returns true,
word = "SEE", -> returns true,
word = "ABCB", -> returns false.
也是一道比较复杂的题目，但不难。用DFS。首先建立一个visited数组，每次察看一个字母后把对应的visited记录，以免重复。DFS进栈要考虑上下左右四个方向和边界。不知道为什么，原来用std::vector作的答案不能通过大数据，所以在网上抄了一个c风格数组的。
```cpp
class Solution {
public:
    bool exist(vector<vector<char> > &board, string word) {  
      if(word.size() ==0) return false;  
      if(board.size() ==0 || board[0].size() == 0) return false;  
      int row = board.size();  
      int col = board[0].size();  
      int * visited = new int[row*col];  
      memset(visited, 0, row*col*sizeof(int));  
      for(int i =0; i< board.size(); i++){  
        for(int j =0; j< board[0].size(); j++){  
          if(board[i][j] == word[0]){  
            visited[i*col+j] = 1;  
            if(search(board, word, visited, -1, 1, i, j))  
              return true;  
            visited[i*col+j] =0;  
          }  
        }  
      }  
      delete visited;  
      return false;  
    }    
    bool search(vector<vector<char> > &board,   
      string& word,  
      int* visited,  
      int op, //0 up, 1 down, 2 left, 3 right  
      int matchLen,  
      int i,  
      int j){  
      if(matchLen == word.size()) return true;  
      int row = board.size();  
      int col = board[0].size();  
      if(i+1<row && op!=0){  
        if(visited[(i+1)*col+j] ==0 &&   
        board[i+1][j] == word[matchLen]){  
          visited[(i+1)*col+j] =1;  
          if(search(board, word, visited, 1, matchLen+1, i+1, j))  
            return true;  
          visited[(i+1)*col+j] =0;  
        }  
      }  
      if(i-1>=0 && op!=1){  
        if(visited[(i-1)*col+j] ==0 && board[i-1][j] == word[matchLen]){  
          visited[(i-1)*col+j] =1;  
          if(search(board, word, visited, 0, matchLen+1, i-1, j))  
            return true;  
          visited[(i-1)*col+j] =0;  
        }  
      }  
      if(j+1<col && op!=2){  
        if(visited[i*col+j+1] ==0 && board[i][j+1] == word[matchLen]){  
          visited[i*col+j+1] =1;  
          if(search(board, word, visited, 3, matchLen+1, i, j+1))  
            return true;  
          visited[i*col+j+1] =0;  
        }  
      }  
      if(j-1>=0 && op!=3){  
        if(visited[i*col+j-1] ==0 && board[i][j-1] == word[matchLen]){  
          visited[i*col+j-1] =1;  
          if(search(board, word, visited, 2, matchLen+1, i, j-1))  
            return true;  
          visited[i*col+j-1] =0;  
        }  
      }  
      return false;      
    } 
    
};
```
**参考文献：**
http://oj.leetcode.com/problems/
