# iOS Flexbox 布局优化 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年03月21日 22:01:55[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：113
本文来自:[https://juejin.im/post/5ab1c1aa518825555d472194](https://juejin.im/post/5ab1c1aa518825555d472194)
### Frame AutoLayout
iOS 开发中给系统给我们提供了两种布局，frame 布局 和 AutoLayout 布局，虽然frame布局性能很高 但面对现在越来越丰富的iPhone尺寸，为了适配各种机型需要写更多的无营养代码，造成代码量增多。AutoLayout是iOS6 之后被引入的，由于其蹩脚、冗长的语法，直到出现一些优秀的开源第三方库，自动布局 才真正意义上被应用到iOS 开发中。AutoLayout将我们从坐标中的点 面 思考带到约束条件的思考，一条约束就是一个线性等式或不等式，一个视图需要添加多个约束才能确定位置，这些约束最后组成一组线性方程组，再由Cassowary算法算出各个视图的frame，所以性能的关键问题是解线性方程组的时间复杂度。
![](https://user-gold-cdn.xitu.io/2018/3/21/162465d5875518e9?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
![](https://user-gold-cdn.xitu.io/2018/3/21/162465d587454ede?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
### Flexbox
Flexbox 是W3C在2009年提出的一种新的前端页面布局，目前，它已经得到了所有浏览器的支持。而最早将这一页面布局方案引入iOS开发中的是开源库 [AsyncDisplayKit](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Ffacebook%2FAsyncDisplayKit%2F)。但随着[React Native](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Ffacebook%2Freact-native)与[Weex](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Falibaba%2Fweex%2F)在动态化领域的兴起，
 让iOS开发越来越多的接触到Flexbox 页面布局。
### Yoga
[Yoga](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Ffacebook%2Fyoga) 是由C实现的Flexbox布局引擎，目前已经被用于在React Native 和 Weex 等开源项目中，性能和稳定性得到了很好的验证。但不足的是Yoga只实现了W3C标准的一个子集。算法逻辑如下图所示：
![](https://user-gold-cdn.xitu.io/2018/3/21/162465d5873fabe7?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
![](https://user-gold-cdn.xitu.io/2018/3/21/162465d588cbab62?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
### 基于Yoga 引擎的Flexbox 布局优化
由于iOS 开发中frame布局的繁琐及Auto Layout的性能问题，我们在开发中引入了Flexbox 布局，这样不仅提高了布局的便利性和性能，而且可以与项目中的React Native 和Weex 保持一致的开发体验。为了充分提高Flexbox 布局的性能和易用性我们维护一个开源的扩展FlexBoxLayout，支持链式调用，布局方便，虚拟视图Div，TableView 支持自动高度、布局缓存，contentView缓存，和自动cache 失效机制ScrollView 支持自适应contentSize，异步计算布局。
[github链接请戳我](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FLPD-iOS%2FFlexBoxLayout)。
### 链式调用
```
- (void)layoutView {
  
  
  [_titleLabel fb_makeLayout:^(FBLayout *layout) {
    layout.margin.equalToEdgeInsets(UIEdgeInsetsMake(10, 0, 0, 0)).wrapContent();
  }];
  
  
  [_contentLabel fb_makeLayout:^(FBLayout *layout) {
    layout.margin.equalToEdgeInsets(UIEdgeInsetsMake(10, 0, 0, 0)).wrapContent();
  }];
  
  [_contentImageView fb_makeLayout:^(FBLayout *layout) {
    layout.margin.equalToEdgeInsets(UIEdgeInsetsMake(10, 0, 0, 0)).wrapContent();
  }];
  
  [_usernameLabel fb_makeLayout:^(FBLayout *layout) {
    layout.wrapContent().flexGrow.equalTo(@(1.0));
  }];
 
  [_timeLabel fb_makeLayout:^(FBLayout *layout) {
      layout.wrapContent().flexGrow.equalTo(@(1.0));
  }];
  
  FBLayoutDiv *div = [FBLayoutDiv layoutDivWithFlexDirection:FBFlexDirectionRow ];
  [div fb_makeLayout:^(FBLayout *layout) {
    layout.flexDirection.equalTo(@(FBFlexDirectionRow)).justifyContent.equalTo(@(FBJustifySpaceBetween)).alignItems.equalTo(@(FBAlignFlexStart)).margin.equalToEdgeInsets(UIEdgeInsetsMake(10, 0, 0, 0)).children(@[_usernameLabel,_timeLabel]);
  }];
  
  [self fb_makeLayout:^(FBLayout *layout) {
    layout.flexDirection.equalTo(@(FBFlexDirectionColumn)).margin.equalToEdgeInsets(UIEdgeInsetsMake(0, 15, 0, 15)).alignItems.equalTo(@(FBAlignFlexStart)).children(@[_titleLabel,_contentLabel,_contentImageView,div]);
  }];
  
}
```
#### UITableView Flexbox 布局滑动性能优化
UITableViewCell 自动计算高度 和 UITableView滑动性能一直是一个重要的性能优化。FlexBoxLayout能自动计算cell高度，优化了滑动性能，使UITableView在滑动时的帧率接近60FPS.
```
[self.tableView fb_setCellContnetViewBlockForIndexPath:^UIView *(NSIndexPath *indexPath) {
    return [[FBFeedView alloc]initWithModel:weakSelf.sections[indexPath.section][indexPath.row]];
  }];
  ....
  - (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return [self.tableView fb_heightForIndexPath:indexPath];
  }
  - (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    return [self.tableView fb_cellForIndexPath:indexPath];
  }
```
上面的代码，做了以下几件事：
- 开发者只需要关注Cell 的ContentView会自动重利用cell，开发者只需要实现cell的contentView具体内容.
- 自动计算高度会根据Flexbox布局自动计算出高度
- 高度缓存机制计算出的高度会自动进行缓存，当滚动cell，重利用cell，后面对应的index path cell的高度的询问都会命中缓存，减少cpu的计算任务。![](https://user-gold-cdn.xitu.io/2018/3/21/162465d588db6967?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
- 高度缓存失效机制当数据源发生变化时，调用以下几个api
```
reloadData
    insertSections:withRowAnimation:
    deleteSections:withRowAnimation:
    reloadSections:withRowAnimation:
    moveSection:toSection:
    insertRowsAtIndexPaths:withRowAnimation:
    deleteRowsAtIndexPaths:withRowAnimation:
    reloadRowsAtIndexPaths:withRowAnimation:
    moveRowAtIndexPath:toIndexPath:
```
刷新页面时，会对已有的高度进行失效处理，并重新计算新的缓存高度。
[Demo](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FLPD-iOS%2FCSSLayout) 界面的刷新一直接近60FPS
![](https://user-gold-cdn.xitu.io/2018/3/21/162465d588fdb45b?imageslim)
性能对比简单测试：
![优化之前](https://user-gold-cdn.xitu.io/2018/3/21/162465d5a7da8f4d?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
![优化之后](https://user-gold-cdn.xitu.io/2018/3/21/162465d5a92c26df?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
#### UIScrollView contentSize 自动计算
为了更好的适应不同的屏幕尺寸，我们在开发中会经常用到scrollview进行适配，针对flexbox布局scrollview的contentsize大小计算问题，我们也进行了封装处理。給scrollview增加了一个contendiv 虚拟容器，所有的子视图只需要添加到contendiv中进行flexbox布局。
```
FBLayoutDiv *root = [FBLayoutDiv layoutDivWithFlexDirection:FBFlexDirectionRow
                                              justifyContent:FBJustifySpaceAround
                                                  alignItems:FBAlignCenter
                                                    children:@[div1,div2]];
  contentView.fb_contentDiv = root;
```
设置UIScrollView的fb_contentDiv属性，当Flexbox布局计算完成应用到视图上时，在layoutSubviews函数中UIScrollView的contentSize 会被设置大小。
#### Flexbox 异步计算
现在的iOS设备都是多核的，为了充分利用设备的多核能力，将布局的计算放在后台线程，异步计算，同时为了减少过多线程切换的开销，在MainRunLoop即将休眠时把计算好的布局应用到视图上。
![](https://user-gold-cdn.xitu.io/2018/3/21/162465d5a9e92efb?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)
RunLoop 在运行时，当切换时会发送通知：
```
typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) {
    kCFRunLoopEntry         = (1UL << 0), // 即将进入Loop
    kCFRunLoopBeforeTimers  = (1UL << 1), // 即将处理 Timer
    kCFRunLoopBeforeSources = (1UL << 2), // 即将处理 Source
    kCFRunLoopBeforeWaiting = (1UL << 5), // 即将进入休眠
    kCFRunLoopAfterWaiting  = (1UL << 6), // 刚从休眠中唤醒
    kCFRunLoopExit          = (1UL << 7), // 即将退出Loop
};
```
创建布局计算完成事件源，当布局完成，发送事件源，唤醒MainRunLoop, 当MainRunLoop处理完成了所有事件，马上要休眠时,批量处理计算好的布局。
```
CFRunLoopObserverRef observer;
  
  CFRunLoopRef runLoop = CFRunLoopGetMain();
  
  CFOptionFlags activities = (kCFRunLoopBeforeWaiting | kCFRunLoopExit);
  
  observer = CFRunLoopObserverCreate(NULL,
                                     activities,
                                     YES,
                                     INT_MAX,
                                     &_messageGroupRunLoopObserverCallback,
                                     NULL);
  
  if (observer) {
    CFRunLoopAddObserver(runLoop, observer, kCFRunLoopCommonModes);
    CFRelease(observer);
  }
  
  CFRunLoopSourceContext  *sourceContext = calloc(1, sizeof(CFRunLoopSourceContext));
  
  sourceContext->perform = &sourceContextCallBackLog;
  
   _runLoopSource = CFRunLoopSourceCreate(NULL, 0, sourceContext);
  
  if (_runLoopSource) {
    CFRunLoopAddSource(runLoop, _runLoopSource, kCFRunLoopCommonModes);
  }
```
当RunLoop马上要休眠时，在_messageGroupRunLoopObserverCallback函数处理布局应用。
### 使用FlexboxLayout
如果你能觉得这个工具能够帮到你，他是容易整合到项目的：
```bash
pod "FlexBoxLayout"
```
GitHub地址：[github.com/LPD-iOS/Fle…](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2FLPD-iOS%2FFlexBoxLayout)
作者：饿了么物流移动团队
链接：https://juejin.im/post/5ab1c1aa518825555d472194
来源：掘金
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
