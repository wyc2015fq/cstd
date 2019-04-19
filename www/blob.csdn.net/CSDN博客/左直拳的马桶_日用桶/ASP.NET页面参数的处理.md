# ASP.NET页面参数的处理 - 左直拳的马桶_日用桶 - CSDN博客
2012年10月24日 15:20:32[左直拳](https://me.csdn.net/leftfist)阅读数：1309
个人分类：[.NET																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94497)
ASP.NET页面参数的处理？谁不会啊!
不就是类似 Request.Querystring["Id"] 这样子吗？
可是，象以下这个句子这么简单明了，有时候都会有问题：
`int Id = (Request.QueryString["Id"] != null) ? Convert.ToInt32(Request.QueryString["Id"]) : 0;`
为什么呢？因为有时候由于各种原因，用户链接过来的地址，后面可能会带上一些奇怪的符号，比如#（浏览器编码后会变成%23）之类，这时上面那条语句就会报错了。
频频报错，会让人感到网站不可靠。所以有必要加以处理：
```
//有时候，有些访问地址参数后面附有各种古怪的字符，引起页面报错，去掉之，以改善体验
    protected int GetIntParam(string param)
    {
        return Convert.ToInt32(_GetClearParam(param));
    }
    protected Int64 GetInt64Param(string param)
    {
        return Convert.ToInt64(_GetClearParam(param));
    }
    string _GetClearParam(string param)
    {
        Regex r = new Regex(@"(?<id>\-?\d+)", RegexOptions.IgnoreCase);
        try
        {
            return r.Match(param).Result("${id}");
        }
        catch
        {
            throw new Exception("参数错误：" + param);
        }
    }
```

