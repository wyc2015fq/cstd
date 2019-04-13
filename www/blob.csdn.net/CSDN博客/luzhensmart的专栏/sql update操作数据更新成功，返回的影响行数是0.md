
# sql update操作数据更新成功，返回的影响行数是0 - luzhensmart的专栏 - CSDN博客


2019年03月08日 15:17:43[芦震](https://me.csdn.net/luzhensmart)阅读数：19


以前使用sql server， mysql时候，本能以为update操作返回的行数能判断对记录的操作结果，除了数据库异常，一般确实能反应操作的真实情况，但是，为何却出现update数据更新成功，反而返回影响行数为0的情况呢？
以为可以判断返回值是否大于0来判断更新是否成功，但是，下面的两种情况均返回0：
1、没有找到需要更新的数据
比如，我们进行update的时候，条件是id=5，但是id=5的数据不存在。这种情况下，更新是失败的，返回0，很正确；
2、要更新的数据和更新的值是完全一样的
比如，我们要对id=5的记录进行更新，把title变成hello。虽然这条记录存在，但是这条记录的title本来就是hello，那么，返回值也是0；
3、本身写的sql语句有问题
比如，update test set cid = ‘5244’ where cid is null or cid=‘’；
第一种情况可以快分析到，但是调查发现不是这种问题，第二种和第三种为啥会出现这样的问题呢? 调查日志发现，同一时间接受到了两条数据导致了二三的出现，所以，大家在碰到这样的问题的时候，看看是不是因为并发请求导致的。控制好自己的并发事物，不然就会怀疑update的这个操作喽~
重大补充：最近发现update操作使用返回的的影响行数来判断操作成功与否，有时候还与第三方持久化框架有关
使用原生的sql（或者mysql） 重复请求update操作的时候，如 update test set cid = ‘5244’；执行两次的时候发现，update返回影响行数为0
但是使用mybatis的时候，结果却不是这样的，执行两次的时候，发现update返回的影响行数始终是1。**spring + mybatis  方法 update 的返回值 默认：是 Rows matched****不是 rows affected（或者Changed）**。后面跟踪发现，原来mybatis这个第三方持久化框架返回来的是被匹配的影响行数，所以返回的始终是1，那如果想要以返回的行数作为判断结果，需要在mybatis连接数据库的时候加上参数，
**jdbc:mysql://${jdbc.host}/${jdbc.db}?useAffectedRows=true**
### 获取matched行数
默认情况下，返回的就是matched 行数。也就是查询条件匹配的行数。
defaultExecutorType有三个执行器SIMPLE、REUSE和BATCH。其中BATCH可以批量更新操作缓存SQL以提高性能，但是有个缺陷就是无法获取update、delete返回的行数。defaultExecutorType的默认执行器是SIMPLE。
> int count = mapper.insertStudent(stu);

> 如果有SQL片段，update student_info set sex = '1' where id =1; 数据库里面执行会影响一条记录

> 当mybatis第一次执行的时候返回的1

> 当mybatis第二次执行的时候返回的也是1,其实我们想要获取的是更改的记录数。也就是Affected rows.

### 如何获取Affected rows
> 因为mybatis返回的默认是匹配的行数，而不是受影响的行数，如何设置返回的是受影响的行数，

> 加上useAffectedRows参数为true

> jdbc:mysql://${jdbc.host}/${jdbc.db}?useAffectedRows=true


