# mongodb适用场景(弊与利) - 三少GG - CSDN博客
2015年12月23日 14:38:04[三少GG](https://me.csdn.net/scut1135)阅读数：1064
1.  不适合社交场景
http://www.sarahmei.com/blog/2013/11/11/why-you-should-never-use-mongodb/
2. reuse connection   
# [Deep Dive into Connection Pooling](http://blog.mongolab.com/2013/11/deep-dive-into-connection-pooling/)
by [chris](http://blog.mongolab.com/author/chris/)on2013/11/06in[general](http://blog.mongolab.com/category/general/)
[Tweet](http://twitter.com/share)
As your application grows in functionality and/or usage, managing resources becomes increasingly important. Failure to properly utilize connection pooling is one major “gotcha” that we’ve seen greatly impact MongoDB performance and trip up developers of all
 levels.
## Connection pools
Creating new authenticated connections to the database is expensive. So, instead of creating and destroying connections for each request to the database, you want to re-use existing connections as much as possible. This is where connection pooling comes in.
A Connection Pool is a cache of database connections maintained by your driver so that connections can be re-used when new connections to the database are required. When properly used, connection pools allow you to minimize the frequency and number of new connections
 to your database.
### Connection churn
Used improperly however, or not at all, your application will likely open and close new database connections too often, resulting in what we call “connection churn”. In a high-throughput application this can result in a constant flood of new connection requests
 to your database which will adversely affect the performance of your database and your application.
### Opening too many connections
Alternately, although less common, is the problem of creating too many MongoClient objects that are never closed.  In this case, instead of churn, you get a steady increase in the number of connections to your database such that you have tens of thousands of
 connections open when your application could almost certainly do with far fewer. Since each connection takes RAM, you may find yourself wasting a good portion of your memory on connections which will also adversely affect your application’s performance.
Although every application is different and the total number of connections to your database will greatly depend on how many client processes or application servers are connected, in our experience, any connection count greater than 1000 – 1500 connections
 should raise an eyebrow, and most of the time your application will require far fewer than that.
## MongoClient and connection pooling
Most MongoDB language drivers implement the MongoClient class which, if used properly, will handle connection pooling for you automatically.
The syntax differs per language, but often you do something like this to create a new connection-pool-enabled client to your database:
mongoClient = new MongoClient(URI, connectionOptions);
Here the mongoClient object holds your connection pool, and will give your app connections as needed. You should strive to create this object once as your application initializes and re-use this object throughout your application to talk to your database. The
 most common connection pooling problem we see results from applications that create a MongoClient object way too often, sometimes on each database request. If you do this you will not be using your connection pool as each MongoClient object maintains a separate
 pool that is not being reused by your application.
## Example with Node.js
