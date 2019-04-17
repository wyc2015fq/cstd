# [CareerCup] 9.4 Subsets 子集合 - Grandyang - 博客园







# [[CareerCup] 9.4 Subsets 子集合](https://www.cnblogs.com/grandyang/p/4820842.html)







9.4 Write a method to return all subsets of a set.



LeetCode上的原题，请参见我之前的博客[Subsets 子集合](http://www.cnblogs.com/grandyang/p/4309345.html)和[Subsets II 子集合之二](http://www.cnblogs.com/grandyang/p/4310964.html)。



解法一：

```
class Solution {
public:
    vector<vector<int> > getSubsets(vector<int> &S) {
        vector<vector<int> > res(1);
        for (int i = 0; i < S.size(); ++i) {
            int size = res.size();
            for (int j = 0; j < size; ++j) {
                res.push_back(res[j]);
                res.back().push_back(S[i]);
            }
        }
        return res;
    }
};
```



解法二：

```
class Solution {
public:
    vector<vector<int> > getSubsets(vector<int> &S) {
        vector<vector<int> > res;
        vector<int> out;
        getSubsetsDFS(S, 0, out, res);
        return res;
    }
    void getSubsetsDFS(vector<int> &S, int pos, vector<int> &out, vector<vector<int> > &res) {
        res.push_back(out);
        for (int i = pos; i < S.size(); ++i) {
            out.push_back(S[i]);
            getSubsetsDFS(S, i + 1, out ,res);
            out.pop_back();
        }
    }
};
```



解法三：

```
class Solution {
public:
    vector<vector<int> > getSubsets(vector<int> &S) {
        vector<vector<int> > res;
        int max = 1 << S.size();
        for (int i = 0; i < max; ++i) {
            vector<int> out = convertIntToSet(S, i);
            res.push_back(out);
        }
        return res;
    }
    vector<int> convertIntToSet(vector<int> &S, int k) {
        vector<int> sub;
        int idx = 0;
        for (int i = k; i > 0; i >>= 1) {
            if ((i & 1) == 1) {
                sub.push_back(S[idx]);
            }
            ++idx;
        }
        return sub;
    }
};
```














