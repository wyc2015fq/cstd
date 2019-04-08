# Android Manifest功能与权限描述大全

Android Manifest是Android程序开发中控制各项功能权限的核心文件，该参考表详尽描述了Android Manifest各个参数项的名称与功能。是Android开发与学习必备的参考表，欢迎需要的朋友使用。

## android.permission

| 权限 | 名称 | 描述 |
| :--------------- | :------------------- | :----------------------------------------------------------- |
| ACCESS_CHECKIN_PROPERTIES | 访问登记属性 | 读取或写入登记check-in数据库属性表的权限 |
| ACCESS_COARSE_LOCATION | 获取错略位置 | 通过WiFi或移动基站的方式获取用户错略的经纬度信息,定位精度大概误差在30~1500米 |
| ACCESS_FINE_LOCATION | 获取精确位置 | 通过GPS芯片接收卫星的定位信息,定位精度达10米以内 |
| ACCESS_LOCATION_EXTRA_COMMANDS | 访问定位额外命令 | 允许程序访问额外的定位提供者指令 |
| ACCESS_MOCK_LOCATION | 获取模拟定位信息 | 获取模拟定位信息,一般用于帮助开发者调试应用 |
| ACCESS_NETWORK_STATE | 获取网络状态 | 获取网络信息状态,如当前的网络连接是否有效 |
| ACCESS_SURFACE_FLINGER | 访问Surface Flinger | Android平台上底层的图形显示支持,一般用于游戏或照相机预览界面和底层模式的屏幕截图 |
| ACCESS_WIFI_STATE | 获取WiFi状态 | 获取当前WiFi接入的状态以及WLAN热点的信息 |
| ACCOUNT_MANAGER | 账户管理 | 获取账户验证信息,主要为GMail账户信息,只有系统级进程才能访问的权限 |
| AUTHENTICATE_ACCOUNTS | 验证账户 | 允许一个程序通过账户验证方式访问账户管理ACCOUNT_MANAGER相关信息 |
| BATTERY_STATS | 电量统计 | 获取电池电量统计信息 |
| BIND_APPWIDGET | 绑定小插件 | 允许一个程序告诉appWidget服务需要访问小插件的数据库,只有非常少的应用才用到此权限 |
| BIND_DEVICE_ADMIN | 绑定设备管理 | 请求系统管理员接收者receiver,只有系统才能使用 |
| BIND_INPUT_METHOD | 绑定输入法 | 请求InputMethodService服务,只有系统才能使用 |
| BIND_REMOTEVIEWS | 绑定RemoteView | 必须通过RemoteViewsService服务来请求,只有系统才能用 |
| BIND_WALLPAPER | 绑定壁纸 | 必须通过WallpaperService服务来请求,只有系统才能用 |
| BLUETOOTH | 使用蓝牙 | 允许程序连接配对过的蓝牙设备 |
| BLUETOOTH_ADMIN | 蓝牙管理 | 允许程序进行发现和配对新的蓝牙设备 |
| BRICK | 变成砖头 | 能够禁用手机,非常危险,顾名思义就是让手机变成砖头 |
| BROADCAST_PACKAGE_REMOVED | 应用删除时广播 | 当一个应用在删除时触发一个广播 |
| BROADCAST_SMS | 收到短信时广播 | 当收到短信时触发一个广播 |
| BROADCAST_STICKY | 连续广播 | 允许一个程序收到广播后快速收到下一个广播 |
| BROADCAST_WAP_PUSH | WAP PUSH广播 | WAP PUSH服务收到后触发一个广播 |
| CALL_PHONE | 拨打电话 | 允许程序从非系统拨号器里输入电话号码 |
| CALL_PRIVILEGED | 通话权限 | 允许程序拨打电话,替换系统的拨号器界面 |
| CAMERA | 拍照权限 | 允许访问摄像头进行拍照 |
| CELL_PHONE_MASTER_EX | 手机优化大师扩展权限 | 手机优化大师扩展权限 |
| CHANGE_COMPONENT_ENABLED_STATE | 改变组件状态 | 改变组件是否启用状态 |
| CHANGE_CONFIGURATION | 改变配置 | 允许当前应用改变配置,如定位 |
| CHANGE_NETWORK_STATE | 改变网络状态 | 改变网络状态如是否能联网 |
| CHANGE_WIFI_MULTICAST_STATE | 改变WiFi多播状态 | 改变WiFi多播状态 |
| CHANGE_WIFI_STATE | 改变WiFi状态 | 改变WiFi状态 |
| CLEAR_APP_CACHE | 清除应用缓存 | 清除应用缓存 |
| CLEAR_APP_USER_DATA | 清除用户数据 | 清除应用的用户数据 |
| CONTROL_LOCATION_UPDATES | 控制定位更新 | 允许获得移动网络定位信息改变 |
| CWJ_GROUP | 底层访问权限 | 允许CWJ账户组访问底层信息 |
| DELETE_CACHE_FILES | 删除缓存文件 | 允许应用删除缓存文件 |
| DELETE_PACKAGES | 删除应用 | 允许程序删除应用 |
| DEVICE_POWER | 电源管理 | 允许访问底层电源管理 |
| DIAGNOSTIC | 应用诊断 | 允许程序到RW到诊断资源 |
| DISABLE_KEYGUARD | 禁用键盘锁 | 允许程序禁用键盘锁 |
| DUMP | 转存系统信息 | 允许程序获取系统dump信息从系统服务 |
| EXPAND_STATUS_BAR | 状态栏控制 | 允许程序扩展或收缩状态栏 |
| FACTORY_TEST | 工厂测试模式 | 允许程序运行工厂测试模式 |
| FLASHLIGHT | 使用闪光灯 | 允许访问闪光灯 |
| FORCE_BACK | 强制后退 | 允许程序强制使用back后退按键,无论Activity是否在顶层 |
| GET_ACCOUNTS | 访问账户Gmail列表 | 访问GMail账户列表 |
| GET_PACKAGE_SIZE | 获取应用大小 | 获取应用的文件大小 |
| GET_TASKS | 获取任务信息 | 允许程序获取当前或最近运行的应用 |
| GLOBAL_SEARCH | 允许全局搜索 | 允许程序使用全局搜索功能 |
| HARDWARE_TEST | 硬件测试 | 访问硬件辅助设备,用于硬件测试 |
| INJECT_EVENTS | 注射事件 | 允许访问本程序的底层事件,获取按键、轨迹球的事件流 |
| INSTALL_LOCATION_PROVIDER | 安装定位提供 | 安装定位提供 |
| INSTALL_PACKAGES | 安装应用程序 | 允许程序安装应用 |
| INTERNAL_SYSTEM_WINDOW | 内部系统窗口 | 允许程序打开内部窗口,不对第三方应用程序开放此权限 |
| INTERNET | 访问网络 | 访问网络连接,可能产生GPRS流量 |
| KILL_BACKGROUND_PROCESSES | 结束后台进程 | 允许程序调用killBackgroundProcesses(String).方法结束后台进程 |
| MANAGE_ACCOUNTS | 管理账户 | 允许程序管理AccountManager中的账户列表 |
| MANAGE_APP_TOKENS | 管理程序引用 | 管理创建、摧毁、Z轴顺序,仅用于系统 |
| MASTER_CLEAR | 软格式化 | 允许程序执行软格式化,删除系统配置信息 |
| MODIFY_AUDIO_SETTINGS | 修改声音设置 | 修改声音设置信息 |
| MODIFY_PHONE_STATE | 修改电话状态 | 修改电话状态,如飞行模式,但不包含替换系统拨号器界面 |
| MOUNT_FORMAT_FILESYSTEMS | 格式化文件系统 | 格式化可移动文件系统,比如格式化清空SD卡 |
| MOUNT_UNMOUNT_FILESYSTEMS | 挂载文件系统 | 挂载、反挂载外部文件系统 |
| MTWEAK_FORUM | 社区权限 | 允许使用mTweak社区权限 |
| MTWEAK_USER | 高级权限 | 允许mTweak用户访问高级系统权限 |
| NFC | 允许NFC通讯 | 允许程序执行NFC近距离通讯操作,用于移动支持 |
| PERSISTENT_ACTIVITY | 永久Activity | 创建一个永久的Activity,该功能标记为将来将被移除 |
| PROCESS_OUTGOING_CALLS | 处理拨出电话 | 允许程序监视,修改或放弃播出电话 |
| READ_CALENDAR | 读取日程提醒 | 允许程序读取用户的日程信息 |
| READ_CONTACTS | 读取联系人 | 允许应用访问联系人通讯录信息 |
| READ_FRAME_BUFFER | 屏幕截图 | 读取帧缓存用于屏幕截图 |
| READ_INPUT_STATE | 读取输入状态 | 读取当前键的输入状态,仅用于系统 |
| READ_LOGS | 读取系统日志 | 读取系统底层日志 |
| READ_PHONE_STATE | 读取电话状态 | 访问电话状态 |
| READ_SMS | 读取短信内容 | 读取短信内容 |
| READ_SYNC_SETTINGS | 读取同步设置 | 读取同步设置,读取Google在线同步设置 |
| READ_SYNC_STATS | 读取同步状态 | 读取同步状态,获得Google在线同步状态 |
| REBOOT | 重启设备 | 允许程序重新启动设备 |
| RECEIVE_BOOT_COMPLETED | 开机自动允许 | 允许程序开机自动运行 |
| RECEIVE_MMS | 接收彩信 | 接收彩信 |
| RECEIVE_SMS | 接收短信 | 接收短信 |
| RECEIVE_WAP_PUSH | 接收Wap Push | 接收WAP PUSH信息 |
| RECORD_AUDIO | 录音 | 录制声音通过手机或耳机的麦克 |
| REORDER_TASKS | 排序系统任务 | 重新排序系统Z轴运行中的任务 |
| RESTART_PACKAGES | 结束系统任务 | 结束任务通过restartPackage(String)方法,该方式将在外来放弃 |
| SEND_SMS | 发送短信 | 发送短信 |
| SET_ACTIVITY_WATCHER | 设置Activity观察其 | 设置Activity观察器一般用于monkey测试 |
| SET_ALWAYS_FINISH | 设置总是退出 | 设置程序在后台是否总是退出 |
| SET_ANIMATION_SCALE | 设置动画缩放 | 设置全局动画缩放 |
| SET_DEBUG_APP | 设置调试程序 | 设置调试程序,一般用于开发 |
| SET_ORIENTATION | 设置屏幕方向 | 设置屏幕方向为横屏或标准方式显示,不用于普通应用 |
| SET_PREFERRED_APPLICATIONS | 设置应用参数 | 设置应用的参数,已不再工作具体查看addPackageToPreferred(String)介绍 |
| SET_PROCESS_LIMIT | 设置进程限制 | 允许程序设置最大的进程数量的限制 |
| SET_TIME | 设置系统时间 | 设置系统时间 |
| SET_TIME_ZONE | 设置系统时区 | 设置系统时区 |
| SET_WALLPAPER | 设置桌面壁纸 | 设置桌面壁纸 |
| SET_WALLPAPER_HINTS | 设置壁纸建议 | 设置壁纸建议 |
| SIGNAL_PERSISTENT_PROCESSES | 发送永久进程信号 | 发送一个永久的进程信号 |
| STATUS_BAR | 状态栏控制 | 允许程序打开、关闭、禁用状态栏 |
| SUBSCRIBED_FEEDS_READ | 访问订阅内容 | 访问订阅信息的数据库 |
| SUBSCRIBED_FEEDS_WRITE | 写入订阅内容 | 写入或修改订阅内容的数据库 |
| SYSTEM_ALERT_WINDOW | 显示系统窗口 | 显示系统窗口 |
| UPDATE_DEVICE_STATS | 更新设备状态 | 更新设备状态 |
| USE_CREDENTIALS | 使用证书 | 允许程序请求验证从AccountManager |
| USE_SIP | 使用SIP视频 | 允许程序使用SIP视频服务 |
| VIBRATE | 使用振动 | 允许振动 |
| WAKE_LOCK | 唤醒锁定 | 允许程序在手机屏幕关闭后后台进程仍然运行 |
| WRITE_APN_SETTINGS | 写入GPRS接入点设置 | 写入网络GPRS接入点设置 |
| WRITE_CALENDAR | 写入日程提醒 | 写入日程,但不可读取 |
| WRITE_CONTACTS | 写入联系人 | 写入联系人,但不可读取 |
| WRITE_EXTERNAL_STORAGE | 写入外部存储 | 允许程序写入外部存储,如SD卡上写文件 |
| WRITE_GSERVICES | 写入Google地图数据 | 允许程序写入Google Map服务数据 |
| WRITE_SECURE_SETTINGS | 读写系统敏感设置 | 允许程序读写系统安全敏感的设置项 |
| WRITE_SETTINGS | 读写系统设置 | 允许读写系统设置项 |
| WRITE_SMS | 编写短信 | 允许编写短信 |

| 权限 | 名称 | 描述 |
| :--------------- | :------------------- | :----------------------------------------------------------- |
| com.android.alarm.permission.SET_ALARM | 设置闹铃提醒 | 设置闹铃提醒 |
| com.android.browser.permission.READ_HISTORY_BOOKMARKS | 读取收藏夹和历史记录 | 读取浏览器收藏夹和历史记录 |
| com.android.browser.permission.WRITE_HISTORY_BOOKMARKS | 写入收藏夹和历史记录 | 写入浏览器历史记录或收藏夹,但不可读取 |


