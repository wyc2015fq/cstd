# a link 标签的disabled属性实现disabled/enable - z69183787的专栏 - CSDN博客
2013年10月22日 11:30:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：28401
**由于a link是disabled属性设置成true,只是颜色变灰色但是还能提交.要想不能提交,可以删除href属性:**
*disable link*
**[html]**[view
 plain](http://blog.csdn.net/liangoo7/article/details/7054195#)[copy](http://blog.csdn.net/liangoo7/article/details/7054195#)
- function disableLink(link) {  
-     //link.disabled = true;     
-     link.setAttribute("disabled",true);   
-     link.removeAttribute('href');     
- }  
*enable link*
**[javascript]**[view
 plain](http://blog.csdn.net/liangoo7/article/details/7054195#)[copy](http://blog.csdn.net/liangoo7/article/details/7054195#)
- function enableLink(link,href) {  
-     link.setAttribute("disabled",false);     
-     link.setAttribute("href",href);     
- }  
