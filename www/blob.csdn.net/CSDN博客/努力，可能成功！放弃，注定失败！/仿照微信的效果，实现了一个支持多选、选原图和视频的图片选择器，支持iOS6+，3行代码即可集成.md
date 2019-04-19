# 仿照微信的效果，实现了一个支持多选、选原图和视频的图片选择器，支持iOS6+，3行代码即可集成 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月01日 14:48:22[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：56
**重要提示**：
- **1.9.0版本已发布，移除了"prefs:root="的调用，这个API已经被列为私有API，请大家尽快升级。**其它同样使用了该API的库大家可以检查下，比如著名的[SVProgressHUD](http://www.cocoachina.com/bbs/read.php?tid=1722166)
- 如果你发现了Bug，请先尝试更新到最新版，此前的版本可能存在一些bug。如果你已经是最新版了，请在github上提一个issue，我看到了会尽快处理哈~
前段时间空余时间比较多，打算尝试做一个图片选择器出来，仔细对比了很多自定义了图片选择器的应用，感觉最喜欢微信的界面效果，当然微博的功能更强大，还支持了LivePhoto，所以打算模仿微信的界面效果，瞄着微博的功能去做一个图片选择器出来。
# 一. TZImagePickerController简介
这个图片选择器还没达到我理想中的效果，但是最近工作开始忙起来了，所以有一些功能放在以后加入吧。目前这个图片选择器：
> - 支持图片多选、选原图和视频；
- 支持预览图片和视频；
- 适配了iOS6~10；
总体上跟微信的照片选择器界面和功能都差不多一样，当然我这个弱不少...效果图如下：
> - TZPhotoPickerController, 照片选择控制器
![](https://upload-images.jianshu.io/upload_images/1171135-8753fb9628146a56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
> 
2.TZPhotoPreviewController. 照片预览控制器
![](https://upload-images.jianshu.io/upload_images/1171135-55bca6df5ee486d7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
> 
3.TZVideoPlayerControlle. 视频预览控制器
![](https://upload-images.jianshu.io/upload_images/1171135-a95008791acff827.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
对照片选择器要求不是很高的应用来说，这个图片选择器还算是够用了，这段时间考察了github上若干个支持多选的图片选择器，界面效果和功能都比这个差些，可能是有更好的我没发现吧...
# 二. 如何集成
该图片选择器支持CocoaPods， 在你的Podfile文件中加入`pod 'TZImagePickerController' 即可集成。
如果你喜欢手动导入，到github下载代码后，将TZImagePickerController文件夹拖入到你的项目中，再`#import "TZImagePickerController.h"`即可。
集成这个图片选择器，最少只需要三行代码，用起来还是很方便，可以节省开发者的时间，把更多的时间和精力花在业务逻辑和自己想钻研的技术点上去。
```
TZImagePickerController *imagePickerVc = [[TZImagePickerController alloc] initWithMaxImagesCount:9 delegate:self];
// You can get the photos by block, the same as by delegate.
// 你可以通过block或者代理，来得到用户选择的照片.
[imagePickerVc setDidFinishPickingPhotosHandle:^(NSArray<UIImage *> *photos, NSArray *assets, BOOL isSelectOriginalPhoto) {
}];
[self presentViewController:imagePickerVc animated:YES completion:nil];
```
代码截图如下：
![](https://upload-images.jianshu.io/upload_images/1171135-2e02cba0df764fab.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
# 三. 相关类的介绍
1.`TZImageManager`：唯一import了和的类，向外界提供有获取相册/照片/视频资源的接口，在里面每个方法内部会判断系统版本，从而选择是从AssetsLibrary库「iOS9以前可用」还是从Photos库里获取照片资源「iOS8以后可用」。
2.`TZImagePickerController`：导航栏控制器，通过改变该控制器的一些属性来达到你想要的效果，比如设置maxImagesCount属性来约束用户最大可选图片张数，默认为9张；设置allowPickingOriginalPhoto属性为NO不允许用户选择视频，默认是YES；设置allowPickingVideo属性为NO不允许用户选择原图，默认是YES。
3.`TZPhotoPickerController`：照片选择控制器，一个collectionView每行4个cell展示图片。
4.`TZPhotoPreviewController`：照片预览控制器，用的是collectionView而不是scrollView，所以不用处理重用逻辑，然而图片的间隙我还没弄出来...以后再弄吧...(已出...)
5.`TZVideoPlayerController`：视频预览控制器，简单的播放/暂停/发送视频功能，用的是AVPlayer来播放。进度条下次有空再加上吧...
6.`TZAssetModel`：装有一个图片/视频/音频资源的模型，对应一个图片/视频Cell「`TZAssetCell`」，资源属性名为asset，可以从`TZImageManager`取出资源对象「图片/视频/音频」。
7.`TZAlbumModel`：装有一个相册资源的模型，对应一个相册Cell「`TZAlbumCell`」，资源属性名为result，可以从`TZImageManager`取出所含有的图片资源数组。
# 四. 微信图片选择器的一些技术点
优点实在太多了，说两个我发现的小缺点吧。
> 
小缺点1：在相册列表页，空的cell也会显示，出现许多空的分割线。「也可能是微信的产品或iOS喜欢这个调调...」
- 我的解决方案：加上这一句`_tableView.tableFooterView = [[UIView alloc] init];`会不再显示空的cell。当然也可以隐藏系统的分割线，自己加一个高0.5~1像素的View作为分割线。
> 
小缺点2：在照片选择器页，微信的设计是不允许同时选择视频和照片的，但是在照片预览页面，用户却可以同时选中视频和照片，此时微信会默认将视频作为图片发送，而没有做一个提示。并且，如果用户选择了发送原图，此时视频虽然也被选中出来了，但是会一直发送失败。
- 我的解决方案：在照片预览页面，如果用户在选择了图片的情况下又选择了视频，提示用户会默认将视频做图片发送。并且在图片浏览器页面，如果预览到了一个视频，则将原图按钮隐藏，在计算原图大小的方法里也会过滤掉视频资源。
当然微信的图片选择器是很优秀的，比如我今天在用iOS6系统的4s真机测试时，发现第一次弹出图片选择器时，如果相机胶卷相册图片数大于1000，我的这个会有比较明显的卡顿，而微信的却依然流畅...考虑到这个罕见的iOS6系统4s机子不是我的、工作要开始忙了、毕竟这只是4s等原因，这个优化就等下次有空再来研究吧...（1.4.2版本更新：这里已优化，是AssetsLibrary的一个坑...）
# 五. 更新记录
**1.4.5版本**（2016.05.23）
在照片列表页新增了拍照按钮，可以全局记录哪个相册已选中了多少张图片，预览控制器可以在外界打开。同时Demo页面也做了一些优化，可以直接删除选中的照片、可以对照片进行长按排序等。当然期间也修复了许多小bug，表现更加好了。
最值得一提的是，1.4.5版本的性能大幅提升了，在我的iOS9.3.2系统6s设备上（870张照片），平均滑动帧数在58左右，滑动十分流畅，在iOS7.0.4的4s设备上(124张照片)，平均滑动帧数在57左右，也十分流畅。经过对比，和QQ的图片选择器滑动帧数表现基本一致，都十分流畅，同时都强于微信的图片选择器。微信的图片选择器，在快速滑动的时候明显感到有一丝卡顿，通过Core Animation查看发现，微信的图片选择器在我的6s设备下帧数平均约52左右，好几次甚至低于50，在4s设备上则表现更糟一些。下面贴上帧数测试截图，大家也可以测试一下~
![](https://upload-images.jianshu.io/upload_images/1171135-7d9d554d016d41e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/540/format/webp)
TZImagePickerController的帧数表现 (iOS9.3.2 6s).png
![](https://upload-images.jianshu.io/upload_images/1171135-d409921fb8915512.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/553/format/webp)
TZImagePickerController的表现 (iOS7.0.4 4s).png
![](https://upload-images.jianshu.io/upload_images/1171135-71628210a4be29de.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/535/format/webp)
QQ图片选择器的帧数表现 (iOS9.3.2 6s).png
![](https://upload-images.jianshu.io/upload_images/1171135-585a3b00bfe3dac0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/623/format/webp)
QQ图片选择器的帧数表现 (iOS7.0.4 4s).png
![](https://upload-images.jianshu.io/upload_images/1171135-93852bd4a64a44bb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/619/format/webp)
微信图片选择器的帧数表现 (iOS9.3.2 6s).png
**1.5.0版本**（2016.06.22）
1.可把拍照按钮放在外面了,可以设置弹出一个sheet提示用户选择相册或相机；
2.新增了sortAscendingByModificationDate属性，默认为YES，设置为NO时，照片会按修改时间降序，拍照按钮会在第一个；
3.优化了Demo页面的UI，新增6个开关控件，可方便地更改各个设置来打开选择器；
4.修复了若干bug；
**1.6.3版本**（2016.08.29）
1.支持单选模式，将maxImagesCount设置为1即可。
2.支持国际化，目前支持中文和英文。
3.图片名字可从外界传，无须再改动源码。
4.图片预览界面照片之间留20像素的间隙，优化预览效果。
5.图片选择页可自定义图片展示列数。
**最近更新**
2.2.2 修复minPhotoWidthSelectable不生效的问题， 使用@available消除警告
2.1.8 优化gif图播放的体验，加入iCloud同步进度条；新增notScaleImage属性，设置为YES时内部不去缩放图片
2.1.6 新增allowCameraLocation属性，默认为YES，置为NO时不会在照相/摄像时定位，修复一个序号紊乱的bug
2.1.4 新增多个页面和组件的样式自定义block，允许自定义绝大多数UI样式
2.1.2 新增showPhotoCannotSelectLayer属性，当已选照片张数达到最大可选张数时，可像微信一样让其它照片显示一个提示不可选的浮层
2.1.1 新增是否显示图片选中序号的属性，优化一些细节
2.1.0.3 新增拍摄视频功能，优化一些细节
2.0.0.6 优化自定义languageBundle的支持，加入使用示例
2.0.0.5 优化性能，提高选择器打开速度，新增越南语支持
2.0.0.2 新增繁体语言，可设置首选语言，国际化支持更强大；优化一些细节
1.9.8 支持Carthage，优化一些细节
1.9.6 优化视频预览和gif预览页toolbar在iPhoneX上的样式
1.9.4 适配iPhoneX
**1.9.0 移除"prefs:root="的调用，这个API已经被列为私有API，请大家尽快升级**
1.8.4 加入横竖屏适配；支持视频/gif多选；支持视频和照片一起选
1.8.1 新增2个代理方法，支持由上层来决定相册/照片的显示与否
1.8.0 修复若干bug, 提升流畅度
...
1.7.7 支持GIF图片的播放和选择
1.7.6 支持对共享相册和同步相册的显示
1.7.5 允许不进入预览页面直接选择照片
1.7.4 支持单选模式下裁剪照片，支持任意矩形和圆形裁剪框
1.7.3 优化iCloud照片的显示与选择
...
# 六. 常见问题
**Q：pod search TZImagePickerController 搜索出来的不是最新版本**
A：需要在终端执行cd转换文件路径命令退回到Desktop，然后执行pod setup命令更新本地spec缓存（可能需要几分钟）,然后再搜索就可以了
**Q：拍照后照片保存失败**
A：请参考issue481：[https://github.com/banchichen/TZImagePickerController/issues/481](https://github.com/banchichen/TZImagePickerController/issues/481) 的信息排查，若还有问题请直接在issue内评论
**Q：photos数组图片不是原图，如何获取原图？**
A：请参考issue457的解释：[https://github.com/banchichen/TZImagePickerController/issues/457](https://github.com/banchichen/TZImagePickerController/issues/457)
**Q：系统语言是中文/英文，界面上却有部分相册名字、返回按钮显示成了英文/中文？**
A：请参考 [https://github.com/banchichen/TZImagePickerController/issues/443](https://github.com/banchichen/TZImagePickerController/issues/443) 和 [https://github.com/banchichen/TZImagePickerController/issues/929](https://github.com/banchichen/TZImagePickerController/issues/929)
**Q：预览界面能否支持传入NSURL、UIImage对象？**
A：排期中，优先级高
**Q：设置可选视频的最大/最小时长？照片的最小/最大尺寸？不符合要求的不显示**
A：可以的，参照Demo的isAssetCanSelect方法实现。我会返回asset出来，显示与否你来决定，注意这个是一个同步方法，对于需要根据asset去异步获取的信息如视频的大小、视频是否存在iCloud里来过滤的，无法做到。如果真要这样做，相册打开速度会变慢，你需要改我源码。
**Q：可否支持横屏？**
A：1.8.4版本已支持
**Q：可否加入视频拍摄功能？**
A：2.1.0.3版本已支持
**Q：可否加入视频多选功能？**
A：1.8.4版本已支持
**Q：可否让视频和图片允许一起选？**
A：1.8.4版本已支持
**Q：可否增加微信编辑图片的功能？**
A：考虑下，优先级低
**Q：是否有QQ/微信群？**
A：有QQ群：778723997
**Q：想提交一个PR？**
A：请先加QQ群和我确认下，避免同时改动同一处内容。**一个PR请只修复1个问题，变动内容越少越好**。
**Q：demo在真机上跑不起来？**
A：1、team选你自己的；2、bundleId也改成你自己的或改成一个不会和别人重复的。可参考[简书的这篇博客](https://www.jianshu.com/p/cbe59138fca6)
# 七. 其它
更具体的信息大家感兴趣的话去看代码吧，如果发现了bug请提一个issue，当然最好你给顺便解决了并提一个pull request，这样最帅了对吧...
代码地址：[https://github.com/banchichen/TZImagePickerController](https://github.com/banchichen/TZImagePickerController)
欢迎使用，觉得不错请给一个小小的star鼓励一下~
作者：半尺尘
链接：https://www.jianshu.com/p/1975411a31bb
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
