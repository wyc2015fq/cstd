# 浅谈iOS中的蓝牙技术(二) -- CoreBluetooth - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月28日 10:09:22[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4293
> 
在上篇文章中我们提到了 iOS 开发中,使用GameKit 框架实现相同网络的 iOS 设备之间传输数据.但是 GameKit 框架在 iOS7 之后就过时了,那么本文将介绍另一种关于蓝牙的框架 CoreBluetooth.
## CoreBluetooth
##### 简介
- CoreBlueTooth.framework
使用 CoreBluetooth 要求设备必须要支持蓝牙4.0(一般也叫BLE<Bluetooth low energy>)。,并且手机型号必须是iPhone4以上,也就是至少4s手机.使用CoreBluetooth进行开发可实现与第三方设备的数据交互.
![](http://upload-images.jianshu.io/upload_images/1230517-806078f28cf91416.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
CoreBluetooth.h
上图是CoreBluetooth 的头文件,在这个框架中最核心的两个东西就是 Central (中心)和 Peripheral(外设).
![](http://upload-images.jianshu.io/upload_images/1230517-266b8c21db0fdaf1.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图片来自网络,如有侵权,请联系
##### CoreBluetooth 中的对象模型
- 
CBCentralManager
它用于管理发现或远程连接外围设备(CBPeripheral对象),包括扫描,发现和外围设备连接到广告.
- 
CBPeripheralManager
它用于管理服务在当地发表外围设备的通用属性配置文件数据库和通知这些服务的中心设备(CBCentral对象).
- 
CBPeripheral
它代表远程周边设备,你的应用程序的中央管理器(CBCentralManager)的一个实例——发现广告或当前连接.
- 
CBCentral
它代表远程中央设备连接到一个应用程序在本地设备上实现外围的作用.
- 
CBService 和 CBMutableService
代表外围的业务数据收集,相关行为完成一个功能或特性的设备(或部分设备).CBMutableService类继承CBService类添加写访问的所有属性.
- 
CBCharacteristic及其子类CBMutableCharacteristic
代表进一步外围的信息服务.CBMutableCharacteristic对象代表当的特色周边的服务
- 
CBUUID
它的实例代表128位全局唯一标识符(uuid)属性中使用蓝牙低能量沟通,比如周边的服务,特点,和特征描述符
- 
CBATTRequest
它代表属性协议(ATT)读写请求从远程中央设备(CBCentral对象)
## 实现通讯
### 中心管理设计模式
- First
导入 CoreBluetooth 头文件,创建中心管理者属性和外设属性,并遵守中心管理者和外设的协议.
![](http://upload-images.jianshu.io/upload_images/1230517-08b0950e45d36d17.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
First
- 建立中心控制器
懒加载创建中心控制器, 创建的时候设置代理为当前控制器,Peripheral Manager将跑在主线程中。如果你想用不同的线程做更加复杂的事情，你需创建一个队列（queue）并将它放在这儿
```
- (CBCentralManager *)getCManager{
    if (!_cManager) {
        _cManager = [[CBCentralManager alloc] initWithDelegate:self queue:dispatch_get_main_queue() options:nil];
    }
    return _cManager;
}
```
- 更新状态 (只要中心管理者初始化,就会触发此代理方法)
中心管理器的状态
![](http://upload-images.jianshu.io/upload_images/1230517-3878f3a47c6202f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
中心管理器
- 
扫描外设
```
// 该方法用于告诉Central Manager，要开始寻找一个指定的服务了.不能在state非ON的情况下对我们的中心管理者进行操作,scanForPeripheralsWithServices方法中 sercices为空则会扫描所有的设备.
- (void)centralManagerDidUpdateState:(CBCentralManager *)central{
  switch (central.state) {
      case CBCentralManagerStateUnknown:
          NSLog(@"中心管理器状态未知");
          break;
      case CBCentralManagerStateResetting:
          NSLog(@"中心管理器状态重置");
          break;
      case CBCentralManagerStateUnsupported:
          NSLog(@"中心管理器状态不被支持");
          break;
      case CBCentralManagerStateUnauthorized:
          NSLog(@"中心管理器状态未被授权");
          break;
      case CBCentralManagerStatePoweredOff:
          NSLog(@"中心管理器状态电源关闭");
          break;
      case CBCentralManagerStatePoweredOn:
      {
          NSLog(@"中心管理器状态电源开启");
          // 在中心管理者成功开启后开始搜索外设
          [self.cManager scanForPeripheralsWithServices:nil options:nil]; 
          // 搜索成功之后,会调用我们找到外设的代理方法 sercices为空则会扫描所有的设备
          // - (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI; //找到外设
      }
          break;
      default:
          break;
  }
}
```
- 
过滤外设,进行连接
如果找到了设备,则代理会调用该方法,过滤外设
```
/*
 *  @param central              中央管理器提供此更新
 *  @param peripheral           一个外设对象
 *  @param advertisementData    一个包含任何广播和扫描响应数据的字典。
 *  @param RSSI                 RSSI（Received Signal Strength Indicator）是接收信号的强度指示
 *
 */
- (void)centralManager:(CBCentralManager *)central didDiscoverPeripheral:(CBPeripheral *)peripheral advertisementData:(NSDictionary *)advertisementData RSSI:(NSNumber *)RSSI{
    if ([peripheral.name hasPrefix:@"XXX"] && (ABS(RSSI.integerValue) > 35)) {
        // 标记我们的外设,延长他的生命周期
        self.peripheral = peripheral;
        // 进行连接
        [self.cManager connectPeripheral:self.peripheral options:nil];
    }
}
```
- 连接状态(成功,失败,断开)
中心管理者连接外设成功,连接成功之后,可以进行服务和特征的发现
```
- (void)centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral{
    NSLog(@"%@连接成功",peripheral.name);
    // 获取外设的服务
    // 设置外设的代理
    self.peripheral.delegate = self;
    // 外设发现服务,传nil代表不过滤
    // 这里会触发外设的代理方法 - (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
    [self.peripheral discoverServices:nil];
}
// 外设连接失败
- (void)centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSLog(@"%@连接失败",peripheral.name);
}
// 丢失连接
- (void)centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error
{
    NSLog(@"%@断开连接",peripheral.name);
}
```
- 发现服务以及内部的特征
```
// 发现外设的服务后调用的方法
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverServices:(NSError *)error
{
    // 判断没有失败
    if (error) {
        NSLog(@"error:%@",error.localizedDescription);
        return;
    }
    for (CBService *service in peripheral.services) {
        [peripheral discoverCharacteristics:nil forService:service];
    }
}
// 发现服务后,让设备再发现服务内部的特征
- (void)peripheral:(CBPeripheral *)peripheral didDiscoverCharacteristicsForService:(CBService *)service error:(NSError *)error
{
    // 遍历特征
    for (CBCharacteristic *characteristic in service.characteristics) {
        // 获取特征对应的描述
        [peripheral discoverDescriptorsForCharacteristic:characteristic];
        // 获取特征的值
        [peripheral readValueForCharacteristic:characteristic];
    }
}
```
- 更新特征
```
// 更新特征的描述的值的时候会调用
- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForDescriptor:(CBDescriptor *)descriptor error:(NSError *)error
{
    [peripheral readValueForDescriptor:descriptor];
}
// 更新特征的value的时候会调用
- (void)peripheral:(CBPeripheral *)peripheral didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic error:(NSError *)error
{
    for (CBDescriptor *descriptor in characteristic.descriptors) {
        [peripheral readValueForDescriptor:descriptor];
    }
}
```
- 外设写数据到特征中
NS_OPTIONS类型的枚举特征属性,可以是多个值,这里需要***注意***,由于枚举属性是NS_OPTIONS,所以一个枚举可能对应多个类型,所以在判断的时候不能用 = ,而应该用包含&
![](http://upload-images.jianshu.io/upload_images/1230517-2483d5a63f66a848.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
NS_OPTIONS类型的特征属性
```
- (void)peripheral:(CBPeripheral *)peripheral didWriteData:(NSData *)data forCharacteristic:(nonnull CBCharacteristic *)characteristic
{
    // 
    if (characteristic.properties & CBCharacteristicPropertyWrite) {
    // 下面方法中参数的意义依次是:写入的数据 写给哪个特征 通过此响应记录是否成功写入
        [peripheral writeValue:data forCharacteristic:characteristic type:CBCharacteristicWriteWithResponse];
    }
}
```
- 通知的订阅和取消订阅
一般来说这两个方法要根据产品需求来确定写在什么地方
```
- (void)peripheral:(CBPeripheral *)peripheral regNotifyWithCharacteristic:(nonnull CBCharacteristic *)characteristic
{
    // 外设为特征订阅通知
    [peripheral setNotifyValue:YES forCharacteristic:characteristic];
}
- (void)peripheral:(CBPeripheral *)peripheral CancleRegNotifyWithCharacteristic:(nonnull CBCharacteristic *)characteristic
{
    // 外设取消订阅通知
    [peripheral setNotifyValue:NO forCharacteristic:characteristic];
}
```
- 断开连接
```
- (void)dismissConentedWithPeripheral:(CBPeripheral *)peripheral
{
    // 停止扫描
    [self.cManager stopScan];
    // 断开连接
    [self.cManager cancelPeripheralConnection:peripheral];
}
```
## END
外设管理设计模式,跟中心管理设计模式类似,就不赘述,需要遵守 外设管理器协议 和中心协议,设置代理,遵守其中的方法.代码地址:[https://github.com/coderqiao/CoreBluetooth](https://github.com/coderqiao/CoreBluetooth).
文／CoderQiao（简书作者）
原文链接：http://www.jianshu.com/p/bad5a30e38d4
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
