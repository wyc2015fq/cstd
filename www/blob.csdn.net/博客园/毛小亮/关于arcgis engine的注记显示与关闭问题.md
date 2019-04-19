# 关于arcgis engine的注记显示与关闭问题 - 毛小亮 - 博客园
# [关于arcgis engine的注记显示与关闭问题](https://www.cnblogs.com/xianerwonder/p/4664994.html)
1、注记的添加需要拿到IGeoFeatureLayer接口下的AnnotationProperties属性，转为IAnnotationLayerPropertiesCollection接口，并创建一个IBasicOverposterLayerProperties对象，加入上面那个属性中去。
2、IGeoFeatureLayer.DisplayAnnotation= true或者=false并不能直接关闭或者打开显示注记。
3、要让注记不显示，目前来看需要axMapcontrol.ActiveView.GraphicsContainer.DeleteAllElements();

