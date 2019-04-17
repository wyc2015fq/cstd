# [CareerCup] 7.2 Ants on Polygon 多边形上的蚂蚁 - Grandyang - 博客园







# [[CareerCup] 7.2 Ants on Polygon 多边形上的蚂蚁](https://www.cnblogs.com/grandyang/p/4765857.html)







7.2 There are three ants on different vertices of a triangle. What is the probability of collision (between any two or all of them) if they start walking on the sides of the triangle? Assume that each ant randomly picks a direction, with either direction being equally likely to be chosen, and that they walk at the same speed. Similarly, find the probability of collision with n ants on an n-vertex polygon.



这道题说有一个三角形，每个顶点上有一只蚂蚁，如果它们随机选一个方向匀速爬，问任意两只会碰撞的概率是多少？然后又拓展到n边形，放n只蚂蚁，还是求会碰撞的概率。

我刚开始还在考虑三角形是否为等边三角形，其实跟这些都没啥关系。任意两只蚂蚁碰撞的概率不好求，我们可以反其道行之，先求出不会碰撞的概率，然后取个反也能达到目的。那么不会有碰撞的概率只有两种情况，所有蚂蚁都顺时针爬，或者都逆时针爬。那么对于三角形来说，3只蚂蚁都顺时针爬的概率是0.53,都逆时针爬的概率也是0.53，则相同方向爬的概率是0.53+0.53=0.25，则会碰撞的概率为1-0.25=0.75。那么对于n边形和n只蚂蚁也是同理，n只蚂蚁都顺时针爬的概率是0.5n,都逆时针爬的概率也是0.5n，则相同方向爬的概率是0.5n+0.5n=0.5n-1，则会碰撞的概率为1-0.5n-1














