# JS中应用正则表达式转换大小写 - z69183787的专栏 - CSDN博客
2018年01月16日 18:40:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1039
JS中应用正则表达式转换大小写，代码很简单，看代码：
以下首字母大写，其它字母小写 
[javascript][view
 plain](http://blog.csdn.net/u010899899/article/details/9125049)[copy](http://blog.csdn.net/u010899899/article/details/9125049)
- 
<script type="text/javascript">  
- function replaceReg(reg,str){  
- 
  str = str.toLowerCase();  
- return str.replace(reg,function(m){return m.toUpperCase()})  
- 
 }  
- var reg = /\b(\w)|\s(\w)/g;  
- var str = 'share javascript';  
- var str2 = 'SHARE JAVASCRIPT';  
- var str3 = 'Test \n str is no good!';  
- var str4 = 'final test';  
- 
 document.write(replaceReg(reg,str)+'<br />');  
- 
 document.write(replaceReg(reg,str2)+'<br />');  
- 
 document.write(replaceReg(reg,str3)+'<br />');  
- 
 document.write(replaceReg(reg,str4)+'<br />');  
- 
</script>  
==============================================================
以下只首字母大写，其它字母大小写不作处理
[javascript][view
 plain](http://blog.csdn.net/u010899899/article/details/9125049)[copy](http://blog.csdn.net/u010899899/article/details/9125049)
- 
<script language="JavaScript">   
- 
<!--   
- var str="xi nAn shi you xUe yuan china people"   
- 
alert(str.replace(/\s[a-z]/g,function($1){return $1.toLocaleUpperCase()}).replace(/^[a-  
- 
z]/,function($1){return $1.toLocaleUpperCase()}))   
- //-->   
- 
</script>   
运行代码，直接看效果就可以了！
