# [LeetCode] Find Duplicate File in System 在系统中寻找重复文件 - Grandyang - 博客园







# [[LeetCode] Find Duplicate File in System 在系统中寻找重复文件](https://www.cnblogs.com/grandyang/p/7007974.html)







Given a list of directory info including directory path, and all the files with contents in this directory, you need to find out all the groups of duplicate files in the file system in terms of their paths.

A group of duplicate files consists of at least two files that have exactly the same content.

A single directory info string in the input list has the following format:

`"root/d1/d2/.../dm f1.txt(f1_content) f2.txt(f2_content) ... fn.txt(fn_content)"`

It means there are n files (`f1.txt`, `f2.txt` ... `fn.txt` with content `f1_content`, `f2_content` ... `fn_content`, respectively) in directory `root/d1/d2/.../dm`. Note that n >= 1 and m >= 0. If m = 0, it means the directory is just the root directory.

The output is a list of group of duplicate file paths. For each group, it contains all the file paths of the files that have the same content. A file path is a string that has the following format:

`"directory_path/file_name.txt"`

Example 1:
Input:
["root/a 1.txt(abcd) 2.txt(efgh)", "root/c 3.txt(abcd)", "root/c/d 4.txt(efgh)", "root 4.txt(efgh)"]
Output:  
[["root/a/2.txt","root/c/d/4.txt","root/4.txt"],["root/a/1.txt","root/c/3.txt"]]



Note:
- No order is required for the final output.
- You may assume the directory name, file name and file content only has letters and digits, and the length of file content is in the range of [1,50].
- The number of files given is in the range of [1,20000].
- You may assume no files or directories share the same name in the same directory.
- You may assume each given directory info represents a unique directory. Directory path and file info are separated by a single blank space.



Follow-up beyond contest:
- Imagine you are given a real file system, how will you search files? DFS or BFS?
- If the file content is very large (GB level), how will you modify your solution?
- If you can only read the file by 1kb each time, how will you modify your solution?
- What is the time complexity of your modified solution? What is the most time-consuming part and memory consuming part of it? How to optimize?
- How to make sure the duplicated files you find are not false positive?



LeetCode的主页又改版了，放了一些五颜六色的按钮上去了，博主个人觉得风格不太搭，还是比较喜欢之前深沉低调的风格，不过也许看久了就习惯了。来看题吧，这道题给了我们一堆字符串数组，每个字符串中包含了文件路径，文件名称和内容，让我们找到重复的文件，这里只要文件内容相同即可，不用管文件名是否相同，而且返回结果中要带上文件的路径。博主个人感觉这实际上应该算是字符串操作的题目，因为思路上并不是很难想，就是要处理字符串，把路径，文件名，和文件内容从一个字符串中拆出来，我们这里建立一个文件内容和文件路径加文件名组成的数组的映射，因为会有多个文件有相同的内容，所以我们要用数组。然后把分离出的路径和文件名拼接到一起，最后我们只要看哪些映射的数组元素个数多于1个的，就说明有重复文件，我们把整个数组加入结果res中，参见代码如下：



```
class Solution {
public:
    vector<vector<string>> findDuplicate(vector<string>& paths) {
        vector<vector<string>> res;
        unordered_map<string, vector<string>> m;
        for (string path : paths) {
            istringstream is(path);
            string pre = "", t = "";
            is >> pre;
            while (is >> t) {
                int idx = t.find_last_of('(');
                string dir = pre + "/" + t.substr(0, idx);
                string content = t.substr(idx + 1, t.size() - idx - 2);
                m[content].push_back(dir);
            }
        }
        for (auto a : m) {
            if (a.second.size() > 1)res.push_back(a.second);
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/91430/c-clean-solution-answers-to-follow-up](https://discuss.leetcode.com/topic/91430/c-clean-solution-answers-to-follow-up)

[https://discuss.leetcode.com/topic/91301/straight-forward-solution-with-a-tiny-bit-of-java8](https://discuss.leetcode.com/topic/91301/straight-forward-solution-with-a-tiny-bit-of-java8)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












