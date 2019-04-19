# iOS | 比例拼图 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年07月18日 16:27:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：162
![](https://upload-images.jianshu.io/upload_images/1692043-53631db99f6659b7.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/640)
给陈意涵打call
### 需求原型图：
![](https://upload-images.jianshu.io/upload_images/1692043-1c34d59dbc16a097.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/550)
**要求：**
各个模块的大小反映各个模块的占比（销售额），所有模块共同组成一个正方形。
### 后台返回的数据格式：
```
{
    "result": true,
    "data": {
        "category_sale": [ {
            "name": "我是你的哥",
            "sale_amount": 1,
            "gross_margin_ratio": 0.22
        }, {
            "name": "不是亲哥哥",
            "sale_amount": 4,
            "gross_margin_ratio": 0
        }, {
            "name": "呵呵哒",
            "sale_amount": 3,
            "gross_margin_ratio": 0.19
        }, {
            "name": "因缺思厅",
            "sale_amount": 2,
            "gross_margin_ratio": 0.4
        }]
    },
    "msg": "ok",
    "code": 200,
    "executed": "0.0320830345"
}
```
注：`gross_margin_ratio`代表“毛利率”，不是模块的占比。
### 分析
第一眼看到这个原型图的时候我就觉得不简单，后面和Android一起研究了一下，也没有想到什么好的算法。正巧那天上司跑来问我们有没有什么需要帮忙的，我赶紧把这个问题扔给他。
**一周后**，他给我说了思路：
> 
每一排放三个，让它们的高度一致。
经他这么一点，这个问题立即就不是问题了（放3个还是放两个通过开方得到最合适的值）。
一排放三个模块，三个一组组成一个矩形，这一组的总面积确定，宽确定，那么高就确定了。高确定，每个模块的面积确定，每个模块的宽也就确定了。至于排版
**交给`UICollectionView`就行了。**
### 实现
**效果如下：**
![](https://upload-images.jianshu.io/upload_images/1692043-8528bef2dfd80ec5.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/324)
**核心代码：**
```
- (void)setModel:(CQCategoryModel *)model {
    _model = model;
    
    CGFloat totalSaleAmount = 0;
    for (CQCategoryItemModel *itemModel in _model.category_sale) {
        totalSaleAmount += itemModel.sale_amount;
    }
    
    for (CQCategoryItemModel *itemModel in _model.category_sale) {
        if (totalSaleAmount == 0) {
            // 特殊处理只有一个item，并且SaleAmount还是0的情况
            itemModel.ratio = 1;
        } else {
            itemModel.ratio = itemModel.sale_amount/totalSaleAmount;
        }
    }
    
    // 计算列数
    NSInteger listCount = 0;
    for (int i = 0; i < _model.category_sale.count; i++) {
        if (i * i < _model.category_sale.count && (i+1) * (i+1) >= _model.category_sale.count) {
            listCount = i+1;
            break;
        }
    }
    
    // 计算行数
    NSInteger rowCount = ceil(_model.category_sale.count / (CGFloat)listCount);
    
    // 这个方阵是listCount*rowCount的矩阵（最后一排可能不足listCount）
    // 同一排的cell高度相同
    for (int i = 0; i < rowCount; i++) { // 行
        CGFloat rowArea = 0; // 行面积
        for (int j = 0; j < listCount; j++) { // 列
            if (i*listCount+j>=_model.category_sale.count) {
                break;
            }
            CQCategoryItemModel *itemModel = _model.category_sale[i*listCount+j];
            itemModel.size = itemModel.ratio * (self.collectionView.width*self.collectionView.width);
            rowArea += itemModel.size;
        }
        // 计算cell的宽高
        for (int j = 0; j < listCount; j++) { // 列
            if (i*listCount+j>=_model.category_sale.count) {
                break;
            }
            CQCategoryItemModel *itemModel = _model.category_sale[i*listCount+j];
            itemModel.height = rowArea / self.collectionView.width;
            itemModel.width = itemModel.size / itemModel.height;
        }
    }
    
    [self.collectionView reloadData];
}
- (CGSize)collectionView:(UICollectionView *)collectionView layout:(UICollectionViewLayout *)collectionViewLayout sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
    CQCategoryItemModel *model = self.model.category_sale[indexPath.row];
    // 减去0.01，避免因小数不精确存储导致一组cell宽度相加超过collectionView的宽度
    return CGSizeMake(model.width-0.01, model.height);
}
```
### 完整demo
**[https://github.com/CaiWanFeng/iOS_Demo](https://github.com/CaiWanFeng/iOS_Demo)**
作者：无夜之星辰
链接：https://www.jianshu.com/p/70eec89b71ac
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
