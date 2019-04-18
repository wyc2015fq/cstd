# sqlalchemy update sql - weixin_33985507的博客 - CSDN博客
2010年10月19日 08:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
![](http://old.nabble.com/images/icon_tri.png)
## [CPyUG] Re: SQLAlchemy对多条数据批量更新的问题
![Click to flag this post](http://old.nabble.com/images/icon_unflagged.png)![](http://old.nabble.com/images/icon_star_blue.png)![](http://old.nabble.com/images/icon_star_blue.png)![](http://old.nabble.com/images/icon_star_blue.png)
by [Gaicitadie](http://old.nabble.com/user/UserProfile.jtp?user=1716088)Apr 13, 2010; 02:27pm :: Rate this Message: ![](http://old.nabble.com/images/icon_blocked_gray.png)![](http://old.nabble.com/images/icon_star_grey.png)![](http://old.nabble.com/images/icon_star_grey.png)![](http://old.nabble.com/images/icon_star_grey.png)![](http://old.nabble.com/images/icon_star_grey.png)![](http://old.nabble.com/images/icon_star_grey.png)![](http://old.nabble.com/images/icon_clear_rating.png)- Use ratings to moderate ([?](http://old.nabble.com/help/Answer.jtp?id=16))
[Reply](http://old.nabble.com/forum/Reply.jtp?post=28226352) | [Print](http://old.nabble.com/forum/PrintPost.jtp?post=28226352) | View Threaded | [Show Only this Message](http://old.nabble.com/-CPyUG--Re%3A-SQLAlchemy%E5%AF%B9%E5%A4%9A%E6%9D%A1%E6%95%B0%E6%8D%AE%E6%89%B9%E9%87%8F%E6%9B%B4%E6%96%B0%E7%9A%84%E9%97%AE%E9%A2%98-p28226352.html)
谢谢，用**update**方法解决了： 
    session.query(News).filter(News.tid==0).**update**({News.tid:100}) 
    session.commit() 
On 4月13日, 上午9时34分, 诚子 <zhicheng1...@...> wrote: 
> [http://www.**sqlalchemy**.org/docs/reference/orm/query.html](http://www.sqlalchemy.org/docs/reference/orm/query.html)
> 用**update**方法的笑而不语 
> 
-- 
来自: `python-cn`:CPyUG ~ 华蟒用户组 | 发言:python-cn@...
退订: [http://tinyurl.com/45a9tb](http://tinyurl.com/45a9tb) //针对163/qq邮箱:[http://tinyurl.com/4dg6hc](http://tinyurl.com/4dg6hc)
详情: [https://groups.google.com/group/python-cn](https://groups.google.com/group/python-cn)
严正: 理解列表! 智慧提问! [http://wiki.woodpecker.org.cn/moin/AskForHelp](http://wiki.woodpecker.org.cn/moin/AskForHelp)
To unsubscribe, reply using "remove me" as the subject. 
