# [CareerCup] 6.3 Water Jug 水罐问题 - Grandyang - 博客园







# [[CareerCup] 6.3 Water Jug 水罐问题](https://www.cnblogs.com/grandyang/p/4753748.html)







6.3 You have a five-quart jug, a three-quart jug, and an unlimited supply of water (but no measuring cups). How would you come up with exactly four quarts of water? Note that the jugs are oddly shaped, such that filling up exactly "half" of the jug would be impossible.



这道题给了我们两个水罐，一个是5夸脱的，一个是3夸脱的，还有无限的水，让我们测量出4夸脱的水，前提是我们不能每次给容器装一半的水，那么这需要来回倒好几次，才能装出4夸脱的水，参见步骤如下：
|5夸脱水罐当前水量|3夸脱水罐当前水量|步骤|
|----|----|----|
|5|0|装满5夸脱水罐|
|2|3|用5夸脱水罐装满3夸脱水罐|
|2|0|倒空3夸脱水罐|
|0|2|把5夸脱水罐中的水倒入3夸脱水罐|
|5|2|装满5夸脱水罐|
|4|3|用5夸脱水罐装满3夸脱水罐|
|4||我们得到了4夸脱的水|












