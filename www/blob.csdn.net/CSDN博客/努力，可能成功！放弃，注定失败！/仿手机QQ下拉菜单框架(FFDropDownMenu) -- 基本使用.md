# 仿手机QQ下拉菜单框架(FFDropDownMenu) -- 基本使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月10日 08:38:14[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1645
最近写了这个框架: FFDropDownMenu,类似手机QQ下拉菜单。
github地址:
[https://github.com/chenfanfang/FFDropDownMenu](https://github.com/chenfanfang/FFDropDownMenu)
更多的使用方法的demo地址:
[https://github.com/chenfanfang/CollectionsOfExample](https://github.com/chenfanfang/CollectionsOfExample)
更多的使用方法的博客地址:
[http://www.jianshu.com/notebooks/5552428/latest](http://www.jianshu.com/notebooks/5552428/latest)
先浏览下效果图：
![](http://upload-images.jianshu.io/upload_images/1594675-50fc56a477704ccd.gif?imageMogr2/auto-orient/strip)
仿手机QQ下拉菜单.gif
![](http://upload-images.jianshu.io/upload_images/1594675-c8959e91b20bb4f9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
仿手机QQ下拉菜单.png
## Installation【安装】
### From CocoaPods【使用CocoaPods】
``pod  FFDropDownMenu``
### Manually【手动导入】
- 
Drag all source files under floder `FFDropDownMenu` to your project.【将`FFDropDownMenu`文件夹中的所有源代码拽入项目中】
- 
FFDropDownMenu文件夹里面的文件有
```
```
FFDropDownMenuBasedCell.h        FFDropDownMenuBasedCell.m
FFDropDownMenuBasedModel.h       FFDropDownMenuBasedModel.m
FFDropDownMenuCell.h             FFDropDownMenuCell.m
FFDropDownMenuModel.h            FFDropDownMenuModel.m
FFDropDownMenuTriangleView.h     FFDropDownMenuTriangleView.m
FFDropDownMenuView.h             FFDropDownMenuView.m
```
```
## Basic Usage 【基本的使用】
先导入头文件
```
//若使用CocoaPods
#import <FFDropDownMenuView.h>
//若使用手动导入
#import "FFDropDownMenuView.h"
```
创建一个strong的下拉菜单属性
```
/** 下拉菜单 */
@property (nonatomic, strong) FFDropDownMenuView *dropDownMenu;
```
创建菜单模型数组
```php
```php
//FFDropDownMenuModel.h
/**
 *  快速实例化一个下拉菜单模型
 *
 *  @param menuItemTitle    菜单选项的标题
 *  @param menuItemIconName 菜单选项的图标名称
 *  @param menuBlock        点击的回调block
 *
 *  @return 实例化的菜单模型
 */
+ (instancetype)ff_DropDownMenuModelWithMenuItemTitle:(NSString *)menuItemTitle menuItemIconName:(NSString *)menuItemIconName menuBlock:(FFMenuBlock)menuBlock;
```
```
```
/** 获取下拉菜单模型数组 */
- (NSArray *)getDropDownMenuModelsArray {
    __weak typeof(self)weakSelf = self;
    //菜单模型0
    FFDropDownMenuModel *menuModel0 = [FFDropDownMenuModel ff_DropDownMenuModelWithMenuItemTitle:@"Twitter" menuItemIconName:@"menu0" menuBlock:^{
        UIViewController *vc = [UIViewController new];
        [weakSelf.navigationController pushViewController:vc animated:YES];
    }];
    //菜单模型1
    FFDropDownMenuModel *menuModel1 = [FFDropDownMenuModel ff_DropDownMenuModelWithMenuItemTitle:@"Line" menuItemIconName:@"menu1" menuBlock:^{
        //Do Something
    }];
    //菜单模型2
    FFDropDownMenuModel *menuModel2 = [FFDropDownMenuModel ff_DropDownMenuModelWithMenuItemTitle:@"QQ" menuItemIconName:@"menu2"  menuBlock:^{
       //Do Something
    }];
    //菜单模型3
    FFDropDownMenuModel *menuModel3 = [FFDropDownMenuModel ff_DropDownMenuModelWithMenuItemTitle:@"QZone" menuItemIconName:@"menu3"  menuBlock:^{
        //Do Something
    }];
    //菜单模型4
    //...................
    NSArray *menuModelArr = @[menuModel0, menuModel1, menuModel2, menuModel3......];
    return menuModelArr;
}
```
创建下拉菜单方式1
```cpp
```cpp
/** 创建下拉菜单方式1 */
- (void)createDropdownMenuMethodOne {
    //若使用默认CGFloat值     请使用 FFDefaultFloat
    //若使用默认CGSize值      请使用 FFDefaultSize
    //若使用默认Cell值        请使用 FFDefaultCell
    //若使用默认Color值       请使用 FFDefaultColor
    //若使用默认ScaleType值   请使用 FFDefaultMenuScaleType
    NSArray *menuModelsArr = [self getDropDownMenuModelsArray];
    self.dropDownMenu = [FFDropDownMenuView ff_DefaultStyleDropDownMenuWithMenuModelsArray:menuModelsArr menuWidth:145 eachItemHeight:40 menuRightMargin:10 triangleRightMargin:20];
    //若还需要对别的属性进行赋值，则可以再对别的属性进行赋值，最后一定要调用setup方法。如下
    /*
    self.dropDownMenu.menuScaleType = FFDropDownMenuViewScaleType_TopRight;
    self.dropDownMenu...... = ......;
    [self.dropDownMenu setup];
     */
}
```
```
创建下拉菜单方式2
```
/** 创建下拉菜单方式2 */
- (void)createDropdownMenuMethodTwo {
    NSArray *menuModelsArr = [self getDropDownMenuModelsArray];
    self.dropDownMenu = [FFDropDownMenuView new];
    //进行属性的赋值
    //若使用默认CGFloat值     请使用 FFDefaultFloat          、或者无需进行赋值
    //若使用默认CGSize值      请使用 FFDefaultSize           、或者无需进行赋值
    //若使用默认Cell值        请使用 FFDefaultCell           、或者无需进行赋值
    //若使用默认Color值       请使用 FFDefaultColor          、或者无需进行赋值
    //若使用默认ScaleType值   请使用 FFDefaultMenuScaleType  、或者无需进行赋值
    /** 下拉菜单模型数组 */
    self.dropDownMenu.menuModelsArray = menuModelsArr;
    /** cell的类名 */
    self.dropDownMenu.cellClassName = FFDefaultCell;
    /** 菜单的宽度(若不设置，默认为 150) */
    self.dropDownMenu.menuWidth = 145;
    /** 菜单的圆角半径(若不设置，默认为5) */
    self.dropDownMenu.menuCornerRadius = FFDefaultFloat;
    /** 每一个选项的高度(若不设置，默认为40) */
    self.dropDownMenu.eachMenuItemHeight = 40;
    /** 菜单条离屏幕右边的间距(若不设置，默认为10) */
    self.dropDownMenu.menuRightMargin = 10;
    /** 三角形颜色(若不设置，默认为白色) */
    self.dropDownMenu.triangleColor = [UIColor whiteColor];
    /** 三角形相对于keyWindow的y值,也就是相对于屏幕顶部的y值(若不设置，默认为64) */
    self.dropDownMenu.triangleY = FFDefaultFloat;
    /** 三角形距离屏幕右边的间距(若不设置，默认为20) */
    self.dropDownMenu.triangleRightMargin = FFDefaultFloat;
    /** 三角形的size  size.width:代表三角形底部边长，size.Height:代表三角形的高度(若不设置，默认为CGSizeMake(15, 10)) */
    self.dropDownMenu.triangleSize = FFDefaultSize;
    /** 背景颜色开始时的透明度(还没展示menu的透明度)(若不设置，默认为0.02) */
    self.dropDownMenu.bgColorbeginAlpha = 0;
    /** 背景颜色结束的的透明度(menu完全展示的透明度)(若不设置，默认为0.2) */
    self.dropDownMenu.bgColorEndAlpha = 0.4;
    /** 动画效果时间(若不设置，默认为0.2) */
    self.dropDownMenu.animateDuration = FFDefaultFloat;
    /** 菜单的伸缩类型 */
    self.dropDownMenu.menuScaleType = FFDefaultMenuScaleType;
    //所有属性赋值完 一定要调用 setup
    [self.dropDownMenu setup];
}
```
显示菜单
`    [self.dropDownMenu showMenu];`
## 期待
- 如果在使用过程中遇到BUG，希望你能在 简书私信我，或者在我简书专题的博客进行评论。谢谢（或者尝试下载最新的框架代码看看BUG修复没有）
- 如果在使用过程中发现功能不够用，希望你能在 简书私信我，或者在我简书专题的博客进行评论。我非常想为这个框架增加更多好用的功能，谢谢
- 如果你想和我一起完善FFDropDownMenu，请Pull Requests我
文／chenfanfang（简书作者）
原文链接：http://www.jianshu.com/p/56f2f2dd4c59
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
