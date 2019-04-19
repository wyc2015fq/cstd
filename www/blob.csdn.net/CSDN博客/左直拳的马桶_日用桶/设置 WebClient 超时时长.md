# 设置 WebClient 超时时长 - 左直拳的马桶_日用桶 - CSDN博客
2018年08月01日 19:33:22[左直拳](https://me.csdn.net/leftfist)阅读数：1657
.NET里面，WebClient并没有什么属性或方法可以设置超时时长，即TimeOut。这个超时时长很重要，默认似乎是120秒，2分钟，太长了。如果网路请求很多，每个都等2分钟，似乎就会造成堵塞。最好设短一点。
这个WebClient怎么设呢？从网络上的文章套路来看，都是重载这个WebClient，然后用重载过的，就不再用WebClient了。
上代码：
```
public class WebClientPro : WebClient
{
    /// <summary>
    /// 过期时间
    /// </summary>
    public int Timeout { get; set; }
    public WebClientPro(int timeout = 30000)
    {//默认30秒
        Timeout = timeout;
    }
    /// <summary>
    /// 重写GetWebRequest,添加WebRequest对象超时时间
    /// </summary>
    /// <param name="address"></param>
    /// <returns></returns>
    protected override WebRequest GetWebRequest(Uri address)
    {//WebClient里上传下载的方法很多，但最终应该都是调用了这个方法
        HttpWebRequest request = (HttpWebRequest)base.GetWebRequest(address);
        request.Timeout = Timeout;
        request.ReadWriteTimeout = Timeout;
        return request;
    }
}
```
然后我们愉快地使用WebClientPro
```
using (WebClientPro client = new WebClientPro())
{
    client.Encoding = System.Text.Encoding.GetEncoding("GB2312");
    Uri uri = new Uri(url);
    return Encoding.UTF8.GetString(client.DownloadData(uri));
}
```
