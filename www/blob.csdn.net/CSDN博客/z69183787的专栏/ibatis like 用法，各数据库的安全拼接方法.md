# ibatis like 用法，各数据库的安全拼接方法 - z69183787的专栏 - CSDN博客
2015年06月10日 21:13:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：966
$ 的作用实际上是字符串拼接，
select * from $tableName$
等效于
StringBuffer sb = new StringBuffer(256);
sb.append(“select * from “).append(tableName);
sb.toString();
#用于变量替换
select * from table where id = #id#
等效于
prepareStement = stmt.createPrepareStement(“select * from table where id = ?”)
prepareStement.setString(1,’abc’);
————————————————
对于变量部分， 应当使用#， 这样可以有效的防止sql注入， 未来，# 都是用到了prepareStement，这样对效率也有一定的提升
$只是简单的字符拼接而已，对于非变量部分， 那只能使用$， 实际上， 在很多场合，$也是有很多实际意义的
例如
select * from $tableName$ 对于不同的表执行统一的查询
update $tableName$ set status = #status# 每个实体一张表，改变不用实体的状态
特别提醒一下， $只是字符串拼接， 所以要特别小心sql注入问题。
总结如下: 
1.#是把传入的数据当作字符串，如#field#传入的是id,则sql语句生成是这样，order by "id",这当然会报错．． 
2.$传入的数据直接生成在sql里，如#field#传入的是id,则sql语句生成是这样，order by id, 这就对了． 
3.#方式能够很大程度防止sql注入． 
4.$方式无法方式sql注入． 
5.$方式一般用于传入数据库对象．例如传入表名. 
6.一般能用#的就别用$.
