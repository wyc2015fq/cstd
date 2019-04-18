# Apache Shiro 退出 - z69183787的专栏 - CSDN博客
2016年07月11日 20:37:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：681
退出操作可以通过调用subject.logout()来释放你的登录信息，如：
currentUser.logout(); //removes all identifying information and invalidates their session too. 
当你调用logout，任何现有的Session 都将会失效，而且任何身份都将会失去关联（例如，在Web 应用程序中，RememberMe cookie 也将被删除）。在Subject 注销后，该Subject 的实例被再次认为是匿名的。
由于在Web 应用程序记住身份往往是依靠Cookies，然而Cookies 只能在Response 被committed 之前被删除，所以强烈建议在调用subject.logout()后立即将终端用户重定向到一个新的视图或页面。这样能够保证任何与安全相关的Cookies都能像预期的一样被删除。这是HTTP cookies 的功能限制，而不是Shiro的问题。
