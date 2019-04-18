# iOS js与objective-c的交互(转) - z69183787的专栏 - CSDN博客
2015年08月28日 15:06:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：951
在写 JavaScript 的时候，可以使用一个叫做 window 的对象，像是我们想要从现在的网页跳到另外一个网页的时候，就会去修改 window.location.href 的位置；在我们的 Objective-C 程序码中，如果我们可以取得指定的 WebView 的指标，也就可以拿到这个出现在 JavaScript 中的 window 对象，也就是 [webView windowScriptObject]。
这个对象就是 WebView 里头的 JS 与我们的 Objective-C程序之间的桥樑－window 对象可以取得网页里头所有的 JS 函数与对象，而如果我们把一个 Objective-C 对象设定成 windowScriptObject 的 value，JS 也便可以调用Objective-C对象的 method。于是，我们可以在Objective-C 程序里头要求 WebView 执行一段 JS，也可以反过来让 JS 调用一段用 Obj C 实作的功能。
**※ 用Objective-C 取得与设定JavaScript 对象**
要从 Objective-C取得网页中的 JavaScript 对象，也就是对 windowScriptObject 做一些 KVC 调用，像是 valueForKey: 与 valueForKeyPath:。如果我们在 JS 里头，想要知道目前的网页位置，会这么写：
　　var location = window.location.href;
用 Objective-C 就可以这么调用：
　　NSString *location = [[webView windowScriptObject] valueForKeyPath:@"location.href"];
如果我们要设定 window.location.href，要求开启另外一个网页，在 JS 里头：
　　window.location.href = 'http://spring-studio.net';
在Objective-C：
　　[[webView windowScriptObject] setValue:@"http://spring-studio.net"forKeyPath:@"location.href"];
由于Objective-C 与 JS 本身的语言特性不同，在两种语言之间相互传递东西之间，就可以看到两者的差别：
- JS 虽然是 OO，但是并没有 class，所以将 JS 对象传到 Obj C 程序里头，除了基本字串会转换成 NSString、基本数字会转成 NSNumber，像是 Array 等其他对象，在 Objective-C 中，都是 WebScriptObject 这个 Class。意思就是，JS 的 Array 不会帮你转换成 NSArray。
- 从 JS 里头传一个空对象给 Objective-C 程序，用的不是 Objective-C 里头原本表示「没有东西」的方式，像是 NULL、nil、NSNull 等，而是专属 WebKit 使用的 WebUndefined。
所以，如果我们想要看一个 JS Array 里头有什麽东西，就要先取得这个对象里头叫做 length 的 value，然后用 webScriptValueAtIndex: 去看在该 index 位置的内容。
假如我们在 JS 里头这样写：
var JSArray = {'zonble', 'dot', 'net'};
for (var i = 0; i < JSArray.length; i++) {
　　console.log(JSArray[i]);
}
在Objective-C 里头就会变成这样：
WebScriptObject *obj = (WebScriptObject *)JSArray;
NSUInteger count = [[obj valueForKey:@"length"] integerValue];
NSMutableArray *a = [NSMutableArray array];
for (NSUInteger i = 0; i < count; i++) {
    NSString *item = [obj webScriptValueAtIndex:i];
    NSLog(@"item:%@", item);
}
**※ 用Objective C 调用 JavaScript function**
要用 Objective-C 调用网页中的 JS function，大概有几种方法。第一种是直接写一段跟你在网页中会撰写的 JS 一模一样的程序，叫 windowScriptObject 用 evaluateWebScript: 执行。
例如，我们想要在网页中产生一个新的 JS function，内容是：
function x(x) {
    return x + 1;
}
所以在 Objective-C 中可以这样写；
[[webView windowScriptObject] evaluateWebScript:@"function x(x) { return x + 1;}"];
接下来我们就可以调用 window.x()：
NSNumber *result = [[webView windowScriptObject] evaluateWebScript:@"x(1)"];
NSLog(@"result:%d", [result integerValue]); // Returns 2
由于在 JS 中，每个 funciton 其实都是对象，所以我们还可以直接取得 window.x 叫这个对象执行自己。
在 JS 里头如果这样写：
window.x.call(window.x, 1);
Objective-C 中便是这样：
WebScriptObject *x = [[webView windowScriptObject] valueForKey:@"x"];
NSNumber *result = [x callWebScriptMethod:@"call"withArguments:[NSArray arrayWithObjects:x, [NSNumbernumberWithInt:1], nil]];
这种让某个 WebScriptObject 自己执行自己的写法，其实比较不会用于从 Objective-C 调用 JS 这一端，而是接下来会提到的，由 JS 调用 Objective-C，因为这样 JS 就可以把一个 callback function 送到 Objective-C 程序里头。
如果我们在做网页，我们只想要更新网页中的一个区块，就会利用 AJAX 的技巧，只对这个区块需要的资料，对 server 发出 request，并且在 request 完成的时候，要求执行一段 callback function，更新这一个区块的显示内容。从 JS 调用 Objective-C也可以做类似的事情，如果 Objective-C程序里头需要一定时间的运算，或是我们可能是在 Objective-C 里头抓取网路资料，我们便可以把一个 callback function 送到 Objective-C程序里，要求Objective-C程序在做完工作后，执行这段
 callback function。
**※DOM**
WebKit 里头，所有的 DOM 对象都继承自 DOMObject，DOMObject 又继承自 WebScriptObject，所以我们在取得了某个 DOM 对象之后，也可以从 Objective-C 程序中，要求这个 DOM 对象执行 JS 程序。
假如我们的网页中，有一个 id 叫做 “#s” 的文字输入框（text input），而我们希望现在键盘输入的焦点放在这个输入框上，在 JS 里头会这样写：
document.querySelector('#s').focus();
在Objective-C中写法：
DOMDocument *document = [[webView mainFrame] DOMDocument];
[[document querySelector:@"#s"] callWebScriptMethod:@"focus"withArguments:nil];
**※ 用 JavaScript 存取 Objective C 的 Value**
要让网页中的 JS 程序可以调用 Objective-C 对象，方法是把某个 Objective-C 对象注册成 JS 中 window 对象的属性。之后，JS 便也可以调用这个对象的 method，也可以取得这个对象的各种 Value，只要是 KVC 可以取得的 Value，像是 NSString、NSNumber、NSDate、NSArray、NSDictionary、NSValue…等。JS 传 Array 到 Objective-C 时，还需要特别做些处理才能变成 NSArray，从 Obj C 传一个
 NSArray 到 JS 时，会自动变成 JS Array。
首先我们要注意的是将 Objective-C 对象注册给 window 对象的时机，由于每次重新载入网页，window 对象的内容都会有所变动－毕竟每个网页都会有不同的 JS 程序，所以，我们需要在适当的时机做这件事情。我们首先要指定 WebView 的 frame loading delegate（用 setFrameLoadDelegate:），并且实作 webView:didClearWindowObject:forFrame:，WebView 只要更新了 windowScriptObject，就会调用这一段程序。
假如我们现在要让网页中的 JS 可以使用目前的 controller 对象，会这样写：
- (void)webView:(WebView *)sender didClearWindowObject:(WebScriptObject *)windowObject forFrame:(WebFrame *)frame
{
    [windowObject setValue:self forKey:@"controller"];
}
如此一来，只要调用 window.controller，就可以调用我们的 Objective-C 对象。假如我们的 Objective-C Class 里头有这些成员变数：
@interface MyController : NSObject
{
    IBOutlet WebView *webView;
    IBOUtlet  NSWindow *window;
    NSString *stringValue;
    NSInteger numberValue;
    NSArray *arrayValue;
    NSDate *dateValue;
    NSDictionary *dictValue;
    NSRect frameValue;
}
@end
指定一下 Value：
stringValue = @"string";
numberValue = 24;
arrayValue = [[NSArray arrayWithObjects:@"text", [NSNumbernumberWithInt:30], nil] retain];
dateValue = [[NSDate date] retain];
dictValue = [[NSDictionary dictionaryWithObjectsAndKeys:@"value1",@"key1", @"value2", @"key2", @"value3", @"key3", nil] retain];
frameValue = [window frame];
用 JS 读读看：
var c = window.controller;
var main = document.getElementById('main');
var HTML = '';
if (c) {
    HTML += '<p>' + c.stringValue + '<p>';
    HTML += '<p>' + c.numberValue + '<p>';
    HTML += '<p>' + c.arrayValue + '<p>';
    HTML += '<p>' + c.dateValue + '<p>';
    HTML += '<p>' + c.dictValue + '<p>';
    HTML += '<p>' + c.frameValue + '<p>';
    main.innerHTML = HTML;
}
结果如下：
string 24 text,30 2010-09-09 00:01:04 +0800 { key1 = value1; key2 = value2; key3 = value3; } NSRect: {{275, 72}, {570, 657}}
不过，如果你看完上面的范例，就直接照做，应该不会直接成功出现正确的结果，而是会拿到一堆 undefined，原因是，Objective-C 对象的 Value 预设被保护起来，不会让 JS 直接存取。要让 JS 可以存取 Objective-C 对象的 Value，需要操作 +isKeyExcludedFromWebScript: 针对传入的 Key 一一处理，如果我们希望 JS 可以存取这个 key，就回传 NO：
+ (BOOL)isKeyExcludedFromWebScript:(const char *)name
{
    if (!strcmp(name, "stringValue")) {
        return NO;
    }
    return YES;
}
除了可以读取 Objective-C对象的 Value 外，也可以设定 Value，相当于在 Objective-C中使用 setValue:forKey:，如果在上面的 JS 程序中，我们想要修改 stringValue，直接调用 c.stringValue = ‘new value’ 即可。像前面提到，在这裡传给 Objective-C的 JS 对象，除了字串与数字外，class 都是 WebScriptObject，空对象是 WebUndefined。
**※用 JavaScript调用 Objective C method**
Objective-C 的语法沿袭自 SmallTalk，Objective-C 的 selector，与 JS 的 function 语法有相当的差异。WebKit 预设的实事是，如果我们要在 JS 调用 Objective-C selector，就是把所有的参数往后面摆，并且把所有的冒号改成底线，而原来 selector 如果有底线的话，又要另外处理。
假使我们的 controller 对象有个 method，在 Objective-C 中写成这样：
- (void)setA:(id)a b:(id)b c:(id)c;
在 JS 中就这么调用：
controller.setA_b_c_('a', 'b', 'c');
实在有点丑。所以 WebKit 提供一个方法，可以让我们把某个 Objective-C selector 变成好看一点的 JS function。我们要实作 webScriptNameForSelector:
+ (NSString *)webScriptNameForSelector:(SEL)selector
{
    if (selector == @selector(setA:b:c:)) {
        return @"setABC";
    }
    return nil;
}
以后就可以这么调用：
controller.setABC('a', 'b', 'c');
我们同样可以决定哪些 selector 可以给 JS 使用，哪些要保护起来，方法是实作 isSelectorExcludedFromWebScript:。而我们可以改变某个 Objective-C selector 在 JS 中的名称，我们也可以改变某个 value 的 key，方法是实作 webScriptNameForKey:。
有几件事情需要注意一下：
用 JavaScript 调用 Objective C 2.0 的 property
在上面，我们用 JS 调用 window.controller.stringValue，与设定里头的 value 时，这边很像我们使用 Objective-C 2.0 的语法，但其实做的是不一样的事情。用 JS 调用 controller.stringValue，对应到的 Objective-C 语法是 [controller valueForKey:@"stringValue"]，而不是调用 Objective-C 对象的 property。
如果我们的 Objective-C 对象有个 property 叫做 stringValue，我们知道，Objective-C property 其实会在编译时，变成 getter/setter method，在 JS 里头，我们便应该要调用 controller.stringValue() 与 controller.setStringValue_()。
Javascript 中，Function 即对象的特性
JS 的 function 是对象，当一个 Objective-C 对象的 method 出现在 JS 中时，这个 method 在 JS 中，也可以或多或少当做对象处理。我们在上面产生了 setABC，也可以试试看把它倒出来瞧瞧：
console.log(controller.setABC);
我们可以从结果看到：
function setABC() { [native code] }
这个 function 是 native code。因为是 native code，所以我们无法对这个 function 调用 call 或是 apply。
另外，在把我们的 Objective-C 对象注册成 window.controller 后，我们会许也会想要让 controller 变成一个 function 来执行，像是调用 window.controller()；或是，我们就只想要产生一个可以让 JS 调用的 function，而不是整个对象都放进 JS 里头。我们只要在 Objective-C 对象中，实作 invokeDefaultMethodWithArguments:，就可以回传在调用 window.controller() 时想要的结果。
现在我们可以综合练习一下。前面提到，由于我们可以把 JS 对象以 WebScriptObject 这个 class 传入 Obj C 程序，Objective-C 程序中也可以要求执行 WebScriptObject 的各项 function。我们假如想把 A 与 B 两个数字丢进 Objective-C 程序里头做个加法，加完之后出现在网页上，于是我们写了一个 Objective-C method：
- (void)numberWithA:(id)a plusB:(id)b callback:(id)callback
{
    NSInteger result = [a integerValue] + [b integerValue];
    [callback callWebScriptMethod:@"call" withArguments:[NSArrayarrayWithObjects:callback, [NSNumber numberWithInteger:result],nil]];
}
JS 里头就可以这样调用：
window.controller.numberWithA_plusB_callback_(1, 2,function(result) {
    var main = document.getElementById('main');
    main.innerText = result;
});
**※其他平台上 WebKit的用法**
除了 Mac OS X，WebKit 这几年也慢慢移植到其他的作业系统与 framework 中，也或多或少都有 Native API 要求 WebView 执行 Js，以及从 JS 调用 Native API 的机制。
跟 Mac OS X 比较起来，IOS 上 UIWebView 的公开 API 实在少上许多。想要让 UIWebView 执行一段 JS，可以透过调用 stringByEvaluatingJavaScriptFromString:，只会回传字串结果，所以能够做到的事情也就变得有限，通常大概就拿来取得像 window.title 这些资讯。在 IOS 上我们没办法将某个 Objective-C 对象变成 JS 对象，所以，在网页中触发了某些事件，想要通知 Objective-C 这一端，往往会选择使用像「zonble://」这类
 Customized URL scheme。
ChromeOS 完全以 WebKit 製作使用者介面，不过我们没办法在 ChomeOS 上写我们在这边所讨论的桌面或行动应用程序，所以不在我们讨论之列。（顺道岔题，ChromeOS 是设计来给 Netbook 使用的作业系统，可是像 Toshiba 都已经用 Android，做出比 Netbook 更小的 Smartbook，而且应用程序更多，ChromeOS 的产品做出来的话，实在很像 Google 拿出两套东西，自己跟自己对打）。
Android 的 WebView 对象提供一个叫做 addJavascriptInterface() 的 method，可以将某个 Java 对象注册成 JS 的 window 对象的某个属性，就可以让 JS 调用 Java 对象。不过，在调用 Java 对象时，只能够传递简单的文字、数字，複杂的 JS 对象就没办法了。而在 Android 上想要 WebView 执行一段 JS，在文件中没看到相关资料，网路上面找到的说法是，可以透过 loadUrl()，把某段 JS 用 bookmarklet 的形式传进去。
在 QtWebKit 里头，可以对 QWebFrame 调用 addToJavaScriptWindowObject，把某个 QObject 暴露在 JS 环境中，我不清楚 JS 可以传递哪些东西到 QObject 里头就是了。在 QtWebKit 中也可以取得网页里头的 DOM 对象（QWebElement
、QWebElementCollection），我们可以对 QWebFrame 还有这些 DOM 对象调用 evaluateJavaScript，执行 Javascript。
GTK 方面，因为是 C API，所以在应用程序与 JS 之间，就不是透过操作包装好的对象，而是调用 WebKit 里头 JavaScript Engine 的 C API。
**※ JavaScriptCore Framework**
我们在 Mac OS X 上面，也可以透过 C API，要求 WebView 执行 Javascript。首先要 import 。如果我们想要简单改一下 window.location.href：
JSGlobalContextRef globalContext = [[webView mainFrame] globalContext];
JSValueRef exception = NULL;
JSStringRef script = JSStringCreateWithUTF8CString("window.location.href='http://spring-studio.net'");
JSEvaluateScript(globalContext, script, NULL, NULL, 0, &exception);
JSStringRelease(script);
如果我们想要让 WebView 里头的 JS，可以调用我们的 C Function：
- (void)webView:(WebView *)sender didClearWindowObject:(WebScriptObject *)windowObject forFrame:(WebFrame *)frame
{
    JSGlobalContextRef globalContext = [frame globalContext];
    JSStringRef name = JSStringCreateWithUTF8CString("myFunc");
    JSObjectRef obj = JSObjectMakeFunctionWithCallback(globalContext, name, (JSObjectCallAsFunctionCallback)myFunc);
    JSObjectSetProperty (globalContext, [windowObject JSObject], name, obj, 0, NULL);
    JSStringRelease(name);
}
那麽，只要 JS 调用 window.myFunc()，就可以取得们放在 myFunc 这个 C function 中回传的结果：
JSValueRef myFunc(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    return JSValueMakeNumber(ctx, 42);
}
