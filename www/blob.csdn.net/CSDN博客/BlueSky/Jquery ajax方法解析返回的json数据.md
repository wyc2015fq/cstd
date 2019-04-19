# Jquery ajax方法解析返回的json数据 - BlueSky - CSDN博客
2016年01月12日 15:04:04[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：549
个人分类：[Java Spring](https://blog.csdn.net/ASIA_kobe/article/category/6039238)
最近在用jquery的ajax方法传递接收json数据时发现一个问题，那就是返回的data数据，有时候可以直接作为json数据使用，可有时候又不行。查了些资料,解释如下:
**[javascript]**[view
 plain](http://blog.csdn.net/haiqiao_2010/article/details/12653555#)[copy](http://blog.csdn.net/haiqiao_2010/article/details/12653555#)
- $.ajax({   
-         url: ajaxurl,  
-         type: "POST",  
-         success: function(data){  
- //假设返回的json数据里有status及info2个属性
- //有时候可以直接ajaxobj.status或者ajaxobj["status"]去访问  
- //但有时候，却要通过eval()或者 $.parsejson();才可以通过ajaxobj.status访问，而且这种情况下，需要是complete而不是success
-             ajaxobj=eval("("+data+")");  
- //或者$.parsejson()
- //var ajaxobj = $.parsejson(data);
- if(ajaxobj.status=="0")  
-             {  
-                 alert("请登陆.");  
-             }  
- elseif(ajaxobj.status=="1")//未绑定微博
-             {  
-                 alert(ajaxobj.info);  
-             }  
- returntrue;  
-         },  
-         error:function(ajaxobj)  
-         {  
- if(ajaxobj.responseText!='')  
-                     alert(ajaxobj.responseText);  
-         }  
-     });  
先说明第一种情况:
    能够直接 data.属性名访问的情况，服务器端代码一定是直接return的一个常量字符串。 
    什么是常量字符串呢，常量字符串就是指直接用“”组成的字符串，没有定义String 变量直接把一串“”print到前台的情况，就可以直接data.属性名访问，而且jquery端只要写success就可以拿到。 
下面是造成要eval并且不能进入success的原因：
     这种情况是因为服务器端向外print的时候是一个String对象，通常此类问题在我的代码里是因为后台json比较复杂，在组织的时候我用到了StringBuffer，然后最后print的时候print的是StringBuffer对象的toString，所以就相当于print了一个String对象 
     这种情况下jquery的ajax方法就不会进入success方法，只能用complete接收，并且想要解析data里的json数据的话，必须对data.responseText进行eval ()或者 $.parsejson();
    除此两点，还有需要注意的是，如果你使用的是jq1.4，那么他对json的格式有着更严格的要求，所有的key和属性都要用双引号标注起来，虽然key不用双引号原生的js是允许的，但是jq1.4似乎有这个要求。
