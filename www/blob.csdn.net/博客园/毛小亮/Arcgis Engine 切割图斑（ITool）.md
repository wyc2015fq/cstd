# Arcgis Engine 切割图斑（ITool） - 毛小亮 - 博客园
# [Arcgis Engine 切割图斑（ITool）](https://www.cnblogs.com/xianerwonder/p/4311479.html)
```csharp;gutter
public override void OnMouseDown(int Button, int Shift, int X, int Y)
        {
            // TODO:  Add Cut.OnMouseDown implementation
            if(Button==1)//左键按下
            {
                //拿到hook
                IMapControl3 pMapControlx = (IMapControl3)m_hookHelper.Hook;
                IFeatureLayer pFeatureLayer = pMapControlx.get_Layer(0) as IFeatureLayer;
                //划线
                IGeometry pline = (IGeometry)pMapControlx.TrackLine();
                
                //拿到划线范围内的所有feature
                IGeometry pGeometry = pline as IGeometry;
                IMap pMap = pMapControlx.Map;
                pMap.SelectByShape(pGeometry, null, false);
                //转成IRealtionOperator接口，以便在遍历时判断几何位置关系。
                IRelationalOperator rela = (pline as IPolyline) as IRelationalOperator;
                //拿到切割目标集合
                IEnumFeature pEnumFeature = (IEnumFeature)pMap.FeatureSelection;
                IFeature pFeature = pEnumFeature.Next();
                //依次遍历
                while (pFeature != null)
                {
                    
                    if(rela.Crosses(pFeature.Shape))//判断是否Cross
                    {
                        try
                        {
                            //把面转成ITopologicalOperator，使用Cut函数
                            ITopologicalOperator topo = (pFeature.Shape as IPolygon) as ITopologicalOperator;
                            //新建两个面，作为切割函数的参数，切割后的两个对象保存于这两个面之中
                            IGeometry pLeftGeometry = new PolygonClass(); 
                            IGeometry pRightGeometry = new PolygonClass();
                            topo.Cut((pline as IPolyline), out pLeftGeometry, out pRightGeometry);
                            //由FeatureClass新建Feature
                            IFeature pFeatureLeft = pFeatureLayer.FeatureClass.CreateFeature();
                            IFeature pFeatureRight = pFeatureLayer.FeatureClass.CreateFeature();
                            //保存
                            pFeatureLeft.Shape = pLeftGeometry;
                            pFeatureRight.Shape = pRightGeometry;
                            pFeatureLeft.Store();
                            pFeatureRight.Store();
                            //删除
                            pFeature.Delete();
                            
                        }
                        catch
                        { 
                        
                        }
                    }
                    pFeature = pEnumFeature.Next();
                }
                //清除选择
                IFeatureSelection pFeatureSelectionClear= pMapControlx.get_Layer(0) as IFeatureSelection;
                pFeatureSelectionClear.Clear();
                pMapControlx.Refresh();
            }
        }
```
