# jabberNet 修改花名册条目的昵称 - 左直拳的马桶_日用桶 - CSDN博客
2014年06月18日 14:57:28[左直拳](https://me.csdn.net/leftfist)阅读数：1916
个人分类：[.NET																[IM](https://blog.csdn.net/leftfist/article/category/2457179)](https://blog.csdn.net/leftfist/article/category/94497)
修改昵称，这么简单的功能，在jabberNet里怎么实现？
翻遍了jabberNet里的代码，jabber.client.RosterManager也，JabberClient也，似乎都没有现成的方法。
经过多次试验，发现可以这样写：
```
public void SetNickname(JID jid,string nickname)
        {
            string strSql = String.Format(@"
<iq id='changenick1' type='set'>
    <query xmlns='jabber:iq:roster'>
        <item jid='{0}' name='{1}' />
    </query>
</iq>",jid.ToString(),nickname);
           jc.Write(strSql);
        }
```
这很类似执行SQL语句。
至于这种“SQL语句”（XML)为什么这样写，要扯到XMPP身上。我还没来得及对XMPP进行了解。XMPP的一个参考网址如下：
[http://wiki.jabbercn.org/RFC6121](http://wiki.jabbercn.org/RFC6121)
