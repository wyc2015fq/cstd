
# 【LeetCode】169. Majority Element (多数投票算法 & 算法迁移能力) - jiange_zh的博客 - CSDN博客


2016年01月09日 10:41:55[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：1577



## Question1
Given an array of size n, find the majority element. The majority element is the element that appears more than ⌊ n/2 ⌋ times.
You may assume that the array is non-empty and the majority element always exist in the array.
## Solution
思路1： 排序之后中间的数即为答案，O(nlogn)
思路2： 扫一遍数组，用map存放出现次数，找出次数大于size/2的数，O(n)
```python
int
```
```python
majorityElement(
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
& nums) {
```
```python
map
```
```python
<
```
```python
int
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
cnt;
```
```python
int
```
```python
n = nums.size()/
```
```python
2
```
```python
;
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
num : nums)
            cnt[num]++;
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
num : cnt)
```
```python
if
```
```python
(num.second > n)
```
```python
return
```
```python
num.first;
    }
```
思路3：看了discuss才发现一种神奇的算法->Boyer–Moore majority vote algorithm，也叫多数投票算法，时间O(n)，空间O(1)。
Wiki：[https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_majority_vote_algorithm](https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_majority_vote_algorithm)
Majority Vote Algorithm：[http://www.cs.utexas.edu/users/moore/best-ideas/mjrty/index.html](http://www.cs.utexas.edu/users/moore/best-ideas/mjrty/index.html)
算法的步骤：
如果count==0，则将vote的值设置为数组的当前元素，将count赋值为1；
否则，如果vote和现在数组元素值相同，则count++，反之count–；
重复上述两步，直到扫描完数组。
count赋值为0，再次从头扫描数组，如果数组元素值与vote的值相同则count++，直到扫描完数组为止。
如果此时count的值大于等于n/2，则返回vote的值，反之则返回-1；
以下是代码实现，由于题目保证结果一定存在，所以我们省去了最后一步的检查验证。
```python
int
```
```python
majorityElement(
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
& nums) {
```
```python
int
```
```python
cnt =
```
```python
0
```
```python
;
```
```python
int
```
```python
vote;
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
num:nums) {
```
```python
if
```
```python
(!cnt) vote = num;
```
```python
if
```
```python
(vote == num) cnt++;
```
```python
else
```
```python
cnt--;
        }
```
```python
return
```
```python
vote;
    }
```
## Question2
229.Majority Element II
Given an integer array of size n, find all elements that appear more than ⌊ n/3 ⌋ times. The algorithm should run in linear time and in O(1) space.
题目指明了空间O(1)，明显是让我们用多数投票算法来做。
仔细分析下问题，可以知道最多只有2个答案。
我们需要做的是找出票数最高的前两名，然后验证下。
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
majorityElement(
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
& nums) {
```
```python
int
```
```python
cnt1 =
```
```python
0
```
```python
;
```
```python
int
```
```python
cnt2 =
```
```python
0
```
```python
;
```
```python
int
```
```python
vote1, vote2;
```
```python
bool
```
```python
two =
```
```python
true
```
```python
;
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
num : nums) {
```
```python
if
```
```python
(!cnt1) {
                vote1 = num;
                cnt1++;
            }
```
```python
else
```
```python
if
```
```python
(cnt1 && num == vote1) {
                cnt1++;
            }
```
```python
else
```
```python
if
```
```python
(!cnt2) {
                vote2 = num;
                cnt2++;
            }
```
```python
else
```
```python
if
```
```python
(cnt2 && num == vote2) {
                cnt2++;
            }
```
```python
else
```
```python
{
                cnt1--;
                cnt2--;
            }
        }
```
```python
if
```
```python
(!cnt1 && !cnt2)
```
```python
return
```
```python
{};
```
```python
if
```
```python
(!cnt2) two =
```
```python
false
```
```python
;
        cnt1 = cnt2 =
```
```python
0
```
```python
;
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
num : nums) {
```
```python
if
```
```python
(num == vote1) cnt1++;
```
```python
else
```
```python
if
```
```python
(two && num == vote2) cnt2++;
        }
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
ans;
```
```python
if
```
```python
(cnt1 > nums.size()/
```
```python
3
```
```python
) ans.push_back(vote1);
```
```python
if
```
```python
(cnt2 > nums.size()/
```
```python
3
```
```python
) ans.push_back(vote2);
```
```python
return
```
```python
ans;
    }
```
## Question3
给定一个流或者数组，一个在0与1之间的实数θ，能否找出其中出现频率超过θ的全部元素.
这里θ取0.5就是我们的question1。
相关论文：
[《A Simple Algorithm for Finding Frequent Elements in Streams and Bags》](http://www.cs.umd.edu/~samir/498/karp.pdf)
论文的结论：
这个问题可以在两次遍历这个流，在O(1/θ)的空间内解决。具体来说如下：
```python
//设X[1...n]为输入，是一个int流
```
```python
//我们把出现频率超过1/theta的数字叫做高频数
```
```python
map
```
```python
<
```
```python
int
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
K;
```
```python
//K中保存潜在的高频数 K[a] = b 表示遍历到当前时数字a还剩b个
```
```python
//第一遍遍历这个流,找出潜在的高频数
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
i =
```
```python
1
```
```python
; i <= n; ++i){
```
```python
//先将X[i]加到K中
```
```python
if
```
```python
( K.find(X[i]) != K.end() )
        K[X[i]] ++;
```
```python
else
```
```python
K[X[i]] =
```
```python
1
```
```python
;
```
```python
//如果K中元素超过 1/theta, 则K中元素每个都被舍弃一次
```
```python
if
```
```python
(K.size() >
```
```python
1
```
```python
/theta){
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
it = m.begin(); it != m.end();){
            it->second --;
```
```python
if
```
```python
(it->second ==
```
```python
0
```
```python
)
                m.erase(it++);
```
```python
else
```
```python
it++;
        }
    }
}
```
```python
// 此时所有高频数肯定在K里面，这时再遍历一遍确认一下即可
```
证明第一遍遍历后需要找的高频数字一定在K里面：
用反证法, 假设a是高频数, 但a最后不在K中。
由于遍历到a时，a一定被加入到K中了，因此唯一的可能性是它后来又被舍弃了。
注意到，我们在舍弃过程中，是遍历K中元素，每个都舍弃了一次(代码中it->second –)。由于此时K至少有1/θ个元素，因此我们至少舍弃了1/θ次。假设a在数组中出现了t次，则每次a被加入后最后都被舍弃，说明我们至少舍弃了t∗1/θ次，这个次数一定不能超过输入流元素的总数N。故
t∗1/θ<=N 即 t/N<=θ，这与a是高频数的定义矛盾！

