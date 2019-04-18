# js中，替换所有的回车换行符 - z69183787的专栏 - CSDN博客
2013年12月23日 23:20:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3324

Javascript代码  ![收藏代码](http://androidnew.iteye.com/images/icon_star.png)
- //替换所有的回车换行
- function TransferString(content)  
- {  
- var string = content;  
- try{  
-         string=string.replace(/\r\n/g,"<BR>")  
-         string=string.replace(/\n/g,"<BR>");  
-     }catch(e) {  
-         alert(e.message);  
-     }  
- return string;  
- }  
注意： 
string=string.replace(/\r\n/g,"<BR>") 这里的为替换"\r\n"  后面加上/g表示要替换掉字符中出现的所有的"\r\n",  没有/g就表示替换第一个出现的。
