# (转)如何让ActiveXObject( "Microsoft.XmlDom ")对象在非IE浏览器下显示数据？firefox（火狐）... - weixin_33985507的博客 - CSDN博客
2014年10月08日 15:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11

# [如何让ActiveXObject( "Microsoft.XmlDom ")对象在非IE浏览器下显示数据？firefox（火狐）](http://blog.csdn.net/lejuo/article/details/11528243)
2013-09-10 16:01 2152人阅读 [评论](http://blog.csdn.net/lejuo/article/details/11528243#comments)(0) 收藏[举报](http://blog.csdn.net/lejuo/article/details/11528243#report)
在IE浏览器下，xmlDom对象一般这样被定义：
**[html]**[view plain](http://blog.csdn.net/lejuo/article/details/11528243)[copy](http://blog.csdn.net/lejuo/article/details/11528243)
- var xmlDom= new ActiveXObject("Microsoft.XMLDOM");  
为了兼容Firefox，需要修改为：
**[html]**[view plain](http://blog.csdn.net/lejuo/article/details/11528243)[copy](http://blog.csdn.net/lejuo/article/details/11528243)
- if (window.ActiveXObject){  
-     var xmlDom=new ActiveXObject("Microsoft.XMLDOM");  
- }  
- else{  
-     if (document.implementation&& document.implementation.createDocument){  
-         var xmlDom= document.implementation.createDocument("","",null);  
-      }  
- }  
在使用DOM操作XML文件时,我们可以使用Load方法直接加载文件路径即可，在ie和ff下通用。但是如果传入的是XML字符串，则在两种浏览器下就
会有所不同，IE下可以使用LoadXML方法直接调入下XML串,在FF下则不存在该方法，因此要使用W3CDom方式，具体如下：
**[html]**[view plain](http://blog.csdn.net/lejuo/article/details/11528243)[copy](http://blog.csdn.net/lejuo/article/details/11528243)
- var oParser=new DOMParser();  
- xmlDom=oParser.parseFromString(xmlStr,"text/xml");  
为兼容各种浏览器，我们这样去做即可：
**[html]**[view plain](http://blog.csdn.net/lejuo/article/details/11528243)[copy](http://blog.csdn.net/lejuo/article/details/11528243)
- - try{  
-   if (window.ActiveXObject){  
- xmlDoc= new ActiveXObject("Microsoft.XMLDOM");  
- xmlDoc.async = false;    
- isLoaded = xmlDoc.load(aXMLFileName);   
-   }   
-   else if  
-      (document.implementation&& document.implementation.createDocument){  
-         try{    
- xmlDoc = document.implementation.createDocument('', '', null);    
- xmlDoc.async = false;    
-             xmlDoc.load(aXMLFileName);    
-         } catch(e){    
-             var xmlhttp = new window.XMLHttpRequest();    
-             xmlhttp.open("GET",aXMLFileName,false);    
-             xmlhttp.send(null);    
- xmlDoc = xmlhttp.responseXML;    
-         }    
-   }  
-   else{  
-       alert("load data error");  
-   }  
-   }  
-   catch(e){  
-     alert(e.message);  
-   }  
