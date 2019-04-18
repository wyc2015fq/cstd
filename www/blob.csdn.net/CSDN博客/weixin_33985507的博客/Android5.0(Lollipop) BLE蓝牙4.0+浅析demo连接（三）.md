# Android5.0(Lollipop) BLE蓝牙4.0+浅析demo连接（三） - weixin_33985507的博客 - CSDN博客
2017年02月20日 14:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
作者：Bgwan
链接：https://zhuanlan.zhihu.com/p/23363591
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
Android5.0(Lollipop) BLE蓝牙4.0+浅析demo连接（三）
上面的两篇文章已经实现了蓝牙之间的搜索和蓝牙搜索的代码的分析，这里需要补充一点，在搜索的时候，我们最好是跳转到用户的setting界面去开启，关闭蓝牙的操作，因为权限的问题，这是一个关乎到用户隐私，而不仅仅是隐私，从技术角度，我们调用bluetoothadapter.enable，也可能不同的手机启动不了，造成不好的用户体验。
这篇文章主要讲解，我们搜索到蓝牙以后如何和蓝牙进行连接的问题，其实也看过一些博客，大多关于蓝牙的文章都融合到一起，我觉得这样不好，我还是喜欢分模块来讲解。
**蓝牙连接。**
The First ：
两个设备通过BLE通信，首先需要建立GATT连接。这里我们讲的是Android设备作为client端，连接GATT Server。
连接GATT Server，你需要调用BluetoothDevice的connectGatt()方法。此函数带三个参数：Context、autoConnect(boolean)和BluetoothGattCallback对象。调用示例：
这是官方的说明：
```
// We want to directly connect to the device, so we are setting the autoConnect
// parameter to false.
mBluetoothGatt = device.connectGatt(this, false, mGattCallback);
```
函数成功，返回BluetoothGatt对象，它是GATT profile的封装。通过这个对象，我们就能进行GATT Client端的相关操作。BluetoothGattCallback用于传递一些连接状态及结果。
BluetoothGatt常规用到的几个操作示例:
```
connect() ：连接远程设备。
discoverServices() : 搜索连接设备所支持的service。
disconnect()：断开与远程设备的GATT连接。
close()：关闭GATT Client端。
readCharacteristic(characteristic) ：读取指定的characteristic。
setCharacteristicNotification(characteristic, enabled) ：设置当指定characteristic值变化时，发出通知。
getServices() ：获取远程设备所支持的services。
```
The Second：
connectGatt()传递了三个参数，一个是context,还有一个是是否开启自动连接，最后是一个mGattCallback，和mLeScanCallback很像。现在来看看mGattCallback。
```
private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
@Override
    public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
        String intentAction;
if (newState == BluetoothProfile.STATE_CONNECTED) {
            intentAction = ACTION_GATT_CONNECTED;
mConnectionState = STATE_CONNECTED;
            broadcastUpdate(intentAction);
            Log.i(TAG, "Connected to GATT server.");
// Attempts to discover services after successful connection.
            Log.i(TAG, "Attempting to start service discovery:" +
mBluetoothGatt.discoverServices());
        } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
            intentAction = ACTION_GATT_DISCONNECTED;
mConnectionState = STATE_DISCONNECTED;
            Log.i(TAG, "Disconnected from GATT server.");
            broadcastUpdate(intentAction);
        }
    }
@Override
    public void onServicesDiscovered(BluetoothGatt gatt, int status) {
if (status == BluetoothGatt.GATT_SUCCESS) {
            broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED);
        } else {
            Log.w(TAG, "onServicesDiscovered received: " + status);
        }
    }
@Override
    public void onCharacteristicRead(BluetoothGatt gatt,
                                     BluetoothGattCharacteristic characteristic,
int status) {
if (status == BluetoothGatt.GATT_SUCCESS) {
            broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
        }
    }
@Override
    public void onCharacteristicChanged(BluetoothGatt gatt,
                                        BluetoothGattCharacteristic characteristic) {
        broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
    }
};
```
可以看到里面接口主要有蓝牙连接的状态改变，ServicesDiscovered方法等，从名字就知道干嘛用的，但是需要特别注意 ，某些函数调用之间存在先后关系。例如首先需要connect上才能discoverServices，还有一些函数调用是异步的，需要得到的值不会立即返回，而会在BluetoothGattCallback的回调函数中返回。例如discoverServices与onServicesDiscovered回调，readCharacteristic与onCharacteristicRead回调，setCharacteristicNotification与onCharacteristicChanged回调等。
The End : 在末尾想了一下，200多行Service代码还是贴出来，这样其实不好，文章太长了。
```
Log.i(TAG, "Connected to GATT server.");
```
这样就代表蓝牙设备之间建立了连接，（我靠这个知乎怎么打出字中间有横线的）
很多人概念蓝牙概念不是很理解的，可以移步至-，Android5.0(Lollipop) BLE蓝牙4.0+浅析Smack术语（五）
下篇文章会讲解蓝牙之间连接的设备如何传递信息（DATA）
-----** BluetoothLeService**
```
/**
 * Service for managing connection and data communication with a GATT server hosted on a
 * given Bluetooth LE device.
 */
public class BluetoothLeService extends Service {
private final static String TAG = BluetoothLeService.class.getSimpleName();
private BluetoothManager mBluetoothManager;
private BluetoothAdapter mBluetoothAdapter;
private String mBluetoothDeviceAddress;
private BluetoothGatt mBluetoothGatt;
private int mConnectionState = STATE_DISCONNECTED;
private static final int STATE_DISCONNECTED = 0;
private static final int STATE_CONNECTING = 1;
private static final int STATE_CONNECTED = 2;
public final static String ACTION_GATT_CONNECTED =
"com.example.bluetooth.le.ACTION_GATT_CONNECTED";
public final static String ACTION_GATT_DISCONNECTED =
"com.example.bluetooth.le.ACTION_GATT_DISCONNECTED";
public final static String ACTION_GATT_SERVICES_DISCOVERED =
"com.example.bluetooth.le.ACTION_GATT_SERVICES_DISCOVERED";
public final static String ACTION_DATA_AVAILABLE =
"com.example.bluetooth.le.ACTION_DATA_AVAILABLE";
public final static String EXTRA_DATA =
"com.example.bluetooth.le.EXTRA_DATA";
public final static UUID UUID_HEART_RATE_MEASUREMENT =
            UUID.fromString(SampleGattAttributes.HEART_RATE_MEASUREMENT);
// Implements callback methods for GATT events that the app cares about.  For example,
    // connection change and services discovered.
    private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
@Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            String intentAction;
if (newState == BluetoothProfile.STATE_CONNECTED) {
                intentAction = ACTION_GATT_CONNECTED;
mConnectionState = STATE_CONNECTED;
                broadcastUpdate(intentAction);
                Log.i(TAG, "Connected to GATT server.");
// Attempts to discover services after successful connection.
                Log.i(TAG, "Attempting to start service discovery:" +
mBluetoothGatt.discoverServices());
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                intentAction = ACTION_GATT_DISCONNECTED;
mConnectionState = STATE_DISCONNECTED;
                Log.i(TAG, "Disconnected from GATT server.");
                broadcastUpdate(intentAction);
            }
        }
@Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
if (status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED);
            } else {
                Log.w(TAG, "onServicesDiscovered received: " + status);
            }
        }
@Override
        public void onCharacteristicRead(BluetoothGatt gatt,
                                         BluetoothGattCharacteristic characteristic,
int status) {
if (status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
            }
        }
@Override
        public void onCharacteristicChanged(BluetoothGatt gatt,
                                            BluetoothGattCharacteristic characteristic) {
            broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
        }
    };
private void broadcastUpdate(final String action) {
final Intent intent = new Intent(action);
        sendBroadcast(intent);
    }
private void broadcastUpdate(final String action,
final BluetoothGattCharacteristic characteristic) {
final Intent intent = new Intent(action);
// This is special handling for the Heart Rate Measurement profile.  Data parsing is
        // carried out as per profile specifications:
        // http://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml
        if (UUID_HEART_RATE_MEASUREMENT.equals(characteristic.getUuid())) {
int flag = characteristic.getProperties();
int format = -1;
if ((flag & 0x01) != 0) {
                format = BluetoothGattCharacteristic.FORMAT_UINT16;
                Log.d(TAG, "Heart rate format UINT16.");
            } else {
                format = BluetoothGattCharacteristic.FORMAT_UINT8;
                Log.d(TAG, "Heart rate format UINT8.");
            }
final int heartRate = characteristic.getIntValue(format, 1);
            Log.d(TAG, String.format("Received heart rate: %d", heartRate));
            intent.putExtra(EXTRA_DATA, String.valueOf(heartRate));
        } else {
// For all other profiles, writes the data formatted in HEX.
            final byte[] data = characteristic.getValue();
if (data != null && data.length > 0) {
final StringBuilder stringBuilder = new StringBuilder(data.length);
for(byte byteChar : data)
                    stringBuilder.append(String.format("%02X ", byteChar));
                intent.putExtra(EXTRA_DATA, new String(data) + "\n" + stringBuilder.toString());
            }
        }
        sendBroadcast(intent);
    }
public class LocalBinder extends Binder {
        BluetoothLeService getService() {
return BluetoothLeService.this;
        }
    }
@Override
    public IBinder onBind(Intent intent) {
return mBinder;
    }
@Override
    public boolean onUnbind(Intent intent) {
// After using a given device, you should make sure that BluetoothGatt.close() is called
        // such that resources are cleaned up properly.  In this particular example, close() is
        // invoked when the UI is disconnected from the Service.
        close();
return super.onUnbind(intent);
    }
private final IBinder mBinder = new LocalBinder();
/**
     * Initializes a reference to the local Bluetooth adapter.
     *
     * @return Return true if the initialization is successful.
     */
    public boolean initialize() {
// For API level 18 and above, get a reference to BluetoothAdapter through
        // BluetoothManager.
        if (mBluetoothManager == null) {
mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
if (mBluetoothManager == null) {
                Log.e(TAG, "Unable to initialize BluetoothManager.");
return false;
            }
        }
mBluetoothAdapter = mBluetoothManager.getAdapter();
if (mBluetoothAdapter == null) {
            Log.e(TAG, "Unable to obtain a BluetoothAdapter.");
return false;
        }
return true;
    }
/**
     * Connects to the GATT server hosted on the Bluetooth LE device.
     *
     * @param address The device address of the destination device.
     *
     * @return Return true if the connection is initiated successfully. The connection result
     *         is reported asynchronously through the
     *         {@code BluetoothGattCallback#onConnectionStateChange(android.bluetooth.BluetoothGatt, int, int)}
     *         callback.
     */
    public boolean connect(final String address) {
if (mBluetoothAdapter == null || address == null) {
            Log.w(TAG, "BluetoothAdapter not initialized or unspecified address.");
return false;
        }
// Previously connected device.  Try to reconnect.
        if (mBluetoothDeviceAddress != null && address.equals(mBluetoothDeviceAddress)
                && mBluetoothGatt != null) {
            Log.d(TAG, "Trying to use an existing mBluetoothGatt for connection.");
if (mBluetoothGatt.connect()) {
mConnectionState = STATE_CONNECTING;
return true;
            } else {
return false;
            }
        }
final BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
if (device == null) {
            Log.w(TAG, "Device not found.  Unable to connect.");
return false;
        }
// We want to directly connect to the device, so we are setting the autoConnect
        // parameter to false.
        mBluetoothGatt = device.connectGatt(this, false, mGattCallback);
        Log.d(TAG, "Trying to create a new connection.");
mBluetoothDeviceAddress = address;
mConnectionState = STATE_CONNECTING;
return true;
    }
/**
     * Disconnects an existing connection or cancel a pending connection. The disconnection result
     * is reported asynchronously through the
     * {@code BluetoothGattCallback#onConnectionStateChange(android.bluetooth.BluetoothGatt, int, int)}
     * callback.
     */
    public void disconnect() {
if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
return;
        }
mBluetoothGatt.disconnect();
    }
/**
     * After using a given BLE device, the app must call this method to ensure resources are
     * released properly.
     */
    public void close() {
if (mBluetoothGatt == null) {
return;
        }
mBluetoothGatt.close();
mBluetoothGatt = null;
    }
/**
     * Request a read on a given {@code BluetoothGattCharacteristic}. The read result is reported
     * asynchronously through the {@code BluetoothGattCallback#onCharacteristicRead(android.bluetooth.BluetoothGatt, android.bluetooth.BluetoothGattCharacteristic, int)}
     * callback.
     *
     * @param characteristic The characteristic to read from.
     */
    public void readCharacteristic(BluetoothGattCharacteristic characteristic) {
if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
return;
        }
mBluetoothGatt.readCharacteristic(characteristic);
    }
/**
     * Enables or disables notification on a give characteristic.
     *
     * @param characteristic Characteristic to act on.
     * @param enabled If true, enable notification.  False otherwise.
     */
    public void setCharacteristicNotification(BluetoothGattCharacteristic characteristic,
boolean enabled) {
if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
return;
        }
mBluetoothGatt.setCharacteristicNotification(characteristic, enabled);
// This is specific to Heart Rate Measurement.
        if (UUID_HEART_RATE_MEASUREMENT.equals(characteristic.getUuid())) {
            BluetoothGattDescriptor descriptor = characteristic.getDescriptor(
                    UUID.fromString(SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG));
            descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
mBluetoothGatt.writeDescriptor(descriptor);
        }
    }
/**
     * Retrieves a list of supported GATT services on the connected device. This should be
     * invoked only after {@code BluetoothGatt#discoverServices()} completes successfully.
     *
     * @return A {@code List} of supported services.
     */
    public List<BluetoothGattService> getSupportedGattServices() {
if (mBluetoothGatt == null) return null;
return mBluetoothGatt.getServices();
    }
```
