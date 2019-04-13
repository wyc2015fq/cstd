
# 17. Letter Combinations of a Phone Number - OraYang的博客 - CSDN博客

2017年09月11日 10:40:58[OraYang](https://me.csdn.net/u010665216)阅读数：127个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a digit string, return all possible letter combinations that the number could represent.
A mapping of digit to letters (just like on the telephone buttons) is given below.
![telephone buttons](https://img-blog.csdn.net/20170911103615890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170911103615890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Input:Digit string “23”
Output: [“ad”, “ae”, “af”, “bd”, “be”, “bf”, “cd”, “ce”, “cf”].
Note:
Although the above answer is in lexicographical order, your answer could be in any order you want.
## 思路
本题，感觉没啥trick，所以就老老实实用嵌套循环来解体。
> step1. 首先建立一个map容器，将数字与字符串做一个映射

> step2. 维护一个string变量res,初始化为”“;

> step3. 求res+map[digits[i]]相加结果，并赋值给res

> step4. 返回

> 代码

> class
> Solution {
> public
> :
> vector
> <
> string
> >
> letterCombinations(
> string
> digits) {
> map
> <
> char
> ,
> string
> >
> mp ;
        mp[
> '2'
> ] =
> "abc"
> ;
        mp[
> '3'
> ] =
> "def"
> ;
        mp[
> '4'
> ] =
> "ghi"
> ;
        mp[
> '5'
> ] =
> "jkl"
> ;
        mp[
> '6'
> ] =
> "mno"
> ;
        mp[
> '7'
> ] =
> "pqrs"
> ;
        mp[
> '8'
> ] =
> "tuv"
> ;
        mp[
> '9'
> ] =
> "wxyz"
> ;
> vector
> <
> string
> >
> res ;
> if
> (digits.size()==
> 0
> )
> return
> res;
        res.push_back(
> ""
> );
> for
> (
> int
> i=
> 0
> ;i<digits.size();i++)
        {
> vector
> <
> string
> >
> tmp;
> for
> (
> int
> j=
> 0
> ;j<mp[digits[i]].size();j++)
> for
> (
> int
> k=
> 0
> ;k<res.size();k++)
                {
                    tmp.push_back(res[k]+mp[digits[i]][j]);
                }
            res.swap(tmp);
        }
> return
> res;
    }
};

