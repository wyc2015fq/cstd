# Android FrameWork Service 之 StatusBarService - xqhrs232的专栏 - CSDN博客
2012年02月29日 16:28:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：652标签：[service																[android																[null																[action																[signal																[frameworks](https://so.csdn.net/so/search/s.do?q=frameworks&t=blog)](https://so.csdn.net/so/search/s.do?q=signal&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/andy_android/article/details/7194589](http://blog.csdn.net/andy_android/article/details/7194589)
### android的状态栏：
![](http://www.bangchui.org/attachment/photo/Mon_1102/16_39011297996990d29be2bf2ffede8.gif)
在android中，状态栏不属于某个应用程序，而是系统的一部分，是通过StatusBarService来管理的。
Android通过StatusBarPolicy及StatusBarService来管理状态栏。
一．    StatusBarPolicy 负责状态栏显示的策略管理
Android中状态栏上有很多图标，这些图标什么时候显示什么时候不显示？这些都是 StatusBarPolicy来管理的。
StatusBarPolicy的构造函数里初始化了好几个图标，如时间icon，闹钟icon，信号栏icon等。默认时有很多图标是不显示的，需要显示时再进行更新。StatusBarPolicy调用 registerReceiver注册了感兴趣的intent,当感兴趣的intent发生时，对图标进行更新。例如，设置一个闹钟后，闹钟模块会发出一个叫做Intent.ACTION_ALARM_CHANGED的intent，然后StatusBarPolicy接收到此intent，继而更新状态栏上的闹钟图标。
见如下的代码：
- private StatusBarPolicy(Context context, StatusBarService service) {
        mContext = context;
        mService = service;
        // clock
        mCalendar = Calendar.getInstance(TimeZone.getDefault());
        mClockData = IconData.makeText("clock", "");
        mClockIcon = service.addIcon(mClockData, null);
        updateClock();
        // battery
        mBatteryData = IconData.makeIcon("battery",
                null, com.android.internal.R.drawable.stat_sys_battery_unknown, 0, 0);
        mBatteryIcon = service.addIcon(mBatteryData, null);
        // phone_signal
        mPhone = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
        mPhoneData = IconData.makeIcon("phone_signal", 
                null, com.android.internal.R.drawable.stat_sys_signal_null, 0, 0);
        mPhoneIcon = service.addIcon(mPhoneData, null);
        // register for phone state notifications.
        ((TelephonyManager)mContext.getSystemService(Context.TELEPHONY_SERVICE))
                .listen(mPhoneStateListener,
                          PhoneStateListener.LISTEN_SERVICE_STATE
                        | PhoneStateListener.LISTEN_SIGNAL_STRENGTH
                        | PhoneStateListener.LISTEN_CALL_STATE
                        | PhoneStateListener.LISTEN_DATA_CONNECTION_STATE
                        | PhoneStateListener.LISTEN_DATA_ACTIVITY);
        // data_connection
        mDataData = IconData.makeIcon("data_connection",
                null, com.android.internal.R.drawable.stat_sys_data_connected_g, 0, 0);
        mDataIcon = service.addIcon(mDataData, null);
        service.setIconVisibility(mDataIcon, false);
        // wifi
        mWifiData = IconData.makeIcon("wifi", null, sWifiSignalImages[0], 0, 0);
        mWifiIcon = service.addIcon(mWifiData, null);
        service.setIconVisibility(mWifiIcon, false);
        // wifi will get updated by the sticky intents
        // bluetooth status
        mBluetoothData = IconData.makeIcon("bluetooth",
                null, com.android.internal.R.drawable.stat_sys_data_bluetooth, 0, 0);
        mBluetoothIcon = service.addIcon(mBluetoothData, null);
        updateBluetooth(null);
        // Gps status
        mGpsEnabledIconData = IconData.makeIcon("gps",
                null, com.android.internal.R.drawable.stat_sys_gps_acquiring_anim, 0, 0);
        mGpsFixIconData = IconData.makeIcon("gps",
                null, com.android.internal.R.drawable.stat_sys_gps_on, 0, 0);
        mGpsIcon = service.addIcon(mGpsEnabledIconData, null);
        service.setIconVisibility(mGpsIcon, false);           
        // Alarm clock
        mAlarmClockIconData = IconData.makeIcon(
                "alarm_clock",
                null, com.android.internal.R.drawable.stat_notify_alarm, 0, 0);
        mAlarmClockIcon = service.addIcon(mAlarmClockIconData, null);
        service.setIconVisibility(mAlarmClockIcon, false);
        // Sync state
        mSyncActiveIcon = service.addIcon(IconData.makeIcon("sync_active",
                null, R.drawable.stat_notify_sync_anim0, 0, 0), null);
        mSyncFailingIcon = service.addIcon(IconData.makeIcon("sync_failing",
                null, R.drawable.stat_notify_sync_error, 0, 0), null);
        service.setIconVisibility(mSyncActiveIcon, false);
        service.setIconVisibility(mSyncFailingIcon, false);
        // volume
        mVolumeData = IconData.makeIcon("volume",
                null, com.android.internal.R.drawable.stat_sys_ringer_silent, 0, 0);
        mVolumeIcon = service.addIcon(mVolumeData, null);
        service.setIconVisibility(mVolumeIcon, false);
        IntentFilter filter = new IntentFilter();
        // Register for Intent broadcasts for...
        filter.addAction(Intent.ACTION_TIME_TICK);
        filter.addAction(Intent.ACTION_TIME_CHANGED);
        filter.addAction(Intent.ACTION_CONFIGURATION_CHANGED);
        filter.addAction(Intent.ACTION_BATTERY_CHANGED);
        filter.addAction(Intent.ACTION_TIMEZONE_CHANGED);
        filter.addAction(Intent.ACTION_ALARM_CHANGED);
        filter.addAction(Intent.ACTION_SYNC_STATE_CHANGED);
        filter.addAction(AudioManager.RINGER_MODE_CHANGED_ACTION);
        filter.addAction(AudioManager.VIBRATE_SETTING_CHANGED_ACTION);
        filter.addAction(BluetoothIntent.ENABLED_ACTION);
        filter.addAction(BluetoothIntent.DISABLED_ACTION);
        filter.addAction(BluetoothIntent.HEADSET_STATE_CHANGED_ACTION);
        filter.addAction(BluetoothA2dp.SINK_STATE_CHANGED_ACTION);
        filter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        filter.addAction(WifiManager.SUPPLICANT_CONNECTION_CHANGE_ACTION);
        filter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        filter.addAction(WifiManager.RSSI_CHANGED_ACTION);
        filter.addAction(GpsLocationProvider.GPS_ENABLED_CHANGE_ACTION);
        filter.addAction(GpsLocationProvider.GPS_FIX_CHANGE_ACTION);
        filter.addAction(TelephonyIntents.ACTION_SIM_STATE_CHANGED);
        mContext.registerReceiver(mIntentReceiver, filter, null, mHandler);
    }
二．    StatusBarService
正如上面看到的，StatusBarPolicy只是一个策略管理，实际的功能是StatusBarService来实现的。StatusBarService初始化时初始化了一个用于显示statusbar 的StatusBarView。StatusBarView里面定义了icon名字，的显示顺序，对应的png图等。代码如下：
- public StatusBarService(Context context) {
        mContext = context;
        mDisplay = ((WindowManager)context.getSystemService(
                Context.WINDOW_SERVICE)).getDefaultDisplay();
        makeStatusBarView(context);
        mUninstallReceiver = new UninstallReceiver();
    }
  private void makeStatusBarView(Context context) {
        Resources res = context.getResources();
        mRightIconSlots = res.getStringArray(com.android.internal.R.array.status_bar_icon_order);
        mRightIcons = new StatusBarIcon[mRightIconSlots.length];
        ExpandedView expanded = (ExpandedView)View.inflate(context,
                com.android.internal.R.layout.status_bar_expanded, null);
        expanded.mService = this;
        StatusBarView sb = (StatusBarView)View.inflate(context,
                com.android.internal.R.layout.status_bar, null);
        sb.mService = this;
……
}
mRightIconSlots 是一个String型数组，里面记录了状态栏上各个图标的名字。StatusBarPolicy就是根据这个名字找到相应的icon，调用setIcon设置其图标。mRightIconSlots的值从\frameworks\base\core\res\res\values\arrays.xml 中获得，现摘取片段如下：
- <string-array name="status_bar_icon_order">
        <item>clock</item>
        <item>alarm_clock</item>
        <item>battery</item>
        <item>phone_signal</item>
        <item>data_connection</item>
        <item>volume</item>
        <item>mute</item>
        <item>speakerphone</item>
        <item>wifi</item>
        <item>bluetooth</item>
        <item>gps</item>
        <item>sync_active</item>
        <item>sync_failing</item>
        <item>ime</item>
    </string-array>
由此已经可以看出要增加一个系统级图标，应该在此xml中增加相应的item，然后在StatusBarPolicy中做相应修改即可。
三   代码例子
      下面给个例子
 原生态的android系统是没有耳机插入或未插入的状态指示的，本文就是讲解如何添加耳机插入的状态指示。效果图如下：
![](http://hi.csdn.net/attachment/201201/11/0_13262794793YbJ.gif)
如图，当插入耳机后，在status bar上出现了一个耳机的图标。
这个耳机的图标和sim卡信号等图标在status bar的右边，因此这个实现肯定不是通过notification实现的，那么添加这个功能只能在framwork里面寻找了。具体的修改步骤如下：
1.在frameworks/base/core/res/res/drawable-mdpi目录先添加一个stat_sys_headset.png资源文件。这里面资源文件的目录要根据设备的分辨率来确定，我的设备是HVGA的，因此资源文件放在drawable-mdpi目录下。
2.修改frameworks/base/services/java/com/android/server/status/StatusBarPolicy.java
publicclass StatusBarPolicy { 
privatestaticfinalString TAG =
"StatusBarPolicy"; 
private
static StatusBarPolicy sInstance; 
    ..... 
// alarm clock 
// Icon lit when clock is set 
private IBinder mAlarmClockIcon; 
private IconData mAlarmClockIconData; 
//modify here start
//HeadSet 
private IBinder mHeadSetPlugIcon; 
private IconData mHeadSetPlugIconData; 
//modify here end 
    ..... 
private BroadcastReceiver mIntentReceiver =
new BroadcastReceiver() { 
@Override
publicvoid onReceive(Context context, Intent intent) {
String action = intent.getAction(); 
if (action.equals(Intent.ACTION_TIME_TICK)) {
                updateClock(); 
            } 
            ..... 
elseif (action.equals(Intent.ACTION_ALARM_CHANGED)) {
                updateAlarm(intent); 
            } 
//modify here start 
elseif (action.equals(Intent.ACTION_HEADSET_PLUG)) {
                updateHeadset(intent); 
            } 
//modify here end 
elseif (action.equals(Intent.ACTION_SYNC_STATE_CHANGED)) {
                updateSyncState(intent); 
            } 
            ...... 
     }; 
private StatusBarPolicy(Context context, StatusBarService service) {
        mContext = context; 
        mService = service; 
        mSignalStrength = new SignalStrength(); 
if(FeatureOption.MTK_GEMINI_SUPPORT == 
true) { 
            mSignalStrengthGemini = new SignalStrength();
        } 
        mBatteryStats = BatteryStatsService.getService(); 
        .... 
//modify here start 
//HeadSet 
        mHeadSetPlugIconData = IconData.makeIcon( 
"headset", 
null,com.android.internal.R.drawable.stat_sys_headset,
0, 0); 
        mHeadSetPlugIcon = service.addIcon(mHeadSetPlugIconData, 
null); 
        service.setIconVisibility(mHeadSetPlugIcon, false);
//modify here end 
        .... 
        IntentFilter filter = 
new IntentFilter(); 
// Register for Intent broadcasts for...
        filter.addAction(Intent.ACTION_TIME_TICK); 
        .... 
//modify here start 
        filter.addAction(Intent.ACTION_HEADSET_PLUG); 
//modify here end 
        .... 
    } 
//modify here start
privatefinalvoid updateHeadset(Intent intent) { 
        Slog.d(TAG, 
"updateHeadset: state=" + intent.getIntExtra("state",
0)); 
        mService.setIconVisibility(mHeadSetPlugIcon, (intent.getIntExtra("state",
0) == 1)?true:false);
    } 
//modify here end 
    ..... 
}
3.修改frameworks/base/core/res/res/values/arrays.xml
<string-array
name="status_bar_icon_order">
<item><xliff:g
id="id">clock</xliff:g></item>
<item><xliff:g
id="id">secure</xliff:g></item>
<item><xliff:g
id="id">alarm_clock</xliff:g></item>
<item><xliff:g
id="id">battery</xliff:g></item>
<item><xliff:g
id="id">phone_signal_2</xliff:g></item>
<item><xliff:g
id="id">phone_signal</xliff:g></item>
<item><xliff:g
id="id">phone_evdo_signal</xliff:g></item>
<item><xliff:g
id="id">data_connection</xliff:g></item>
<item><xliff:g
id="id">cdma_eri</xliff:g></item>
<item><xliff:g
id="id">tty</xliff:g></item>
<item><xliff:g
id="id">volume</xliff:g></item>
<item><xliff:g
id="id">mute</xliff:g></item>
<item><xliff:g
id="id">speakerphone</xliff:g></item>
<!-- modify here start. -->
<item><xliff:g
id="id">headset</xliff:g></item>
<!-- modify here end -->
<item><xliff:g
id="id">wifi</xliff:g></item>
<item><xliff:g
id="id">tty</xliff:g></item>
<item><xliff:g
id="id">bluetooth</xliff:g></item>
<item><xliff:g
id="id">gps</xliff:g></item>
<item><xliff:g
id="id">sync_active</xliff:g></item>
<item><xliff:g
id="id">sync_failing</xliff:g></item>
<item><xliff:g
id="id">ime</xliff:g></item>
</string-array>
其中“headset”这个字符串就是“mHeadSetPlugIconData = IconData.makeIcon(
"headset", null0,
0);”中的第一个参数。“<item><xliff:g
id="id">headset</xliff:g></item> ”这个语句的位置于图标在status bar上显示的位置有关。
按照这3个步骤修改后重新编译源码之后，就可以看到如上图的效果了。
