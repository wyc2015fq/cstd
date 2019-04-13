
# intersection算子耗时问题 - 郭云飞的专栏 - CSDN博客


2014年01月03日 13:09:52[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：2259


在Halcon中，我们通常认为算子
```python
intersection(Region1, Region2 : RegionIntersection : : )
```
中的Region1和Region2是不分先后的。大部分情况下他们确实不用分先后顺序，因为大部分情况下，输入的两个区域都各包含一个区域，此时无论两个区域谁是先谁是后，出来的结果都是一样的。
但根据Halcon的解释：
intersectioncalculates the intersection of the regions inRegion1with the regions inRegion2.
 Each region inRegion1is intersected with all regions inRegion2.
 That is, internally all regions ofRegion2are united to a single region before the individual regions ofRegion1are intersected with the united region. The order of regions inRegionIntersectionis identical to the order of regions inRegion1.
我们知道，如果Region1和Region2分别包含多个区域时，出来的结果会不尽相同。算则内部会把Region2内所有的区域union1一下，然后将Region1中的区域逐个和union1之后的区域取交集。
但本文不是关注这种区别。而是这个算子的执行时间问题。实际使用时发现：
```python
intersection(RegionBig, RegionSmall, RegionIntersection)
```
```python
intersection(RegionSmall, RegionBig, RegionIntersection)
```
即一个区域非常大，一个区域非常小时，求其交集时两个区域放置的先后顺序不同，算子的执行时间有很大差距。把大的区域放到前边会非常省时，反之则非常耗时。

