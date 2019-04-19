# iOS 模仿淘宝的首页UI - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月19日 14:44:22[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5062
要看UICollectionView的可以关了
全程都是tableview加button和autolayout
最近用autolayout真是越来越顺手了
Level:菜鸟
环境：Xcode7.3 iOS9.3
效果：
![](http://upload-images.jianshu.io/upload_images/537940-576154d90baabd60.gif?imageMogr2/auto-orient/strip)
6s.gif
顶部轮播就不弄了。
好用的轮播传送：
[https://github.com/gsdios/SDCycleScrollView](https://github.com/gsdios/SDCycleScrollView)
图片我都是截图的command+shift+4
主要是学习autolayout
注意的几个点：
1.任何自定义view都可以用storyboard里的cell做，只要用到tableview时，都如下取到自定义view，不需要重新自定义xib（主要是使用xib之后，在放回controller里要设置frame）：
```
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    static NSString *cellIdentifier = @"sectionHeadView";
    UITableViewCell *headView =[tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if(!headView){
        headView=[[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifier];
    }
    return headView;
}
```
2.倒计时
传送：[https://github.com/search?q=倒计时&type=Repositories&utf8=✓](https://github.com/search?q=%E5%80%92%E8%AE%A1%E6%97%B6&type=Repositories&utf8=%E2%9C%93)
![](http://upload-images.jianshu.io/upload_images/537940-6a337102bd0ced42.gif?imageMogr2/auto-orient/strip)
5s.gif
![](http://upload-images.jianshu.io/upload_images/537940-83e95f75d422854b.gif?imageMogr2/auto-orient/strip)
4s.gif
源码：
[https://github.com/icharlie2014/PLTaobaoIndex](https://github.com/icharlie2014/PLTaobaoIndex)
文／造夢（简书作者）
原文链接：http://www.jianshu.com/p/6343ac896028
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
