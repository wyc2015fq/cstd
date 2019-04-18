# 链接：USB：主机 - 一世豁然的专栏 - CSDN博客





2017年05月18日 10:42:03[一世豁然](https://me.csdn.net/Explorer_day)阅读数：327标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









当您的Android设备处于USB主机模式时，它充当USB主机，为总线供电，并枚举连接的USB设备。 Android 3.1及更高版本支持USB主机模式。







一、API概览

在开始之前，重要的是要了解您需要处理的类。 下表描述了android.hardware.usb包中的USB主机API。



|Class|Description|
|----|----|
|`[UsbManager](https://developer.android.google.cn/reference/android/hardware/usb/UsbManager.html)`|允许您枚举和连接所连接的USB设备。|
|`[UsbDevice](https://developer.android.google.cn/reference/android/hardware/usb/UsbDevice.html)`|表示连接的USB设备，并包含访问其标识信息，接口和端点的方法。|
|`[UsbInterface](https://developer.android.google.cn/reference/android/hardware/usb/UsbInterface.html)`|表示USB设备的接口，它定义了设备的一组功能。 设备可以具有一个或多个在其上进行通信的接口。|
|`[UsbEndpoint](https://developer.android.google.cn/reference/android/hardware/usb/UsbEndpoint.html)`|表示接口端点，它是该接口的通信通道。 接口可以具有一个或多个端点，并且通常具有用于与设备进行双向通信的输入和输出端点。|
|`[UsbDeviceConnection](https://developer.android.google.cn/reference/android/hardware/usb/UsbDeviceConnection.html)`|表示与设备的连接，该设备在端点上传输数据。 此类允许您以异步方式或异步方式来回发送数据。|
|`[UsbRequest](https://developer.android.google.cn/reference/android/hardware/usb/UsbRequest.html)`|表示通过UsbDeviceConnection与设备通信的异步请求。|
|`[UsbConstants](https://developer.android.google.cn/reference/android/hardware/usb/UsbConstants.html)`|定义与Linux内核的linux / usb / ch9.h中的定义相对应的USB常量。|


在大多数情况下，您需要使用所有这些类（在与USB设备进行通信时，仅需要UsbRequest才能进行异步通信）。 一般来说，您将获得一个UsbManager来检索所需的UsbDevice。 当您拥有该设备时，您需要找到该接口的相应的UsbInterface和UsbEndpoint进行通信。 获得正确的端点后，打开UsbDeviceConnection以与USB设备进行通信。








二、Android清单要求

以下列表描述了在使用USB主机API之前需要添加到应用程序的清单文件中：


1、因为并非所有Android设备都能保证支持USB主机API，还包括一个<uses-feature>元素，此元素声明您的应用程序使用android.hardware.usb.host功能。




2、将应用程序的最小SDK设置为API Level 12或更高版本。 早期API级别上不存在USB主机API。




3、如果您希望您的应用程序被通知附加的USB设备，请在主要活动中为android.hardware.usb.action.USB_DEVICE_ATTACHED意图指定一个<intent-filter>和<meta-data>元素对。 <meta-data>元素指向一个外部XML资源文件，它声明了要检测的设备的信息。




在XML资源文件中，为要过滤的USB设备声明<usb-device>元素。 以下列表描述了<usb-device>的属性。 一般来说，如果要过滤特定设备并使用类，子类和协议，如果要过滤一组USB设备（如大容量存储设备或数码相机），请使用供应商和产品ID。 您可以指定这些属性中的任何一个或全部。 指定没有属性匹配每个USB设备，所以只有在您的应用程序需要它时：



- `vendor-id`
- `product-id`
- class
- subclass
- `protocol` (设备或借口)




将资源文件保存在res / xml /目录中。 资源文件名（不含.xml扩展名）必须与您在<meta-data>元素中指定的文件名相同。 XML资源文件的格式在下面的示例中。





一）、清单和资源文件示例

以下示例显示了示例清单及其相应的资源文件：




```
<manifest ...>
    <uses-feature android:name="android.hardware.usb.host" />
    <uses-sdk android:minSdkVersion="12" />
    ...
    <application>
        <activity ...>
            ...
            <intent-filter>
                <action android:name="android.hardware.usb.action.USB_DEVICE_ATTACHED" />
            </intent-filter>

            <meta-data android:name="android.hardware.usb.action.USB_DEVICE_ATTACHED"
                android:resource="@xml/device_filter" />
        </activity>
    </application>
</manifest>
```


在这种情况下，应将以下资源文件保存在res / xml / device_filter.xml中，并指定具有指定属性的任何USB设备都应过滤：



```
<?xml version="1.0" encoding="utf-8"?>

<resources>
    <usb-device vendor-id="1234" product-id="5678" class="255" subclass="66" protocol="1" />
</resources>
```



三、使用设备

当用户将USB设备连接到Android设备时，Android系统可以确定您的应用程序是否对连接的设备感兴趣。 如果需要，您可以设置与设备的通信。 为此，您的应用程序必须：


1、通过使用意图过滤器来发现连接的USB设备，以在用户连接USB设备时通知或通过枚举已连接的USB设备。




2、请求用户连接USB设备的权限（如果尚未获得）。




3、通过在相应的接口端点上读取和写入数据与USB设备进行通信。




一）、发现一个设备

您的应用程序可以通过使用意图过滤器在用户连接设备时通知或通过枚举已连接的USB设备来发现USB设备。 如果您希望能够让应用程序自动检测到所需的设备，则使用意图过滤器很有用。 枚举所连接的USB设备是有用的，如果您想获得所有连接的设备的列表，或者您的应用程序没有过滤的意图。





1、使用意图过滤器

要让您的应用程序发现一个特定的USB设备，您可以指定一个意图过滤器来过滤android.hardware.usb.action.USB_DEVICE_ATTACHED意图。 除了此意图过滤器，您还需要指定一个资源文件，该文件指定USB设备的属性，例如产品和供应商ID。 当用户连接与设备过滤器匹配的设备时，系统会向他们显示一个对话框，询问他们是否要启动您的应用程序。 如果用户接受，您的应用程序自动有权访问设备，直到设备断开连接。





以下示例显示如何声明intent过滤器：




```
<activity ...>
...
    <intent-filter>
        <action android:name="android.hardware.usb.action.USB_DEVICE_ATTACHED" />
    </intent-filter>

    <meta-data android:name="android.hardware.usb.action.USB_DEVICE_ATTACHED"
        android:resource="@xml/device_filter" />
</activity>
```


以下示例显示如何声明指定您感兴趣的USB设备的相应资源文件：



```
<?xml version="1.0" encoding="utf-8"?>

<resources>
    <usb-device vendor-id="1234" product-id="5678" />
</resources>
```


在您的活动中，您可以从以下意图获取代表附加设备的UsbDevice：



```java
UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
```





2、枚举设备

如果您的应用程序有兴趣检查应用程序运行时当前连接的所有USB设备，它可以枚举总线上的设备。 使用getDeviceList（）方法获取所有连接的USB设备的哈希映射。 如果要从地图获取设备，则哈希映射由USB设备的名称键入。




```java
UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
...
HashMap<String, UsbDevice> deviceList = manager.getDeviceList();
UsbDevice device = deviceList.get("deviceName");
```


如果需要，您还可以从散列图获取迭代器，并逐个处理每个设备：



```java
UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
...
HashMap<String, UsbDevice> deviceList = manager.getDeviceList();
Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
while(deviceIterator.hasNext()){
    UsbDevice device = deviceIterator.next();
    //your code
}
```



二）、获取与设备进行通信的权限

在与USB设备进行通信之前，您的应用程序必须获得用户的许可。





注意：如果您的应用程序使用意图过滤器来连接USB设备，则如果用户允许您的应用程序处理意图，则它将自动接收权限。 如果没有，您必须在连接到设备之前在应用程序中明确请求许可。





在某些情况下，显式请求许可可能是必需的，例如当您的应用程序枚举已连接的USB设备，然后要与其通信时。 在尝试与之通信之前，您必须检查访问设备的权限。 如果没有，如果用户拒绝访问设备的权限，您将收到运行时错误。





要明确获得许可，首先创建广播接收器。 该接收器侦听当您调用requestPermission（）时获得广播的意图。 对requestPermission（）的调用向用户显示一个对话框，要求用户连接设备。 以下示例代码显示如何创建广播接收器：




```java
private static final String ACTION_USB_PERMISSION =
    "com.android.example.USB_PERMISSION";
private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        if (ACTION_USB_PERMISSION.equals(action)) {
            synchronized (this) {
                UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);

                if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                    if(device != null){
                      //call method to set up device communication
                   }
                }
                else {
                    Log.d(TAG, "permission denied for device " + device);
                }
            }
        }
    }
};
```


要注册广播接收器，请在您的活动中的onCreate（）方法中添加：



```java
UsbManager mUsbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
private static final String ACTION_USB_PERMISSION =
    "com.android.example.USB_PERMISSION";
...
mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
registerReceiver(mUsbReceiver, filter);
```


要显示请求用户连接到设备的权限的对话框，请调用requestPermission（）方法：



```java
UsbDevice device;
...
mUsbManager.requestPermission(device, mPermissionIntent);
```


当用户回复对话框时，您的广播接收方收到包含EXTRA_PERMISSION_GRANTED extra的意图，这是一个表示答案的布尔值。 在连接到设备之前，请检查此值是否为true。




三）、与设备通信

与USB设备的通信可以是同步或异步的。 在任一情况下，您应该创建一个新的线程来执行所有数据传输，因此您不会阻止UI线程。 要正确设置与设备的通信，您需要获得要进行通信的设备的相应UsbInterface和UsbEndpoint，并使用UsbDeviceConnection在此端点上发送请求。 一般来说，您的代码应该：


1、检查UsbDevice对象的属性，例如产品ID，供应商ID或设备类，以确定是否要与设备进行通信。




2、当您确定要与设备通信时，请找到要用于与该接口的相应UsbEndpoint进行通信的适当的UsbInterface。 接口可以具有一个或多个端点，并且通常将具有用于双向通信的输入和输出端点。




3、找到正确的端点时，在该端点上打开一个UsbDeviceConnection。




4、使用bulkTransfer（）或controlTransfer（）方法提供要在端点上传输的数据。 您应该在另一个线程中执行此步骤，以防止阻塞主UI线程。 有关在Android中使用线程的更多信息，请参阅进程和线程。




以下代码片段是进行同步数据传输的简单方法。 您的代码应该有更多的逻辑来正确找到正确的接口和端点进行通信，并且应该在与主UI线程不同的线程中进行数据传输：




```java
private Byte[] bytes;
private static int TIMEOUT = 0;
private boolean forceClaim = true;

...

UsbInterface intf = device.getInterface(0);
UsbEndpoint endpoint = intf.getEndpoint(0);
UsbDeviceConnection connection = mUsbManager.openDevice(device);
connection.claimInterface(intf, forceClaim);
connection.bulkTransfer(endpoint, bytes, bytes.length, TIMEOUT); //do in another thread
```


要异步发送数据，请使用UsbRequest类初始化并排队异步请求，然后等待具有requestWait（）的结果。




有关详细信息，请参阅AdbTest示例，其中显示了如何进行异步批量传输以及MissileLauncher示例，该示例显示了如何异步侦听中断端点。





四）、终止与设备的通信

当您完成与设备的通信或设备分离时，通过调用releaseInterface（）和close（）关闭UsbInterface和UsbDeviceConnection。 要收听分离的事件，请创建如下广播接收器：




```java
BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();

      if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action)) {
            UsbDevice device = (UsbDevice)intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
            if (device != null) {
                // call your method that cleans up and closes communication with the device
            }
        }
    }
};
```


在应用程序中创建广播接收者，而不是清单，允许您的应用程序在运行时仅处理分离的事件。 这样，分离的事件只会发送到当前运行的应用程序，而不是广播到所有的应用程序。



