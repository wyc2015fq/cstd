# Solve error: 'class vtkImageActor' has no member named 'SetInput' - Grandyang - 博客园







# [Solve error: 'class vtkImageActor' has no member named 'SetInput'](https://www.cnblogs.com/grandyang/p/4955125.html)







Replacement of SetInput() with SetInputData() and SetInputConnection()



```
someFilter->SetInput(someReader->GetOutput()); // Outdated

// Replace to the following:
someFilter->SetInputConnection(someReader->GetOutputPort());
someFilter->SetInputData(aDataObject);
```



For details, please see [here](http://www.vtk.org/Wiki/VTK/VTK_6_Migration/Replacement_of_SetInput).












