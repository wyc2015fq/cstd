# RN交互iOS - weixin_33985507的博客 - CSDN博客
2019年02月28日 02:57:19[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
RN与原生iOS交互有传值 通知
第一种RN传值iOS
```
原生代码
RCT_EXPORT_MODULE() 导出模块
RCT_EXPORT_METHOD(RNOpenVC:(NSString *)msg){} 导出函数名以及参数
import{NativeModules}
const xxx = NativeModules.'模块名称';
xx.RNOpenVC(('xxxx'));
复制代码
```
第二种
```
通知
原生代码
原生类文件继承：RCTEventEmitter<RCTBridgeModule>
RCT_EXPORT_MODULE(); //导出模块
//添加通知名称
- (NSArray<NSString *> *)supportedEvents {
  return @[@"locationCenterEvent",
           @"decompileAddressEvent",
           @"onClickDetailPage",
           @"onPressMarkDetailinfo",
           @"onTagFuncationIndex",
           @"onPressPrivateChat",
           @"onPressChildPageDetailinfo",
           @"chlidPagelocation",
           @"onPressHeadPersonalPageClick",
           @"onPressMapSearchClick"]; //这里返回的将是你要发送的消息名的数组。
}
//添加监听通知方法（注意： 函数名字不可随意改变）
- (void)startObserving{
    //点击进入搜索
    [[NSNotificationCenter defaultCenter] addObserver:self
    selector:@selector(onPressMapSearchClick:)
    name:@"onPressMapSearchClick"
    object:nil];
}
//实现通知方法
- (void)onPressMapSearchClick:(NSNotification *)notify{
    [self sendEventWithName:@"onPressMapSearchClick"  body:@{ @"code": @"1",
    @"result": notify.object, }];
}
//停止监听通知（移除通知）
- (void)stopObserving{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
RN代码
复制代码
```
import {  NativeModules,
NativeEventEmitter,}
//调用通知模块
const tagEventEmitter = NativeModules.tagEventEmitter;
const tagEventEmitters = new NativeEventEmitter(tagEventEmitter);
```
监听方法
 //搜索跳转
  this.onPressMapSearchClick = tagEventEmitters.addListener('onPressMapSearchClick', (obj) => {
    this.props.navigation.navigate('Search');
  });
  
  //移除通知
 componentWillUnmount() {
    this.onPressMapSearchClick.remove();
}
以上便是项目种使用到的RN与原生交互方式，其他交互方式敬请期待。。。。。复制代码
```
