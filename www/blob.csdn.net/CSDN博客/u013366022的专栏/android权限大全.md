# android权限大全 - u013366022的专栏 - CSDN博客
2014年05月28日 10:31:39[slitaz](https://me.csdn.net/u013366022)阅读数：396
Ø  **android.permission.ACCESS_CHECKIN_PROPERTIES**
允许读写访问"properties"表在checkin数据库中，改值可以修改上传( Allows read/write access to the "properties" table inthe checkin database, to change values that get uploaded)
Ø  **android.permission.ACCESS_COARSE_LOCATION**
允许一个程序访问CellID或WiFi热点来获取粗略的位置(Allows an application to access coarse (e.g., Cell-ID, WiFi)location)
Ø  **android.permission.ACCESS_FINE_LOCATION**
允许一个程序访问精良位置(如GPS) (Allows an application to access fine (e.g., GPS) location)
Ø  **android.permission.ACCESS_LOCATION_EXTRA_COMMANDS**
允许应用程序访问额外的位置提供命令(Allows an application to access extra location provider commands)
Ø  **android.permission.ACCESS_MOCK_LOCATION**
允许程序创建模拟位置提供用于测试(Allows an application to create mock location providers fortesting)
Ø  **android.permission.ACCESS_NETWORK_STATE**
允许程序访问有关GSM网络信息(Allows applications to access information about networks)
Ø  **android.permission.ACCESS_SURFACE_FLINGER**
允许程序使用SurfaceFlinger底层特性(Allows an application to use SurfaceFlinger's low level features)
Ø  **android.permission.ACCESS_WIFI_STATE**
允许程序访问Wi-Fi网络状态信息(Allows applications to access information about Wi-Fi networks)
Ø  **android.permission.ADD_SYSTEM_SERVICE**
允许程序发布系统级服务(Allows an application to publish system-level services).
Ø  **android.permission.BATTERY_STATS**
允许程序更新手机电池统计信息(Allows an application to update the collected battery statistics)
Ø  **android.permission.BLUETOOTH**
允许程序连接到已配对的蓝牙设备(Allows applications to connect to paired bluetooth devices)
Ø  **android.permission.BLUETOOTH_ADMIN**
允许程序发现和配对蓝牙设备(Allows applications to discover and pair bluetooth devices)
Ø  **android.permission.BRICK**
请求能够禁用设备(非常危险)(Required to be able to disable the device (very *erous!).)
Ø  **android.permission.BROADCAST_PACKAGE_REMOVED**
允许程序广播一个提示消息在一个应用程序包已经移除后(Allows an application to broadcast a notification that anapplication package has been removed)
Ø  **android.permission.BROADCAST_STICKY**
允许一个程序广播常用intents(Allows an application to broadcast sticky intents)
Ø  **android.permission.CALL_PHONE**
允许一个程序初始化一个电话拨号不需通过拨号用户界面需要用户确认(Allows an application to initiate a phone call without goingthrough the Dialer user interface for the user to confirm the call beingplaced.)
Ø  **android.permission.CALL_PRIVILEGED**
允许一个程序拨打任何号码，包含紧急号码无需通过拨号用户界面需要用户确认(Allows an application to call any phone number, including emergencynumbers, without going through the Dialer user interface for the user toconfirm the call being placed)
Ø  **android.permission.CAMERA**
请求访问使用照相设备(Required to be able to access the camera device. )
Ø  **android.permission.CHANGE_COMPONENT_ENABLED_STATE**
允许一个程序是否改变一个组件或其他的启用或禁用(Allows an application to change whether an application component(other than its own) is enabled or not. )
Ø  **android.permission.CHANGE_CONFIGURATION**
允许一个程序修改当前设置，如本地化(Allows an application to modify the current configuration, such aslocale. )
Ø  **android.permission.CHANGE_NETWORK_STATE**
允许程序改变网络连接状态(Allows applications to change network connectivity state)
Ø  **android.permission.CHANGE_WIFI_STATE**
允许程序改变Wi-Fi连接状态(Allows applications to change Wi-Fi connectivity state)
Ø  **android.permission.CLEAR_APP_CACHE**
允许一个程序清楚缓存从所有安装的程序在设备中(Allows an application to clear the caches of all installedapplications on the device. )
Ø  **android.permission.CLEAR_APP_USER_DATA**
允许一个程序清除用户设置(Allows an application to clear user data)
Ø  **android.permission.CONTROL_LOCATION_UPDATES**
允许启用禁止位置更新提示从无线模块(Allows enabling/disabling location update notifications from theradio. )
Ø  **android.permission.DELETE_CACHE_FILES**
允许程序删除缓存文件(Allows an application to delete cache files)
Ø  **android.permission.DELETE_PACKAGES**
允许一个程序删除包(Allows an application to delete packages)
Ø  **android.permission.DEVICE_POWER**
允许访问底层电源管理(Allows low-level access to power management)
Ø  **android.permission.DIAGNOSTIC**
允许程序RW诊断资源(Allows applications to RW to diagnostic resources. )
Ø  **android.permission.DISABLE_KEYGUARD**
允许程序禁用键盘锁(Allows applications to disable the keyguard )
Ø  **android.permission.DUMP**
允许程序返回状态抓取信息从系统服务(Allows an application to retrieve state dump information fromsystem services.)
Ø  **android.permission.EXPAND_STATUS_BAR**
允许一个程序扩展收缩在状态栏,android开发网提示应该是一个类似Windows Mobile中的托盘程序(Allows an application to expand or collapse the status bar. )
Ø  **android.permission.FACTORY_TEST**
作为一个工厂测试程序，运行在root用户(Run as a manufacturer test application, running as the root user. )
Ø  **android.permission.FLASHLIGHT**
访问闪光灯,android开发网提示HTC Dream不包含闪光灯(Allows access to the flashlight )
Ø  **android.permission.FORCE_BACK**
允许程序强行一个后退操作是否在顶层activities(Allows an application to force a BACK operation onwhatever is the top activity. )
Ø  **android.permission.FOTA_UPDATE**
暂时不了解这是做什么使用的，android开发网分析可能是一个预留权限.
Ø  **android.permission.GET_ACCOUNTS**
访问一个帐户列表在Accounts Service中(Allows access to the list of accounts in the Accounts Service)
Ø  **android.permission.GET_PACKAGE_SIZE**
允许一个程序获取任何package占用空间容量(Allows an application to find out the space used by any package. )
Ø  **android.permission.GET_TASKS**
允许一个程序获取信息有关当前或最近运行的任务，一个缩略的任务状态，是否活动等等(Allows an application to get information about the currently orrecently running tasks: a thumbnail representation of the tasks, whatactivities are running in it, etc.)
Ø  **android.permission.HARDWARE_TEST**
允许访问硬件(Allows access to hardware peripherals. )
Ø  **android.permission.INJECT_EVENTS**
允许一个程序截获用户事件如按键、触摸、轨迹球等等到一个时间流，android 开发网提醒算是hook技术吧(Allows an application to inject user events (keys, touch,trackball) into the event stream and deliver them to ANY window.)
Ø  **android.permission.INSTALL_PACKAGES**
允许一个程序安装packages(Allows an application to install packages. )
android.permission.INTERNAL_SYSTEM_WINDOW
允许打开窗口使用系统用户界面(Allows an application to open windows that are for use by parts ofthe system user interface. )
Ø  **android.permission.INTERNET**
允许程序打开网络套接字(Allows applications to open network sockets)
Ø  **android.permission.MANAGE_APP_TOKENS**
允许程序管理(创建、催后、z- order默认向z轴推移)程序引用在窗口管理器中(Allows an application to manage (create, destroy, Z-order)application tokens in the window manager. )
Ø  **android.permission.MASTER_CLEAR**
目前还没有明确的解释，android开发网分析可能是清除一切数据，类似硬格机
Ø  **android.permission.MODIFY_AUDIO_SETTINGS**
允许程序修改全局音频设置(Allows an application to modify global audio settings)
Ø  **android.permission.MODIFY_PHONE_STATE**
允许修改话机状态，如电源，人机接口等(Allows modification of the telephony state - power on, mmi, etc. )
Ø  **android.permission.MOUNT_UNMOUNT_FILESYSTEMS**
允许挂载和反挂载文件系统可移动存储(Allows mounting and unmounting file systems for removable storage.)
Ø  **android.permission.PERSISTENT_ACTIVITY**
允许一个程序设置他的activities显示(Allow an application to make its activities persistent. )
Ø  **android.permission.PROCESS_OUTGOING_CALLS**
允许程序监视、修改有关播出电话(Allows an application to monitor, modify, or abort outgoing calls)
Ø  **android.permission.READ_CALENDAR**
允许程序读取用户日历数据(Allows an application to read the user's calendar data.)
Ø  **android.permission.READ_CONTACTS**
允许程序读取用户联系人数据(Allows an application to read the user's contacts data.)
Ø  **android.permission.READ_FRAME_BUFFER**
允许程序屏幕波或和更多常规的访问帧缓冲数据(Allows an application to take screen shots and more generally getaccess to the frame buffer data)
Ø  **android.permission.READ_INPUT_STATE**
允许程序返回当前按键状态(Allows an application to retrieve the current state of keys andswitches. )
Ø  **android.permission.READ_LOGS**
允许程序读取底层系统日志文件(Allows an application to read the low-level system log files. )
Ø  **android.permission.READ_OWNER_DATA**
允许程序读取所有者数据(Allows an application to read the owner's data)
Ø  **android.permission.READ_SMS**
允许程序读取短信息(Allows an application to read SMS messages.)
Ø  **android.permission.READ_SYNC_SETTINGS**
允许程序读取同步设置(Allows applications to read the sync settings)
Ø  **android.permission.READ_SYNC_STATS**
允许程序读取同步状态(Allows applications to read the sync stats)
Ø  **android.permission.REBOOT**
请求能够重新启动设备(Required to be able to reboot the device. )
Ø  **android.permission.RECEIVE_BOOT_COMPLETED**
允许一个程序接收到 ACTION_BOOT_COMPLETED广播在系统完成启动(Allows an application to receive the ACTION_BOOT_COMPLETED that isbroadcast after the system finishes booting. )
Ø  **android.permission.RECEIVE_MMS**
允许一个程序监控将收到MMS彩信,记录或处理(Allows an application to monitor incoming MMS messages, to recordor perform processing on them. )
Ø  **android.permission.RECEIVE_SMS**
允许程序监控一个将收到短信息，记录或处理(Allows an application to monitor incoming SMS messages, to recordor perform processing on them.)
Ø  **android.permission.RECEIVE_WAP_PUSH**
允许程序监控将收到WAP PUSH信息(Allows an application to monitor incoming WAP push messages. )
Ø  **android.permission.RECORD_AUDIO**
允许程序录制音频(Allows an application to record audio)
Ø  **android.permission.REORDER_TASKS**
允许程序改变Z轴排列任务(Allows an application to change the Z-order of tasks)
Ø  **android.permission.RESTART_PACKAGES**
允许程序重新启动其他程序(Allows an application to restart other applications)
Ø  **android.permission.SEND_SMS**
允许程序发送SMS短信(Allows an application to send SMS messages)
Ø  **android.permission.SET_ACTIVITY_WATCHER**
允许程序监控或控制activities已经启动全局系统中Allows an application to watch and control how activities arestarted globally in the system.
Ø  **android.permission.SET_ALWAYS_FINISH**
允许程序控制是否活动间接完成在处于后台时Allows an application to control whether activities are immediatelyfinished when put in the background.
Ø  **android.permission.SET_ANIMATION_SCALE**
修改全局信息比例(Modify the global animation scaling factor.)
Ø  **android.permission.SET_DEBUG_APP**
配置一个程序用于调试(Configure an application for debugging.)
Ø  **android.permission.SET_ORIENTATION**
允许底层访问设置屏幕方向和实际旋转(Allows low-level access to setting the orientation (actuallyrotation) of the screen.)
Ø  **android.permission.SET_PREFERRED_APPLICATIONS**
允许一个程序修改列表参数PackageManager.addPackageToPreferred()和PackageManager.removePackageFromPreferred()方法(Allows anapplication to modify the list of preferred applications with thePackageManager.addPackageToPreferred() andPackageManager.removePackageFromPreferred()
 methods.)
Ø  **android.permission.SET_PROCESS_FOREGROUND**
允许程序当前运行程序强行到前台(Allows an application to force any currently running process to bein the foreground.)
Ø  **android.permission.SET_PROCESS_LIMIT**
允许设置最大的运行进程数量(Allows an application to set the maximum number of (not needed)application processes that can be running. )
Ø  **android.permission.SET_TIME_ZONE**
允许程序设置时间区域(Allows applications to set the system time zone)
Ø  **android.permission.SET_WALLPAPER**
允许程序设置壁纸(Allows applications to set the wallpaper )
Ø  **android.permission.SET_WALLPAPER_HINTS**
允许程序设置壁纸hits(Allows applications to set the wallpaper hints)
Ø  **android.permission.SIGNAL_PERSISTENT_PROCESSES**
允许程序请求发送信号到所有显示的进程中(Allow an application to request that a signal be sent to allpersistent processes)
Ø  **android.permission.STATUS_BAR**
允许程序打开、关闭或禁用状态栏及图标Allows an application to open, close, or disable the status bar andits icons.
Ø  **android.permission.SUBSCRIBED_FEEDS_READ**
允许一个程序访问订阅RSS Feed内容提供(Allows an application to allow access the subscribed feedsContentProvider. )
Ø  **android.permission.SUBSCRIBED_FEEDS_WRITE**
系统暂时保留改设置,android开发网认为未来版本会加入该功能。
