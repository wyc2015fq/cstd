# getline(ss, temp, '/')示例 - 李鑫o_O - CSDN博客

2016年04月02日 16:17:47[hustlx](https://me.csdn.net/HUSTLX)阅读数：958



Given an absolute path for a file (Unix-style), simplify it.

For example,
path = `"/home/"`, => `"/home"`
path = `"/a/./b/../../c/"`, => `"/c"`

[click to show corner cases.](https://leetcode.com/problems/simplify-path/#)

Corner Cases:

- Did you consider the case where path = `"/../"`?

In this case, you should return `"/"`.
- Another corner case is the path might contain multiple slashes `'/'` together,
 such as `"/home//foo/"`.

In this case, you should ignore redundant slashes and return `"/home/foo"`.


```cpp
string simplifyPath(string path) {
    string res, temp;
    vector<string> stk;
    stringstream ss(path);
    while (getline(ss, temp, '/')) {
        if (temp == "." || temp == "") continue;
        else if (temp == ".."&&!stk.empty()) stk.pop_back();
        else if(temp!="..") stk.push_back(temp);
    }
    for (auto str : stk) res += "/" + str;
    return res.empty() ? "/" : res;
}
```


