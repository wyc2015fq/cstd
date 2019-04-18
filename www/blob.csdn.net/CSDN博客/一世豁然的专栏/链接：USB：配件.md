# 链接：USB：配件 - 一世豁然的专栏 - CSDN博客





2017年05月18日 09:50:23[一世豁然](https://me.csdn.net/Explorer_day)阅读数：393








USB附件模式允许用户连接专为Android设备设计的USB主机硬件。 配件必须遵守Android配件开发套件文档中概述的Android附件协议。 这使得无法充当USB主机的Android电源设备仍然可以与USB硬件交互。 当Android设备处于USB附件模式时，所附的Android USB附件充当主机，为USB总线供电，并列举连接的设备。 Android 3.1（API级别12）支持USB附件模式，并且该功能也被返回到Android 2.3.4（API级别10），以支持更广泛的设备。







一、选择正确的USB附件API

虽然USB附件API已经在Android 3.1平台上引入，但Android 2.3.4中也可以使用Google API附加库。 因为这些API是使用外部库进行反向输入的，所以有两个包可以导入以支持USB附件模式。 根据您要支持的Android驱动的设备，您可能需要使用其他设备：


1、`com.android.future.usb`: 为了在Android 2.3.4中支持USB附件模式，Google API附加库包括后端的USB附件API，它们包含在此命名空间中。 Android 3.1还支持在此命名空间中导入和调用类，以支持使用附加库编写的应用程序。 这个附加库是围绕android.hardware.usb附件API的薄包装，不支持USB主机模式。
 如果要支持最广泛的支持USB附件模式的设备，请使用附加库并导入此软件包。 重要的是要注意，并不是所有的Android 2.3.4设备都需要支持USB附件功能。 每个设备制造商决定是否支持此功能，这就是为什么必须在清单文件中声明它。




2、`[android.hardware.usb](https://developer.android.google.cn/reference/android/hardware/usb/package-summary.html)`: 此命名空间包含在Android
 3.1中支持USB附件模式的类。 该软件包作为框架API的一部分，所以Android 3.1支持USB附件模式而不使用附加库。 如果您只关心具有USB附件模式硬件支持的Android 3.1或更新的设备，您可以在清单文件中声明，请使用此软件包。




一）、安装Google API附加库

如果要安装该附加组件，可以通过使用SDK Manager安装Google API Android API 10软件包。 有关安装附加库的更多信息，请参阅安装Google API插件。








二、API概览

因为附加库是框架API的包装器，所以与支持USB附件功能的类似。 即使您正在使用附加库，也可以使用android.hardware.usb的参考文档。





注意：您应该注意的附加库和框架API之间存在较小的使用差异。





下表介绍支持USB附件API的类：



|Class|Description|
|----|----|
|`[UsbManager](https://developer.android.google.cn/reference/android/hardware/usb/UsbManager.html)`|允许您与所连接的USB附件进行枚举和交流。|
|`[UsbAccessory](https://developer.android.google.cn/reference/android/hardware/usb/UsbAccessory.html)`|表示USB附件，包含访问其识别信息的方法。|


一）、附加库和平台API之间的使用差异

使用Google API附加库和平台API之间有两种使用差异。





如果您正在使用附加库，则必须以下列方式获取UsbManager对象：




```java
UsbManager manager = UsbManager.getInstance(this);
```


如果您不使用附加库，则必须以下列方式获取UsbManager对象：



```java
UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
```


当您使用意图过滤器过滤连接的附件时，UsbAccessory对象包含在传递给应用程序的意图内。 如果您正在使用附加库，则必须以以下方式获取UsbAccessory对象：



```java
UsbAccessory accessory = UsbManager.getAccessory(intent);
```


如果您不使用附加库，则必须以以下方式获取UsbAccessory对象：



```java
UsbAccessory accessory = (UsbAccessory) intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
```






三、Android清单要求

以下列表描述了在使用USB辅助API之前需要添加到应用程序的清单文件中。 清单和资源文件示例显示如何声明这些项目：


1、因为并非所有Android设备都能保证支持USB附件API，还包括一个<uses-feature>元素，此元素声明您的应用程序使用android.hardware.usb.accessory功能。




2、如果您正在使用附加库，请添加指定库的com.android.future.usb.accessory的<uses-library>元素。




3、如果您正在使用附加库，请将应用程序的最小SDK设置为API Level 10，如果使用的是android.hardware.usb包，则将其设置为12。




4、如果希望您的应用程序被通知附加的USB附件，请在主要活动中为android.hardware.usb.action.USB_ACCESSORY_ATTACHED意图指定<intent-filter>和<meta-data>元素对。 <meta-data>元素指向一个外部XML资源文件，它声明要检测的附件的信息。

在XML资源文件中，为要过滤的附件声明<usb-accessory>元素。 每个<usb-accessory>可以具有以下属性：

 1）、manufacturer

 2）、model

 3）、version




将资源文件保存在res / xml /目录中。 资源文件名（不含.xml扩展名）必须与您在<meta-data>元素中指定的文件名相同。 XML资源文件的格式也在下面的示例中显示。





一）、清单和资源文件示例

以下示例显示了示例清单及其相应的资源文件：




```
<manifest ...>
    <uses-feature android:name="android.hardware.usb.accessory" />
    
    <uses-sdk android:minSdkVersion="<version>" />
    ...
    <application>
      <uses-library android:name="com.android.future.usb.accessory" />
        <activity ...>
            ...
            <intent-filter>
                <action android:name="android.hardware.usb.action.USB_ACCESSORY_ATTACHED" />
            </intent-filter>

            <meta-data android:name="android.hardware.usb.action.USB_ACCESSORY_ATTACHED"
                android:resource="@xml/accessory_filter" />
        </activity>
    </application>
</manifest>
```


在这种情况下，应将以下资源文件保存在res / xml / accessory_filter.xml中，并指定具有相应型号，制造商和版本的任何附件应被过滤。 配件将这些属性发送给Android设备：



```
<?xml version="1.0" encoding="utf-8"?>

<resources>
    <usb-accessory model="DemoKit" manufacturer="Google" version="1.0"/>
</resources>
```






四、使用配件

当用户将USB附件连接到Android设备时，Android系统可以确定您的应用程序是否对连接的附件感兴趣。 如果是这样，需要的话，您可以设置与附件的通信。 为此，您的应用程序必须：


1、通过使用过滤附件事件的意图过滤器或枚举连接的附件并找到适当的附件来发现连接的附件。




2、要求用户与附件通信，如果尚未获得。




3、通过在适当的接口端点上读取和写入数据来与附件通信。




一）、发现配件

您的应用程序可以通过使用意图过滤器来发现附件，以便在用户连接附件或枚举已连接的附件时收到通知。 如果希望能够让应用程序自动检测到所需的附件，则使用意图过滤器很有用。 如果您想获得所有连接的配件的列表，或者您的应用程序没有为意图过滤，枚举连接的附件是有用的。





1、使用意图过滤器

要使您的应用程序发现一个特定的USB附件，您可以指定一个意图过滤器来过滤android.hardware.usb.action.USB_ACCESSORY_ATTACHED意图。 除了该意图过滤器之外，还需要指定一个资源文件，该文件指定USB配件的属性，如制造商，型号和版本。 当用户连接与您的附件过滤器相匹配的附件时，





以下示例显示如何声明intent过滤器：




```
<activity ...>
    ...
    <intent-filter>
        <action android:name="android.hardware.usb.action.USB_ACCESSORY_ATTACHED" />
    </intent-filter>

    <meta-data android:name="android.hardware.usb.action.USB_ACCESSORY_ATTACHED"
        android:resource="@xml/accessory_filter" />
</activity>
```


以下示例显示如何声明指定您感兴趣的USB附件的相应资源文件：



```
<?xml version="1.0" encoding="utf-8"?>

<resources>
    <usb-accessory manufacturer="Google, Inc." model="DemoKit" version="1.0" />
</resources>
```


在您的活动中，您可以从这样的意图（使用附加库）获取代表附件的UsbAccessory：



```java
UsbAccessory accessory = UsbManager.getAccessory(intent);
```


或者像这样（使用平台API）：



```java
UsbAccessory accessory = (UsbAccessory)intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
```


2、枚举配件

您的应用程序可以枚举在应用程序运行时识别自己的附件。





使用getAccessoryList（）方法获取所有连接的USB附件的阵列：




```java
UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
UsbAccessory[] accessoryList = manager.getAcccessoryList();
```


注意：一次只支持一个连接的附件。




二）、获取与附件通信的权限

在与USB配件进行通信之前，您的应用程序必须获得用户的许可。





注意：如果您的应用程序使用意图过滤器在附件中发现附件，则如果用户允许您的应用程序处理意图，则它会自动接收权限。 如果没有，您必须在连接到附件之前在应用程序中明确请求许可。





在某些情况下，明确请求许可可能是必需的，例如当您的应用程序枚举已连接的配件，然后要与其进行通信时。 在尝试与之通信之前，您必须检查访问附件的权限。 如果不是，如果用户拒绝访问附件的权限，您将收到运行时错误。





要明确获得许可，首先创建广播接收器。 该接收器侦听当您调用requestPermission（）时获得广播的意图。 对requestPermission（）的调用向用户显示一个对话框，要求连接到附件的权限。 以下示例代码显示如何创建广播接收器：




```java
private static final String ACTION_USB_PERMISSION =
    "com.android.example.USB_PERMISSION";
private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        if (ACTION_USB_PERMISSION.equals(action)) {
            synchronized (this) {
                UsbAccessory accessory = (UsbAccessory) intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);

                if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                    if(accessory != null){
                        //call method to set up accessory communication
                    }
                }
                else {
                    Log.d(TAG, "permission denied for accessory " + accessory);
                }
            }
        }
    }
};
```


要注册广播接收器，请将其放在您的活动中的onCreate（）方法中：



```java
UsbManager mUsbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
private static final String ACTION_USB_PERMISSION =
    "com.android.example.USB_PERMISSION";
...
mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
registerReceiver(mUsbReceiver, filter);
```


要显示要求用户连接附件的权限的对话框，请调用requestPermission（）方法：



```java
UsbAccessory accessory;
...
mUsbManager.requestPermission(accessory, mPermissionIntent);
```


当用户回复对话框时，您的广播接收方收到包含EXTRA_PERMISSION_GRANTED extra的意图，这是一个表示答案的布尔值。 在连接附件之前，请检查这个额外的值是否为true。




三）、与配件通讯

您可以通过使用UsbManager与附件通信，以获取一个文件描述符，您可以设置输入和输出流来读写数据到描述符。 流代表配件的输入和输出批量端点。 您应该在另一个线程中设置设备和附件之间的通信，因此您不会锁定主UI线程。 以下示例显示如何打开附件进行通信：




```java
UsbAccessory mAccessory;
ParcelFileDescriptor mFileDescriptor;
FileInputStream mInputStream;
FileOutputStream mOutputStream;

...

private void openAccessory() {
    Log.d(TAG, "openAccessory: " + accessory);
    mFileDescriptor = mUsbManager.openAccessory(mAccessory);
    if (mFileDescriptor != null) {
        FileDescriptor fd = mFileDescriptor.getFileDescriptor();
        mInputStream = new FileInputStream(fd);
        mOutputStream = new FileOutputStream(fd);
        Thread thread = new Thread(null, this, "AccessoryThread");
        thread.start();
    }
}
```


在线程的run（）方法中，您可以使用FileInputStream或FileOutputStream对象来读写附件。 使用FileInputStream对象从附件读取数据时，请确保您使用的缓冲区足够大以存储USB数据包数据。 Android附件协议支持高达16384字节的数据包缓冲区，因此您可以选择始终声明缓冲区为了简化大小。




注意：在较低的级别，USB全速附件的数据包为64字节，USB高速附件为512字节。 为了简单起见，Android附件协议将两个速度的数据包捆绑在一起成为一个逻辑数据包。





有关在Android中使用线程的更多信息，请参阅进程和线程。





四）、终止与附件的通信

当您完成与附件的通信或附件脱离后，通过调用close（）关闭您打开的文件描述符。 要收听分离的事件，请创建如下广播接收器：




```java
BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();

        if (UsbManager.ACTION_USB_ACCESSORY_DETACHED.equals(action)) {
            UsbAccessory accessory = (UsbAccessory)intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
            if (accessory != null) {
                // call your method that cleans up and closes communication with the accessory
            }
        }
    }
};
```


在应用程序中而不是清单中创建广播接收者，允许您的应用程序在运行时仅处理分离的事件。 这样，分离的事件只会发送到当前运行的应用程序，而不是广播到所有的应用程序。



