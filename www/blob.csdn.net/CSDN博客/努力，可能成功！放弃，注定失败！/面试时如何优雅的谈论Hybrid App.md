# 面试时如何优雅的谈论Hybrid App - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月05日 20:25:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1975
> 
重要的并不是我们提供的信息量有多大，而是我们能否给他们提供真正需要的信息。 -《瞬间之美》
本文涉及如下方面：
- 什么是Hybrid架构(可略读)
- Hybrid与Native的关系
- 设计上的考虑
- 一句话说明白
- 技术实现
- Cordova
#### Hybrid架构
> 
Hybrid App：Hybrid App is a mobile application that is coded in both browser-supported language and computer language. They are available through application distribution platforms such as the Apple App Store, Google Play etc. Usually, they are downloaded
 from the platform to a target device, such as iPhone, Android phone or Windows Phone. The subscribers need to install to run them.
![](http://upload-images.jianshu.io/upload_images/1170656-3b19957dce0c01d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Hybrid App（混合模式移动应用）是指介于web-app、native-app这两者之间的app，兼具“Native App良好用户交互体验的优势”和“Web App跨平台开发的优势”。Hybrid开发效率高、跨平台、低层本的特点，Hybrid从业务开发上讲，没有版本问题，对于BUG能及时修复。但Hybrid也是有缺点的，Hybrid体验比不上Native，同样的功能在性能上存在巨大差距。Web界面上JS对HTML Node的操作需要消耗大量的CPU资源，手机CPU的性能还不能与PC相提并论，就算在智能手机之间，硬件水准也参差不齐，一个可以在iPhone
 6上流畅运行的界面，跑到三星上很可能就卡住不动了。所以我们经常可以发现一些富交互页面上的操作无法达到令人满意的流畅度，而流畅度也正是用户评价一款App优劣的最直观因素。所以混合式编程比较适用于需要快速试错、快速占领市场的团队，当然，也不是绝对。
![](http://upload-images.jianshu.io/upload_images/1170656-2d23bdaa9e7a426a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Hybrid App按网页语言与程序语言的混合，通常分为三种类型：多View混合型，单View混合型，Web主体型。
多View混合型即Native View和Web View独立展示，交替出现。这种应用混合逻辑相对简单。即在需要的时候，将WebView当成一个独立的View（Activity）运行起来，在WebView内完成相关的展示操作。这种移动应用主体通常是Native App，Web技术只是起到补充作用。开发难度和Native App基本相当。
单View混合型即在同一个View内，同时包括Native View和Web View。互相之间是覆盖（层叠）的关系。这种Hybrid App的开发成本较高，开发难度较大，但是体验较好。如百度搜索为代表的单View混合型移动应用，既可以实现充分的灵活性，又能实现较好的用户体验。
Web主体型即移动应用的主体是Web View，主要以网页语言编写，穿插Native功能的Hybrid App开发类型。这种类型开发的移动应用体验相对而言存在缺陷，但整体开发难度大幅降低，并且基本可以实现跨平台。Web主体型的移动应用用户体验的好坏，主要取决于底层中间件的交互与跨平台的能力。国外的appMobi、PhoneGap和国内的WeX5、AppCan和Rexsee都属于Web主体型移动应用中间件。其中Rexsee不支持跨平台开发。appMobi和PhoneGap除基础的底层能力更多是通过**插件（Plugins）**扩展的机制实现Hybrid。AppCan除了插件机制，还提供了大量的单View混合型的接口来完善和弥补Web主体型Hybrid
 App体验差的问题，接近Native App的体验。而WeX5则在揉合PhoneGap和Bootstrap等主流技术的基础上，对性能进一步做了深度优化，不但完全具备Native App对本地资源的调用能力，性能体验也不输原生；WeX5所开发出来的app具备完全的跨端运行能力，可以无需任何修改直接运行在各种前端环境上。
多主体共存型（灵活型）是一种新型的开发模式，即支持Web主体型的应用，又支持以Native主体的应用，也支持两者混合的开发模式。比如kerkee框架 ，它具有跨平台、用户体验好、性能高、扩展性好、灵活性强、易维护、规范化、具有Debug环境、彻底解决跨域问题等特点。用户体验与Native App媲美。功能方面，开发者可随意扩展接口。
#### Hybrid与Native的关系
在Hybrid架构设计中Native提供的是一宿主环境，实现合理的逻辑架构需要考虑：交互设计，资源访问，Hybrid开发调试，通讯设计、并发设计、异常处理、日志监控以及安全模块等问题，前端要做的事情就是封装Native提供的各种能力。
处理交互设计时需要考虑前端与Native配合处理，如H5与Native的互相跳转逻辑；
资源访问机制需要做到既能以file的方式访问Native内部资源，又能使用url的方式访问线上资源，处理好跳转的逻辑；需要提供前端资源增量替换机制，以摆脱APP迭代发版问题，避免用户升级APP，这里将涉及到静态资源在APP中的存放策略，更新策略的设计，以及服务器端的支持等。当然，有现成框架和解决方案的支持，但这些确实是不可忽略的问题。
Hybrid的交互是通过Native调用前端页面的JS方法，或者前端页面通过JS调用Native提供的接口实现的，两者交互的桥梁是Webview，iOS可以使用javascripbrige来实现（现成框架），在UIWebView的shouldStartLoadWithRequest方法里面捕获url的变化，解析出需要的参数，然后传给一个统一的处理方法。app自身可以自定义url schema，并且把自定义的url注册在调度中心。JS与Native通信可以创建这类URL被Native捕获处理，即schema跳转逻辑。APP安装后会在手机上注册一个schema，比如淘宝是taobao://，Native会有一个进程监控Webview发出的所有schema://请求，然后分发到“控制器”。hybridapi处理程序，Native控制器处理时会需要param提供的参数（encode过），根据实际需要处理业务逻辑问题。
#### 设计上的考虑
无论在Android还是iOS平台上，都有各自的一套交互习惯，包括视觉风格，界面切换，操作习惯等，与Web习惯完全不同。以iOS界面切换为例，系统风格是新界面自右向左推入，后退时界面自左向右推出，而旧界面保持状态。Web开发的默认习惯则是刷新页面，无论新载入页面或是后退，都会对页面进行刷新。因此使用Web模式开发多界面功能就面临这样的交互习惯差异，造成体验上的损失。当然Web方式也可模拟Native的交互方式，但这样的模拟首先提高了开发成本，有悖于最初的高效原则，从效果上看，也很难达到Native的流畅性。
#### 一句话说明白
如果用一句话来讲明白这个问题，就是Hybrid混合式编程可以在Native原生应用的基础上通过一些Hybrid框架实现本地和JS的交互处理。在不影响性能的情况下，实现应用的动态化。
#### 技术实现原理
Hybrid技术将静态资源存于Native，目录结构可以如下面划分，当然不是绝对的，基本要求是将对应版本的Hybrid插件下载到本地，存放方便就好。
![](http://upload-images.jianshu.io/upload_images/1170656-17f9fb8918c3d88c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
在正式的开发中，还要考虑增量机制，因为不同的版本会对应着不同的业务逻辑，真实的增量机制需要服务器端的配合，版本映射表是每次大版本APP发布时由服务器端生成。当APP启动时，APP会读取版本信息，如果发现本地版本号比线上的小，便会下载对应的zip文件，然后解压之后并且替换整个原有的文件，本次增量结束，因为所有的版本文件不会重复，APP回滚时可用回到任意想去的版本，也可以对任意版本做BUG修复。
以上，我们得知，Hybrid和Native交互可以通过两种方式：Url Schema和JavaScriptCore等地交互。H5与Native交互的桥梁为Webview，“联系”的方式以url schema的方式，在用户安装app后，app可以自定义url schema，并且把自定义的url注册在调度中心。
`demoschema://hybridapi?callback=hybrid_1446276509894¶m=%7B%22data1%22%3A1%2C%22data2%22%3A2%7D`
App调用H5:将一组API绑定在webView的window对象上，App通过iOS原生方法调用window对象中的js方法。
H5与App通信：App实现对Webview URL的观察者模式，H5 通过改变URL的哈希值，通过解析哈希值的变化执行相应的操作。
另一种是JavaScriptCore，具体的可以看之前写的文章[JavaScriptCore框架](http://www.jianshu.com/p/bbfa8dee967e)。
```cpp
```cpp
context[@"makeUIColor"] = ^(NSDictionary *rgbColor){
float red = [rgbColor[@"red"] floatValue];
float green = [rgbColor[@"green"] floatValue];
float blue = [rgbColor[@"blue"] floatValue];
return [UIColor colorWithRed:(red / 255.0)
                   green:(green / 255.0)
                    blue:(blue / 255.0)
                   alpha:1];
};
JSValue *color = [context evaluateScript:@"makeUIColor({red: 50, green: 150, blue: 250})"];
NSLog(@"color:%@",[color toObject]);
```
```
通过这种方式就可以在JavaScript中调用Objective-C。
当然，除此之外也可以用JSBridge技术实现。
JS中：
```java
```java
button.onclick = function(e) {
    e.preventDefault();
    var data = 'Hello from JS button';
    log('JS sending message', data);
    window.external.test1(data, 'haha');
}
```
```
通过JSBridge,执行本地类中的test1方法。此处只是一个引子，详细的会在另一篇文章中单独引出。
至于H5和Native的跳转问题，可以采用统跳协议来完成。这又是另外一个问题了。当然也可以在JS里定义跳转地址，通过JS交互来处理。
#### Cordova
写了那么久，感觉现在才进入主题。
Cordova提供了一组设备相关的API，通过这组API，移动应用能够以JavaScript访问原生的设备功能，如摄像头、麦克风等。Cordova还提供了一组统一的JavaScript类库，以及为这些类库所用的设备相关的原生后台代码。
测试使用可以到Github上下载[源码](https://github.com/apache/cordova-ios),也可以使用命令：
```
```
cordova create hello com.example.hello HelloWorld
cd hello
cordova platform add ios
```
```
这样就创建了hello文件夹下的测试目录(com.example.hello 为应用程序的id名，与Xcode中类似，可以在 config.xml 中修改，如果不指定的话默认为 io.cordova.hellocordova)。
> 
目录文件说明：
config.xml ：cordova的配置文件
hooks/ ：存放自定义cordova命令的脚本文件。
platforms/ ：各个平台原生工程代码，会在build时被覆盖勿修改
plugins/ ：插件目录（主要是提供各个平台的原生API）
www/ ：用H5编写的源代码目录，build时会被放入各个平台的assets\www目录。
www/index.html ：App入口html文件
在Xcode编译运行的话，使用的是 Staging 下面的html页面。
命令安装需要安装cordova命令
```
brew install node //该命令执行后，自动装好node和npm。
brew upgrade node //更新node(可不执行)
```
为了解决校验码不正确问题。npm 使用国内镜像的方法 （三种办法任意一种都能解决问题，建议使用第三种，将配置写死，下次用的时候配置还在）:
```cpp
```cpp
1.通过config命令
npm config set registry http://registry.cnpmjs.org npm info underscore （如果上面配置正确这个命令会有字符串response）
2.命令行指定
npm --registry http://registry.cnpmjs.org info underscore 
3.编辑 ~/.npmrc 加入下面内容
registry = http://registry.cnpmjs.org
```
```
之后：
``sudo npm install -g cordova``
打开platform/ios里的HelloWorld.xcodeproj即可测试。
下面就是源码分析了，因为要对照着源码来说，大家可以到[这里](https://github.com/dangdangfeng/Cordova-Source-Code-Analyse)，参考详细的源码分析。通过这里，写一个混合式App应该不是问题了。
最后，祝大家看的愉快！
文／吴白（简书作者）
原文链接：http://www.jianshu.com/p/e83aa2d1ade3
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
