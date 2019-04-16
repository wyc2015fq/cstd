# JavaScript屏蔽Backspace键 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaScript屏蔽Backspace键](https://www.cnblogs.com/xdp-gacl/p/3785806.html)



　　今天在IE浏览器下发现，当把使用readonly="readonly"属性将文本框设置成只读<input type="text" readonly="readonly"/>时有一个奇怪的问题：如果光标进入只读文本框，然后按下Backspace键，就会跳转到上一个页面，效果就像点击了浏览器的后退按钮返回前一个页面一样，而在火狐和google下没有这样的问题出现，为了解决这个问题，写了一个如下的处理方法，如果文本框是只读的，那么就禁用Backspace键。

代码如下：

```
1 //处理键盘事件 禁止后退键（Backspace）密码或单行、多行文本框除外
 2     function banBackSpace(e){   
 3         var ev = e || window.event;//获取event对象   
 4         var obj = ev.target || ev.srcElement;//获取事件源   
 5         var t = obj.type || obj.getAttribute('type');//获取事件源类型  
 6         //获取作为判断条件的事件类型
 7         var vReadOnly = obj.getAttribute('readonly');
 8         //处理null值情况
 9         vReadOnly = (vReadOnly == "") ? false : vReadOnly;
10         //当敲Backspace键时，事件源类型为密码或单行、多行文本的，
11         //并且readonly属性为true或enabled属性为false的，则退格键失效
12         var flag1=(ev.keyCode == 8 && (t=="password" || t=="text" || t=="textarea") 
13                     && vReadOnly=="readonly")?true:false;
14         //当敲Backspace键时，事件源类型非密码或单行、多行文本的，则退格键失效
15         var flag2=(ev.keyCode == 8 && t != "password" && t != "text" && t != "textarea")
16                     ?true:false;        
17         
18         //判断
19         if(flag2){
20             return false;
21         }
22         if(flag1){   
23             return false;   
24         }   
25     }
26 
27 window.onload=function(){
28     //禁止后退键 作用于Firefox、Opera
29     document.onkeypress=banBackSpace;
30     //禁止后退键  作用于IE、Chrome
31     document.onkeydown=banBackSpace;
32 }
```

　　加上这样的处理之后，就可以解决"只读输入框在IE下按下Backspace键回退到前一个页面的问题了"









