# arcgis engine 中出现的内存堆栈溢出问题。 - 毛小亮 - 博客园
# [arcgis engine 中出现的内存堆栈溢出问题。](https://www.cnblogs.com/xianerwonder/p/4717371.html)
**两种解决方案：**
　　1、循环加载mxd文档的时候出现的堆栈溢出，解决办法是每次循环结束时清空FeatureLayer，感觉并不好，但是确实可以实现功能。
　　2、循环调取featureclass的search方法，游标循环赋值引用时（循环次数超过200时就会出现）。
　　　　会报错：HRESULT:0x80040258。
　　　　解决的办法是system.runtime.interopService.marshal.releaseComObject();
 ps:1.featureCursor和feature都需要释放。
　　2.workspaceFactory打开工作空间时一定要注意传入的参数，如果传错了也会报错HRESULT:0x80040258
　　3.像是ShapefileWorkspaceFactory在OpenFromFile时，传入的路径就应该去掉后缀名.shp

