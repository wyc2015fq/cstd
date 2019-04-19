# SQL SERVER之居然连计算机管理员都无法访问 - 左直拳的马桶_日用桶 - CSDN博客
2012年05月18日 16:53:10[左直拳](https://me.csdn.net/leftfist)阅读数：1730标签：[sql server																[数据库																[windows																[服务器																[配置管理](https://so.csdn.net/so/search/s.do?q=配置管理&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
本人对SQL SERVER一知半解，因为懒惰和资质，好多东西都未尝知道。（说这种话的人一般都很厉害，但遗憾的是，我例外）
前不久接触一个交易系统，系统是另外一家公司搞的，但服务器却是我在管理。我是服务器管理员。让我大吃一斤的是，服务器上的数据库，我居然无法登录、访问。
而我之前的经历都是，计算机管理员，可以随便用WINDOWS登录模式，无须输入密码，就直接以管理员的身份打开这个数据库。天啊，这次他们是怎么做到的？
后来我又装了一次数据库，发现大概是安装过程中设置的。就是在选择登录模式那个窗口中，让你选一个管理员。有个按钮是选当前用户，如果我象以前那样选了，当然就是administrator了。但因为有了疑问，所以这次我选了另外一个账户。果然安装好以后，我身为计算机管理员，可以启动、关闭数据库，但却无法登录、打开这个数据库。
装好以后，我想将计算机管理员加到数据库管理员名单里面，却好像办不到。
其实是这样的，先将计算机管理员添加到SQL SERVER的登录名单（WINDOWS登录），角色选sysadmin就可以鸟。
另外，出于安全考虑，我们通常将计算机管理员从默认的 administrator 重命名为别的名字。这是好事。不过我昨天发现了一个问题，就是安装SQL SERVER的时候，计算机管理员名称是 administrator；现在我觉醒了，就将名称改了。改了以后，打开SQL SERVER配置管理器，发现里面的各种服务就看不到了，仿佛是一个没有权限的WINDOWS账户打开一样。将管理员名字改回来，却又可以。
什么毛病？说明SQL SERVER的账户 和 WINDOWS的账户，是两个独立，之间没有直接关联的系统。

