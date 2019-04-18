# select * from Employee group by name这样的语法有什么意义？ - z69183787的专栏 - CSDN博客
2017年01月16日 10:52:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1401
神奇的mysql才会支持select * from Employee group by name 这种反逻辑的SQL（假定该表非仅name一个列）
mysql
 的逻辑是：select 的返回字段，如果不在groupby中，也不是统计函数，那么字段的取值是从匹配的记录中随机取的；在做表关联统计的时候，可以简化 group by 后面的字段，只需要 group by parent.id 就可以了，而不用把 parent 的所有字段都列出来
在表关联统计的时候，mysql这种特性可以简化
 group by 后面的字段，但跟排序没太大关系
例如，订单表inv和明细表item，如果想返回inv的所有字段和item的个数，可以这样写：
select inv.*, count(itme.id) item_count
from inv left join item on inv.id=item.inv_id
group by inv.id
