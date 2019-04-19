# MongoDB使用初步 - 左直拳的马桶_日用桶 - CSDN博客
2014年12月05日 22:54:42[左直拳](https://me.csdn.net/leftfist)阅读数：1607
我很快就要离开现在这个使用nodejs + mongodb + redis的项目，转而去搞 塞特ID 之类的别的项目了。可惜这些技术对我来说浅尝辄止，半生不熟，胎死腹中。业余时间自学当然也可以，但哪有实际工作应用进步那么快？你可知道，上两周因为刚接触CoffeeScript，好多东西不懂，被迫夜夜加班，有一天还搞到凌晨2点半才回去的？巨大的压力下，硬是抗了过来，进步也较快。coffeescript内容不多，现在已经基本能读会写了。
其他也差不多。nodejs体系中，好多新工具，新语言，但内容都不多，花一点时间一般可以掌握。在这个微博、短信、快餐盛行的时代，什么都讲求短平快，连技术也一样。那些类似java、.NET等基本书籍就有厚厚一大本的技术，已经不合时宜了？
人年纪大了，记忆力就不行。趁我还没忘记前，先记录一下一些mongodb的操作
1、进入mongodb
$ mongo [host:port]
如果是redis,则为
$ redis-cli -h host -p port
2、查看库、转到某库、查看当前库下的表
> show dbs 
> use test 
> show collections
3、查看某表下的记录
> db.users.find().pretty() 
> db.users.find({"email":"chendaqu@163.com"})
4、删除
> db.users.remove({"email":"chendaqu@163.com"});
5、更新
> db.users.update({"email":"chendaqu@163.com"},{$set:{"email":"chendaqu@hotmail.com"}},false,false);
这个更新要比较小心，有4个参数。
第一个参数是过滤条件
第二个参数是更新操作。注意要有这个$set，代表将 "email"字段更改。如果没有这个东西，那么这条记录“email”将更改内容，但其他所有字段都被删掉！整条记录只剩下一个email字段。多么奇特的语法。
第三个参数是，true的话，如果没有符合过滤条件的记录，则按照更新值帮你插入一条。多么奇特的做法。
第四个参数是，true的话，凡符合条件的记录都被更改，否则只更改第一条。多么奇特的做法
