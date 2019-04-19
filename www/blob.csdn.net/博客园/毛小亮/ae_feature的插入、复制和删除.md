# ae_feature的插入、复制和删除 - 毛小亮 - 博客园
# [ae_feature的插入、复制和删除](https://www.cnblogs.com/xianerwonder/p/4238983.html)
```csharp;gutter
1、插入
/// <summary>
///向featureclass中批量插入features ,批量插入features，用buffer的方法，要比循环一个个Store的方法快
///</summary>
/// <param name="pFeatureClass">操作数据所在的IFeatureClass</param>
private void InsertFeatures( IFeatureClass pFeatureClass , List< IGeometry> pGeos )
{
　　IFeatureBuffer pFeatBuf = pFeatureClass.CreateFeatureBuffer();
　　IFeature pFeat = pFeatBuf;
　　IFeatureCursor pFeatCur= pFeatureClass.Insert(true);
　　for(int i = 0 ;i< pGeos.Count ;i++)
　　{
　　pFeat.Shape = pGeos(i);
　　pFeatCur.InsertFeature(pFeatBuf);
　　//避免一次flush，导致速度过慢
　　if (i Mod 100 == 0 )
　　{
　　　　pFeatCur.Flush()
　　}
　　pFeatCur.Flush()
}
2、拷贝
/// <summary>
///复制一个IFeatureClass中的要素到另外一个当中
/// </summary>
/// <param name="pFromFeatureClass">源数据所在的IFeatureClass</param>       
/// <param name="pToFeatureClass">目标数据所在的IFeatureClass</param>
private void CopyFeatureClass(IFeatureClass pFromFeatureClass, IFeatureClass pToFeatureClass)
{
　　try {
　　　　IFeatureCursor pFromFeatureCursor = pFromFeatureClass.Search(null, false);
　　　　IFeatureCursor pToFeatureCursor = pToFeatureClass.Insert(true);
　　　　IFeatureBuffer pFeatureBuffer = pToFeatureClass.CreateFeatureBuffer();
　　　　IFeature pFromFeature = pFromFeatureCursor.NextFeature();
　　　　while (pFromFeature != null)
　　　　{
　　　　　　int IndexShape = pFeatureBuffer.Fields.FindField("Shape");
　　　　　　pFeatureBuffer.set_Value(IndexShape, pFromFeature);
　　　　　　pFeatureBuffer.Shape = pFromFeature.Shape;
　　　　　　pToFeatureCursor.InsertFeature(pFeatureBuffer);
　　　　　　pFromFeature = pFromFeatureCursor.NextFeature();
　　　　　　pFeatureBuffer = pToFeatureClass.CreateFeatureBuffer();
　　　　}
      　　pToFeatureCursor.Flush();
　　}
　　catch (Exception ex)
　　{
　　　　throw ex;
　　}
}
3、删除
///<summary>删除某featurelayer中所有feature
/// 该方法可以给一个queryfilter，进行删除符合条件的Ifeatures///</summary>
/// <param name="pLayer ">:操作的图层</param>
Private void DeleteAllFeatures(IFeatureLayer pLayer )
{
　　ITable pTable = pLayer.FeatureClass;
　　pTable.DeleteSearchedRows(null);
}
```
