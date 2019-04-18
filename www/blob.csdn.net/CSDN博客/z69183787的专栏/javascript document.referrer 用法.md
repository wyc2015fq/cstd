# javascript document.referrer 用法 - z69183787的专栏 - CSDN博客
2014年02月18日 16:28:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：859
举例： 
1. a.html文件内容如下： 
<a href="b.html">浏览b.html </a> 
2. b.html文件中的内容如下： 
<body> 
<script type="text/javascript"> 
document.write(document.referrer); 
</script> 
</body> 
3. 则在通过a.html中的超链接访问b.html的时候，显示的结果是： 
http://127.0.0.1:8180/a.html 
说明： 
经过测试，需要将两个文件放在服务器中才能得到想要的结果,若直接在本地文件夹中则得到空字符串，若直接在浏览器地址栏中输入b.html的URL地址或使用打开菜单访问b.html，则document.referrer的值为空字符串。
