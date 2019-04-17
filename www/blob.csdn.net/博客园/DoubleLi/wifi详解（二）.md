# wifi详解（二） - DoubleLi - 博客园






# 1        **Wifi****模块解析和启动流程**

## 1.1      框架分析

WIFI整体框架如图所示：



首先，用户程序使用WifiManager类来管理Wifi模块，它能够获得Wifi模块的状态，配置和控制Wifi模块，而所有这些操作都要依赖 Wifiservice类来实现。

 WifiService和WifiMonitor类是Wifi框架的核心，如图所示。下面先来看看WifiService是什么时候，怎么被创建和初始化 的。

 在systemServer启动之后，它会创建一个 ConnectivityServer对象，这个对象的构造函数会创建一个WifiService的实例，代码如下所示：



framework/base/services/java/com/android/server/ConnectivityService.java

{

……

case ConnectivityManager.TYPE_WIFI:

               if(DBG) Slog.v(TAG, "Starting Wifi Service.");

               WifiStateTrackerwst = new WifiStateTracker(context,mHandler);                            //创建WifiStateTracker实例

**WifiService wifiService = newWifiService(context,wst);//创建WifiService实例**

               ServiceManager.addService(Context.WIFI_SERVICE,wifiService);           //向服务管理系统添加Wifi服务

               wifiService.startWifi();    //启动Wifi

               mNetTrackers[ConnectivityManager.TYPE_WIFI]= wst;

wst.startMonitoring(); //启动WifiMonitor中的WifiThread线程

……

}

       WifiService的主要工作：WifiMonitor和Wpa_supplicant的启动和关闭，向Wpa_supplicant发送命令。

       WifiMonitor的主要工作：阻塞监听并接收来自Wpa_supplicant的消息，然后发送给WifiStateTracker。

      上面两个线程通过AF_UNIX套接字和Wpa_supplicant通信，在通信过程中有两种连接方式：控制连接和监听连接。它们创建代码如下：

ctrl_conn =wpa_ctrl_open(ifname);

.. .. ..

 monitor_conn = wpa_ctrl_open(ifname);

## 1.2      Wifi启动流程

       （1）使能Wifi

      要想使用Wifi模块，必须首先使能Wifi，当你第一次按下Wifi使能按钮时，WirelessSettings会实例化一个WifiEnabler 对象，实例化代码如下：

packages/apps/settings/src/com/android/settings/WirelessSettings.java

protected void onCreate(Bundle savedInstanceState) {

       super.onCreate(savedInstanceState);

……

             CheckBoxPreferencewifi = (CheckBoxPreference) findPreference(KEY_TOGGLE_WIFI);

**mWifiEnabler=new WifiEnabler(this, wifi);**

……

}

       WifiEnabler类的定义大致如下，它实现了一个监听接口，当WifiEnabler对象被初始化后，它监听到你按键的动作，会调用响应函数 onPreferenceChange（），这个函数会调用WifiManager的setWifiEnabled（）函数。

public class WifiEnabler implementsPreference.OnPreferenceChangeListener{

……

public boolean onPreferenceChange(Preference preference,Objectvalue) {

        booleanenable = (Boolean)value;

……

if (mWifiManager.setWifiEnabled(enable)) {

               mCheckBox.setEnabled(false);

……

}

……

}

       我们都知道Wifimanager只是个服务代理，所以它会调用WifiService的setWifiEnabled（）函数，而这个函数会调用 sendEnableMessage（）函数，了解android消息处理机制的都知道，这个函数最终会给自己发送一个 MESSAGE_ENABLE_WIFI的消息，被WifiService里面定义的handlermessage()函数处理，会调用 setWifiEnabledBlocking（）函数。下面是调用流程：

**mWifiEnabler.onpreferencechange()=>mWifiManage.setWifienabled()=>mWifiService.setWifiEnabled()=>mWifiService.sendEnableMessage()=>mWifiService.handleMessage()=>mWifiService.setWifiEnabledBlocking().**

在 setWifiEnabledBlocking()函数中主要做如下工作：加载Wifi驱动，启动wpa_supplicant，注册广播接收器，启动 WifiThread监听线程。代码如下：

……

if (enable) {

           if(!mWifiStateTracker.loadDriver()) {

               Slog.e(TAG,"Failed toload Wi-Fi driver.");

**setWifiEnabledState(WIFI_STATE_UNKNOWN,uid);**

               return false;

           }

           if(!mWifiStateTracker.startSupplicant()) {

               mWifiStateTracker.unloadDriver();

               Slog.e(TAG, "Failed tostart supplicant daemon.");

               setWifiEnabledState(WIFI_STATE_UNKNOWN, uid);

               return false;

           }



**registerForBroadcasts();**

**           mWifiStateTracker.startEventLoop();**

……

 至此，Wifi使能结束，自动进入扫描阶段。

(2) 扫描AP

      当驱动加载成功后，如果配置文件的AP_SCAN = 1，扫描会自动开始，WifiMonitor将会从supplicant收到一个消息EVENT_DRIVER_STATE_CHANGED，调用 handleDriverEvent（），然后调用mWifiStateTracker.notifyDriverStarted()，该函数向消息队列 添加EVENT_DRIVER_STATE_CHANGED，handlermessage()函数处理消息时调用scan()函数，并通过 WifiNative将扫描命令发送到wpa_supplicant。

Frameworks/base/wifi/java/android/net/wifi/WifiMonitor.java

private void handleDriverEvent(Stringstate) {

           if(state == null) {

               return;

           }

           if(state.equals("STOPPED")) {

               mWifiStateTracker.notifyDriverStopped();

           }else if (state.equals("STARTED")) {

**mWifiStateTracker.notifyDriverStarted();**

           }else if (state.equals("HANGED")) {

               mWifiStateTracker.notifyDriverHung();

           }

       }

Frameworks/base/wifi/java/android/net/wifi/WifiStateTracker.java

case **EVENT_DRIVER_STATE_CHANGED:**



               switch(msg.arg1) {

               case **DRIVER_STARTED**:

                   /**

                    *Set the number of allowed radio channels according

                    *to the system setting, since it gets reset by the

                    *driver upon changing to the STARTED state.

                    */

                   setNumAllowedChannels();

                   synchronized(this) {

                       if(mRunState == RUN_STATE_STARTING) {

                           mRunState= RUN_STATE_RUNNING;

                           if(!mIsScanOnly) {

                               reconnectCommand();

                           }else {

                               // In somesituations, supplicant needs to be kickstarted to

                               // start thebackground scanning

**scan(true);**

                           }

                       }

                   }

                   break;             

上面是启动Wifi 时，自动进行的AP的扫描，用户当然也可以手动扫描AP，这部分实现在WifiService里面，WifiService通过startScan()接 口函数发送扫描命令到supplicant。

Frameworks/base/wifi/java/android/net/wifi/WifiStateTracker.java

public boolean startScan(booleanforceActive) {

       enforceChangePermission();



       switch(mWifiStateTracker.getSupplicantState()) {

           caseDISCONNECTED:

           caseINACTIVE:

           caseSCANNING:

           caseDORMANT:

               break;

           default:

               mWifiStateTracker.setScanResultHandling(

                       WifiStateTracker.SUPPL_SCAN_HANDLING_LIST_ONLY);

               break;

       }

** return mWifiStateTracker.scan(forceActive);**

    }

       然后下面的流程同上面的自动扫描，我们来分析一下手动扫描从哪里开始的。我们应该知道手动扫描是通过菜单键的扫描键来响应的，而响应该动作的应该是 WifiSettings类中Scanner类的handlerMessage()函数，它调用WifiManager的 startScanActive()，这才调用WifiService的startScan()。

packages/apps/Settings/src/com/android/settings/wifiwifisettings.java

public boolean onCreateOptionsMenu(Menu menu) {

       menu.add(Menu.NONE,MENU_ID_SCAN, 0, R.string.wifi_menu_scan)

               .setIcon(R.drawable.ic_menu_scan_network);

       menu.add(Menu.NONE,MENU_ID_ADVANCED, 0, R.string.wifi_menu_advanced)

               .setIcon(android.R.drawable.ic_menu_manage);

       returnsuper.onCreateOptionsMenu(menu);

    }

       当按下菜单键时，WifiSettings就会调用这个函数绘制菜单。如果选择扫描按钮，WifiSettings会调用 onOptionsItemSelected()。

packages/apps/Settings/src/com/android/settings/wifiwifisettings.java

public booleanonOptionsItemSelected(MenuItem item) {

       switch (item.getItemId()){

           caseMENU_ID_SCAN:

               if(mWifiManager.isWifiEnabled()) {

**mScanner.resume();**

               }

               return true;

           caseMENU_ID_ADVANCED:

               startActivity(new Intent(this,AdvancedSettings.class));

               return true;

       }

       returnsuper.onOptionsItemSelected(item);

}



private class Scanner extends Handler {

       private int mRetry = 0;



       void resume() {

           if (**!hasMessages(0)**) {

** sendEmptyMessage(0);**

           }

       }



       void pause() {

           mRetry= 0;

           mAccessPoints.setProgress(false);

           removeMessages(0);

       }



       @Override

       public voidhandleMessage(Message message) {

           if(**mWifiManager.startScanActive()**){

               mRetry = 0;

           }else if (++mRetry >= 3) {

               mRetry = 0;

               Toast.makeText(WifiSettings.this,R.string.wifi_fail_to_scan,

                       Toast.LENGTH_LONG).show();

               return;

           }

           mAccessPoints.setProgress(mRetry!= 0);

**sendEmptyMessageDelayed(0, 6000);**

       }

    }

这里的mWifiManager.startScanActive()就会调用WifiService里 的startScan（）函数，下面的流程和上面的一样，这里不赘述。

当supplicant完成了这个扫描命令后，它会发送一个消息给上 层，提醒他们扫描已经完成，WifiMonitor会接收到这消息，然后再发送给WifiStateTracker。

Frameworks/base/wifi/java/android/net/wifi/WifiMonitor.java

void handleEvent(int event, String remainder) {

           switch (event) {

               caseDISCONNECTED:

                   handleNetworkStateChange(NetworkInfo.DetailedState.DISCONNECTED,remainder);

                   break;



               case CONNECTED:

                   handleNetworkStateChange(NetworkInfo.DetailedState.CONNECTED,remainder);

                   break;



**case SCAN_RESULTS:**

**                   mWifiStateTracker.notifyScanResultsAvailable();**

                   break;



               case UNKNOWN:

                   break;

            }

        }

WifiStateTracker将会广播SCAN_RESULTS_AVAILABLE_ACTION消息：

Frameworks/base/wifi/java/android/net/wifi/WifiStateTracker.java

public voidhandleMessage(Message msg) {

        Intent intent;

……

case **EVENT_SCAN_RESULTS_AVAILABLE:**

               if(ActivityManagerNative.isSystemReady()) {

**mContext.sendBroadcast(newIntent(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));**

               }

               sendScanResultsAvailable();

               /**

                * On receiving the first scanresults after connecting to

                * the supplicant, switch scanmode over to passive.

                */

               setScanMode(false);

               break;

……

｝

       由于WifiSettings类注册了intent，能够处理SCAN_RESULTS_AVAILABLE_ACTION消息，它会调用 handleEvent()，调用流程如下所示。

**WifiSettings.handleEvent()=>WifiSettings.updateAccessPoints() => mWifiManager.getScanResults()=> mService.getScanResults()=> mWifiStateTracker.scanResults() =>WifiNative.scanResultsCommand()……**

将 获取AP列表的命令发送到supplicant，然后supplicant通过Socket发送扫描结果，由上层接收并显示。这和前面的消息获取流程基本 相同。

(3)配置，连接AP

当用户选择一个活跃的AP时，WifiSettings响应打开一个对话框来配 置AP，比如加密方法和连接AP的验证模式。配置好AP后，WifiService添加或更新网络连接到特定的AP。

packages/apps/settings/src/com/android/settings/wifi/WifiSetttings.java

public booleanonPreferenceTreeClick(PreferenceScreen screen,Preference preference) {

       if (preference instanceofAccessPoint) {

           mSelected= (AccessPoint) preference;

**showDialog(mSelected, false);**

       } else if (preference ==mAddNetwork) {

           mSelected= null;

           showDialog(null,true);

       } else if (preference ==mNotifyOpenNetworks) {

           Secure.putInt(getContentResolver(),

                   Secure.WIFI_NETWORKS_AVAILABLE_NOTIFICATION_ON,

                   mNotifyOpenNetworks.isChecked()? 1 : 0);

       } else {

           returnsuper.onPreferenceTreeClick(screen, preference);

       }

       return true;

    }

       配置好以后，当按下“Connect Press”时，WifiSettings通过发送LIST_NETWORK命令到supplicant来检查该网络是否配置。如果没有该网络或没有配置 它，WifiService调用addorUpdateNetwork（）函数来添加或更新网络，然后发送命令给supplicant，连接到这个网络。 下面是从响应连接按钮到WifiService发送连接命令的代码：

packages/apps/settings/src/com/android/settings/wifi/WifiSetttings.java

public void onClick(DialogInterfacedialogInterface, int button) {

       if (button ==WifiDialog.BUTTON_FORGET && mSelected != null) {

           forget(mSelected.networkId);

       } else if (button ==WifiDialog.BUTTON_SUBMIT && mDialog !=null) {

           WifiConfigurationconfig = mDialog.getConfig();



           if(config == null) {

               if (mSelected != null&& !requireKeyStore(mSelected.getConfig())) {

                   connect(mSelected.networkId);

               }

           }else if (config.networkId != -1) {

               if (mSelected != null) {

**mWifiManager.updateNetwork(config);**

                   saveNetworks();

               }

           }else {

               intnetworkId =mWifiManager.addNetwork(config);

               if (networkId != -1) {

                   mWifiManager.enableNetwork(networkId,false);

                   config.networkId =networkId;

                   if (mDialog.edit || requireKeyStore(config)){

                       saveNetworks();

                   } else {

                       connect(networkId);

                   }

               }

           }

       }

    }

Frameworks\base\wifi\java\android\net\wifi\WifiManager.java

public intupdateNetwork(WifiConfiguration config) {

        if(config == null ||config.networkId < 0) {

           return-1;

        }

        return**addOrUpdateNetwork(config);**

}

private intaddOrUpdateNetwork(WifiConfiguration config) {

       try {

           return **mService.addOrUpdateNetwork(config)**;

       } catch (RemoteExceptione) {

           return-1;

       }

    }



WifiService.addOrUpdateNetwork()通过调用mWifiStateTracker.setNetworkVariable()将连接命令发送到Wpa_supplicant。

（4） 获取IP地址

当连接到supplicant后，WifiMonitor就会通知WifiStateTracker。

Frameworks/base/wifi/java/android/net/wifi/WifiMonitor.java

Public void Run(){

if (connectToSupplicant()) {

               // Send a message indicatingthat it is now possible to send commands

               // tothe supplicant

** mWifiStateTracker.notifySupplicantConnection();**

           }else {

               mWifiStateTracker.notifySupplicantLost();

               return;

           }

……

}

WifiStateTracker 发送EVENT_SUPPLICANT_CONNECTION消息到消息队列，这个消息有自己的handlermessage()函数处理，它会启动一个 DHCP线程，而这个线程会一直等待一个消息事件，来启动DHCP协议分配IP地址。

frameworks/base/wifi/java/android/net/wifi/WifiStateTracker.java

void notifySupplicantConnection() {

**sendEmptyMessage(EVENT_SUPPLICANT_CONNECTION);**

}



public void handleMessage(Message msg) {

       Intent intent;



       switch (msg.what) {

           caseEVENT_SUPPLICANT_CONNECTION:

            ……

**HandlerThread dhcpThread =newHandlerThread("DHCP Handler Thread");**

**               dhcpThread.start();**

**               mDhcpTarget =newDhcpHandler(dhcpThread.getLooper(), this);**

**……**

……

}

当 Wpa_supplicant连接到AP后，它会发送一个消息给上层来通知连接成功，WifiMonitor会接受到这个消息并上报给 WifiStateTracker。

Frameworks/base/wifi/java/android/net/wifi/WifiMonitor.java

void handleEvent(int event, String remainder) {

           switch(event) {

               caseDISCONNECTED:

                   handleNetworkStateChange(NetworkInfo.DetailedState.DISCONNECTED,remainder);

                   break;



               caseCONNECTED:

** handleNetworkStateChange(NetworkInfo.DetailedState.CONNECTED,remainder);**

                   break;

               ……

}



private void handleNetworkStateChange(NetworkInfo.DetailedStatenewState, String data) {

        StringBSSID = null;

        intnetworkId = -1;

        if(newState ==NetworkInfo.DetailedState.CONNECTED) {

           Matchermatch = mConnectedEventPattern.matcher(data);

           if(!match.find()) {

               if(Config.LOGD) Log.d(TAG, "Could not find BSSID in CONNECTEDeventstring");

           }else {

               BSSID= match.group(1);

               try{

                   networkId= Integer.parseInt(match.group(2));

               }catch (NumberFormatException e) {

                   networkId= -1;

               }

            }

        }

**mWifiStateTracker.notifyStateChange(newState,BSSID,networkId);**

}



void notifyStateChange(DetailedState newState, StringBSSID, intnetworkId) {

        Messagemsg =Message.obtain(

           this,EVENT_NETWORK_STATE_CHANGED,

           newNetworkStateChangeResult(newState, BSSID, networkId));

       msg.sendToTarget();

    }



caseEVENT_NETWORK_STATE_CHANGED:

……

**configureInterface();**

**……**



private void configureInterface() {

       checkPollTimer();

        mLastSignalLevel = -1;

**if(!mUseStaticIp){          //使用DHCP线程动态IP**

           if(!mHaveIpAddress && !mObtainingIpAddress) {

               mObtainingIpAddress= true;



                                  //发送启动DHCP线程获取IP

** mDhcpTarget.sendEmptyMessage(EVENT_DHCP_START);**

            }

**} else{        //使用静态IP，IP信息从mDhcpInfo中获取**

           intevent;

           if(**NetworkUtils.configureInterface(mInterfaceName,mDhcpInfo**)) {

               mHaveIpAddress= true;

               event= EVENT_INTERFACE_CONFIGURATION_SUCCEEDED;

               if(LOCAL_LOGD) Log.v(TAG, "Static IP configurationsucceeded");

           }else {

               mHaveIpAddress= false;

               event= EVENT_INTERFACE_CONFIGURATION_FAILED;

               if(LOCAL_LOGD) Log.v(TAG, "Static IP configuration failed");

            }

           sendEmptyMessage(event);          //发送IP获得成功消息事件

        }

    }

             DhcpThread获取EVENT_DHCP_START消息事件后，调用handleMessage（）函数，启动DHCP获取IP地址的服务。

public void handleMessage(Message msg) {

           intevent;

switch (msg.what) {

               caseEVENT_DHCP_START:



……

Log.d(TAG, "DhcpHandler: DHCP requeststarted");

//启动一个DHCPclient的精灵进 程，为mInterfaceName请求分配一个IP地//址

    if (**NetworkUtils.runDhcp(mInterfaceName, mDhcpInfo**)) {

     event=EVENT_INTERFACE_CONFIGURATION_SUCCEEDED;

         if(LOCAL_LOGD)Log.v(TAG, "DhcpHandler: DHCP request succeeded");

    } else {

           event=EVENT_INTERFACE_CONFIGURATION_FAILED;

          Log.i(TAG,"DhcpHandler: DHCP request failed: " +

                           NetworkUtils.getDhcpError());

        }

……

}

这 里调用了一个NetworkUtils.runDhcp（）函数，NetworkUtils类是一个网络服务的辅助类，它主要定义了一些本地接口，这些接 口会通过他们的JNI层android_net_NetUtils.cpp文件和DHCP client通信，并获取IP地址。

至此，IP 地址获取完毕，Wifi启动流程结束。









