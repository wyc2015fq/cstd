# Android 4.0和WP7.5开发技术对比 - 我相信...... - CSDN博客





2011年11月12日 08:57:33[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1167










作者: Android开发网原创时间: 2011-10-09



对于谷歌Android 4.0 Ice Cream SandWich和微软WP7.5 Mango在开发技术上作对比，可以看出早期Google学习了Microsoft的WPF的设计方式，由于微软Windows Phone 7发布缓慢，导致Google在技术上将微软复杂的设计方式改进，从现在来看安卓冰激凌三明治和微软WP7芒果来看开发技术上仍然存在着巨大的差异。对于准备步入移动开发的网友，就目前主流的系统做简单的分析:

一、UI控件和设计模式

对于WP7来说，整体控件数量不如Android丰富和实用，但也有一些创新的地方，比如说枢轴Piovt控件，在触控操作上比Android的TabHost强了很多，但没有提供类似GridView这样的控件在Siliverlight中，由于WP7的UI刷新机制基于MVVM模式，相对于Android的MVC有些不同，需要继承INotifyPropertyChanged接口，通过数据绑定来更新UI控件的值，相对比较繁琐。在控件自定义方面，WP7可以使用银光的UserControl或从任何已有的控件中直接派生，对于Android来说这点基本相同，但提供的方法比如触控、长按相关的微软这点都要
 依赖Toolkit来实现，整体相对繁琐很多。

二、数据库层

微软在数据库方面一直有MS SQL，在手机上有SQL CE这样的轻量级数据库，但操作处理方面同样需要实现 INotifyPropertyChanged接口进行更新，修改表，似乎变得更加抽象。同时数据交换方面微软还有LINQ，在Web Services方面几乎和J2EE差不多，但相对于手机上而言Android的SQLite似乎更好用明了，这点微软确实输了。

三、系统执行模型

在WP7.5 Mango上支持了多任务，内存管理几乎和Android一模一样，同时新增了类似iOS上的数据推送，需要微软服务器中转的Push Notification，由于考虑应用随时被系统内存回收，必须保存每个页面的数据到持久化的存储中，比如类似Android的SharedPreferences而在WP中为银光的IsolatedStorageSettings，在很多地方微软都在学习着谷歌，相比WP7到WM6的改进来看。

四、图形系统

这点WP7和Android有着天壤之别，Windows CE下3D为DirectX，2D为GDI方式渲染，而Android下为OpenGL，同时WP7无法使用本地语言开发，导致iPhone和Android程序员在3D方面很是陌生。

五、文件系统

在WP7芒果上，微软学习了iOS那样的封闭，每个应用完全独立，所以没有文件管理器这个概念，导致了Adobe Reader这样的应用必须将pdf放入邮箱中，通过邮件附件来打开阅读。同样每个Windows Phone 7芒果手机一般不会有microSD卡，而是机身至少内置8GB或16GB的存储空间，同样无法用U盘功能，每个应用相对隔离，运行在自己的沙盒空间里，对于应用可靠性和安全有了保障，但是对于部分实现方面确实不够好用。

六、运行流畅度

如果你看见WP7手机运行很流畅，那么你可能真的错了，在本质上Java虚拟机和微软的.Net Framework运行时库的底层效率差不多，但Android的手机一般运行着更复杂的桌面，比如左右N个主屏和一堆appWidget小插件，而微软似乎Windows Phone 7的应用一般限制在5个同时工作的，否则将会被结束任务，同时由于部分低价Android设备，导致整体硬件配置偏低，比如有ARM11的CPU，RAM只有256MB的情况，而微软WP7第一代手机至少配备了A8 1GHz的处理器，512MB的RAM，第二代的WP7
 Mango手机配备了1.4GHz新一代高通Snapdragon，比如8X55系列，所以很多地方了看WP7更流畅，但其实从单个应用来看，WP7应用的启动速度远不如Android平台上的应用，所以微软甚至设计出了SplashScreenImage.jpg这样的图片为每个默认工程提供一个载入图片。如果是微软系统自带的应用由于可以调用本地的方法相对于第三方的Windows Phone应用来说启动会快很多。

最终结论是Android123仍然推荐大家在高效和开放的Google提供的平台开发，至少不会束缚你的想象力和实现速度，微软设计了很多限制和门槛，同时不开源在分析和实现上都会产生很多问题，至少Android的SDK可以工作在Mac OS、Windows各个平台和Linux上，而微软似乎将WP7 SDK锁定在Vista和Win7上。








