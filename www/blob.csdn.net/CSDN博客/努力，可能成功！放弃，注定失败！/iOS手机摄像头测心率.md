# iOS手机摄像头测心率 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月21日 09:00:11[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2590
> 
最近在封装一个手机摄像头测心率的模块，搞得精神各种紧张，导致吃饭、路上、做梦，甚至都在想这个东西，就在刚刚终于搞完了，写个文章庆祝一下。
## 原理
简单介绍一下，网上可以查到很多关于手机测心率的这种项目，大概就是：*把手指放在摄像头和闪光灯上，通过手指处脉搏跳动充血导致的细微颜色变化来确定心跳波动，确定波峰波谷，根据两个波峰之间的时间差来确定瞬时心率。*
## 思路
- 首先，采集视频流，根据拿到的RGB颜色转成HSV颜色集，其实我们只用到了HSV的H。
- 对拿到的H进行一些处理，看跟人喜好或者具体情况，主要是用于后面的折线图和计算瞬时心率，如果有能力的话可以处理一下噪音数据，因为可能测的时候手指轻微抖动会造成一些不稳定的数据。
- 根据处理后的H就可以进行画折线图了，我是把处理后的H和时间戳进行了绑定，用来后面的计算心率。
- 根据处理后的H来确定波峰波谷，利用两个波谷之间的时间差计算心率。
## 实现
> 
大致思路就是上面这样，下面来看一下代码具体实现以下。
##### 1.首先我先初始化了一些数据,方便后面使用
```
// 设备
@property (strong, nonatomic) AVCaptureDevice           *device;
// 结合输入输出
@property (strong, nonatomic) AVCaptureSession          *session;
// 输入设备
@property (strong, nonatomic) AVCaptureDeviceInput      *input;
// 输出设备
@property (strong, nonatomic) AVCaptureVideoDataOutput  *output;
// 输出的所有点
@property (strong, nonatomic) NSMutableArray            *points;
// 记录浮点变化的前一次的值
static float lastH = 0;
// 用于判断是否是第一个福点值
static int   count = 0;
// 初始化
self.device     = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
self.session    = [[AVCaptureSession alloc]init];
self.input      = [[AVCaptureDeviceInput alloc]initWithDevice:self.device error:nil];
self.output     = [[AVCaptureVideoDataOutput alloc]init];
self.points     = [[NSMutableArray alloc]init];
```
##### 2.设置视频采集流，为了节省内存，我没有输出视频画面
```
// 开启闪光灯
  if ([self.device isTorchModeSupported:AVCaptureTorchModeOn]) {
      [self.device lockForConfiguration:nil];
        // 开启闪光灯
       self.device.torchMode=AVCaptureTorchModeOn;
        // 调低闪光灯亮度（为了减少内存占用和避免时间长手机发烫）
        [self.device setTorchModeOnWithLevel:0.01 error:nil];
        [self.device unlockForConfiguration];
    }
    // 开始配置input output
    [self.session beginConfiguration];
    // 设置像素输出格式
    NSNumber *BGRA32Format = [NSNumber numberWithInt:kCVPixelFormatType_32BGRA];
    NSDictionary *setting  =@{(id)kCVPixelBufferPixelFormatTypeKey:BGRA32Format};
    [self.output setVideoSettings:setting];
    // 抛弃延迟的帧
    [self.output setAlwaysDiscardsLateVideoFrames:YES];
    //开启摄像头采集图像输出的子线程
    dispatch_queue_t outputQueue = dispatch_queue_create("VideoDataOutputQueue", DISPATCH_QUEUE_SERIAL);
    // 设置子线程执行代理方法
    [self.output setSampleBufferDelegate:self queue:outputQueue];
    // 向session添加
    if ([self.session canAddInput:self.input])   [self.session addInput:self.input];
    if ([self.session canAddOutput:self.output]) [self.session addOutput:self.output];
    // 降低分辨率，减少采样率（为了减少内存占用）
    self.session.sessionPreset = AVCaptureSessionPreset1280x720;
    // 设置最小的视频帧输出间隔
    self.device.activeVideoMinFrameDuration = CMTimeMake(1, 10);
    // 用当前的output 初始化connection
    AVCaptureConnection *connection =[self.output connectionWithMediaType:AVMediaTypeVideo];
    [connection setVideoOrientation:AVCaptureVideoOrientationPortrait];
    // 完成编辑
    [self.session commitConfiguration];
    // 开始运行
    [self.session startRunning];
```
*这里我降低了闪光灯亮度，降低了分辨率，减少了每秒钟输出的帧。主要就是为了减少内存的占用。（我手里只有一台6，没有测其他设备可不可以）*
##### 3.在output的代理方法中采集视频流
```cpp
```cpp
// captureOutput->当前output   sampleBuffer->样本缓冲   connection->捕获连接
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    //获取图层缓冲
    CVPixelBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    CVPixelBufferLockBaseAddress(imageBuffer, 0);
    uint8_t*buf = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(imageBuffer);
    size_t width = CVPixelBufferGetWidth(imageBuffer);
    size_t height = CVPixelBufferGetHeight(imageBuffer);
    float r = 0, g = 0,b = 0;
    float h,s,v;
    // 计算RGB
    TORGB(buf, width, height, bytesPerRow, &r, &g, &b);
    // RGB转HSV
    RGBtoHSV(r, g, b, &h, &s, &v);
    // 获取当前时间戳（精确到毫秒）
    double t = [[NSDate date] timeIntervalSince1970]*1000;
    // 返回处理后的浮点值
    float p = HeartRate(h);
    // 绑定浮点和时间戳
    NSDictionary *point = @{[NSNumber numberWithDouble:t]:[NSNumber numberWithFloat:p]};
    //下面按个人情况可以进行计算心率或者画心率图
}
```
```
*到这里数据已经处理好了，后面可以根据数据画折线图，或者计算心率*
**计算RGB**
```cpp
```cpp
void TORGB (uint8_t *buf, float ww, float hh, size_t pr, float *r, float *g, float *b) {
    float wh = (float)(ww * hh );
    for(int y = 0; y < hh; y++) {
        for(int x = 0; x < ww * 4; x += 4) {
            *b += buf[x];
            *g += buf[x+1];
            *r += buf[x+2];
        }
        buf += pr;
    }
    *r /= 255 * wh;
    *g /= 255 * wh;
    *b /= 255 * wh;
}
```
```
**RGB转HSV**
```cpp
```cpp
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v ) {
    float min, max, delta;
    min = MIN( r, MIN(g, b ));
    max = MAX( r, MAX(g, b ));
    *v = max;
    delta = max - min;
    if( max != 0 )
        *s = delta / max;
    else {
        *s = 0;
        *h = -1;
        return;
    }
    if( r == max )
        *h = ( g - b ) / delta;
    else if( g == max )
        *h = 2 + (b - r) / delta;
    else
        *h = 4 + (r - g) / delta;
    *h *= 60;
    if( *h < 0 )
        *h += 360;
}
```
```
**根据h处理浮点**
```cpp
```cpp
float HeartRate (float h) {
    float low = 0;
    count++;
    lastH = (count==1)?h:lastH;
    low = (h-lastH);
    lastH = h;
    return low;
}
```
```
##### 4.分析数据，计算心率
> 
这里我纠结了好长时间，试了几种不同的方法，都没有一个比较理想的结果，计算出来的特别不准。后来看了[http://ios.jobbole.com/88158/](http://ios.jobbole.com/88158/) 这篇文章，后面优化的部分有一个`基音算法`，虽不明，但觉厉，对此表示非常感谢。吼吼吼。
**原理:** 就是说划一个时间段,在这个时间段里面找到一个`最低峰值`,然后确定一个`周期`，然后分别在`这个峰值`前间隔`0.5个周期`的`1周期里`和`这个峰值`后间隔`0.5个周期`的`1周期`里找到一个最低峰值。 然后根据这几个值来确定瞬时心率。
```
- (void)analysisPointsWith:(NSDictionary *)point {
    [self.points addObject:point];
    if (self.points.count<=30) return;
    int count = (int)self.points.count;
    if (self.points.count%10 == 0) {
        int d_i_c = 0;          //最低峰值的位置 姑且算在中间位置 c->center
        int d_i_l = 0;          //最低峰值左面的最低峰值位置 l->left
        int d_i_r = 0;          //最低峰值右面的最低峰值位置 r->right
        float trough_c = 0;     //最低峰值的浮点值
        float trough_l = 0;     //最低峰值左面的最低峰值浮点值
        float trough_r = 0;     //最低峰值右面的最低峰值浮点值
        // 1.先确定数据中的最低峰值
        for (int i = 0; i < count; i++) {
            float trough = [[[self.points[i] allObjects] firstObject] floatValue];
            if (trough < trough_c) {
                trough_c = trough;
                d_i_c = i;
            }
        }
        // 2.找到最低峰值以后  以最低峰值为中心 找到前0.5-1.5周期中的最低峰值  和后0.5-1.5周期的最低峰值
        if (d_i_c >= 1.5*T) {
            // a.如果最低峰值处在中心位置， 即距离前后都至少有1.5个周期
            if (d_i_c <= count-1.5*T) {
                // 左面最低峰值
                for (int j = d_i_c - 0.5*T; j > d_i_c - 1.5*T; j--) {
                    float trough = [[[self.points[j] allObjects] firstObject] floatValue];
                    if (trough < trough_l) {
                        trough_l = trough;
                        d_i_l = j;
                    }
                }
                // 右面最低峰值
                for (int k = d_i_c + 0.5*T; k < d_i_c + 1.5*T; k++) {
                    float trough = [[[self.points[k] allObjects] firstObject] floatValue];
                    if (trough < trough_r) {
                        trough_r = trough;
                        d_i_r = k;
                    }
                }
            }
            // b.如果最低峰值右面不够1.5个周期 分两种情况 不够0.5个周期和够0.5个周期
            else {
                // b.1 够0.5个周期
                if (d_i_c <count-0.5*T) {
                    // 左面最低峰值
                    for (int j = d_i_c - 0.5*T; j > d_i_c - 1.5*T; j--) {
                        float trough = [[[self.points[j] allObjects] firstObject] floatValue];
                        if (trough < trough_l) {
                            trough_l = trough;
                            d_i_l = j;
                        }
                    }
                    // 右面最低峰值
                    for (int k = d_i_c + 0.5*T; k < count; k++) {
                        float trough = [[[self.points[k] allObjects] firstObject] floatValue];
                        if (trough < trough_r) {
                            trough_r = trough;
                            d_i_r = k;
                        }
                    }
                }
                // b.2 不够0.5个周期
                else {
                    // 左面最低峰值
                    for (int j = d_i_c - 0.5*T; j > d_i_c - 1.5*T; j--) {
                        float trough = [[[self.points[j] allObjects] firstObject] floatValue];
                        if (trough < trough_l) {
                            trough_l = trough;
                            d_i_l = j;
                        }
                    }
                }
            }
        }
        // c. 如果左面不够1.5个周期 一样分两种情况  够0.5个周期 不够0.5个周期
        else {
            // c.1 够0.5个周期
            if (d_i_c>0.5*T) {
                // 左面最低峰值
                for (int j = d_i_c - 0.5*T; j > 0; j--) {
                    float trough = [[[self.points[j] allObjects] firstObject] floatValue];
                    if (trough < trough_l) {
                        trough_l = trough;
                        d_i_l = j;
                    }
                }
                // 右面最低峰值
                for (int k = d_i_c + 0.5*T; k < d_i_c + 1.5*T; k++) {
                    float trough = [[[self.points[k] allObjects] firstObject] floatValue];
                    if (trough < trough_r) {
                        trough_r = trough;
                        d_i_r = k;
                    }
                }
            }
            // c.2 不够0.5个周期
            else {
                // 右面最低峰值
                for (int k = d_i_c + 0.5*T; k < d_i_c + 1.5*T; k++) {
                    float trough = [[[self.points[k] allObjects] firstObject] floatValue];
                    if (trough < trough_r) {
                        trough_r = trough;
                        d_i_r = k;
                    }
                }
            }
        }
        // 3. 确定哪一个与最低峰值更接近 用最接近的一个最低峰值测出瞬时心率 60*1000两个峰值的时间差
        if (trough_l-trough_c < trough_r-trough_c) {
            NSDictionary *point_c = self.points[d_i_c];
            NSDictionary *point_l = self.points[d_i_l];
            double t_c = [[[point_c allKeys] firstObject] doubleValue];
            double t_l = [[[point_l allKeys] firstObject] doubleValue];
            NSInteger fre = (NSInteger)(60*1000)/(t_c - t_l);
            if (self.frequency)
                self.frequency(fre);
            if ([self.delegate respondsToSelector:@selector(startHeartDelegateRateFrequency:)])
                [self.delegate startHeartDelegateRateFrequency:fre];
        } else {
            NSDictionary *point_c = self.points[d_i_c];
            NSDictionary *point_r = self.points[d_i_r];
            double t_c = [[[point_c allKeys] firstObject] doubleValue];
            double t_r = [[[point_r allKeys] firstObject] doubleValue];
            NSInteger fre = (NSInteger)(60*1000)/(t_r - t_c);
            if (self.frequency)
                self.frequency(fre);
            if ([self.delegate respondsToSelector:@selector(startHeartDelegateRateFrequency:)])
                [self.delegate startHeartDelegateRateFrequency:fre];
        }
        // 4.删除过期数据
        for (int i = 0; i< 10; i++) {
            [self.points removeObjectAtIndex:0];
        }
    }
}
```
*我目前是这样处理的，后面是用的前后两个峰值与`最低峰值`最接近的那个峰值的时间差，测了几次又和别的app比较了一下，基本都是正确的，最多也就是上下差1-2次每分钟。（在数据比较稳定的情况下，如果有更好的方法请推荐，谢谢）*
##### 5.画折线图 这里用到了 `CoreGraphics`
PS:首先，使用这个CoreGraphics要在View里面，并且要在View的`drawRect:`方法中使用，不然拿不到画布。我是为了封装单独建立了一个UIView的类。
###### a.首先还是数据，没有数据怎么画
```
@property (strong, nonatomic) NSMutableArray *points;
// 在init中初始化数组
self.points = [[NSMutableArray alloc]init];
// 这个可以翻译过来，也是在init中
self.clearsContextBeforeDrawing = YES;
// 外部调用方法
- (void)drawRateWithPoint:(NSNumber *)point {
    // 倒叙插入数组
    [self.points insertObject:point atIndex:0];
    // 删除溢出屏幕数据
    if (self.points.count > self.frame.size.width/6) {
        [self.points removeLastObject];
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        // 这个方法自动调取 drawRect:方法
        [self setNeedsDisplay];
    });
}
```
*之前调`setNeedsDisplay`,一直没有走`drawRect:`方法，或者就直走了一次，然后去百度是说`setNeedsDisplay`会在系统空闲的时候执行`drawRect:`,然后我尝试着回归到主线程中调用，就好了。具体原因不是很清楚，也可能是因为要在主线程中修改View。*
###### b.画折线的方法，具体怎么调整看个人心情了。
```
CGFloat ww = self.frame.size.width;
    CGFloat hh = self.frame.size.height;
    CGFloat pos_x = ww;
    CGFloat pos_y = hh/2;
    // 获取当前画布
    CGContextRef context = UIGraphicsGetCurrentContext();
    // 折线宽度
    CGContextSetLineWidth(context, 1.0);
    //消除锯齿
    //CGContextSetAllowsAntialiasing(context,false);
    // 折线颜色
    CGContextSetStrokeColorWithColor(context, [UIColor redColor].CGColor);
    CGContextMoveToPoint(context, pos_x, pos_y);
    for (int i = 0; i < self.points.count; i++) {
        float h = [self.points[i] floatValue];
        pos_y = hh/2 + (h * hh/2) ;
        CGContextAddLineToPoint(context, pos_x, pos_y);
        pos_x -=6;
    }
    CGContextStrokePath(context);
```
###### c.为了看起来好看，我还加了网格，当然也是在`drawRect:`中调用的
```
static CGFloat grid_w = 30.0f;
- (void)buildGrid {
    CGFloat wight  = self.frame.size.width;
    CGFloat height = self.frame.size.height;
    // 获取当前画布
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGFloat pos_x = 0.0f;
    CGFloat pos_y = 0.0f;
    // 在wight范围内画竖线
    while (pos_x < wight) {
        // 设置网格线宽度
        CGContextSetLineWidth(context, 0.2);
        // 设置网格线颜色
        CGContextSetStrokeColorWithColor(context, [UIColor greenColor].CGColor);
        // 起点
        CGContextMoveToPoint(context, pos_x, 1.0f);
        // 终点
        CGContextAddLineToPoint(context, pos_x, height);
        pos_x +=grid_w;
        //开始划线
        CGContextStrokePath(context);
    }
    // 在height范围内画横线
    while (pos_y < height) {
        CGContextSetLineWidth(context, 0.2);
        CGContextSetStrokeColorWithColor(context, [UIColor greenColor].CGColor);
        CGContextMoveToPoint(context, 1.0f, pos_y);
        CGContextAddLineToPoint(context, wight, pos_y);
        pos_y +=grid_w;
        CGContextStrokePath(context);
    }
    pos_x = 0.0f; pos_y = 0.0f;
    // 在wight范围内画竖线
    while (pos_x < wight) {
        CGContextSetLineWidth(context, 0.1);
        CGContextSetStrokeColorWithColor(context, [UIColor greenColor].CGColor);
        CGContextMoveToPoint(context, pos_x, 1.0f);
        CGContextAddLineToPoint(context, pos_x, height);
        pos_x +=grid_w/5;
        CGContextStrokePath(context);
    }
    // 在height范围内画横线
    while (pos_y < height) {
        CGContextSetLineWidth(context, 0.1);
        CGContextSetStrokeColorWithColor(context, [UIColor greenColor].CGColor);
        CGContextMoveToPoint(context, 1.0f, pos_y);
        CGContextAddLineToPoint(context, wight, pos_y);
        pos_y +=grid_w/5;
        CGContextStrokePath(context);
    }
}
```
### 总结
没有图，没有效果，只有思路和代码，我把封装好的代码放在了GitHub上，地址：[https://github.com/YvanLiu/HeartBeatsPlugin.git](https://github.com/YvanLiu/HeartBeatsPlugin.git)
写这个功能的时候，自己有很多思考，也参考了很多其他人的博客、代码还有别人的毕业论文，呵呵呵，还问了几个学医的同学，代码不难，数据处理的部分可能不太好弄，但是写完还是有点成就感的。
代码里还存在很多问题，心率的计算还存在很高的错误率，现在正在想办法改进，欢迎指正，欢迎批评，欢迎提问。
文／YvanLiu（简书作者）
原文链接：http://www.jianshu.com/p/9f678e0bdf9b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
