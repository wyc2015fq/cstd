# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之80——BREW应用间通信之回调通知 - 我相信...... - CSDN博客





2010年09月29日 14:03:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1081








AEE层 支持通知的概念，通知是一种当其它应用中发生某些差错时可以为该事件注册不同应用来处理的机制。例如，可以将应用注册为在有进入的 SMS 消息时接收通知。这表示只要有进入的 SMS 消息，已注册接收通知的应用就会收到一个事件，这个事件就是 EVT_NOTIFY。通知程序就是产生通知的应用，例如： ITAPI 可充当通知程序，在有进入的 SMS 消息时就会发会通知。每当通知程序需要发送通知时，都要使用 ISHELL_Notify() 方法。 应用可以通过注册来接收通知，要这样做，一般要使用函数 ISHELL_RegisterNotify()。 例如，应用可以使用ISHELL_RegisterNotify()注册从 AEECLSID_TAPI 接口接收该类特定事件的通知。 

INotifier 是指定必须由需要成为通知程序的应用来实现的函数接口。任何要发出通知以便其它应用能够收到它的应用必须实现 Inotifier 接口中的函数，且实现 Inotifier 接口并定义该类能够发出的通知（或掩码）组，0xFFFFFFFF 代表接收所有的通知，0x00000000代表所有通知都不接收，例如：TAPI可以发出的通知有NMASK_TAPI_SMS、NMASK_TAPI_TS_PAGE 和 NMASK_TAPI_TS_VMAIL等类型。 这些掩码必须对其它应用可用，以便关注这些通知的应用可以注册它们。通知程序无论何时发出通知，均必须调用 ISHELL_Notify() 函数。这时， AEE层负责通知已注册接收该通知的所有应用。

从BREW 3.1.4以后，引入了IALARMMGR接口，一个应用可以通过告警设置来通知另一个应用执行相关的操作。相对于ISHELL_SetAlarm而言,IALARMMGR可以对以秒为单位的时间间隔来设定告警触发，而且可以根据相当时间或者绝对时间来设定。IALARMMGR的一个重要功能是在设定告警触发前可以为用户保留一个唯一的告警代码。告警代码是一个16 bit的整数，可以使用 IALARMMGR_SetUserCode() 和IALARMMGR_GetUserCode() 来设定和获取该告警代码。通过IALARMMGR_SetAlarm()指定一个有效的 ClassId就可以让另一个应用接收告警了，接收告警的应用监听到EVT_ALARM事件， wParam是告警代码，然后就可以执行相关的操作了。



