# 模拟登陆CSDN——就是这么简单 - z69183787的专栏 - CSDN博客
2017年04月26日 14:16:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：608
## 工具介绍
本篇文章主要是讲解如何模拟登陆CSDN，使用的工具是HttpClient+Jsoup
其中HttpClient主要是负责发送请求，而Jsoup主要是解析HTML
你可能对HttpClient的API不太了解，不过没关系，往下看就好了~
Jsoup的语法类似[jQuery](http://lib.csdn.net/base/jquery)的选择器，相信有一定web基础的人都可以很快的掌握
其中select(String selector)就是最强大的选择器，另外还提供一系列的细化的方法，比如：
getElementById(String id), getElementsByClass(String class), getElementsByTag(String tagName)
是不是很亲切？对~这个就跟[JavaScript](http://lib.csdn.net/base/javascript)的方法类似了~
所以Jsoup对于开发WEB的朋友的学习成本是相当的低的！那么，继续吧骚年！
## 步骤分析
**第一步、**首先需要拿到模拟登陆的请求地址，在CSDN登陆页就可以找到：https://passport.csdn.net/account/login，不错，第一步已经成功
**第二步、**抓包得到post请求需要发送的参数，可以用FF或chrome来抓，如下图所示：
![](https://img-blog.csdn.net/20140823224750578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjM0NTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**第三步、**其中username和password是由我们填的，那么后面三个参数呢？不急，看看登陆页面的源代码
![](https://img-blog.csdn.net/20140823225055067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjM0NTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
原来在这儿呢！到这里一切都异常的顺利~
整理一下思路，不要被顺利冲昏了头脑~
1、首先我们需要发送一个get请求来得到登陆页面，并从登陆页面上得到三个请求参数
2、用从1中得到的请求参数和账号密码模拟发送post请求到登陆请求地址
3、最后分析post返回的结果判断登陆是否成功
有了思路之后，我们还需要借助编程来实现它，这里需要一个工具——HttpClient
## 如何简单快速使用HttpClient
可能你对HttpClient的API不熟悉，那么如何在项目中快速使用HttpClient呢？
这里已经封装了两个最常用的get和post请求方法，所以之前就让你别担心啦~^_^
如果不想花时间看API的话直接拿去用就可以了
**[java]**[view
 plain](http://blog.csdn.net/u012345283/article/details/38788659#)[copy](http://blog.csdn.net/u012345283/article/details/38788659#)
- /**
-  * Http工具类
-  * 
-  * @author Zhu
-  * 
-  */
- publicclass HttpUtils {  
- 
- privatestatic CloseableHttpClient httpClient = HttpClients.createDefault();  
- privatestatic HttpClientContext context = new HttpClientContext();  
- 
- private HttpUtils() {  
- 
-     }  
- 
- publicstatic String sendGet(String url) {  
-         CloseableHttpResponse response = null;  
-         String content = null;  
- try {  
-             HttpGet get = new HttpGet(url);  
-             response = httpClient.execute(get, context);  
-             HttpEntity entity = response.getEntity();  
-             content = EntityUtils.toString(entity);  
-             EntityUtils.consume(entity);  
- return content;  
-         } catch (Exception e) {  
-             e.printStackTrace();  
- if (response != null) {  
- try {  
-                     response.close();  
-                 } catch (IOException e1) {  
-                     e1.printStackTrace();  
-                 }  
-             }  
-         }  
- return content;  
-     }  
- 
- publicstatic String sendPost(String url, List<NameValuePair> nvps) {  
-         CloseableHttpResponse response = null;  
-         String content = null;  
- try {  
- //　HttpClient中的post请求包装类
-             HttpPost post = new HttpPost(url);  
- // nvps是包装请求参数的list
- if (nvps != null) {  
-                 post.setEntity(new UrlEncodedFormEntity(nvps, "UTF-8"));  
-             }  
- // 执行请求用execute方法，content用来帮我们附带上额外信息
-             response = httpClient.execute(post, context);  
- // 得到相应实体、包括响应头以及相应内容
-             HttpEntity entity = response.getEntity();  
- // 得到response的内容
-             content = EntityUtils.toString(entity);  
- //　关闭输入流
-             EntityUtils.consume(entity);  
- return content;  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         } finally {  
- if (response != null) {  
- try {  
-                     response.close();  
-                 } catch (IOException e) {  
-                     e.printStackTrace();  
-                 }  
-             }  
-         }  
- return content;  
-     }  
- }  
现在get和post对你来说都已经轻而易举了，那么开始模拟登陆吧~
## 模拟登陆实战
按照我们先前的思路来前进吧！
1、首先我们需要发送一个get请求来得到登陆页面，并从登陆页面上得到三个请求参数
**[java]**[view
 plain](http://blog.csdn.net/u012345283/article/details/38788659#)[copy](http://blog.csdn.net/u012345283/article/details/38788659#)
- /**
-  * 获取必要的登陆参数信息
-  * 
-  * @throws IOException
-  */
- privatevoid fetchNecessaryParam() throws IOException {  
- // 查看CSDN登陆页面源码发现登陆时需要post5个参数
- // name、password，另外三个在页面的隐藏域中，a good start
-     logger.info("获取必要的登陆信息。。。。。");  
- // 这样登陆不行，因为第一次需要访问需要拿到上下文context
- // Document doc = Jsoup.connect(LOGIN_URL).get();
-     String html = HttpUtils.sendGet(LOGIN_URL);  
-     Document doc = Jsoup.parse(html);  
-     Element form = doc.select(".user-pass").get(0);  
-     lt = form.select("input[name=lt]").get(0).val();  
-     execution = form.select("input[name=execution]").get(0).val();  
-     _eventId = form.select("input[name=_eventId]").get(0).val();  
-     logger.info("获取成功。。。。。");  
- }  
2、用从1中得到的请求参数和账号密码模拟发送post请求到登陆请求地址
3、最后分析post返回的结果判断登陆是否成功
**[java]**[view
 plain](http://blog.csdn.net/u012345283/article/details/38788659#)[copy](http://blog.csdn.net/u012345283/article/details/38788659#)
- privateboolean mockLogin() {  
-     logger.info("开始登陆。。。。。");  
- boolean result = false;  
-     List<NameValuePair> nvps = new ArrayList<NameValuePair>();  
-     nvps.add(new BasicNameValuePair("username", username));  
-     nvps.add(new BasicNameValuePair("password", password));  
-     nvps.add(new BasicNameValuePair("lt", lt));  
-     nvps.add(new BasicNameValuePair("execution", execution));  
-     nvps.add(new BasicNameValuePair("_eventId", _eventId));  
-     String ret = HttpUtils.sendPost(LOGIN_URL, nvps);  
- if (ret.indexOf("redirect_back") > -1) {  
-         logger.info("登陆成功。。。。。");  
-         result = true;  
-     } elseif (ret.indexOf("登录太频繁") > -1) {  
-         logger.info("登录太频繁，请稍后再试。。。。。");  
-     } else {  
-         logger.info("登陆失败。。。。。");  
-     }  
- return result;  
- }  
## 题外话：
模拟登陆之后你就可以随心所欲的操作了~可以写个直接发blog的小程序或者是刷访问量之类的~
不过访问的太频繁可能会被封IP之类的~~~~
