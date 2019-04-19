# iOS 10 创建iMessage App - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月19日 08:31:11[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5999
[原文作者 澳大利亚19岁少年--Davis Allie ----原文地址](http://code.tutsplus.com/tutorials/create-an-imessage-app-in-ios-10--cms-26870)
*时值中秋佳节, 送给出门在外的程序猿们 ! *
**骚年们, 自己写个表情包斗图可否 !**
![](http://upload-images.jianshu.io/upload_images/326255-058975e1761e784c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
斗-seal.svg.png
> 
第一. 介绍
随着**iOS10**的发布,苹果对开发者开放了Messages应用程序,开发人员现在可以创建他们自己的各种类型 并且可以内联iMessage内容的** iMessage apps**,包含简单的表情包到可以交互的复杂界面。
在这个教程中, 我将会为你介绍新的framework--Messages framework, 并且为你展示如何创建属于自己的iMessage Apps;
这个教程的前提是你在OS X El Capitan或以上版本中运行xcode8, 并且有一定的iOS开发经验;如果你是刚开始学习iOS开发, 你可以先学习这个课程[iOS From Scratch With Swift](https://code.tutsplus.com/series/ios-from-scratch-with-swift--cms-909); 在这系列中,你将了解如何开始开发iOS平台的深入的文章和教程。
1.生态系统(Ecosystem)
iMessage APP生态系统开始于一个完整的**App Store**,用户通过Message 应用程序都可以访问。这家**App Store**是完全独立于之前的应用商店,并将只显示**iMessage-related应用**。
iMessage apps的存在在一定程度上延展了印象中的iOS应用程序, 比如表情包(斗图岂能少了表情包)和自定义键盘;最重要的不同在于,这个iMessage App Store
**只存在于系统Messages之中**, 你创建iMessage APP 不需要像创建iOS APP那样显示在手机主屏幕上, iMessage应用程序创建一个扩展的有效空白和无形的iOS应用程序。然而,如果你想开发一个包含iOS和iMessage的应用,你可以创建一个像任何其他类型的扩展的iMessage扩展。
最后要说的是, 这个 iMessage apps 只有效于iOS系统, 但它的内容可以在MacOS和watchOS上显示;此外,watchOS 3的用户能够查看最近使用的表情包,并可以从苹果的手表把这些表情直接直接发给联系人。
> 
第二. 基本的表情包
为了让开发者快速简单的创建一个iMessage 表情表, Xcode提供了一个没有任何代码的模板! 这将是一个伟大的工具,使艺术家与没有编程知识的人都可以创建一个表情包。
使用新的**Messages framework **创建一个表情,你可以创建小的, 不大不小的, 大的表情, 这个尺寸,适用于所有的表情包。你只需要提供最大图像大小为每个表情在你的表情包中,如果需要缩减在某些设备上需要缩放, 系统会替你完成。
虽然没有严格限制,但是苹果建议的表情文件大小:
```cpp
```cpp
1. Small: 100 x 100 pt @3x scale (300 x 300 pixel image)
2. Medium: 136 x 136 pt @3x scale (378 x 378 pixel image)
3. Large: 206 x 206 pt @3x scale (618 x 618 pixel image)
```
```
也有其他的一些限制, 表情包的大小:
```cpp
```cpp
1. 文件中images不可以大于500kb;
2. iamge不可以小于100 x 100 pt (300 x 300 pixels).
3. iamge不可以大于206 x 206 pt (618 x 618 pixels).
4. 图片格式必须是 PNG, APNG, JPEG, GIF ;
```
```
打开Xcode, 创建一个新的工程, 选择**iOS > Application**, 你将会看到** iMessage Application**和**Sticker Pack Application**的可选项, **第一个例子中**, 选择**Sticker Pack Application **
: 
![](http://upload-images.jianshu.io/upload_images/326255-c6520cce8393be3d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
如果你一直照着做,并且想得到一些表情, 你可以从这里下载--[tutorial GitHub repo.](https://github.com/tutsplus/create-an-imessage-app-in-ios-10)
打开项目, 你会发现Xcode的左侧边栏中有个**Stickers.xcstickers **, 这个文件夹中包含了iMessage APP的icon 和表情, 稍后将这些表情拖拽到你的项目中的**Sticker Pack 文件夹中的New Sticker Sequence**;
![](http://upload-images.jianshu.io/upload_images/326255-10fa6b772bad0c98.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
如果你想要添加**动态表情**那么你添加一组图片即可, 你可以点击**Sticker Pack **文件夹, 选择** Add Assets > New Sticker Sequence**这个选项, 在这个表情包中, 你可以再次设置; 
**注意一定要按步骤添加图片**
![](http://upload-images.jianshu.io/upload_images/326255-8753c92ac108cd74.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
3.png
最后, 你如果想要更改表情包得默认名称和大小, 你可以在右侧**Attributes inspector **中更改;
![](http://upload-images.jianshu.io/upload_images/326255-ee39052a501d6579.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
4.png
##### 测试你的表情包
测试表情包, **CMD + R**即可, 也可以点击左上角的运行按钮, 然后模拟器会弹出如下菜单:
![](http://upload-images.jianshu.io/upload_images/326255-379cdda6573b3f56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
5.png
选择**Message**然后点击**run**按钮, 模拟器展示出**Message**应用, 点击**APP store按钮**, 即靠近输入框的按钮;
![](http://upload-images.jianshu.io/upload_images/326255-8d9d8bb39bf56f6d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
6.png
你的表情包将会展示在最前边,你会发现两个表情并且可供使用和发送;
![](http://upload-images.jianshu.io/upload_images/326255-0c36c855106bec97.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
7.png
点击任何一个表情, 它将会被添加在当前的信息中, 
![](http://upload-images.jianshu.io/upload_images/326255-582359c2a21e4b21.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
8.png
正如你所看到的, 在iOS10中你可以快速简单的创建表情包并且不需要任何代码;
> 
第三 自定义标签应用
处于用户的需要,基本的表情应用程序提供的模板可能不完全满足需求;处于这个原因, 苹果也提供了一种方法来使你创建更为复杂的标签应用程序;如果你想一块做这个APP, 那么创建一个项目**CustomStickerPack**, 这次选择**iMessage Application**模板;
创建完这个空的工程, 你会看到类似iOS APP的文件列表, 但多了一个**MessagesExtension**文件夹, 最上边的**CustomStickerPack**文件夹包含**Assets.xcassets**和**Info.plist**文件,重要的是,你提供所有正确的大小的应用程序图标在这个**Assets.xcassets**目录,因为iOS也将使用它,例如用户的存储使用的设置。
![](http://upload-images.jianshu.io/upload_images/326255-fc411bf4de5638d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
9.png
我们集中精力在**MessageExtension**文件下, 
```
```
1. MessagesViewController.swift : iMessage app的程序入口;
2. MainInterface.storyboard: 可视化操作;
3. Assets.xcassets: 图片集合;
4. Info.plist : 配置一些扩展信息;
```
```
在我们的例子中, 我们创建**MSStickerBrowserViewController**这个类;
打开**MessagesViewController.swift **, 你会发现它是**MSMessagesAppViewController**的子类, 而再往上看父类就是**UIViewController**, 这意味着在iMessage APP中都符合UIKit的标准;
这个**MSMessagesAppViewController**类提供了许多回调函数, 你可以覆盖他们, 进一步定制应用程序的功能,但是我们先不需要这些;接下来先配置**MSStickerBrowserViewDataSource**协议;
```
class MessagesViewController: MSMessagesAppViewController, MSStickerBrowserViewDataSource {
    ...
}
```
之前我们可以展示我们的表情, 我们需要添加文件到我们的工程并且加载他们;拖拽之前用过的图片文件夹到工程中;并且确保他们添加到**MessagesExtension**这个目标中,因为这个路径将会作为URL来加载他们, 当加载表情时使用使用更加简单;
接下来在**MessagesViewController**中创建一个**MSSticker**数组来存储我们的表情包, 并写一个方法来总本地加载他们;
如下代码:
```
var stickers = [MSSticker]()
func loadStickers() {
        for i in 1...2 {
//语法变了
            if let url = Bundle.main.url(forResource: "Sticker \(i)", withExtension: "png") {
                do {
                    let sticker = try MSSticker(contentsOfFileURL: url, localizedDescription: "")
                    stickers.append(sticker)
                } catch {
                    print(error)
                }
            }
        }
    }
```
然后在这个类中添加**createStickerBrowser**, 我们初始化一个**MSStickerBrowserViewController**作为根视图, 并设置宽高约束:
```
func createStickerBrowser() {
    let controller = MSStickerBrowserViewController(stickerSize: .large)
    addChildViewController(controller)
    view.addSubview(controller.view)
     //语法变了
    controller.stickerBrowserView.backgroundColor = UIColor.blue
    controller.stickerBrowserView.dataSource = self
    view.topAnchor.constraint(equalTo: controller.view.topAnchor).isActive = true
    view.bottomAnchor.constraint(equalTo: controller.view.bottomAnchor).isActive = true
    view.leftAnchor.constraint(equalTo: controller.view.leftAnchor).isActive = true
    view.rightAnchor.constraint(equalTo: controller.view.rightAnchor).isActive = true
}
```
然后我们需要实现**MSStickerBrowserViewDataSource**必须要实现的代理方法;
```
func numberOfStickers(in stickerBrowserView: MSStickerBrowserView) -> Int {
    return stickers.count
}
func stickerBrowserView(_ stickerBrowserView: MSStickerBrowserView, stickerAt index: Int) -> MSSticker {
    return stickers[index]
}
```
最后, 所有的代码都已经完成, 替换**viewDidLoad**中的代码:
```
override func viewDidLoad() {
    super.viewDidLoad()
    loadStickers()
    createStickerBrowser()
}
```
像之前那样运行APP, 一旦一切都完成加载, 应该会看到一个完全相同的屏幕除了蓝色背景, 除了蓝色的背景色;
![](http://upload-images.jianshu.io/upload_images/326255-ae1933b7b973d2df.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
10.png
在这个教程中, 我们只是加载了应用程序中的图片,但好的是我们可以自定义标签应用程序了, 然而, 你可否加载网络服务器呢? 在展示你的**MSStickerBrowserViewController**之前,可以通过使用别的**view controllers**来实现的; 再继续一步吧!
> 
自定义APP
在最后一节, 我们将创建一个非常基本的iMessage应用程序来创建一个独特的消息。;
如果你愿意一块继续完成这个目标, 打开xcode创建一个**MessageApp**, 打开**MainInterface.storyboard**,删除默认的**label**, 添加一个**stepper**和一个**button**, 如下图:
![](http://upload-images.jianshu.io/upload_images/326255-0dce497fdcdbe7d9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
11.png
请注意,为了让你的iMessage APP的视图位置正确展示在所有的设备中, 你需要添加布局约束, 在这个例子中我已经设置了stepper在视图中间, 这个button在视图的下侧;
然后, 选择**stepper**打开**Attributes inspector**, 改变最小最大值为0和10:
![](http://upload-images.jianshu.io/upload_images/326255-bf8a7f349311fcb5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
12.png
接着在** Assistant editor **中打开**MessagesViewController.swift**并链接起来;
```
@IBOutlet weak var stepper: UIStepper!
@IBAction func didPress(button sender: AnyObject) {
}
```
接下来, 写代码的时刻到啦, 第一步我介绍一些用到的类:
```
```
1. MSConversation: 代表了当前打开的对话。可以使用这个类来操作一些步骤,例如通过插入消息或当前选择的信息;
2. MSMessage: 代表单个消息,判断是否由你插入对话或已经存在的对话;
3. MSMessageTemplateLayout: 创建一个消息气泡,如下图, 里边有许多属性和空间来提供给你来自定义message;
```
```
![](http://upload-images.jianshu.io/upload_images/326255-44be9e1f44d4c30c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
13.png
特别提醒, 左上角的空间是展示你的icon, 所有的属性都是可选的, 提供任何标题字符串将摆脱底部部分的布局。
在**MessagesViewController.swift**中修改如下代码:
在这个方法中, 我们将当前的**stepper**的数值显示到圆形的label中, 然后将label放在**UIImage**对象中, 以便于我们可以在message中捕获到;
```
func createImageForMessage() -> UIImage? {
        let background = UIView(frame: CGRect(x: 0, y: 0, width: 300, height: 300))
        background.backgroundColor = UIColor.white
        let label = UILabel(frame: CGRect(x: 75, y: 75, width: 150, height: 150))
        label.font = UIFont.systemFont(ofSize: 56.0)
        label.backgroundColor = UIColor.red
        label.textColor = UIColor.white
        label.text = "\(Int(stepper.value))"
        label.textAlignment = .center
        label.layer.cornerRadius = label.frame.size.width/2.0
        label.clipsToBounds = true
        background.addSubview(label)
        background.frame.origin = CGPoint(x: view.frame.size.width, y: view.frame.size.height)
        view.addSubview(background)
        UIGraphicsBeginImageContextWithOptions(background.frame.size, false, UIScreen.main.scale)
        background.drawHierarchy(in: background.bounds, afterScreenUpdates: true)
        let image = UIGraphicsGetImageFromCurrentImageContext()
        UIGraphicsEndImageContext()
        background.removeFromSuperview()
        return image
    }
```
接下来在**didPress(button:) **中修改为如下代码:
首先创建message的布局, 并且设置**image和caption**, 接着我们创建**MSMessage**对象并插入到会话中;
```
@IBAction func didPress(button sender: AnyObject) {
        if let image = createImageForMessage(), let conversation = activeConversation {
            let layout = MSMessageTemplateLayout()
            layout.image = image
            layout.caption = "Stepper Value"
            let message = MSMessage()
            message.layout = layout
            message.url = URL(string: "emptyURL")
            conversation.insert(message, completionHandler: { (error) in
                print(error)
            })
        }
    }
```
要注意, 在iMessage中自定义message必须要设置**layout和URL**, 这个URL是链接某个web页面的一些内容, 进而MacOS用户可以看到iMessage的内容, 在这个例子中, 我们只是创建了简单的字符URL;
最后我们将这个message插入到当前活动的会话中, 调用这个方法不会发送信息, 相反,它把你的信息在用户的输入字段,以便他们可以发送。
再次运行APP, 你会看到如下界面:
![](http://upload-images.jianshu.io/upload_images/326255-89b6ecadd95780b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
14.png
你点击**Create Message**按钮, 你会发现message的位置在输入框中间, 并且可以被发送;
![](http://upload-images.jianshu.io/upload_images/326255-eb39e7add45882b3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
15.png
> 
总结
在这个教程中, 我将你介绍了iOS10中新的**Messages framework**, 允许你创建表情包和iMessage应用程序, 我们重写了基础的类, 包括:**MSStickerBroeserViewcontroller, MSConversation, MSMessage, MStemplateMessageLayout**;
这个框架许多API来开发自己的iMessage APP, 更进一步, 我会推荐你查看苹果的文档--[Messages Framework Reference.](https://developer.apple.com/reference/messages)

文／Mugworts（简书作者）
原文链接：http://www.jianshu.com/p/8d8844f621b2
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
