# [CareerCup] 9.6 Generate Parentheses 生成括号 - Grandyang - 博客园







# [[CareerCup] 9.6 Generate Parentheses 生成括号](https://www.cnblogs.com/grandyang/p/4828614.html)







9.6 Implement an algorithm to print all valid (e.g., properly opened and closed) combinations of n-pairs of parentheses.
EXAMPLE
Input: 3
Output: ((())), (()()), (())(), ()(()), ()()()



LeetCode上的原题，请参见我之前的博客[Generate Parentheses 生成括号](http://www.cnblogs.com/grandyang/p/4444160.html)。



解法一：

```
class Solution {
public:
    vector<string> generateParens(int n) {
        set<string> t;
        if (n == 0) t.insert("");
        else {
            vector<string> pre = generateParens(n - 1);
            for (auto a : pre) {
                for (int i = 0; i < a.size(); ++i) {
                    if (a[i] == '(') {
                        a.insert(a.begin() + i + 1, '(');
                        a.insert(a.begin() + i + 2, ')');
                        t.insert(a);
                        a.erase(a.begin() + i + 1, a.begin() + i + 3);
                    }
                }
                t.insert("()" + a);
            }
        }
        return vector<string>(t.begin(), t.end());
    }
};
```



解法二:

```
class Solution {
public:
    vector<string> generateParens(int n) {
        vector<string> res;
        generateParensDFS(n, n, "", res);
        return res;
    }
    void generateParensDFS(int left, int right, string out, vector<string> &res) {
        if (left > right) return;
        if (left == 0 && right == 0) res.push_back(out);
        else {
            if (left > 0) generateParensDFS(left - 1, right, out + '(', res);
            if (right > 0) generateParensDFS(left, right - 1, out + ')', res);
        }
    }
};
```














