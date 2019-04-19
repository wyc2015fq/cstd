# jsp中按钮的submit 和 button类型 - xqhrs232的专栏 - CSDN博客
2018年10月26日 11:57:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：364
原文地址::[https://blog.csdn.net/huoqubing/article/details/2029427](https://blog.csdn.net/huoqubing/article/details/2029427)
相关文章
1、jsp的form表单提交几种方式----[https://blog.csdn.net/java_exception_souji/article/details/53928384?utm_source=blogxgwz1](https://blog.csdn.net/java_exception_souji/article/details/53928384?utm_source=blogxgwz1)
2、jsp页面中的错误总结，关于form表单的button和submit按钮----[https://blog.csdn.net/kakasun_0/article/details/52549142?utm_source=blogxgwz0](https://blog.csdn.net/kakasun_0/article/details/52549142?utm_source=blogxgwz0)
在JSP中按钮的类型有submit ,button,reset三种。
button为普通按钮，可以有其响应事件。
reset从字面就可以看出是重置的意思，点击后把表单的内容清空。
submit  提交按钮，可以用表单的action="*****,jsp"项 使之跳转到另外的一个页面。当一个表单有多个submit按钮,且需要跳转到不同的页面时，将action清空   然后通过script脚本跳转。
<script   language= "JavaScript "> 
<!-- 
function   check(){ 
frm.action   =   "checkname.jsp " 
} 
function   mysubmit()   { 
frm.action   =   "zhuce.jsp " 
} 
</script> 
<form   method=post   action= " "   name= "frm "> 
<input   type= "submit "   onclick= "check() "> 
<input   type= "submit "   onclick= "mysubmit() "> 
</form> 
ps：参考csdn--  http://topic.csdn.net/u/20070823/16/a612f99b-b283-47ff-a64b-ca572e4dde3a.html
--------------------- 
作者：huoqubing 
来源：CSDN 
原文：https://blog.csdn.net/huoqubing/article/details/2029427 
版权声明：本文为博主原创文章，转载请附上博文链接！
