# Android开发问题笔记 - =朝晖= - 博客园
# [Android开发问题笔记](https://www.cnblogs.com/dhcn/p/7121358.html)
1、Toolbar问题：最低版本15，必须使用support，才能使用Toolbar，Toobar是5.0引入的
2、BottomTab:这个用TabLayout解决了
3、后端API最好采用一个成熟的框架来做，比如Restful的Django-Rest-Framework，既可以简化IT设施层、又可以复用成熟的设计：
4、最后还是用[Android](http://lib.csdn.net/base/android) Studio生成向导的TabLayout+ViewPager+PlaceFragment解决问题。
5、官网的Best Practice值得参考借鉴
6、采用OkHTTP做网络访问
7、采用Picasso做图片加载
8、采用ExpandListView做According:
[http://www.cnblogs.com/eyu8874521/archive/2012/08/16/2642605.html](http://www.cnblogs.com/eyu8874521/archive/2012/08/16/2642605.html)
[http://my.oschina.net/amigos/blog/62614](http://my.oschina.net/amigos/blog/62614)
9、非动态调整的信息尽量在layout里面指明
10、代码Copy+粗心大意坑死人：出现问题应该先自己定位。
11、[android](http://lib.csdn.net/base/android)的UI设计思路不是做好UI套数据，而是由[数据结构](http://lib.csdn.net/base/datastructure)出UI结构，UI和数据通过Adapter在结构上匹配生成。所以本质上android不是拖拽式开发的界面设计,而是官方所宣称的MCV结构，类似于网页的MVT[架构](http://lib.csdn.net/base/architecture)。
12、Android设计开发第一坑：布局
13、ListView子项的margin可以用devider来设置
14、调效果用手机，比模拟器快，调代码用模拟器，跟踪比真机方便。
15、要努力拉长App代码调试的时间周期，因为run一次的时间消耗不短。
16、通过沟通提高前后端联调效率的一个方法：对领域模型的设计要有专门的沟通通气会议。
17、AndroidSDK的本地安装文档，因为引用了远程文件的原因，可用性不是很好。
18、关键设计一定要有相关方审查步骤，比如数据模型、比如移动后端的API。
19、ListView里面不应该出现布局不一致的ListItem，这样就会导致ItemView的复用出现问题
20、Button可以用text显示，tag传值。
21、给RxJava传递的final参数可能需要copy复制才可以保证多线程读写问题，一般只有UI主线程和后台线程的情境下不需要
22、资源状态列表，这个信息可以从官方API指南找到:
- <selector xmlns:android="http://schemas.android.com/apk/res/android"; >   
- <item   
- android:color="hex_color"   
- android:state_pressed=["true" | "false"]   
- android:state_focused=["true" | "false"]   
- android:state_selected=["true" | "false"]   
- android:state_active=["true" | "false"]   
- android:state_checkable=["true" | "false"]   
- android:state_checked=["true" | "false"]   
- android:state_enabled=["true" | "false"]   
- android:state_window_focused=["true" | "false"] />   
- </selector>   

23、Fragment的onCreate用来初始化数据，onCreateView用来初始化界面，前者一般只执行一次，后者随界面切换多次执行，所以后者里面最好不要做数据加载，如果某些数据加载必须在界面初始化以后做，要加上首次执行条件判断。
24、TextView里可用的HTML标签，注意标签不能String资源里面直接写，直接写就被过滤掉了，但是写在代码的String变量里面可以，虽然不太符合Android开发规范，但是避免了字符转换，算是比较nice的解决方式。
- <a href="...">  
- <b>  
- <big>  
- <blockquote>  
- <br>  
- <cite>  
- <dfn>  
- <div align="...">  
- <em>  
- <font size="..." color="..." face="...">  
- <h1>  
- <h2>  
- <h3>  
- <h4>  
- <h5>  
- <h6>  
- <i>  
- <img src="...">  
- <p>  
- <small>  
- <strike>  
- <strong>  
- <sub>  
- <sup>  
- <tt>  
- <u>  
25、onCreate、onStart里面对View做操作，View都可能还没有渲染成功，所以比如getHeight得到的可能是0，要确保view渲染以后代码执行，可以用view.post(runnable)
26、ListView显示不全，原因是AppbarLayout占了两行空间。把TabLayout移出AppbarLayout
27、为什么要用接口：没有基类的实现负担，实现基类的不同子类对象实例的灵活可换。丛某种角度讲：接口是静态类型语言非鸭式辨型导致的累赘。
28、Fragment的的资源新建初始化必须放在onCreate，放在onCreateView里面会导致重复调用，从而资源泄露。
29、has leaked window com.android.internal.policy.impl.PhoneWindow$ that was originally added here  这个问题的交互诱因是Activity退出的时候，其界面上的dialog还在显示，解决方法，覆盖相关组件的onDestroy方法，把dialog释放掉：
- @Override  
- public void onDestroy(){  
- super.onDestroy();  
- if ( mLoadingDialog!=null && mLoadingDialog.isShowing() ){  
-         mLoadingDialog.dismiss();  
-     }  
- }  
另外多说一句话，静态对象不能引用组件资源，否则会导致被引用的组件资源无法正常地被系统框架释放掉。
30、Android Studio 主题资源站：http://www.androidstudiothemes.com/
31、一般来讲带反向应用的组件实例对象变量要在组件onDestroy的时候把这些变量都要释放掉（=null），至于静态变量最好不要有对组件的引用，也就是说有对组件有反向应用的对象变量最好不要做成静态的。一般有队组件有反向引用的对象主要是系统组件对象。
32、AVD虚拟机启动错误提示：
- emulator: WARNING: .//android/metrics/metrics_reporter_ga.c:138: curl_easy_perform() failed with code 7 (Error)  
- emulator: WARNING: .//android/metrics/metrics_reporter_ga.c:145: Got HTTP response code 0  
修改方法：Open the Android Studio->Preferences, go to Appearance & Behavior->System Settings->Usage Statistics, and uncheck "Send usage statistics to Google". Now the matrics_reporter will be disabled during emulator starting. 
推荐出不出错误，都做此修改，不然虚拟机启动以后，Log老报这方面的连接错误。
33、Fragment的getView()在onCreateView这个函数执行之前是空的，所以onCreateView函数里面只能用自己inflate的view来做findViewbyID操作。
34、RxAndroid导致的空指针问题：一般网络数据爬取完，要操作Activity或者Fragment的资源来完成数据显示更新，但是由于网络访问和用户切换界面操作的时间差，当你做数据显示更新的时候，也许Activtiy或者fragment的资源已经不复存在由此导致NullPointer Error问题，可以通过操作这些资源之前做个空指针判断来解决这个问题。
35、Fragment的Context Activity空指针问题，由于API不同版本之间的不一致性，在Fragment的生存生命周期内，可能存在其Context Activity其实不存在或者被销毁，在这种情况下，如果要使用这个Actiity对象，也要做空指针判断。
36、对于Fragment的使用，《Android权威编程指南》曾经推荐尽量使用标准库里的Fragment，并且放弃使用FragmentActivity，转而使用标准库中的Activity类，放弃使用support.v4.app.fragment,转而使用app.fragment,放弃使用getSupportFragmentManager().转而使用getFragmentManager()方法
37、对于所有View的取其界面属性值的处理,比如getHeight，getWidth，都要用post异步化，因为做成同步，很容易是界面未正确渲染时的结果。

