# hive left outer join问题 - zj360202的专栏 - CSDN博客





2013年08月09日 19:03:39[zj360202](https://me.csdn.net/zj360202)阅读数：675
个人分类：[Hadoop																[Hive](https://blog.csdn.net/zj360202/article/category/1298103)](https://blog.csdn.net/zj360202/article/category/1298102)







select * FROM t_xg_product a join t_xg_product_subject_mapping m on (m.product_id=a.product_id)

left outer join 

(select (case when d.product_id is null then '' else cast(d.product_id as string) end) as product_id,

(case when count(1) > 0 then count(1) else cast(0 as bigint) end) as order_num_i 

from src_xg_ordr_item d where substr_split(substr(d.create_time,0,10),'-',0,2,'')='20130805' group by d.product_id) e on (a.product_id=e.product_id)

;




left outer join 的时候  on的条件 （a.product_id=e.product_id）两个product_id的类型必须是一样的，不然就是等价匹配，而不是左联接匹配




