# 自动计算cell高度 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月14日 19:52:13[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1230
> 
首先申明一下我的这个代码是把第三方的UITableView+FDKeyedHeightCache.m 和 UITableView+FDTemplateLayoutCell抽离出来的，抽离后我个人认为比较简单，而且便于理解
1.先上一个图片
![](http://upload-images.jianshu.io/upload_images/1940471-6851f1346d299d53.gif?imageMogr2/auto-orient/strip)
自定布局.gif
2.基本的代码是 
> 
首先 定义一个缓存的高度类别 UITableView+LeeKeyedHeightCache
.h 文件
```
//
//  UITableView+LeeKeyedHeightCache.h
//  动态计算cell 的高度
//
//  Created by apple on 16/6/13.
//  Copyright © 2016年 李重阳. All rights reserved.
//
#import <UIKit/UIKit.h>
/*缓存对象Cache**/
@interface LeeKeyedHeightCache : NSObject
@end
/*-------------------华丽的分界线-------------------**/
@interface UITableView (LeeKeyedHeightCache)
/*
 * 下面两个方法  用户是可以使用
 * 情况  1.当cell 改变的时候需要删除缓存高度
        2.当cell 删除、增加 等等，只要index的顺序改变了，都要重新改变
 **/
/* 清空某个Key的缓存的高度 **/
- (void)removeHeightCacheOfCellForKey:(NSString *)key;
/* 清空所有的缓存的高度 **/
- (void)removeAllHeightCacheOfCell;
/*
 * 下面两个方法不需要 用户使用的
 **/
/* 把某个高度 缓存到 key 中去**/
- (void)cacheCellHeight:(CGFloat)height forKey:(NSString *)key;
/* 从key 中取出某个高度 **/
- (CGFloat)heightOfCellForKey:(NSString *)key;
@end
```
> 
.m文件
```
//
//  UITableView+LeeKeyedHeightCache.m
//  动态计算cell 的高度
//
//  Created by apple on 16/6/13.
//  Copyright © 2016年 李重阳. All rights reserved.
//
#import "UITableView+LeeKeyedHeightCache.h"
#import <objc/runtime.h>
@interface LeeKeyedHeightCache ()
/* 竖着的 高度缓存数组**/
@property (nonatomic, strong) NSCache *heightValuesForPortrait;
/* 全屏的 高度缓存数组**/
@property (nonatomic, strong) NSCache *heightValuesForLandscape;
/* 把某个高度 缓存到 key 中去**/
- (void)cacheHeight:(CGFloat)height forKey:(NSString *)key;
/* 从key 中取出某个高度 **/
- (CGFloat)heightForKey:(NSString *)key;
/* 清空某个Key的缓存的高度 **/
- (void)removeHeightForKey:(NSString *)key;
/* 清空所有的缓存的高度 **/
- (void)removeAllHeightCache;
@end
@implementation LeeKeyedHeightCache
#pragma mark - 初始化数据
/*初始化数据源 **/
- (NSCache *)heightValuesForPortrait {
    if (_heightValuesForPortrait == nil) {
        _heightValuesForPortrait = [[NSCache alloc]init];
    }
    return _heightValuesForPortrait;
}
- (NSCache *)heightValuesForLandscape {
    if (_heightValuesForLandscape == nil) {
        _heightValuesForLandscape = [[NSCache alloc]init];
    }
    return _heightValuesForLandscape;
}
#pragma mark - 私有方法
/* 判断出是 竖屏 还是 横屏中的某个值 **/
- (NSCache *)heightValuesForCurrentOrientation {
    return UIDeviceOrientationIsPortrait([UIDevice currentDevice].orientation) ? self.heightValuesForPortrait:self.heightValuesForLandscape;
}
/* 通过number类型转换成CGFloat 类型**/
- (CGFloat)getFloatValueWithNumber:(NSNumber *)number {
#if CGFLOAT_IS_DOUBLE
    return [number doubleValue];
#else
    return [number floatValue];
#endif
}
#pragma mark - 公共接口
/* 把某个高度 缓存到 key 中去**/
- (void)cacheHeight:(CGFloat)height forKey:(NSString *)key {
    if (height > 0) {
        [self.heightValuesForCurrentOrientation setObject:@(height) forKey:key];
    }
}
/* 从key 中取出某个高度 **/
- (CGFloat)heightForKey:(NSString *)key {
    NSNumber * number = [self.heightValuesForCurrentOrientation objectForKey:key];
    return [self getFloatValueWithNumber:number];
}
/* 清空某个Key的缓存的高度 **/
- (void)removeHeightForKey:(NSString *)key {
    [self.heightValuesForPortrait  removeObjectForKey:key];
    [self.heightValuesForLandscape removeObjectForKey:key];
}
/* 清空所有的缓存的高度 **/
- (void)removeAllHeightCache {
    [self.heightValuesForPortrait  removeAllObjects];
    [self.heightValuesForLandscape removeAllObjects];
}
@end
/*-------------------华丽的分界线-------------------**/
/* 在类别中加入一个缓存对象**/
static const char LeeKeyedHeightCacheKey;
@implementation UITableView (LeeKeyedHeightCache)
- (LeeKeyedHeightCache *)keyedHeighCache {
    LeeKeyedHeightCache * cache = objc_getAssociatedObject(self, &LeeKeyedHeightCacheKey);
    if (cache == nil) {
        cache = [[LeeKeyedHeightCache alloc]init];
        objc_setAssociatedObject(self, &LeeKeyedHeightCacheKey, cache, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }
    return cache;
}
/* 把某个高度 缓存到 key 中去**/
- (void)cacheCellHeight:(CGFloat)height forKey:(NSString *)key {
    [self.keyedHeighCache cacheHeight:height forKey:key];
}
/* 从key 中取出某个高度 **/
- (CGFloat)heightOfCellForKey:(NSString *)key {
    return [self.keyedHeighCache heightForKey:key];
}
/* 清空某个Key的缓存的高度 **/
- (void)removeHeightCacheOfCellForKey:(NSString *)key {
    [self.keyedHeighCache removeHeightForKey:key];
}
/* 清空所有的缓存的高度 **/
- (void)removeAllHeightCacheOfCell {
    [self.keyedHeighCache removeAllHeightCache];
}
@end
```
> 
接下来定义一个计算高度的类别UITableView+LeeAutoLayoutCell
.h文件
```
//
//  UITableView+LeeAutoLayoutCell.h
//  动态计算cell 的高度
//
//  Created by apple on 16/6/13.
//  Copyright © 2016年 李重阳. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "UITableView+LeeKeyedHeightCache.h"
@interface UITableView (LeeAutoLayoutCell)
/* 返回自动布局cell的高度**/
- (CGFloat)heightForAutoLayoutCellWithIdentifier:(NSString *)identifier
                                     cacheForKey:(NSString *)key
                                   configuration:(void (^)(id cell))configuration;
@end
```
> 
.m 文件
```
//
//  UITableView+LeeAutoLayoutCell.m
//  动态计算cell 的高度
//
//  Created by apple on 16/6/13.
//  Copyright © 2016年 李重阳. All rights reserved.
//
#import "UITableView+LeeAutoLayoutCell.h"
#import <objc/runtime.h>
static const char cellCacheKey;
@implementation UITableView (LeeAutoLayoutCell)
#pragma mark - 私有方法
/* 通过自动布局 来计算cell的高度 **/
- (CGFloat)heightForCellWithIdentifier:(NSString *)identifier configuration:(void (^)(id cell))configuration {
    UITableViewCell *cell = [self cellForReuseIdentifier:identifier];
    /*手动调用确保cell 在显示屏幕中 **/
    [cell prepareForReuse];
    /*需要把这个cell 传递出去 **/
    if (configuration) {
        configuration(cell);
    }
    /* 获得cell 的宽度 **/
    CGFloat contentViewWidth = CGRectGetWidth(self.frame);
    /* 修复cell 的宽度 **/
    if (cell.accessoryView) {
        contentViewWidth -= 16 + CGRectGetWidth(cell.accessoryView.frame);
    } else {
        static const CGFloat systemAccessoryWidths[] = {
            [UITableViewCellAccessoryNone] = 0,
            [UITableViewCellAccessoryDisclosureIndicator] = 34,
            [UITableViewCellAccessoryDetailDisclosureButton] = 68,
            [UITableViewCellAccessoryCheckmark] = 40,
            [UITableViewCellAccessoryDetailButton] = 48
        };
        contentViewWidth -= systemAccessoryWidths[cell.accessoryType];
    }
    if (contentViewWidth <= 0) {
        return 0;
    }
    CGSize fittingSize = CGSizeZero;
    /* 因为label 换行的时候 需要知道contentView 的最大的宽度 
     * 这个方法很good
     **/
    NSLayoutConstraint *tempWidthConstraint = [NSLayoutConstraint constraintWithItem:cell.contentView attribute:NSLayoutAttributeWidth relatedBy:NSLayoutRelationEqual toItem:nil attribute:NSLayoutAttributeNotAnAttribute multiplier:1.0 constant:contentViewWidth];
    [cell.contentView addConstraint:tempWidthConstraint];
    // 自动布局的系统方法 计算高度
    fittingSize = [cell.contentView systemLayoutSizeFittingSize:UILayoutFittingCompressedSize];
    /* 计算完后 再删除 **/
    [cell.contentView removeConstraint:tempWidthConstraint];
    /* 修复 线 的1个像素 **/
    if (self.separatorStyle != UITableViewCellSeparatorStyleNone) {
        fittingSize.height += 1.0 / [UIScreen mainScreen].scale;
    }
    return fittingSize.height;
}
/* 获取 cell **/
- (UITableViewCell *)cellForReuseIdentifier:(NSString *)identifier {
    NSCache * cellCache = objc_getAssociatedObject(self, &cellCacheKey);
    if (cellCache == nil) {
        cellCache = [[NSCache alloc]init];
        objc_setAssociatedObject(self, &cellCacheKey, cellCache, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    }
    UITableViewCell *cell = [cellCache objectForKey:identifier];
    if (cell == nil) {
        cell = [self dequeueReusableCellWithIdentifier:identifier];
        /* cell 如果是nil 就报一个错误：提示用户应该要在tableView 注册cell**/
        NSAssert(cell != nil, @"Cell must be registered to table view for identifier - %@", identifier);
        cell.contentView.translatesAutoresizingMaskIntoConstraints = NO;
        [cellCache setObject:cell forKey:identifier];
    }
    return cell;
}
#pragma mark - 公共方法
/* 返回自动布局cell的高度**/
- (CGFloat)heightForAutoLayoutCellWithIdentifier:(NSString *)identifier
                                     cacheForKey:(NSString *)key
                                   configuration:(void (^)(id cell))configuration {
    if (identifier.length == 0 || key.length == 0) {
        return 0;
    }
    /*先从缓存中取 **/
    CGFloat cachedHeight = [self heightOfCellForKey:key];
    if (cachedHeight >0) {
        return cachedHeight;
    }else {
        //计算cell 的高度并且缓存进去
        CGFloat height = [self heightForCellWithIdentifier:identifier configuration:configuration];
        /* 缓存cell的高度**/
        [self cacheCellHeight:height forKey:key];
        return height;
    }
}
@end
```
3.计算高度和逻辑交互
> 
计算高度
```
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
    return [self.tableView heightForAutoLayoutCellWithIdentifier:@"MasCell" cacheForKey:[NSString stringWithFormat:@"MasCell%ld_%ld",indexPath.section,indexPath.row] configuration:^(MasCell *cell) {
        /*cell  需要重新布局 **/
        cell.model = self.dataArr[indexPath.row];
    }];
}
```
> 
点击事件的处理
```
/* 删除所有的 cell 的高度缓存**/
    [self.tableView removeAllHeightCacheOfCell];
    [self.tableView reloadData];
```
[github 中的包内容](https://github.com/lichory/autolayoutCell.git)
- [](https://github.com/lichory/autolayoutCell)
###### GitHub - lichory/autolayoutCell: 计算 自动布局cell的高度，并且缓存cell 的高度
—
文／李重阳_arc（简书作者）
原文链接：http://www.jianshu.com/p/475579e90644
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
