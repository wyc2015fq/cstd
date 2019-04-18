# Android音视频API(android.media.midi)：概览 - 一世豁然的专栏 - CSDN博客





2017年07月07日 09:22:53[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2704








提供通过USB，蓝牙LE和虚拟（应用程序间）传输使用标准MIDI事件协议发送和接收消息的类。




一、概要

Android MIDI包允许用户：


1、将MIDI键盘连接到Android以播放合成器或驱动音乐应用程序。

2、将可选的MIDI控制器连接到Android。

3、从Android驱动外部MIDI合成器。

4、从Android驱动外部外设，灯光，显示控制等。

5、从游戏或音乐制作应用程序动态生成音乐。

6、在一个应用程序中生成MIDI消息并将其发送到第二个应用程序。

7、使用以外设模式运行的Android设备作为连接到笔记本电脑的多点触控控制器。




一）、API功能包括：

1、枚举当前可用的设备。 信息包括名称，供应商，能力等。

2、在MIDI设备插入或拔出时提供通知。

3、支持单个或多个短1-3字节MIDI消息的高效传输。

4、支持SysEx等任意长度数据的传输。

5、时间戳以避免抖动。

6、支持创建可连接到其他设备的虚拟MIDI设备。 一个例子可能是一个可由合成应用程序控制的合成器应用程序。

7、支持设备的直接连接或“修补”以降低延迟。




二）、传输支持

API是“传输不可知论”。 但目前有几种运输支持：


1、USB

2、软件路由

3、BTLE







二、Android MIDI术语

一）、术语

**设备**是具有零个或多个InputPorts和OutputPorts的具有MIDI能力的对象。


**InputPort**有16个通道，可以从OutputPort或应用程序接收MIDI消息。


**OutputPort**有16个通道，可以将MIDI信息发送到InputPort或应用程序。


**MidiService**是一个集中的过程，可以跟踪所有设备和经纪人之间的沟通。


**MidiManager**是应用程序或设备管理器调用与MidiService进行通信的类。








三、编写MIDI应用程序

一）、在清单中声明功能

需要MIDI API的应用程序应声明在AndroidManifest.xml文件中。 那么对于不支持MIDI API的旧设备，该应用不会出现在Play商店中。



`<uses-feature android:name="android.software.midi" android:required="true"/>`

二）、检查功能支持

应用程序还可以在运行时检查平台上是否支持MIDI功能。 当您直接在设备上安装应用程序时，这在开发过程中特别有用。




```java
if (context.getPackageManager().hasSystemFeature(PackageManager.FEATURE_MIDI)) {
    // do MIDI stuff
}
```


三）、MidiManager

访问MIDI包的主要类是通过MidiManager。




```java
MidiManager m = (MidiManager)context.getSystemService(Context.MIDI_SERVICE);
```


四）、获取已插入实体的列表

当应用程序启动时，它可以获得所有可用MIDI设备的列表。 该信息可以呈现给用户，允许他们选择一个设备。




```java
MidiDeviceInfo[] infos = m.getDevices();
```


五）、通知MIDI设备HotPlug事件

例如，当插入或拔出键盘时，应用程序可以请求通知。




```java
m.registerDeviceCallback(new MidiManager.DeviceCallback() {
    public void onDeviceAdded( MidiDeviceInfo info ) {
      ...
    }
    public void onDeviceRemoved( MidiDeviceInfo info ) {
      ...
    }
  });
```


六）、设备和端口信息

您可以查询输入和输出端口的数量。




```java
int numInputs = info.getInputPortCount();
int numOutputs = info.getOutputPortCount();
```


注意，“输入”和“输出”是从设备的角度出发的。 因此，合成器将具有接收消息的“输入”端口。 键盘将有一个“输出”端口发送消息。




MidiDeviceInfo有一组属性。




```java
Bundle properties = info.getProperties();
String manufacturer = properties
      .getString(MidiDeviceInfo.PROPERTY_MANUFACTURER);
```


其他资源包括PROPERTY_PRODUCT，PROPERTY_NAME，PROPERTY_SERIAL_NUMBER




您可以从PortInfo对象获取端口的名称和类型。 类型将是TYPE_INPUT或TYPE_OUTPUT。




```java
MidiDeviceInfo.PortInfo[] portInfos = info.getPorts();
String portName = portInfos[0].getName();
if (portInfos[0].getType() == MidiDeviceInfo.PortInfo.TYPE_INPUT) {
    ...
}
```


七）、打开MIDI设备

要访问MIDI设备，您需要先打开它。 open是异步的，所以你需要提供一个回调来完成。 如果要在特定线程上发生回调，可以指定可选的处理程序。




```java
m.openDevice(info, new MidiManager.OnDeviceOpenedListener() {
    @Override
    public void onDeviceOpened(MidiDevice device) {
        if (device == null) {
            Log.e(TAG, "could not open device " + info);
        } else {
            ...
        }
    }, new Handler(Looper.getMainLooper())
    );
```


八）、打开MIDI输入端口

如果要将信息发送到MIDI设备，则需要打开具有独占访问权限的“输入”端口。




```java
MidiInputPort inputPort = device.openInputPort(index);
```


九）、发送一个NoteOn

MIDI消息作为字节数组发送。 在这里我们编码一个注释消息。




```java
byte[] buffer = new byte[32];
int numBytes = 0;
int channel = 3; // MIDI channels 1-16 are encoded as 0-15.
buffer[numBytes++] = (byte)(0x90 + (channel - 1)); // note on
buffer[numBytes++] = (byte)60; // pitch is middle C
buffer[numBytes++] = (byte)127; // max velocity
int offset = 0;
// post is non-blocking
inputPort.send(buffer, offset, numBytes);
```


有时可以方便地发送带有时间戳的MIDI信息。 通过调度将来的事件，我们可以掩盖调度抖动。 Android MIDI时间戳基于单调纳秒系统定时器。 这与其他音频和输入定时器一致。




在这里，我们将来发送一个时间戳为2秒的消息。




```java
final long NANOS_PER_SECOND = 1000000000L;
long now = System.nanoTime();
long future = now + (2 * NANOS_PER_SECOND);
inputPort.send(buffer, offset, numBytes, future);
```


如果要取消以后安排的事件，请调用flush（）。



```java
inputPort.flush(); // discard events
```


如果缓冲区中有任何MIDI NoteOff消息，那么它们将被丢弃，您可能会收到卡纸。 所以我们建议在进行冲水后发送“所有音符关闭”。




十）、接收一个Note

要从设备接收MIDI数据，您需要扩展MidiReceiver。 然后将您的接收机连接到设备的输出端口。




```java
class MyReceiver extends MidiReceiver {
    public void onSend(byte[] data, int offset,
            int count, long timestamp) throws IOException {
        // parse MIDI or whatever
    }
}
MidiOutputPort outputPort = device.openOutputPort(index);
outputPort.connect(new MyReceiver());
```


到达的数据不会以任何特定的方式进行验证或对齐。 它是原始的MIDI数据，可以包含多个消息或部分消息。 它可能包含系统实时消息，可以在其他消息中进行交织。







四、创建MIDI虚拟设备服务

一个应用程序可以提供可由其他应用程序使用的MIDI服务。 例如，应用程序可以提供其他应用程序可以发送消息的自定义合成器。 该服务必须通过“android.permission.BIND_MIDI_DEVICE_SERVICE”的许可进行保护。





一）、清单文件

一个应用程序声明它将在AndroidManifest.xml文件中用作MIDI服务器。




```
<service android:name="MySynthDeviceService"
  android:permission="android.permission.BIND_MIDI_DEVICE_SERVICE">
  <intent-filter>
    <action android:name="android.media.midi.MidiDeviceService" />
  </intent-filter>
  <meta-data android:name="android.media.midi.MidiDeviceService"
      android:resource="@xml/synth_device_info" />
</service>
```


此示例中的资源的详细信息存储在“res / xml / synth_device_info.xml”中。 您在此文件中声明的端口名称将可从PortInfo.getName（）获得。



```
<devices>
    <device manufacturer="MyCompany" product="MidiSynthBasic">
        <input-port name="input" />
    </device>
</devices>
```


二）、扩展MidiDeviceService

然后，通过扩展android.media.midi.MidiDeviceService来定义服务器。 假设你有一个扩展MidiReceiver的MySynthEngine类。




```java
import android.media.midi.MidiDeviceService;
import android.media.midi.MidiDeviceStatus;
import android.media.midi.MidiReceiver;

public class MidiSynthDeviceService extends MidiDeviceService {
    private static final String TAG = "MidiSynthDeviceService";
    private MySynthEngine mSynthEngine = new MySynthEngine();
    private boolean synthStarted = false;

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Override
    public void onDestroy() {
        mSynthEngine.stop();
        super.onDestroy();
    }

    @Override
    // Declare the receivers associated with your input ports.
    public MidiReceiver[] onGetInputPortReceivers() {
        return new MidiReceiver[] { mSynthEngine };
    }

    /**
     * This will get called when clients connect or disconnect.
     * You can use it to turn on your synth only when needed.
     */
    @Override
    public void onDeviceStatusChanged(MidiDeviceStatus status) {
        if (status.isInputPortOpen(0) && !synthStarted) {
            mSynthEngine.start();
            synthStarted = true;
        } else if (!status.isInputPortOpen(0) && synthStarted){
            mSynthEngine.stop();
            synthStarted = false;
        }
    }
}
```





五、在蓝牙LE上使用MIDI

MIDI设备可以使用蓝牙LE连接到Android。





在使用设备之前，应用程序必须扫描可用的BTLE设备，然后允许用户连接。 将提供一个示例程序，以便在Android开发者网站上查找。





一）、请求BTLE的位置许可

扫描蓝牙设备的应用程序必须在清单文件中请求许可。 需要此LOCATION权限，因为可能会通过查看附近的BTLE设备来猜测Android设备的位置。




```
<uses-permission android:name="android.permission.BLUETOOTH"/>
<uses-permission android:name="android.permission.BLUETOOTH_ADMIN"/>
<uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION"/>
```


应用程序还必须在运行时请求用户的位置许可。 有关详细信息和示例，请参阅Activity.requestPermissions（）的文档。




二）、扫描MIDI设备

该应用程序只想看到MIDI设备，而不是鼠标或其他非MIDI设备。 因此，使用UUID构建一个ScanFilter，用于基于BTLE的标准MIDI。




```java
MIDI over BTLE UUID = "03B80E5A-EDE8-4B33-A751-6CE34EC4C700"
```


三）、打开一个MIDI蓝牙设备

有关详细信息，请参阅android.bluetooth.le.BluetoothLeScanner.startScan（）方法的文档。 当用户选择MIDI / BTLE设备时，您可以使用MidiManager打开它。




```java
m.openBluetoothDevice(bluetoothDevice, callback, handler);
```


一旦MIDI / BTLE设备被一个应用程序打开，那么也可以使用上述MIDI设备发现调用对其他应用程序使用。







**接口**

[MidiManager.OnDeviceOpenedListener](https://developer.android.google.cn/reference/android/media/midi/MidiManager.OnDeviceOpenedListener.html)


用于接收openDevice（MidiDeviceInfo，MidiManager.OnDeviceOpenedListener，Handler）和openBluetoothDevice（BluetoothDevice，MidiManager.OnDeviceOpenedListener，Handler）的结果的侦听器类





**类**

[MidiDevice](https://developer.android.google.cn/reference/android/media/midi/MidiDevice.html)


此类用于从MIDI设备发送和接收数据此类的实例由openDevice（MidiDeviceInfo，MidiManager.OnDeviceOpenedListener，Handler）创建。





[MidiDevice.MidiConnection](https://developer.android.google.cn/reference/android/media/midi/MidiDevice.MidiConnection.html)


该类表示一个设备的输出端口和另一个设备的输入端口之间的连接。





[MidiDeviceInfo](https://developer.android.google.cn/reference/android/media/midi/MidiDeviceInfo.html)


该课程包含描述MIDI设备的信息。





[MidiDeviceInfo.PortInfo](https://developer.android.google.cn/reference/android/media/midi/MidiDeviceInfo.PortInfo.html)


包含有关输入或输出端口的信息。





[MidiDeviceService](https://developer.android.google.cn/reference/android/media/midi/MidiDeviceService.html)


实现虚拟MIDI设备的服务。





[MidiDeviceStatus](https://developer.android.google.cn/reference/android/media/midi/MidiDeviceStatus.html)


这是描述MIDI设备端口的当前状态的不可变类。





[MidiInputPort](https://developer.android.google.cn/reference/android/media/midi/MidiInputPort.html)


该类用于将数据发送到MIDI设备上的端口





[MidiManager](https://developer.android.google.cn/reference/android/media/midi/MidiManager.html)


这个类是MIDI服务的公共应用程序界面。





[MidiManager.DeviceCallback](https://developer.android.google.cn/reference/android/media/midi/MidiManager.DeviceCallback.html)


用于客户端接收MIDI设备的Callback类添加和删除通知





[MidiOutputPort](https://developer.android.google.cn/reference/android/media/midi/MidiOutputPort.html)


此类用于从MIDI设备上的端口接收数据





[MidiReceiver](https://developer.android.google.cn/reference/android/media/midi/MidiReceiver.html)


用于向MIDI设备发送和接收数据的接口。





[MidiSender](https://developer.android.google.cn/reference/android/media/midi/MidiSender.html)


由设备提供的接口，可以将MIDI接收器连接到MIDI设备。




