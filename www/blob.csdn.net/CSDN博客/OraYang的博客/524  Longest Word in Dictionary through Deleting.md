
# 524. Longest Word in Dictionary through Deleting - OraYang的博客 - CSDN博客

2017年12月07日 11:48:08[OraYang](https://me.csdn.net/u010665216)阅读数：353所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a string and a string dictionary, find the longest string in the dictionary that can be formed by deleting some characters of the given string. If there are more than one possible results, return the longest word with the smallest lexicographical order. If there is no possible result, return the empty string.
Example 1:
> Input:

> s = “abpcplea”, d = [“ale”,”apple”,”monkey”,”plea”]

> Output:

> “apple”

> Example 2:

> Input:

> s = “abpcplea”, d = [“a”,”b”,”c”]

> Output:

> “a”

> Note:

> All the strings in the input will only contain lower-case letters.

> The size of the dictionary won’t exceed 1,000.

> The length of all the strings in the input won’t exceed 1,000.

> 思路

> 本题给你一个字符串和字符串数组，让你在可以顺序删除字符串中部分字符的情况下找到字符串数组中相等的字符串，并返回字符串最长的那个字符串。。。很拗口，但是看题目就能明白。思路没有什么花里胡哨的地方，就是老老实实，遍历数组，看看数组里面的字符串的字符在给定字符串中是不是顺序存在的。在遍历过程中，保存一个字符串变量用来存储当前时刻的最长有效字符串。

> 代码

> class
> Solution {
> public
> :
> string
> findLongestWord(
> string
> s,
> vector
> <
> string
> >
> & d) {
> string
> max =
> ""
> ;
> for
> (
> auto
> str:d)
        {
> if
> (isin(s,str))
                {
> if
> (max.size()<str.size())
                        max = str;
> else
> if
> (max.size()==str.size())
                        max = max>str?str:max;
                }
        }
> return
> max;
    }
> private
> :
> bool
> isin(
> string
> a,
> string
> b){
> int
> i,j;
> for
> (i=
> 0
> ,j=
> 0
> ;i<b.size()&j<a.size();)
        {
> if
> (b[i]==a[j])
            {
                i++;
                j++;
            }
> else
> j++;
        }
> if
> (i==b.size())
> return
> true
> ;
> else
> return
> false
> ;
    }
};

