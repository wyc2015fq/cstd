# [CareerCup] 10.1 Client-facing Service 面向客户服务器 - Grandyang - 博客园







# [[CareerCup] 10.1 Client-facing Service 面向客户服务器](https://www.cnblogs.com/grandyang/p/4851869.html)







10.1 Imagine you are building some sort of service that will be called by up to 1000 client applications to get simple end-of-day stock price information (open, close, high, low). You may assume that you already have the data, and you can store it in any format you wish. How would you design the client-facing service which provides the information to client applications? You are responsible for the development, rollout, and ongoing monitoring and maintenance of the feed. Describe the different methods you considered and why you would recommend your approach. Your service can use any technologies you wish, and can distribute the information to the client applications in any mechanism you choose.



这道题是一道设计题，说是有一些数据信息要给1000个客户端应用访问，问我们的用什么样的面向客户服务器来实现所有的功能。根据树中描述，我们的服务器需要易于用户使用，也要易于自己使用，可以易于未来需要的变更修改，高效和可扩展性好，那么主要有以下三种实现方法：

1. 使用txt文件，这是最简单的方法，用户在FTP服务器上下载这个文本文件，这可能一定程度上方便了维护，因为文本文件易于浏览和备份，但是访问解析起来很麻烦，尤其是添加了新数据之后。

2. 使用SQL数据库，让客户直接对数据库操作。好处是我们可以利用数据库强大的检索功能找出我们想要的一些条件搜索结果，数据库可以回朔，且备份方便，便于客户集成现有的应用程序。坏处是可能过载了，我们需要整个SQL数据库的东西来维护，还需要实现额外层来浏览和维护数据，尽管数据库很安全，但是我们不能让用户访问一些他们不应接触的数据。

3. 使用XML，如果我们的数据里固定的格式和固定的大小，例如company_name, open. high, low, losing price. 那么XML可以写出如下这样：

```
<root>
    <date value="2008-10-12">
        <company name="foo">
            <open>126.23</open>
            <high>130.27</hight>
            <low>122.83</low>
            <closingPrice>127.30</closingPrice>
        </company>
        <company name="bar">
            <open>52.73</open>
            <high>60.27</high>
            <low>50.29</low>
            <closingPrice>54.91</closingPrice>
        </company>
    </date>       
    <date value="2008-10-12">
    ...
    </date>
</root>
```

使用XML的优点是便于发布，便于被机器和人读取，大多数程序语言都有XML的解析，便于增加数据，不会影响到解析，可使用现有工具进行备份。缺点是会给客户所有的数据，如果客户只要部分数据，这种方法就不高效，而且对数据的查询需要解析整个文件。

不管我们用上述三种方法的哪种，我们可以提供一个网络服务器让客户接触到我们允许其访问的数据，这样用更便于控制。具体用何方法还是要权衡利弊，根据实际情况来做判断。














