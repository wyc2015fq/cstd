# iPhone开发笔记[10/50]：调用initWithNibName方法时不能写上.xib - weixin_33985507的博客 - CSDN博客
2011年07月14日 09:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
在用代码初始化一个ViewController时，使用initWithNibName方法时会犯一个习惯性的小错误：
// 下面这行是错误写法!!!
MyViewController *myViewController = [[MyViewController alloc] initWithNibName:@"MyViewController.xib" bundle:nil];
问题相当简单，就是NibName是指不包含.xib的字符串，把.xib去掉就OK了
MyViewController *myViewController = [[MyViewController alloc] initWithNibName:@"MyViewController" bundle:nil];
一个小错误，又耽误5分钟！
