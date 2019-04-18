# 自己写的好用的3D Touch封装类 - weixin_33985507的博客 - CSDN博客
2017年04月30日 21:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
最近工作不忙,无聊的封装了3D Touch工具,使用简单,来跟大家分享一下,喜欢的朋友请加星哦😄。
[CXForceTouchManager github地址](https://link.jianshu.com?t=https://github.com/chengxuzheng/CX3DTouchManager.git)
![3683848-2f0e85a6d53195db.png](https://upload-images.jianshu.io/upload_images/3683848-2f0e85a6d53195db.png)
触发的样式图
###### 下面附加使用方法
```
/** 检测ForceTouch是否可用 **/
+ (BOOL)checkForceTouchCapabilityAvailable;
/** 开启设置方法 **/
+ (void)makeShortcutItems:(void(^_Nonnull)(CXForceTouchManager *_Nonnull manager))block;
/** 添加item的方法
    type 通知标识符
    title 标题
    subtitle 副标题
    imageName 图片名
 */
- (CXForceTouchManager *_Nonnull(^_Nonnull)(NSString * _Nonnull type, NSString * _Nonnull title, NSString * _Nullable subtitle, NSString * _Nonnull imageName))addItem;
/** 完成添加item的方法 **/
- (CXForceTouchManager * _Nonnull (^_Nonnull)())finished;
/** 点击item的方法 **/
+ (void)didSelectItemWithType:(NSString *_Nonnull)type;
```
###### 在项目中的实际使用如下
- 需要添加3D Touch快捷按钮
```
if([CXForceTouchManager checkForceTouchCapabilityAvailable]) {
        [CXForceTouchManager makeShortcutItems:^(CXForceTouchManager *manager) {
            manager.addItem(@"kNotificationForceTouch1",@"按键一",nil,@"add");
            manager.addItem(@"kNotificationForceTouch2",@"按键二",nil,@"addTime");
            manager.addItem(@"kNotificationForceTouch3",@"按键三",nil,@"clock");
            manager.finished();
        }];
    } else {
        NSLog(@"Force Touch 不可用");
    }
```
- 在AppDelegate里- (void)application:(UIApplication *)application performActionForShortcutItem:(UIApplicationShortcutItem *)shortcutItem completionHandler:(void (^)(BOOL))completionHandler方法中调用3D Touch按钮被点击事件发出通知
```
[CXForceTouchManager didSelectItemWithType:shortcutItem.type];
```
- 通过type类型(如:kNotificationForceTouch1)添加通知的接收者
```
[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(kNotificationForceTouch1Action) name:@"kNotificationForceTouch1" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(kNotificationForceTouch2Action) name:@"kNotificationForceTouch2" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(kNotificationForceTouch3Action) name:@"kNotificationForceTouch3" object:nil];
```
- 通知调用的跳转方法如下:
```
- (void)kNotificationForceTouch1Action {
    FirstViewController *first = [FirstViewController new];
    [self.window.rootViewController presentViewController:first animated:YES completion:nil];
}
 - (void)kNotificationForceTouch2Action {
    SecondViewController *second = [SecondViewController new];
    [self.window.rootViewController presentViewController:second animated:YES completion:nil];
}
 - (void)kNotificationForceTouch3Action {
    ThirdViewController *third = [ThirdViewController new];
    [self.window.rootViewController presentViewController:third animated:YES completion:nil];
}
```
- 记得要在dealloc方法中移除通知哦
```
- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"kNotificationForceTouch1" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"kNotificationForceTouch2" object:nil];
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"kNotificationForceTouch3" object:nil];
}
```
