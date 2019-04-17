# 3D Slicer Modify Mouse Event 修改3D Slicer中的鼠标响应事件 - Grandyang - 博客园







# [3D Slicer Modify Mouse Event 修改3D Slicer中的鼠标响应事件](https://www.cnblogs.com/grandyang/p/6880814.html)







在3D Slicer中，我们如果想在自己写的插件中来修改默认的鼠标响应事件的话，就需要先将原有的响应事件链接删除，然后建立自定义的响应事件链接，然后将自己要实现的功能写在响应事件函数中。

比如Slicer中默认的鼠标左键拖拽，是任意旋转视图中的Camera，默认中的滑轮操作是拉远和拉近视图摄像机，默认的鼠标右键操作也是拉远和拉近视图摄像机的操作。这样滑轮就和鼠标右键的功能重复了，我们可以让鼠标右键实现其他的功能。比如在模型的Pitch， Yaw， Roll三个旋转方向上(其实旋转的只是试图中的摄像机，我们看起来像是场景中3D模型在旋转)。方向键的上下键实现了对模型的Pitch旋转，左右键实现了对模型的Yaw旋转，唯独没有对模型的Roll方向旋转的操作。这样我们空余出来的右键可以实现Roll旋转的操作，我们也可以随意在键盘上找其他两个键来添加上Roll旋转的功能。

我们首先来看如何移除原有的鼠标事件响应链接，由于Slicer的核心是由VTK来实现的，所以我们参考VTK中的Observer的操作，参见这个帖子[VTK/Examples/Python/Interaction/MouseEventsObserver](http://www.vtk.org/Wiki/VTK/Examples/Python/Interaction/MouseEventsObserver)，知道了我们一定要获得用户交互实例interactor才能进行响应事件的修改，但是我们不直接用例子中的代码，毕竟二者的实现细节上还有些差异。Slicer的用户界面是Qt写的，所以我们首先要获取slicer的层管理器，然后获得3D窗口的控件，然后获得3D窗口的View，从view中获得交互实例interator，才能进行修改。我们将原有的鼠标右键响应事件删除，链接上自定义的响应函数，参见代码如下：



```
# Interator
layoutManager = slicer.app.layoutManager()
threeDWidget = layoutManager.threeDWidget(0)
threeDView = threeDWidget.threeDView()
interactor = threeDView .interactorStyle().GetInteractor()
interactor.AddObserver(vtk.vtkCommand.RightButtonPressEvent, self.onRightButtonPressed)

#
# Customized mouse right button pressed event
#
def onRightButtonPressed(self, obj, event=None):
  print ('onRightButtonPressed...')
  # Do something here
```



我们只要将需要实现的功能写在自定义的响应函数中即可，对于鼠标左键或者滑轮的修改也可以这样来做。下列各项事件均可自定义：



```
vtk.vtkCommand.KeyPressEvent
vtk.vtkCommand.KeyReleaseEvent
vtk.vtkCommand.LeftButtonPressEvent
vtk.vtkCommand.LeftButtonReleaseEvent
vtk.vtkCommand.MouseMoveEvent
vtk.vtkCommand.MouseWheelForwardEvent
vtk.vtkCommand.MouseWheelBackwardEvent
vtk.vtkCommand.RightButtonPressEvent
vtk.vtkCommand.RightButtonReleaseEvent
vtk.vtkCommand.MiddleButtonPressEvent
vtk.vtkCommand.MiddleButtonReleaseEvent
vtk.vtkCommand.CursorChangedEvent
```



参考资料：

[http://www.vtk.org/Wiki/VTK/Examples/Python/Interaction/MouseEventsObserver](http://www.vtk.org/Wiki/VTK/Examples/Python/Interaction/MouseEventsObserver)

[http://slicer-users.65878.n3.nabble.com/Capturing-mouse-events-td4029365.html](http://slicer-users.65878.n3.nabble.com/Capturing-mouse-events-td4029365.html)

[http://web.mit.edu/16.225/dv/VTK/Examples/ImageProcessing/Python/ImageSlicing.py](http://web.mit.edu/16.225/dv/VTK/Examples/ImageProcessing/Python/ImageSlicing.py)

[https://www.slicer.org/wiki/Documentation/Nightly/ScriptRepository#Customize_viewer_layout](https://www.slicer.org/wiki/Documentation/Nightly/ScriptRepository#Customize_viewer_layout)












