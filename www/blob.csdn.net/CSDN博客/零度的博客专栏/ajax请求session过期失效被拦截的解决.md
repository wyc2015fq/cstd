# ajax请求session过期失效被拦截的解决 - 零度的博客专栏 - CSDN博客
2016年11月23日 10:09:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：3525
通常使用过滤器 或拦截器来进行判断session或cookies里是否失效从而跳转页面，通常普通连接请求实现如下：
**[java]**[view plain](http://blog.csdn.net/itlqi/article/details/48706663#)[copy](http://blog.csdn.net/itlqi/article/details/48706663#)[print](http://blog.csdn.net/itlqi/article/details/48706663#)[?](http://blog.csdn.net/itlqi/article/details/48706663#)
- UserInfo user = (UserInfo)request.getSession().getAttribute("user");  
- if(user == null) {  
-     response.sendRedirect("/login.html");  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
UserInfo user = (UserInfo)request.getSession().getAttribute("user");
if(user == null) {
	response.sendRedirect("/login.html");
}
```
对于一个普通的页面没什么问题，但通过ajax请求拦截后却无法实现跳转，扑捉到登录的源代码，在网上查了很多资料
方法也是众说纷纭，有的ajaxSetup设置全局 后台拦截器中通过请求头部信息判断是否ajax请求然后返回参数判断，
有的则是修改jquery ajax源码，太多了 
查了资料搞了个最简单的后台不用判断前台 使用一个最方便的做了一次扩展 重写ajax方法实现前置处理，代码如下：
**[javascript]**[view plain](http://blog.csdn.net/itlqi/article/details/48706663#)[copy](http://blog.csdn.net/itlqi/article/details/48706663#)[print](http://blog.csdn.net/itlqi/article/details/48706663#)[?](http://blog.csdn.net/itlqi/article/details/48706663#)
- jQuery(function($){  
- // 备份jquery的ajax方法  
- var _ajax=$.ajax;  
- // 重写ajax方法，先判断登录在执行success函数 
-     $.ajax=function(opt){  
- var _success = opt && opt.success || function(a, b){};  
- var _opt = $.extend(opt, {  
-             success:function(data, textStatus){  
- // 如果后台将请求重定向到了登录页，则data里面存放的就是登录页的源码，这里需要找到data是登录页的证据(标记)
- if(data!='[object Object]' && data.indexOf('weidenglu') != -1) {  
-                     window.location.href= "gz.html";  
- return;  
-                 }  
-                 _success(data, textStatus);   
- 
-             }    
-         });  
-         _ajax(_opt);  
-     };  
- });  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
jQuery(function($){
    // 备份jquery的ajax方法  
    var _ajax=$.ajax;
    // 重写ajax方法，先判断登录在执行success函数 
    $.ajax=function(opt){
    	var _success = opt && opt.success || function(a, b){};
        var _opt = $.extend(opt, {
        	success:function(data, textStatus){
        		// 如果后台将请求重定向到了登录页，则data里面存放的就是登录页的源码，这里需要找到data是登录页的证据(标记)
        		if(data!='[object Object]' && data.indexOf('weidenglu') != -1) {
        			window.location.href= "gz.html";
        			return;
        		}
        		_success(data, textStatus); 
        		
            }  
        });
        _ajax(_opt);
    };
});
```
将其代码写到 一个通用的js文件中 页面调用一下即可 ，每个ajax执行之前都会执行它，data!='[object Object]' data.indexOf('weidenglu')，
data 返回值如果是 判断里面是否存在weidenglu  ，weidenglu在你的login.html页面中写上一个隐藏域即可 因为一旦被拦截未登录返回的就是登录的代码。
