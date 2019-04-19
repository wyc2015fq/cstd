# android wifi连接+基于socket的文件数据传输（上） - 右上角同学的专栏 - CSDN博客
2016年06月27日 22:06:35[右上角徐](https://me.csdn.net/u011032983)阅读数：6409
分客户端和服务器端两大部分介绍：
**客户端：**
1.WiFi连接：
1.1开启WiFi
```java
public void openWifi () {
              if (!mWifiManager .isWifiEnabled()) {
                 mWifiManager.setWifiEnabled(true);
             }
         }
```
1.2添加一个网络并连接（添加网络即将此WifiConfiguration至wificonfiglist）
```java
mWifiAdmin.addNetwork( mWifiAdmin.createWifiInfo(Constant.HOST_SPOT_SSID,        Constant.HOST_SPOT_PASS_WORD , WifiAdmin. TYPE_WPA));
```
     具体实现函数：
     // 添加一个网络并连接
```java
public void addNetwork(WifiConfiguration wcg) {
       
        register();
       
        WifiApAdmin. closeWifiAp(mContext);
       
        int wcgID = mWifiManager .addNetwork(wcg );//成功返回wifiId,否则-1
        boolean b = mWifiManager.enableNetwork(wcgID, true);
    }
```
     可见，addNetwork之前需要先根据SSID,password，type（加密协议）创建WifiConfiguration，，并且每次都需重新创建（若已经在wificonfiglist 列表存在，则先删除），然后通过.addNetwork函数添加至在wificonfiglist 列表，然后通过.enableNetwork方法连接该WiFi。
2.socket通信：
 2.1.获取服务器的IP，端口信息
     首先获得服务器端即热点的IP
```java
WifiManager wifiManage = (WifiManager) getSystemService (Context.WIFI_SERVICE );
                            DhcpInfo info = wifiManage.getDhcpInfo ();
     String serverAddress = intToIp(info. serverAddress );
```
 2.2将服务器端IP和要传输数据作为参数构造客户端线程（），并启动。
```java
client=new Sender (serverAddress, messagec);
                            client.start();
```
     在Sender线程类的run方法中，先声明sock，其中参数为IP地址和自定义端口，定义输出流和输入流，其中客户端输出流发送数据至服务器端，目的是唤醒服务器端输入流内阻塞中的read函数，让服务器端自动继续往下执行输出流发送数据的程序；客户端输入流接收从服务器端传入的数据，放于可变字符串StringBuffer内，再由handler机制，传至主线程，刷新TextView控件
**服务器端：**
有点累，下篇再整理。。。
