# [asp.net]cookie和session - TaigaComplex求职中 - 博客园







# [[asp.net]cookie和session](https://www.cnblogs.com/TaigaCon/archive/2012/08/15/2641003.html)





cookie是浏览器保存网页信息的一小块地方，用cookie，就能轻松实现浏览器的记住用户ID的功能，cookie是与域名相关的，只有同一域名下的网页才能读取该域名的cookie。

能记住网页或者用户的某些信息固然方便，不过对于某些既要求保存，而且安全要求高的信息，却不能保存在cookie中，以防止用户篡改数据，这时候就用到了session。

从功能上来说，cookie跟session是一致的，都是用于保存网页信息（少量信息），不过从媒介上来说是不同的，cookie是保存在用户主机上，session是保存在服务器上。

session，准确来说是存放到服务器的内存上。由于是存放到内存上的数据，当网站的访问量大的时候，服务器就可能发生内存不足的问题，因此session存放的信息量必须小，而且当某session处于不再活动状态的时候就要释放内存，这样的话就要求session中有超时的概念：如果超过某个时间段，服务器没收到该用户的请求，则判断该session为死亡状态，进而释放内存。



cookie的主要字段有两个，key和value，当要从cookie中取某个值的时候，通过指定key得到其value，如果不存在该key，那么当然会返回空。

session主要字段有三个，ID，key和value，因为session是存放到服务器上的，服务器又有许多用户访问，所以要为不同用户分配不同ID，分配方法各种各样，有GUID，或者网站的用户ID等。



以下为session的原理性代码

```
1 public class SessionMgr
 2 {
 3     private static IDictionary<string ,IDictionary<string ,object>> data=new Dictionary<string,IDictionary<string,object>>();
 4     //data为保存session的全局变量，共三个字段：第一个string为ID，第二个string为key，第三个object为value  
 5     //等号右边的式子分配内存，为Dictionary对象，这样的话就能通过ID找到（key，value），然后再通过key查找value
 6     //左边的式子得到IDictionary接口，借口中实现各种操作Dictionary的方法，接口的具体实现可以去了解一下com
 7 
 8 
 9     public IDictionary<string, object> GetSession(string sessionID)
10         //GetSession实现的是通过ID查找（key，value），再把该对象返回用于其它操作的方法
11     {
12         if (data.ContainsKey(sessionID))
13             //如果存在该ID则返回其（key，value）
14         {
15             return data["sessionID"];
16         }
17         else
18             //如果不存在该ID则用该ID新建一个（key，value），并返回
19         {
20             IDictionary<string,object> session=new Dictionary<string,object>();
21             data["sessionID"] = session;
22             return session;
23         }
24     }
25 }
```












