# [CareerCup] 6.4 Blue Eyes People on Island 岛上的蓝眼人 - Grandyang - 博客园







# [[CareerCup] 6.4 Blue Eyes People on Island 岛上的蓝眼人](https://www.cnblogs.com/grandyang/p/4753751.html)







6.4 A bunch of people are living on an island, when a visitor comes with a strange order: all blue-eyed people must leave the island as soon as possible. There will be a flight out at 8:00pm every evening. Each person can see everyone else's eye color, but they do not know their own (nor is anyone allowed to tell them). Additionally, they do not know how many people have blue eyes, although they do know that at least one person does. How many days will it take the blue-eyed people to leave?



这道题假定一个岛上有很多人，其中有些人是蓝眼睛的，设定是蓝眼睛的人要尽可能早的离开岛，每天晚上八点都有飞机离开岛。每个人可以看见别人眼睛的颜色，但是不知道自己眼睛的颜色，而且也不能被告知，问需要多少天可以让蓝眼睛人都离开。

这道题让我想起来了很小时候听过的一道智力题，是小猴带帽的问题，说是有四个帽子，三个蓝色一个红色的，有三只小猴一人戴其中一个帽子，每只小猴不知道自己戴的什么颜色的帽子，但能看到其他小猴戴的帽子的颜色。其中一只小猴看到其他两只小猴都是蓝色的帽子，它就在考虑自己的到底是红色的还是蓝色的，当它发现大家都陷入了沉思时，它马上说自己戴的是蓝色的，请问问什么？我小时候想了很久都不明白，其实很简单，如果这小猴戴的是红色的帽子，那么其他小猴马上就知道自己戴的是蓝色的，因为只有一顶红色的帽子，既然其他小猴也不知道，那么自己戴的肯定是蓝色的。

这道题的思路和上面的小猴戴帽有些类似，我们先从简单的开始分析，假设岛上有c个人是蓝眼睛：

1. c = 1, 这种情况最简单，应为那个人看到周围都没有蓝眼睛，由于题目设定了一定有蓝眼睛，那么自己肯定是，所以当晚就可以坐飞机离开。

2. c = 2，这时候蓝眼睛的人可以看到周围有一个人是蓝眼睛，那么他可以推断岛上只有1个或2个蓝眼睛人，如果那人第一晚，没走，他就可以推断出自己也是蓝眼睛，于是两个蓝眼睛的人第二晚同时走了。

3. c = 3，和上面类似，蓝眼睛的人可以看到周围有两个蓝眼睛，则推断岛上有2个或3个蓝眼睛人，如果只有两个蓝眼睛，那么他两第二天晚上就会同时离开，如果第二晚没有离开，则说明自己也是蓝眼睛，则三人第三晚上同时离开。

4. c = c, 推广到c个人，根据如上分析，c个蓝眼睛的人会等到第c天晚上，所有c个蓝眼睛的人同时离开。














