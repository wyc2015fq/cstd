# 修改oracle密码有效期限制及其他profile - z69183787的专栏 - CSDN博客
2013年11月07日 17:04:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1469
修改oracle密码有效期限制
由于项目的一个bug，导致好几个现网项目都出现了异常。
bug说明：
oracle11g，静默安装后用户的密码有效期默认设置为180天，180天后密码将失效，oracle会提示要修改密码。
我们项目用的是jdbc连接oracle数据库，没法自动处理oracle的这种密码错误提示，直接就连接oracle失败。
解决思路一：
定期修改数据库用户密码。
解决思路二：
将数据库密码设置为永久有效。
思路一具有一定的安全保护作用，但由于业务不支持修改业务部件中的数据库密码配置（部件较多，修改过于麻烦，所以干脆要求不能修改，想想有点矬），于是决定采用思路二。
具体设置命令如下：
假定我们所使用的数据库用户为test，密码为test123456
步骤一：登陆oracle所在的linux服务器（我们的oracle假设在linux服务器上），注意要用dba用户。
步骤二：执行'sqlplus / as sysdba'
步骤三：输入'alter profile default limit password_life_time unlimited;'并回车
步骤四：输入'alter user test identified by test123456;'
步骤五：输入'quit;'退出sqlplus。
步骤六：重启各个业务部件，一切OK。
改了用户的profile后，只需要该user退出再重新登录一次，该用户所使用的新profile就生效了。
需要注意的是，对于第四步，可能会失败，因为oracle有可能设置了修改密码时不能与历史密码重复的限制，我们可以先执行以下命令去掉限制，然后再执行第四步。虽然对于第四步而言设置前后密码没有改变，但却是必要的吗，实际测试中发现即使做了第三步处理，如果不执行第四步，jdbc连接数据时依然会连接失败。
'alter profile default limit password_reuse_max unlimited;'
'alter profile default limit password_reuse_time unlimited;'
 在使用profile并想重复利用密码的时，需要满足如下条件
1）password_reuse_time和password_reuse_max都为unlimted的时候
可以随便重置
2）当指定password_reuse_time和password_reuse_max其中一个，另外一个unlimted的时候
密码永远无法重置
3）当同时指定password_reuse_time和password_reuse_max的时候
在满足password_reuse_max的时候，可以重置密码
在同时满足password_reuse_time和password_reuse_max的时候，可以重置密码
在满足password_reuse_time但从为更换过密码的时候，也就是password_reuse_max一次也没用的时候密码无法重置. 
上面这种处理方案是修复现网问题的弥补性措施，根本的要在oralce安装的时候就去掉密码有效期的限制。
