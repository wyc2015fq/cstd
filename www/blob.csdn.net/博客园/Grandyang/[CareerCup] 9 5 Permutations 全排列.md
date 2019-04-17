# [CareerCup] 9.5 Permutations 全排列 - Grandyang - 博客园







# [[CareerCup] 9.5 Permutations 全排列](https://www.cnblogs.com/grandyang/p/4822859.html)







9.5 Write a method to compute all permutations of a string.



LeetCode上的原题，请参加我之前的博客[Permutations 全排列](http://www.cnblogs.com/grandyang/p/4358848.html)和[Permutations II 全排列之二](http://www.cnblogs.com/grandyang/p/4359825.html)。



解法一：

```
class Solution {
public:
    vector<string> getPerms(string &s) {
        vector<string> res;
        getPermsDFS(s, 0, res);
        return res;
    }
    void getPermsDFS(string &s, int level, vector<string> &res) {
        if (level == s.size()) res.push_back(s);
        else {
            for (int i = level; i < s.size(); ++i) {
                swap(s[level], s[i]);
                getPermsDFS(s, level + 1 ,res);
                swap(s[level], s[i]);
            }
        }
    }
};
```



解法二：

```
class Solution {
public:
    vector<string> getPerms(string s) {
        vector<string> res;
        vector<bool> visited(s.size(), false);
        getPermsDFS(s, 0, visited, "", res);
        return res;
    }
    void getPermsDFS(string s, int level, vector<bool> &visited, string out, vector<string> &res) {
        if (level == s.size()) res.push_back(out);
        else {
            for (int i = 0; i < s.size(); ++i) {
                if (!visited[i]) {
                    visited[i] = true;
                    out.push_back(s[i]);
                    getPermsDFS(s, level + 1, visited, out , res);
                    out.pop_back();
                    visited[i] = false;
                }   
            }
        }
    }
};
```



解法三：

```
class Solution {
public:
    vector<string> getPerms(string s) {
        if (s.empty()) return vector<string>(1, "");
        vector<string> res;
        char first = s[0];
        string remainder = s.substr(1);
        vector<string> words = getPerms(remainder);
        for (auto &a : words) {
            for (int i = 0; i <= a.size(); ++i) {
                string out = insertCharAt(a, first, i);
                res.push_back(out);
            }
        }
        return res;
    }   
    string insertCharAt(string s, char c, int i) {
        string start = s.substr(0, i);
        string end = s.substr(i);
        return start + c + end;
    }
};
```














