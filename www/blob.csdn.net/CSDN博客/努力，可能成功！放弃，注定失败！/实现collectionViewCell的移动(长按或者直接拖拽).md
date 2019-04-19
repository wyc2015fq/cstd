# 实现collectionViewCell的移动(长按或者直接拖拽) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月04日 09:28:59[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2902
最近在实现类似网易新闻的首页滑块的编辑效果: 长按后进入编辑界面, 然后可以通过拖拽实现cell的移动, 研究后发现两种实现方式: 第一种是直接利用系统提供的UICollectionView API实现移动, 不过只能在iOS9上面使用. 所以这里就介绍另外一种方式.
源码效果示例:
![](http://upload-images.jianshu.io/upload_images/1271831-fedc212e01f79347.gif?imageMogr2/auto-orient/strip)
更新示例.gif
### 原理部分
- 添加一个长按手势到UICollectionView上
- 在这个手势的selector中通过获取到当前手势在collectionVIew的location来获取到一个indexPath, 如果这个indexPath是有效的, 那么就可以通过这个indexPath获取到对应的cell.
- 将获取到的cell截图, 然后将这个cell隐藏, 通过设置这个截图的frame使得这个截图跟随手指同步移动, 如果截图移动到了另外一个cell的位置, 则通过调用collectionView的方法将这两个cell交换位置, 同时需要更新collectionView的dataSource.
### 实现部分
- 在collectionView上添加一个长按手势
```
let longGes = UILongPressGestureRecognizer(target: self, action: #selector(self.longPressd(_:)))
     collectionView.addGestureRecognizer(longGes)
```
2 在selector中处理手势的响应
- 2.1 获取当前手势的位置
`      let location = ges.locationInView(self.collectionView)`
- 
2.2 获取这个位置对应的在collectionView中的indexPath, 注意这个indexPath可能为nil(比如手指没有在cell的位置上时)
```
// 当手指的位置不在collectionView的cell范围内时为nil
      let notSureIndexPath = self.collectionView.indexPathForItemAtPoint(location)
```
- 
2.3 通过当前手势的状态不同的处理
![](http://upload-images.jianshu.io/upload_images/1271831-efdcb24c75580a7d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
遍历状态.png
- 
2.3.1 case .Began 手势开始的时候
```cpp
```cpp
if let indexPath = notSureIndexPath { // 获取到的indexPath是有效的, 可以放心使用
            // a.
            currentIndexPath = indexPath
            // b. 
            let cell = collectionView.cellForItemAtIndexPath(indexPath)!
            // c.
            snapedImageView = getTheCellSnap(cell)
            deltaSize = CGSize(width: location.x - cell.frame.origin.x, height: location.y - cell.frame.origin.y)
            // d.
            snapedImageView.center = cell.center
            snapedImageView.transform = CGAffineTransformMakeScale(1.1, 1.1)
            // e.
            cell.alpha = 0.0
            // f.
            collectionView.addSubview(snapedImageView)
}
```
```
a. 记录下当前的indexPath以便于在手指移动的过程中进入.Changed状态的时候使用
b. 通过这个indexPath获取到对应的cell
c. 获取到这个cell截图
d. 并且设置截图的初始位置
e. 隐藏当前的cell
f. 将截图添加到collectionView中
- 2.3.2 case .Changed 手指在移动的时候
```
// a. 
            if snapedImageView == nil { return }
            // b. 
            snapedImageView.frame.origin.x = location.x - deltaSize.width
            snapedImageView.frame.origin.y = location.y - deltaSize.height
            // c.
            if let newIndexPath = notSureIndexPath, let oldIndexPath = currentIndexPath {
                if newIndexPath != oldIndexPath && newIndexPath.section == oldIndexPath.section {// 只在同一组中移动
            // d.
                    collectionView.moveItemAtIndexPath(oldIndexPath, toIndexPath: newIndexPath)
                    // 更新dataSource
            // e. 
                    let cell = collectionView.cellForItemAtIndexPath(newIndexPath)
                    cell?.alpha = 0.0
            // f.
                    currentIndexPath = newIndexPath
                }
            }
```
a. 如果在began状态中没有获取到截图直接返回
b. 设置截图的位置, 以达到和手指同步移动
c. 如果新获取到的indexPath有效并且和原来的不相同
d. 移动cell, 更新dataSource
e. 设置新的cell的属性
f. 更新当前的indexPath
- 2.3.3 case .End 手指离开屏幕的时候
```
if let oldIndexPath = currentIndexPath {
                // a.
                let cell = collectionView.cellForItemAtIndexPath(oldIndexPath)!
                // b. 
                UIView.animateWithDuration(0.25, animations: {[unowned self] in
                        self.snapedImageView.transform = CGAffineTransformIdentity
                        self.snapedImageView.frame = cell.frame
                    }, completion: {[unowned self] (_) in
                        self.snapedImageView.removeFromSuperview()
                        self.snapedImageView = nil
                        self.currentIndexPath = nil
                        cell.alpha = 1.0
                })
            }
```
a. 获取到当前移动完成的cell
b. 使用动画移除截图并且设置当前的移动完成的cell的属性
### 到目前位置, 设置好collectionView的datasource和delegate方法后就可以实现以下的效果了
![](http://upload-images.jianshu.io/upload_images/1271831-315bf022de6ce1ce.gif?imageMogr2/auto-orient/strip)
示例效果.gif
> 
详细请移步[源码](https://github.com/jasnig/ScrollPageView), 如果您觉得有帮助,不妨给个star鼓励一下, 欢迎关注
文／ZeroJ（简书作者）
原文链接：http://www.jianshu.com/p/76925fda6048
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
