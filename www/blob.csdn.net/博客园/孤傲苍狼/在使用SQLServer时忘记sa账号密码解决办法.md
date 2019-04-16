# 在使用SQLServer时忘记sa账号密码解决办法 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [在使用SQLServer时忘记sa账号密码解决办法](https://www.cnblogs.com/xdp-gacl/p/3490119.html)



先以windows 身份验证方式登录SQLServer数据库，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/25101909-f56fe071dd6b4e449bf4b1da0ba24f2d.png)

打开查询分析器，运行如下代码：

**sp_password Null,'****新密码','sa'**

即可把原来的密码修改成新密码

例如：把sa用户的登录密码修改123456

```
1 sp_password Null,'123456','sa'
```

![](https://images0.cnblogs.com/blog/289233/201312/25102303-4ac09a46fc3f40dd806e10d52518f943.png)

这样就可以把原来的密码设置为空，sa用户的新密码就是123456了















