# [iOS]动态修改app 图标（icon） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年03月20日 10:34:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：9标签：[[iOS]动态修改app 图标（icon）](https://so.csdn.net/so/search/s.do?q=[iOS]动态修改app 图标（icon）&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
动态修改app的图标，就是在不重新安装app的情况下，可以修改当前的icon图标；在某些情况下，是有这个需求的；例如，可以更换主题的app中，一般都会有一套完整的主题包含相应的icon；还有就是一些节日主题的icon或者促销的icon，例如淘宝、京东等的节日icon。
在iOS 10.3之后，苹果官方提供了相关的API来实现这个功能，主要是下面这几个方法：
```
@interface UIApplication (UIAlternateApplicationIcons)
// 如果为NO，表示当前进程不支持替换图标
@property (readonly, nonatomic) BOOL supportsAlternateIcons NS_EXTENSION_UNAVAILABLE("Extensions may not have alternate icons") API_AVAILABLE(ios(10.3), tvos(10.2));
// 传入nil代表使用主图标. 完成后的操作将会在任意的后台队列中异步执行; 如果需要更改UI，请确保在主队列中执行.
- (void)setAlternateIconName:(nullable NSString *)alternateIconName completionHandler:(nullable void (^)(NSError *_Nullable error))completionHandler NS_EXTENSION_UNAVAILABLE("Extensions may not have alternate icons") API_AVAILABLE(ios(10.3), tvos(10.2));
// 如果alternateIconName为nil，则代表当前使用的是主图标.
@property (nullable, readonly, nonatomic) NSString *alternateIconName NS_EXTENSION_UNAVAILABLE("Extensions may not have alternate icons") API_AVAILABLE(ios(10.3), tvos(10.2));
@end
```
方法很简单，但是使用之前需要进行一些配置：
1. 配置icon
添加图片icon
动态修改的icon不能放在 Assets.xcassets 里，但是正常的主icon还是可以在这里设置的，也可以按下面的方法来设置；
首先，把需要修改的icon放在一个文件夹内：
![](https://upload-images.jianshu.io/upload_images/1928848-d47b7b71d7f512e0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/518/format/webp)
icons
其文件夹内是这样的
![](https://upload-images.jianshu.io/upload_images/1928848-9d7261321cd78fb5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
这里每种icon我只放了一个，如果有多个尺寸的icon，也可以直接全放进去：
![](https://upload-images.jianshu.io/upload_images/1928848-ab1ad81c5c35f0a0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/508/format/webp)
多尺寸icon
然后，文件夹会变成这样：
![](https://upload-images.jianshu.io/upload_images/1928848-6a2540e3171bd44f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
> 
这里的icon名称只需要和下面配置一致即可
配置info.plist
在info.plist中右键 -> Add Row ：
输入Icon... 会有提示，选择Icon files（iOS 5）
![](https://upload-images.jianshu.io/upload_images/1928848-b37a04e25da7aada.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/636/format/webp)
这时候，内容是这样的：
![](https://upload-images.jianshu.io/upload_images/1928848-61c1c7d59b63edc5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/844/format/webp)
> 
这里的Icon files（iOS 5）是个字典，其中可包含的Key值有CFBundlePrimaryIcon -> Primary Icon
CFBundleAlternateIcons
UINewsstandIcon -> Newsstand Icon
这里的Primary Icon是设置app的主icon，可以在这里的Icon files数组内添加，有多个的话，依次添加，也可以这里不用填写，直接在Assets.xcassets 里配置；
下面的Newsstand Icon，暂时用不到，不用管，也可以删除。
在 Icon files（iOS 5）内添加一个Key： CFBundleAlternateIcons ，类型为字典，在这个字典里配置我们所有需要动态修改的icon：键为icon的名称，值为一个字典（这个字典里包含两个键：CFBundleIconFiles，其值类型为Array，内容为icon的名称；UIPrerenderedIcon，其值类型为bool，内容为NO，也可以不加此key），例如：
![](https://upload-images.jianshu.io/upload_images/1928848-06a58440b9e2aaae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/922/format/webp)
把第一步中添加的图片全部添加进来就是这样的：
![](https://upload-images.jianshu.io/upload_images/1928848-e4285bb504e5f038.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
到此，info.plist的配置即完成了；
或者将info.plist文件以 Source code 方式打开，添加以下代码：
```
<key>CFBundleIcons</key>
    <dict>
        <key>CFBundleAlternateIcons</key>
        <dict>
            <key>rain</key>
            <dict>
                <key>CFBundleIconFiles</key>
                <array>
                    <string>rain</string>
                </array>
                <key>UIPrerenderedIcon</key>
                <false/>
            </dict>
            <key>snow</key>
            <dict>
                <key>CFBundleIconFiles</key>
                <array>
                    <string>snow</string>
                </array>
                <key>UIPrerenderedIcon</key>
                <false/>
            </dict>
            <key>sunshine</key>
            <dict>
                <key>CFBundleIconFiles</key>
                <array>
                    <string>sunshine</string>
                </array>
                <key>UIPrerenderedIcon</key>
                <false/>
            </dict>
            <key>cloudy</key>
            <dict>
                <key>CFBundleIconFiles</key>
                <array>
                    <string>cloudy</string>
                </array>
                <key>UIPrerenderedIcon</key>
                <false/>
            </dict>
        </dict>
        <key>CFBundlePrimaryIcon</key>
        <dict>
            <key>CFBundleIconFiles</key>
            <array>
                <string></string>
            </array>
            <key>UIPrerenderedIcon</key>
            <false/>
        </dict>
        <key>UINewsstandIcon</key>
        <dict>
            <key>CFBundleIconFiles</key>
            <array>
                <string></string>
            </array>
            <key>UINewsstandBindingType</key>
            <string>UINewsstandBindingTypeMagazine</string>
            <key>UINewsstandBindingEdge</key>
            <string>UINewsstandBindingEdgeLeft</string>
        </dict>
    </dict>
```
如果是添加了多个尺寸icon，也要在这里分别配置，以上面添加的sunshine图标为例：
![](https://upload-images.jianshu.io/upload_images/1928848-4bf57f3332d765f6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
使用的时候还是使用sunshine进行赋值即可！
代码
配置完成后，代码部分就比较简单了：
```
- (void)changeAppIconWithName:(NSString *)iconName {
    if (![[UIApplication sharedApplication] supportsAlternateIcons]) {
        return;
    }
    
    if ([iconName isEqualToString:@""]) {
        iconName = nil;
    }
    [[UIApplication sharedApplication] setAlternateIconName:iconName completionHandler:^(NSError * _Nullable error) {
        if (error) {
            NSLog(@"更换app图标发生错误了 ： %@",error);
        }
    }];
}
```
在需要修改icon的地方调用这个方法，并把相应的icon名称传进去即可：
```
- (IBAction)snow:(id)sender {
    [self changeAppIconWithName:@"snow"];
}
- (IBAction)rain:(id)sender {
    [self changeAppIconWithName:@"rain"];
}
- (IBAction)cloudy:(id)sender {
    [self changeAppIconWithName:@"rain"];
}
- (IBAction)sunshine:(id)sender {
    [self changeAppIconWithName:@"sunshine"];
}
```
示意图：
![](https://upload-images.jianshu.io/upload_images/1928848-f35450e6bd0fd5ec.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/294/format/webp)
示意图
设置iPad动态图标
iPad的动态图标设置和上面步骤基本一样，有的文章说是将 CFBundleIcons 改为 CFBundleIcons~ipad，即：
![](https://upload-images.jianshu.io/upload_images/1928848-400f9deb7f6cc21d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
iPad
但是，在测试中发现，使用上面的key值也是可以实现动态改变的，即不做任何修改，iPhone和iPad使用相同的配置，即：CFBundleIcons。
去掉更换icon时的弹框
从上面的示意图可以发现，在设置icon的时候，会有个系统弹框，这样有时候会不太友好，我们可以使用Runtime，对UIViewController进行扩展来隐藏这个弹框：
```
// UIViewController+LQNoPresent.h
#import <UIKit/UIKit.h>
@interface UIViewController (LQNoPresent)
@end
// UIViewController+LQNoPresent.m
#import "UIViewController+LQNoPresent.h"
#import <objc/runtime.h>
@implementation UIViewController (LQNoPresent)
+ (void)load {
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        Method presentM = class_getInstanceMethod(self.class, @selector(presentViewController:animated:completion:));
        Method presentSwizzlingM = class_getInstanceMethod(self.class, @selector(lq_presentViewController:animated:completion:));
        
        method_exchangeImplementations(presentM, presentSwizzlingM);
    });
}
- (void)lq_presentViewController:(UIViewController *)viewControllerToPresent animated:(BOOL)flag completion:(void (^)(void))completion {
    
    if ([viewControllerToPresent isKindOfClass:[UIAlertController class]]) {
//        NSLog(@"title : %@",((UIAlertController *)viewControllerToPresent).title);
//        NSLog(@"message : %@",((UIAlertController *)viewControllerToPresent).message);
        
        UIAlertController *alertController = (UIAlertController *)viewControllerToPresent;
        if (alertController.title == nil && alertController.message == nil) {
            return;
        }
    }
    
    [self lq_presentViewController:viewControllerToPresent animated:flag completion:completion];
}
@end
```
这样在切换图标的时候就没有系统的弹框了：
![](https://upload-images.jianshu.io/upload_images/1928848-e41266dd7be87451.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/294/format/webp)
去掉系统弹框
参考文章
[iOS 10.3 如何更换 app 图标](https://link.jianshu.com?t=http%3A%2F%2Fwww.ctolib.com%2Ftopics-115639.html)
作者：流火绯瞳
链接：https://www.jianshu.com/p/69313970d0e7
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
