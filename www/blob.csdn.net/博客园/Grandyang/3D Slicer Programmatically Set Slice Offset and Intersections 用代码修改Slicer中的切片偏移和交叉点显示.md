# 3D Slicer Programmatically Set Slice Offset and Intersections 用代码修改Slicer中的切片偏移和交叉点显示 - Grandyang - 博客园







# [3D Slicer Programmatically Set Slice Offset and Intersections 用代码修改Slicer中的切片偏移和交叉点显示](https://www.cnblogs.com/grandyang/p/6922459.html)







在3D Slicer中，当我们导入.mha/.mhd等格式的volume文件后，那么我们就可以在Axial, Sagittal, Coronal三个方向来观察我们的MRI或者CT的图像了。3D Slicer界面提供了很多交互的功能，比如正上方就有一个滑动条slider，可以用来滑动切片观察，如果我们不想手动来滑动slider，而是通过事件触发，或者其他功能来触发时，那么我们就是要通过代码来对切片进行交互。Slicer的用户界面是Qt写的，所有我们能通过用户交互触发的功能，都可以通过代码来触发。我们首先要获取slicer的层管理器，然后获得三个窗口的控件，然后通过窗口控件来获得Logic类。因为Slicer使用的是典型的MVC框架，模型，视图和控制器都分离开了。Slicer的模型就是各种节点Node，视图就是各种窗口控件Widget，而控制器就是这些Logic类，所有的算法部分都可以放到Logic类里头，而跟用户界面相关的控件都在Wdiget类里。那么我们通过代码来设置切片偏移如下所示：



```
lm = slicer.app.layoutManager()
redLogic  = lm.sliceWidget('Red').sliceLogic()
yellowLogic  = lm.sliceWidget('Yellow').sliceLogic()
greenLogic  = lm.sliceWidget('Green').sliceLogic()
# Set offset
redLogic.SetSliceOffset(10)
yellowLogic.SetSliceOffset(20)
greenLogic.SetSliceOffset(30)
```



我们在工具栏中的Crosshair按钮下点击最后一个项Slice Intersections可以打开Slice的交叉十字标，如果我们不想手动点击，而是加载我们的插件的时候就自动打开，可以使用如下代码：



```
# Set intersection visible
redLogic.GetSliceCompositeNode().SetSliceIntersectionVisibility(1)
yellowLogic.GetSliceCompositeNode().SetSliceIntersectionVisibility(1)
greenLogic.GetSliceCompositeNode().SetSliceIntersectionVisibility(1)
```



如果我们想在建立模型的时候，自动打开Slice Intersections Visible的话，可以添加下面这行代码：



```
# Set slice intersections visible
slicer.util.getNode('YourModelNode').GetDisplayNode(). SetSliceIntersectionVisibility(1)
```














