# iOS10 iMessage 带来了你需要的MessageExtension - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月29日 20:22:53[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：13495
# iOS10 iMessage 带来了你需要的MessageExtension
字数2778
阅读119评论0喜欢6
# 项目要求
Xcode8 + iOS10
现在都是测试版，去开发者网站可以下载。如果没有的话，这个，那个，就看下我的配图吧，哈哈
# 前言
iOS10测试版已经出来一个多月了，一直对这个东西很感兴趣。但是查了好多，基本都是用Sticker的那个方面直接放图片使用的。到现在我是没有搜索到用代码编写的iMessage的项目，所以用了一天把这个项目写出来（前几天看过几次概念）。不理解的就去官方api文档看。现在是把苹果说的功能大概写了出来。有不对之处，请大家指出。
[demo地址](https://github.com/sunyunfei/iOS10-iMessage.git)
# 正文
在这里引用一下别人写的iMessage的解释，感觉解释的很好，我也直接引用了：
```cpp
```cpp
在iOS中新增了两种iMessage的方式，1.内置表情包,2.iMessage应用
1.内置表情包(Sticker Packs)
可以通过在Xcode中新建Sticker Pack Application来创建。这种方式可以简单地通过添加图片来在iMessage中添加表情包。添加的贴纸需要满足一下条件
图片类型必须是 png、apng、gif或者jpeg
文件大小必须 小于500K
图片大小必须在 100100 到 206206 之间
需要注意的是：必须要永远提供 @3x 大小的图片(即 300300 到 618618)。系统可以根据当前设备通过 runtime 自动调整图片来呈现 @2x 和 @1x
系统能够自适应的展示贴纸，所以为了更好的展示贴纸，最好提供的贴纸是以下三种大小的类型
小型 100*100
中型 136*136
大型 206*206
2.iMessage应用
iMessage app使用完整的框架和Message app进行交互。使用iMessage app能够
在消息应用内呈现一个自定义的用户交互界面。 使用 MSMessagesAppViewController
创建一个自定义或者动态的表情包浏览器。使用 MSStickerBrowserViewController
添加文本、表情、或者媒体文件到消息应用的文本输入框。使用 MSConversation
创建带有特定的应用数据交互的消息。使用 MSMessage
更新可以相互影响的消息(例如，创建游戏或者可以合作的应用)。使用 MSSession
```
```
内置表情包我就不在说了，我看了看网上有很好的例子也很简单。
MSMessagesAppViewController说白了就是显示表情的vc，和普通的vc一样，只是他多了一些方法，和AppDelegate的差不多。
我用了一个collectionview去显示图片，但是有一点需要注意，如果你直接用UIImageView去直接显示图片也是可以的，运行在message也是显示的，但是你却无法进行正常的拖拽，点击操作，因为你只是给了一个UIImageView而已。在这里要用到一个专门为显示iMessage里面图片用的类MSStickerView，他是UIView的一个子类，里面的属性很少
```
@interface MSStickerView : UIView
/*!
 @method   initWithFrame:sticker:
 @abstract   Initializes a MSStickerView with a frame and a MSSticker conforming object to display.
 */
- (instancetype)initWithFrame:(CGRect)frame sticker:(nullable MSSticker *)sticker;
/*!
 @property   sticker
 @abstract   The MSSticker object to display.
 @discussion Set this property to nil to remove the current sticker. Setting the
 sticker property does not change the size of a MSStickerView. Call sizeToFit to
 adjust the size of the view to match the sticker.
 */
@property (nonatomic, strong, readwrite, nullable) MSSticker *sticker;
/*!
 @property   animationDuration
 @abstract   The amount of time it takes to go through one cycle of the sticker animation.
 */
@property(nonatomic, readonly) NSTimeInterval animationDuration;
/*!
 @method    startAnimating
 @abstract  Starts animating the sticker in the receiver.
 @discussion This method always starts the animation from the first frame.
 */
-(void) startAnimating;
/*!
 @method    stopAnimating
 @abstract  Stops animating the sticker in the receiver.
 */
-(void) stopAnimating;
/*!
 @method   isAnimating
 @abstract   Returns a Boolean value indicating whether the animation is running.
 */
- (BOOL)isAnimating;
```
一目了然，sticker我的理解就是特殊的数据，让MSStickerView根据这个数据进行相应的显示操作。至于stopAnimating那几个方法我暂时没有用到。
UICollectionView代码块:
```
#import "StickerCollectionView.h"
#import "StickerCell.h"
#import "CommonCell.h"
static NSString * const STICKERCELL = @"StickerCell";
static NSString * const COMMONCELL = @"CommonCell";
@interface StickerCollectionView()
@property(nonatomic,strong)NSMutableArray *stickerArray;//数据
@end
@implementation StickerCollectionView
- (instancetype)initWithCoder:(NSCoder *)aDecoder{
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self registerNib:[UINib nibWithNibName:STICKERCELL bundle:nil] forCellWithReuseIdentifier:STICKERCELL];//注册单元格
        [self registerNib:[UINib nibWithNibName:COMMONCELL bundle:nil] forCellWithReuseIdentifier:COMMONCELL];
    }
    return self;
}
- (void)awakeFromNib{
    [super awakeFromNib];
    self.delegate = self;
    self.dataSource = self;
}
//数据加载
- (NSMutableArray *)stickerArray{
    if (!_stickerArray) {
        _stickerArray = [NSMutableArray array];
        NSArray *dataArray = @[@"1",@"2",@"3",@"4",@"5",@"6",@"7"];
        //把图片转换为mssticker类的形式，用于显示
        [dataArray enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            NSURL *urlStr = [[NSBundle mainBundle]URLForResource:obj withExtension:@"jpg"];
            MSSticker *placeSticker = [[MSSticker alloc]initWithContentsOfFileURL:urlStr localizedDescription:obj error:nil];
            [_stickerArray addObject:placeSticker];
        }];
    }
    return _stickerArray;
}
#pragma mark ------九宫格代理
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return self.stickerArray.count + 4;
}
- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView cellForItemAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.row >= self.stickerArray.count) {
        CommonCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:COMMONCELL forIndexPath:indexPath];
        cell.imageView.image = [UIImage imageNamed:[NSString stringWithFormat:@"other_%lu",indexPath.row - self.stickerArray.count + 1]];
        return cell;
    }else{
        StickerCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:STICKERCELL forIndexPath:indexPath];
        cell.sticker.sticker = self.stickerArray[indexPath.row];//把mssticker类型的数据赋予msstickerview里面的sticker属性
        return cell;
    }
}
- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    if (indexPath.row == self.stickerArray.count) {
        //文字
        if (self.event1Block) {
            self.event1Block();
        }
    }else if (indexPath.row == self.stickerArray.count + 1){
        if (self.event2Block) {
            self.event2Block();
        }
        //音频
    }else if (indexPath.row == self.stickerArray.count + 2){
        if (self.event3Block) {
            self.event3Block();
        }
        //详细图
    }else if (indexPath.row == self.stickerArray.count + 3){
        if (self.event4Block) {
            self.event4Block();
        }
        //多图
    }
}
```
在这里面你会发现数据数组不再是简单的图片名字数据了，是一个MSSticker类型的数据。
```
- (nullable instancetype)initWithContentsOfFileURL:(NSURL *)fileURL localizedDescription:(NSString *)localizedDescription error:(NSError * _Nullable *)error NS_DESIGNATED_INITIALIZER;
/*!
 @property   imageFileURL
 @abstract   The file URL to the Sticker was initialized with.
 */
@property (nonatomic, strong, readonly) NSURL *imageFileURL;
/*!
 @property   localizedDescription
 @abstract   A succinct localized string describing the sticker.
 */
@property (nonatomic, copy, readonly) NSString *localizedDescription;
```
这个类里面的东西更加的少了，一个创建方法，一个NSURL，一个NSString。而且后面两个都是只读类型的，所以你基本不用操作。直接使用它唯一的方法就行了，localizedDescription要尽量设置的不同(官方好像说的是一个iMessage App里面的每一个MSSticker 的localizedDescription定义好了就不变了，除非卸载这个App.我解决的这个不是很好，英语太差)
把我们的基本数据转换成这个类之后然后传给MSStickerView类进行擦操作就行。
` cell.sticker.sticker = self.stickerArray[indexPath.row];//把mssticker类型的数据赋予msstickerview里面的sticker属性`
至于他是怎么显示出来的，这个不用你担心。
显示效果如下：
![](http://upload-images.jianshu.io/upload_images/1210430-8090646760034457.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果一
然后点击发送：
![](http://upload-images.jianshu.io/upload_images/1210430-3e2cb0e6c708f2b4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果二
![](http://upload-images.jianshu.io/upload_images/1210430-38b363f6b07652a4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果三
这是基本的显示效果，iMessage还可以显示规定的文字，音频，详细的图片...
这些效果的实现是在MSMessagesAppViewController里面，MSMessagesAppViewController有个属性activeConversation，属于MSConversation类
```php
```php
/*!
 @method     insertMessage:localizedChangeDescription:completionHandler:
 @abstract   Stages provided the MSMessage for sending.
 @discussion This method inserts a MSMessage object into the Messages input field,
 Subsequent calls to this method will replace any existing message on the input field. 
 If the message was successfully inserted on the input field, the completion handler
 will be called with a nil error parameter otherwise the error parameter will be
 populated with an NSError object describing the failure.
 @param      message            The MSMessage instance describing the message to be sent.
 @param      completionHandler  A completion handler called when the message has been staged or if there was an error.
 */
- (void)insertMessage:(MSMessage *)message completionHandler:(nullable void (^)(NSError * _Nullable))completionHandler;
/*!
 @method     insertSticker:completionHandler:
 @abstract   The sticker is inserted into the Messages.app input field.
 @param      sticker            The sticker to be inserted.
 @param      completionHandler  A completion handler called when the insert is complete.
 */
- (void)insertSticker:(MSSticker *)sticker completionHandler:(nullable void (^)(NSError * _Nullable))completionHandler;
/*!
 @method     insertText:completionHandler:
 @abstract   The NSString instance provided in the text parameter is inserted into the Messages.app input field.
 @param      text               The text to be inserted.
 @param      completionHandler  A completion handler called when the insert is complete.
 */
- (void)insertText:(NSString *)text completionHandler:(nullable void (^)(NSError * _Nullable))completionHandler;
/*!
 @method     insertAttachment:withAlternateFilename:completionHandler:
 @abstract   The NSURL instance provided in the URL parameter is inserted into the Messages.app
 input field. This must be a file URL.
 @param      URL                The URL to the media file to be inserted.
 @param      filename           If you supply a string here, the message UI uses it for the attachment. Use an alternate filename to better describe the attachment or to make the name more readable.
 @param      completionHandler  A completion handler called when the insert is complete.
 */
- (void)insertAttachment:(NSURL *)URL withAlternateFilename:(nullable NSString *)filename completionHandler:(nullable void (^)(NSError * _Nullable))completionHandler;
```
```
这是MSConversation类的四个方法，也是他的全部方法，用于插入各种对用的内容。
## 文字
```
self.collectionView.event1Block = ^{
        //发送文字
        [weakSelf.activeConversation insertText:@"Fuck the world if you are rich,otherwise fuck youself!" completionHandler:^(NSError * _Nullable error) {
            //使用的时候的回调,做自己想做的事情
        }];
```
效果：
![](http://upload-images.jianshu.io/upload_images/1210430-918eb42cc0de6bce.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
文字
## 音频
```
//发送音频
        NSString *path = [[NSBundle mainBundle]pathForResource:@"music" ofType:@"mp3"];
        NSURL *url = [NSURL fileURLWithPath:path];
        [self.activeConversation insertAttachment:url withAlternateFilename:@"音乐" completionHandler:^(NSError * _Nullable error) {
            //使用的时候的回调,做自己想做的事情
        }];
```
效果：
![](http://upload-images.jianshu.io/upload_images/1210430-b3860ab5c3b1fafa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
音频
![](http://upload-images.jianshu.io/upload_images/1210430-74132495131521c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
音频
## 详细的MSMessage
```
//详细图片
        MSMessage *message = [[MSMessage alloc]init];
        MSMessageTemplateLayout *layout = [[MSMessageTemplateLayout alloc]init];
        layout.imageTitle = @"蜡笔";
        layout.imageSubtitle = @"蜡笔小新的故事";
        layout.caption = @"蜡笔";
        layout.subcaption = @"蜡笔sub";
        layout.trailingCaption = @"蜡笔";
        layout.trailingSubcaption = @"蜡笔sub";
        NSString *path2 = [[NSBundle mainBundle]pathForResource:@"1" ofType:@"jpg"];
        layout.mediaFileURL = [NSURL fileURLWithPath:path2];
        message.layout = layout;
        [self.activeConversation insertMessage:message completionHandler:^(NSError * error) {
            //使用的时候的回调,做自己想做的事情
        }];
```
效果:
![](http://upload-images.jianshu.io/upload_images/1210430-acc5e21e8d2b4b49.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
详细图片
这是我用UICollectionView实现的布局，其实iMessage也给了一个类似的可以实现的view和vc：MSStickerBrowserView，MSStickerBrowserViewController
```
@interface MSStickerBrowserViewController : UIViewController <MSStickerBrowserViewDataSource>
/*!
 @method   initWithStickerSize:
 @abstract Initializes a MSStickerBrowserViewController and configures it's MSStickerBrowserView with the provided sticker size class.
 */
- (instancetype)initWithStickerSize:(MSStickerSize)stickerSize NS_DESIGNATED_INITIALIZER;
/*!
 @property   stickerBrowserView
 @abstract   Returns the sticker browser view managed by the controller object.
 */
@property (nonatomic, strong, readonly) MSStickerBrowserView *stickerBrowserView;
/*!
 * @abstract Controls the size of the stickers are displayed at in the sticker browser view.
 */
@property (nonatomic, readonly) MSStickerSize stickerSize;
```
MSStickerBrowserViewController里面就这一点东西，一个构造，两个只读属性。
要创建MSStickerBrowserViewController一定要实现代理的两个方法，与表很像的两个代理：
```php
```php
/*!
 * @abstract Returns the number of Stickers that the sticker browser should show.
 * @param stickerBrowserView The sticker browser view .
 * @result The number of stickers.
 */
- (NSInteger)numberOfStickersInStickerBrowserView:(MSStickerBrowserView *)stickerBrowserView;
/*!
 * @abstract Returns the sticker that the sticker browser should show in the browser.
 * @param stickerBrowserView The sticker browser view.
 * @param index The index of the sticker to show.
 * @result A MSSticker object.
 */
- (MSSticker *)stickerBrowserView:(MSStickerBrowserView *)stickerBrowserView stickerAtIndex:(NSInteger)index;
```
```
就是个数和显示的内容,我实现的是两行显示，我不知道可以可以改变这个:
```
//数据加载
- (NSMutableArray *)browArray{
    if (!_browArray) {
        _browArray = [NSMutableArray array];
        NSArray *dataArray = @[@"dt_1",@"dt_2",@"dt_3",@"dt_4",@"dt_5",@"dt_6",@"dt_7",@"dt_8",@"dt_9",@"dt_10",@"dt_11",@"dt_12",@"dt_13"];
        //把图片转换为mssticker类的形式，用于显示
        [dataArray enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            NSURL *urlStr = [[NSBundle mainBundle]URLForResource:obj withExtension:@"gif"];
            MSSticker *placeSticker = [[MSSticker alloc]initWithContentsOfFileURL:urlStr localizedDescription:obj error:nil];
            [_browArray addObject:placeSticker];
        }];
    }
    return _browArray;
}
#pragma mark----代理
- (NSInteger)numberOfStickersInStickerBrowserView:(MSStickerBrowserView *)stickerBrowserView{
    return self.browArray.count;
}
- (MSSticker *)stickerBrowserView:(MSStickerBrowserView *)stickerBrowserView stickerAtIndex:(NSInteger)index{
    return self.browArray[index];
}
```
效果：
![](http://upload-images.jianshu.io/upload_images/1210430-14e58bb666726b7a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果
# 结语
基本说完了，我没有详细的介绍每个类用法，里面什么属性方法，这个我感觉放到demo你一跑起来看下代码就应该明白。其实iMessage很简单，我写的demo只是简单的把它基本使用了，还有好多等待你的开发。如果你感觉对你有一点帮助，请给个github星星，谢了.
文／（简书作者）
原文链接：http://www.jianshu.com/p/73b9f8552626
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
