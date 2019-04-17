# [LeetCode] Simplify Path 简化路径 - Grandyang - 博客园







# [[LeetCode] Simplify Path 简化路径](https://www.cnblogs.com/grandyang/p/4347125.html)







Given an absolute path for a file (Unix-style), simplify it.

For example,
path = `"/home/"`, => `"/home"`
path = `"/a/./b/../../c/"`, => `"/c"`

[click to show corner cases.](https://leetcode.com/problems/simplify-path/)

Corner Cases:


- Did you consider the case where path = `"/../"`?
In this case, you should return `"/"`.
- Another corner case is the path might contain multiple slashes `'/'` together, such as `"/home//foo/"`.
In this case, you should ignore redundant slashes and return `"/home/foo"`.




这道题让简化给定的路径，光根据题目中给的那一个例子还真不太好总结出规律，应该再加上两个例子 path = `"/a/./b/../c/"`, => `"/a/c"和path = `"/a/./b/c/"`, => `"/a/b/c"，` 这样我们就可以知道中间是"."的情况直接去掉，是".."时删掉它上面挨着的一个路径，而下面的边界条件给的一些情况中可以得知，如果是空的话返回"/"，如果有多个"/"只保留一个。那么我们可以把路径看做是由一个或多个"/"分割开的众多子字符串，把它们分别提取出来一一处理即可，代码如下：`



C++ 解法一:

```
class Solution {
public:
    string simplifyPath(string path) {
        vector<string> v;
        int i = 0;
        while (i < path.size()) {
            while (path[i] == '/' && i < path.size()) ++i;
            if (i == path.size()) break;
            int start = i;
            while (path[i] != '/' && i < path.size()) ++i;
            int end = i - 1;
            string s = path.substr(start, end - start + 1);
            if (s == "..") {
                if (!v.empty()) v.pop_back(); 
            } else if (s != ".") {
                v.push_back(s);
            }
        }
        if (v.empty()) return "/";
        string res;
        for (int i = 0; i < v.size(); ++i) {
            res += '/' + v[i];
        }
        return res;
    }
};
```



还有一种解法是利用了C语言中的函数strtok来分隔字符串，但是需要把string和char*类型相互转换，转换方法请猛戳[这里](http://www.cnblogs.com/grandyang/p/4312273.html)。除了这块不同，其余的思想和上面那种解法相同，代码如下：



C 解法一:

```
class Solution {
public:
    string simplifyPath(string path) {
        vector<string> v;
        char *cstr = new char[path.length() + 1];
        strcpy(cstr, path.c_str());
        char *pch = strtok(cstr, "/");
        while (pch != NULL) {
            string p = string(pch);
            if (p == "..") {
                if (!v.empty()) v.pop_back();
            } else if (p != ".") {
                v.push_back(p);
            }
            pch = strtok(NULL, "/");
        }
        if (v.empty()) return "/";
        string res;
        for (int i = 0; i < v.size(); ++i) {
            res += '/' + v[i];
        }
        return res;
    }
};
```



C++中也有专门处理字符串的机制，我们可以使用stringstream来分隔字符串，然后对每一段分别处理，思路和上面的方法相似，参见代码如下：



C++ 解法二：

```
class Solution {
public:
    string simplifyPath(string path) {
        string res, t;
        stringstream ss(path);
        vector<string> v;
        while (getline(ss, t, '/')) {
            if (t == "" || t == ".") continue;
            if (t == ".." && !v.empty()) v.pop_back();
            else if (t != "..") v.push_back(t);
        }
        for (string s : v) res += "/" + s;
        return res.empty() ? "/" : res;
    }
};
```



Java 解法二：

```
public class Solution {
    public String simplifyPath(String path) {
        Stack<String> s = new Stack<>();
        String[] p = path.split("/");
        for (String t : p) {
            if (!s.isEmpty() && t.equals("..")) {
                s.pop();
            } else if (!t.equals(".") && !t.equals("") && !t.equals("..")) {
                s.push(t);
            }
        }
        List<String> list = new ArrayList(s);
        return "/" + String.join("/", list);
    }
}
```



参考资料：

[https://discuss.leetcode.com/topic/8678/c-10-lines-solution](https://discuss.leetcode.com/topic/8678/c-10-lines-solution)

[https://discuss.leetcode.com/topic/7675/java-10-lines-solution-with-stack](https://discuss.leetcode.com/topic/7675/java-10-lines-solution-with-stack)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












