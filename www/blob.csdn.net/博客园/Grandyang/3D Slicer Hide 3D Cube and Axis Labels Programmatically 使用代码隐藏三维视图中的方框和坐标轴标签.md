# 3D Slicer Hide 3D Cube and Axis Labels Programmatically 使用代码隐藏三维视图中的方框和坐标轴标签 - Grandyang - 博客园







# [3D Slicer Hide 3D Cube and Axis Labels Programmatically 使用代码隐藏三维视图中的方框和坐标轴标签](https://www.cnblogs.com/grandyang/p/9814297.html)







在3D Slicer中，我们如果想在自己写的插件中来修改三维视图中的默认设置的话，那么首先就需要获得三维视图的结点，其类型为[vtkMRMLViewNode](https://apidocs.slicer.org/v4.8/classvtkMRMLViewNode.html)，获得了这个结点后，我们就可以用代码来修改一系列属性了，比如隐藏方框和坐标轴标签等。这里我们通过使用系统函数getNode()来获得三维视图节点，需要注意的是，这里的视图结点的名字是vtkMRMLViewNode1，那么代码如下：



```
viewNode = slicer.util.getNode('vtkMRMLViewNode1')
viewNode.SetAxisLabelsVisible(False)
viewNode.SetBoxVisible(False)
```














