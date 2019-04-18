# [译]Android ImageView ScaleType：图解 - weixin_33985507的博客 - CSDN博客
2016年09月05日 21:07:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
如果你跟我一样追求美观界面，同时又非常健忘。当谈到`ImageView`缩放图片时，偏偏就把各种`ScaleType`的展示效果给忘了。所以你花了10~15分钟把每种效果都编译运行一遍看每种效果的异同。不久之后，这个循环又将发生一遍。
下面展示不同`ScaleType`的效果。接着是官方文档的解释，最后是一点有用的提醒。
![652417-eda520ea98c72773.png](https://upload-images.jianshu.io/upload_images/652417-eda520ea98c72773.png)
scaletype.png
[Scale Types](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#scale-types)
完整的官方解释[Android documentation](https://link.jianshu.com?t=https://developer.android.com/reference/android/widget/ImageView.ScaleType.html).
[CENTER](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#center)
放中间，不拉伸。
[CENTER_CROP](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#center_crop)
均匀拉伸（保持宽高比），宽高>=`ImageView`的宽高
[CENTER_INSIDE](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#center_inside)
均匀拉伸（保持宽高比），宽高<=`ImageView`的宽高
[FIT_CENTER](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#fit_center)
用`Matrix.ScaleToFit.CENTER`来拉伸图片。
`Matrix.ScaleToFit.CENTER`：等比缩放，确保原图会整个放在View内，宽高至少有一个适配容器宽高，居中显示。
[FIT_END](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#fit_end)
用`Matrix.ScaleToFit.END`来拉伸图片。
`Matrix.ScaleToFit.END`：同上，底部显示。
[FIT_START](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#fit_start)
用`Matrix.ScaleToFit.START`来拉伸图片。
`Matrix.ScaleToFit.START`：同上，头部显示。
[FIT_XY](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#fit_xy)
用`Matrix.ScaleToFit.FILL`来拉伸图片。
Matrix.ScaleToFit.FILL：原图完全填充容器，可能会改变宽高比。
[MATRIX](https://link.jianshu.com?t=https://robots.thoughtbot.com/android-imageview-scaletype-a-visual-guide#matrix)
用`Matrix`旋转图片
#### 调整边界
`ImageView.ScaleType`并不是万金油。如果你注意到赋值了`CENTER_INSIDE`, `FIT_CENTER`, `FIT_END` 和`FIT_START`的`ImageView`的实际边界往往都比缩放后的图片要大。如果要设置`ImageView`的宽高于缩放后的图片一致，在xml中添加
```
android:adjustViewBounds="true”
```
结果
![652417-291358fa92a8138b.png](https://upload-images.jianshu.io/upload_images/652417-291358fa92a8138b.png)
adjustviewbounds
