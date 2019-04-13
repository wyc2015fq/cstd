
# 609. Find Duplicate File in System - OraYang的博客 - CSDN博客

2017年12月04日 11:38:13[OraYang](https://me.csdn.net/u010665216)阅读数：243所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a list of directory info including directory path, and all the files with contents in this directory, you need to find out all the groups of duplicate files in the file system in terms of their paths.
A group of duplicate files consists of at least two files that have exactly the same content.
A single directory info string in the input list has the following format:
> “root/d1/d2/…/dm f1.txt(f1_content) f2.txt(f2_content) … fn.txt(fn_content)”
It means there are n files (f1.txt, f2.txt … fn.txt with content f1_content, f2_content … fn_content, respectively) in directory root/d1/d2/…/dm. Note that n >= 1 and m >= 0. If m = 0, it means the directory is just the root directory.
The output is a list of group of duplicate file paths. For each group, it contains all the file paths of the files that have the same content. A file path is a string that has the following format:
> “directory_path/file_name.txt”
Example 1:
Input:
> [“root/a 1.txt(abcd) 2.txt(efgh)”, “root/c 3.txt(abcd)”, “root/c/d 4.txt(efgh)”, “root 4.txt(efgh)”]
Output:
> [[“root/a/2.txt”,”root/c/d/4.txt”,”root/4.txt”],[“root/a/1.txt”,”root/c/3.txt”]]
Note:
No order is required for the final output.
You may assume the directory name, file name and file content only has letters and digits, and the length of file content is in the range of [1,50].
The number of files given is in the range of [1,20000].
You may assume no files or directories share the same name in the same directory.
You may assume each given directory info represents a unique directory. Directory path and file info are separated by a single blank space.
Follow-up beyond contest:
> Imagine you are given a real file system, how will you search files? DFS or BFS?

> If the file content is very large (GB level), how will you modify your solution?

> If you can only read the file by 1kb each time, how will you modify your solution?

> What is the time complexity of your modified solution? What is the most time-consuming part and memory consuming part of it? How to optimize?

> How to make sure the duplicated files you find are not false positive?

## 思路
本题考察了哈希表和流处理方法，其中我们整体框架思路是：新建两个容器，一个是map数组，里面存储文件内容与路径数组之间的映射；第二个容器是二维数组，里面存储最终的结果。那么接下来的问题就成了如何从字符串中读取文件目录，文件名及文件内容。这里我们采用了C++两种标准库，一种是stringstream流处理函数，一种是getline读取函数。
## 代码
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
vector
```
```python
<
```
```python
vector
```
```python
<
```
```python
string
```
```python
>
```
```python
>
```
```python
findDuplicate(
```
```python
vector
```
```python
<
```
```python
string
```
```python
>
```
```python
& paths) {
```
```python
unordered_map
```
```python
<
```
```python
string
```
```python
,
```
```python
vector
```
```python
<
```
```python
string
```
```python
>
```
```python
>
```
```python
hash_fn;
```
```python
vector
```
```python
<
```
```python
vector
```
```python
<
```
```python
string
```
```python
>
```
```python
>
```
```python
res;
```
```python
for
```
```python
(
```
```python
auto
```
```python
path:paths)
        {
```
```python
stringstream
```
```python
ss(path);
```
```python
string
```
```python
root;
```
```python
string
```
```python
s;
            getline(ss,root,
```
```python
' '
```
```python
);
```
```python
while
```
```python
(getline(ss,s,
```
```python
' '
```
```python
)){
```
```python
string
```
```python
filepath = root+
```
```python
'/'
```
```python
+ s.substr(
```
```python
0
```
```python
,s.find(
```
```python
'('
```
```python
));
```
```python
string
```
```python
filecontent = s.substr(s.find(
```
```python
'('
```
```python
)+
```
```python
1
```
```python
,s.find(
```
```python
')'
```
```python
)-s.find(
```
```python
'('
```
```python
)-
```
```python
1
```
```python
);
                hash_fn[filecontent].push_back(filepath);
            }
        }
```
```python
for
```
```python
(
```
```python
auto
```
```python
item:hash_fn)
        {
```
```python
if
```
```python
(item.second.size()>
```
```python
1
```
```python
)
                res.push_back(item.second);
        }
```
```python
return
```
```python
res;
    }
};
```

