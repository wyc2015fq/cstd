# 访问需要HTTP Basic Authentication认证的资源的各种语言的实现 - z69183787的专栏 - CSDN博客
2013年10月15日 20:19:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6897
无聊想调用下嘀咕的api的时候，发现需要HTTP Basic Authentication，就看了下。
什么是HTTP Basic Authentication？直接看[http://en.wikipedia.org/wiki/Basic_authentication_scheme](http://en.wikipedia.org/wiki/Basic_authentication_scheme) 吧。
在你访问一个需要HTTP Basic Authentication的URL的时候，如果你没有提供用户名和密码，服务器就会返回401，如果你直接在浏览器中打开，浏览器会提示你输入用户名 和密码(google浏览器不会，bug？)。你可以尝试点击这个url看看效果：[http://api.minicloud.com.cn/statuses/friends_timeline.xml](http://api.minicloud.com.cn/statuses/friends_timeline.xml)
要在发送请求的时候添加HTTP Basic Authentication认证信息到请求中，有两种方法：
- 一是在请求头中添加Authorization：
Authorization: "Basic 用户名和密码的base64 加密字符串"
- 二是在url中添加用户名和密码：
[http://userName:password@ api.minicloud.com.cn/statuses/friends_timeline.xml](http://api.minicloud.com.cn/statuses/friends_timeline.xml)
下面来看下对于第一种在请求中添加Authorization头部的各种语言的实现代码。
先看.NET的吧：
Java代码  ![收藏代码](http://wangleyiang.iteye.com/images/icon_star.png)
- string username="username";  
- string password="password";  
- //注意这里的格式哦，为 "username:password"
- string usernamePassword = username + ":" + password;  
- CredentialCache mycache = new CredentialCache();  
- mycache.Add(new Uri(url), "Basic", new NetworkCredential(username, password));  
- myReq.Credentials = mycache;  
- myReq.Headers.Add("Authorization", "Basic " + Convert.ToBase64String(new ASCIIEncoding().GetBytes(usernamePassword)));   
- 
- WebResponse wr = myReq.GetResponse();  
- Stream receiveStream = wr.GetResponseStream();  
- StreamReader reader = new StreamReader(receiveStream, Encoding.UTF8);  
- string content = reader.ReadToEnd();  
你当然也可以使用HttpWebRequest或者其他的类来发送请求。
然后是Python的：
Java代码  ![收藏代码](http://wangleyiang.iteye.com/images/icon_star.png)
- import urllib2  
- import sys  
- import re  
- import base64  
- from urlparse import urlparse   
- 
- theurl = 'http://api.minicloud.com.cn/statuses/friends_timeline.xml'
- 
- username = 'qleelulu'
- password = 'XXXXXX'  # 你信这是密码吗？   
- 
- base64string = base64.encodestring(  
- '%s:%s' % (username, password))[:-1] #注意哦，这里最后会自动添加一个\n  
- authheader =  "Basic %s" % base64string  
- req.add_header("Authorization", authheader)  
- try:  
-     handle = urllib2.urlopen(req)  
- except IOError, e:  
-     # here we shouldn't fail if the username/password is right  
-     print "It looks like the username or password is wrong."
-     sys.exit(1)  
- thepage = handle.read()  
 再来是PHP的：
Java代码  ![收藏代码](http://wangleyiang.iteye.com/images/icon_star.png)
- <?php  
- $fp = fsockopen("www.mydomain.com",80);  
- fputs($fp,"GET /downloads HTTP/1.0");  
- fputs($fp,"Host: www.mydomain.com");  
- fputs($fp,"Authorization: Basic " . base64_encode("user:pass") . "");  
- fpassthru($fp);  
- ?>  
 还有flash的AS3的：
Java代码  ![收藏代码](http://wangleyiang.iteye.com/images/icon_star.png)
- import mx.rpc.events.FaultEvent;  
- import mx.rpc.events.ResultEvent;  
- import mx.utils.Base64Encoder;  
- import mx.rpc.http.HTTPService;  
- URLRequestDefaults.authenticate = false;//设默认为false，否则用户较验错误时会弹出验证框 
- 
- private var result:XML;  
- private function initApp():void
- {  
-     var base64enc:Base64Encoder = new Base64Encoder;  
-     base64enc.encode("user:password"); //用户名和密码需要Base64编码
-     var user:String = base64enc.toString();   
- 
-     var http:HTTPService = new HTTPService;  
-     http.addEventListener(ResultEvent.RESULT,resultHandler);//监听返回事件
-     http.addEventListener(FaultEvent.FAULT,faultHandler);     //监听失败事件
-     http.resultFormat = "e4x";//返回格式
-     http.url = "http://api.digu.com/statuses/friends_timeline.xml"; 以嘀咕网的API为列  
-     http.headers = {"Authorization":"Basic " + user};  
-     http.send();  
- }  
- private function resultHandler(e:ResultEvent):void
- {  
-     result = XML(e.result);  
-     test.dataProvider = result.status;//绑定数据
- }  
- private function faultHandler(e:ResultEvent):void
- {  
- //处理失败
- }  
 还有Ruby On Rails的：
Java代码  ![收藏代码](http://wangleyiang.iteye.com/images/icon_star.png)
- class DocumentsController < ActionController  
-   before_filter :verify_access   
- 
-   def show  
- @document = @user.documents.find(params[:id])  
-   end   
- 
-   # Use basic authentication in my realm to get a user object.  
-   # Since this is a security filter - returnfalseif the user is not  
-   # authenticated.  
-   def verify_access  
-     authenticate_or_request_with_http_basic("Documents Realm") do |username, password|  
- @user = User.authenticate(username, password)  
-     end  
-   end  
- end  
 JavaScript的：
Java代码  ![收藏代码](http://wangleyiang.iteye.com/images/icon_star.png)
- //需要Base64见：http://www.webtoolkit.info/javascript-base64.html
- function make_base_auth(user, password) {  
-   var tok = user + ':' + pass;  
-   var hash = Base64.encode(tok);  
- return"Basic " + hash;  
- }   
- 
- var auth = make_basic_auth('QLeelulu','mypassword');  
- var url = 'http://example.com';   
- 
- // 原始JavaScript
- xml = new XMLHttpRequest();  
- xml.setRequestHeader('Authorization', auth);  
- xml.open('GET',url)   
- 
- // ExtJS
- Ext.Ajax.request({  
-     url : url,  
-     method : 'GET',  
-     headers : { Authorization : auth }  
- });   
- 
- // jQuery
- $.ajax({  
-     url : url,  
-     method : 'GET',  
-     beforeSend : function(req) {  
-         req.setRequestHeader('Authorization', auth);  
-     }  
- });  
 这里提醒下，HTTP Basic Authentication对于跨域又要发送post请求的用JavaScript是实现不了的(注：对于Chrome插件这类允许通过AJAX访问跨域资源的，是可以的)。
来源：[http://www.cnblogs.com/QLeelulu/archive/2009/11/22/1607898.html](http://www.cnblogs.com/QLeelulu/archive/2009/11/22/1607898.html)
