# on duplicate key update导致主键不连续自增的问题 - z69183787的专栏 - CSDN博客
2018年01月19日 15:07:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：487
[http://blog.csdn.net/u011900448/article/details/55044990](http://blog.csdn.net/u011900448/article/details/55044990)
最近项目上需要实现这么一个功能:统计每个人每个软件的使用时长,客户端发过来消息,如果该用户该软件已经存在增更新使用时间,如果没有则新添加一条记录,代码如下:
[html][view
 plain](http://blog.csdn.net/u011900448/article/details/55044990#)[copy](http://blog.csdn.net/u011900448/article/details/55044990#)
- <!-- 批量保存软件使用时长表 -->
- <updateid="saveApp"parameterType="java.util.List">
- <foreachcollection="appList"item="item"index="index"separator=";">
-             insert into app_table(userName,app,duration)  
-             values(#{userName},#{item.app},#{item.duration})  
-             on duplicate key update duration=duration+#{item.duration}  
- </foreach>
- </update>
为了效率用到了on duplicate key update进行自动判断是更新还是新增,一段时间后发现该表的主键id(已设置为连续自增),不是连续的自增,总是跳跃的增加,这样就造成id自增过快,已经快超过最大值了,通过查找资料发现,on duplicate key update有一个特性就是,每次是更新的情况下id也是会自增加1的,比如说现在id最大值的5,然后进行了一次更新操作,再进行一次插入操作时,id的值就变成了7而不是6.
为了解决这个问题,有两种方式,第一种是修改innodb_autoinc_lock_mode中的模式,第二种是将语句修拆分为更新和操作2个动作
第一种方式:innodb_autoinc_lock_mode中有3中模式,0,1和2,mysql5的默认配置是1,
0是每次分配自增id的时候都会锁表.
1只有在bulk insert的时候才会锁表,简单insert的时候只会使用一个light-weight mutex,比0的并发性能高
2.没有仔细看,好像是很多的不保证...不太安全.
数据库默认是1的情况下,就会发生上面的那种现象,每次使用insert into .. on duplicate key update 的时候都会把简单自增id增加,不管是发生了insert还是update
由于该代码数据量大,同时需要更新和添加的数据量多,不能使用将0模式,只能将数据库代码拆分成为更新和插入2个步骤,第一步先根据用户名和软件名更新使用时长,代码如下:
[html][view
 plain](http://blog.csdn.net/u011900448/article/details/55044990#)[copy](http://blog.csdn.net/u011900448/article/details/55044990#)
- <updateid="updateApp"parameterType="App">
-     update app_table  
-     set duration=duration+#{duration}  
-     where userName=#{userName} and appName=#{appName}  
- </update>
然后根据返回值,如果返回值大于0,说明更新成功不再需要插入数据,如果返回值小于0则需要进行插入该条数据,代码如下:
[html][view
 plain](http://blog.csdn.net/u011900448/article/details/55044990#)[copy](http://blog.csdn.net/u011900448/article/details/55044990#)
- <insertid="saveApp"keyProperty = "id"useGeneratedKeys = "true"parameterType="App">
-     insert into app_table(userName,appName,duration)  
-     values(#{userName},#{appName},#{duration})  
- </insert>
这样解决效率上肯定为受到影响,不知道会不会丢数据,观察一段时间再优化吧!
