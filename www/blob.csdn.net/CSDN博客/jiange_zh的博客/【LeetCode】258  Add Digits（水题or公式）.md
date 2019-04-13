
# 【LeetCode】258. Add Digits（水题or公式） - jiange_zh的博客 - CSDN博客


2016年01月06日 15:36:08[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：540标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[算法与数据结构																](https://blog.csdn.net/jiange_zh/article/category/5787591)



## 原题
Given a non-negative integer num, repeatedly add all its digits until the result has only one digit.
For example:
Given num = 38, the process is like: 3 + 8 = 11, 1 + 1 = 2. Since 2 has only one digit, return it.
## 解答
非递归做法：
```python
int
```
```python
addDigits(
```
```python
int
```
```python
num) {
```
```python
int
```
```python
ans = num;
```
```python
while
```
```python
(ans/
```
```python
10
```
```python
)
    {
        ans =
```
```python
0
```
```python
;
```
```python
while
```
```python
(num)
        {
            ans += num
```
```python
%10
```
```python
;
            num /=
```
```python
10
```
```python
;
        }
        num = ans;
    }
```
```python
return
```
```python
ans;
}
```
递归：
```python
int
```
```python
addDigits(
```
```python
int
```
```python
num) {
```
```python
if
```
```python
(num/
```
```python
10
```
```python
==
```
```python
0
```
```python
)
```
```python
return
```
```python
num;
```
```python
int
```
```python
ans =
```
```python
0
```
```python
;
```
```python
while
```
```python
(num)
    {
        ans += num
```
```python
%10
```
```python
;
        num /=
```
```python
10
```
```python
;
    }
```
```python
return
```
```python
addDigits(ans);
}
```
嗯，然后以为就完了？
不！
Follow up:
Could you do it without any loop/recursion in O(1) runtime?
没能想出来……看了答案，原来是有公式的：

![这里写图片描述](https://img-blog.csdn.net/20160106150924482)[ ](https://img-blog.csdn.net/20160106150924482)
[
](https://img-blog.csdn.net/20160106150924482)详见 Wiki：[https://en.wikipedia.org/wiki/Digital_root](https://en.wikipedia.org/wiki/Digital_root)
[
](https://img-blog.csdn.net/20160106150924482)比较佩服的还是网友们各种简洁的代码：
[
](https://img-blog.csdn.net/20160106150924482)
```python
//公式解，一行代码
```
```python
int
```
```python
addDigits(
```
```python
int
```
```python
num) {
```
```python
return
```
```python
(num && !(num
```
```python
%=
```
```python
9
```
```python
)) ?
```
```python
9
```
```python
: num;
}
```
[

](https://img-blog.csdn.net/20160106150924482)
```python
//公式解，一行代码
```
```python
int
```
```python
addDigits(
```
```python
int
```
```python
num) {
```
```python
return
```
```python
1
```
```python
+ (num -
```
```python
1
```
```python
) %
```
```python
9
```
```python
;
}
```
[
](https://img-blog.csdn.net/20160106150924482)
```python
//非递归解，两行
```
```python
int
```
```python
addDigits(
```
```python
int
```
```python
num) {
```
```python
while
```
```python
(num /
```
```python
10
```
```python
) num = num /
```
```python
10
```
```python
+ num %
```
```python
10
```
```python
;
```
```python
return
```
```python
num;
}
```
[
](https://img-blog.csdn.net/20160106150924482)虽然一味追求代码简短并不一定好（可能丢失可读性），但是还是可以在保证代码可读性的情况下争取代码简洁的！
[
](https://img-blog.csdn.net/20160106150924482)实际上，上面这段两行的代码，他的思路跟我是有点不同的。
[
](https://img-blog.csdn.net/20160106150924482)我的思路是很规矩的来加，比如：
[
](https://img-blog.csdn.net/20160106150924482)385 -> 3+8+5 = 16 -> 1+6 = 7
[
](https://img-blog.csdn.net/20160106150924482)而他的思路是：
[
](https://img-blog.csdn.net/20160106150924482)385 -> 38+5 = 43 -> 4+3 = 7
[
](https://img-blog.csdn.net/20160106150924482)为何这样做是对的呢？
[
](https://img-blog.csdn.net/20160106150924482)主要看 38+5 与 3+8+5 的不同，38+5 将 8+5 的进位1累加到高位的3上面， 而 3+8+5将 8+5 的进位保留在13中。
[
](https://img-blog.csdn.net/20160106150924482)而实际上两者是没有区别的：
[
](https://img-blog.csdn.net/20160106150924482)38 + 5 -> (3+1)+3
[
](https://img-blog.csdn.net/20160106150924482)3+8+5 -> 3+(10+3) -> 3+1+3
[            ](https://img-blog.csdn.net/20160106150924482)

