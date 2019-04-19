# iOS 仿微信、微博底部弹出选项框 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月21日 13:47:41[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：138
由于项目中经常使用，封装了一个简单的底部弹出框控件，可以自定义一下属性，希望可以帮到需要的朋友。
[GitHub地址](https://github.com/zhenguanqing/ZGQActionSheetView)
使用方法很简单，将文件夹导入工程中，并引入头文件`#import "ZGQActionSheetView.h"`
同时支持block与delegate两种回调方式，推荐使用block
block方式：
```
- (IBAction)ShowAction:(id)sender {
    NSArray *optionArray = @[@"发送给朋友",@"收藏",@"保存图片",@"编辑"];
    ZGQActionSheetView *sheetView = [[ZGQActionSheetView alloc] initWithOptions:optionArray completion:^(NSInteger index) {
        NSLog(@"%@",optionArray[index]);
    } cancel:^{
         NSLog(@"取消");
    }];
    [sheetView show];
}
```
delegate方式：
```
- (IBAction)ShowAction1:(UIButton *)sender {
    NSArray *optionArray = @[@"发送给朋友",@"收藏",@"保存图片",@"编辑"];
    optionArray = [NSArray array];
    ZGQActionSheetView *sheetView = [[ZGQActionSheetView alloc] initWithOptions:optionArray];
    sheetView.delegate = self;
    [sheetView show];
}
- (void)ZGQActionSheetView:(ZGQActionSheetView *)sheetView didSelectRowAtIndex:(NSInteger)index text:(NSString *)text {
    NSLog(@"%zd,%@",index,text);
}
```
sheetView 样式及属性可自定义，头文件中已给出详细注释
效果图：
![](https://upload-images.jianshu.io/upload_images/1749044-797df5aa5d2ef4bf?imageMogr2/auto-orient/strip%7CimageView2/2/w/351/format/webp)
image
作者：左左4143
链接：https://www.jianshu.com/p/000402aa5f78
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
