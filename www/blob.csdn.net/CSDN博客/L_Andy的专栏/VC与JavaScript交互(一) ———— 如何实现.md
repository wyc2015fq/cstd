# VC与JavaScript交互(一) ———— 如何实现 - L_Andy的专栏 - CSDN博客

2014年10月11日 18:51:50[卡哥](https://me.csdn.net/L_Andy)阅读数：4984


为什么要让VC与JavaScript交互？

1.有时候我们需要让自己的软件打开一个网页，来获取页面上的一些数据。这时，可以用mshtml解析HTML提取出数据，也可以向HTML文档动态写入我们准备好的JS代码，用JS代码获取HTML上的数据，然后用VC调用该JS代码取得数据。

2.有时候我们需要让自己的软件打开一个网页并操控该网页，填写表单，提交等动作。这时，可以用mshtml操作HTML，给文本框赋值，模拟点击按钮。也可以向HTML文档动态写入我们准备好的JS代码，用JS代码实现填单，提交等动作，然后用VC调用一下JS代码即可。

3.有时候我们需要用HTML网页做界面，用JS管理HTML页面，用VC调用JS传递进数据，JS把这些数据通过HTML显示在界面上。CHtmlDialog正是这种产物。

4.有时候我们需要基于HTTP协议与WEB服务器直接交互。比如基于HTTP协议来登录QQ空间。但是对浏览器抓包发现QQ号是明文传输，但QQ密码是密文，它是如何加密的呢？这些加密算法肯定都在页面上的JS里，因为这个加密过程是在客户浏览器上实现的。我们可以找到加密相关的JS代码，[仔细阅读](http://www.so.com/s?q=%E4%BB%94%E7%BB%86%E9%98%85%E8%AF%BB&ie=utf-8&src=se_lighten_f)分析，改写为C++代码实现，但比较复杂。一个简单的办法就是直接把这些用于加密的JS代码复制出来，用VC调用JS代码，让它的JS代码完成加密过程，然后我们把密文拿过来，用HTTP协议POST发送到WEB服务器，即完成了登陆动作。

5.等等，还有很多用途。

那么，在Windows平台上用VC开发的程序，如何与JavaScript交互？

通常，我们都是用WebBrowser加载包含JS代码的HTML，然后通过WebBrowser对象获取IHTMLDocument接口(对于ActiveX的WebBrowser来说是get_Document方法，对于CHtmlView对象来说是GetHtmlDocument方法)。其中IHTMLDocument2接口有一个get_Script方法，可以获取用于控制JS代码的IDispatch接口。VC调用JS函数，都是通过这个IDispatch接口的Invoke方法来完成。使用IDispatch接口的GetIDsOfNames方法根据JS函数名获取调度标识符DISPID，使用Invoke来调用JS函数。Invoke最后两个参数用于返回错误信息，可为NULL。这个IDispatch接口调用起来很麻烦，下文将会介绍如何简便的调用。网上有个外国人写了个CWebPage类实现VC与JS交互，用的正是这种方法。
[http://www.codeproject.com/Articles/2352/JavaScript-call-from-C](http://www.codeproject.com/Articles/2352/JavaScript-call-from-C)


关于WebBrowser：

在VC中使用WebBrowser，一般有两种方法。MFC中有个CHtmlView封装了WebBrowser，用起来很方便。虽然CHtmlView派生于CView，是个视图类，但它也派生于CWnd，将其用于对话框上，完全没有问题，只是在某些地方需要小修改一下。其中需要注意的两个问题就是：

1. CHtmlView的构造函数是protected的，不允许直接构造一个CHtmlView对象。必须从CHtmlView派生后再构造。

2.如果是在栈上创建CHtmlView对象，必须重载PostNcDestroy并什么也不写。因为默认的PostNcDestroy会delete
  this;而出错。如果是在堆上创建CHtmlView对象则要注意防止二次delete。

另一种方法是使用WebBrowser的ActiveX控件，这种方法可以在MFC项目中使用，也可以在非MFC项目中使用。

关于IDispatch：

我们知道IDispatch是COM双接口中的调度接口。一般用于供脚本语言调用COM组件。对于编译型的C++语言，让它调用这种接口，是很麻烦的。毕竟用IDispatch接口调用COM对象的各种方法、设置与获取COM对象的属性、让COM对象回调我们，都是用IDispatch的Invoke方法来实现。一个Invoke就要实现那么多功能，用起来当然很麻烦。不过好在ATL智能指针类中的CComDispatchDriver(即CComQIPtr<IDispatch>)封装了IDispatch接口，使用我们用起来大大的方便！CComDispatchDriver对GetIDsOfNames和Invoke进一步进行了封装，只需更少的参数即方便可调用。

获取和设置COM对象属性可以用CComDispatchDriver的这些方法：

GetProperty

GetPropertyByName

PutProperty

PutPropertyByName

其实使用IDispatch调度接口来设置、获取COM属性，调用COM方法，都是使用GetIDsOfNames和Invoke。实际上这四个方法都是对GetIDsOfNames和Invoke的封装。简化调用的复杂性。

调用COM对象的方法可以用这些方法：

Invoke0    //调用0个参数的方法

Invoke1    //调用1个参数的方法

Invoke2    //调用2个参数的方法

InvokeN    //调用多个参数的方法

这些函数都有两个版本，一个是接受调度标示符DISPID，需要自己先调用GetIDsOfNames来获取。一个是接受OLE字符串的版本，这个版本在内部会调用GetIDsOfNames来获取DISPID。这些函数用起来很方便，不需要我们自己填充DISPPARAMS结构，但是它对原始Invoke的调用时，最后两个参数都是NULL，即不需要获取错误信息。如果需要获取错误信息，我们需要自行调用原始Invoke方法。

注意，这些方法是ALT的CComDispatchDriver封装的方法，调用时应使用"."而不是"->"。因为"->"获得的是CComDispatchDriver内部的Dispatch指针。

另一个要注意的问题是，一定要等Navigate完全加载一个html文档后（触发OnDocumentComplete）。才能获取IHTMLDocument2和Script。否则会出现空指针或找不到JS函数。所以不能在调用Navigate打开HTML后就紧接着获取IHTMLDocument2和Script，要等HTML文档加载完。

上面说了这么多COM对象，和VC调用JS有什么关系？别忘了我们用IHTMLDocument2接口的get_Script方法获取到了代表HTML文档中JS代码的IDispatch接口，我们用IDispatch接口，把HTML文档中的这堆JS代码当作一个COM对象，来操控它。上面说的Invoke0,Invoke1,Invoke2,InvokeN，正是分别被我们用来调用0个参数的JS函数，1个参数的JS函数，2个参数的JS函数，N个参数的JS函数。

说了那么多，下一篇文章，让我们来实际动手，用VC调用一下JS函数看看。


