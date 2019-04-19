# 关于arcgis engine的工作空间（IWorkspace）和选择集(FeatureSelection) - 毛小亮 - 博客园
# [关于arcgis engine的工作空间（IWorkspace）和选择集(FeatureSelection)](https://www.cnblogs.com/xianerwonder/p/4598169.html)
1、通过某个WorkspaceFactoryClass（例如AccessWorkspaceFactoryClass）拿到工作空间工厂接口，这时的OpenFromFile方法可以直接打开mdb类型文件，作为工作空间。如果是ShapefileWorkspaceFactoryClass，则直接打开shp文件所在的文件夹。
2、IEnumDataset 接口直接获取工作空间的数据集，然后迭代拿到要素集（可根据别名）。
3、可以用IMap接口先调用SelectByShape方法，之后使用Map.FeatureSelection得到IEnumFeature，之后可迭代，获得某个要素及其字段值。

