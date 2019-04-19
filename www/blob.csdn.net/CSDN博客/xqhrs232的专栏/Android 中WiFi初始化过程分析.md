# Android 中WiFi初始化过程分析 - xqhrs232的专栏 - CSDN博客
2012年01月01日 20:44:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1785
原文地址::[http://blog.csdn.net/anskya520/article/details/6912528](http://blog.csdn.net/anskya520/article/details/6912528)
Android的WiFi
我们通常看到WiFi的守护进程wpa_supplicant在我们的ps的进程列表中，这个就是我们的wifi守护进程。wpa_supplicant在external/wpa_supplicant里实现
wpa_supplicant适配层是通用的wpa_supplicant的封装，在Android中作为WIFI部分的硬件抽象层来使用。wpa_supplicant适配层主要用于封装与wpa_supplicant守护进程的通信，以提供给Android框架使用。它实现了加载，控制和消息监控等功能。
wpa_supplicant适配层的头文件如下所示：
hardware/libhardware_legacy/include/hardware_legacy/wifi.h
我们看它的加载过程
Init会在系统启动首先加载init.rc这个文件会加载所有service，这是linux启动的第一个用户空间的应用（属于linux进程，不属于Android应用）。
Service wpa_supplicant /system/bin/wpa_supplicant –Dwext –iwlan0 –d –c /data/misc/wifi/wpa_supplicant.conf
#user wifi
#group wifi system
Socket wpa_eth0 dgram 0660 wifi system
Disabled
Oneshot
Serive dhcpcd /system/bin/dhcpcd –f /system/etc/dhcpcd/dhcpcd.conf –d eth0
Disabled
Onshot
On property:init.svc.wpa_supplicant=stopped
Stop dhcpcd
添加/system/etc/wifi/wpa_supplicant.conf
Update_config=1
Ctrl_interface=/data/system/wpa_supplicant //和IFACE_DIR对应
Eapol_verison=1
Ap_scan=1
Fast_reauth=1
通过linux内核模块/system/lib/modules/wlan.ko 这个wifi模块定义在/hardware/libhardware_legacy/wifi/wifi.c
当SystemServer启动后会加载一系列的Service其中init2启动的就有ConnectivityService。ConnectivityService.java (frameworks/base/services/java/com/android/server)会管理所有的Connectivity相关的比如APN，WiFi。看看是怎么启动WiFi
 Service的：
if (DBG) Log.v(TAG, "Starting Wifi Service.");
WifiStateTracker wst = new WifiStateTracker(context, mHandler);
WifiService wifiService = new WifiService(context, wst);
ServiceManager.addService(Context.WIFI_SERVICE, wifiService);
WifiStateTracker会创建WifMonitor来接受来自底层的事件。WifiService和WifiMonitor是整个模块的核心部分，WifiService负责启动关闭wpa_supplicant、发命令给wpa_supplicant进程，WiFiMonitor负责从wpa_supplicant接收事件
整个流程是
SystemServer -> ServerThread -> ConnectivityService -> ConnectivityThread -> WifiTracker->WifiService -> WifiMonitor
WiFi 的启动过程
用户在设置界面下开启了WiFi，调用应用程序Settings中的setWifiEnabler的onPerferenceChange,再由WifiEnable调用WifiService，发送MESSAGE_ENABLE_WIFI,首先装载wifi内核模块wlan.ko然后启动wpa_supplicant（用/data/misc/wifi/wpa_supplicant.conf配置），再通过WifiStateTracker来启动WifiMonitor监视线程
WifiSettings.java (packages/apps/settings/src/com/android/settings/wifi)启动
mWifiEnabled = (CheckBoxPreference) preferenceScreen.findPreference(KEY_WIFI_ENABLED);
mWifiEnabler = new WifiEnabler(this, (WifiManager) getSystemService(WIFI_SERVICE),
mWifiEnabled);
这样就启动WifiEnabler
WifiEnabler.java (packages/apps/settings/src/com/android/settings/wifi)通过WifiManager调用WifiManager.java (frameworks/base/wifi/java/android/net/wifi) setWifiEnabled 中的 IWifiManager来启动wifiservice[mService.setWifiEnabled(enabled);]
WifiService.java (frameworks/base/services/java/com/android/server)又setWifiEnabled()这个里面的sendEnableMessage(enable, true, Binder.getCallingUid());来发送一则消息
Message msg = Message.obtain(mWifiHandler,
(enable ? MESSAGE_ENABLE_WIFI : MESSAGE_DISABLE_WIFI),
(persist ? 1 : 0), uid);
msg.sendToTarget();发送给自身的消息。
通过WifiHandler的 handleMessage来维护这些消息，enable的时候会调用setWifiEnabledBlocking这个函数，这个函数会做setWifiEnabledState 然后做两件事： 1. 调用wifi 本地方法JNI的WifiNative.loadDriver
下面说本地方法WifiNative.loadDriver函数 WifiNative.java (frameworks/base/wifi/java/android/net/wifi) Android的WIFI系统的JNI的部分：
frameworks/base/core/jni/android_net_wifi_Wifi.cpp 中的android_net_wifi_loadDriver()可以把wifi驱动模块装载
Wifi.c (hardware/libhardware_legacy/wifi) 内核模块/system/lib/modules/wlan.ko中的wifi_load_driver()
设置wlan.driver.status属性为ok，至此wifi模块加载完毕。
2. 再来看看启动，同样是在WifiService 中的setWifiEnabledBlocking这个函数会调用startSupplicant 通过WifiNative.java (frameworks/base/wifi/java/android/net/wifi)的startSupplicant来启动JNI：frameworks/base/core/jni/android_net_wifi_Wifi.cpp的android_net_wifi_startSupplicant调用驱动模块Wifi.c
 (hardware/libhardware_legacy/wifi) wlan.ko中的wifi_start_supplicant, Wifi 启动完毕
成功启动wifi之后setWifiEnabledBlocking运行mWifiStateTracker.startEventLoop();事件循环，来监视事件mWifiMonitor.startMonitoring(); à MonitorThread().start();一直在线程里循环调用WifiNative.waitForEvent();最后调用
setWifiEnabledState(eventualWifiState, uid); intent = new Intent(WifiManager.WIFI_STATE_CHANGED_ACTION);广播消息向外界通知wifi已经成功启动了。
查找热点AP
上面说了WifiManager发送广播WIFI_STATE_CHANGED_ACTION，只要Android应用注册了接受该Action的就接受，我们的WifiLayer注册了接收到该Action
WifiSettings.java (packages/apps/settings/src/com/android/settings/wifi)中有mWifiLayer.onCreate();（这个函数创建WifiLayer指定接受的Action）
WifiLayer.java (packages/apps/settings/src/com/android/settings/wifi)中的BroadcastReceiver 有一句话else if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
handleWifiStateChanged(intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE,
WifiManager.WIFI_STATE_UNKNOWN));
这个函数会调用loadConfiguredAccessPoints和attemptScan来开始扫描，调用WifiManager的mWifiManager.startScanActive，WifiManager.java中的mService.startScan通过WifiService中的startScan通过本地方法WifiNative.setScanResultHandlingCommand启动JNI
 android_net_wifi_Wifi.cpp (frameworks/base/core/jni) 中的android_net_wifi_setScanResultHandlingCommand的命令“AP_SCAN 模式” Wifi.c ::wifi_command(cmd)开始扫描wifi_send_command发出SCAN命令调用wpa_supplicant开始扫描
扫描完成之后会发送SCAN_RESULT 在WifiMonitor的HandleEvent里处理调用mWifiStateTracker.notifyScanResultsAvailable(); à sendEmptyMessage(EVENT_SCAN_RESULTS_AVAILABLE); mWifiStateTracker中的 handleMessage接收到case
 EVENT_SCAN_RESULTS_AVAILABLE:之后发送广播mContext.sendBroadcast(new Intent(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));
WiFiLayer接收到这个消息在mReceiver = new BroadcastReceiver()中处理handleScanResultsAvailable();
WiFi 连接流程
用户在AccessPointDialog中输入密码之后点击连接按钮，Android调用顺序如下：
AccessPointDialog.java (packages/apps/settings/src/com/android/settings/wifi) -> onClick -> handleConnect(); -> mWifiLayer.connectToNetwork ->通过WifiConfiguration config = findConfiguredNetwork(state);查看是不是配置过的，如果是就直接使用了，如果不是config
 = addConfiguration(state, 0); -> managerEnableNetwork -> mWifiManager.enableNetwork -> mService.enableNetwork -> WifiService. enableNetwork -> WifiNative.enableNetworkCommand -> JNI: android_net_wifi_Wifi.cpp android_net_wifi_enableNetworkCommand 调用wpa_suppcant发送相关命令返回之后由WiFiMonitor处理跟以前类似，连接的中间流程与查找AP的流程类似，都经过了WifiMonitor对“CONNECTED”消息响应的捕获，以及WifiStateTracker对EVENT_SUPPLICANT_STATE_
 CHANGED的处理。还有一个比较重
要的步骤是WifiStateTracker通过对DHCP服务器的申请进行了IP地址分配。最终会广播NETWORK_STATE_CHANGED_ ACTION消息，由WifiLayer响应。
IP地址分配
由上面继续说IP地址分配，因为当wpa_supplicant链接AP成功之后，它会发出事件从而wifi_for_event函数会接收到该事件，由WifiMonitor中的MonitorThread执行执行这个事件handleEvent-> case CONNECTED: handleNetworkStateChange -> mWifiStateTracker.notifyStateChange
 -> EVENT_NETWORK_STATE_CHANGED -> handleMessage 下的：case EVENT_SUPPLICANT_STATE_CHANGED: -> intent = new Intent(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION); Wi-Fi supplicant state changed:
èSettingsObserver专门是观察该类变化的
if (changed) {
resetInterface(true);
configureInterface();
if (mUseStaticIp) {
mTarget.sendEmptyMessage(EVENT_CONFIGURATION_CHANGED);
}
}
è
mDhcpTarget.sendEmptyMessage(EVENT_DHCP_START);
->
DhcpHandler的handleMessage函数case EVENT_DHCP_START: NetworkUtils.runDhcp获取DHCP的IP地址，成功之后发送EVENT_INTERFACE_CONFIGURATION_SUCCEEDED:
event通过WifiStateTracker的HandleMessage函数case EVENT_INTERFACE_CONFIGURATION_SUCCEEDED:会调用sendNetworkStateChangeBroadcast Intent intent = new Intent(WifiManager.NETWORK_STATE_CHANGED_ACTION);发送全局Intent
 Action 完成网络切换。
**-------------------------------------------------------------------WIFI启动 代码流程走读**
初始化
在 SystemServer 启动的时候,会生成一个 ConnectivityService 的实例 ,
try {
Log.i(TAG, "Starting Connectivity Service.");
ServiceManager.addService(Context.CONNECTIVITY_SERVICE, new
ConnectivityService(context));
} catch (Throwable e) {
Log.e(TAG, "Failure starting Connectivity Service", e);
}
ConnectivityService 的构造函数 会创建 WifiService,
if (DBG) Log.v(TAG, "Starting Wifi Service.");
mWifiStateTracker = new WifiStateTracker(context, handler);
WifiService wifiService = new WifiService(context, mWifiStateTracker);
ServiceManager.addService(Context.WIFI_SERVICE, wifiService);
WifiStateTracker 会创建 WifiMonitor 接收来自底层的事件,WifiService 和 WifiMonitor 是整
个模块的核心 。WifiService 负责启动关闭 wpa_supplicant、启动关闭 WifiMonitor 监视线程
和把命令 下发给 wpa_supplicant,而 WifiMonitor 则负责从 wpa_supplicant 接收事件通知。
连接 AP
1. 使能 WIFI
WirelessSettings 在初始化的时候配置了由 WifiEnabler 来处理 Wifi 按钮,
private void initToggles() {
mWifiEnabler = new WifiEnabler(
this,
(WifiManager) getSystemService(WIFI_SERVICE),
(CheckBoxPreference) findPreference(KEY_TOGGLE_WIFI));
当用户按下 Wifi 按钮后, Android 会调用 WifiEnabler 的 onPreferenceChange, 再由 WifiEnabler
调用 WifiManager 的 setWifiEnabled 接口 函数,通过 AIDL,实际调用的是 WifiService 的
setWifiEnabled 函数,WifiService 接着向自身发送一条 MESSAGE_ENABLE_WIFI 消息,在
处理该消息的代码 中做真正的使能工作:首先装载 WIFI 内核模块(该模块的位置硬编码为
"/system/lib/modules/wlan.ko" ), 然 后 启 动 wpa_supplicant ( 配 置 文 件 硬 编 码 为
"/data/misc/wifi/wpa_supplicant.conf") 再通过 WifiStateTracker 来启动 WifiMonitor 中的监视
,
线程。
private boolean setWifiEnabledBlocking(boolean enable) {
final int eventualWifiState = enable ? WIFI_STATE_ENABLED :
WIFI_STATE_DISABLED;
updateWifiState(enable ? WIFI_STATE_ENABLING : WIFI_STATE_DISABLING);
if (enable) {
if (!WifiNative.loadDriver()) {
Log.e(TAG, "Failed to load Wi-Fi driver.");
updateWifiState(WIFI_STATE_UNKNOWN);
return false;
}
if (!WifiNative.startSupplicant()) {
WifiNative.unloadDriver();
Log.e(TAG, "Failed to start supplicant daemon.");
updateWifiState(WIFI_STATE_UNKNOWN);
return false;
}
mWifiStateTracker.startEventLoop();
}
// Success!
persistWifiEnabled(enable);
updateWifiState(eventualWifiState);
return true;
}
当使能成功后,会广播发送 WIFI_STATE_CHANGED_ACTION 这个 Intent 通知外界 WIFI
已 经 成 功 使 能 了 。 WifiEnabler 创 建 的 时 候 就 会 向 Android 注 册 接 收
WIFI_STATE_CHANGED_ACTION,因此它会收到该 Intent,从而开始扫描。
private void handleWifiStateChanged(int wifiState) {
if (wifiState == WIFI_STATE_ENABLED) {
loadConfiguredAccessPoints();
attemptScan();
}
2. 查找 AP
扫描的入口函数是 WifiService 的 startScan,它其实也就是往 wpa_supplicant 发送 SCAN 命
令。
static jboolean android_net_wifi_scanCommand(JNIEnv* env, jobject clazz)
{
jboolean result;
// Ignore any error from setting the scan mode.
// The scan will still work.
(void)doBooleanCommand("DRIVER SCAN-ACTIVE", "OK");
result = doBooleanCommand("SCAN", "OK");
(void)doBooleanCommand("DRIVER SCAN-PASSIVE", "OK");
return result;
}
当 wpa_supplicant 处理完 SCAN 命令后,它会向控制通道发送事件通知扫描完成,从而
wifi_wait_for_event 函数会接收到该事件,由此 WifiMonitor 中的 MonitorThread 会被执行来
出来这个事件,
void handleEvent(int event, String remainder) {
case SCAN_RESULTS:
mWifiStateTracker.notifyScanResultsAvailable();
break;
WifiStateTracker 则接着广播发送 SCAN_RESULTS_AVAILABLE_ACTION 这个 Intent
case EVENT_SCAN_RESULTS_AVAILABLE:
mContext.sendBroadcast(new
Intent(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));
WifiLayer 注册了接收 SCAN_RESULTS_AVAILABLE_ACTION 这个 Intent,所以它的相关
处理函数 handleScanResultsAvailable 会被调用,在该函数中,先会去拿到 SCAN 的结果(最
终是往 wpa_supplicant 发送 SCAN_RESULT 命令并读取返回值来实现的) ,
List<ScanResult> list = mWifiManager.getScanResults();
对每一个扫描返回的 AP,WifiLayer 会调用 WifiSettings 的 onAccessPointSetChanged 函数,
从而最终把该 AP 加到 GUI 显示列表中。
public void onAccessPointSetChanged(AccessPointState ap, boolean added) {
AccessPointPreference pref = mAps.get(ap);
if (added) {
if (pref == null) {
pref = new AccessPointPreference(this, ap);
mAps.put(ap, pref);
} else {
pref.setEnabled(true);
}
mApCategory.addPreference(pref);
}
}
3. 配置 AP 参数
当用户在 WifiSettings 界面上选择了一个 AP 后,会显示配置 AP 参数的一个对话框,
public boolean onPreferenceTreeClick(PreferenceScreen preferenceScreen, Preference
preference) {
if (preference instanceof AccessPointPreference) {
AccessPointState state = ((AccessPointPreference)
preference).getAccessPointState();
showAccessPointDialog(state, AccessPointDialog.MODE_INFO);
}
}
4. 连接
当用户在 AcessPointDialog 中选择好加密方式和输入密钥之后,再点击连接按钮,Android
就会去连接这个 AP。
private void handleConnect() {
String password = getEnteredPassword();
if (!TextUtils.isEmpty(password)) {
mState.setPassword(password);
}
mWifiLayer.connectToNetwork(mState);
}
WifiLayer 会先检测这个 AP 是不是之前被配置过,这个是通过向 wpa_supplicant 发送
LIST_NETWORK 命令并且比较返回值来实现的,
// Need WifiConfiguration for the AP
WifiConfiguration config = findConfiguredNetwork(state);
如果 wpa_supplicant 没有这个 AP 的配置信息, 则会向 wpa_supplicant 发送 ADD_NETWORK
命令来添加该 AP,
if (config == null) {
// Connecting for the first time, need to create it
config = addConfiguration(state,
ADD_CONFIGURATION_ENABLE|ADD_CONFIGURATION_SAVE);
}
ADD_NETWORK 命 令 会 返 回 一 个 ID , WifiLayer 再 用 这 个 返 回 的 ID 作 为 参 数 向
wpa_supplicant 发送 ENABLE_NETWORK 命令,从而让 wpa_supplicant 去连接该 AP。
// Make sure that network is enabled, and disable others
mReenableApsOnNetworkStateChange = true;
if (!mWifiManager.enableNetwork(state.networkId, true)) {
Log.e(TAG, "Could not enable network ID " + state.networkId);
error(R.string.error_connecting);
return false;
}
5. 配置 IP 地址
当 wpa_supplicant 成功连接上 AP 之后,它会向控制通道发送事件通知连接上 AP 了,从而
wifi_wait_for_event 函数会接收到该事件,由此 WifiMonitor 中的 MonitorThread 会被执行来
出来这个事件,
void handleEvent(int event, String remainder) {
case CONNECTED:
handleNetworkStateChange(NetworkInfo.DetailedState.CONNECTED,
remainder);
break;
WifiMonitor 再调用 WifiStateTracker 的 notifyStateChange,WifiStateTracker 则接着会往自身
发送 EVENT_DHCP_START 消息来启动 DHCP 去获取 IP 地址,
private void handleConnectedState() {
setPollTimer();
mLastSignalLevel = -1;
if (!mHaveIPAddress & !mObtainingIPAddress) {
mObtainingIPAddress = true;
mDhcpTarget.obtainMessage(EVENT_DHCP_START).sendToTarget();
}
}
然后再广播发送 NETWORK_STATE_CHANGED_ACTION 这个 Intent
case EVENT_NETWORK_STATE_CHANGED:
if (result.state != DetailedState.DISCONNECTED || !mDisconnectPending) {
intent = new
Intent(WifiManager.NETWORK_STATE_CHANGED_ACTION);
intent.putExt ra(WifiManager.EXTRA_NETWORK_INFO,
mNetworkInfo);
if (result.BSSID != null)
intent.putExtra(WifiManager.EXTRA_BSSID, result.BSSID);
mContext.sendStickyBroadcast(intent);
}
break;
WifiLayer 注册了接收 NETWORK_STATE_CHANGED_ACTION 这个 Intent,所以它的相关
处理函数 handleNetworkStateChanged 会被调用,
当 DHCP 拿到 IP 地址之后,会再发送 EVENT_DHCP_SUCCEEDED 消息,
private class DhcpHandler extends Handler {
public void handleMessage(Message msg) {
switch (msg.what) {
case EVENT_DHCP_START:
if (NetworkUtils.runDhcp(mInterfaceName, mDhcpInfo)) {
event = EVENT_DHCP_SUCCEEDED;
}
WifiLayer 处 理 EVENT_DHCP_SUCCEEDED 消 息 , 会 再 次 广 播 发 送
NETWORK_STATE_CHANGED_ACTION 这个 Intent,这次带上完整的 IP 地址信息。
case EVENT_DHCP_SUCCEEDED:
mWifiInfo.setIpAddress(mDhcpInfo.ipAddress);
setDetailedState(DetailedState.CONNECTED);
intent = new
Intent(WifiManager.NETWORK_STATE_CHANGED_ACTION);
intent.putExtra(WifiManager.EXTRA_NETWORK_INFO, mNetworkInfo);
mContext.sendStickyBroadcast(intent);
break;
至此为止,整个连接过程完成。
问题:
目前的实现不支持 Ad-hoc 方式。<!--++ plugin_code qcomic begin--> <!--++ plugin_code qcomic end-->
**---------------------------------------------------------------------------------android 网络连接管理**
系统中对网络的判断和选在是在Connectivityervice这个服务中来处理的，在系统启动的时候会启动这个系统服务：
系统启动完毕后，ConnectivityService在系统启动的时候就启动了。 
在android内部，用framework/base/core/res/res/values/config.xml中定义了网络的类型： 
<string-array translatable="false" name="networkAttributes"> 
<item>"default,wifi,0"</item> 
<item>"default,mobile,0"</item> 
<item>"mms,mobile,1"</item> 
<item>"supl,mobile,1"</item> 
<item>"dun,mobile,1"</item> 
<item>"hipri,mobile,2"</item> 
</string-array> 
<string-array translatable="false" name="radioAttributes"> 
<item>"wifi,1,1"</item> 
<item>"mobile,0,1"</item> 
</string-array> 
ConnectivityManager定义了向对应的字符串常量： 
public static final int TYPE_MOBILE = 0; 
/** 
* The Default WIFI data connection. When active, all data traffic 
* will use this connection by default. Should not coexist with other 
* default connections. 
*/ 
public static final int TYPE_WIFI = 1; 
/** 
* An MMS-specific Mobile data connection. This connection may be the 
* same as {@link #TYPEMOBILE} but it may be different. This is used 
* by applications needing to talk to the carrier's Multimedia Messaging 
* Service servers. It may coexist with default data connections. 
* {@hide} 
*/ 
public static final int TYPE_MOBILE_MMS = 2; 
/** 
* A SUPL-specific Mobile data connection. This connection may be the 
* same as {@link #TYPEMOBILE} but it may be different. This is used 
* by applications needing to talk to the carrier's Secure User Plane 
* Location servers for help locating the device. It may coexist with 
* default data connections. 
* {@hide} 
*/ 
public static final int TYPE_MOBILE_SUPL = 3; 
/** 
* A DUN-specific Mobile data connection. This connection may be the 
* same as {@link #TYPEMOBILE} but it may be different. This is used 
* by applicaitons performing a Dial Up Networking bridge so that 
* the carrier is aware of DUN traffic. It may coexist with default data 
* connections. 
* {@hide} 
*/ 
public static final int TYPE_MOBILE_DUN = 4; 
/** 
* A High Priority Mobile data connection. This connection is typically 
* the same as {@link #TYPEMOBILE} but the routing setup is different. 
* Only requesting processes will have access to the Mobile DNS servers 
* and only IP's explicitly requested via {@link #requestRouteToHost} 
* will route over this interface. 
*{@hide} 
*/ 
public static final int TYPE_MOBILE_HIPRI = 5; 
/** {@hide} */ 
public static final int MAX_RADIO_TYPE = TYPE_WIFI; 
/** {@hide} */ 
public static final int MAX_NETWORK_TYPE = TYPE_MOBILE_HIPRI; 
public static final int DEFAULT_NETWORK_PREFERENCE = TYPE_WIFI; 
并设置了默认的网络连接是TYPE_WIFI. 
ConnectivityManager的方法是通过AIDL的使用，调用ConnectivityService中的方法来实现的。ConnectivityService继承了IConnectivityManage.stub.
在ConnectivityService内部，定义了两个类来解析xml中的网络类型，类的代码如下： 
private class NetworkAttributes { 
/** 
* Class for holding settings read from resources. 
*/ 
public String mName; 
public int mType; 
public int mRadio; 
public int mPriority; 
public NetworkInfo.State mLastState; 
public NetworkAttributes(String init) { 
String fragments[] = init.split(","); 
mName = fragments[0].toLowerCase(); 
if (fragments[1].toLowerCase().equals("wifi")) { 
mRadio = ConnectivityManager.TYPE_WIFI; 
} else { 
mRadio = ConnectivityManager.TYPE_MOBILE; 
} 
if (mName.equals("default")) { 
mType = mRadio; 
} else if (mName.equals("mms")) { 
mType = ConnectivityManager.TYPE_MOBILE_MMS; 
} else if (mName.equals("supl")) { 
mType = ConnectivityManager.TYPE_MOBILE_SUPL; 
} else if (mName.equals("dun")) { 
mType = ConnectivityManager.TYPE_MOBILE_DUN; 
} else if (mName.equals("hipri")) { 
mType = ConnectivityManager.TYPE_MOBILE_HIPRI; 
} 
mPriority = Integer.parseInt(fragments[2]); 
mLastState = NetworkInfo.State.UNKNOWN; 
} 
public boolean isDefault() { 
return (mType == mRadio); 
} 
} 
private class RadioAttributes { 
public String mName; 
public int mPriority; 
public int mSimultaneity; 
public int mType; 
public RadioAttributes(String init) { 
String fragments[] = init.split(","); 
mName = fragments[0].toLowerCase(); 
mPriority = Integer.parseInt(fragments[1]); 
mSimultaneity = Integer.parseInt(fragments[2]); 
if (mName.equals("wifi")) { 
mType = ConnectivityManager.TYPE_WIFI; 
} else { 
mType = ConnectivityManager.TYPE_MOBILE; 
} 
} 
} 
并通过一下代码，来给网络分配优先级， 
mPriorityList = new int[naStrings.length]; 
{ 
int priority = 0; //lowest 
int nextPos = naStrings.length-1; 
while (nextPos>-1) { 
for (int i = 0; i < mNetAttributes.length; i++) { 
if(mNetAttributes[i].mPriority == priority) { 
mPriorityList[nextPos--] = i; 
} 
} 
priority++; 
} 
} 
mNetRequestersPids = 
new ArrayList[ConnectivityManager.MAX_NETWORK_TYPE+1]; 
for (int i=0; i<=ConnectivityManager.MAX_NETWORK_TYPE; i++) { 
mNetRequestersPids[i] = new ArrayList(); 
} 
其中，TYPE_MOBILE_HIPRI的优先级最高，其次为TYPE_MOBILE_MMS，TYPE_MOBILE_SUPL，TYPE_MOBILE_DUN， 
优先级最低的为TYPE_WIFI，TYPE_MOBILE。TYPE_WIFI，TYPE_MOBILE两个网络类型中，TYPE_WIFI大于TYPE_MOBILE的优先级，
在打开wifi的连接后，mobile网络会被关闭。wifi网络连接关闭后，mobile网络会重新连接。在处理网络连接的Handler的代码中有处理： 
private void handleConnect(NetworkInfo info) { 
int type = info.getType(); 
Log.d(TAG, "Got Network Connection Succ from Driver nwtype="+type); 
// snapshot isFailover, because sendConnectedBroadcast() resets it 
boolean isFailover = info.isFailover(); 
NetworkStateTracker thisNet = mNetTrackers[type]; 
// if this is a default net and other default is running 
// kill the one not preferred 
if (mNetAttributes[type].isDefault()) { 
if (mActiveDefaultNetwork != -1 && mActiveDefaultNetwork != type) { 
if ((type != mNetworkPreference && 
mNetAttributes[mActiveDefaultNetwork].mPriority > 
mNetAttributes[type].mPriority) || 
mNetworkPreference == mActiveDefaultNetwork) { 
if(!((SystemProperties.get(CNE.UseCne,"false").equals("true") || 
SystemProperties.get(CNE.UseCne,"false").equals("TRUE"))&& 
CNE.isCndUp)) { 
// don't accept this one 
if (DBG) Log.v(TAG, "Not broadcasting CONNECT_ACTION " + 
"to torn down network " + info.getTypeName()); 
teardown(thisNet); 
} 
return; 
} else { 
// tear down the other 
NetworkStateTracker otherNet = 
mNetTrackers[mActiveDefaultNetwork]; 
if (DBG) Log.v(TAG, "Policy requires " + 
otherNet.getNetworkInfo().getTypeName() + 
" teardown"); 
if(!((SystemProperties.get(CNE.UseCne,"false").equals("true") || 
SystemProperties.get(CNE.UseCne,"false").equals("TRUE"))&& 
CNE.isCndUp)) { 
if (DBG) Log.i(TAG, "CNE To support Simultaneous Nws we"+ 
" will not tear down other nw"); 
if (!teardown(otherNet)) { 
Log.e(TAG, "Network declined teardown request"); 
return; 
} 
} 
if (isFailover) { 
otherNet.releaseWakeLock(); 
} 
} 
} 
mActiveDefaultNetwork = type; 
} 
thisNet.setTeardownRequested(false); 
thisNet.updateNetworkSettings(); 
handleConnectivityChange(); 
sendConnectedBroadcast(info); 
} 
SystemServer启动ConnectivityService，ConnectivityService启动对网络的监视器。 
在SystemServer的run()函数中，启动ConnectivityService的代码： 
try { 
Log.i(TAG, "Connectivity Service"); 
connectivity = ConnectivityService.getInstance(context); 
ServiceManager.addService(Context.CONNECTIVITY_SERVICE, connectivity); 
connectivity.startCne(); 
} catch (Throwable e) { 
Log.e(TAG, "Failure starting Connectivity Service", e); 
} 
在ConnectivityService的构造函数中启动网络监视器的代码： 
if (DBG) Log.v(TAG, "Starting Wifi Service."); 
WifiStateTracker wst = new WifiStateTracker(context, mHandler); 
WifiService wifiService = new WifiService(context, wst); 
ServiceManager.addService(Context.WIFI_SERVICE, wifiService); 
mNetTrackers[ConnectivityManager.TYPE_WIFI] = wst; 
mNetTrackers[ConnectivityManager.TYPE_MOBILE] = 
new MobileDataStateTracker(context, mHandler, 
ConnectivityManager.TYPE_MOBILE, Phone.APN_TYPE_DEFAULT, 
"MOBILE"); 
mNetTrackers[ConnectivityManager.TYPE_MOBILE_MMS] = 
new MobileDataStateTracker(context, mHandler, 
ConnectivityManager.TYPE_MOBILE_MMS, Phone.APN_TYPE_MMS, 
"MOBILE_MMS"); 
mNetTrackers[ConnectivityManager.TYPE_MOBILE_SUPL] = 
new MobileDataStateTracker(context, mHandler, 
ConnectivityManager.TYPE_MOBILE_SUPL, Phone.APN_TYPE_SUPL, 
"MOBILE_SUPL"); 
mNetTrackers[ConnectivityManager.TYPE_MOBILE_DUN] = 
new MobileDataStateTracker(context, mHandler, 
ConnectivityManager.TYPE_MOBILE_DUN, Phone.APN_TYPE_DUN, 
"MOBILE_DUN"); 
mNetTrackers[ConnectivityManager.TYPE_MOBILE_HIPRI] = 
new MobileDataStateTracker(context, mHandler, 
ConnectivityManager.TYPE_MOBILE_HIPRI, Phone.APN_TYPE_HIPRI, 
"MOBILE_HIPRI"); 
mNumDnsEntries = 0; 
mTestMode = SystemProperties.get("cm.test.mode").equals("true") 
&& SystemProperties.get("ro.build.type").equals("eng"); 
for (NetworkStateTracker t : mNetTrackers) 
t.startMonitoring(); 
// Constructing this starts it too 
mWifiWatchdogService = new WifiWatchdogService(context, wst); 
在settings中可以设置网络连接，比如打开wifi，打开bluetooth,设置apn的连接等等，在设置完成后，设置的消息会存在一个数据库中保存，并发送系统消息来广播网络设置的变化。
在网络监视器中捕捉了settings中发出的相应的网络广播信息， 
网络监视器中注册了settings中网络变化的信息，有变化会做相应的处理，并将处理的结果存储在NetworkInfo类的一个对象中，在ConnectivityService中通过
public NetworkInfo getNetworkInfo(int networkType)方法可以得知当前networkType类型网络的连接情况。 
在app中，我们可以通过ConnectivityManager来获取当前的网络信息，并能指定当前程序需要的网络类型： 
ConnectivityManager mCnn = context.getSystemService(context.NONNECTIVITY_SERVICE);
NetworkInfo mNetinfo = mCnn.getActiveNetworkInfo(); 
mCnn.setNetworkPreference(int preference);//设定首选网络类型。 
假如没有设定，网络类型为系统默认。在wifi，3G网络同时存在的情况下，系统会默认的调用wifi网络，加载wifi的驱动，走wifi网络。 
