# 生成SQL SERVER数据库脚本 - 左直拳的马桶_日用桶 - CSDN博客
2012年05月18日 16:34:16[左直拳](https://me.csdn.net/leftfist)阅读数：4006
数据库迁移，如果源库的SQL SERVER版本与目标库相同，或更低，没有什么问题，可以采用2种方式：
1）备份、还原的方式
2）直接拷贝数据库文件的方式（从源库分离，拷贝到目标库附加）
但如果源库的SQL SERVER版本比目标库高，就没办法了，只好用 生成数据库脚本 + 导出导入数据 的老土办法了。生成数据库脚本好像也有个选项可以带数据，我没试过，但有大虾说这只适合少量数据的情况。
生成数据库脚本，有些地方值得一提：
1、权限默认没有选上。应该选上。点“高级”可发现
![](https://img-my.csdn.net/uploads/201205/18/1337328714_2530.JPG)
这样生成的脚本，就可以将权限带出来，如：
GRANT EXECUTE ON [dbo].[news_prd_GetForSlideShow] TO [spider] AS [dbo]
GO
存储过程[dbo].[news_prd_GetForSlideShow]  的 权限EXECUTE 赋给了用户spider
这里有一个比较古怪的现象，就是你想将用户的权限导出来的话，不能只选用户导出，要将表、存储过程、用户都选上，高级选项里再选上“生成对象级权限”，生成的脚本里才含有某个表对于某用户的权限如何如何，某个存储过程对某个用户如何如何。
2、在目标库，将用户与登录名映射。
数据库的用户与登录名是分开的。二者是映射的关系。一个登录名可以映射多个数据库里的用户，但每个数据库只能映射一个；而用户则只能映射一个登录名。
对于数据库迁移，很可能出现登录名跟之前发生变化的状况，或者要新增，或者是映射现有的。之前我一直习惯用户名和登录名的名字相同，迁移之后，搞得很狼狈。
在导出来的脚本中，对于用户名和登录名部分，会有如下语句：
/****** Object:  User [spider]    Script Date: 05/18/2012 10:09:27 ******/
CREATE USER [spider] FOR LOGIN [spider] WITH DEFAULT_SCHEMA=[spider]
GO
意思就是说将用户名 [spider] 映射到登录名 [spider]。（习惯了两个名字取得一模一样）
但在目标数据库实例里面，不存在一个这么一个叫[spider]的登录名，只有一个叫[big_spider]，那应该将语句改成
CREATE USER [spider] FOR LOGIN [big_spider] WITH DEFAULT_SCHEMA=[spider]
GO
或者硬着头皮运行脚本，报错，但用户名还是会建立起来，再加以修改：
EXEC sp_change_users_login 'update_one' , 'spider' , 'big_spider' ;
注意：
如果生成的用户脚本，有 WITHOUT LOGIN，则是指定不应将用户映射到现有登录名之意，
CREATE USER [myuser] WITHOUT LOGIN WITH DEFAULT_SCHEMA=[myuser]
GO
这种情况下，执行以上EXEC sp_change_users_login语句，会报错：
正在终止此过程。缺少User 名称'myuser' 或该名称无效。
仔细观察该用户，会是这种情况：
![](https://img-my.csdn.net/uploads/201206/07/1339022682_2245.png)
