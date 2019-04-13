
# 76. Minimum Window Substring - OraYang的博客 - CSDN博客

2017年12月19日 22:41:52[OraYang](https://me.csdn.net/u010665216)阅读数：449所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 76. Minimum Window Substring
标签（空格分隔）： leetcode hard
---题目

---Given a string S and a string T, find the minimum window in S which will contain all the characters in T in complexity O(n).

---For example,

---S

---=

---"ADOBECODEBANC"

---T

---=

---"ABC"


---Minimum window is

---"BANC"

---.

---Note:

---If there is no such window in S that covers all characters in T, return the empty string

---""

---.

---If there are multiple such windows, you are guaranteed that there will always be only one unique minimum window in S.


---思路

---本题是典型的子字符串求解问题。对于大多数子字符串问题，我们得到一个字符串，需要找到满足某些限制条件的子字符串。一般的方法是使用一个包含两个指针的hashmap。模板如下所示。

---int

---findSubstring(

---string

---s){

---vector

---<

---int

--->

---map

---(

---128

---,

---0

---);

---int

---counter;

---// check whether the substring is valid

---int

---begin=

---0

---, end=

---0

---;

---//two pointers, one point to tail and one  head

---int

---d;

---//the length of substring

---for

---() {

---/* initialize the hash map here */

---}

---while

---(end<s.size()){

---if

---(

---map

---[s[end++]]-- ?){

---/* modify counter here */

---}

---while

---(

---/* counter condition */

---){

---/* update d here if finding minimum*/

---//increase begin to make it invalid/valid again

---if

---(

---map

---[s[begin++]]++ ?){

---/*modify counter here*/

---}
            }

---/* update d here if finding maximum*/

---}

---return

---d;
  }

---本题的解决方案就呼之欲出了

---Note

---：需要提到的一点是，当要求找到最大子字符串时，我们应该在内部while循环之后更新最大值，以保证子字符串是有效的。另一方面，当要求找到最小子字符串时，我们应该在内部while循环中更新最小值。

---代码

---class

---Solution {

---public

---:

---string

---minWindow(

---string

---s,

---string

---t) {

---//定义变量count,头尾指针begin及end

---int

---count=t.size(),begin=

---0

---,end=

---0

---,head=

---0

---,d=INT_MAX;

---//利用ASCII码来代表字符并进行存储

---vector

---<

---int

--->

---map

---(

---128

---,

---0

---);

---for

---(

---auto

---item:t)

---map

---[item]++;

---//移动头尾指针开始寻找最小子串

---while

---(end<s.size())
        {

---if

---(

---map

---[s[end++]]-->

---0

---)
                count--;

---while

---(count==

---0

---)
            {

---//比较是否为最短子串

---if

---((end-begin)<d) d =end-(head=begin);

---if

---(

---map

---[s[begin++]]++==

---0

---)
                    count++;
            }
        }

---return

---d==INT_MAX?

---""

---:s.substr(head,d);        
    }
};


