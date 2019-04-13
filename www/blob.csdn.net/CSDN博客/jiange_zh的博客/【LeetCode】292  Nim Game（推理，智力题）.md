
# 【LeetCode】292. Nim Game（推理，智力题） - jiange_zh的博客 - CSDN博客


2016年01月05日 21:34:12[jiange_zh](https://me.csdn.net/jiange_zh)阅读数：428


好久没刷题了，现在算法题做起来都很吃力。
接下来开始刷LeetCode，并同时复习算法与数据结构～
今晚注册了LeetCode，按难易度排序，选了第一道。
原题：
You are playing the following Nim Game with your friend: There is a heap of stones on the table, each time one of you take turns to remove 1 to 3 stones. The one who removes the last stone will be the winner. You will take the first turn to remove the stones.
Both of you are very clever and have optimal strategies for the game. Write a function to determine whether you can win the game given the number of stones in the heap.
For example, if there are 4 stones in the heap, then you will never win the game: no matter 1, 2, or 3 stones you remove, the last stone will always be removed by your friend.
想了一小会，搜索？DP？
不！这不是一道easy的题吗？不会那么复杂的吧？……
放弃……还是看看题解吧……
T^T……
代码只有一行：
```python
return
```
```python
n
```
```python
%4
```
```python
;
```
What？原来是一道规律题，突然间想起Alice and Bob系列题目……
然而规律从何而来，以下是题友的解释：
I think this thinking way can help us to solve and understand this problem quickly.
First,Let’s begin with some tries.
when the rock is 1(win) 2(win) 3(win) 4(lose) I think everyone can do this,then we stop and think.
After you take rock and your friend becomes “you” think it carefully for example. when there are 4 rocks,after your taking ,there are three situations.so your friend begin with 1,2,or 3 you lose(when there are 4 rocks) = your friend win = start with(4-1),(4-2),(4-3) win
so there must be 3 win before 1 lose
so 4 is a group.
in c language we just return n%4
by 会飞的鸵鸟
下面是我根据该网友的说法整理的思路：
n=1,2,3时你赢，n=4时对手赢；
n=5时，根据上一点，你只需先拿走1个石子，于是剩下4个，由于“n=4时对手赢”，而对手的对手是你，因此你必赢；
n=6，7时同理；
n=8时，你有3种选择，结果分别剩下7,6,5个石子，此时对手站在你的角度，根据上面结论，对手必赢。
……如何循环下去，你总是3赢1输。
ORZ，接下来要好好补补脑了！
One day one question at least！

