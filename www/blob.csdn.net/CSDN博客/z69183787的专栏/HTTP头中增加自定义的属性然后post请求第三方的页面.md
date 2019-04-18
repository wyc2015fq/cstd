# HTTP头中增加自定义的属性然后post请求第三方的页面 - z69183787的专栏 - CSDN博客
2014年05月09日 10:40:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：26033
场景是这样的，第三方页面访问需要鉴权，所以需要在HTTP头中增加鉴权的属性和内容发送给第三方网页。
我试过用response.addHeader("Authorization","xxxxxxxxx")方法，但发送的请求还是不带Authorization属性，还请大牛们不吝指教。
你这个问题实际上是服务器访问第三方鉴权系统，这个访问过程和客户端没关系的
和response,request都没关系
是要在服务器端模拟http请求，在这个请求里面增加自定义header
用HttpClient很容易
```java
HttpClient httpClient=new HttpClient(new HttpClientParams(),new SimpleHttpConnectionManager(true));
List<Header> headers;
headers=new ArrayList<Header>();
headers.add(new Header("someitem","someContent"));
List<Header> headers = new ArrayList<Header>();  
headers.add(new Header("Authorization", "OAuth2 " + "2.00Tw_PoBdBYEND600e0aa317Sm_qoC"));  
headers.add(new Header("API-RemoteIP", "http://10.1.41.252"));  
client.getHostConfiguration().getParams().setParameter("http.default-headers", headers); 
client.executeMethod(method);
```
这个第三方系统验证完你的header无误之后，肯定会在返回的session里有些标志
把这些标志再给你的客户端，这样你的客户端就可以访问这个第三方了
