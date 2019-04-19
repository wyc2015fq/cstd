# WebService路径在部署阶段改变问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年09月11日 17:54:37[左直拳](https://me.csdn.net/leftfist)阅读数：661
.NET中使用WebService，途径我知道的有二：
一、代理模式 
代理模式就是首先在项目里对这个WebService进行引用，生成一个代理，然后操作这个代理，达到调用WebService的目的。
但是，在开发阶段，因为引用WebService，创建代理之时，这个WebService是有个路径的，那么真正部署之后，路径跟开发阶段不同怎么办？
这样处理：
```
string servicepath = ConfigurationManager.AppSettings["ServicePath"];//从配置文件中读取WebService路径
Binding httpBinding = new BasicHttpBinding();
EndpointAddress httpAddress = new EndpointAddress(servicepath);
SmsServiceClient proxy = new SmsServiceClient(httpBinding, httpAddress);
```
二、直接访问 
直接访问服务地址
```
using (WebClient webClient = new WebClient())
{
    webClient.Encoding = Encoding.GetEncoding("utf-8");
    string url = ConfigurationManager.AppSettings["ServicePath"];//从配置文件中读取WebService路径;
    string re = webClient.DownloadString(url);
}
```
