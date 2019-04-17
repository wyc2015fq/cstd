# [CareerCup] 10.6 Find Duplicate URLs 找重复的URL链接 - Grandyang - 博客园







# [[CareerCup] 10.6 Find Duplicate URLs 找重复的URL链接](https://www.cnblogs.com/grandyang/p/4865651.html)







10.6 You have 10 billion URLs. How do you detect the duplicate documents? In this case, assume that "duplicate" means that the URLs are identical.



这道题让我们在一百亿个URL链接中寻找相同项，看这数据量简直吓尿了，如果每个URL链接平均100个字符的话，每个字符是4个字节，那么总共需要占4TB的空间，我们无法在内存中导入这么大的数据量。假如我们恩能够把数据全部导入到内存中，那么找重复项就不是一件难事，我们可以使用哈希表来建立每个URL和其是否存在过建立映射，很容易能找到重复项。那么下面来看我们怎么处理这么大的数据量，我们可以有如下两种方法：

1. 硬盘存储

将所有的数据存到一台机子上，我们可以把4TB的数据分为4000份，每份1GB大小，然后我们把每个URL u存在文件x.txt中，其中x=hash(u)%4000，这样具有相同哈希值的URL都被放到一个文件中了。然后我们再把每个文件导入内存，来寻找重复值。

2. 多台机器

另一种方法是使用多台机器，我们不是将数据存在x.txt，而是将URL发给机器x. 使用这种方法有好处也有坏处。好处是可以并行操作，4000个块可以同时进行操作。坏处是我们需要4000台机器，这不太现实，而且还要考虑如何处理失败。














