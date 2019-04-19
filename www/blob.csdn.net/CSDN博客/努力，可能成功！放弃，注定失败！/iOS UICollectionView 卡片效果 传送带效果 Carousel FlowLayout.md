# iOS UICollectionView 卡片效果 传送带效果 Carousel FlowLayout - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年03月20日 10:50:58[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：20
# 场景
在我们的app中需要一个类似影院传送带式选择电影场次的控件，效果如下：
![](https://upload-images.jianshu.io/upload_images/2027951-999193b853330d64.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/378/format/webp)
CarouselCenter.gif
# 实现思路
- 控件选择
	看控件特征，是一个可滚动的长列表，在iOS中一般都使用`UICollectionView`来展现，这里我们也选择它。
- 布局选择
`UICollectionView`中每个`item`的显示样式都通过`UICollectionViewLayout`来控制，这里明显是一个“流式布局”，我们可以选择`UICollectionViewFlowLayout`来定制样式。
- 布局控制
	在上面的显示效果中，我们需要控制两个点：
	- 缩放效果
		
```
// 该方法指定UICollectionView的每个item滚动到相应rect的显示效果（UICollectionViewLayoutAttributes，包含尺寸、透明度等信息）
open func layoutAttributesForElements(in rect: CGRect) -> [UICollectionViewLayoutAttributes]?
```
- 滚动结束，定位到中间位置
		
```
// 该方法指定UICollectionView滚动到的目标位置
// a layout can return the content offset to be applied during transition or update animations
open func targetContentOffset(forProposedContentOffset proposedContentOffset: CGPoint) -> CGPoint
```
# 实现细节
- 缩放效果
```
override open func layoutAttributesForElements(in rect: CGRect) -> [UICollectionViewLayoutAttributes]? {
    guard let superAttributes = super.layoutAttributesForElements(in: rect), let attributes = NSArray(array: superAttributes, copyItems: true) as? [UICollectionViewLayoutAttributes] else {
        return nil
    }
    return attributes.map({ self.transformLayoutAttributes($0) })
}
fileprivate func transformLayoutAttributes(_ attributes: UICollectionViewLayoutAttributes) -> UICollectionViewLayoutAttributes {
    guard let collectionView = self.collectionView else { return attributes }
    let isHorizontal = (self.scrollDirection == .horizontal)
    let collectionCenter = isHorizontal ? collectionView.frame.size.width / 2 : collectionView.frame.size.height / 2
    let offset = isHorizontal ? collectionView.contentOffset.x : collectionView.contentOffset.y
    let normalizedCenter = (isHorizontal ? attributes.center.x : attributes.center.y) - offset
    let maxDistance = (isHorizontal ? self.itemSize.width : self.itemSize.height) + self.minimumLineSpacing
    let distance = min(abs(collectionCenter - normalizedCenter), maxDistance)
    let ratio = (maxDistance - distance) / maxDistance
    let alpha = ratio * (1 - self.sideItemAlpha) + self.sideItemAlpha
    let scale = ratio * (1 - self.sideItemScale) + self.sideItemScale
    attributes.alpha = alpha
    attributes.transform3D = CATransform3DScale(CATransform3DIdentity, scale, scale, 1)
    attributes.zIndex = Int(alpha * 10)
    let scrollDirectionItemHeight = isHorizontal ? itemSize.height : itemSize.width
    var sideItemFixedOffset: CGFloat = 0
    switch sideItemBaselineType {
    case .top:
        sideItemFixedOffset = -(scrollDirectionItemHeight - scrollDirectionItemHeight * self.sideItemScale) / 2
    case .center:
        sideItemFixedOffset = 0
    case .bottom:
        sideItemFixedOffset = (scrollDirectionItemHeight - scrollDirectionItemHeight * self.sideItemScale) / 2
    }
    let shift = (1 - ratio) * (sideItemOffset + sideItemFixedOffset)
    if isHorizontal {
        attributes.center.y += shift
    } else {
        attributes.center.x += shift
    }
    
    return attributes
}
```
- 滚动结束，定位到中间位置
```
guard let collectionView = collectionView , !collectionView.isPagingEnabled,
    let layoutAttributes = self.layoutAttributesForElements(in: collectionView.bounds)
    else { return super.targetContentOffset(forProposedContentOffset: proposedContentOffset) }
let isHorizontal = (self.scrollDirection == .horizontal)
let midSide = (isHorizontal ? collectionView.bounds.size.width : collectionView.bounds.size.height) / 2
let proposedContentOffsetCenterOrigin = (isHorizontal ? proposedContentOffset.x : proposedContentOffset.y) + midSide
var targetContentOffset: CGPoint
if isHorizontal {
    let closest = layoutAttributes.sorted { abs($0.center.x - proposedContentOffsetCenterOrigin) < abs($1.center.x - proposedContentOffsetCenterOrigin) }.first ?? UICollectionViewLayoutAttributes()
    targetContentOffset = CGPoint(x: floor(closest.center.x - midSide), y: proposedContentOffset.y)
} else {
    let closest = layoutAttributes.sorted { abs($0.center.y - proposedContentOffsetCenterOrigin) < abs($1.center.y - proposedContentOffsetCenterOrigin) }.first ?? UICollectionViewLayoutAttributes()
    targetContentOffset = CGPoint(x: proposedContentOffset.x, y: floor(closest.center.y - midSide))
}
return targetContentOffset
```
# 封装
[Demo](https://link.jianshu.com?t=https://github.com/wusuowei/WTCarouselFlowLayout)
拓展实现
- 
`WTCarouselFlowLayoutBaselineType.top`
![](https://upload-images.jianshu.io/upload_images/2027951-e929c571fd4c3c33.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/378/format/webp)
CarouselTop.gif
- 
`WTCarouselFlowLayoutBaselineType.center`
![](https://upload-images.jianshu.io/upload_images/2027951-9cbeea171535f2ba.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/378/format/webp)
CarouselCenter.gif
- 
`WTCarouselFlowLayoutBaselineType.bottom`
![](https://upload-images.jianshu.io/upload_images/2027951-d9cc10589b8b652f.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/378/format/webp)
CarouselBottom.gif
- 
`layout.itemSpacing = -15`
![](https://upload-images.jianshu.io/upload_images/2027951-2e86fd4dd0e86a84.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/378/format/webp)
CarouselOverlap.gif
# 集成使用
- 
Requirements
	iOS 8.0+
- 
CocoaPods
```
pod "WTCarouselFlowLayout"
```
- 
Example
```
import WTCarouselFlowLayout
```
```
let layout = self.collectionView.collectionViewLayout as! WTCarouselFlowLayout
layout.itemSize = CGSize(width: 70, height: 100)
layout.scrollDirection = .horizontal
layout.spacingMode = WTCarouselFlowLayoutSpacingMode.between(spacing: 50)
//  layout.spacingMode = WTCarouselFlowLayoutSpacingMode.overlap(overlapSpacing: 15)
layout.sideItemScale = 0.7
layout.sideItemAlpha = 0.7
layout.sideItemBaselineType = .center
layout.sideItemOffset = 0.0
```
# 参考
[UPCarouselFlowLayout](https://link.jianshu.com?t=https://github.com/ink-spot/UPCarouselFlowLayout)
作者：wentianen
链接：https://www.jianshu.com/p/1a45ecdc2d5c
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
