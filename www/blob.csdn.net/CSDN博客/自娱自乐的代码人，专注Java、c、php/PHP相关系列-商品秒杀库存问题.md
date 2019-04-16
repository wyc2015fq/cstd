# PHP相关系列 - 商品秒杀库存问题 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月19日 15:00:49[initphp](https://me.csdn.net/initphp)阅读数：3787









库存问题一直是电子商务一个头疼的话题，我们开始做网上销售商品时也经常遇到库存为负，出现超卖的问题，相信很多人也遇到过，特别是秒杀活动中．防止这个问题首先要在mysql操作前拦掉一些并发，如用Beanstalkd之类的队列工具，这里只说下mysql层面怎么防止超卖．

乐观锁

#buy_num# : 购买数

#id# : 商品id

update table set quantity=quantity - #buy_num# where id = #id# and 
quantity >= #buy_num#;


关键点在where里的quantity >= #buy_num#，这样只有库存大于等于购买数的时候才能执行成功．从而避免了库存出现负数





