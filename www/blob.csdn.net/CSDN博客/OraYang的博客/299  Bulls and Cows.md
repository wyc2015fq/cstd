
# 299. Bulls and Cows - OraYang的博客 - CSDN博客

2018年01月13日 17:14:05[OraYang](https://me.csdn.net/u010665216)阅读数：179所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
You are playing the following[Bulls and Cows](https://en.wikipedia.org/wiki/Bulls_and_Cows)game with your friend: You write down a number and ask your friend to guess what the number is. Each time your friend makes a guess, you provide a hint that indicates how many digits in said guess match your secret number exactly in both digit and position (called “bulls”) and how many digits match the secret number but locate in the wrong position (called “cows”). Your friend will use successive guesses and hints to eventually derive the secret number.

For example:
Secret number:  "1807"
Friend's guess: "7810"Hint:`1`bull and`3`cows. (The bull is`8`, the cows are`0`,`1`and`7`.)
Write a function to return a hint according to the secret number and friend’s guess, use`A`to indicate the bulls and`B`to indicate the cows. In the above example, your function should return`"1A3B"`.
Please note that both secret number and friend’s guess may contain duplicate digits, for example:
Secret number:  "1123"
Friend's guess: "0111"In this case, the 1st`1`in friend’s guess is a bull, the 2nd or 3rd`1`is a cow, and your function should return`"1A1B"`.
You may assume that the secret number and your friend’s guess only contain digits, and their lengths are always equal.
# 思路
这里有个简单直接易于理解的思路：只需要遍历三遍，就能得到最终的答案。现在来详细介绍：
> 第一步遍历两个字符串，同时利用两个哈希表分别统计两个字符串中，字符出现的次数

> 第二步遍历两个字符串，同时求出有几个字符满足位置及数字相等的，并将满足条件的字符分别从上述两个哈希表中减去1

> 第三步遍历其中一个哈希表，判断另一个哈希表中是否出现对应的字符，如果出现就取个数最少的那个字符对应出现的次数
具体看代码，一看就了解啦
# 代码
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
string
```
```python
getHint(
```
```python
string
```
```python
secret,
```
```python
string
```
```python
guess) {
```
```python
int
```
```python
A=
```
```python
0
```
```python
,B=
```
```python
0
```
```python
;
```
```python
unordered_map
```
```python
<
```
```python
char
```
```python
,
```
```python
int
```
```python
>
```
```python
counta,countb;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<secret.size();i++)
        {
            counta[secret[i]]++;
            countb[guess[i]]++;
        }
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<secret.size();i++)
        {
```
```python
if
```
```python
(secret[i]==guess[i])
            {
                A++;
                counta[secret[i]]--;
                countb[guess[i]]--;
            }
        }
```
```python
unordered_map
```
```python
<
```
```python
char
```
```python
,
```
```python
int
```
```python
>
```
```python
::iterator itr=counta.begin();
```
```python
while
```
```python
(itr!=counta.end())
        {
```
```python
if
```
```python
(countb.find(itr->first)!=countb.end())
                B+=min(itr->second,countb[itr->first]);
            itr++;
        }
```
```python
return
```
```python
to_string(A)+
```
```python
'A'
```
```python
+to_string(B)+
```
```python
'B'
```
```python
;
    }
};
```

