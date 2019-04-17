# git错误: The requested URL returned error: 403 Forbidden while accessing https://github.com/wangz/futu - V__KING__的专栏 - CSDN博客





2013年11月04日 10:41:31[v__king__](https://me.csdn.net/V__KING__)阅读数：2997








github push错误：


- git push  
- error: The requested URL returned error: 403 Forbidden while accessing https://github.com/V-KING/future.git/info/refs  





V-KING是你的github用户名


解决方案：

vim .git/config

修改



**[plain]**[view plain](http://blog.csdn.net/happyteafriends/article/details/11554043#)[copy](http://blog.csdn.net/happyteafriends/article/details/11554043#)[print](http://blog.csdn.net/happyteafriends/article/details/11554043#)[?](http://blog.csdn.net/happyteafriends/article/details/11554043#)

- [remote "origin"]  
-     url = https://github.com/V-KING/example.git  


为：



**[plain]**[view plain](http://blog.csdn.net/happyteafriends/article/details/11554043#)[copy](http://blog.csdn.net/happyteafriends/article/details/11554043#)[print](http://blog.csdn.net/happyteafriends/article/details/11554043#)[?](http://blog.csdn.net/happyteafriends/article/details/11554043#)

- [remote "origin"]  
-     url = https://V-KING@github.com/wangz/example.git  


再次git push，弹出框输入密码，即可提交



