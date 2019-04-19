# mgo使用指南-高级篇(长连接/重连) - 三少GG - CSDN博客
2016年01月16日 14:59:29[三少GG](https://me.csdn.net/scut1135)阅读数：5409
最佳使用方法：
[http://denis.papathanasiou.org/posts/2012.10.14.post.html](http://denis.papathanasiou.org/posts/2012.10.14.post.html)
可稍微参考： https://github.com/goinggo/beego-mgo/blob/a78190c1e061ffa5fcc0f222cd4b879b8c266f10/utilities/mongo/mongo.go
##################################
## Keeping an mgo (any other db engine) connection alive
I have yet to write a service that would keep a connection alive and handle reconnecting. What's the best practice for this in Go and mgo / other engines?
**In mgo, what I think is recommended (at least it seems to work for me) is that youuse mgo.Dial to connect to MongoDB on startup. Then you use Session.Copy on that original session for every time you need to concurrently use the DB (such as per incoming http request if you are writing a web service).Other than that, connection pooling and reconnection should be handled by mgo internally.**
**the correct way to use "database/sql" is rather that you open a DB once, and then concurrently execute queries on that db object, and those queries uses a connection pool within the db object. **
**#######################**
**https://github.com/go-mgo/mgo/issues/166   **
# "Closed Explicitly" error
Q:After running for a while, the service stopped working properly. Every mongo action, such as `Entries.find(...)` returns
 an err of: 
```
Closed
 Explicitly
```
.
What does that mean? Should I be refreshing the mongoDB connection periodically or should I make it close and re-establish connection with each request?
The app is performance oriented, so despite mongo connection being down, everything is still up and running as everything operates on in-memory or cluster cache. I don't want to do something stupid that delays processing.
A:
The code you show above is fine by itself, and that error could mean different things, but one good guess is that there was a fallover and pre-existent connections were closed for resync.
That said, this should be a one-off situation. The application should be immediately able to reconnect and restore its working state. The only thing mgo requires is for the error to be acknowledged by the app if it was in the middle of something so that incorrect
 behavior doesn't go unperceived.
There are two easy ways to organize the application to do that:
- 
If the application is organized after some sort of loop, call Refresh on the session at the end of each iteration of the loop. This makes it discard (or put back in the pool, if the
 connection is good) the connection it's holding, and pick a new one when necessary.
- 
If handling multiple independent requests concurrently use an independent session for each request by calling session.Copy on the master session whenever a new session is needed, and
 then call session.Close when done with it. This also pools connections properly, and means using multiple connections to the database when necessary.
Your case sounds like the second one.
A:
First of all try enabling debug mode in mgo in order to get some more info about what's happening.
I suppose the server is dropping the connection after some inactivity time. In any case, usually the approach is to do a mgo `Dial` and
 then `Copy` the
 connection at the beginning of every request handling(using a middleware).
#####
How to handle disconnections to mongodb server? (aka. autoconnect)
https://groups.google.com/forum/#!topic/mgo-users/XM0rc6p-V-8
It actually reconnects. What it doesn't do is to reset the session 
you're holding, because that might hide real issues in your 
application. 
There are two easy ways to get rid of the error: 
1) Call Refresh on the session, which makes it discard (or put back in 
the pool, if the connection is good) the connection it's holding, and 
pick a new one when necessary. 
2) Instead of using a single session, use many by calling session.Copy 
when you need a new session, and then call session.Close when you're 
done with it. This will also mean you're using multiple connections to 
the database, when necessary. 
###
MongoDB和Server是分开的两个服务器。但有个问题就是如果MongoDb因为某种原因断开连接（这种情况很明显不可避免），哪怕只断开有一秒钟，Server那边网站就会出现数据库无法连接的错误。
 不知道有没有哪位大神能有好点的解决方案？

