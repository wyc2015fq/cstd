# [LeetCode] Design TinyURL 设计精简URL地址 - Grandyang - 博客园







# [[LeetCode] Design TinyURL 设计精简URL地址](https://www.cnblogs.com/grandyang/p/7675140.html)







> Note: For the coding companion problem, please see: [Encode and Decode TinyURL](https://leetcode.com/problems/encode-and-decode-tinyurl/).

How would you design a URL shortening service that is similar to [TinyURL](https://en.wikipedia.org/wiki/TinyURL)?

Background:
TinyURL is a URL shortening service where you enter a URL such as `https://leetcode.com/problems/design-tinyurl` and it returns a short URL such as `http://tinyurl.com/4e9iAk`.

Requirements:
- For instance, "http://tinyurl.com/4e9iAk" is the tiny url for the page `"https://leetcode.com/problems/design-tinyurl"`. The identifier (the highlighted part) can be any string with 6 alphanumeric characters containing `0-9`, `a-z`, `A-Z`.
- Each shortened URL must be unique; that is, no two different URLs can be shortened to the same URL.



Note about Questions:
Below are just a small subset of questions to get you started. In real world, there could be many follow ups and questions possible and the discussion is open-ended (No one true or correct way to solve a problem). If you have more ideas or questions, please ask in Discuss and we may compile it here!

Questions:
- How many unique identifiers possible? Will you run out of unique URLs?
- Should the identifier be increment or not? Which is easier to design? Pros and cons?
- Mapping an identifier to an URL and its reversal - Does this problem ring a bell to you?
- How do you store the URLs? Does a simple flat file database work?
- What is the bottleneck of the system? Is it read-heavy or write-heavy?
- Estimate the maximum number of URLs a single machine can store.
- Estimate the maximum number of queries per second (QPS) for decoding a shortened URL in a single machine.
- How would you scale the service? For example, a viral link which is shared in social media could result in a peak QPS at a moment's notice.
- How could you handle redundancy? i,e, if a server is down, how could you ensure the service is still operational?
- Keep URLs forever or prune, pros/cons? How we do pruning? (Contributed by @alex_svetkin)
- What API would you provide to a third-party developer? (Contributed by @alex_svetkin)
- If you can enable caching, what would you cache and what's the expiry time? (Contributed by @Humandroid)



这道系统设计的题跟之前的算法还是不一样的，代码只是其中的一部分，估计大部分还是要跟面试官侃大山，博主也不太熟悉这类题目，还是照着[ztlevi大神的帖子](https://discuss.leetcode.com/topic/95853/a-complete-solution-for-tinyurl-leetcode-system-design)来写吧。



**S: Scenario 场景**

长URL和短URL的相互转换



**N: Need 需求**

- QPS (Queires Per Second) 每秒查询数

  - 日活用户：100M

  - 每日人均使用量：（写）long2short 0.1，（读） short2long 1

  - 每日请求量：写 10M，读 100M

  - QPS：一天共有86400秒，约100K。写 100， 读 1K

  - 峰值QPS：写 200， 读 2K

（千级的量可以用一个单SSD的MySQL机器来处理）

- Storage 存储

  - 每天10M个新映射（长URL到短URL）

  - 一个映射大约占100B的大小

  - 每天1GB，1TB大约能扛三年

  对于这种系统来说，存储不是问题。只有像Netflix那样的系统可能会有存储问题。通过SN分析，我们对系统有了一个大框架印象，这个系统可以使用单SSD机器来实现。



**A: API 接口**

只有一种类型的服务：URLService

- Core (Business Logic) Layer

- Class: URLService

- Interface:

- URLService.encode(string long_url)

- URLService.decode(stirng short_url)

- Web Layer

- REST API:

- GET: /{short_url}, return a http redirect response (301)

- POST: [goo.gl](https://goo.gl/) method - [google shorten URL](https://developers.google.com/url-shortener/v1/getting_started#actions)

Request Body: {url=longUrl} e.g. {"longUrl": "[http://www.google.com/](https://www.google.com/)"}
Return OK(200), short_url is included in the data



**K: Data Access 数据访问**

Step 1: Pick a storage structure 选择一个存储结构

- SQL VS NoSQL？

  - 需要支持事务Transactions吗？NoSQL不支持事务Transactions。

  - 需要Rich SQL Query吗？ NoSQL不支持SQL那么多的Query。

  - 需要高效开发吗？大多数的网络框架对SQL的支持性非常好，意味着系统不需要太多的代码。

  - 需要AUTO_INCREMENT ID吗？ NoSQL不支持这个，仅有一个全局卫衣的Object_id。

  - 需要高QPS吗？NoSQL有高性能。比如Memcached的QPS可达到百万级，MondoDB可达万级，MySQL只有千级。

  - 系统的可伸缩性Scalability有多高？SQL需要开发者写代码去伸缩Scale，而NoSQL自带该功能（Sharding，replica）。

- Answer 回答：

  - 不需要 -> NoSQL

  - 不需要 -> NoSQL

  - 无所谓，因为只有很少的代码 -> NoSQL

  - 算法需要AUTO_INCREMENT ID -> SQL

  - 写 200，读 2K，不高 -> SQL

  - 不高 -> SQL

- System Alogrithm 系统算法

  - Hash 函数

　  long_url => md5/sha1

　  - md5将一个字符串转为128位，通常用16个字节的十六进制来表示：

[http://site.douban.com/chuan](https://site.douban.com/chuan) -> c93a360dc7f3eb093ab6e304db516653

　  - sha1将字符串转为160位，通常用20个字节的十六进制来表示：

[http://site.douban.com/chuan](https://site.douban.com/chuan) -> dff85871a72c73c3eae09e39ffe97aea63047094

　 这两个算法使得哈希值是随机分布的，但是冲突Conflicts无法避免。任何哈希算法都无法避免冲突问题。

     - 优点：简单。我们用转换字符串的前6个字符

     - 缺点：冲突

　　  解决方法 1. 使用（long_url + timestamp）作为哈希函数的关键字Key。2. 当冲突时，重新生成哈希值（生成的值不同因为时间戳改变了）。

　　  总之，当urls的个数超过十亿个，可能会有大量的冲突使得系统不高效。

  - base62

　  将short_url用62 base标记。6位可以表示62^6 57 billion。

　  每个short_url表示一个十进制数，可以当作SQL数据库中的AUTO_INCREMENT ID。



```
class URLService {
public:
    URLService() {
        COUNTER = 1;
        elements = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }

    string longToShort(string url) {
        string short_url = base10ToBase62(COUNTER);
        long2short[url] = COUNTER;
        short2long[COUNTER] = url;
        ++COUNTER;
        return "http://tiny.url/" + short_url;
    }

    string shortToLong(string url) {
        string prefix = "http://tiny.url/";
        url = url.substr(prefix.size());
        int n = base62ToBase10(url);
        return short2long[n];
    }

    int base62ToBase10(string s) {
        int n = 0;
        for (int i = 0; i < s.size(); ++i) {
            n = n * 62 + convert(s[i]);
        }
        return n;
    }

    int convert(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        } else if (c >= 'a' && c <= 'z') {
            return c - 'a' + 10;
        } else if (c >= 'A' && c <= 'Z') {
            return c - 'A' + 36;
        }
        return -1;
    }

    string base10ToBase62(int n) {
        string str = "";
        while (n != 0) {
            str.insert(str.begin(), elements[n % 62]);
            n /= 62;
        }
        while (str.size() != 6) {
            str.insert(str.begin(), '0');
        }
        return str;
    }

private:
    unordered_map<string, int> long2short;
    unordered_map<int, string> short2long;
    int COUNTER;
    string elements;
};
```



Step 2: Database Schema 数据库概要

一个表（id, long_url）。id是主键，通过long_url排序。基本的系统架构为：

Browser <-> Web <-> Core <-> DB



**O: Optimize 优化**

如何提高响应速度？

- 在网络服务器和数据库之间提高响应速度

使用Memcached来提高响应速度。当获得long_url时，先在缓存中搜索。我们可以把90%的读请求放在缓存当中。

- 在网络服务器和用户浏览器之间提高响应速度

不同的地区使用不同的网络服务器和缓存服务器。所有的地区共享一个数据库用来匹配用户到最近的网络服务器（通过DNS），当他们不在缓存中的时候。

如果我们需要多于一台的MySQL机器？

- 问题：

  - 缓存用完了 

  - 越来越多的请求

  - 越来越多的缓存丢失

- 解决方案：

  - 垂直切分 Vertical Sharding

  - 水平切分 Horizontal Sharding

  最好的方式是水平切分。当前的表结构是（id, long_url），哪列可以当作切分关键字。

  一个简单的方法是id模块切分。

  现在有另一个问题：如何能使多个机器共享一个全局的AUTO_INCREMENT ID？

  两种方法：1. 多使用一个机器去维护id。2. 使用zookeeper。都很操蛋。

  所以，我们不适用AUTO_INCREMENT ID

  好处是将切分关键字当作short_url的第一个字节。

  另一种方法是用统一的哈希将循环断成62份。有多少份并没有啥关系，因为可能并没有62台机器（可能有360或其他的）。每台机器都是为循环的一部分的服务负责。

  write long_url -> hash(long_url)%62 -> put long_url to the specific machine according to hash value -> generate short_url on this machine -> return short_url

  short_url request -> get the sharding key (first byte of the short_url) -> search in the corresponding machine based on sharding key -> return long_url

  每当我们增加一台新机器，将最多使用的机器的一半范围放到心的机器中。



更多优化

将中文服务器放在中国，美国的服务器放在美国。使用地理信息当作切分关键字，例如，0是中国的网站，1是美国的网站。



参考资料：

[https://discuss.leetcode.com/topic/95853/a-complete-solution-for-tinyurl-leetcode-system-design](https://discuss.leetcode.com/topic/95853/a-complete-solution-for-tinyurl-leetcode-system-design)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












