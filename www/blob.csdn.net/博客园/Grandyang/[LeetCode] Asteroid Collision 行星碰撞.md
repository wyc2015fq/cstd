# [LeetCode] Asteroid Collision 行星碰撞 - Grandyang - 博客园







# [[LeetCode] Asteroid Collision 行星碰撞](https://www.cnblogs.com/grandyang/p/8035551.html)







We are given an array `asteroids` of integers representing asteroids in a row.

For each asteroid, the absolute value represents its size, and the sign represents its direction (positive meaning right, negative meaning left). Each asteroid moves at the same speed.

Find out the state of the asteroids after all collisions. If two asteroids meet, the smaller one will explode. If both are the same size, both will explode. Two asteroids moving in the same direction will never meet.

Example 1:
Input: 
asteroids = [5, 10, -5]
Output: [5, 10]
Explanation: 
The 10 and -5 collide resulting in 10.  The 5 and 10 never collide.



Example 2:
Input: 
asteroids = [8, -8]
Output: []
Explanation: 
The 8 and -8 collide exploding each other.



Example 3:
Input: 
asteroids = [10, 2, -5]
Output: [10]
Explanation: 
The 2 and -5 collide resulting in -5.  The 10 and -5 collide resulting in 10.



Example 4:
Input: 
asteroids = [-2, -1, 1, 2]
Output: [-2, -1, 1, 2]
Explanation: 
The -2 and -1 are moving left, while the 1 and 2 are moving right.
Asteroids moving the same direction never meet, so no asteroids will meet each other.



Note:
- The length of `asteroids` will be at most `10000`.
- Each asteroid will be a non-zero integer in the range `[-1000, 1000].`.



这道题用一个数组来模拟行星碰撞，正数代表行星向右移动，负数表示向左移动，绝对值大小表示行星的质量，如果两个相邻的行星相向移动会碰撞，质量大的行星会完好无损的保存，质量小的就会灰飞烟灭。那么博主最开始想的方法就是按照题目要求来一个一个的处理，我们先把给定的数组放到结果res中，然后进行while循环，如果此时结果res中的数字个数小于等于1个，直接返回即可，没有可碰撞的了。否则我们建立一个临时数组t，把结果res中的首元素放到t中，然后从第二个数字开始遍历结果res，如果此时t为空了，或者当前数字大于0而t数组最后一个数字小于0（此时两个行星向相反方向运动，不会相撞），或者两个数字的符号相同（此时两个行星向同一个方向运动，不会相撞），这三种情况下都把当前数字res[i]加到数组t中；那么剩下的情况就是两个行星相向运动了，如果两个数字相加等于0，则说明两个行星质量相同，且相向运动，则一起消失，我们将数组t中最后一个数字移除；如果当前数字小于0，且两个数字相加小于0，那么此时相撞后会留下质量大的行星，我们将数组t的最后一个数字赋值为res[i]即可。for循环之和，如果数组t和结果res的大小相等，说明此时状态已经稳定了，我们直接break，否则就把数组t赋值给结果res并继续循环，参见代码如下：



解法一：

```
class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        vector<int> res = asteroids;
        while (true) {
            if (res.size() <= 1) return res;
            vector<int> t{res[0]};
            for (int i = 1; i < res.size(); ++i) {
                if (t.empty() || (res[i] > 0 && t.back() < 0) || res[i] * t.back() > 0) {
                    t.push_back(res[i]);
                } else if (res[i] + t.back() == 0) {
                    t.pop_back();
                } else if (res[i] < 0 && res[i] + t.back() < 0) {
                     t.back() = res[i];
                }
            }
            if (t.size() == res.size()) break;
            else res = t;
        }
        return res;
    }
};
```



实际上我们可以写的更加简洁一些，我们遍历所有的数字，如果当前数字是正数的话，我们直接加入结果res；否则我们遇到的都是负数，如果结果res为空，或者结果res的最后一个数字小于0（此时两个行星同时向左运动），直接将当前数字加入结果res；如果结果res的最后一个数字（此时为正数）小于当前数字的绝对值，说明碰撞后消失了，那么我们将i自减一个，然后将res最后一个数字移除，这样下次遍历的时候还是这个质量大的行星。如果两个质量相等，那么直接移除res最后一个数字，此时两个行星都消失了，参见代码如下：



解法二：

```
class Solution {
public:
    vector<int> asteroidCollision(vector<int>& asteroids) {
        vector<int> res;
        for (int i = 0; i < asteroids.size(); ++i) {
            if (asteroids[i] > 0) {
                res.push_back(asteroids[i]);
            } else if (res.empty() || res.back() < 0) {
                res.push_back(asteroids[i]);
            } else if (res.back() <= -asteroids[i]) {
                if (res.back() < -asteroids[i]) --i;
                res.pop_back();
            }
        }
        return res;
    }
};
```



类似题目：

[Can Place Flowers](http://www.cnblogs.com/grandyang/p/6983982.html)



参考资料：

[https://discuss.leetcode.com/topic/112034/java-c-clean-code](https://discuss.leetcode.com/topic/112034/java-c-clean-code)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












