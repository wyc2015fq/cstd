
# 一个key 在10w  k-v  找到对应的. (B-tree), 这10w放进B-tree 会有多少层. - 致林 - 博客园






# [一个key 在10w  k/v  找到对应的. (B-tree), 这10w放进B-tree 会有多少层.](https://www.cnblogs.com/bincoding/p/5895687.html)
B-tree是二叉平衡查找树，相邻两层节点层数不超过1
所有10w 即 2^16=65536 < 10w < 2^17=131072；
会有17层，最多查询17次。





