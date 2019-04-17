# 浅析android下如何通过jni监控wifi网络连接、dhcpcd执行和power电源控制 - DoubleLi - 博客园






libs/android_runtime/android_net_wifi_Wifi.cpp
部分jni接口
static JNINativeMethod gWifiMethods[] = {
{ "loadDriver", "()Z", (void *)android_net_wifi_loadDriver },
{ "setPowerModeCommand", "(I)Z", (void*) android_net_wifi_setPowerModeCommand },//电源管理
{ "connectToSupplicant", "()Z", (void *)android_net_wifi_connectToSupplicant },
{ "waitForEvent", "()Ljava/lang/String;", (void*) android_net_wifi_waitForEvent },
{ "disconnectCommand", "()Z", (void *)android_net_wifi_disconnectCommand },
...
};
int register_android_net_wifi_WifiManager(JNIEnv* env)
{
...
return AndroidRuntime::registerNativeMethods(env,
WIFI_PKG_NAME, gWifiMethods, NELEM(gWifiMethods));//登记jni
}
libs/android_runtime/AndroidRuntime.cpp
static const RegJNIRec gRegJNI[] = {
...
REG_JNI(register_android_net_wifi_WifiManager),
...
};
int AndroidRuntime::startReg(JNIEnv* env)
{
...
register_jni_procs(gRegJNI, NELEM(gRegJNI), env);
...
}
AndroidRuntime::start
=>startReg(env)即调用方法int AndroidRuntime::startReg(JNIEnv* env)
=============================================================================================================
wifi_load_driver
wifi_start_supplicant
=>ensure_config_file_exists
//检查/data/misc/wifi/wpa_supplicant.conf文件是否存在,如果不存在,那么从/system/etc/wifi/wpa_supplicant.conf动态拷贝一份
android_net_wifi_connectToSupplicant
=>wifi_connect_to_supplicant
=>
ctrl_conn = wpa_ctrl_open(ifname);
monitor_conn = wpa_ctrl_open(ifname);
wpa_ctrl_attach(monitor_conn);
android_net_wifi_waitForEvent
=>wifi_wait_for_event
=>wpa_ctrl_recv(monitor_conn, buf, &nread);
=>recv(ctrl->s, reply, *reply_len, 0);//阻塞等待wpa_supplicant的netlink数据过来
=>如果接收的buf数据区,buf[0]为'<',那么说明有level级别信息,所以将'<'...'>'数据剔除,然后wifi_wait_for_event函数返回[luther.gliethttp].
java/android/android/net/wifi/WifiMonitor.java
public class WifiMonitor {
...
public void startMonitoring() {
new MonitorThread().start();//启动java线程
}
class MonitorThread extends Thread {
public MonitorThread() {
super("WifiMonitor");
}
public void run() {
for (;;) {
ensureSupplicantConnection();//=>WifiNative.connectToSupplicant调用jni函数android_net_wifi_connectToSupplicant
String eventStr = WifiNative.waitForEvent();//=>调用jni函数android_net_wifi_waitForEvent
//private static final int CONNECTED = 1;
//private static final int DISCONNECTED = 2;
//private static final String eventPrefix = "CTRL-EVENT-";
//private static final int eventPrefixLen = eventPrefix.length();
//private static final String connectedEvent = "CONNECTED";
//private static final String disconnectedEvent = "DISCONNECTED";
String eventName = eventStr.substring(eventPrefixLen);//去掉"CTRL-EVENT-"字符串
int nameEnd = eventName.indexOf(' ');//找到随后的空格位置,这在wpa_supplicant发送时
//#define WPA_EVENT_CONNECTED "CTRL-EVENT-CONNECTED "中,已经内置空格了.
if (nameEnd != -1)
eventName = eventName.substring(0, nameEnd);
int event;
if (eventName.equals(connectedEvent))//检测netlink过来的字符串action类型
event = CONNECTED;
else if (eventName.equals(disconnectedEvent))
event = DISCONNECTED;
...
int ind = eventStr.indexOf(" - ");//CTRL-EVENT-CONNECTED - Connection to ...
if (ind != -1)
eventData = eventStr.substring(ind + 3);
//剔除前导控制字符,将" - "后面的描述字符串作为真实数据,继续处理
...
if (event == STATE_CHANGE) {
handleSupplicantStateChange(eventData);
} else if (event == DRIVER_STATE) {
handleDriverEvent(eventData);
} else {
handleEvent(event, eventData);//对于CONNECTED和DISCONNECTED等netlink事件将执行此操作来处理[luther.gliethttp]
// If supplicant is gone, exit the thread
if (event == TERMINATING) {
break;
}
}
...
void handleEvent(int event, String remainder) {
switch (event) {
case DISCONNECTED:
handleNetworkStateChange(NetworkInfo.DetailedState.DISCONNECTED, remainder);
break;
case CONNECTED:
handleNetworkStateChange(NetworkInfo.DetailedState.CONNECTED, remainder);//控制界面显示
break;
...
}
public class WifiStateTracker extends NetworkStateTracker {
...
public void startEventLoop() {
mWifiMonitor.startMonitoring();//启动上面的MonitorThread线程
}
...
}
java/services/com/android/server/WifiService.java
public class WifiService extends IWifiManager.Stub {
...
private boolean setWifiEnabledBlocking(boolean enable) {
final int eventualWifiState = enable ? WIFI_STATE_ENABLED : WIFI_STATE_DISABLED;
...
if (enable) {
if (WifiNative.loadDriver()) {
Log.e(TAG, "Failed to load Wi-Fi driver.");
updateWifiState(WIFI_STATE_UNKNOWN);
return false;
}
if (WifiNative.startSupplicant()) {
WifiNative.unloadDriver();
Log.e(TAG, "Failed to start supplicant daemon.");
updateWifiState(WIFI_STATE_UNKNOWN);
return false;
}
mWifiStateTracker.startEventLoop();
//启动MonitorThread线程,等待wpa_supplicant将netlink数据转发过来,然后根据netlink动作类型,进一步影响界面显示[luther.gliethttp].
}
...
}
java/android/android/net/wifi/WifiStateTracker.java
电源管理
private void handleConnectedState() {
...
mDhcpTarget.obtainMessage(EVENT_DHCP_START).sendToTarget();//传递到下面的handleMessage方法
...
}
public void onChange(boolean selfChange) {
...
handleConnectedState();
...
}
public class WifiStateTracker extends NetworkStateTracker {
...
public void handleMessage(Message msg) {
switch (msg.what) {
case EVENT_SUPPLICANT_CONNECTION:
case EVENT_NETWORK_STATE_CHANGED:
handleConnectedState();//调用
...
private class DhcpHandler extends Handler {
private Handler mTarget;
public DhcpHandler(Looper looper, Handler target) {
super(looper);
mTarget = target;
}
public void handleMessage(Message msg) {
int event;
//private static final int DRIVER_POWER_MODE_AUTO = 0;
//private static final int DRIVER_POWER_MODE_ACTIVE = 1;
switch (msg.what) {
case EVENT_DHCP_START:
synchronized (this) {
WifiNative.setPowerModeCommand(DRIVER_POWER_MODE_ACTIVE);//设置电源模式,调用 android_net_wifi_setPowerModeCommand
}
Log.d(TAG, "DhcpHandler: DHCP request started");
//libs/android_runtime/android_net_NetUtils.cpp
//static JNINativeMethod gNetworkUtilMethods[] = {
//{ "runDhcp", "(Ljava/lang/String;Landroid/net/DhcpInfo;)Z", (void *)android_net_utils_runDhcp },
// ...
//};
if (NetworkUtils.runDhcp(mInterfaceName, mDhcpInfo)) {//执行dhcp申请ip地址操作
event = EVENT_INTERFACE_CONFIGURATION_SUCCEEDED;
if (LOCAL_LOGD) Log.v(TAG, "DhcpHandler: DHCP request succeeded");
} else {
event = EVENT_INTERFACE_CONFIGURATION_FAILED;
Log.i(TAG, "DhcpHandler: DHCP request failed: " +
NetworkUtils.getDhcpError());
//如果dhcpcd分配ip失败,那么Message.obtain(mTarget, event).sendToTarget();将执行
//WifiNative.disconnectCommand();即：static JNINativeMethod gWifiMethods[] = {
//android_net_wifi_disconnectCommand发送"DISCONNECT"字符串[luther.gliethttp]
//然后在wpa_supplicant服务端执行wpa_supplicant_ctrl_iface_process
//wpa_supplicant_disassociate
}
synchronized (this) {
WifiNative.setPowerModeCommand(DRIVER_POWER_MODE_AUTO);
}
Message.obtain(mTarget, event).sendToTarget();
break;
}
}
}
...
/**
* Send the tracker a notification that a connection to the supplicant
* daemon has been established.
*/
//在上面的public class WifiMonitor=>ensureSupplicantConnection
//=>
//while (!supplicantConnected) {
// boolean connected;
//synchronized (mWifiStateTracker) {
//connected = WifiNative.connectToSupplicant();//如果没有连接成功,那么while循环尝试,直到尝试成功,或者定义了oneShot,仅一次尝试
//=>mWifiStateTracker.notifySupplicantConnection();//如果WifiNative.connectToSupplicant()成功,那么将执行
//mWifiStateTracker.notifySupplicantConnection();的调用.
void notifySupplicantConnection() {//向对象发送message
Message.obtain(this, EVENT_SUPPLICANT_CONNECTION).sendToTarget();
}
void notifyStateChange(SupplicantState newState) {
Message.obtain(this, EVENT_SUPPLICANT_STATE_CHANGED, newState).sendToTarget();
}
...
}
static jboolean android_net_wifi_setPowerModeCommand(JNIEnv* env, jobject clazz, jint mode)
{
char cmdstr[256];
sprintf(cmdstr, "DRIVER POWERMODE %d", mode);
return doBooleanCommand(cmdstr, "OK");
}
android_net_wifi_setPowerModeCommand
=>doBooleanCommand
=>doCommand
=>wifi_command
=>wifi_send_command
=>wpa_ctrl_request
=>send给wpa_supplicant
然后wpa_supplicant将做如下接收操作：
system/extra/wpa_supplicant/main.c
=>wpa_supplicant_add_iface
=>wpa_supplicant_init_iface2
=>wpa_supplicant_ctrl_iface_init
=>注册ctrl_conn控制端口和monitor_conn监听端口的处理函数
eloop_register_read_sock(priv->sock, wpa_supplicant_ctrl_iface_receive, wpa_s, priv);//ctrl_conn端口的handler处理函数
wpa_msg_register_cb(wpa_supplicant_ctrl_iface_msg_cb);//monitor_conn端口的回调处理函数,处理netlink数据到所有monitor_conn监听端口
=>wpa_supplicant_ctrl_iface_receive//对于unix通信方式
=>wpa_supplicant_ctrl_iface_process
=>如果wpa_cli发送的是wpa_cli driver xxx形式的命令,那么调用这个函数
if (os_strncmp(buf, "DRIVER ", 7) == 0) {//掠过前7个,直接将命令传过去
reply_len = wpa_supplicant_driver_cmd(wpa_s, buf + 7, reply, reply_size);
=>wpa_supplicant_driver_cmd
=>wpa_drv_driver_cmd
=>自定义DRIVER扩展处理函数,所以对于java传递过来的power电源管理命令,wpa_drv_driver_cmd将收到"POWERMODE 0"或者"POWERMODE 1"字符串[luther.gliethttp]
=============================================================================================================
jni
=>runDhcp
=>android_net_utils_runDhcp
libs/netutils/dhcp_utils.c
=>dhcp_do_request
=>
static const char DAEMON_NAME[] = "dhcpcd";
static const char DAEMON_PROP_NAME[] = "init.svc.dhcpcd";
static const char DHCP_PROP_NAME_PREFIX[] = "dhcp";
const char *ctrl_prop = "ctl.start";
const char *desired_status = "running";
snprintf(result_prop_name, sizeof(result_prop_name), "%s.%s.result",
DHCP_PROP_NAME_PREFIX,
interface);
property_set(result_prop_name, "");//设置dhcp.eth0.result="";等到成功完成dhcp之后,
property_set(ctrl_prop, DAEMON_NAME);//向名字为dhcpcd的service,发送"ctrl.start"启动命令字,该service在init.rc中
//init.rc中dhcpcd服务进程命令字
//service dhcpcd /system/bin/dhcpcd eth0
// disabled
// oneshot 
wait_for_property(DAEMON_PROP_NAME, desired_status, 10);
//init.c=>init进程
//=>handle_property_set_fd因为是"ctrl.start"命令字,所以调用handle_control_message处理控制信息
//=>handle_control_message
//=>msg_start
//=>
// struct service *svc = service_find_by_name(name);
// service_start(svc);//启动svc,即执行：/system/bin/dhcpcd eth0
//=>service_start
//=>pid = fork();
// if(pid == 0)execve(svc->args[0], (char**) svc->args, (char**) ENV);子进程执行execve运行/system/bin/dhcpcd,参数为eth0
//=>否则父进程,即init进程将
//=>notify_service_state(svc->name, "running");设置该svc的状态prop
// snprintf(pname, sizeof(pname), "init.svc.%s", name);
// property_set(pname, state);//所以这样上面wait_for_property(DAEMON_PROP_NAME, desired_status, 10);也才能够正常pass[luther.gliethttp].
wait_for_property(result_prop_name, NULL, 15);//等待dhcp.eth0.result=非空
=============================================================================================================
system/extra/dhcpcd-4.0.0-beta9/dhcpcd.c
dhcpcd
=>main
# define SYSCONFDIR "/system/etc/dhcpcd"
#define PACKAGE "dhcpcd"
# define CONFIG SYSCONFDIR "/" PACKAGE ".conf"
# define LIBEXECDIR "/system/etc/dhcpcd"
# define SCRIPT LIBEXECDIR "/" PACKAGE "-run-hooks"
=>strlcpy(options->script, SCRIPT, sizeof(options->script));//默认的options->script="/system/etc/dhcpcd /dhcpcd-run-hooks"
=>f = fopen(cf ? cf : CONFIG, "r");//如果没有指定.conf文件,那么使用默认.conf文件
=>parse_config_line//解析"/system/etc/dhcpcd/dhcpcd.conf"默认配置文件
=>parse_option
=>如果在"/system/etc/dhcpcd/dhcpcd.conf"有"script"这个节
=>那么执行strlcpy(options->script, oarg, sizeof(options->script));直接拷贝
/*
{"script", required_argument, NULL, 'c'},
{"option", required_argument, NULL, 'o'},
"/system/etc/dhcpcd/dhcpcd.conf"中的部分内容如下：
...
option domain_name_servers, domain_name, domain_search, host_name
...
*/
=>dhcp_run
=>handle_dhcp_packet
=>handle_dhcp
=>bind_dhcp
reason = "TIMEOUT";reason = "BOUND";reason = "REBIND";reason = "RENEW";
system/extra/dhcpcd-4.0.0-beta9/configure.c
=> configure(iface, reason, state->new, state->old, &state->lease, options, 1);
//如果dhcp超时或者dhcp成功,都会调用exec_script来执行脚本,
//执行setprop dhcp.${interface}.result "failed"或者
//执行setprop dhcp.${interface}.result "ok"
=>exec_script(options, iface->name, reason, NULL, old);
=>然后configure_env通过环境变量将reason传递到脚本中
int exec_script(const struct options *options, const char *iface, const char *reason,
const struct dhcp_message *dhcpn, const struct dhcp_message *dhcpo)
=>pid = fork();
=>if(pid == 0)execve(options->script, argv, env);//子进程执行脚本,默认"/system/etc/dhcpcd/dhcpcd-run-hooks"
//dhcpcd-run-hooks脚本会根据level值,决定是否执行system/etc/dhcpcd/dhcpcd-hook/*目录下的相应文件
//我们的系统在该system/etc/dhcpcd/dhcpcd-hook/*目录下有如下3个文件
//95-configured
//20-dns.conf
//01-test
=>父进程返回while (waitpid(pid, &status, 0) == -1)等待子进程脚本执行完成
system/extra/dhcpcd-4.0.0-beta9/dhcpcd-hooks/20-dns.conf
system/extra/dhcpcd-4.0.0-beta9/dhcpcd-hooks/95-configured
...
setprop dhcp.${interface}.ipaddress "${new_ip_address}"
setprop dhcp.${interface}.result "ok"//设置属性为ok
setprop dhcp.${interface}.result "failed"
...
=============================================================================================================
inet_init、tcp_prot
sock->ops->sendmsg(iocb, sock, msg, size);
=>inetsw_array[]
=>inet_stream_ops
=>tcp_sendmsg
=============================================================================================================
wpa_cli.c
=>main
=>wpa_cli_interactive
=>wpa_cli_recv_pending(monitor_conn, 0, 0);//阻塞等待wpa_supplicant发送数据过来
=>如果action_monitor为true,那么将执行一些简单加工操作,否则将直接将wpa_supplicant发过来的数据打印到console上[luther.gliethttp].

原地址：http://www.wangchao.net.cn/it/detail_60528.html









