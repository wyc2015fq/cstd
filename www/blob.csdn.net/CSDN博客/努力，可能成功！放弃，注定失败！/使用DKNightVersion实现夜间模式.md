# 使用DKNightVersion实现夜间模式 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月24日 08:39:41[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1267
### 概述
DKNightVersion是github上面一个用于实现iOS应用夜间模式和多种主题的开源库。github上面有两个star数较高的库，DKNightVersion和SwiftTheme。后者源码是用swift实现的，OC和Swift混编导致应用的体积大幅度增加，于是选择了DKNightVersion。
### 使用方法
举例说明，此处假设我们的Theme只有两种：普通模式，夜间模式。
```
DKColorPicker Examples
view.dk_backgroundColorPicker = DKColorPickerWithColors([UIColor whiteColor], [UIColor darkGrayColor]); // => view的backgroundColor在普通模式、夜间模式下分别为white、darkGray。
label.dk_textColorPicker = DKColorPickerWithColors([UIColor whiteColor], [UIColor darkGrayColor]);      // => label的textColor在普通模式、夜间模式下分别为white、darkGray。
tabBar.dk_barTintColorPicker = DKColorPickerWithColors([UIColor whiteColor], [UIColor darkGrayColor]);  // => tabBar的barTintColor在普通模式、夜间模式下分别为white、darkGray。
```
等等，能用别的方式来创建dk_XXXColorPicker吗，比如RGB数值？当然可以DKNightVersion提供了 DKColorPickerWithRGB(NSUInteger normal, ...)接口来根据色号生成dk_XXXColorPicker。以上我们的Theme有n种，那么我们就需要在在dk_XXXColorPicker里面传入n个代表颜色的参数。
也可以设置不同Theme下的图片。
```
DKImagePicker Examples
imageView.dk_image = DKImagePickerWithImages([UIImage imageNamed:@"white"], [UIImage imageNamed:@"black"]); // => imageView的image在普通模式、夜间模式分别为图片名为white、black代表的图片。
```
也有很多方法来生成dk_image,例如：DKImagePickersWithImageNames(@"white",@"black")，直接根据图片名来生成dk_image，等等。
设置好了不同Theme下的颜色和图片，如下代码即可：
```php
```php
Theme Switch
[DKNightVersionManager sharedManager].themeVersion = DKThemeVersionNormal;// or DKThemeVersionNight => 将当前的主题切换到普通模式或夜间模式。
```
```
### 实现思路
先看下上面的例子中用到的一些属性。
```
UIView+night
// DKColorPicker definition
@property (nonatomic, copy, setter = dk_setBackgroundColorPicker:) DKColorPicker dk_backgroundColorPicker;
// DKImagePicker definition
@property (nonatomic, copy, setter = dk_setTintColorPicker:) DKColorPicker dk_tintColorPicker;
```
```
UIImageView+night
@property (nullable, nonatomic, copy, setter = dk_setImagePicker:) DKImagePicker dk_imagePicker;
```
从使用方法里面可以看到我们在设置给UI控件的DKColorPicker属性赋值时，传入了n个颜色。n个颜色对应了n中Theme，而且他们根据索引一一对应。
DKNightVersionManager是一个用于管理主题的单例。当[DKNightVersionManager sharedManager].themeVersion 发生改变时，也就是当前的Theme发生了个改变。会发一个通知告诉所有的设置过DKColorPicker的UI控件。
UI控件收到通知后去找DKNightVersionManager去拿到当前的Theme，根据Theme更新UI控件的相关属性(backgroundColor，tintColor, textColor, image等)，这边是实现这个功能一个大体的思路。
DKColorPicker是什么？它并不是一个用来存color的数组，它的定义是这样的：
```
DKColorPicker，DKImagePicker
typedef UIColor *(^DKColorPicker)(DKThemeVersion *themeVersion); 
typedef UIImage *(^DKImagePicker)(DKThemeVersion *themeVersion);
```
它是一个block，传入一个我们已经定义好了的Theme，这个block给出一个color，用以更新。DKImagePicker同理。
每个对象都有一个pickers属性
```
pickers property
@interface NSObject ()
@property (nonatomic, strong) NSMutableDictionary<NSString *, DKColorPicker> *pickers;
@end
```
在第一次使用这个属性时，当前对象注册为 DKNightVersionThemeChangingNotification 通知的观察者。pickers属性只有在对象的某个DKColorPicker/DKImagePicker首次被赋值时才会被创建。
```
dk_backgroundColorPicker setter
- (void)dk_setBackgroundColorPicker:(DKColorPicker)picker {
    objc_setAssociatedObject(self, @selector(dk_backgroundColorPicker), picker, OBJC_ASSOCIATION_COPY_NONATOMIC);
    self.backgroundColor = picker(self.dk_manager.themeVersion);
    [self.pickers setValue:[picker copy] forKey:@"setBackgroundColor:"];
}
```
当Theme发生变化时，DKNightVersionManager会发出通知，所有监听DKNightVersionThemeChangingNotification的对象调用night_update方法去更新色值和图片。实现如下：
```
notification action
- (void)night_updateColor {
    [self.pickers enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull selector, DKColorPicker  _Nonnull picker, BOOL * _Nonnull stop) {
        SEL sel = NSSelectorFromString(selector);
        // picker根据Theme拿到color/image值
        id result = picker(self.dk_manager.themeVersion);
        [UIView animateWithDuration:DKNightVersionAnimationDuration
                         animations:^{
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
                             [self performSelector:sel withObject:result];
#pragma clang diagnostic pop
                         }];
    }];
}
```
从dk_backgroundColor的setter方法中可以知道，上面的selector一般为setBackgroundColor:，setTintColor;，setImage:，根据selector生成方法，然后去更新对象的颜色，图片等。这个库已经包含了所有的原生UI控件的color和image属性，通过runtime,category给UI控件添加属性。
作者推荐我们使用如下的方式来创建DKColorPicker。在DKColorTable.txt中，配置我们需要的色值和主题，内容如下：
```
```
NORMAL   NIGHT    RED
#ffffff  #343434  #fafafa BG
#aaaaaa  #313131  #aaaaaa SEP
#0000ff  #ffffff  #fa0000 TINT
#000000  #ffffff  #000000 TEXT
#ffffff  #444444  #ffffff BAR
#f0f0f0  #222222  #dedede HIGHLIGHTED
```
```
NORMAL 、NIGHT、RED分别对应三个主题。
那么通过 DKColorPickerWithKey(BG)，生成对应三个主题的DKColoPicker，并且目前的Theme只能通过修改DKColorTable.txt的文件内容进行管理。
### 总结
- 这个库可以实现我们当前的大多数的需求，目前这个库还不能比较方便的解决富文本的不同主题的不同样式问题，我们可以参照它的实现给需要使用富文本的控件添加DKNightVersionThemeChangingNotification监听，从而根据不同的Theme做出不同的展现，这个思路当然也可以拓展到其他地方，虽然会造成比较强的耦合关系，如不同主题下的不同样式的展现等等。
- 鉴于当前Theme的管理方式，而且DKColorPicker用DKColorPickerWithKey()以外的其他方法创建传入的数值并非动态的，所以以后增加Theme时可能会比较棘手。作者不推荐我们手动创建DKColorPicker,而推荐使用DKColorTable.txt 来进行主题管理， 这样DKColorPicker中包含的色值由DKColorTable.txt中的配置决定，这样会更加方便。
- 它不仅仅支持原生的backgroundColor，tintColor等属性，可以给自定义的控件添加一个你想要的的color/image属性，例如pressedColor，在不同的主题做出不同的展现。
- 这个库的一个比较好的实现我觉得是把Block当做一个属性赋值给对象而不是存储一个数组或字典，然后根据其他变量的变化做出响应的一个思路。
文／断肠人在摸虾（简书作者）
原文链接：http://www.jianshu.com/p/bae45500366b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
