
# 767. Reorganize String - OraYang的博客 - CSDN博客

2018年02月28日 15:08:01[OraYang](https://me.csdn.net/u010665216)阅读数：480所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given a string`S`, check if the letters can be rearranged so that two characters that are adjacent to each other are not the same.
If possible, output any possible result.  If not possible, return the empty string.
**Example 1:**
**Input:**S = "aab"**Output:**"aba"**Example 2:**
**Input:**S = "aaab"**Output:**""**Note:**
Swill consist of lowercase letters and have length in range[1, 500].


# 思路
该题目类型属于排序题，我认为这个归类不是太准确，应该归到字符串处理这一类。题目要求将字符串中的字符重新排列，要求两两之间不能有相同字符，并输出其中一种结果，如果不存在上述情况则输出空字符串。
# 代码
```python
class Solution {
```
```python
public
```
```python
String
```
```python
reorganizeString
```
```python
(String S) {
```
```python
int
```
```python
N = S.length();
```
```python
int
```
```python
[] cnt =
```
```python
new
```
```python
int
```
```python
[
```
```python
256
```
```python
];
```
```python
//count ASCII(256)
```
```python
char
```
```python
mc =
```
```python
'a'
```
```python
;
```
```python
//most frequent char(default 'a')
```
```python
//Frequency statistics
```
```python
for
```
```python
(
```
```python
char
```
```python
c:S.toCharArray())
        {
            cnt[c]++;
            mc = (cnt[c]>cnt[mc])?c:mc;
```
```python
//most frequent char
```
```python
}
```
```python
//return ""
```
```python
if
```
```python
((N-cnt[mc])<(cnt[mc]-
```
```python
1
```
```python
))
```
```python
return
```
```python
""
```
```python
;
```
```python
//return one possible case
```
```python
StringBuilder[] sb =
```
```python
new
```
```python
StringBuilder[cnt[mc]];
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
;i<cnt[mc];i++)
        {
            sb[i] =
```
```python
new
```
```python
StringBuilder();
            sb[i].append(mc);
        }
```
```python
int
```
```python
k =
```
```python
0
```
```python
;
```
```python
//Loop interpolation
```
```python
for
```
```python
(
```
```python
char
```
```python
c =
```
```python
'a'
```
```python
;c<=
```
```python
'z'
```
```python
;c++)
```
```python
// defult in a~z
```
```python
{
```
```python
while
```
```python
(cnt[c]>
```
```python
0
```
```python
&&c!=mc)
            {
                sb[k++].append(c);
                cnt[c]--;
                k%=cnt[mc];
```
```python
//loop
```
```python
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
int
```
```python
i=
```
```python
1
```
```python
;i<cnt[mc];i++)
            sb[
```
```python
0
```
```python
].append(sb[i]);
```
```python
return
```
```python
sb[
```
```python
0
```
```python
].toString();
    }
}
```

