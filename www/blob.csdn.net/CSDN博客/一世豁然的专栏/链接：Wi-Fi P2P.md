# 链接：Wi-Fi P2P - 一世豁然的专栏 - CSDN博客





2017年05月17日 16:26:37[一世豁然](https://me.csdn.net/Explorer_day)阅读数：662








Wi-Fi点对点（P2P）允许具有适当硬件的Android 4.0（API级别14）或更高版本的设备通过Wi-Fi直接连接，无需中间接入点（Android的Wi-Fi P2P框架符合 Wi-Fi联盟的Wi-Fi Direct™认证计划）。 使用这些API，您可以在每个设备支持Wi-Fi P2P时发现并连接到其他设备，然后通过比蓝牙连接更长的距离进行快速连接进行通信。 这对于在用户之间共享数据的应用程序是有用的，例如多人游戏或照片共享应用程序。




Wi-Fi P2P API由以下主要部分组成：


1、WifiP2pManager类中定义了允许您发现，请求和连接到对等体的方法。




2、侦听器可以通知您WifiP2pManager方法调用的成功或失败。 当调用WifiP2pManager方法时，每个方法都可以接收作为参数传入的特定侦听器。




3、通知您Wi-Fi P2P框架检测到的特定事件的意图，例如丢弃的连接或新发现的对等体。




您经常将这三个主要组件的API一起使用。 例如，您可以向调用discoverPeers（）提供一个WifiP2pManager.ActionListener，以便您可以通过ActionListener.onSuccess（）和ActionListener.onFailure（）方法通知。 如果discoverPeers（）方法发现对等体列表已更改，则还会广播WIFI_P2P_PEERS_CHANGED_ACTION意图。








一、API概览

WifiP2pManager类提供了一种方法，允许您与设备上的Wi-Fi硬件进行交互，以便发现并连接到对等体。 以下操作可用：





表1.Wi-Fi P2P方法



|方法|描述|
|----|----|
|`[initialize()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#initialize(android.content.Context,%20android.os.Looper,%20android.net.wifi.p2p.WifiP2pManager.ChannelListener))`|使用Wi-Fi框架注册应用程序。 在调用任何其他Wi-Fi P2P方法之前，必须先调用它。|
|`[connect()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#connect(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pConfig,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`|启动与具有指定配置的设备的对等连接。|
|`[cancelConnect()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#cancelConnect(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`|取消任何正在进行的对等群组协商。|
|`[requestConnectInfo()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#requestConnectionInfo(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ConnectionInfoListener))`|请求设备的连接信息。|
|`[createGroup()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#createGroup(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`|使用当前设备创建一个对等组，作为组所有者。|
|`[removeGroup()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#removeGroup(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`|删除当前的对等组。|
|`[requestGroupInfo()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#requestGroupInfo(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.GroupInfoListener))`|请求点对点组信息。|
|`[discoverPeers()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#discoverPeers(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`|启动对等体发现|
|`[requestPeers()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#requestPeers(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.PeerListListener))`|请求发现的对等体的当前列表。|


WifiP2pManager方法让您通过监听器，使Wi-Fi P2P框架可以通知您活动的呼叫状态。 可用的侦听器接口和相应的使用侦听器的WifiP2pManager方法调用如下表所示：





表2. Wifi P2P侦听器



|Listener interface|Associated actions|
|----|----|
|`[WifiP2pManager.ActionListener](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.ActionListener.html)`|`[connect()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#connect(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pConfig,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`, `[cancelConnect()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#cancelConnect(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`, `[createGroup()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#createGroup(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`, `[removeGroup()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#removeGroup(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`, and `[discoverPeers()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#discoverPeers(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ActionListener))`|
|`[WifiP2pManager.ChannelListener](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.ChannelListener.html)`|`[initialize()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#initialize(android.content.Context,%20android.os.Looper,%20android.net.wifi.p2p.WifiP2pManager.ChannelListener))`|
|`[WifiP2pManager.ConnectionInfoListener](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.ConnectionInfoListener.html)`|`[requestConnectInfo()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#requestConnectionInfo(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.ConnectionInfoListener))`|
|`[WifiP2pManager.GroupInfoListener](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.GroupInfoListener.html)`|`[requestGroupInfo()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#requestGroupInfo(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.GroupInfoListener))`|
|`[WifiP2pManager.PeerListListener](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.PeerListListener.html)`|`[requestPeers()](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#requestPeers(android.net.wifi.p2p.WifiP2pManager.Channel,%20android.net.wifi.p2p.WifiP2pManager.PeerListListener))`|


Wi-Fi P2P API定义了当某些Wi-Fi P2P事件发生时广播的意图，例如当发现新的对等体或设备的Wi-Fi状态改变时。 您可以通过创建处理这些意图的广播接收器来注册以在应用程序中接收这些意图：





表3. Wifi P2P意图



|Intent|Description|
|----|----|
|`[WIFI_P2P_CONNECTION_CHANGED_ACTION](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#WIFI_P2P_CONNECTION_CHANGED_ACTION)`|当设备的Wi-Fi连接状态发生变化时进行广播。|
|`[WIFI_P2P_PEERS_CHANGED_ACTION](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#WIFI_P2P_PEERS_CHANGED_ACTION)`|当你调用discoverPeers（）时广播。 如果您在应用程序中处理此意图，通常需要调用requestPeers（）获取对等体的更新列表。|
|`[WIFI_P2P_STATE_CHANGED_ACTION](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#WIFI_P2P_STATE_CHANGED_ACTION)`|在设备上启用或禁用Wifi P2P时广播。|
|`[WIFI_P2P_THIS_DEVICE_CHANGED_ACTION](https://developer.android.google.cn/reference/android/net/wifi/p2p/WifiP2pManager.html#WIFI_P2P_THIS_DEVICE_CHANGED_ACTION)`|当设备的详细信息更改时进行广播，例如设备的名称。|





二、为Wi-Fi P2P意图创建广播接收机

广播接收器允许您接收Android系统广播的意图，以便您的应用程序可以响应您感兴趣的事件。创建广播接收器以处理Wi-Fi P2P意图的基本步骤如下：


1、创建一个扩展BroadcastReceiver类的类。 对于类的构造函数，您最有可能希望为WifiP2pManager，WifiP2pManager.Channel以及该广播接收者注册的活动提供参数。这允许广播接收者向活动发送更新，并且可以访问 Wi-Fi硬件和通信通道。




2、在广播接收器中，检查您对onReceive（）感兴趣的意图。 根据收到的意图执行任何必要的操作。 例如，如果广播接收方接收到WIFI_P2P_PEERS_CHANGED_ACTION意图，则可以调用requestPeers（）方法来获取当前发现的对等体的列表。




以下代码显示了如何创建典型的广播接收器。 广播接收者将WifiP2pManager对象和活动作为参数，并且当广播接收者接收到意图时，使用这两个类来适当地执行所需的动作：




```java
/**
 * A BroadcastReceiver that notifies of important Wi-Fi p2p events.
 */
public class WiFiDirectBroadcastReceiver extends BroadcastReceiver {

    private WifiP2pManager mManager;
    private Channel mChannel;
    private MyWiFiActivity mActivity;

    public WiFiDirectBroadcastReceiver(WifiP2pManager manager, Channel channel,
            MyWifiActivity activity) {
        super();
        this.mManager = manager;
        this.mChannel = channel;
        this.mActivity = activity;
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();

        if (WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION.equals(action)) {
            // Check to see if Wi-Fi is enabled and notify appropriate activity
        } else if (WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION.equals(action)) {
            // Call WifiP2pManager.requestPeers() to get a list of current peers
        } else if (WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION.equals(action)) {
            // Respond to new connection or disconnections
        } else if (WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION.equals(action)) {
            // Respond to this device's wifi state changing
        }
    }
}
```








三、创建Wifi P2P应用程序

创建Wi-Fi P2P应用程序涉及创建和注册应用程序的广播接收器，发现对等体，连接到对等体，并将数据传输到对等体。 以下部分介绍如何执行此操作。





一）、初始设置

在使用Wi-Fi P2P API之前，您必须确保您的应用程序可以访问硬件，并且该设备支持Wi-Fi P2P协议。 如果支持Wi-Fi P2P，您可以获得WifiP2pManager的实例，创建和注册广播接收者，并开始使用Wi-Fi P2P API。





1、请求允许在设备上使用Wi-Fi硬件，并声明您的应用程序在Android清单中具有正确的最低SDK版本：



```
<uses-sdk android:minSdkVersion="14" />
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
<uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
<uses-permission android:name="android.permission.CHANGE_NETWORK_STATE" />
<uses-permission android:name="android.permission.INTERNET" />
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
```


2、检查是否打开并支持Wi-Fi P2P。 当收到WIFI_P2P_STATE_CHANGED_ACTION意图时，您的广播接收者可以检查这个。 通知您的Wi-Fi P2P状态的活动，并作出相应的反应：



```java
@Override
public void onReceive(Context context, Intent intent) {
    ...
    String action = intent.getAction();
    if (WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION.equals(action)) {
        int state = intent.getIntExtra(WifiP2pManager.EXTRA_WIFI_STATE, -1);
        if (state == WifiP2pManager.WIFI_P2P_STATE_ENABLED) {
            // Wifi P2P is enabled
        } else {
            // Wi-Fi P2P is not enabled
        }
    }
    ...
}
```


3、在活动的onCreate（）方法中，通过调用initialize（）获取一个WifiP2pManager实例，并使用Wi-Fi P2P框架注册应用程序。 此方法返回一个WifiP2pManager.Channel，用于将您的应用程序连接到Wi-Fi P2P框架。 您还应使用WifiP2pManager和WifiP2pManager.Channel对象以及对您的活动的引用来创建广播接收器的实例。 这允许您的广播接收者通知您的活动有趣的事件，并相应更新。 它还允许您在必要时操作设备的Wi-Fi状态：



```java
WifiP2pManager mManager;
Channel mChannel;
BroadcastReceiver mReceiver;
...
@Override
protected void onCreate(Bundle savedInstanceState){
    ...
    mManager = (WifiP2pManager) getSystemService(Context.WIFI_P2P_SERVICE);
    mChannel = mManager.initialize(this, getMainLooper(), null);
    mReceiver = new WiFiDirectBroadcastReceiver(mManager, mChannel, this);
    ...
}
```


4、创建一个意图过滤器，并添加广播接收者检查的相同意图：



```java
IntentFilter mIntentFilter;
...
@Override
protected void onCreate(Bundle savedInstanceState){
    ...
    mIntentFilter = new IntentFilter();
    mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_STATE_CHANGED_ACTION);
    mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION);
    mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_CONNECTION_CHANGED_ACTION);
    mIntentFilter.addAction(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION);
    ...
}
```


5、在活动的onResume（）方法中注册广播接收者，并在活动的onPause（）方法中取消注册：



```java
/* register the broadcast receiver with the intent values to be matched */
@Override
protected void onResume() {
    super.onResume();
    registerReceiver(mReceiver, mIntentFilter);
}
/* unregister the broadcast receiver */
@Override
protected void onPause() {
    super.onPause();
    unregisterReceiver(mReceiver);
}
```


当您获得WifiP2pManager.Channel并设置广播接收器时，您的应用程序可以进行Wi-Fi P2P方法呼叫并接收Wi-Fi P2P意图。




当您获得WifiP2pManager.Channel并设置广播接收器时，您的应用程序可以进行Wi-Fi P2P方法呼叫并接收Wi-Fi P2P意图。您现在可以通过调用Wi-Fi P2P功能来实现应用程序并使用WifiP2pManager中的方法来使用Wi-Fi P2P功能。 接下来的部分将介绍如何发现和连接到对等的常见操作。





一）、发现peer

要发现可用于连接的对等体，请调用discoverPeers（）来检测范围内的可用对等体。 对此函数的调用是异步的，如果创建了WifiP2pManager.ActionListener，则通过onSuccess（）和onFailure（）将成功或失败传达给应用程序。 onSuccess（）方法仅通知您发现过程成功，并且不提供有关其发现的实际对等体的任何信息（如果有）：




```java
mManager.discoverPeers(channel, new WifiP2pManager.ActionListener() {
    @Override
    public void onSuccess() {
        ...
    }

    @Override
    public void onFailure(int reasonCode) {
        ...
    }
});
```


如果发现过程成功并检测到对等体，则系统广播WIFI_P2P_PEERS_CHANGED_ACTION意图，您可以在广播接收器中收听以获取对等体列表。 当您的应用程序收到WIFI_P2P_PEERS_CHANGED_ACTION意图时，您可以使用requestPeers（）请求发现的对等体的列表。 以下代码显示如何设置：



```java
PeerListListener myPeerListListener;
...
if (WifiP2pManager.WIFI_P2P_PEERS_CHANGED_ACTION.equals(action)) {

    // request available peers from the wifi p2p manager. This is an
    // asynchronous call and the calling activity is notified with a
    // callback on PeerListListener.onPeersAvailable()
    if (mManager != null) {
        mManager.requestPeers(mChannel, myPeerListListener);
    }
}
```


requestPeers（）方法也是异步的，并且可以在WifiP2pManager.PeerListListener接口中定义的onPeersAvailable（）可用的对等体列表时通知您的活动。 onPeersAvailable（）方法提供了一个WifiP2pDeviceList，您可以通过它进行迭代查找要连接到的对等体。




二）、连接peer

在获取可能的对等体列表之后，当您找出想要连接的设备时，请调用connect（）方法连接到设备。 此方法调用需要一个包含要连接的设备的信息的WifiP2pConfig对象。 您可以通过WifiP2pManager.ActionListener通知连接成功或失败。 以下代码显示如何创建到所需设备的连接：




```java
//obtain a peer from the WifiP2pDeviceList
WifiP2pDevice device;
WifiP2pConfig config = new WifiP2pConfig();
config.deviceAddress = device.deviceAddress;
mManager.connect(mChannel, config, new ActionListener() {

    @Override
    public void onSuccess() {
        //success logic
    }

    @Override
    public void onFailure(int reason) {
        //failure logic
    }
});
```


三）、传输数据

建立连接后，您可以使用插座在设备之间传输数据。 数据传输的基本步骤如下：


1、创建一个ServerSocket。 该套接字等待来自指定端口上的客户端的连接，并阻塞直到发生，因此在后台线程中执行此操作。




2、创建客户端套接字。 客户端使用服务器套接字的IP地址和端口连接到服务器设备。




3、将数据从客户端发送到服务器。 当客户端套接字成功连接到服务器套接字时，您可以使用字节流将数据从客户端发送到服务器。




4、服务器套接字等待客户端连接（使用accept（）方法）。 这个调用阻塞直到客户端连接，所以调用这是另一个线程。 当连接发生时，服务器设备可以从客户端接收数据。 对此数据执行任何操作，例如将其保存到文件或将其呈现给用户。




从Wi-Fi P2P Demo示例修改的以下示例显示了如何创建客户端 - 服务器套接字通信，并将JPEG图像从客户端传输到具有服务的服务器。 有关完整的工作示例，请编译并运行Wi-Fi P2P Demo示例。




```java
public static class FileServerAsyncTask extends AsyncTask {

    private Context context;
    private TextView statusText;

    public FileServerAsyncTask(Context context, View statusText) {
        this.context = context;
        this.statusText = (TextView) statusText;
    }

    @Override
    protected String doInBackground(Void... params) {
        try {

            /**
             * Create a server socket and wait for client connections. This
             * call blocks until a connection is accepted from a client
             */
            ServerSocket serverSocket = new ServerSocket(8888);
            Socket client = serverSocket.accept();

            /**
             * If this code is reached, a client has connected and transferred data
             * Save the input stream from the client as a JPEG file
             */
            final File f = new File(Environment.getExternalStorageDirectory() + "/"
                    + context.getPackageName() + "/wifip2pshared-" + System.currentTimeMillis()
                    + ".jpg");

            File dirs = new File(f.getParent());
            if (!dirs.exists())
                dirs.mkdirs();
            f.createNewFile();
            InputStream inputstream = client.getInputStream();
            copyFile(inputstream, new FileOutputStream(f));
            serverSocket.close();
            return f.getAbsolutePath();
        } catch (IOException e) {
            Log.e(WiFiDirectActivity.TAG, e.getMessage());
            return null;
        }
    }

    /**
     * Start activity that can handle the JPEG image
     */
    @Override
    protected void onPostExecute(String result) {
        if (result != null) {
            statusText.setText("File copied - " + result);
            Intent intent = new Intent();
            intent.setAction(android.content.Intent.ACTION_VIEW);
            intent.setDataAndType(Uri.parse("file://" + result), "image/*");
            context.startActivity(intent);
        }
    }
}
```


5、在客户端上，使用客户端套接字连接到服务器套接字并传输数据。 此示例在客户端设备的文件系统上传输JPEG文件。



```java
Context context = this.getApplicationContext();
String host;
int port;
int len;
Socket socket = new Socket();
byte buf[]  = new byte[1024];
...
try {
    /**
     * Create a client socket with the host,
     * port, and timeout information.
     */
    socket.bind(null);
    socket.connect((new InetSocketAddress(host, port)), 500);

    /**
     * Create a byte stream from a JPEG file and pipe it to the output stream
     * of the socket. This data will be retrieved by the server device.
     */
    OutputStream outputStream = socket.getOutputStream();
    ContentResolver cr = context.getContentResolver();
    InputStream inputStream = null;
    inputStream = cr.openInputStream(Uri.parse("path/to/picture.jpg"));
    while ((len = inputStream.read(buf)) != -1) {
        outputStream.write(buf, 0, len);
    }
    outputStream.close();
    inputStream.close();
} catch (FileNotFoundException e) {
    //catch logic
} catch (IOException e) {
    //catch logic
}

/**
 * Clean up any open sockets when done
 * transferring or if an exception occurred.
 */
finally {
    if (socket != null) {
        if (socket.isConnected()) {
            try {
                socket.close();
            } catch (IOException e) {
                //catch logic
            }
        }
    }
}
```





