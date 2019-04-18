# 使用 xib 设置约束 - weixin_33985507的博客 - CSDN博客
2017年04月17日 11:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
0.使用代码设置约束和使用 xib 设置约束,两种方式各有好处吧.代码约束便于维护和修改, xib 约束则可快速搭建界面,免去了很多无聊的 UI 代码.我本人呢,之前用代码做约束,新项目里因为工期紧张,则用的 xib 设置的约束.下面说一下如何设置居中以及如何按比例设置约束.
1.居中
```
首先打开 xib 文件,拖拽一个 UIView 到 xib 的控制器内.
```
![111368-f9cff46fe17e6b22.png](https://upload-images.jianshu.io/upload_images/111368-f9cff46fe17e6b22.png)
拖拽一个 UIView
```
然后点击右下角的约束设置图标
```
![111368-9530ef8d6555bd36.png](https://upload-images.jianshu.io/upload_images/111368-9530ef8d6555bd36.png)
点击右下角的设置约束按钮
```
在这里可以设置水平居中和垂直居中,若都选上的话则控件会位于屏幕中心点.我们这里选择第一个,水平居中.选择之后点击添加约束按钮.这时控件可能会出现一个红色边框,这是因为缺少约束条件造成的.因为我们目前只设置了水平居中的位置,所以还需要其他约束条件.
```
```
添加新的约束条件
```
![111368-40cea1e81a31d124.png](https://upload-images.jianshu.io/upload_images/111368-40cea1e81a31d124.png)
点击这里.png
![111368-e28f4cf00690c3bf.png](https://upload-images.jianshu.io/upload_images/111368-e28f4cf00690c3bf.png)
补充约束条件
```
这时约束条件全部添加上去了,但我们可能会发现左侧出现了一个黄色的图标.这是因为我们设置的约束条件跟控件当前的位置不符合.我们需要更新一下.
```
```
点击黄色图标,选择第一个 Update frames 即可.至此我们便设置了一个宽高为 240*80,y 坐标为 110,水平居中的控件在控制器里面了.
```
2.设置比例宽/高
我们经常会遇到想要一个控件的宽度与屏幕宽度成一定的比例.下面来说一下该如何设置.
拖拽一个 UIView
设置其宽高和 y 轴坐标.先不设置其宽度.
```
在左侧点击该 UIView, 然后按住 Shift 键,选中其父控件.类似下图:
```
![111368-e631ee2f63fd37cb.png](https://upload-images.jianshu.io/upload_images/111368-e631ee2f63fd37cb.png)
选中 UIView 和其父控件
![111368-40cea1e81a31d124.png](https://upload-images.jianshu.io/upload_images/111368-40cea1e81a31d124.png)
点击这里.png
```
选择 Equal Width 选项.然后点击左侧的黄色图标, Update frames.
```
```
此时控件的宽度是和其父控件宽度是一致的.接下来我们点击右上角的一个像尺子的图标.
```
![111368-490f12d4a83e582e.png](https://upload-images.jianshu.io/upload_images/111368-490f12d4a83e582e.png)
点击这里修改宽度系数
```
然后点击宽度的约束项,设置系数为0.5,即父视图宽度的一半.点击回车键即可.此时会发现控件宽度已变为父视图宽度的一半.至此,设置完毕.
```
![111368-e1f59f31103b6af5.png](https://upload-images.jianshu.io/upload_images/111368-e1f59f31103b6af5.png)
修改比例系数.png
