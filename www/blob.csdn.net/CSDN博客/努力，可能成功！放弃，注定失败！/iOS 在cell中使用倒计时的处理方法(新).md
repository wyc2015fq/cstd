# iOS 在cell中使用倒计时的处理方法(新) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年07月05日 10:35:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1895
## 本文来自简书，原文地址:[http://www.jianshu.com/p/af62a56ef7e2](http://www.jianshu.com/p/af62a56ef7e2)
## 前言
之前的文章[iOS 在cell中使用倒计时的处理方法](http://www.jianshu.com/p/0f9ed092dd7f)得到大量的支持, 在这先感谢大家的支持. 但是也收到不少人的回复表示不会用, 需要一一解答, 由于之前写的时候没有使用Markdown编辑, 所以现在没法更新之前的文章, 重新写一份清晰的文章
> 
需求: 每条Cell中显示倒计时, 并随时间进行倒数
语言: Objective-C & Swift
系统: iOS
Github地址: [OYCountDownManager](https://github.com/herobin22/OYCountDownManager)
## 原理分析
![](http://upload-images.jianshu.io/upload_images/1646270-622a49c3f6d9b4f3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
原理分析图.png
![](http://upload-images.jianshu.io/upload_images/1646270-9493be0729fc6e21.gif?imageMogr2/auto-orient/strip)
效果图.gif
## 使用方法
### 1. 导入"OYCountDownManager.h"
### 2. 在第一次使用的地方调用[kCountDownManager start]
```
- (void)viewDidLoad {
    [super viewDidLoad];
    // 启动倒计时管理
    [kCountDownManager start];
}
```
### 3. 在Cell初始化中监听通知 kCountDownNotification
```
- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        // 监听通知
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(countDownNotification) name:kCountDownNotification object:nil];
    }
    return self;
}
```
### 4. 在cell设置通知回调, 取得时间差, 根据时间差进行处理
```
- (void)countDownNotification {
    /// 计算倒计时
    NSInteger countDown = [self.model.count integerValue] - kCountDownManager.timeInterval;
    if (countDown <= 0) {
          // 倒计时结束时回调
          xxxx(使用代理或block)
    }return;
    /// 重新赋值
    self.timeLabel.text = [NSString stringWithFormat:@"倒计时%02zd:%02zd:%02zd", countDown/3600,       (countDown/60)%60, countDown%60];
}
```
### 5. 当刷新数据时,调用reload方法
```
- (void)reloadData {
    // 网络加载数据
    // 调用[kCountDownManager reload]
    [kCountDownManager reload];
    // 刷新
    [self.tableView reloadData];
}
```
### 6. 当不需要倒计时时, 废除定时器
`      [kCountDownManager invalidate];`
## 注意事项
> 
### 滚动cell时出去文字闪烁
在给cell的模型赋值后, 最好手动调用一下countDownNotification方法, 保证及时刷新
```
///  重写setter方法
 - (void)setModel:(Model *)model {
     _model = model;
     self.titleLabel.text = model.title;
     // 手动调用通知的回调
     [self countDownNotification];
}
```
### 倒计时为0后出现复用问题
在倒计时为0后, 应该回调给控制器, 从后台请求一次数据, 保证倒计时没有出现误差
```
if (countDown <= 0) {
           // 倒计时结束时回调
           xxxx(使用代理或block)
     }return;
```
### 出现每秒倒计时减2的问题
查看定时器设置是否正确, 或者通知是否监听了两次, 最好将监听通知的方法写在init中, 而不是写在awakeFromNib中
如果还有不懂的问题, 或者出现其它bug
请查看Demo: [Demo](https://github.com/herobin22/OYCountDownManager)
