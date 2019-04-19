# iOS 蓝牙使用 - xqhrs232的专栏 - CSDN博客
2018年12月05日 14:51:21[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：25
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/shihuboke/article/details/75212358](https://blog.csdn.net/shihuboke/article/details/75212358)
蓝牙技术，很早以前就被有了，如今已更新4.0版本。很多热门技术都是基于它工作的，如Android平台的NFC，iOS的iBeancon等，现在的[智能](http://lib.csdn.net/base/aiplanning)家居基本也是基于蓝牙4.0与APP进行通信。在[iOS](http://lib.csdn.net/base/ios)中，蓝牙是基于4.0标准的，设备间低功耗通信。
**核心成员**
在开始前我们回忆下传统的Socket编程，里面有Server服务端与Client端的区别。那么在蓝牙编程也是如此，其中Peripheral外设相当于Socket编程中的Server服务端，Central中心相当于Client客户端(ps吐槽下，Central中心，作为服务端，不更适合吗！)
![](https://img-blog.csdn.net/20170716214311715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hpaHVib2tl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
你可以理解外设是一个广播数据的设备，它开始告诉外面的世界说它这儿有一些数据，并且能提供一些服务。另一边中心开始扫描周边有没有合适的设备，如果发现后，会和外设做连接请求，一旦连接确定后，两个设备就可以传输数据了。
在iOS6之后，[ios](http://lib.csdn.net/base/ios) 设备可以是外设，也可以是中心，就像Socket编程中一样，你可以是服务端也可以是客户端，但与Socket不同的是不能在同时间扮演两个角色。
服务(service)和特征(characteristic)     
每个蓝牙4.0的设备都是通过服务和特征来展示自己的，一个设备必然包含一个或多个服务，每个服务下面又包含若干个特征。特征是与外界交互的最小单位。比如说，智能音响设备，用服务A标识播放模块，特征A1来表示播放上一首，特征A2来表示播放下一首；服务B标识设置模块，特征B1设置彩灯颜色。这样做的目的主要为了模块化。
上面说了设备可以是外设，也可以是中心，也就是 本地中心->远程外设  、本地外设与远程中心，
不过在智能家居开发中，大部分硬件蓝牙都是担任外设的角色，也就是说我们应用只要扮演中心即可了。下面我们一一到来：
本地中心与远程外设
在中心这边，由CBCentralManager对象管理本地中心，来发现或连接远程外设。此时
正在连接的外设用CBPeripheral 对象表示。
![](https://img-blog.csdn.net/20170716214345884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hpaHVib2tl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
远程外设的数据由CBService和CBCharacteristic对象表示
当你与远程外设CBPeripheral对象进行数据交互时，是由一个服务与特征操作的。
![](https://img-blog.csdn.net/20170716214405005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hpaHVib2tl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
本地外设与远程中心
外设方面，由本地的外设CBPeripheralManager对象表示，同样，正在连接中的远程中心用CBCentral表示
![](https://img-blog.csdn.net/20170716214427876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hpaHVib2tl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当你设置好和本地Peripheral(表示为[CBPeripheralManager](https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/CBPeripheralManager_Class/Reference/CBPeripheralManager.html%23//apple_ref/occ/cl/CBPeripheralManager))数据交互，就可以处理Service和Characteristic的可变版本。在Core Bluetooth框架中，本地Peripheral的Service表示为[CBMutableService](https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/CBMutableService_Class/CBMutableService/CBMutableService.html%23//apple_ref/occ/cl/CBMutableService)。同样地，本地Peripheral中Service的Characteristic表示为[CBMutableCharacteristic](https://developer.apple.com/library/ios/documentation/CoreBluetooth/Reference/CBMutableCharacteristic_Class/Reference/CBMutableCharacteristic.html%23//apple_ref/occ/cl/CBMutableCharacteristic)。如下图所示：
![](https://img-blog.csdn.net/20170716214456139?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2hpaHVib2tl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
使用
我们在这里只介绍本地中心与远程外设，如果你需要自己创建外设，请前往[创建外设](https://developer.apple.com/library/ios/documentation/NetworkingInternetWeb/Conceptual/CoreBluetooth_concepts/PerformingCommonPeripheralRoleTasks/PerformingCommonPeripheralRoleTasks.html%23//apple_ref/doc/uid/TP40013257-CH4-SW1)。
编程步骤：建立中心角色—扫描外设（discover）—连接外设(connect)—扫描外设中的服务和特征(discover)—与外设做数据交互(exploreand interact)—断开连接(disconnect)。
1、建立中心角色
_manager = [[CBCentralManageralloc]initWithDelegate:selfqueue:nil];
需要实现CBCentralManagerDelegate，queue为执行队列，默认为主线程
之后会回调，返回当前设备蓝牙状态
-(void)centralManagerDidUpdateState:(CBCentralManager *)central
{
    switch (central.state){
        case CBCentralManagerStatePoweredOn:
          NSLog(@"蓝牙已打开,请扫描外设");
            [selfscan];//扫描
           break;
        case CBCentralManagerStateUnsupported:
            [AlertUtilsshowAlert:@"您的设备不支持蓝牙或蓝牙4.0"];
           break;
        case CBCentralManagerStateUnauthorized:
          NSLog(@"未授权");
           break;
        caseCBCentralManagerStatePoweredOff://蓝牙未打开，系统会自动提示打开，所以不用自行提示
       default:
           break;
    }
}
2、扫描外设(discover)
//扫描
-(void)scan{
    NSLog(@"开始扫描。。");
    [_managerscanForPeripheralsWithServices:niloptions:@{CBCentralManagerScanOptionAllowDuplicatesKey:@NO}];
    //30秒以后停止
    double delayInSeconds = 30.0;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds*NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        [_managerstopScan];//停止扫描
       NSLog(@"停止扫描。。");
    });
}
使用scanForPeripheralsWithServices:options:进行扫描，第一个参数为Services的UUID(外设端的UUID)，如果你只想发现周边特定的设备(自家的音响)，那么就可以通过设备的UUID来限制。第二参数的CBCentralManagerScanOptionAllowDuplicatesKey为已发现的设备是否重复扫描，如果是同一设备会多次回调。
在开始扫描后，我们启动了一个延时队列，用于30秒后停止扫描，要知道扫描是件非常耗电的操作，因此你的应用不应该实时扫描周边设备。
发现外设，回调
//发现设备
-(void)centralManager:(CBCentralManager*)central didDiscoverPeripheral:(CBPeripheral*)peripheral advertisementData:(NSDictionary*)advertisementData RSSI:(NSNumber *)RSSI{
    if (![_peripheralscontainsObject:peripheral]) {
       [_peripheralsaddObject:peripheral];
       NSLog(@"发现设备:%@",_peripherals);
       [_tViewreloadData];
    }
}
3、连接外设
//连接设备
-(BOOL)connect:(CBPeripheral*)peripheral{
    NSLog(@"connectstart");
    _currentPeripheral = nil;
    [_managerconnectPeripheral:peripheraloptions:[NSDictionarydictionaryWithObject:[NSNumbernumberWithBool:YES]forKey:CBConnectPeripheralOptionNotifyOnDisconnectionKey]];
    //开一个定时器监控连接超时的情况
   connectTimer = [NSTimer scheduledTimerWithTimeInterval:5.0ftarget:selfselector:@selector(connectTimeout:) userInfo:peripheral repeats:NO];
    return YES;
}
连接成功回调
//连接外设成功
- (void)centralManager:(CBCentralManager*)central didConnectPeripheral:(CBPeripheral*)peripheral
{
    //[connectTimerinvalidate];//停止时钟
    NSLog(@"Did connect toperipheral: %@", peripheral);
    _currentPeripheral = peripheral;
   [peripheral setDelegate:self];
    [peripheraldiscoverServices:nil];//发现服务
}
//连接外设失败
-(void)centralManager:(CBCentralManager*)central didFailToConnectPeripheral:(CBPeripheral*)peripheral error:(NSError *)error
{
    NSLog(@"%@",error);
}
4、发现服务与特征
//已发现服务
-(void) peripheral:(CBPeripheral*)peripheral didDiscoverServices:(NSError*)error{
    for (CBService*s in peripheral.services){
       [_serversaddObject:s];
    }
    for (CBService*s in peripheral.services){
       NSLog(@"服务 UUID: %@(%@)",s.UUID.data,s.UUID);
        [peripheral discoverCharacteristics:nil forService:s];
    }
}
//已搜索到Characteristics
-(void) peripheral:(CBPeripheral*)peripheral didDiscoverCharacteristicsForService:(CBService*)service error:(NSError*)error{
   NSLog(@"发现特征的服务:%@ (%@)",service.UUID.data ,service.UUID);
    for (CBCharacteristic *c in service.characteristics) {
        NSLog(@"特征UUID: %@ (%@)",c.UUID.data,c.UUID);
//        if ([c.UUID isEqual:[CBUUIDUUIDWithString:@"2A06"]]) {//设置
//           _writeCharacteristic = c;
//       }
//        if ([c.UUID isEqual:[CBUUIDUUIDWithString:@"2A19"]]) {//读取
//           [_peripheral readValueForCharacteristic:c];
//       }
//       
//       if ([c.UUID isEqual:[CBUUIDUUIDWithString:@"FFA1"]]) {
//           [_peripheral readRSSI];
//       }
//       [_nCharacteristics addObject:c];
    }
}
5、数据交互，分为两种，一种是直接读，另外一种订阅
6、断开连接
//主动断开设备
-(void)disConnect
{
    if (_currentPeripheral != nil)
    {
        NSLog(@"disConnectstart");
        [_managercancelPeripheralConnection:_currentPeripheral];
    }
}
至此整个流程就跑完啦！
**谢谢!!!**
