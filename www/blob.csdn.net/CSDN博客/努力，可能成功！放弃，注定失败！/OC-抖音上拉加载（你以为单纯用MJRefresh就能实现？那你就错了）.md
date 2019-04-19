# OC-抖音上拉加载（你以为单纯用MJRefresh就能实现？那你就错了） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年07月18日 16:09:15[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：184
先上[DEMO](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fqlyx%2FdouyinRefresh)记得star哦
![](https://upload-images.jianshu.io/upload_images/2989469-42c7d4ef99e62703.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/268)
效果图
之前实现了抖音下拉刷新效果之后就没再继续研究，想着上拉加载随便集成一下MJRefresh就可以了，很简单嘛，等需要的时候再加进去就好了。
直到某一天有个小伙伴跟我说添加上拉加载有问题，我就不信了，怎么可能呢，自己试了试还真是不少坑。
现在想来之前真的是图样图森破，你以为你以为的就是你以为的？实践出真知，万事都不能想当然，只有自己真的去操作了才能明白。
下面咱们来一起实现一下：
看之前先熟悉一下[OC-仿抖音下拉刷新](https://www.jianshu.com/p/b68813c540c6),操作是基于之前的demo的
1、添加上拉加载-使用延时模拟请求
```
self.tableView.mj_footer = [MJRefreshBackNormalFooter footerWithRefreshingBlock:^{
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [weakSelf getMoreData];
        });
        
    }];
```
加上mj_footer之后运行起来，发现根本触发不了mj，因为之前代码里默认关闭了tableView的bounces，滑到最后不打开弹性效果是没办法往上拖拽触发mj。
一开始的想法是判断当前是否播放到列表的最后一个cell，如果是就打开，不是就关闭。后来想想其实只有第一个cell的时候才需要关闭弹性效果，就改成了下面代码：
```
//此方法每次cell播放的时候都会掉用，所以写在了这里，实时判断
- (void)tableView:(UITableView *)tableView willPlayVideoOnCell:(UITableViewCell *)cell {
    VideoTableViewCell *Cell = (VideoTableViewCell *)cell;
    Cell.playButton.selected = NO;
    playIndex = (int)Cell.playButton.tag;
    [cell.jp_videoPlayView jp_resumeMutePlayWithURL:cell.jp_videoURL
                                 bufferingIndicator:nil
                                       progressView:nil
                            configurationCompletion:^(UIView * _Nonnull view, JPVideoPlayerModel * _Nonnull playerModel) {
                                view.jp_muted = NO;
                            }];
    if (Cell.playButton.tag==0) {
        //列表第一个cell时关闭
        self.tableView.bounces = NO;
    }else
        self.tableView.bounces = YES;
}
```
2、pagingEnabled和mj的矛盾
按照第一步，调整bounces的开关后，确实能够触发mj了，但是由于pagingEnabled的回弹效果，每次上拉触发mj后页面又回滚回去，没有正常上拉加载时footer悬停然后转圈的效果，导致用户根本看不见mj的加载状态，并且加载完成后reloadData，cell的位置会错乱，不是刚好整屏整屏的显示，有偏移。
1）先解决下回滚的问题
既然pagingEnabled影响了加载效果的显示，那是不是也可以通过手段来控制pagingEnabled的开关呢？
于是我开始监控tableView的滑动，通过playIndex记录当前播放到第几个cell,当播放到最后一个cell，用户又继续上滑说明用户正在上拉加载，此时是关闭pagingEnabled的最佳时机，具体代码如下，有注释：
```
- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    [self.tableView jp_scrollViewDidScroll];
    int index= (int)self.tableView.contentOffset.y/kHeight;
    //scroll是与整屏相比的偏移量，肯定是正的
    float scroll = self.tableView.contentOffset.y- index*kHeight;
    //与上一个滑动点比较，区分上滑还是下滑
    float offset = self.tableView.contentOffset.y- oldOffset.y;
    //记录当前tableView.contentOffset
    oldOffset = self.tableView.contentOffset;
    if (offset>0) {
        //上拉-44是mj_footer的高度，当拖拽超过44的时候会触发mj
        if (playIndex==_tableView.items.count-1&&scroll>44) {
            if (_updating==NO) {
                //判断是否正在刷新，正在刷新就不再进行如下设置，以免重复加载
                _updating = YES;
                //进到这里说明用户正在上拉加载，触发mj,此时要关闭翻页功能否则页面回弹mj_footer就看不到了，setContentOffset也无效
                self.tableView.pagingEnabled = NO;
                //给tableView设置一个固定的Offset，往上偏移点，将footer展示出来，要大于44才会触发footer
                [self.tableView setContentOffset:CGPointMake(0, index*kHeight+50) animated:NO];
                [self.tableView.mj_footer beginRefreshing];
            } 
        }
    }
}
```
然后在加载结束后再打开pagingEnabled即可，这样就能看到mj的加载状态了，抖音加载结束后会自动滚动到下一个cell播放，所以我也做了此操作，但是自动滚动后就会发现当前cell总是向上偏移了一部分，露出当前cell的下一个cell,如图：
![](https://upload-images.jianshu.io/upload_images/2989469-b5555488452abf96.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/452)
红框内偏移高度
2）解决偏移问题
我打印了cell自动滚动后self.tableView.contentOffset.y，与整屏偏移量是44，也就是说页面刚好上移了44像素，那不就是footer的高度吗？于是我在自动滚动后让tableView.contentOffset.y再下移44，矫正过来，发现好使，代码如下：
```
-(void)getMoreData
{
    _updating=NO;
    [self.tableView.mj_footer endRefreshing];
    int index = (int)self.pathStrings.count;
    [self.pathStrings addObjectsFromArray:@[@"http://p11s9kqxf.bkt.clouddn.com/coder.mp4",@"http://p11s9kqxf.bkt.clouddn.com/cat.mp4",@"http://p11s9kqxf.bkt.clouddn.com/coder.mp4",@"http://p11s9kqxf.bkt.clouddn.com/cat.mp4"]];
    [self.tableView reloadData];
    //滚动到下一个cell
    [self.tableView scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:index inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
    NSLog(@"1w:%.f",self.tableView.contentOffset.y);
    //mjfooter高度是44，上拉加载时页面会向上偏移44像素，数据加载完毕后需要将contentOffset复位
    self.tableView.contentOffset =CGPointMake(0, self.tableView.contentOffset.y-44);
    NSLog(@"%.f",self.tableView.contentOffset.y);
    //让cell开始播放
    VideoTableViewCell *cell = [self.tableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:index inSection:0]];
    [self tableView:self.tableView willPlayVideoOnCell:cell];
    //刷新结束，开启翻页功能
    self.tableView.pagingEnabled = YES;
}
```
3、ios11适配问题
根据上面，小偏移问题解决了，可是我继续再上滑查看后面cell的时候，诡异的事情发生了，后面的偏移更大了，而且滑动到底部就再也拽不动了：
![](https://upload-images.jianshu.io/upload_images/2989469-538aeb9477ea9e56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/457)
红框内偏移部分有点大
这就有点傻眼了，这偏移的过分了点，我轻轻滑动页面，打印tableView减速后contentOffset.y的数值，发现数值没问题，刚好是屏幕高度的整数倍，一点都没偏移。
这是什么情况？翻页效果、回弹效果都没问题，contentOffset.y也对着呢，就是cell显示的不正常，难道是pagingEnabled在反复开关、设置contentOffset.y之后有点神经错乱了？
于是我尝试了各种办法，彻底关闭pagingEnabled正常滑动cell、修改cell的高度不让它全屏在进行测试、通过监控滑动过程自己模拟翻页效果、翻看老项目非全屏cell滑动加载效果……所有测试失败后我又回退到一开始的样子
啊啊啊啊，总之我折腾了一天，感觉要疯的节奏，眼看快下班了，要不真机试试吧，说不定是模拟器有问题呢？（当时我的手机是11.3，刚升级，xcode是9.2不支持，升级xcode太费时间了，所以一直在用模拟器测试）
从同事那借了个低版本的手机，10.3的，运行后，完美，拖拽，刷新，加载，复位一点问题都没有，很顺畅。（此时的我心中一万个草泥马奔腾而过……模拟器的锅，让我浪费了一天）
没问题了，就开心的下班了。
第二天到了公司，想着要不给xcode升个级吧，总借手机用怪麻烦的，然后是漫长的等待升级。升级结束后，把程序在我手机上运行了一下，发现昨天的诡异问题又出现了。就隔了一个晚上，怎么就不好使了？谁动我代码了？
又跑去同事那借了手机运行，没问题，在我手机上再次运行，有问题。同样的代码，设备一样，效果却不一样。唯一的差异在于系统，我是11.3，他是10.3。等等，难道是ios11的适配没做好？
翻了翻代码，果然没适配，于是把之前适配ios11的代码搬过来，一运行，彻底没问题了。
```
self.estimatedRowHeight = 0;
        self.estimatedSectionHeaderHeight = 0;
        self.estimatedSectionFooterHeight = 0;
        //适配ios11自适应上导航 安全区域
        self.separatorStyle = UITableViewCellSeparatorStyleNone;
        SEL selector = NSSelectorFromString(@"setContentInsetAdjustmentBehavior:");
        if ([self respondsToSelector:selector]) {
            IMP imp = [self methodForSelector:selector];
            void (*func)(id, SEL, NSInteger) = (void *)imp;
            func(self, selector, 2);
            
        }
```
适配ios11的代码有那么多行，其实有效果的是estimatedRowHeight这个。具体请参考链接：[关于iOS11中estimatedRowHeight](https://link.jianshu.com?t=http%3A%2F%2Fwww.cocoachina.com%2Fios%2F20171109%2F21103.html)看完你就明白了
4、细节整理
确定没问题了之后，我将代码整理了一番，整理好之后又测试了一遍，将延时时间调大了观察效果，当页面停留在加载状态的时候我手指下滑了一下，发现没有翻页回弹的效果了，一下子下滑了好几个cell。
哦，对了，我在加载的时候关闭了pagingEnabled，得在加载结束后才会打开，可是此时如果用户像我一样下滑了不就露馅了了么？本着求知心态，我将手机设置为3G网络，快速滑动抖音首页，出现加载后，我又下滑观察，抖音是有翻页效果的。摸清套路后我开始思考我该怎么做？
审查代码后发现scrollViewDidScroll中我只判断了用户上拉，没有处理下滑，那就再加上下滑判断。
整理后代码如下
```
- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
    [self.tableView jp_scrollViewDidScroll];
    int index= (int)self.tableView.contentOffset.y/kHeight;
    //scroll是与整屏相比的偏移量，肯定是正的
    float scroll = self.tableView.contentOffset.y- index*kHeight;
    //与上一个滑动点比较，区分上滑还是下滑
    float offset = self.tableView.contentOffset.y- oldOffset.y;
    //记录当前tableView.contentOffset
    oldOffset = self.tableView.contentOffset;
    if (offset>0) {
        //上拉-44是mj_footer的高度，当拖拽超过44的时候会触发mj
        if (playIndex==_tableView.items.count-1&&scroll>44) {
            if (_tableView.updating==NO) {
                //判断是否正在刷新，正在刷新就不再进行如下设置，以免重复加载
                _tableView.updating = YES;
                //进到这里说明用户正在上拉加载，触发mj,此时要关闭翻页功能否则页面回弹mj_footer就看不到了，setContentOffset也无效
                self.tableView.pagingEnabled = NO;
                //给tableView设置一个固定的Offset，往上偏移点，将footer展示出来，要大于44才会触发footer
                [self.tableView setContentOffset:CGPointMake(0, index*kHeight+50) animated:NO];
                [self.tableView.mj_footer beginRefreshing];
            }
            
        }
    }
    else if (offset<0)
    {
        if (_tableView.updating==YES) {
            //如果用户上拉加载时，又进行下滑操作，就要打开翻页功能（可能加载时间长用户不想等又往上翻之前的cell）-这种情况少见但不排除，不做此操作的话，将请求延时十秒就会看到区别，但一旦用户有这种操作就会有闪屏问题，即用户在第10个cell上拉加载了，然后又下滑倒第5个cell，当拿到返回数据之后页面会从5自动滚动到第11个cell，造成闪屏，但在3G网络下经测试抖音也是这样，故就这样吧
            self.tableView.pagingEnabled = YES;
        }
        
    }
}
```
到此就算搞定上拉加载了。
如果你使用时有什么问题，请留言。
感谢NewPan大神的[JPVideoPlayer](https://www.jianshu.com/p/5de1965824d4)实现了抖音的自动播放
如果对您有帮助记得点赞收藏哦^ _ ^
作者：乔兰伊雪
链接：https://www.jianshu.com/p/313d56c2854b
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
