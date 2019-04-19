# JavaScript:浅谈iOS与H5的交互-JavaScriptCore框架 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月17日 22:10:28[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：619
![](http://upload-images.jianshu.io/upload_images/1396375-c33a7e1575a6c4ae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
#### 前言
小的作为一个iOS程序猿,可能研究JavaScript以及H5相关的知识并不是为了真正的要去转行做这一方面,其实更多的为了要研究OC中的JavaScriptCore框架,JavaScriptCore框架主要是用来实现iOS与H5的交互.当然了,最早我曾经写过[iOS浅谈webView的JS一种用法.(去掉web显示页的广告)](http://www.jianshu.com/p/c3204ed25b68),这种算得上最简单的iOS与H5的交互了.现在混合编程越来越多,H5的相对讲多,所以研究JavaScriptCore框架是相当有必要的.好了不扯这些没用的了,来搞今天的主题---JavaScriptCore框架,首先来说说JavaScriptCore框架几个常用的类.
#### JavaScriptCore的类说明
在做OC与H5的交互之前,我们需要先导入JavaScriptCore框架.
`#import <JavaScriptCore/JavaScriptCore.h>`
然后我们进入JavaScriptCore框架的JavaScriptCore.h,我们发现总共如下有几个类.
![](http://upload-images.jianshu.io/upload_images/1396375-44a82556730ff976.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
###### JSContext
JSContext代表一个JavaScript的执行环境的一个实例。所有JavaScript执行都是在上下文内进行。JSContext还用于管理对象的生命周期内JavaScript的虚拟机(API文档原文翻译)。作为上下文很多地方我们的都有可能使用过,比如CoreData,CoreGraphics等等,那么上下文对象到底是什么呢?你可以理解为是一个两者的连接桥梁,如图所示.
![](http://upload-images.jianshu.io/upload_images/1396375-55b648abc23f3f6a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
- 
###### JSValue
JSValue的主要作用就是用来接收JSContext执行后的返回结果,当然了,JSValue可以是js的任意类型,例如,js中的变量,对象以及函数.下面我们就举个例子说明一下,例如我们现在需要从JSContext对象中接收到一个变量.我们可以如下所示.
首先在<script>标签中我们定义了一个字符串变量.
```xml
<script type="text/javascript">
      var myObject = "myObject";
  </script>
```
然后我们在OC的代码中如下表示,其中self.context代表着已经初始化完成的JSContext对象.
`  JSValue *myObject = self.context[@"myObject"];`
当然了,我们如果需要在OC中使用JSValue对象,那么我们可能需要适当的转换一下他的类型,框架提供了很多方法.比如上面的例子我们想要打印的话就可以如下所示.
`  NSLog(@"%@",[myObject toString]);`
- 
###### JSManagedValue
JSManagedValue这个类其实到现在我还没有具体用到过.当时这里我也要根据API文档说明一下.JSManagedValue是JSValue的封装，用它可以解决JS和OC代码之间循环引用的问题,JSManagedValue最常用的用法就是安全的从内存堆区里面引用JSValue对象.如果JSValue存储在内存的堆区的方式是不正确的,很容易造成循环引用,然后导致JSContext对象不能正确的释放掉.
- 
###### JSVirtualMachine
一个JSVirtualMachine对象其实代表着一个JavaScript对象空间或者一组执行资源。JSVirtualMachine支持线程安全锁,虚拟机,并发分配支持的JavaScript执行.也就说JSVirtualMachine用来管理整个JavaScript,当然了,这个类我也没有用到过.
- 
###### JSExport
JSExport是一个协议,通过实现它可以完成把一个native对象暴漏给JS,当然了我们要指定native对象.比如我们指定native对象就是自身.如下所示.
`  self.context[@"native"] = self;`
#### JavaScriptCore中JSContext对象的初始化
在进行iOS与H5的交互之前,我们需要做的是对JSContext对象进行初始化的设置,全程是在webView的**- (void)webViewDidFinishLoad:(UIWebView *)webView**这个代理方法中进行的.所以我们需要对webView进行初始化以及设置代理,这里就不过多的言语了.我们直接看一下在代理方法中如何对JSContext对象进行初始化设置.代码如下所示.
```
- (void)webViewDidFinishLoad:(UIWebView *)webView{
     //对JSContext对象进行初始化
     self.context = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
         //验证JSContext对象是否初始化成功
     self.context.exceptionHandler = ^(JSContext *context, JSValue *exceptionValue)
    {
        context.exception = exceptionValue;
    };
}
```
#### JavaScript调取OC代码
上一个模块我们已经对JSContext对象进行了初始化,那么要使用JavaScript调取OC代码主要有两种方式,一种是使用block(简单粗暴);另外一种是使用JSExport协议(决胜千里).
- 
##### block方式
block方式比较简单,也是我比价推荐的一种,但是要注意防止循环引用问题的发生.首先我们先说一下不带有参数的函数调用,也就是说我们不需要从网页那边有参数的传值,比如跳转页面等等.代码如下所示.
HTML文件中的<body>标签的按钮代码
```xml
<button onclick="myAction();" style="">点击按钮返回上一个页面</button>
```
OC中在`- (void)webViewDidFinishLoad:(UIWebView *)webView`方法中对block块进行代码实现.
```
__weak typeof(self)temp = self;
  self.context[@"myAction"] = ^(){
      [temp.navigationController popViewControllerAnimated:YES];
  };
```
![](http://upload-images.jianshu.io/upload_images/1396375-3ed4f3f951c3dc7d.gif?imageMogr2/auto-orient/strip)
接下来我们看一下,通过页面的传值,我们把H5标签的值作为参数进行传值操作,并且调用OC的block进行打印.
HTML文件中的<body>标签的div标签代码(一个输入框,一个按钮)
```xml
<input type="text" name="输入框" id="textField" placeholder="请输入要打印的信息" />
          <button onclick="log(document.getElementById('textField').value);">打印输入框信息</button>
```
然后我们在OC的代码中实现以下block函数.其中**string**参数就是网页中输入框的值.
```
self.context[@"log"] = ^(NSString *string){
      NSLog(@"%@",string);
  };
```
效果图如下所示.坐标是手机模拟器,右边是Xcode控制台.
![](http://upload-images.jianshu.io/upload_images/1396375-db74d00f08c93bd0.gif?imageMogr2/auto-orient/strip)
- 
##### JSExport协议方式
通过实现JSExport协议方式进行OC与JS的交互,这里我只是简单的实现以下没有参数的函数调用.首先,我们在HTML文件中创建一个按钮用来调用OC中JSExport协议方法.
```xml
<button onclick="native.myLog();">调用OC中myLog方法</button>
```
我们需要把native对象指定为当前控制器,也就是自己.这句代码是在`- (void)webViewDidFinishLoad:(UIWebView *)webView`方法中实现的.
`  self.context[@"native"] = self;`
然后我们要写一个继承于JSExport的协议,协议如下所示.这里我要做一下对JSExportAs(PropertyName,Selector)这个宏定义做一下解释,也就说在PropertyName代表着JS函数的名称,而Selector代表着OC的协议方法,要注意的是Selector必须要有参数,不管是否需要传值.(不知所以然.
