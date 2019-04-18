# LLServer1.0及C#客户端正式发布并开源 - weixin_33985507的博客 - CSDN博客
2011年08月23日 13:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
    LLServer是本人基于libevent和leveldb这两个开源软件，开发的轻量级数据存储服务器软件，借助libevent高效网络接口实现对leveldb的访问封装。
    项目网址：http://code.google.com/p/llserver/
　 使用环境：Linux
　 作者：代震军
　 目前发布版本：1.0
　 其支持http协议和memcached协议。也就是可以通过浏览器或现有的memcached客户端来进行数据的CURD操作。 
    下面简单介绍一下如何安装使用LLServer。   
**LLServer 编译安装：**
**1.安装libevent2.0**
```
ulimit -SHn 65535
wget http://lnamp-web-server.googlecode.com/files/libevent-2.0.12-stable.tar.gz
tar zxvf libevent-2.0.12-stable.tar.gz
cd libevent-2.0.12-stable/
./configure --prefix=/usr
make && make install
cd ../
```
**2.通过svn:客户端下载leveldb**到本地leveldb文件夹,链接：http://leveldb.googlecode.com/svn/trunk/     
    或暂时用我这个打好包的地址下载：
wget http://llserver.googlecode.com/files/leveldb.tar.gz
        tar zxvf leveldb.tar.gz
    之后编译安装   
```
cd leveldb/    
make -f Makefile   
cp libleveldb.a /usr/local/lib/
cp -rf include/* /usr/local/include/
cd ../
```
**3.LLServer下载地址****：**
```
wget http://llserver.googlecode.com/files/llserver-1.0.tar.gz
tar zxvf llserver-1.0.tar.gz
cd llserver/
make -f Makefile
make install
cd ../
```
**4、LLServer 使用文档：**
  　[root@~]# llserver -h
    -l <ip_addr> 监听的IP地址，默认值为 0.0.0.0
    -p <num> 监听的TCP端口（默认值：11211）
    -x <path> 数据库目录，目录不存在会自动创建（例如：/llserver/data）
    -c 数据缓存队列单位，默认为100m
    -t <second> HTTP请求的超时时间
    -s 1:http协议  other:memcached协议
    -d 以守护进程运行
    -h 显示帮助
![](https://images.cnblogs.com/cnblogs_com/daizhj/llserver_6.png)
**使用示例：**
```
llserver -l 127.0.0.1 -p 11211  -d -s 1 -x /llserver/db0
```
　 HTTP GET 协议（以curl命令为例）：
```
curl "http://127.0.0.1:11211/?opt=set&charset=utf-8&key=username&value=daizhj"
curl "http://127.0.0.1:11211/?opt=set&charset=utf-8&key=username&value=daizhj&exptime=(unix时间戳，设置有效期)"
```
   HTTP POST 协议（以curl命令为例）：
```
curl -d "daizhj" "http://127.0.0.1:11211/?opt=set&charset=utf-8&key=username"
```
   打开浏览器：   
   如果set成功，返回：LLServer_SET_OK
![](https://images.cnblogs.com/cnblogs_com/daizhj/llserver_1.png)
 　如果set失败，返回：LLServer_SET_ERROR
　 获取数据：
 　HTTP GET 协议（以curl命令为例）：
   curl "http://127.0.0.1:11211/?opt=get&charset=utf-8&key=username"
   打开浏览器：
   如果get成功，返回相应数据信息
![](https://images.cnblogs.com/cnblogs_com/daizhj/llserver_2.png)
 　如果get失败，返回：LLServer_GETKEY_ERROR
![](https://images.cnblogs.com/cnblogs_com/daizhj/llserver_3.png)
 　删除数据：
 　HTTP GET 协议（以curl命令为例）：
   curl "http://127.0.0.1:11211/?opt=delete&key=username"
   打开浏览器：
   如果delete成功，返回：LLServer_DELETE_OK
![](https://images.cnblogs.com/cnblogs_com/daizhj/llserver_4.png)
 　如果delete失败，返回：LLServer_DELETE_ERROR
 　删除全部数据：
 　HTTP GET 协议（以curl命令为例）：
   curl "http://127.0.0.1:11211/?opt=deleteall&key=username"
   打开浏览器：
    如果delete成功，返回：LLServer_DELETEALL_OK
 　如果delete失败，返回：LLServer_DELETEALL_ERROR
**使用telnet链接示例(基于memcached协议)：**
```
llserver -l 10.0.4.188 -p 11211  -d -s 2 -x /llserver/db1
```
   set操作：
     client =>  set username 0 0 6
                daizhj     
     llserver=> STORED
   get操作：
     client=>   get username
     llserver=> daizhj
   delete操作：
     client=>   delete username
     llserver=> DELETED
   quit：
     client=>   quit
     llserver=> close client_fd
   version：
     client=>  version
     llserver=>1.0
![](https://images.cnblogs.com/cnblogs_com/daizhj/llserver_5.png)
**客户端 C#源码包：**
   1.这里将Discuz!NT使用的memcached 客户端（已修改过部分代码），大家可以使用它（支持链接池）来直接访问LLServer,相应的示例在压缩包的MemcachedApp\MemcachedApp\sample\sample_1.aspx中。
     下载地址：[http://files.cnblogs.com/daizhj/MemcachedApp_llserver.rar](http://files.cnblogs.com/daizhj/MemcachedApp_llserver.rar)
   2.当然也可以使用c#的httpwebrequest来直接访问，比如（代码源自discuz!nt）：
```
namespace Discuz.Common
{
    /// <summary>
    /// http POST请求url
    /// </summary>
    /// <param name="url">请求的url链接</param>
    /// <param name="method">方法名称，如"POST", "GET"</param>
    /// <param name="postData">方法名称为"POST"时，所有提交的数据</param>
    /// <returns></returns>
   public static string GetHttpWebResponse(string url, string method, string postData)
   {
       HttpWebRequest request = (HttpWebRequest)HttpWebRequest.Create(url);
       request.Method = method;
       request.ContentType = "application/x-www-form-urlencoded";
       request.ContentLength = string.IsNullOrEmpty(postData) ? 0 : postData.Length;
       //request.UserAgent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; .NET CLR 1.0.3705;)";
       request.Timeout = 60000;
       HttpWebResponse response = null;
       try
       {
           if (!string.IsNullOrEmpty(postData))
           {
               StreamWriter swRequestWriter = new StreamWriter(request.GetRequestStream());
               swRequestWriter.Write(postData);
               if (swRequestWriter != null)
                   swRequestWriter.Close();
           }
           response = (HttpWebResponse)request.GetResponse();
           using (StreamReader reader = new StreamReader(response.GetResponseStream(), Encoding.UTF8))
           {
               return reader.ReadToEnd();
           }
       }
       catch
       {
           return null;
       }
       finally
       {
           if (response != null)
               response.Close();
       }
   }
 
   //这里发送请求并返回数据      
   string result = GetHttpWebResponse("http://127.0.0.1:11211/?opt=get&charset=utf-8&key=username", "GET", null);
```
  　当然你可也以用其它语言的memcached客户端来访问LLServer,不过有一点要说明的是，目前llserver只存储序列化的字符串对象，如果你的数据为泛型或数组等类型，请先序列化之后再进行存储。 　
  　还有就是在c#里可以对数据进行二进制序列化，这里必须将序列化后的对象进行base64位编码之后再存储，否则会报异常，切记。 　
 　http://blog.nosqlfan.com/html/2819.html  LevelDB、TreeDB、SQLite3性能对比测试
 　http://blog.nosqlfan.com/html/2882.html  LevelDB内部实现
 　好了，大家下载体验一下吧，操作数据速度飞快呀。   
   原文链接:http://www.cnblogs.com/daizhj/archive/2011/08/23/2150422.html 
   作者: daizhj, 代震军   
   微博: http://weibo.com/daizhj
   Tags: libevent,leveldb,llserver,key/value db
