# 用Jquery控制文本框只能输入数字和字母 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [用Jquery控制文本框只能输入数字和字母](https://www.cnblogs.com/xdp-gacl/p/3467245.html)



　　在公司开发WinForm项目时，发现公司自主研发的textbox控件非常强大，可以实现"只能输入数字"、"只能输入字母"和"只能输入数字和字母"的三种输入限制，这样就可以精确控制用户输入的内容范围，让"用户永远没有办法输入限定的内容范围之外的其他内容"，也就是"用户即使想犯错误也没有机会"，这种限制控件输入的方式给了我很大的启发，如果在web项目中也能做到这样的精确控制，那么就可以避免因为一些非法输入而造成系统出错，既然WinForm里面可以实现这样的控件，那么web项目里面也应该有办法去实现类似这样的控件或者能够做到类似的效果，经过自己的一番研究和查找资料，终于做到了类似的效果，针对"只能输入数字"、"只能输入字母"和"只能输入数字和字母"的三种输入限制，我封装成onlyNum()，onlyAlpha()和onlyNumAlpha()3个Jquery扩展方法，方便复用，由于里面一些JS代码涉及到了"禁用输入法，获取剪切板的内容"，而"禁用输入法，获取剪切板的内容"只能在IE浏览器下才有效，对于别的浏览器是无效的，因此这三个方法只适合在IE浏览器下使用才有效，三个方法的代码如下

## 一、限制只能输入数字

```
1 // ----------------------------------------------------------------------
 2 // <summary>
 3 // 限制只能输入数字
 4 // </summary>
 5 // ----------------------------------------------------------------------
 6 $.fn.onlyNum = function () {
 7     $(this).keypress(function (event) {
 8         var eventObj = event || e;
 9         var keyCode = eventObj.keyCode || eventObj.which;
10         if ((keyCode >= 48 && keyCode <= 57))
11             return true;
12         else
13             return false;
14     }).focus(function () {
15     //禁用输入法
16         this.style.imeMode = 'disabled';
17     }).bind("paste", function () {
18     //获取剪切板的内容
19         var clipboard = window.clipboardData.getData("Text");
20         if (/^\d+$/.test(clipboard))
21             return true;
22         else
23             return false;
24     });
25 };
```

## 二、限制只能输入字母

```
1 // ----------------------------------------------------------------------
 2 // <summary>
 3 // 限制只能输入字母
 4 // </summary>
 5 // ----------------------------------------------------------------------
 6 $.fn.onlyAlpha = function () {
 7     $(this).keypress(function (event) {
 8         var eventObj = event || e;
 9         var keyCode = eventObj.keyCode || eventObj.which;
10         if ((keyCode >= 65 && keyCode <= 90) || (keyCode >= 97 && keyCode <= 122))
11             return true;
12         else
13             return false;
14     }).focus(function () {
15         this.style.imeMode = 'disabled';
16     }).bind("paste", function () {
17         var clipboard = window.clipboardData.getData("Text");
18         if (/^[a-zA-Z]+$/.test(clipboard))
19             return true;
20         else
21             return false;
22     });
23 };
```

##  三、 限制只能输入数字和字母

```
1 // ----------------------------------------------------------------------
 2 // <summary>
 3 // 限制只能输入数字和字母
 4 // </summary>
 5 // ----------------------------------------------------------------------
 6 $.fn.onlyNumAlpha = function () {
 7     $(this).keypress(function (event) {
 8         var eventObj = event || e;
 9         var keyCode = eventObj.keyCode || eventObj.which;
10         if ((keyCode >= 48 && keyCode <= 57) || (keyCode >= 65 && keyCode <= 90) || (keyCode >= 97 && keyCode <= 122))
11             return true;
12         else
13             return false;
14     }).focus(function () {
15         this.style.imeMode = 'disabled';
16     }).bind("paste", function () {
17         var clipboard = window.clipboardData.getData("Text");
18         if (/^(\d|[a-zA-Z])+$/.test(clipboard))
19             return true;
20         else
21             return false;
22     });
23 };
```

使用方法：首先在画面加载完成之后编写如下的JS脚本

```
1 $(function () {
2     // 限制使用了onlyNum类样式的控件只能输入数字
3     $(".onlyNum").onlyNum();
4     //限制使用了onlyAlpha类样式的控件只能输入字母
5     $(".onlyAlpha").onlyAlpha();
6     // 限制使用了onlyNumAlpha类样式的控件只能输入数字和字母
7     $(".onlyNumAlpha").onlyNumAlpha();
8    });
```

对需要做输入控制的控件设置class样式

```
1  <ul>
2         <li>只能输入数字：<input type="text" class="onlyNum" /></li>
3         <li>只能输入字母：<input type="text" class="onlyAlpha" /></li>
4         <li>只能输入数字和字母：<input type="text" class="onlyNumAlpha" /></li>
5  </ul>
```

这样画面上凡是设置了class="onlyNum"的控件就只能输入数字，设置了class="onlyAlpha"的控件只能输入字母，设置了class="onlyNumAlpha"的控件只能输入数字和字母，通过这种方式就可以限制了用户的输入范围，避免用户进行一些非法的输入。

　　在我看来，这种限制控件输入的方式是比较好的一种方式，可以避免用户犯错，平时传统的做法都是用户输入了非法字符后，我们再弹出一个消息框告诉用户输入了非法字符，这种方式就是"**用户已经做了，我们才告诉用户不能这么做**"，而上面的那种方式是却能让用户只能输入我们指定范围内的字符，让"**用户永远没有机会去犯错**"。



　　写这篇文章既是对我自己学习的一个总结，也希望能够对一些初学者朋友们有一定的帮助，以上文章内容如果有写得不对的地方，欢迎广大朋友指正，我感激不尽！，另外，如果有朋友有办法在火狐和google下也能够禁用输入法和获取剪切板的内容的，也希望能够告知我一下，感激不尽！









