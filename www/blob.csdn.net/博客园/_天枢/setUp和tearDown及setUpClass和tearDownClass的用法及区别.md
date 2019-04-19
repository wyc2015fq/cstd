# setUp和tearDown及setUpClass和tearDownClass的用法及区别 - _天枢 - 博客园
## [setUp和tearDown及setUpClass和tearDownClass的用法及区别](https://www.cnblogs.com/yhleng/p/8057336.html)
2017-12-18 14:12 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8057336)
① setup():每个测试函数运行前运行
② teardown():每个测试函数运行完后执行
③ setUpClass():必须使用@classmethod 装饰器,所有test运行前运行一次
④ tearDownClass():必须使用@classmethod装饰器,所有test运行完后运行一次
