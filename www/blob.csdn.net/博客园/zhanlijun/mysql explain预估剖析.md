# mysql explain预估剖析 - zhanlijun - 博客园
# [mysql explain预估剖析](https://www.cnblogs.com/LBSer/p/3333881.html)
[**http://www.cnblogs.com/LBSer/p/3333881.html**](http://www.cnblogs.com/LBSer/p/3333881.html)
**引子：**
　　使用MySQL建立了一张表country，总共有才3121行记录。
但是使用explain select count(*) from country;的时候，发现行数rows达到6897，让我大吃一惊。
```
mysql> explain select count(*) from country;
+----+-------------+---------+------+---------------+------+---------+------+------+-------+
| id | select_type | table   | type | possible_keys | key  | key_len | ref  | **rows** | Extra |
+----+-------------+---------+------+---------------+------+---------+------+------+-------+
|  1 | SIMPLE      | country | ALL  | NULL          | NULL | NULL    | NULL | 6897 | NULL  |
+----+-------------+---------+------+---------------+------+---------+------+------+-------+
```
**问题：为什么explain的结果和真实的结果运行不一致，并且产生这么大的误差？**
　　针对这个问题，上网查了些资料，特此发博文总结下，当然自己也是刚刚使用mysql，有很多不了解的地方，希望多多指正。
**一、explain是什么？**
　　通过explain可以查看MySQL的执行计划，从而知道MySQL是如何处理我们的SQL语句。具体来说通过explain我们能得到一系列的关键信息，比如哪些索引被实际使用，查询了多少行等等。
　　explain使用Rows来告知我们数据库即将要阅读的行数，但是实际将要阅读的行数和explain所记载的将要阅读的行数可能会有差异，这是因为explain并没有真的去执行sql语句从而得出行数，而是进行了某种预估。
**二、explain怎么预估行数**
　　找了半天得知真相的我眼泪掉下来：[http://lists.mysql.com/commits/115810](http://lists.mysql.com/commits/115810)
1）mysql-5.5之前
　　首先找到查询第一个记录所在的page（记为*PLeft*），统计PLeft里的记录数（记为*Records_PLeft*），之后找到最后一个记录所在的page（记为*PRight*），统计PRight的记录数（*Records_PRight*），之后将*Records_PLeft*与*Records_PRight*取平均，最后乘以总共的page数目（记为*Page_Num*）。公式如下：
```
***Rows = ((Records_PLeft + Records_PRight)/2)*Page_Num***
```
　　统计上讲这个预估方法是很有偏的。比如总共4个page：page1(999 records), page2(1 record), page3(1 record), page4(1 record)，这样预估出来的Rows=((999+1)/2)*4 = 2000，然而实际上才总共才有1002个记录。
2）mysql-5.5之后
　　上述预估偏差大的关键在于有偏，而有偏的关键在于采样的page数太少了，事实上只采样了边界2个，新算法的思路很简单，增加采样数目，比如采样10个page，这样可以在一定程度上降低偏差。
　　具体来说，mysql除了边界2个外，还沿着左侧page往右连续查找8个page，如果总的page数目小于等于10个，那么预估的Rows和真实的Rows一致。
```
**Rows = ((Records_PLeft +  Records_P1 + Records_P2 + ... + Records_P8 + Records_PRight)/10)*Page_Num**
```
上述方法只是在一定程度上缓解了有偏的问题，但是不准确还是存在的，事实上楼主的mysql版本是*5.6*版本，可见还是没有解决的很好*。*
**三、思考**
　　为什么是从左往右连续选8个page，而不是在首尾之间随机选择8个page，既然要缓解采样有偏的问题，那么随机选应该更好。猜想可能有两个原因：1）随机选择每次explain得到的Rows不一样，不方便应用；2）随机选会造成I/O开销，尤其是数据量大的时候，毕竟explain是希望能快速得到预估结果。
　　我觉得应该还有更好的算法，能实现explain效率与精度的tradeoff，希望大家能给出建议。

