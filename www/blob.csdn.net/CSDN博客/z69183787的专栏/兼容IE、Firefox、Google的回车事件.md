# 兼容IE、Firefox、Google的回车事件 - z69183787的专栏 - CSDN博客
2012年11月25日 20:40:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：795
个人分类：[Javascript-概述																[前端-浏览器兼容性](https://blog.csdn.net/z69183787/article/category/2176023)](https://blog.csdn.net/z69183787/article/category/2175033)

**[java]**[view
 plain](http://blog.csdn.net/geloin/article/details/7577417#)[copy](http://blog.csdn.net/geloin/article/details/7577417#)
- function randKeyDown(evt) {  
-     evt = (evt) ? evt : ((window.event) ? window.event : "");  
-     keyCode = evt.keyCode ? evt.keyCode : (evt.which ? evt.which  
-             : evt.charCode);  
- if (keyCode == 13) {  
-         login();  
-     }  
- 
- }  

**[java]**[view
 plain](http://blog.csdn.net/geloin/article/details/7577417#)[copy](http://blog.csdn.net/geloin/article/details/7577417#)
- onkeydown="javascript:randKeyDown(event);"
