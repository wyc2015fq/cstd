# C#的get set 可访问性 - 左直拳的马桶_日用桶 - CSDN博客
2011年07月19日 00:16:01[左直拳](https://me.csdn.net/leftfist)阅读数：994标签：[c#																[url																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=url&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
public string Url
    {
        get
        {
            return url;
        }
private set
        {
            url = value;
        }
    }
在类内部，可读可写；外部，只读。
