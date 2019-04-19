# UIStackView的演练-学习 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月20日 10:19:34[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：636
UIStackView是iOS9的一个新特性，特别好用。就根据我上课学的案例，来展示一下。就不过多介绍了，直接上案例，自己体会。
> 
##### 一、创建项目，导入素材
![](http://upload-images.jianshu.io/upload_images/2156697-edb4ba5d8f2c5d5d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
创建项目-导入素材
##### 二、搭建UI界面
![](http://upload-images.jianshu.io/upload_images/2156697-6b0b5b824adbfc4d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
搭建UI界面
##### 二、将所需控件添加到UIStackView中
- 单个增加
![](http://upload-images.jianshu.io/upload_images/2156697-3934dba8179f1881.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
步骤①
- 组合
![](http://upload-images.jianshu.io/upload_images/2156697-df269be4ac65cb39.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
组合
- 填充
![](http://upload-images.jianshu.io/upload_images/2156697-44955b2c5d213eb3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
填充
- 效果展示
![](http://upload-images.jianshu.io/upload_images/2156697-b84de41a16607b6f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果展示
##### 三、调整-StackView中的控件
- 使StackView中的按钮等宽
![](http://upload-images.jianshu.io/upload_images/2156697-290028552a8f6f3e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
使stackView中的按钮等宽
- 效果展示
![](http://upload-images.jianshu.io/upload_images/2156697-b880261782af94b7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
效果展示
##### 四、设置约束，解决报错
- 为了解决约束报错,设置整体UIStackView的约束上(0)、下(20)，左(20)、右(20)，内部间距设置为20，按钮的高度约束调整为44，带星星的图片高度约束设置为44，其中图片，设置为view->Mode->Aspecified,顶部lable宽度约束设置为20，设置完之后运行。
![](http://upload-images.jianshu.io/upload_images/2156697-9b2c4db4bcc0eb1f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
设置约束，解决报错
运行效果：
![](http://upload-images.jianshu.io/upload_images/2156697-57edfb8c0c71ee57.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
界面展示
##### 四、增加按钮点击事件
![](http://upload-images.jianshu.io/upload_images/2156697-748a5dbf30a8621b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
增加按钮点击事件
- 点击事件的实现
![](http://upload-images.jianshu.io/upload_images/2156697-d92abf91890c1880.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
点击事件的实现
- 多次点击,只出现一个图片是因为:图片重叠在了一块
![](http://upload-images.jianshu.io/upload_images/2156697-038a6f6c0a3a40e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图片重叠
##### 五、解决图片重叠问题，和适配星星图片大小。
- 注意底部容器内部控件的排序和图片的比例适配
![](http://upload-images.jianshu.io/upload_images/2156697-a0b12f6632cc1646.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
解决图片重叠问题
- 调整排序方式
![](http://upload-images.jianshu.io/upload_images/2156697-3c69928834ee02ce.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
图片的调整
- 解决只能出现3个图片（排列满了就添加不进去了）
![](http://upload-images.jianshu.io/upload_images/2156697-0a8bfa37695fb775.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
解决只能出现3个图片
- 展示效果
![](http://upload-images.jianshu.io/upload_images/2156697-4b3debaed3b5a197.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
展示效果
##### 六、设置撤销按钮。
- 利用这个撤销，只是把控件的约束撤销掉了，而没把控件撤掉
![](http://upload-images.jianshu.io/upload_images/2156697-64e23b6ce2df75b5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
设置撤销按钮
- 完善收尾
![](http://upload-images.jianshu.io/upload_images/2156697-e087d3c1938b40f3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
移除-完善收尾
到此结束。
文／_正阳_（简书作者）
原文链接：http://www.jianshu.com/p/3b2b3b8a6c8f
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
