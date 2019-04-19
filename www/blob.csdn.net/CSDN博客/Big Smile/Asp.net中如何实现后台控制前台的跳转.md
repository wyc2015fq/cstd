# Asp.net中如何实现后台控制前台的跳转 - Big Smile - CSDN博客
2016年09月04日 13:26:42[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2254
         背景：在做订餐系统的修改密码功能的时候，有这样一个需求，就是要在修改完成密码之后，需要跳转到登陆界面进行验证重新登陆，于是就有了今天的博客。
# 一、触发的方式
        对于一般的触发的方式，我们还是经常以直接跳转到相应的页面为主，但是我们还需要一个提示框来提示成功，所以就不能在后台让界面直接跳转，如果直接跳转的话，新的页面会把要弹出的对话框的页面刷新，导致了对话框好像就没有弹出一样，这要怎么办呢？
         首先呢，对于弹出框，都是用的JavaScript代码来进行alert才出来的，所以，我们需要在后台调用前端的JS代码才行，但是这要怎么做才好呢?
# 二、Page的方法
         以前，我们在后台想要弹出框，用的就是
```
Page.ClientScript.RegisterStartupScript(Page.GetType(), "message", "<script language='javascript' defer>alert('密码更新失败，请联系管理员！');</script>");
```
          这个里面用的page的方法，然后再页面上面运行的后面写的js代码，那么这样也写弹出在加上跳转不就可以了，然而事实时残酷的，这样写是不行的，那么怎么办呢？
        经过我在网上查找资源的时候发现，page还有别的方法可以操作JS代码，就是可以用来调用页面上的js代码，书写方式如下：
`Page.RegisterClientScriptBlock("aaa", "<script>javascript:fun1()</script>");`
这段代码可以调用页面上的fun1这个方法，具体前面写的aaa是干什么用的我也不知道，但是这样确实可以执行，所以，在页面上我写了如下的代码：
```java
<script type="text/javascript">      /*修改密码的提示和重新登录*/
        function fun1() {
            alert("密码修改成功！");
            window.location.href = "../Login.aspx";
        }
 </script>
```
就这样修改完密码之后，就可以运行这个函数了，并且弹出框可以运行，不会被刷新。
# 三、总结
        看来，page这个对象的方法还有很多，等着我们去探索。
