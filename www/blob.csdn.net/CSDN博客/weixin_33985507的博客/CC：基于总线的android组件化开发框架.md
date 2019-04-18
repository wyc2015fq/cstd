# CC：基于总线的android组件化开发框架 - weixin_33985507的博客 - CSDN博客
2017年12月09日 07:47:42[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
*本篇文章已授权微信公众号 guolin_blog （郭霖）独家发布
CC：Component Caller，一个android组件化开发框架， 已开源，github地址：[github.com/luckybilly/…](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC)
本文主要讲解框架实现原理，如果只是想了解一下如何使用，可直接到github上查看README文档
# 前言
首先说明一下，本文将讲述的组件化与业内的插件化（如：Atlas, RePlugin等）不是同一个概念
【图片来源于网络】
组件化开发：就是将一个app分成多个Module，每个Module都是一个组件(也可以是一个基础库供组件依赖)，开发的过程中我们可以单独调试部分组件，组件间不需要互相依赖，但可以相互调用，最终发布的时候所有组件以lib的形式被主app工程依赖并打包成1个apk。
插件化开发：和组件化开发略有不用，插件化开发时将整个app拆分成很多模块，这些模块包括一个宿主和多个插件，每个模块都是一个apk（组件化的每个模块是个lib），最终打包的时候将宿主apk和插件apk(或其他格式)分开或者联合打包。
**本文将主要就以下几个方面进行介绍：**
一、为什么需要组件化？
二、CC的功能介绍
三、CC技术要点
四、CC执行流程详细解析
五、使用方式介绍
六、老项目进行组件化改造的成本有多高？
# 一、为什么需要组件化？
关于使用组件化的理由，上网能搜到很多，如业务隔离、单独以app运行能提高开发及调试效率等等这里就不多重复了，我补充一条：组件化之后，我们能很容易地实现一些组件层面的AOP，例如：
- 轻易实现页面数据(网络请求、I/O、数据库查询等)预加载的功能
- 组件被调用时，进行页面跳转的同时异步执行这些耗时逻辑
- 页面跳转并初始化完成后，再将这些提前加载好的数据展示出来
- 在组件功能调用时进行登录状态校验
- 借助拦截器机制，可以动态给组件功能调用添加不同的中间处理逻辑
# 二、CC的功能介绍
- 支持组件间相互调用（不只是Activity跳转，支持任意指令的调用/回调）
- 支持组件调用与Activity、Fragment的生命周期关联
- 支持app间跨进程的组件调用(组件开发/调试时可单独作为app运行)
- 在独立运行组件时非常有用，比如：一个组件的某个功能要用到用户的登录信息，若未登录则调起登录组件的登录页面，若已登录则获取当前用户信息。此时可以直接使用主app中的登录组件及用户在主app中的登录状态，该组件作为app独立运行时无需依赖登录组件，可以始终保持独立运行状态进行开发。
- 支持app间调用的开关及权限设置（满足不同级别的安全需求，默认打开状态且不需要权限）
- 支持同步/异步方式调用
- 支持同步/异步方式实现组件
- 调用方式不受实现方式的限制（例如:可以异步调用另一个组件的同步实现功能。注：不要在主线程同步调用耗时操作）
- 支持添加自定义拦截器（按添加的先后顺序执行）
- 支持超时设置
- 支持手动取消
- 编译时自动注册组件(IComponent)，无需手动维护组件注册表(使用ASM修改字节码的方式实现)
- 支持动态注册/反注册组件(IDynamicComponent)
- 支持组件间传递Fragment等非基础类型的对象（组件在同一个app内时支持、跨app传递非基础类型的对象暂不支持）
- 尽可能的解决了使用姿势不正确导致的crash：
- 组件调用处、回调处、组件实现处的crash全部在框架内部catch住
- 同步返回或异步回调的CCResult对象一定不为null，避免空指针，同时可以根据CCResult的结果进行降级处理。
### demo效果演示
组件A打包在主app中，组件B为单独运行的组件app，下图演示了在主app中调用两者的效果，并将结果以Json的格式显示在下方。[demo下载地址](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC%2Fblob%2Fmaster%2FREADME.md)）：
# 三、 CC技术要点
实现CC组件化开发框架主要需要解决的问题有以下几个方面：
- 组件如何自动注册？
- 如何兼容同步/异步方式调用组件？
- 如何兼容同步/异步方式实现组件？
- 如何跨app调用组件？
- 组件如何更方便地在application和library之间切换？
- 如何实现startActivityForResult？
- 如何阻止非法的外部调用？
- 如何与Activity、Fragment的生命周期关联起来
### 3.1 组件如何自动注册？
为了减少后期维护成本，想要实现的效果是：当需要添加某个组件到app时，只需要在gradle中添加一下对这个module的依赖即可（通常都是maven依赖，也可以是project依赖）
最初想要使用的是annotationProcessor通过编译时注解动态生成组件映射表代码的方式来实现。但尝试过后发现行不通，因为编译时注解的特性只在源码编译时生效，无法扫描到aar包里的注解（project依赖、maven依赖均无效），也就是说必须每个module编译时生成自己的代码，然后要想办法将这些分散在各aar种的类找出来进行集中注册。
ARouter的解决方案是：
- 每个module都生成自己的java类，这些类的包名都是'com.alibaba.android.arouter.routes'
- 然后在运行时通过读取每个dex文件中的这个包下的所有类通过反射来完成映射表的注册，详见[ClassUtils.java源码](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Falibaba%2FARouter%2Fblob%2Fmaster%2Farouter-api%2Fsrc%2Fmain%2Fjava%2Fcom%2Falibaba%2Fandroid%2Farouter%2Futils%2FClassUtils.java)
运行时通过读取所有dex文件遍历每个entry查找指定包内的所有类名，然后反射获取类对象。这种效率看起来并不高。
ActivityRouter的解决方案是(demo中有2个组件名为'app'和'sdk')：
- 
在主app module中有一个`@Modules({"app", "sdk"})`注解用来标记当前app内有多少组件，根据这个注解生成一个RouterInit类
- 
在RouterInit类的init方法中生成调用同一个包内的RouterMapping_app.map
- 
每个module生成的类(RouterMapping_app.java 和 RouterMapping_sdk.java)都放在com.github.mzule.activityrouter.router包内（在不同的aar中，但包名相同）
- 
在RouterMapping_sdk类的map()方法中根据扫描到的当前module内所有路由注解，生成了调用Routers.map(...)方法来注册路由的代码
- 
在Routers的所有api接口中最终都会触发RouterInit.init()方法，从而实现所有路由的映射表注册
这种方式用一个RouterInit类组合了所有module中的路由映射表类，运行时效率比扫描所有dex文件的方式要高，但需要额外在主工程代码中维护一个组件名称列表注解: @Modules({"app", "sdk"})
还有没有更好的办法呢？
Transform API: 可以在编译时(dex/proguard之前)扫描当前要打包到apk中的所有类，包括: 当前module中java文件编译后的class、aidl文件编译后的class、jar包中的class、aar包中的class、project依赖中的class、maven依赖中的class。
ASM: 可以读取分析字节码、可以修改字节码
二者结合，可以做一个gradle插件，在编译时自动扫描所有组件类(IComponent接口实现类)，然后修改字节码，生成代码调用扫描到的所有组件类的构造方法将其注册到一个组件管理类([ComponentManager](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC%2Fblob%2Fmaster%2Fcc%2Fsrc%2Fmain%2Fjava%2Fcom%2Fbilly%2Fcc%2Fcore%2Fcomponent%2FComponentManager.java))中，生成组件名称与组件对象的映射表。
此gradle插件被命名为：[AutoRegister](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FAutoRegister)，现已开源，并将功能升级为编译时自动扫描任意指定的接口实现类(或类的子类)并自动注册到指定类的指定方法中。只需要在app/build.gradle中配置一下扫描的参数，没有任何代码侵入，原理详细介绍[传送门](https://link.juejin.im?target=http%3A%2F%2Fblog.csdn.net%2Fcdecde111%2Farticle%2Fdetails%2F78074692)
### 3.2 如何兼容同步/异步方式调用组件？
通过实现`java.util.concurrent.Callable`接口同步返回结果来兼容同步/异步调用：
- 同步调用时，直接调用`CCResult result = Callable.call()`来获取返回结果
- 异步调用时，将其放入线程池中运行，执行完成后调用回调对象返回结果: [IComponentCallback.onResult(cc, result)](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC%2Fblob%2Fmaster%2Fcc%2Fsrc%2Fmain%2Fjava%2Fcom%2Fbilly%2Fcc%2Fcore%2Fcomponent%2FIComponentCallback.java)
```java
ExecutorService.submit(callable)
复制代码
```
### 3.3 如何兼容同步/异步方式实现组件？
调用组件的onCall方法时，可能需要异步实现，并不能同步返回结果，但同步调用时又需要返回结果，这是一对矛盾。
此处用到了Object的wait-notify机制，当组件需要异步返回结果时，在CC框架内部进行阻塞，等到结果返回时，通过notify中止阻塞，返回结果给调用方
*注意，这里要求在实现一个组件时，必须确保组件一定会回调结果，即：需要确保每一种导致调用流程结束的逻辑分支上(包括if-else/try-catch/Activity.finish()-back键-返回按钮等等)都会回调结果，否则会导致调用方一直阻塞等待结果，直至超时。类似于向服务器发送一个网络请求后服务器必须返回请求结果一样，否则会导致请求超时。*
### 3.4 如何跨app调用组件？
**为什么需要跨app进行组件调用呢？**
- 对现有项目进行组件化改造的过程，肯定不是一蹴而就，而是一个个组件逐步从主工程中抽离，这就涉及到主工程与组件间的通信。如果不能跨app进行组件调用，开发时就需要跟主工程一起打包，失去了组件化开发的一个非常大的优势：组件单独编译运行提高开发&测试效率。
- 当独立运行的组件需要调用到其他组件的功能时，不需要将其他组件编译进来一起打包，可以调用主app中的组件，可以始终保持单module编译运行的状态进行开发。
**目前，常见的组件化框架采用的跨app通信解决方案有：**
- URLScheme(如：[ActivityRouter](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fmzule%2FActivityRouter)、[阿里ARouter](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Falibaba%2FARouter)等)
- 优点：
- 基因中自带支持从webview中调用
- 不用互相注册（不用知道需要调用的app的进程名称等信息）
- 缺点:
- 只能单向地给组件发送信息，适用于启动Activity和发送指令，不适用于获取数据(例如：获取用户组件的当前用户登录信息)
- 需要有个额外的中转Activity来统一处理URLScheme，然后进行转发
- 如果设备上安装了多个使用相同URLScheme的app，会弹出选择框（多个组件作为app同时安装到设备上时会出现这个问题）
- 无法进行权限设置，无法进行开关设置，任意app都可调用，存在安全性风险
- AIDL (如：[ModularizationArchitecture](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FSpinyTech%2FModularizationArchitecture))
- 优点：
- 可以传递Parcelable类型的对象
- 效率高
- 可以设置跨app调用的开关
- 缺点：
- 调用组件之前需要提前知道该组件在那个进程，否则无法建立ServiceConnection
- 组件在作为独立app和作为lib打包到主app时，进程名称不同，维护成本高
设计此功能时，我的出发点是：作为组件化开发框架基础库，想尽量让跨进程调用与在进程内部调用的功能一致，对使用此框架的开发者在切换app模式和lib模式时尽量简单，另外需要尽量不影响产品安全性。因此，跨组件间通信实现的同时，应该满足以下条件：
- 每个app都能给其它app调用
- app可以设置是否对外提供跨进程组件调用的支持
- 组件调用的请求发出去之后，能自动探测当前设备上是否有支持此次调用的app
- 支持超时、取消
基于这些需求，我最终选择了BroadcastReceiver + Service + LocalSocket来作为最终解决方案：
*如果appA内发起了一个当前app内不存在的组件:Component1，则建立一个LocalServerSocket，同时发送广播给设备上安装的其它同样使用了此框架的app，同时，若某个appB内支持此组件，则根据广播中带来的信息与LocalServerSocket建立连接，并在appB内调用组件Component1，并将结果通过LocalSocket发送给appA。BroadcastReceiver是android四大组件之一，可以设置接收权限，能避免外部恶意调用。并且可以设置开关，接收到此广播后决定是否响应(假装没接收到...)。之所以建立LocalSocket链接，是为了能继续给这次组件调用请求发送超时和取消的指令。*
用这种方式实现时，遇到了3个问题：
- 由于广播接收器定义在基础库中，所有app内都有，当用户在主线程中同步调用跨app的组件时，调用方主线程被阻塞，广播接收器也在需要主线程中运行，导致广播接收器无法运行，直至timeout，组件调用失败。
- 将广播接收器放到子进程中运行问题得到解决
- 被调用的app未启动或被手动结束进程，遇到广播接收不到的问题
- 这个问题暂时未很好的解决，但考虑到组件化开发只在开发期间需要用到跨进程通信，开发者可以通过手动在系统设置中给对应的app赋予自启动权限来解决问题
- 跨进程调用时，只能传递基本数据类型，无法获取Fragment等java对象
- 这个问题在app内部调用时不存在，app内部来回传递的都是Map，可以传递任何数据类型。但由于进程间通信是通过字符串来回发送的，暂时支持不了非基本数据类型，未来可以考虑支持Serializable
### 3.5 组件如何更方便地在application和library之间切换？
关于切换方式在网络上有很多文章介绍，基本上都是一个思路：在module的build.gradle中设置一个变量来控制切换`apply plugin: 'com.android.application'`或`apply plugin: 'com.android.library'`以及sourceSets的切换。
为了避免在每个module的build.gradle中配置太多重复代码，我做了个封装，默认为library模式，提供2种方式切换为application模式：在module的build.gradle中添加`ext.runAsApp = true`或在工程根目录中local.properties中添加`module_name=true`
使用这个封装只需一行代码：
```
// 将原来的 apply plugin: 'com.android.application'或apply plugin: 'com.android.library'
//替换为下面这一行
apply from: 'https://raw.githubusercontent.com/luckybilly/CC/master/cc-settings.gradle'
复制代码
```
注：[cc-settings.gradle源码传送门](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC%2Fblob%2Fmaster%2Fcc-settings.gradle)
### 3.6 如何实现startActivityForResult？
android的startActivityForResult的设计也是为了页面传值，在CC组件化框架中，页面传值根本不需要用到startActivityForResult，直接作为异步实现的组件来处理(在原来setResult的地方调用`CC.sendCCResult(callId, ccResult)`，*另外需要注意：按back键及返回按钮的情况也要回调结果*)即可。
如果是原来项目中存在大量的startActivityForResult代码，改造成本较大，可以用下面这种方式来保留原来的onActivityResult(...)及activity中setResult相关的代码：
- 
在原来调用startActivityForResult的地方，改用CC方式调用，将当前context传给组件
```java
CC.obtainBuilder("demo.ComponentA")
	.setContext(context)
	.addParams("requestCode", requestCode)
	.build()
	.callAsync();
复制代码
```
- 
在组件的onCall(cc)方法中用startActivityForResult的方式打开Activity
```java
@Override
   public boolean onCall(CC cc) {
       Context context = cc.getContext();
       Object code = cc.getParams().get("requestCode");
       Intent intent = new Intent(context, ActivityA.class);
       if (!(context instanceof Activity)) {
           //调用方没有设置context或app间组件跳转，context为application
           intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
       }
       if (context instanceof Activity && code != null && code instanceof Integer) {
           ((Activity)context).startActivityForResult(intent, (Integer)code);
       } else {
           context.startActivity(intent);
       }
       CC.sendCCResult(cc.getCallId(), CCResult.success());
       return false;
   }
复制代码
```
### 3.7 如何阻止非法的外部调用？
为了适应不同需求，有2个安全级别可以设置：
- 
权限验证（给进程间通信的广播设置权限，一般可设置为签名级权限校验），步骤如下：
- 新建一个module
- 在该module的build.gradle中添加对基础库的依赖，如： `compile 'com.billy.android:cc:0.3.0'`
- 在该module的src/main/AndroidManifest.xml中设置权限及权限的级别，参考[component_protect_demo](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC%2Fblob%2Fmaster%2Fcomponent_protect_demo%2Fsrc%2Fmain%2FAndroidManifest.xml)
- 其它每个module都额外依赖此module，或自定义一个全局的cc-settings.gradle，参考[cc-settings-demo-b.gradle](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC%2Fblob%2Fmaster%2Fcc-settings-demo-b.gradle)
- 
外部调用是否响应的开关设置（这种方式使用起来更简单一些）
- 在Application.onCreate()中调用`CC.enableRemoteCC(false)`可关闭响应外部调用
为了方便开发者接入，默认是开启了对外部组件调用的支持，并且不需要权限验证。app正式发布前，建议调用CC.enableRemoteCC(false)来关闭响应外部调用本app的组件。
### 3.8  如何与Activity、Fragment的生命周期关联起来
背景：在使用异步调用时，由于callback对象一般是使用匿名内部类，会持有外部类对象的引用，容易引起内存泄露，这种内存泄露的情况在各种异步回调中比较常见，如Handler.post(runnable)、Retrofit的Call.enqueue(callback)等。
为了避免内存泄露及页面退出后取消执行不必要的任务，CC添加了生命周期关联的功能，在onDestroy方法被调用时自动cancel页面内所有未完成的组件调用
- 
Activity生命周期关联
在api level 14 (android 4.0)以上可以通过注册全局activity生命周期回调监听，在`onActivityDestroyed`方法中找出所有此activity关联且未完成的cc对象，并自动调用取消功能：
```java
application.registerActivityLifecycleCallbacks(lifecycleCallback);
复制代码
```
- 
android.support.v4.app.Fragment生命周期关联
support库从 [25.1.0](https://link.juejin.im?target=https%3A%2F%2Fdeveloper.android.com%2Freference%2Fandroid%2Fsupport%2Fv4%2Fapp%2FFragmentManager.html%23getPrimaryNavigationFragment()) 开始支持给fragment设置生命周期监听：
```java
FragmentManager.registerFragmentLifecycleCallbacks(callback)
复制代码
```
可在其`onFragmentDestroyed`方法中取消未完成的cc调用
- 
andorid.app.Fragment生命周期关联（暂不支持）
# 四、 CC执行流程详细解析
组件间通信采用了组件总线的方式，在基础库的组件管理类(ComponentMananger)中注册了所有组件对象，ComponentMananger通过查找映射表找到组件对象并调用。
当ComponentMananger接收到组件的调用请求时，查找当前app内组件清单中是否含有当前需要调用的组件
- 有： 执行App内部CC调用的流程：
- 
没有：执行App之间CC调用的流程
### 4.1  组件的同步/异步实现和组件的同步/异步调用原理
- 组件实现时，当组件调用的相关功能结束后，通过CC.sendCCResult(callId, ccResult)将调用结果发送给框架
- IComponent实现类(组件入口类)onCall(cc)方法的返回值代表是否异步回调结果：
- true:   将异步调用CC.sendCCResult(callId, ccResult)
- false: 将同步调用CC.sendCCResult(callId, ccResult)。意味着在onCall方法执行完之前会调用此方法将结果发给框架
- 当IComponent.onCall(cc)返回false时，直接获取CCResult并返回给调用方
- 当IComponent.onCall(cc)返回true时，将进入wait()阻塞，知道获得CCResult后通过notify()中止阻塞，继续运行，将CCResult返回给调用方
- 通过ComponentManager调用组件时，创建一个实现了`java.util.concurrent.Callable`接口`ChainProcessor`类来负责具体组件的调用
- 同步调用时，直接执行`ChainProcessor.call()`来调用组件，并将CCResult直接返回给调用方
- 异步调用时，将`ChainProcessor`放入线程池中执行，通过`IComponentCallback.onResult(cc, ccResult)`将CCResult回调给调用方
执行过程如下图所示：
### 4.2 自定义拦截器(`ICCInterceptor`)实现原理
- 
所有拦截器按顺序存放在调用链(Chain)中
- 
在自定义拦截器之前有1个CC框架自身的拦截器：
- `ValidateInterceptor`
- 
在自定义拦截器之后有2个CC框架自身的拦截器：
- `LocalCCInterceptor`（或`RemoteCCInterceptor`)
- `Wait4ResultInterceptor`
- 
Chain类负责依次执行所有拦截器`interceptor.intercept(chain)`
- 
拦截器`intercept(chain)`方法通过调用`Chain.proceed()`方法获取CCResult
### 4.3 App内部CC调用流程
当要调用的组件在当前app内部时，执行此流程，完整流程图如下：
CC的主体功能由一个个拦截器(`ICCInterceptor`)来完成，拦截器形成一个调用链(`Chain`)，调用链由ChainProcessor启动执行，ChainProcessor对象在ComponentManager中被创建。
因此，可以将ChainProcessor看做一个整体，由ComponentManager创建后，调用组件的onCall方法，并将组件执行后的结果返回给调用方。
ChainProcessor内部的`Wait4ResultInterceptor`
ChainProcessor的执行过程可以被timeout和cancel两种事件中止。
### 4.4 App之间CC调用流程
当要调用的组件在当前app内找不到时，执行此流程，完整流程图如下：
# 五、使用方式介绍
CC的集成非常简单，仅需4步即可完成集成：
- 
添加自动注册插件
```
buildscript {
    dependencies {
        classpath 'com.billy.android:autoregister:1.0.4'
    }
}
复制代码
```
- 
引用apply cc-settings.gradle文件代替 'app plugin ...'
```
apply from: 'https://raw.githubusercontent.com/luckybilly/CC/master/cc-settings.gradle'
复制代码
```
- 
实现IComponent接口创建一个组件类
```java
public class ComponentA implements IComponent {
    
    @Override
    public String getName() {
        //组件的名称，调用此组件的方式：
        // CC.obtainBuilder("demo.ComponentA").build().callAsync()
        return "demo.ComponentA";
    }
    @Override
    public boolean onCall(CC cc) {
        Context context = cc.getContext();
        Intent intent = new Intent(context, ActivityComponentA.class);
        if (!(context instanceof Activity)) {
            //调用方没有设置context或app间组件跳转，context为application
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        }
        context.startActivity(intent);
        //发送组件调用的结果（返回信息）
        CC.sendCCResult(cc.getCallId(), CCResult.success());
        
        return false;
    }
}
复制代码
```
- 
使用`CC.obtainBuilder("component_name").build().call()`调用组件
```java
//同步调用，直接返回结果
CCResult result = CC.obtainBuilder("demo.ComponentA").build().call();
//或 异步调用，不需要回调结果
CC.obtainBuilder("demo.ComponentA").build().callAsync();
//或 异步调用，在子线程执行回调
CC.obtainBuilder("demo.ComponentA").build().callAsync(new IComponentCallback(){...});
//或 异步调用，在主线程执行回调
CC.obtainBuilder("demo.ComponentA").build().callAsyncCallbackOnMainThread(new IComponentCallback(){...});
复制代码
```
更多用法请看github上的[README](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC%2Fblob%2Fmaster%2FREADME.md)
PS:配合我的另一个库（[PreLoader](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FPreLoader)）一起食用味道更佳：AOP实现在打开页面之前预加载页面所需的数据，并且这个预加载功能完全在组件内部实现，与外部无耦合。
# 六、老项目进行组件化改造的成本有多高？
有些同学很想尝试组件化开发，但仅仅停留在了解的阶段，原因是担心在老项目上进行改造的工程量太大，不敢大改。
CC框架本身就是在老项目进行组件化改造的需求下设计出来的，考虑到了组件化过程中的一些痛点：
- 万事开头难，不要太多配置：CC将配置封装到了一个gradle文件，需要组件化的module在其build.gradle中添加`apply from: 'https://raw.githubusercontent.com/luckybilly/CC/master/cc-settings.gradle'`即可
- 万事开头难，不要改动太大：其实，在组件化改造之前，如果封装的比较好的话，很多lib module与主工程的耦合仅仅是Activity的类名。例如我们项目中的收银台，组件化之前通过startActivityForResult的方式打开收银台主界面并通过setResult的方式返回结果给调用方，只需要在收银台module中增加一个IComponent的实现类，在其onCall方法中调用cc.getContext().startActivityForResult打开收银台页面，并将原来通过startActivityForResult打开收银台的地方改成CC调用即可，收银台module原来的代码纹丝不动。后续优化时再将结果的返回方式从setResult换成CC.sendCCResult(callId, result)就彻底完成了收银台的组件化改造工作。
- BaseActivity/BaseFragment/Util类/MVP封装的各种Base等：组件化改造并不是让所有module都变成组件，还有一些可以下沉作为公共库，
- 模块太复杂，短时间内改不动： 页面改版是个很好的机会，提前与产品经理沟通，获知那些模块即将改版，乘着改版对模块进行组件化改造一举两得。我们的详情页、购物车、首页都是在这个过程中完成组件化改造的。
- 依赖太多，解耦不便：在改造初期，可以先粗粒度定义组件，熟练之后再继续优化
- 组件化开发框架学习成本高，要团队内每个人都了解需要较长时间：CC为所有的组件调用提供了统一的调用方式和实现方式，没有任何注解、仅1个接口类。另外library模式切换成application的方式简化为在local.properties中配置一行`module名称=true`即可。几乎零门槛进行组件化开发
# 结语
本文比较详细地介绍了android组件化开发框架《CC》的主要功能、技术方案及执行流程，并给出了使用方式的简单示例。
大家如果感兴趣的话可以[从GitHub上clone源码](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluckybilly%2FCC)来进行具体的分析，如果有更好的思路和方案也欢迎贡献代码进一步完善CC。
# 系列文章
[CC框架实践(1)：实现登录成功再进入目标界面功能](https://juejin.im/post/5a30d5f151882534af25f1c2)
[CC框架实践(2)：Fragment和View的组件化](https://juejin.im/post/5a3e48f26fb9a045132ae7ff)
[CC框架实践(3): 让jsBridge更优雅](https://juejin.im/post/5a3fa7fe6fb9a045204c702f)
# 致谢
[ActivityRouter](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fmzule%2FActivityRouter)
[ARouter](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Falibaba%2FARouter)
[ModularizationArchitecture](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FSpinyTech%2FModularizationArchitecture)
[Android架构思考(模块化、多进程)](https://link.juejin.im?target=http%3A%2F%2Fblog.spinytech.com%2F2016%2F12%2F28%2Fandroid_modularization%2F)
[开源最佳实践：Android平台页面路由框架ARouter](https://link.juejin.im?target=https%3A%2F%2Fyq.aliyun.com%2Farticles%2F71687)
[DDComponentForAndroid](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fluojilab%2FDDComponentForAndroid)
[Router](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FJumeiRdGroup%2FRouter)
# 交流
