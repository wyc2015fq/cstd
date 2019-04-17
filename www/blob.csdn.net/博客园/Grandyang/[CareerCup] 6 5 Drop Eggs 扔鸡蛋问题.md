# [CareerCup] 6.5 Drop Eggs 扔鸡蛋问题 - Grandyang - 博客园







# [[CareerCup] 6.5 Drop Eggs 扔鸡蛋问题](https://www.cnblogs.com/grandyang/p/4762756.html)







6.5 There is a building of 100 floors. If an egg drops from the Nth floor or above, it will break. If it's dropped from any floor below, it will not break. You're given two eggs. Find N, while minimizing the number of drops for the worst case



这道题说有100层楼，从N楼开始扔鸡蛋会碎，低于N楼扔不会碎，现在给我们两个鸡蛋，让我们找到N，并且最小化最坏情况。

因为只有两个鸡蛋，所以第一个鸡蛋应该是按一定的间距扔，比如10楼，20楼，30楼等等，比如10楼和20楼没碎，30楼碎了，那么第二个鸡蛋就要做线性搜索，分别尝试21楼，22楼，23楼等等直到鸡蛋碎了，就能找到临界点。那么我们来看下列两种情况：

1. 假如临界点是9楼，那么鸡蛋1在第一次扔10楼碎掉，然后鸡蛋2依次遍历1到9楼，则总共需要扔10次。

2. 假如临界点是100楼，那么鸡蛋1需要扔10次，到100楼时碎掉，然后鸡蛋2依次遍历91楼到100楼，总共需要扔19次。

所以上述方法的最坏情况是19次，那么有没有更少的方法呢，上面那个方法每多扔一次鸡蛋1，鸡蛋2的线性搜索次数最多还是10次，那么最坏情况肯定会增加，所以我们需要让每多扔一次鸡蛋1，鸡蛋2的线性搜索最坏情况减少1，这样恩能够保持整体最坏情况的平衡，那么我们假设鸡蛋1第一次在第X层扔，然后向上X-1层扔一次，然后向上X-2层扔，以此类推直到100层，那么我们通过下面的公式求出X：

X + (X-1) + (X-2) + ... + 1 = 100 -> X = 14

所以我们先到14楼，然后27楼，然后39楼，以此类推，最坏情况需要扔14次。












