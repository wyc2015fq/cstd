# 一个潜伏了 5 年的 bug - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [usher2007](http://www.jobbole.com/members/usher2007) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
前两天策划反馈某项活动中，理论上大概千分之一中奖率的一个奖品，连续四次被同一个玩家得到了。他之前已经找了几个程序复查过代码，都说没啥问题，但是从概率上讲，肯定是有问题的，所以希望我“为程序员正名”。经过一上午的奋战，我也算不辱使命，找出了这个潜伏了5年的bug，也了解了关于mysql的一个潜规则。
整个业务场景大概是这样的（下面代码仅为示意，不可直接运行）：
首先，每个玩家报名的时候，向数据库中插入一行，记录下活动id（activity_id），玩家uuid（player_uuid），申请序号（apply_no）以及一些其他的玩家相关信息。
同时，在内存中维护一个整数，记录当前活动的申请总人数apply_total_num。同时使用这个整数来获得下一个插入行的apply_no。
第二，由策划填写的数据表导成数据结构表示奖励结构，即每个等级的奖励各有多少份，如下所示：


```
awards = {
	1: 1, 
	2: 15, 
	3: 50, 
	...
	}
```
第三，当活动报名完毕时，根据当前的apply_total_num和总奖励份数sum(awards.values())，生成一个长度为总奖励份数的随机的apply_no序列。比如有10000人报名，总奖励分数是1000，那么我们有可能生成如下的一个包含1000个apply_no的序列：


```
lucky_apply_nos = [234, 123, 1356, 8765, 12, ...]
```
第四，从MySQL数据库中取出lucky_apply_nos对应的uuid：


```
lucky_uuids = SELECT player_uuid FROM world_lottery WHERE apply_no IN lucky_apply_nos;
```
拿到lucky_uuids之后，根据awards对应分配奖励：lucky_uuids[0]对应1等奖，lucky_uuids[1]~lucky_uuids[15]对应2等奖，依此类推。
看上去没有什么问题吧。其实问题就出现在SELECT语句这里了。在这段流程中有一个想当然的推论，就是lucky_uuids的顺序是和lucky_apply_nos中的顺序一致的。但是，其实这里踩了MySQL的一个潜规则坑：
对于没有ORDER BY子句的SELECT语句，其返回顺序是和所使用的引擎有关：
> 
对于MyISAM引擎来说，其返回顺序是其物理存储顺序；
对于InnoDB引擎来说，其返回顺序是按照主键排序的。
也就是说，无论哪种情况，都不会按照IN子句里的列表lucky_apply_nos的顺序返回……
具体到我们这里的情况来说，引擎使用的是InnoDB，主键是player_uuid，也就是说返回的lucky_uuids是依据palyer_uuid排过序的。
那这样会导致什么问题呢？
如果某个玩家的uuid特别小，那么只要这个玩家进入了奖励大名单（lucky_apply_nos），那么他就一定会获得价值最高的1等奖。这样这个玩家连中四次一等奖的概率就是0.1^4，而不是0.0001^4。
这个玩法如果在奖励大名单的中奖概率和各个奖项的中奖概率相差不大的时候，bug体现的就不太明显。但是在这次活动中，策划加大了奖励大名单的数量（主要是通过增加低价值奖励的数量），这样就比较容易出现某位玩家连中几次高价值奖励的情况了。
解决方法也很简单，拿到数据库的返回结果lucky_uuids之后，再进行一次shuffle即可。
## 总结
MySQL对于无ORDER BY子句的SELECT的语句的返回结果有潜规则：
- 对于MyISAM引擎来说，其返回顺序是其物理存储顺序；
- 对于InnoDB引擎来说，其返回顺序是按照主键排序的。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/08/a41e08fcaca939c581c931c115da0a8d.png)![](http://jbcdn2.b0.upaiyun.com/2016/08/c07df7ab8dad3a49f6ce602eb7d62949.jpg)
