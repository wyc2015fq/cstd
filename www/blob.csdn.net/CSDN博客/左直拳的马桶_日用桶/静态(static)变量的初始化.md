# 静态(static)变量的初始化 - 左直拳的马桶_日用桶 - CSDN博客
2013年02月22日 15:31:54[左直拳](https://me.csdn.net/leftfist)阅读数：1446
公司为了记录我们都上了什么网，将网络改成需要使用代理才能上网。具体表现就是上网前要输入账号和密码。
这样我们那些爬虫代码也要跟着改写了。
```
Uri target = new Uri(sUrl);
HttpWebRequest resquest = (HttpWebRequest)WebRequest.Create(target);
ProxySetting(resquest);
……
//设置代理
private static void ProxySetting(WebRequest request)
{
    WebProxy proxy = WebProxy.GetDefaultProxy();//获取IE缺省设置
    if (proxy.Address != null)//如果地址为空，则不需要代理服务器
    {
        try
        {
            string dm = System.Configuration.ConfigurationManager.AppSettings["strDomain"];
            string name = System.Configuration.ConfigurationManager.AppSettings["strUserName"];
            string pwd = System.Configuration.ConfigurationManager.AppSettings["strPassWord"];
            proxy.Credentials = new NetworkCredential( name,pwd,dm );
            request.Proxy = proxy;
        }
        catch (Exception e)
        {
            throw e;
        }
    }
}
```
这样出来一个问题，就是每次抓取都要判断、读取、设置一下这个代理，对于爬虫这种多线程的程序来说，是很大的浪费，造成抓取时间大大延长。
能不能一次性设置好这个代理，然后每次抓取的时候，直接赋给这个WebRequest.Proxy呢？又或者有些网络环境根本不需要代理，那么就完全可以不去设置这个代理？
最简单的做法就是在调用方，先配置好这个代理，然后将这个proxy作为参数传进来（没有代理的环境，proxy == null），直接赋给WebRequest.Proxy。但是这样的话，函数的参数就变了，会引起很多使用这个函数的地方都要修改，成本太高了。并且将设置的工作丢给调用方，显得很不智能，万一调用这里不知道要设置这个代理呢？
还有个问题，我们抓取函数是静态函数，这意味着我们不能构造一个实例，在里面先初始化好一个Proxy成员变量，然后给抓取函数调用。
这也不行，那也不行，愁煞寡人。
十月革命一声炮响，后来想到静态变量。将proxy作为静态变量，估计仅需初始化一次（就是第一次调用之时），以后在进程生命周期内，都无须再初始化了，可谓一次初始，无限调用。
```
//静态变量
static WebProxy _proxy = null;
//静态构造函数
static CSocket()
{
    WebProxy proxy = WebProxy.GetDefaultProxy();//获取IE缺省设置
    if (proxy.Address != null)
    {
        try
        {
            string dm = System.Configuration.ConfigurationManager.AppSettings["strDomain"];
            string name = System.Configuration.ConfigurationManager.AppSettings["strUserName"];
            string pwd = System.Configuration.ConfigurationManager.AppSettings["strPassWord"];
            proxy.Credentials = new NetworkCredential(name, pwd, dm);//从配置封装参数中创建
            _proxy = proxy;
        }
        catch (Exception e)
        {
        }
    }
}
//抓取函数
Uri target = new Uri(sUrl);
HttpWebRequest resquest = (HttpWebRequest)WebRequest.Create(target);
resquest.Proxy = _proxy;
```
