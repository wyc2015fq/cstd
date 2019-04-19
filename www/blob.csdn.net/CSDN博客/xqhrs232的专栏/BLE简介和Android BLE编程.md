# BLE简介和Android BLE编程 - xqhrs232的专栏 - CSDN博客
2015年02月28日 16:19:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1187
原文地址::[http://www.tuicool.com/articles/quiuMv](http://www.tuicool.com/articles/quiuMv)
原文[http://blog.csdn.net/cnbloger/article/details/41382653](http://blog.csdn.net/cnbloger/article/details/41382653?utm_source=tuicool)
一．BLE和BT区别
其实我知道许多程序员不太喜欢阅读除了代码以外的文档，因为有时这些过于冗长的文档对编程并没有更多的好处，有了协议，接口，demo差不多很多人就能写出很好质量的代码了。但其实更深入的编程是少了阅读的，阅读文档可以解决很多编程中遇到的困难，比如在大数据大流量情况下，很多正常的程序会表现出不可靠的一面，这已经不是够编程能解决的了，硬件的配置，服务器带宽，使用的数据库，调用的接口都有可能是瓶颈。比如BLE，同样的蓝牙，但却有着本质区别，一个表现就是不是所有蓝牙设备都支持BLE，编程如果不考虑这个问题，程序出错就会迷茫。再比如连接到BLE或者蓝牙上的设备是有数量限制的，如果不知道这个限制，在连接不上时，也会六神无主。
BLE在智家居中和移动设备中的应用方兴未艾，做深入的研究推广到更多的应用上，还是有意义的。
1蓝牙的历史：蓝牙的创始公司是爱立信。1994年爱立信开始对小范围无线通信技术进行研发，在1997年，爱立信的研究激发了其他公司的浓厚兴趣，于是1998年2月，一些跨国大公司包括诺基亚、苹果、三星组成的一个特殊兴趣小组（SIG），他们共同的目标是建立一个全球性的小范围无线通信技术，该项技术就是蓝牙。
２.BLE 是Bluetooth Low Energy的缩写，又叫蓝牙4.0，区别于蓝牙3.0和之前的技术。BLE前身是NOKIA开发的Wibree技术，主要用于实现移动智能终端与周边配件之间的持续连接，是功耗极低的短距离无线通信技术，并且有效传输距离被提升到了100米以上，同时只需要一颗纽扣电池就可以工作数年之久。3. BLE是在蓝牙技术的基础上发展起来的，既同于蓝牙，又区别于传统蓝牙。BLE设备分单模和双模两种，双模简称BR，商标为Bluetooth Smart Ready，单模简称BLE或者LE,商标为Bluetooth Smart。Android是在4.3后才支持BLE，这可以解释不是所有蓝牙手机都支持BLE，而且支持BLE的蓝牙手机一般是双模的。4.双模兼容传统蓝牙，可以和传统蓝牙通信，也可以和BLE通信，常用在手机上，android4.3和IOS4.0之后版本都支持BR，也就是双模设备。单模只能和BR和单模的设备通信，不能和传统蓝牙通信，由于功耗低，待机长，所以常用在手环的智能设备上。这可以解释手机上的BLE与手环等设备上的BLE的区别。5不是所有手机都支持BLE，因为BLE不仅仅依靠软件实现，同时需要硬件支持，于是有很多手机不能联接智能手环等设备。Android4.3手机上安装的是双模BR，因此兼容蓝牙3.0之前的技术，既能与BLE设备通信，也能与传统蓝牙通信，比较耗电，能够像传统设备一样高速传输。大部分智能手环使用的单工BLE，不支持传统蓝牙，不能与之联结和通信，低功耗低速率设备。
二．BLE（低功耗蓝牙）的用途
应用特点：小包传输，手机扩展，低功耗
☆ 2.4G 蓝牙低功耗系统
☆ 消费类电子产品
☆ 秱劢电话外围扩展设备
☆ 运劢和休闲设备
☆ 健康医疗用品 (血压计，体温计 …)
☆ 汽车电子设备
☆ 人机接口设备 (键盘，鼠标，遥控器 …)
☆ USB Dongle
三 蓝牙的工作原理
1 蓝牙通信的主从关系
蓝牙技术规定每一对设备之间进行蓝牙通讯时，必须一个为主角色，另一为从角色，才能进行通信，通信时，必须由主端进行查找，发起配对，建链成功后，双方即可收发数据。理论上，一个蓝牙主端设备，可同时与7个蓝牙从端设备进行通讯。一个具备蓝牙通讯功能的设备，可以在两个角色间切换，平时工作在从模式，等待其它主设备来连接，需要时，转换为主模式，向其它设备发起呼叫。一个蓝牙设备以主模式发起呼叫时，需要知道对方的蓝牙地址，配对密码等信息，配对完成后，可直接发起呼叫。这可以解释为什么有时无法连接蓝牙，有可能是连接的蓝牙设备过多。
2蓝牙的呼叫过程
蓝牙主端设备发起呼叫，首先是查找，找出周围处于可被查找的蓝牙设备。主端设备找到从端蓝牙设备后，与从端蓝牙设备进行配对，此时需要输入从端设备的PIN码，也有设备不需要输入PIN码。配对完成后，从端蓝牙设备会记录主端设备的信任信息，此时主端即可向从端设备发起呼叫，已配对的设备在下次呼叫时，不再需要重新配对。已配对的设备，做为从端的蓝牙耳机也可以发起建链请求，但做数据通讯的蓝牙模块一般不发起呼叫。链路建立成功后，主从两端之间即可进行双向的数据或语音通讯。在通信状态下，主端和从端设备都可以发起断链，断开蓝牙链路。
3 蓝牙一对一的串口数据传输应用
蓝牙数据传输应用中，一对一串口数据通讯是最常见的应用之一，蓝牙设备在出厂前即提前设好两个蓝牙设备之间的配对信息，主端预存有从端设备的PIN码、地址等，两端设备加电即自动建链，透明串口传输，无需外围电路干预。一对一应用中从端设备可以设为两种类型，一是静默状态，即只能与指定的主端通信，不被别的蓝牙设备查找；二是开发状态，既可被指定主端查找，也可以被别的蓝牙设备查找建链。
四．Android与BLE
#### 1.关键概念：
#### Generic Attribute Profile (GATT)
通过BLE连接，读写属性类小数据的Profile通用规范。现在所有的BLE应用Profile都是基于GATT的。
#### Attribute Protocol (ATT)
GATT是基于ATTProtocol的。ATT针对BLE设备做了专门的优化，具体就是在传输过程中使用尽量少的数据。每个属性都有一个唯一的UUID，属性将以characteristics and services的形式传输。
#### Characteristic
Characteristic可以理解为一个数据类型，它包括一个value和0至多个对次value的描述（Descriptor）。
#### Descriptor
对Characteristic的描述，例如范围、计量单位等。
#### Service
Characteristic的集合。例如一个service叫做“Heart Rate Monitor”，它可能包含多个Characteristics，其中可能包含一个叫做“heart ratemeasurement"的Characteristic。一个手环可能
#### 2.角色和职责：
Android设备与BLE设备交互有两组角色：
中心设备和外围设备（Central vs. peripheral）；
GATT server vs. GATT client.
Central vs. peripheral:
中心设备和外围设备的概念针对的是BLE连接本身。Central角色负责scan advertisement。而peripheral角色负责make advertisement。
GATT server vs. GATT client:
这两种角色取决于BLE连接成功后，两个设备间通信的方式。
举例说明：
现 有一个活动追踪的BLE设备和一个支持BLE的Android设备。Android设备支持Central角色，而BLE设备支持peripheral角色。创建一个BLE连接需要这两个角色都存在，都仅支持Central角色或者都仅支持peripheral角色则无法建立连接。
当 连接建立后，它们之间就需要传输GATT数据。谁做server，谁做client，则取决于具体数据传输的情况。例如，如果活动追踪的BLE设备需要向 Android设备传输sensor数据，则活动追踪器自然成为了server端；而如果活动追踪器需要从Android设备获取更新信息，则 Android设备作为server端可能更合适。
#### 3权限及feature：
和经典蓝牙一样，应用使用蓝牙，需要声明BLUETOOTH权限，如果需要扫描设备或者操作蓝牙设置，则还需要BLUETOOTH_ADMIN权限：
<uses-permissionandroid:name="android.permission.BLUETOOTH"/><uses-permission android:name="android.permission.BLUETOOTH_ADMIN"/>
除了蓝牙权限外，如果需要BLE feature则还需要声明uses-feature：
<uses-featureandroid:name="android.hardware.bluetooth_le"android:required="true"/>
按时required为true时，则应用只能在支持BLE的Android设备上安装运行；required为false时，Android设备均可正常安装运行，需要在代码运行时判断设备是否支持BLE feature：
// Use this check to determine whether BLE is supportedon the device. Then
// you can selectively disable BLE-related features.
if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)){
Toast.makeText(this, R.string.ble_not_supported,Toast.LENGTH_SHORT).show();
finish();
}
#### 4、启动蓝牙：
在使用蓝牙BLE之前，需要确认Android设备是否支持BLE feature(required为false时)，另外要需要确认蓝牙是否打开。
如果发现不支持BLE，则不能使用BLE相关的功能。如果支持BLE，但是蓝牙没打开，则需要打开蓝牙。
打开蓝牙的步骤：
1、获取BluetoothAdapter
BluetoothAdapter是Android系统中所有蓝牙操作都需要的，它对应本地Android设备的蓝牙模块，在整个系统中BluetoothAdapter是单例的。当你获取到它的示例之后，就能进行相关的蓝牙操作了。
获取BluetoothAdapter代码示例如下：
// Initializes Bluetooth adapter.
final BluetoothManager bluetoothManager =
(BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
mBluetoothAdapter = bluetoothManager.getAdapter();
注：这里通过getSystemService获取BluetoothManager，再通过BluetoothManager获取BluetoothAdapter。BluetoothManager在Android4.3以上支持(API level 18)。
2、判断是否支持蓝牙，并打开蓝牙
获取到BluetoothAdapter之后，还需要判断是否支持蓝牙，以及蓝牙是否打开。
如果没打开，需要让用户打开蓝牙：
private BluetoothAdapter mBluetoothAdapter;
...
// Ensures Bluetooth is available on the device and it is enabled. If not,
// displays a dialog requesting user permission to enable Bluetooth.
if (mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled()) {
Intent enableBtIntent = newIntent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
}
#### 5、搜索BLE设备：
通过调用BluetoothAdapter的 [startLeScan()](http://developer.android.com/reference/android/bluetooth/BluetoothAdapter.html#startLeScan%28android.bluetooth.BluetoothAdapter.LeScanCallback%29)搜索BLE设备。调用此方法时需要传入 [BluetoothAdapter.LeScanCallback](http://developer.android.com/reference/android/bluetooth/BluetoothAdapter.LeScanCallback.html)参数。
因此你需要实现 [BluetoothAdapter.LeScanCallback](http://developer.android.com/reference/android/bluetooth/BluetoothAdapter.LeScanCallback.html)接口，BLE设备的搜索结果将通过这个callback返回。
由于搜索需要尽量减少功耗，因此在实际使用时需要注意：
1、当找到对应的设备后，立即停止扫描；
2、不要循环搜索设备，为每次搜索设置适合的时间限制。避免设备不在可用范围的时候持续不停扫描，消耗电量。
搜索的示例代码如下：
```java
/**
 * Activity for scanning and displaying available BLE devices.
 */
public class DeviceScanActivity extends ListActivity {
  private BluetoothAdapter mBluetoothAdapter;
  private boolean mScanning;
  private Handler mHandler;
  // Stops scanning after 10 seconds.
  private static final long SCAN_PERIOD = 10000;
  ...
  private void scanLeDevice(final boolean enable) {
    if (enable) {
      // Stops scanning after a pre-defined scan period.
      mHandler.postDelayed(new Runnable() {
        @Override
        public void run() {
          mScanning = false;
          mBluetoothAdapter.stopLeScan(mLeScanCallback);
        }
      }, SCAN_PERIOD);
      mScanning = true;
      mBluetoothAdapter.startLeScan(mLeScanCallback);
    } else {
      mScanning = false;
      mBluetoothAdapter.stopLeScan(mLeScanCallback);
    }
    
  }
}
```
如果你只需要搜索指定UUID的外设，你可以调用 [startLeScan(UUID[],
 BluetoothAdapter.LeScanCallback)](http://developer.android.com/reference/android/bluetooth/BluetoothAdapter.html#startLeScan%28android.bluetooth.BluetoothAdapter.LeScanCallback%29)方法。
其中UUID数组指定你的应用程序所支持的GATT Services的UUID。
[BluetoothAdapter.LeScanCallback](http://developer.android.com/reference/android/bluetooth/BluetoothAdapter.LeScanCallback.html)的实现示例如下：
```java
private LeDeviceListAdapter mLeDeviceListAdapter;
...
// Device scan callback.
private BluetoothAdapter.LeScanCallback mLeScanCallback =
  new BluetoothAdapter.LeScanCallback() {
    @Override
    public void onLeScan(final BluetoothDevice device, int rssi,
      byte[] scanRecord) {
  runOnUiThread(new Runnable() {
     @Override
     public void run() {
         mLeDeviceListAdapter.addDevice(device);
         mLeDeviceListAdapter.notifyDataSetChanged();
     }
       });
   }
};
```
注意：搜索时，你只能搜索传统蓝牙设备或者BLE设备，两者完全独立，不可同时被搜索。
#### 6、连接GATTServer：
两个设备通过BLE通信，首先需要建立GATT连接。这里我们讲的是Android设备作为client端，连接GATT Server。
连接GATT Server，你需要调用BluetoothDevice的 [connectGatt()](http://developer.android.com/reference/android/bluetooth/BluetoothDevice.html#connectGatt%28android.content.Context,%20boolean,%20android.bluetooth.BluetoothGattCallback%29) 方法。此函数带三个参数：Context、autoConnect(boolean)和 [BluetoothGattCallback](http://developer.android.com/reference/android/bluetooth/BluetoothGattCallback.html) 对象。调用示例：
mBluetoothGatt = device.connectGatt(this, false,mGattCallback);
函数成功，返回 [BluetoothGatt](http://developer.android.com/reference/android/bluetooth/BluetoothGatt.html) 对象，它是GATT profile的封装。通过这个对象，我们就能进行GATT Client端的相关操作。 [BluetoothGattCallback](http://developer.android.com/reference/android/bluetooth/BluetoothGattCallback.html) 用于传递一些连接状态及结果。
BluetoothGatt常规用到的几个操作示例:
connect() ：连接远程设备。
discoverServices() : 搜索连接设备所支持的service。
disconnect()：断开与远程设备的GATT连接。
close()：关闭GATTClient端。
readCharacteristic(characteristic) ：读取指定的characteristic。
setCharacteristicNotification(characteristic, enabled)：设置当指定characteristic值变化时，发出通知。
getServices() ：获取远程设备所支持的services。
等等。
注：
1、某些函数调用之间存在先后关系。例如首先需要connect上才能discoverServices。
2、 一些函数调用是异步的，需要得到的值不会立即返回，而会在BluetoothGattCallback的回调函数中返回。例如 discoverServices与onServicesDiscovered回调，readCharacteristic与 onCharacteristicRead回调，setCharacteristicNotification与onCharacteristicChanged回调等。
[http://www.blogjava.net/zh-weir/archive/2013/12/09/407373.html](http://www.blogjava.net/zh-weir/archive/2013/12/09/407373.html)
#### http://blog.csdn.net/mov2012/article/details/16368441
百度文档：
BLE入门教程.pdf
BLE_和经典蓝牙Android编程说明.pdf
