# SQL走不了索引的情况 - Orisun - 博客园







# [SQL走不了索引的情况](https://www.cnblogs.com/zhangchaoyang/articles/3735951.html)





### 谓词使用IN、NOT IN、EXISTS、NOT EXISTS

 < 、> 、= 、!= 、<>、if null 、between依然可以走索引。

![](https://images0.cnblogs.com/i/103496/201408/191337562377634.png)

![](https://images0.cnblogs.com/i/103496/201408/191338073936043.png)

### LIKE前通配

like后通配依然可以走索引。

![](https://images0.cnblogs.com/i/103496/201408/191317142379796.png)

### 索引列使用了函数、数学运行、其他表达式等

![](https://images0.cnblogs.com/i/103496/201408/191305553316920.png)

建议：不要在where的过滤字段上加函数，而应该在比较值上加函数。

![](https://images0.cnblogs.com/i/103496/201408/191306109402090.png)

![](https://images0.cnblogs.com/i/103496/201408/191306199711883.png)



### 隐式类型转换

![](https://images0.cnblogs.com/i/103496/201408/191338229561497.png)

![](https://images0.cnblogs.com/i/103496/201408/191338317993234.png)

## 强制使用特定的索引

![](https://images2015.cnblogs.com/blog/103496/201604/103496-20160415190551082-425215248.png)
mysql会从possible_keys中选择一个代价小的索引，但并不是每次都能做出正确的选择。比如上例中where条件中包含了id和deliverTime，id是主键，deliverTime是索引，从下面的运行耗时来看mysql选择了按id进行查找，但实际上这样将扫描大量的数据，非常耗时。好在我们可以使用force index强制告诉mysql按哪个索引进行查找。

![](https://images2015.cnblogs.com/blog/103496/201604/103496-20160415191026254-194238791.png)

## 强制不使用特定的索引

select * from user_deliver ignore index(idx_delTime) where id>42342 and deliverTime>20160101;














