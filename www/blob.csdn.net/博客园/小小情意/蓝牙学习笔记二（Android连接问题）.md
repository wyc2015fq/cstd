
# 蓝牙学习笔记二（Android连接问题） - 小小情意 - 博客园






# [蓝牙学习笔记二（Android连接问题）](https://www.cnblogs.com/xiaoxiaoqingyi/p/9293206.html)
可以通过以下两点加速蓝牙连接：
1、更新连接参数
interval：连接间隔（connection intervals ），范围在 7.5 毫秒 到 4 秒。
latency：连接延迟
。。。 还有一些其它参数.
Android API不提供具体的参数值， 只提供了三个常量：
CONNECTION_PRIORITY_HIGH
CONNECTION_PRIORITY_BALANCED
CONNECTION_PRIORITY_LOW_POWER
从Android的源码找到对应的参数：
![](https://img-blog.csdn.net/20180711101849664?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAyMDM3MTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在发起连接请求时，通过BluetoothGatt实例发起更新：
mBluetoothGatt.requestConnectionPriority(BluetoothGatt.CONNECTION_PRIORITY_HIGH);
参考资料：[http://www.novelbits.io/ble-connection-intervals/](http://www.novelbits.io/ble-connection-intervals/)
2、在请求连接时，设置自动连接标识为 false。
自动连接标识为true 时，虽然能提高连接成功的概率，但是会导致连接时间加长.
具体解释如下：
![](https://img-blog.csdn.net/20180711101900892?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAyMDM3MTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
资料链接：[https://github.com/iDevicesInc/SweetBlue/wiki/Android-BLE-Issues](https://github.com/iDevicesInc/SweetBlue/wiki/Android-BLE-Issues)
连接失败的问题
由于Android的蓝牙协议栈bluedroid 在低版本存在一些bug，如当连接的时候，callback 返回 status=133 的情况. 出现这种情况的原因应该手机APP频繁的操作连接以及断开。解决的方式可以参考以下几种：
1、当断开连接时，调用mBluetoothGatt.disconnect(); 该方式只是断开连接，并没有真正释放资源，可以在 disconnect 的回调里调用 mBluetoothGatt.close()
2、当连接同一台设备时，可通过判断地址是否一样，使用mBluetoothGatt.connect(); 但这方式似乎会导致连接放慢.
3、出现这种情况，delay 一会儿，然后再重连. 只能通过多次重连方式。
4、从google 回来的一些资料显示，当手机断开连接 mBluetoothGatt.disconnect(); 甚至是mBluetoothGatt.close()。 app蓝牙只是给蓝牙服务发送了一个断开连接，蓝牙服务是以队列的形式去处理它. 要彻底断开该连接，可以通过让对方设备（从设备）也主动断开。我尝试过这种方式，确实解决了我当前的问题.
connection fail 资料：
[https://issuetracker.google.com/issues/37127889](https://issuetracker.google.com/issues/37127889)
[https://issuetracker.google.com/issues/36995652](https://issuetracker.google.com/issues/36995652)
status=133， 对于状态的错误：GATT_ERROR
[https://android.googlesource.com/platform/external/bluetooth/bluedroid/+/android-4.4.4_r2.0.1/stack/include/gatt_api.h](https://android.googlesource.com/platform/external/bluetooth/bluedroid/+/android-4.4.4_r2.0.1/stack/include/gatt_api.h)
connections fail ：
1）If your connections fail to happen, the BLE peripheral may be sending packets too slowly, or their signal level may be too low. This can be an issue with output power, range between the devices, interference, or other issues.
2）The BLE connection settings are some of the most critical parameters to understand in BLE. Connection interval, slave latency and other settings are sent upon during a connection, and they can tell you a lot about what the devices agree to. If you’re having connection problems, it can be that the two devices don’t agree on the parameters (and the central device disconnects).

另外可以查看一下Android源码 packages\apps\Bluetooth 目录下 bluetooth APP 的源码，看处理的方式.

BLE 连接过程：
![](https://img-blog.csdn.net/20180711101916309?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTAyMDM3MTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注：图中M代表手机，S代表设备B，M->S表示手机将数据包发给设备B，即手机开启Tx窗口，设备B开启Rx窗口；S->M正好相反，表示设备B将数据包发给手机，即设备B开启Tx窗口，手机开启Rx窗口。
如图所示，手机在收到A1广播包ADV_IND后，以此为初始锚点（这个锚点不是连接的锚点），T_IFS后给Advertiser发送一个connection request命令，即A2数据包，告诉advertiser我将要过来连你，请做好准备。Advertiser根据connect_req命令信息做好接收准备，connect_req包含如下关键信息：
Transmit window offset，发送窗口偏移
Transmit window size， 发送窗口大小
connection interval  连接间隔
latency 从设备延迟
timeout 监控超时
.......
这些参数可以由主设备确定，也可以让从设备确定. 我们取了最优（也是最耗电的配置）
connection interval = 7.5ms
latency = 0
timeout = 4000ms
当主设备发起一个连接请求后（意思告诉从设备，我要连你， 要你做好准备）， 在一个1.25ms时间单位， 加上发送窗口大小，发送窗口偏移时间后，发送一个空白给 从设备，并准备接收从设备的包，当接收到从设备的空包后，表示连接正式确立。我们可以理想大概这样计算连接时间:
time = 1.25ms + Transmit window offset + Transmit window size +  connection interval
连接成功后，master和slave在每一个connection interval开始的时候，都必须交互一次，即master给slave发一个包，slave再给master发一个包，整个交互过程称为一个connection event。
参考资料：BLE连接建立过程[http://www.mamicode.com/info-detail-2275982.html](http://www.mamicode.com/info-detail-2275982.html)

其它资料：
Android BLE Issues：
开源框架 SweetBlue 对蓝牙问题的总结：
[https://github.com/iDevicesInc/SweetBlue/wiki/Android-BLE-Issues](https://github.com/iDevicesInc/SweetBlue/wiki/Android-BLE-Issues)
google bluetooth的问题列表：
[https://android.googlesource.com/platform/packages/apps/Bluetooth](https://android.googlesource.com/platform/packages/apps/Bluetooth)
[Android 4.3 Bluetooth Low Energy unstable](https://stackoverflow.com/questions/17870189/android-4-3-bluetooth-low-energy-unstable)
[https://stackoverflow.com/questions/17870189/android-4-3-bluetooth-low-energy-unstable](https://stackoverflow.com/questions/17870189/android-4-3-bluetooth-low-energy-unstable)





