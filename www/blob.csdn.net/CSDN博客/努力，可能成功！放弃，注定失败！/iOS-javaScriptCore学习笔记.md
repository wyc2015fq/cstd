# iOS-javaScriptCore学习笔记 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月29日 20:33:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4650
# iOS-javaScriptCore学习笔记
字数7533
阅读34评论0喜欢1
# 一.简介
- 1.是什么?
> 
javaScriptCore是苹果在iOS7以后推出的一套框架, 一套将原生(OC,Swift均可)和js之间通讯变简单,变强大的框架.
1.1 JSBinding ? Hybrid?
JSBinding是一个技术术语, 通常是这么说的 "JSBinding技术", 那什么是JSBinding技术?
应该拆开看一个是 JS 一个是binding, 字面意思是js绑定/桥接, js绑定/桥接? 绑定/桥接什么?
答:绑定桥接原生.就是js和其他语言的绑定/桥接. 就是js和Native之间建立起了一中绑定/桥接的技术, 使得Native和js之间可以交流的一种技术.
那么我们学的这个javaScriptCore就是一种JSBinding技术,苹果原生提供给我们的.
![](http://upload-images.jianshu.io/upload_images/571446-374b9b55839603d6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160729_7.png
![](http://upload-images.jianshu.io/upload_images/571446-1d49a5152e5e6056.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160729_8.png
结构图, 下面橘黄色那一层就是JSBinding技术, JSBinding它打通和js和native之间的鸿沟, 让js和native两者之间能够进行通讯
![](http://upload-images.jianshu.io/upload_images/571446-a427a7c75f79fe9e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160729_9.png
1.2Hybrid?
Hybrid是web技术和native之间的桥接的技术.
![](http://upload-images.jianshu.io/upload_images/571446-5608d58a5521c1fc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160729_10.png
结构图
![](http://upload-images.jianshu.io/upload_images/571446-0eec6c51ece361df.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160729_11.png
1.3jsBinding和Hybrid什么关系.
1.3.1首先他两个不是一个东西.
1.3.2第二他两个都可以实现native和js之间 的通讯, jsBinding用的是javaScriptCore而hybrid是通过webVew(我这里仅仅是说iOS,其他平台我不晓得哈.)
1.4JSBinding和Native
// 两者各有各的优点, 那为什么不结合一下了
![](http://upload-images.jianshu.io/upload_images/571446-8aef863dbd968368.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160729_12.png
// 结合
![](http://upload-images.jianshu.io/upload_images/571446-e239c0ae6bedd619.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160729_2.png
[上面的图片来源 大城小胖的视频分享](http://v.youku.com/v_show/id_XNzExNzE1NTY4.html?from=s1.8-1-1.2)
- 
2.能做什么?
> 
1.我们可以通过javaScriptCore提供的接口,相对简单进行原生(OC,Swift)调用js代码
2.当然我们也可以js调用原生(OC,Swift)功能
~其实就是相互间的调用,在网页上做了一些操作然后想调起手机app原生的某些功能,比如:
①.我们有一个h5的一个商品列表页,然后点击了其中某个商品想进入商品详情页,而商品详情页是一个原生的ViewController,这就涉及的js调用了原生(OC, Swift); ----js->native
②.我们有一个详情页,后台给我们的是一个链接或者html的代码, 其中关于图片显示的问题, 后台设置的css样式显示的图片在我们手机端显示不正常,我们可以用调用js修改图片标签的css的样式,根据我们手机屏幕当前大小做出合适图片显示尺寸. ------native->js
- 
3.过往?
> 
1.没有javaScriptCore之前我们也可以进行原生与js之间的交互.但是必须依赖UIWebView(单纯的就依赖UIWebView着一点来说哈,我个人觉得说不说好坏,因为我觉得,原生和js之间的交互就是通过在h5上发生的一些操作嘛,要展示h5就离不开UIWebView嘛.).
> 
①.js调用原生: 通过实现自己协议URL,在UIWebView的代理,方法
`- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType`
根据request拿到请求的url来进行判断,如果url的协议是我自己制定的,那我们就进行js调用原生的相关操作. 比如:~从当前UIWebView控制器,跳转到其他控制器.
②.原生调用js
原生调用js,是通过webView调用其stringByEvaluatingJavaScriptFromString方法发送一个字符串,字符串里面是js代码如下.
```
NSString * domStr =
    @"var headDiv = document.getElementsByClassName('menu')[0];"
    "var myTitle = ['嘻嘻', '哈哈', '看看', '吱吱'];"
    "var myIndex = 0;"
    "for (var i = 0; i < headDiv.childNodes.length; i++){"
    "if(headDiv.childNodes[i].nodeName != '#text'){"
    "var myA = headDiv.childNodes[i];"
    "myA.innerText = myTitle[myIndex];"
    "myIndex++;"
    "}"
    "}"
    ;
    [self.webView stringByEvaluatingJavaScriptFromString:domStr];
```
- 木有javaScriptCore之前相互间的调用例子,修改纵横网的头部分类标题名称(native-->js), 点击纵横网头部分类标题弹出一个控制器.
```
#import "ViewController.h"
@interface ViewController ()<UIWebViewDelegate>
@property (weak, nonatomic) IBOutlet UIWebView *webView;
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    NSURLRequest * request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://m.zongheng.com/"]];
    [self.webView loadRequest:request];
}
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    NSString * domStr =
    @"var headDiv = document.getElementsByClassName('menu')[0];"
    "var myTitle = ['嘻嘻', '哈哈', '看看', '吱吱'];"
    "var myIndex = 0;"
    "for (var i = 0; i < headDiv.childNodes.length; i++){"
        "if(headDiv.childNodes[i].nodeName != '#text'){"
            "var myA = headDiv.childNodes[i];"
            "myA.innerText = myTitle[myIndex];"
            "myIndex++;"
        "}"
    "}"
    ;
    [webView stringByEvaluatingJavaScriptFromString:domStr];
}
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString * str = request.URL.absoluteString;
    NSLog(@"%@", str);
    // 当点击首页(上面被我们改成了~"嘻嘻")时我们弹出一个VC
    NSRange range = [str rangeOfString:@"h5/index?h5=1"];
    if (range.location != NSNotFound) {
        [self popToViewController];
    }
    return YES;
}
- (void)popToViewController
{
    UIViewController * vc = [[UIViewController alloc]init];
    vc.view.backgroundColor = [UIColor purpleColor];
    [self presentViewController:vc animated:YES completion:nil];
}
@end
```
### 上面的例子中我们没有自己定义url协议, 我直接根据纵横网的url来判断的, 其实关键在于能否做到js调用原生,至于是通过自定义url协议或者其他,在于你自己.
> 
2, javaScriptCore
使用javaScriptCore调用js不需要依赖UIWebView
javaScriptCore里有个JSContext可以直接执行js代码,下面是详细介绍~
# 二.javaScriptCore成员介绍
- 我们在我们的控制器里包含
`#import <JavaScriptCore/JavaScriptCore.h>`
```
// 进入框架头文件我们看见如下
#import "JSContext.h"
#import "JSValue.h"
#import "JSManagedValue.h"
#import "JSVirtualMachine.h"
#import "JSExport.h"
```
- 
--1--JSContext: JSContext代表JS的执行环境，通过JSContext对象调用evaluateScript方法就可以执行JS代码代. javaScript的所有变量方法都会在ISContext的保存,我们也可通过JSContext对象执行js代码生成新的js函数,js对象变量什么的.当然也可以通过JSContext执行相应的js拿到js里面的变量函数对象什么的.
- 
--2--JSValue 是JSContext执行js代码后得到的结果.JSValue是一个结果,是什么结果? 那就要看看JSContext执行的js代码了. JSValue可以是一个数字, 字符串, 对象, 函数等~当然这些"数字" ,"字符串"都是js类型我们可以通过JSValue的toXXXX, 将这些数据类型转换成OC类型的对应的数据类型.
--- js 和 OC 之间数据类型的转换如下图
![](http://upload-images.jianshu.io/upload_images/571446-3f5a9fe30a058769.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
[图片来源-----iderzheng的博客](http://blog.iderzheng.com/introduction-to-ios7-javascriptcore-framework/)
- --3--JSExport: JSExport是一个协议,不是一个对象哦!实现这个协议后,我们就能将原生的对象和类暴露给js.
> 
暴露给js干嘛?
作用大的去了! 我们把原生的一个对象暴露给js, js就可以调用原生对象里的方法,使用原生对象的属性. 把一个类暴露给js嘛,当然可以使用原生类的类方法喽,同时也可以在js里用原生的类生成原生的对象然后再调用对象的方法. JSExport我觉得真的好厉害的! 后面会详细介绍.
## JSManagedValue先不用看, 先看完第三部分相互调用再过来看这部分, 因为JSManagedValue涉及到js和原生之间的相互引用,内存什么的, 如果js和原生之间怎么调用的都不了, 看这部分有点困难, 如果了解的话那就来一起研究, 我也不知我下面说的对不对,大家一起研究哈, 如果错了请告诉我, 因为js 和 原生 之间的内存关系,有点难~ 需要大家一起研究.
- 
--4--JSManagedValue
- 有人说"JSManagedValue是JSValue的封装，用它可以解决js和原声代码之间循环引用的问题." 我觉得这样说有点不怎么好
// 因为 JSManagedValue他是继承NSObject的,最开始我还以为他是继承JSValue的哦,然而并不是, JSManagedValue它的功能是用来解决js和原生之间的循环引用的,这一点没错.
```
NS_CLASS_AVAILABLE(10_9, 7_0)
@interface JSManagedValue : NSObject
```
js与原生之间的循环引用?
就是原生的对象引用了js里的对象, js对象又对原生的对象引用.
例如说: 我们在Native 里面有的Person这么一个类(这个类必须遵守JSExport协议),这个类生成一个boy对象, 我们将boy这个类塞给jscontext, 然后在js里面用js里的一个girl对象对传进来的这个boy对象进行引用(具体这么引用? 具体就是:girl.boyFriend = boy;), 然后呢我们的boy也引用girl.(你girl对象都把boy对象赋值给了自己boyFriend属性,boy对象必须主动了so:boy.girlFriend = girl;)
但是我们如果向下图一样,互相强引用,倒时候就会造成内存泄漏
![](http://upload-images.jianshu.io/upload_images/571446-a2ad65ee5214bba1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160727_4.png
所以这样相互间的强引用是错错错滴XXXXXX
![](http://upload-images.jianshu.io/upload_images/571446-228a27b97da92e34.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160727_5.png
上图的代码体现如下------完整代码在下面
```
```
- (void)setGirlFriend:(JSValue *)girlFriend
{
    _girlFriend = girlfriend; 
}
```
```
上面的不行,那就用JSManagedValue, 如图我们用JSManagedValue搞一下,在引用的时候
![](http://upload-images.jianshu.io/upload_images/571446-0844e9f9836db46a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160727_3.png
代码体现如下------完整代码在下面
```
```
- (void)setGirlFriend:(JSValue *)girlFriend
{
    _girlFriend = (JSValue *)[JSManagedValue managedValueWithValue:girlFriend];
}
```
```
```
// 关于上面这个哈,我想说明一下哈
// 1.我Person.h里的"girlFriend"这个是下面这个
@property (nonatomic, strong)JSValue * girlFriend;
// 但是
[JSManagedValue managedValueWithValue:girlFriend];
// 上面这个的返回值是JSManagedValue,这样是可以的,到时候你取的时候知道这个属性是JSManagedValue就行了
// 你可能会说干嘛外面不直接这样
@property (nonatomic, strong) JSManagedValue * girlFriend;
// 这样是不行的,因为从js那边传过来的是JSValue,如果我这样,因为穿过来的是JSValue,而我们的是 JSManagedValue, 所以js那边赋值时根本进不了set方法
// js那边根本进不来 
- (void)setGirlFriend:(JSManagedValue *)girlFriend
{}
// 当然你也在JSExport里面不是暴露如下的这种属性
@property (nonatomic, strong)JSValue * girlFriend;
// 你可以暴露一个接受一个JSValue 参数的方法, 然后让js那边调用这个方法把girl传过来
```
// 点击查看视频资料
[上面的三个图和关于相互之间引用的例子思想,来源一个大神视频分享](http://v.youku.com/v_show/id_XNzExNzIxODI4.html?from=s1.8-1-1.2)
### js中的 girl 于 oc中的boy 之间的相互引用 和完整代码如下
## HTML(js~)--------"jsOCTest.html"
```xml
<html>
<head lang="en">
    <meta charset="UTF-8">
    <title></title>
</head>
<body>
<p></p>
<div id="myDiv"  style="background-color: red; width: 200px; height: 22px;"></div>
<button id="myBtn2">加属性用的</button>
<button id="mybtn">点我啊笨蛋</button>
<!---->
<script type="text/javascript">
    var boy;
    var btn = document.getElementById('mybtn');
var div = document.getElementById('myDiv'); 
    var btn2 = document.getElementById('myBtn2');
        function JsGirl(){
        this.boyFriend = "";
        this.name = "beauty girl";
        this.changeDivColor = function(){
                div.style.backgroundColor = "#333";
                }
        }               
     var jsGirl = new JsGirl(); 
        btn2.onclick = function(){
        alert("进来了!");
          jsGirl.boyFriend = boy;
          jsGirl.boyFriend.nickName = "pig";
          boy.girlFriend = jsGirl; 
         }
    btn.onclick = function(){
        console.log("你还真点啊!");
        alert("你还真点啊!");
        //boy.seyHello('xixi');
        //boy.seyHi([90,23,"22",{"key":"99"}]);
//      boy.gotoVcWithTitle("detailVcType", "I miss You!");
        jsGirl.boyFriend = boy;
        boy.girlFriend = jsGirl;        
        boy.girlFriend.changeDivColor();
        jsGirl.boyFriend.pay();
//      boy.gotoAVc("213123", "hi meirenji");  
//      boy = NULL;
//      boy.seyClassMethod();
//var testPerson = boy.myAllocInit();
        //var testPerson = boy.alloc().init();
        //testPerson.gotoAVc("213123", "hi meirenji");
        //alert(testPerson);
//      testPerson.gotoAVc("213123", "hi meirenji"); 
  }
    //btn.onclick = tt.seyHello();
</script>
</body>
</html>
```
## 原生(OC~)
person.h
```
//
//  Person.h
//  javaScriptCoreWebView
//
//  Created by Mac on 16/7/25.
//  Copyright © 2016年 wutong. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
@class Person;
@protocol PersonProtocol <JSExport>
- (void)pay;
- (void)seyHello;
- (void)seyHi:(NSArray *)arr;
- (void)gotoVcWith:(NSString *)typeStr title:(NSString *)titleStr;
+ (Person *)myAllocInit;
JSExportAs(gotoAVc,
           - (void)gotoActivityVcWithId:(NSString *)aId title:(NSString *)titleStr
           );
+ (void)seyClassMethod;
@property (nonatomic, strong)JSValue * girlFriend;
@property (nonatomic, strong)NSString *  nickName;
//+ (instancetype)alloc OBJC_SWIFT_UNAVAILABLE("use object initializers instead");
//- (instancetype)init;
@end
@interface Person : NSObject<PersonProtocol>
@property (nonatomic, strong)NSString * nickName;
@property (nonatomic, strong)JSValue * girlFriend;
- (void)pay;
- (void)seyHello;
- (void)seyHi:(NSArray *)arr;
- (void)gotoVcWith:(NSString *)typeStr title:(NSString *)titleStr;
- (void)gotoActivityVcWithId:(NSString *)aId title:(NSString *)titleStr;
+ (void)seyClassMethod;
+ (Person *)myAllocInit;
@end
```
## person.m
```
//
//  Person.m
//  javaScriptCoreWebView
//
//  Created by Mac on 16/7/25.
//  Copyright © 2016年 wutong. All rights reserved.
//
#import "Person.h"
@implementation Person
- (void)setGirlFriend:(JSValue *)girlFriend
{
    _girlFriend = (JSValue *)[JSManagedValue managedValueWithValue:girlFriend];
}
- (void)pay
{
    NSLog(@"付钱!!!");
}
- (void)setNickName:(NSString *)nickName
{
    _nickName = nickName;
}
//- (void)setNickName:(NSString *)nickName
//{
//}
- (void)seyHello
{
    NSLog(@"------helloh");
}
- (void)seyHi:(NSArray *)arr
{
    NSArray * testArr = [NSArray array];
    NSMutableArray * testMArr = [NSMutableArray array];
    NSLog(@"%@----M:%@", NSStringFromClass([testArr class]), NSStringFromClass([testMArr class]));
    NSLog(@"----%@", NSStringFromClass([arr class]));
    NSLog(@"hi-----%@", arr);
}
- (void)gotoVcWith:(NSString *)typeStr title:(NSString *)titleStr
{
    NSLog(@"typeStr:%@ -----titleStr:%@", typeStr, titleStr);
}
- (void)gotoActivityVcWithId:(NSString *)aId title:(NSString *)titleStr
{
    NSLog(@"活动id:%@-----活动title:%@", aId, titleStr);
}
+ (Person *)myAllocInit
{
    return [[Person alloc]init];
}
+ (void)seyClassMethod
{
    NSLog(@"hi, 我是类方法哦~");
}
- (void)dealloc
{
    NSLog(@"person-----挂了!");
}
@end
```
### VC调用
```
//
//  ViewController.m
//  javaScriptCoreWebView
//
//  Created by Mac on 16/7/25.
//  Copyright © 2016年 wutong. All rights reserved.
//
#import "ViewController.h"
#import "Person.h"
@interface ViewController ()<UIWebViewDelegate>
@property (weak, nonatomic) IBOutlet UIWebView *webView;
@property (nonatomic, strong)JSManagedValue * jsMValue;
@property (nonatomic, strong)JSContext * jsContext;
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
   NSURL * jsFileUrl = [[NSBundle mainBundle] URLForResource:@"jsOCTest.html" withExtension:nil];
    NSURLRequest * request = [NSURLRequest requestWithURL:jsFileUrl];
    Person * p = [[Person alloc]init];
    [self.webView loadRequest:request];
}
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    self.jsContext = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
    Person *jsObject = [[Person alloc]init];
    //self.jsContext[@"Person"] = [Person class];
//    self.jsContext[@"boy"] = jsObject;    //将对象注入这个context中
//    JSValue * value = (JSValue *)jsObject;
    self.jsContext[@"boy"] = jsObject;
    //JSValue * valueM = [self.jsContext evaluateScript:@"boy"];
//    
//    _jsMValue = [JSManagedValue managedValueWithValue:valueM];
//    [self.jsContext.virtualMachine addManagedReference:_jsMValue withOwner:self];
}
- (IBAction)callJsGrilDo:(id)sender {
    JSValue * pJsValue = self.jsContext[@"boy"];
    Person * person = [pJsValue toObject];
    JSManagedValue * jsMa = person.girlFriend;
    JSValue * jsV = jsMa.value;
    JSValue * kk = jsV[@"changeDivColor"];
    [kk callWithArguments:nil];
}
-(void)dealloc{
    NSLog(@"VC-----挂了!!!");
    if(_jsContext){
//        [_jsContext.virtualMachine removeManagedReference:_jsMValue withOwner:self];
        _jsContext[@"boy"] = nil;
        _jsContext = nil;
    }
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
@end
```
### 上的例子HTML的注释部分不用看,那是我们下面要讲js和native之间的相互调用要用的东西,针对性的看看js girl和 oc中Person的girlFriend的set方法部分就行了VC调用当然看看一步一执行那不部分就行了,其他的暂时不用看
### 关于js和native 的相互之间引用(不一定的是循环引用)
### 向上面那个如果不在js里把girl给boy, 仅仅是把boy给了girl, 仅仅是js引用了原生的一个对象boy如下图后面的代码, 按道理下面的jsObject这个局部变量应该过了}后就挂了, 但是没有,因为js引用了它.
```
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    self.jsContext = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
    Person *jsObject = [[Person alloc]init];
    self.jsContext[@"boy"] = jsObject;
}
```
## 没错上面说到jsObject之所以没有被释放是因为js引用了它,但是如果向下面这种结构, 点击button然后才进入我们的"调用VC", 当我们返回图中button VC时, 我们的js"调用VC"会被释放然后它里面的webView就会挂掉,然后webView的JSContent应该也是挂掉, 然后js引用的上面的jsObject应该也会挂掉,然后如果jsObject挂掉的话,那么就会调用Person的dealloc方法,但是我测试了没有dealloc方法. (希望你们能知道我在说什么.或者直接看我们demo), 这是为什么?????
![](http://upload-images.jianshu.io/upload_images/571446-4d2d8be997bc024d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160728_6.png
### 下面这几句话摘抄iderzheng的博客
[iderzheng的博客](http://blog.iderzheng.com/ios7-objects-management-in-javascriptcore-framework/)
```
不同内存管理机制—Reference Counting vs. Garbage Collection
虽然Objetive-C和JavaScript都是面向对象的语言，而且它们都可以让程序员专心于业务逻辑，不用担心内存回收的问题。但是两者的内存回首机制全是不同的，Objective-C是基于引用计数，之后Xcode编译器又支持了自动引用计数(ARC, Automatic Reference Counting)；JavaScript则如同Java/C#那样用的是垃圾回收机制(GC, Garbage Collection)。当两种不同的内存回收机制在同一个程序中被使用时就难免会产生冲突。
比如，在一个方法中创建了一个临时的Objective-C对象，然后将其加入到JSContext放在JavaScript中的变量中被使用。因为JavaScript中的变量有引用所以不会被释放回收，但是Objective-C上的对象可能在方法调用结束后，引用计数变0而被回收内存，因此JavaScript层面也会造成错误访问。
同样的，如果用JSContext创建了对象或者数组，返回JSValue到Objective-C，即使把JSValue变量retain下，但可能因为JavaScript中因为变量没有了引用而被释放内存，那么对应的JSValue也没有用了。
怎么在两种内存回收机制中处理好对象内存就成了问题。JavaScriptCore提供了JSManagedValue类型帮助开发人员更好地管理对象内存。
```
#### 好接着上面的那个问题,为什么那个jsObject没有被释放???
#### 我个人对于iderzheng中的两种内存管理机制的理解来解释哈,不知道对不对,如果你有正确的解释请你告诉我,留言或者私信我,谢谢!
### 我的解释是: 上面的jsObject在我们 的webViewDidFinishLoad方法里确实是一个局部变量嘛,然后局部变量出了{}就会被销毁,然后当原生的内存管理机制发现本来在{}以后引用计数器减一应该为0 了哈, 然后原生的内存管理就要去干掉jsObject,但是没有成功,应为jsObject被js给强引用了, js和原生的内存管理机制不同, 然后我感觉就是,原生说好吧既然你jsObject被js给引用了,那么我就不管jsObject你了, 然后js到底什么是收回jsObject的内存,那就得看js了,然后原生在收回一个对象是会给对象发送一个dealloc消息,然而js的回收机制会不会给jsObject发送dealloc消息了,那可不一定,再说了咱们上面的Person遵守的JSExport这个协议里面如果不把dealloc方法暴露,js也掉不了.所以jsObject是会被释放的但是什么时候被释放,咱们不得而知.
demo
- 
--5--JSVirtualMachine
- JSVirtualMachine是js的虚拟机,为JavaScript的运行提供了底层资源.
# 三.相互调用
- 
相互调用主要说的是js调用原生, 原生调用js
- 
我们不需要依赖webView,这一部分简单的来说,有JSValue和JSContext就行了.
// VC里基本的代码
```
#import "ViewController.h"
#import <JavaScriptCore/JavaScriptCore.h>
@interface ViewController ()
@property (nonatomic, strong)JSContext * jsContext;
@end
@implementation ViewController
- (JSContext *)jsContext{
    if (!_jsContext) {
        _jsContext = [[JSContext alloc]init];
    }
    return _jsContext;
}
- (void)viewDidLoad {
    [super viewDidLoad];
}
@end
```
#### 下面的调用案例都是基于上面基本代码来说的, 必须要导入#import <JavaScriptCore/JavaScriptCore.h>, 然后就是有一个JSContext * jsContext;
### 3.1原生调用 js
##### 3.1.1 简单加法
```
- (void)nativeCallJS_1
{
    JSValue * resultValue = [self.jsContext evaluateScript:@"1 + 2"];
    int32_t result = [resultValue toInt32];
    result--;
    NSLog(@"%zd", result);  //  2
}
```
数字加字符串等于字符串
```
- (void)nativeCallJS_1
{
    JSValue * resultValue = [self.jsContext evaluateScript:@"1 + '2' "];
    NSString * result = [resultValue toString];
    NSLog(@"%@", result); // @"12"
    NSLog(@"%d", result.length); // 2
}
```
##### 3.1.2 函数调用----------不传参数
// 调用js函数,返回普通的结果
```
- (void)nativeCallJS_func1
{
    // 给js注入一个函数test1, 让后调用test1
    JSValue * jsFunctionResult = [self.jsContext evaluateScript:
                             @"function test1(){"
                             "return 'xixi';"
                             "}"
                             "test1();"];
    // js函数返回的是字符串 'xixi',我们将结果用NSString接收
    NSString * result = [jsFunctionResult toString];
    NSLog(@"%@", result); // xixi
}
```
// 调用js函数, 返回函数 ---------------------------闭包1
```
- (void)nativeCallJS_func2
{
    // 给js注入一个函数test1, 然后返回的是这个函数----闭包
    JSValue * jsFunctionResult = [self.jsContext evaluateScript:
                             @"(function test1(){"
                             "return 'xixi';"
                             "})"
                              ];
    // 返回的是一个函数, 我们调用这个函数然后拿到函数的返回结果xixi
    // callWithArguments 后面要一个数组,这个数组是说test1如果需要参数我们调用时,参数就是放在数组中,后面会说~
    JSValue * jsFunctionStringReasult = [jsFunctionResult callWithArguments:nil];
    NSString * result = [jsFunctionStringReasult toString];
    NSLog(@"%@", result);
}
```
// 调用js函数, 返回函数 ---------------------------闭包2
```
- (void)nativeCallJS_func3
{
    // 执行一个闭包
    JSValue * jsFunctionResult = [self.jsContext evaluateScript:
                          @"function block3(){"
                          "var xixi = '(*^__^*) 嘻嘻……';"
                          "var lala = '♪(^∇^*)';"
                            "function block4(){"
                                "return xixi + lala;"
                            "}"
                          "return block4;"
                          "}"
                          "block3();"];
    JSValue * jsFunctionStringReasult = [jsFunctionResult callWithArguments:nil];
    NSString * result = [jsFunctionStringReasult toString];
    NSLog(@"%@", result);//(*^__^*) 嘻嘻……♪(^∇^*)
}
```
// 调用js函数---------------------------文件执行
//工程里新建一个test.js文件
// test.js文件里面代码如下
```php
```php
function blockSum(a,b){
    var num = 99;
    function blockS(){
        return a+b+num;
    }
    return blockS;
}
blockSum(5,6)();
```
```
// 调用
```
- (void)nativeCallJS_func4
{
    // 执行一个文件
    NSString *path = [[NSBundle mainBundle] pathForResource:@"test" ofType:@"js"];
    NSString *jsFileStr = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil];
    JSValue * jsValue3 = [self.jsContext evaluateScript:jsFileStr];
    int32_t result = [jsValue3 toInt32];
    NSLog(@"%d", result); // 110
}
```
// 调用js函数,--------------------------- 先注册后调用
```
- (void)nativeCallJS_func5
{
    // 注册一个js然后执行
    // 1.变量接收函数
    [self.jsContext evaluateScript:
     @"var hellow = function(){"
     "return 'hello';"
     "}"
     ];
    // 2.函数名
    [self.jsContext evaluateScript:
     @"function hi(){"
     "return 'hi';"
     "}"
     ];
    // 3.闭包函数体内
    [self.jsContext evaluateScript:
     @"function hiBlock(){"
         "var myHi = 'hi block';"
         "function sayHi(){"
            "return myHi;"
         "}"
         "return sayHi;"
     "}"
     ];
    //    // 4.闭包匿名?
    JSValue *jsFunction = [self.jsContext evaluateScript:
                           @"(function(){"
                           "return 'I am block too';"
                           "})"
                           ];
    JSValue * jsValueHello = [self.jsContext evaluateScript:@"hellow();"];
    NSLog(@"%@", jsValueHello);
    JSValue * jsValueHi = [self.jsContext evaluateScript:@"hi();"];
    NSLog(@"%@", jsValueHi);
    JSValue * jsValueSayHi = [self.jsContext evaluateScript:@"hiBlock()();"];
    NSLog(@"%@", jsValueSayHi);
    JSValue * jsFunctionValue = [jsFunction callWithArguments:nil];
    NSLog(@"%@", jsFunctionValue);
}
```
##### 3.1.3 函数调用----------传参数
```
// 传值几种情况
- (void)nativeCallJS_func6
{
    JSValue * sum = [self.jsContext evaluateScript:
                     @"(function sum(a, b){"
                        "return a + b;"
                     "})"
                     ];
    JSValue * result = [sum callWithArguments:@[@11, @19]];
    NSLog(@"%@",result);
    [self.jsContext evaluateScript:
                        @"function getInfo(){"
                           "var name = '小明';"
                            "function info(addr){"
                                "return name + '家住' + addr;"
                            "}"
                            "return info;"
                        "}"
                        ];
    JSValue * block2Result = [self.jsContext[@"getInfo"] callWithArguments:nil];
    JSValue * block2Result2 = [block2Result callWithArguments:@[@"北京"]];
    NSLog(@"%@", block2Result2);
    [self.jsContext evaluateScript:
                     @"function sumS(a, b){"
                     "return a + b;"
                     "}"
                     ];
    JSValue * result2 = [self.jsContext evaluateScript:@"sumS(33,67);"];
    NSLog(@"%@",result2);
}
```
##### 3.1.4 操作js中的变量
// 操作数组
```
- (void)nativeCallJS_value
{
    // 创建数组修改数组获得数组
    //1. 拿到js中的数组
    [self.jsContext evaluateScript:@"var arr = ['哈哈哈', '(*^__^*) 嘻嘻……'];"];
    JSValue * arrValue = self.jsContext[@"arr"];
    NSArray * arr = [arrValue toArray];
    NSLog(@"%@",arr);
    // 2.修改数组
    arrValue[1] = @"O(∩_∩)O哈哈哈~";
    arrValue[5] = @123; // js中没有下标5的. 没有的用空补齐
    NSLog(@"%@",arrValue);// 打印:  哈哈哈,O(∩_∩)O哈哈哈~,,,,123
    NSArray * arrChange = [arrValue toArray];
    NSLog(@"%@",arrChange);
    /*
     哈哈哈,
     O(∩_∩)O哈哈哈~,
     <null>,
     <null>,
     <null>,
     123
     )
     2,3,4下标以前不存在, 所用null代替
     */
}
```
// 操作js对象
```
// 获得对象, 调用对象方法
- (void)nativeCallJS_value_Obj
{
    [self.jsContext evaluateScript:
     @"function myObject(){"
     "var name = 'wutong';"
     "this.mylog = function(){"
     "return name;"
     "}"
     "}"
     "var myOj1 = new myObject();"
     ];
    // 拿现成的
//    JSValue * value1 = [self.jsContext evaluateScript:@"myOj1.mylog()"];
//    NSLog(@"%@", value1);
//    JSValue * value2 = [value1 callWithArguments:nil];
//    NSLog(@"---%@", value2);
    // 自己创建一个对象
    JSValue * value3 = [self.jsContext evaluateScript:@"(function(){var myOj2 = new myObject(); return myOj2;})()"];
    JSValue * value4 = [value3[@"mylog"] callWithArguments:nil];
    NSLog(@"--value4:%@", value4);
}
```
## 3.2 js 调用原生
#### 3.2.1 非对象调用
// block传入js, 当做js的函数(方法)调用
/ / 调用js方法然后回调到oc中, 虽然是在oc里面但是js方法里面的特性,比如参数不固定,在回调的block中依然是保留的.
// 使用JSContext 的+ (NSArray *)currentArguments方法获取传进来的所有参数
[解释文案来之--iderzheng博客](http://blog.iderzheng.com/introduction-to-ios7-javascriptcore-framework/)
```
- (void)jSCallNative
{
    // 在下面这个block中绝对不能使用self.jsContext
    self.jsContext[@"sum"] = ^(){
//        self.jsContext......  使用外部定义好的Context绝对不行,如果一定要使用Context,如下
        //
//        [JSContext currentContext]; /如果要使用就这样获取然后再使用
        NSArray * args = [JSContext currentArguments];
        int32_t result = 0;
        for (JSValue *jsVal in args) {
            NSLog(@"%@",jsVal);
            result = result + [jsVal toInt32];
        }
        NSLog(@"result: %d", result);
    };
    [self.jsContext evaluateScript:@"sum(1,2,4,3)"];
}
```
### 3.2.2 对象调用----------JSExport
###### ----3.2.2.1-----说明
JSExport 上面说了是一个协议嘛,这个协议是原生和js之间互通的.本身这个协议里面没有约定什么方法.
点进去看,果然毛都没有,除了一大段的注释
```
@protocol JSExport
@end
```
我们实现一个继承了JSExport协议的协议,这个协议里的方法就可以在js里面调用.废话不多说看看我的Person
```
//
//  Person.h
//  javaScriptCoreWebView
//
//  Created by Mac on 16/7/25.
//  Copyright © 2016年 wutong. All rights reserved.
//
#import <Foundation/Foundation.h>
#import <JavaScriptCore/JavaScriptCore.h>
@class Person;
@protocol PersonProtocol <JSExport>
- (void)pay;
- (void)seyHello;
- (void)seyHi:(NSArray *)arr;
- (void)gotoVcWith:(NSString *)typeStr title:(NSString *)titleStr;
+ (Person *)myAllocInit;
JSExportAs(gotoAVc,
           - (void)gotoActivityVcWithId:(NSString *)aId title:(NSString *)titleStr
           );
+ (void)seyClassMethod;
@property (nonatomic, strong)JSValue * girlFriend;
@property (nonatomic, strong)NSString *  nickName;
//+ (instancetype)alloc OBJC_SWIFT_UNAVAILABLE("use object initializers instead");
//- (instancetype)init;
@end
@interface Person : NSObject<PersonProtocol>
@property (nonatomic, strong)NSString * nickName;
@property (nonatomic, strong)JSValue * girlFriend;
- (void)pay;
- (void)seyHello;
- (void)seyHi:(NSArray *)arr;
- (void)gotoVcWith:(NSString *)typeStr title:(NSString *)titleStr;
- (void)gotoActivityVcWithId:(NSString *)aId title:(NSString *)titleStr;
+ (void)seyClassMethod;
+ (Person *)myAllocInit;
@end
```
恩恩~那啥,代码从上往下看哈,
- - PersonProtocol继承了JSExport,在PersonProtocol里面的属性啊方法,在js里面都可以调用.
- 2.有了PersonProtocol,然后在PersonProtocol 里面 也实现了些方法和属性什么的,怎么调用了在js里面, 我们需要一个载体Person类,Person遵守PersonProtocol,实际PersonProtocol里的方法啊属性啊什么的都是遵守PersonProtocol的某个类的,这个类就是Person.
- 3.到底要怎么调用了------>:生成一个person对象boy,然后将boy对象传入到JSContext. 然后在js环境里面就可以把传进来的boy对象当做js自己的对象一样调用方法(函数),使用属性.
- 4.其中有些细节,比如调用对象方法,调用类方法,没有参数, 一个参数,多个参数.
- 5.(注释掉的alloc和init方法)在js里面生成一个原生的类.怎么生成?
- 6.(JSExportAs------)给原生的方法生成一个---------绰号,然后在js里用绰号调用
#### 其实代码就是上面说JSManagedValue那三段代码,具体代码我在这里就不再贴出来了.
</>
###### ----3.2.2.1-----具体调用
我想对上面的6点挑几个详细说一下
// 1.把Person类的对象塞给JSContext上下文
// 这一点是最基础的,不把圆的对象给js, 要js怎么调用原生的嘛,代码如下
// 我这里是结合了webView 来说的, 所以我在在加载完成页面的时候把boy对象给webView的上下文
```
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    self.jsContext = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
    Person *boy = [[Person alloc]init];
    self.jsContext[@"boy"] =boy;
}
```
把对象boy给了js后,就可以在js那边调用原生的东西了
// 2.调用没有boy对象方法---------没有参数
// 我不把全部代码提出来了哈, 但我说明一下boy在js里全局变量,触发js调用是我们点击html 上面的btn. , btn我们通过dom方式已经拿到了,具体看上面html的代码哈, 下面的话我就直接用了不解释了哈.
```xml
<script type="text/javascript">
btn.onclick = function(){
        console.log("你还真点啊!");
        alert("你还真点啊!");
        boy.seyHello();
  }
</script>
```
// 3.调用没有boy对象方法---------一个参数
```xml
<script type="text/javascript">
btn.onclick = function(){
        console.log("你还真点啊!");
        alert("你还真点啊!");
    var arr = ['xixi', 'haha'];
        boy.seyHi(arr);
  }
</script>
```
// 4.调用没有boy对象方法---------多个参数
- 关于多个参数可以说一下了我们iOS里面的方法如果多个参数的话就是如下
```
- (void)gotoVcWithTypeStr:(NSString *)typeStr title:(NSString *)titleStr;
- (void)gotoVcWithId:(NSString *)idStr type:(NSString *)typeStr title:(NSString *)titleStr;
```
- 但是js调用boy的方法是按照js的调用函数式来的,它是不能像我在iOS里面那样的.
- 在js里面调用原生对象的多参方法,我们需要把对象的方法改造一下, 去掉":(XXX *)XXX" 拼接成一个函数名形式.......那个啥我的表达能力有限我不想讲一大通复杂的语言,说怎么改改.看代码吧.
// 上面两个多参方法要想在js里用boy对象调用应该修改为如下
```
gotoVcWithTypeStrTitle
gotoVcWithIdTypeTitle
去掉":(XXX *)XXX" 让后后面的方法名拼过来第一个字母大写
那么当boy调用就是如下
boy.gotoVcWithTypeStrTitle("typeStr", "titleStr");
boy.gotoVcWithIdTypeTitle("13231", "typeStr", "titleStr");/ /第一个是id,第二个是type,第三个是title
```
```xml
<script type="text/javascript">
btn.onclick = function(){
        console.log("你还真点啊!");
        alert("你还真点啊!");
    boy.gotoVcWithTitle("typeStr", "titleStr");
  }
</script>
```
// 其实如果你的参数多或者你赖的修改方法, 你可以给boy对象的多参方法,取一个绰号,然后在js用方法的绰号调用也是一样的,只要你参数顺序要对,一一对应.
// 在PersonProtocol协议里取绰号(别名)
```
@protocol PersonProtocol <JSExport>
- (void)gotoVcWith:(NSString *)typeStr title:(NSString *)titleStr;
JSExportAs(gotoAVc,
           - (void)gotoActivityVcWithId:(NSString *)aId title:(NSString *)titleStr
           );
@end
```
```xml
<script type="text/javascript">
btn.onclick = function(){
        console.log("你还真点啊!");
        alert("你还真点啊!");
    //boy.gotoVcWithTitle("typeStr", "titleStr");
        boy. gotoAVc("typeStr", "titleStr");// 和上面是一个意思
  }
</script>
```
#### 上面全部是说的js 调用对象的方法, 下面我们说说js调用类的方法
// 5.调用Person类------注册Person类
```
- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    self.jsContext = [webView valueForKeyPath:@"documentView.webView.mainFrame.javaScriptContext"];
    self.jsContext[@"Person"] = [Person class];
}
```
// 6.调用Person类------调用类方法
多参什么的和上面的规则一样
```xml
<script type="text/javascript">
btn.onclick = function(){
        console.log("你还真点啊!");
        alert("你还真点啊!");
    Person.seyClassMethod();
  }
</script>
```
// 7.调用Person类------在js里面创建对象
// 一般在js 里面创建对象是用构造函数,相当于iOS的类嘛, 我们刚才已经注册了一个Person类了(相当于构造函数),我们是不就可以像js那样new一个对象出来了.像下面这个
`var student = new Person();`
// 答案是否定!!!!!
/ / 我们这边是注册了一个类给js,但是js那边如果我们打印实际上Person还是一个对象,而不是一个函数, 说以如果我们需要在js里面创建Person的对象,我们需要下面两个步奏
- 步奏①
// 在PersonProtocol暴露 alloc 和init方法如下
```
@protocol PersonProtocol <JSExport>
+ (instancetype)alloc OBJC_SWIFT_UNAVAILABLE("use object initializers instead");
- (instancetype)init;
@end
```
- 步奏②
// 在js里的使用如下
```bash
var testPerson = Person.alloc().init();
    testPerson.gotoAVc("213123", "hi meirenji");
    alert(testPerson);
    testPerson.gotoAVc("213123", "hi meirenji");
```
# 四.异常处理
在js执行时如果遇到错误,会被JSContext捕获,并会存储在exception属性上,但是错误并不会向外面抛出. 我们可以设置JSContext对象的exceptionHandler. 是一个block
`@property (copy) void(^exceptionHandler)(JSContext *context, JSValue *exception);`
这样每当发生错误被JSContext捕获,然后赋值时,我们就可以打印一下如下
```php
```php
self.jsContext.exceptionHandler = ^(JSContext *context, JSValue *exception) {
        NSLog(@"%@", exception);
        context.exception = exception;
    };
    // 错误测试
    [self.jsContext evaluateScript:@"nibaba();"];
```
```
本文参考
[iOS7新JavaScriptCore框架入门介绍](http://blog.iderzheng.com/introduction-to-ios7-javascriptcore-framework/)
[JavaScriptCore初探](https://hjgitbook.gitbooks.io/ios/content/04-technical-research/04-javascriptcore-note.html)
[iOS开发:JavaScriptCore.framework的简单使用--JS与OC的交互篇](http://www.cnblogs.com/shaoting/p/5247208.html)
[大城小胖的视频分享----JSBinding、ios7_and_JavaScriptCore](http://v.youku.com/v_show/id_XNzExNzE1NTY4.html?from=s1.8-1-1.2)
## Demo下载
native调用js看
[javaScriptCoreDemo](https://github.com/coderTong/javaScriptCoreDemo)
js调用Native看
[javaScriptCoreDemo-WebView](https://github.com/coderTong/javaScriptCoreDemo-WebView)
