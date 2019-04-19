# iOS微信朋友圈与摇一摇 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月04日 08:35:45[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：392
本Demo为练手小项目,主要是熟悉目前主流APP的架构模式.此项目中采用MVC设计模式,纯代码和少许XIB方式实现.主要实现了朋友圈功能和摇一摇功能.
预览效果:
![](http://upload-images.jianshu.io/upload_images/2276378-5ecef9e5da956ecf.gif?imageMogr2/auto-orient/strip)
WeChat_Demo.gif
#### 主要重点
###### 1.整体架构
利用UITabBarController和UINavigationController配合实现.其中要注意定义基类,方便整体上的管理,例如对UINavigationController头部的颜色,字体和渲染颜色等设置.以及对UITabBarController的底部的渲染等.
```
[self.navigationBar setBackgroundImage:[UIImage imageNamed:@"Dimensional-_Code_Bg"] forBarMetrics:UIBarMetricsDefault];      
[self.navigationBar setTitleTextAttributes:@{
                                             NSForegroundColorAttributeName:[UIColor whiteColor]
                                             }];
[self.navigationBar setTintColor:[UIColor whiteColor]];
```
###### 2.发现界面和我的界面
利用UITableViewController和Plist文件实现界面的展示.实现过程中有采用数据模型或直接利用字典等方式.这里的实现比较简单,就不多说啦.
```
- (instancetype)initWithDict:(NSDictionary *)dict{
if (self = [super init]) {
    [self setValuesForKeysWithDictionary:dict];
}
return self;
}
+ (instancetype)pictureWithDict:(NSDictionary *)dict{
return [[self alloc] initWithDict:dict];
}
```
###### 3.朋友圈功能的实现
这里面主要的难点在于朋友圈首页的下拉刷新效果的实现,和选择照片页的状态重用问题,以及照片的传递和代理的实现等.
朋友圈首页的下拉刷新效果:主要利用transform属性和scrollview的多种滚动状态.
```
- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView{
   self.dragging = YES;
}
- (void)scrollViewDidScroll:(UIScrollView *)scrollView{
if (self.num == 0) {
    self.num ++;
    return;
}
CGFloat offsetY = scrollView.contentOffset.y;
CGFloat angle = -offsetY * M_PI / 30;
if (self.dragging == YES) {
    if (offsetY <= 110) {
        self.containerView.y = 10 + offsetY;
    }
}else {
    if (self.currentY < 120) {
        self.containerView.y = 10 + offsetY;
    }
}
self.activityView.transform = CGAffineTransformMakeRotation(angle);
}
- (void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate{
self.dragging = NO;
CGFloat currentY = self.containerView.y;
self.currentY = currentY;
}
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView{
[UIView animateWithDuration:0.25 animations:^{
    self.containerView.frame = CGRectMake(15, 120, 30, 30);
    self.activityView.transform = CGAffineTransformMakeRotation(2 * M_PI);
}];
}
```
其中照片的展示是采用UICollectionViewController来实现的.没有直接调用系统的相册,因此加大了难度.自定义了cell,并采用了代理方式来实现类与类之间的通信.
```
@protocol YYPictureCellDelegate <NSObject>
@optional
- (void)pictureCell:(YYPictureCell *)cell withDidClickBtn:(UIButton *)btn;
@end
- (IBAction)clickSureBtn:(UIButton *)sender {
if ([self.delegate respondsToSelector:@selector(pictureCell:withDidClickBtn:)]) {
    [self.delegate pictureCell:self withDidClickBtn:sender];
}
}
- (void)pictureCell:(YYPictureCell *)cell withDidClickBtn:(UIButton *)btn{
if ((self.selectedBtn.count == 9) && (!btn.isSelected)) {
    UIAlertView *alert = [[UIAlertView alloc]initWithTitle:nil message:@"最多选取9张照片哦,亲!" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles: nil];
    [alert show];
    return;
}
btn.selected = !btn.isSelected;
NSIndexPath *indexPath = [self.collectionView indexPathForCell:cell];
YYPictureModel *model = self.dataArray[indexPath.row];
if (btn.isSelected) {
    model.clickedBtn = YES;
    [self.selectedBtn addObject:btn];
    [self.selImageArray addObject:model];
} else{
    model.clickedBtn = NO;
    [self.selectedBtn removeObject:btn];
    [self.selImageArray removeObject:model];
}
if (self.selectedBtn.count > 0) {
    self.doneBtn.enabled = YES;
    self.doneBtn.selected = YES;
    self.previewBtn.enabled = YES;
    self.previewBtn.selected = YES;
}else {
    self.doneBtn.enabled = NO;
    self.doneBtn.selected = NO;
    self.previewBtn.enabled = NO;
    self.previewBtn.selected = NO;
}   
}
```
###### 4.摇一摇功能的实现
摇一摇功能的本身实现十分简单,就是调用系统的两个方法即可.难点在于动画效果.其实这里的动画效果也不是很难.主要是计算有点复杂.可能是我在网上搜索到的素材有点不合适.导致要考虑各个控件的frame问题...
```
//实现摇一摇功能
- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event{
self.upLine.hidden = NO;
self.downLine.hidden = NO;
[UIView animateWithDuration:0.6 animations:^{
    self.upImageView.y -= 60;
    self.upLine.y -= 60;
    self.downImageView.y += 60;
    self.downLine.y += 60;
}completion:^(BOOL finished) {
    [UIView animateWithDuration:0.6 animations:^{
        self.upImageView.y += 60;
        self.upLine.y += 60;
        self.downImageView.y -= 60;
        self.downLine.y -= 60;
    } completion:^(BOOL finished) {
        self.upLine.hidden = YES;
        self.downLine.hidden = YES;
        //弹出搜索框
        [self showSearchView];
        [self.searchView performSelector:@selector(removeFromSuperview) withObject:nil afterDelay:2.4];
    }];
}];
}
//摇一摇结束后
- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event{
}
```
### 结束语:
在此,还有感谢提供素材的网友们.此demo主要是把功能实现,代码方面没有做过多的优化,但是易懂,呵呵...有待后续添加其他功能和代码的优化.需要源代码的请戳这里:[https://github.com/yaomars/WeChat_Demo](https://github.com/yaomars/WeChat_Demo)
文／YYSky（简书作者）
原文链接：http://www.jianshu.com/p/fe8d577c596b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
