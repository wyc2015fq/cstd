# C#抓取需要登录的页面 - 左直拳的马桶_日用桶 - CSDN博客
2012年02月05日 23:27:38[左直拳](https://me.csdn.net/leftfist)阅读数：5774
抓取需要登录的页面的内容，原理主要是：**先模拟登录，获取到COOKIE，然后接下来的访问，都使用这个COOKIE，就可以访问到需要登录的页面。**
理论上，浏览器可以做到的事情，程序应该也可以。
不过，模拟登录，说起来容易，但不同站点有不同的处理方式，复杂程度不同。
1、最简单的，是POST适当的数据，不用验证码
2、象Discuz!系列的，要先访问某一个页面，获得随机码，然后置于POST数据中，才可以登录
3、需要验证码。验证码识别是另外一个课题。
无论如何，POST数据必须的。那么，模拟登录，需要POST哪些数据呢？
其实，需要POST哪些数据，每个站点都有所不同，所以要有一个合适的工具来进行分析。我装的是firefox的扩展控件：**HttpFox**。利用它，可以很方便的获取登录指定站点时，所需要提交的数据串。
模拟登录的代码是这样的：
```
//sPostData，待提交的数据串，如http://www.test.com/login.aspx?user=admin&pwd=123456
        public static CookieContainer Login(string url, string sPostData, CookieContainer cc)
        {
            CookieContainer container = (cc == null) ? new CookieContainer() : cc;
            ASCIIEncoding encoding = new ASCIIEncoding();
            byte[] data = encoding.GetBytes(sPostData);
            HttpWebRequest resquest = ResquestInit(url);
            resquest.Method = "POST";
            resquest.ContentLength = data.Length;
            resquest.CookieContainer = container;
            Stream newStream = resquest.GetRequestStream();
            newStream.Write(data, 0, data.Length);
            newStream.Close();
            try
            {
                HttpWebResponse response = (HttpWebResponse)resquest.GetResponse();
                response.Cookies = container.GetCookies(resquest.RequestUri);
            }
            catch{}
            return container;
        }
//这个函数的作用就是统一Request的格式，使得每次访问目标网站都用相同的口径。如果参数不同的话，可能造成COOKIE无效，因而登录无效
 public static HttpWebRequest ResquestInit(string url)
        {
            Uri target = new Uri(url);
            HttpWebRequest resquest = (HttpWebRequest)WebRequest.Create(target);
            resquest.UserAgent = "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.2) Gecko/20100316 Firefox/3.6.2 (.NET CLR 3.5.30729)";
            resquest.Accept = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8";
            resquest.AllowAutoRedirect = true;
            resquest.KeepAlive = true;
            resquest.ReadWriteTimeout = 120000;
            resquest.ContentType = "application/x-www-form-urlencoded";
            resquest.Referer = url;
            return resquest;
        }
```
获得这个CookieContainer后，保存下来，以后每访问该网站，都带上它。CookieContainer相当于浏览器的COOKIE容器，里面存放访问各个网站的COOKIE。
带COOKIE访问代码如下：
```
static HttpWebResponse GetResponse(string url, CookieContainer cc)
        {
            try
            {
                CookieContainer container = (cc == null) ? new CookieContainer() : cc;
                HttpWebRequest resquest = ResquestInit(url);
                resquest.CookieContainer = container;
                HttpWebResponse response = (HttpWebResponse)resquest.GetResponse();
                response.Cookies = container.GetCookies(resquest.RequestUri);
                return response;
            }
            catch
            {
                return null;
            }
        }
```
其中参数 **CookieContainer cc** 就是保存的CookieContainer。

