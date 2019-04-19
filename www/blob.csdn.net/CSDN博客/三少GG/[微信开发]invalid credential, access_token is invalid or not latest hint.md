# [微信开发]invalid credential, access_token is invalid or not latest hint - 三少GG - CSDN博客
2016年09月03日 05:23:45[三少GG](https://me.csdn.net/scut1135)阅读数：83952
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
##################正解################
**这种情况跟这个库没有直接关系。请检查一下是否有别的地方同时请求了access token，导致微信服务器发放了新的access token给别人。尤其是dev环境**
**##################正解################**
查了好久，先发现下载到本地的文件size很小，只有100来字节，我以为只有头文件，老大说，你要么看看这文件的二进制流是什么，打开一看原来是一个error信息，是JSON字符串
{"errcode":40001,"errmsg":"invalid credential, access_token is invalid or not latest hint: [2HYQIa0031ge10] "}
看了下错误code，是access_token过期。
但是官网里说 token默认是7200秒过期，为啥那么快？结果后来百度到，如果有第二地方也请求同一个token的话，那么第一个token会在5分钟之内过期。这也就说明了，为什么在我搭建好第二个环境的时候，老环境就出现了这种问题。因为这两个环境用的是同一个AppID和AppSecret来取得的
access_token，而这个access_token的取得并不是在服务器启动的时候，而且是在需要调用接口的画面初期化的时候去取得的。把取得的token放在s()全局缓存变量中，3600秒过期，方法是
$token = s("access_token");
if($token){
//重新取得token；
}
return $token;
所以就一直使用过期的token;一旦token过期，就无法调用js接口了。
http://www.mamicode.com/info-detail-1123323.html
