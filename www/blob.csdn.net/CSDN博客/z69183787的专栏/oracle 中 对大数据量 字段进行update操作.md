# oracle 中 对大数据量 字段进行update操作 - z69183787的专栏 - CSDN博客
2013年12月10日 11:24:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4016
最近对一个七十多万条数据的表进行update操作，刚开始直接使用Update语句，如下：
|`1`|`update``content ``set``pic_bak=pic;`|
发现对于一两万条数据的表非常快，但是对于几十万条的表来说就非常慢了，不推荐。因此在网上查阅了一下，采用了declare方法,具体实现如下：
[view
 source](http://my.oschina.net/guyfar/blog/73829#viewSource)
[print](http://my.oschina.net/guyfar/blog/73829#printSource)[?](http://my.oschina.net/guyfar/blog/73829#about)
|`01`|`declare``　　声明变量的关键字`|
|`02`|```v_num number;　　定义number类型的变量v_num```|
|`03`|`begin``　　`|
|`04`|```v_num :=11521488;　　　　给声明的变量赋值（:=），不支持声明时就赋值```|
|`05`|```while v_num < 17163145 loop　　 循环体```|
|`06`|`update``table_name ``set``tmp=content　　sql语句`|
|`07`|`where``id>=v_num　　　　　　　　　id是我的主键(sql中使用索引是更快)`|
|`08`|`and``id<v_num+3000;`|
|`09`|`commit``;　　　　　　　　　　每执行3000行一提交，这样确保不会出现错误时回滚`|
|`10`|```v_num := v_num+3000;```|
|`11`|`end``loop;`|
|`12`|`end``;`|
通过这种方法，发现执行七十多万条数据执行时间为3分左右，还可以接受。
