# vue开发spa应用时 微信支付问题(尤其是ios) - 三少GG - CSDN博客
2016年09月05日 10:49:22[三少GG](https://me.csdn.net/scut1135)阅读数：11056

**Tips:换ios的试试，ios会告诉你什么错误；Android没有**
######################终结#######################
window.location.href导致!!!  待跳转路径要加上/#!/
临时解决办法： 两个都加上支付路径：
http://example.com/#!/http://example.com/#!/order
######################终结#######################

[http://get.ftqq.com/8572.get](http://get.ftqq.com/8572.get)
我们的应用中有3个页面用到微信支付：
http://example.com/#!/cart/indexhttp://example.com/#!/order/orderlisthttp://example.com/#!/order/orderinfo
微信支付允许配置3个目录为授权目录，另外允许配置一个测试环境目录，授权目录必须配置到最后一级目录，配置在根目录不行。**到这里其实还都不是问题，问题是微信判断当前路径的方式。**
我们先定义两个名词：首先把当前页面叫做“Current Page”；当我们从微信别的地方点击链接呼出微信浏览器时所落在的页面、或者点击微信浏览器的刷新按钮时所刷新的页面，我们叫做“Landing Page”。举个例子，我们从任何地方点击链接进入页面A，然后依次浏览到B、C、D，那么Current Page就是D，而Landing Page是A，如果此时我们在D页面点击一下浏览器的刷新按钮，那么Landing Page就变成了D（以上均是在单页应用的环境下，即以hashbang模式通过js更改浏览器路径，直接href跳转的不算）。
问题来了，在ios和安卓下呼出微信支付的时候，微信支付判断当前路径的规则分别是：
- IOS：Landing Page
- 安卓：Current Page
这个规则是我试了N次试出来的，非常的坑，这就意味着，在ios环境下，任何一个页面都有可能成为支付页面（因为我无法预知和控制用户在哪个页面点微信浏览器的刷新按钮，或是用户通过哪个连接从外部进入到系统）。我在网上用了各种方式搜索这个问题的解决方案，都没找到好的，有关这个问题的帖子都少之又少，都只有吐槽它而没有解决了的。
我把这个问题放了好几天，虽然有一个解决方案，就是进入到支付页面的时候使用href跳转的方式，会造成页面刷新，影响一些用户体验。就是因为这个方案不完美，我就一直搁着，今天呢就又把这个问题拿出来鼓捣。
井号“#”后面应该都忽略才对。于是乎，我想了想，反正入口文件就是个静态html，服务器端也不直接接收query，于是就把链接改成了
```
http://example.com/?#!/cart/index
```
就加了一个问号，于是微信浏览器妥妥的把井号“#”后面的内容给去掉了
############################################################
官网调用方式不一致问题：
https://pay.weixin.qq.com/wiki/doc/api/jsapi.php?chapter=7_7&index=6
http://mp.weixin.qq.com/wiki/7/aaa137b55fb2e0456bf8dd9148dd613f.html#.E5.8F.91.E8.B5.B7.E4.B8.80.E4.B8.AA.E5.BE.AE.E4.BF.A1.E6.94.AF.E4.BB.98.E8.AF.B7.E6.B1.82
解决办法：
```
success: function (res) {
  if(res.**errMsg** == "**chooseWXPay:ok**" ) {
      //支付成功
  }else{
      alert(res.errMsg);
  }
  },
**cancel**:function(res){
  //支付取消
}
```
参考：http://www.liball.me/wxpay-is-shit/
############################################################
必看文章：
1.[微信支付遇到的各种坑及解决方案随笔记录，](http://www.cnblogs.com/xueranzp/p/5287691.html)[http://www.cnblogs.com/xueranzp/p/5287691.html](http://www.cnblogs.com/xueranzp/p/5287691.html)
支付目录设置，根据 https://pay.weixin.qq.com/wiki/doc/api/jsapi.php?chapter=7_3 配置授权目录，**需要特别注意的是，目录要配置到精确目录，而不是二级或者三级目录**，并且区分大小写。目录指的是确认付款的页面目录而不是调用支付的接口目录。 比如商品支付页面链接为 http://www.XXX.com.cn/pay/payorder/a.html 需要配置为
 www.XXX.com.cn/pay/payorder/ 
2.微信支付的那些坑[http://www.liball.me/wxpay-is-shit/](http://www.liball.me/wxpay-is-shit/)
3.
**[https://blog.asaki.me/posts/chooseWXPay-fail/](https://blog.asaki.me/posts/chooseWXPay-fail/)**
对着官方文档这句：“发起支付的页面目录必须与设置的精确匹配。”看了半个小时，我突然想到是不是URL hash生成的“`/#!/`目录”被微信当成了一个次级目录了？越想越觉得就是那么回事儿，然后把支付授权目录改成了这样：
![支付授权目录](https://blog.asaki.me/posts/chooseWXPay-fail/02.png)支付授权目录
十分钟后，终于！终于微信支付这个智障认了这个地址！仰天长啸一声卷铺盖下班……
总结一下，写单页应用碰到微信支付的同行们如果用URL hash来实现路由的话，务必要把URL hash后的“目录”也配置进支付授权目录里去，微信是不会理会这个html文件的真正路径的……
