# 浅谈iOS中的蓝牙技术(一) --GameKit.framework - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月28日 10:08:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：642
> 
蓝牙低能耗(BLE)技术是低成本、短距离、可互操作的无线技术，工作在免许可的2.4GHz ISM射频频段.
## First
- GameKit.framework
只能用于**iOS设备之间**的**同个应用内**连接,多用于游戏中,但是从iOS7之后就过期了,最好别利用蓝牙发送比较大的数据.
## Second
我们通过GameKit框架来演示建立连接传输数据(从手机的相册中传输一张照片)的过程.
- 搭建UI
我们不再演示搭建UI的过程,总之界面如下:
![](http://upload-images.jianshu.io/upload_images/1230517-3a59797ce599ecc8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
UI
使用,storyboard搭建UI,其中建立连接和发送数据是两个button,并且脱出响应事件,绿色的部分是UIImageView,并且设置其UserInteractionEnable为YES,并且添加Tap手势,拖出Tap的响应事件.
- UI的功能
Tap手势点击UIImageView进入相册选择照片并且显示在UIImageView上,
建立连接按钮 开始查找周围的设备,并且建立连接
发送数据按钮 将图片发送至连接到的设备上
## Code
#### 点击UIImageView选择图片
在tap手势的点击事件中:
```
- (IBAction)tap:(UITapGestureRecognizer *)sender {
    // 判断是否存在相册,不存在直接return
    if (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) {
        return;
    }
    // 创建控制器
    UIImagePickerController *ipc = [[UIImagePickerController alloc]init];
    // 设置图片来源为相册
    ipc.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    // 设置代理
    ipc.delegate = self;
    // 模态出控制器
    [self presentViewController:ipc animated:YES completion:nil];
}
// UIImagePickerController代理方法
// 选择完照片之后调用
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<NSString *,id> *)info{
    [picker dismissViewControllerAnimated:YES completion:nil];
// 将选择的图片显示在UIImageView上
    self.iamgeView.image = info[UIImagePickerControllerOriginalImage];
}
```
#### 建立连接
```
// 建立连接
- (IBAction)connection:(UIButton *)sender {
    GKPeerPickerController *ppc = [[GKPeerPickerController alloc] init];
    // 设置代理监听连接成功的方法
    ppc.delegate = self;
    [ppc show];
}
```
当我们点击 **建立连接**按钮的时候弹出GKPeerPickerController控制器,来查找周围的设备.我们从字面上理解peer就是
**窥视,盯着看** 的意思,就是说,在检测周围可连接的设备.并且我们为该控制器设置代理方法,注意遵循
GKPeerPickerControllerDelegate协议,用来监听连接是否成功,以便进行接下来的操作.
#### 发送数据
```
// 发送数据
- (IBAction)send:(UIButton *)sender {
// 如果UIImageView上没有图片,那么不发送直接return
    if (self.iamgeView.image == nil) {
        return;
    }
    /**
       * 回话调用传输数据的方法向所有连接成功的设备发送数据
       * 在Iphone上有两种读取图片数据的简单方法: UIImageJPEGRepresentation和UIImagePNGRepresentation. 这里选择其一,其中前一个在处理图片的时候是可以进行压缩的
        *DataMode中有两个元素,GKSendDataReliable和GKSendDataUnreliable,前者是可靠传输,如果出现网络拥塞,会分片和重组这个数据消息,后者不能保证数据的完整性
        */
    [self.session sendDataToAllPeers:UIImagePNGRepresentation(self.iamgeView.image) withDataMode:GKSendDataReliable error:nil];
}
```
#### 接收数据
![](http://upload-images.jianshu.io/upload_images/1230517-192bac1e104cff2d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
GKPeerPickerControllerDelegate方法
我们可以看到上图是GKPeerPickerControllerDelegate中的方法.
- 第一个方法的作用是通知代理,由用户选择连接类型
- 第二个方法的作用是通知代理,连接类型请求GKSession对象
注: 这里返回一个GKSession对象给peer使用。如果这个方法不执行或返回nil,那么就会创建一个默认GKSession代理的代表。
- 第三个方法的作用是通知代理,已经完成连接,连接到了一个GKSession
- 第四个方法的作用是通知委托用户取消了picker.
在这里我们主要用到的是第三个方法,已经成功建立了连接.连接建立完成之后我们将GKPeerPickerController给取消掉调用dismiss方法.然后由于我们要传输这个数据,所以我们需要保留回话,而会话就是方法参数中的session,保留session,我们创建一个GKSession的属性,然后将参数中的session赋值给属性session就可以保证在连接和传输的整个过程中使用了.
`@property (strong, nonatomic)GKSession *session;`
```
- (void)peerPickerController:(GKPeerPickerController *)picker didConnectPeer:(NSString *)peerID toSession:(GKSession *)session {
    // 撤销picker这个控制器
    [picker dismiss];
    // 保留会话
    self.session = session;
    // 由当前控制器处理数据,蓝牙设备接收到数据时,就会调用 [self receiveData:fromPeer:inSession:context:]]  上下文context设置为nil即可.
    [self.session setDataReceiveHandler:self
                            withContext:nil];
}
```
#### 接收到数据之后
```
// 蓝牙设备接收到数据时,就会调用该方法
/**
   *data表示接收到的数据
   *peer表示传输数据的设备
   *session表示这个连接的整个会话
   *context表示上下文
   */
- (void)receiveData:(NSData *)data fromPeer:(NSString *)peer inSession:(GKSession *)session context:(void *)context
{
    // 显示
    self.iamgeView.image = [UIImage imageWithData:data];
    // 写入相册
    UIImageWriteToSavedPhotosAlbum(self.iamgeView.image, nil, nil, nil);
    // 弹窗提示来自哪个设备
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:peer message:peer preferredStyle:UIAlertControllerStyleAlert];
    [self presentViewController:alert animated:YES completion:nil];
}
```
## END
![](http://upload-images.jianshu.io/upload_images/1230517-29b48ab4c8564600.gif?imageMogr2/auto-orient/strip)
演示
由于蓝牙需要使用真机测试,所以模拟器上的演示只能至此,如果有兴趣,可以下载 Demo [https://github.com/coderqiao/GameKit--](https://github.com/coderqiao/GameKit--) ,在两台真机之间传输测试.
本文学习自小码哥大神班的视频,在此感谢......
文／CoderQiao（简书作者）
原文链接：http://www.jianshu.com/p/11f19f302b28
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
