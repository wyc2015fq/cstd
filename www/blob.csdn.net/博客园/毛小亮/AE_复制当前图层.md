# AE_复制当前图层 - 毛小亮 - 博客园
# [AE_复制当前图层](https://www.cnblogs.com/xianerwonder/p/4238928.html)
```csharp;gutter
private void 复制ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int layercount = axMapControl2.LayerCount;
            if (layercount <= 0)
            { MessageBox.Show("没有图层！", "系统错误", MessageBoxButtons.OK, MessageBoxIcon.Warning); }
            else
            {
            IFeatureLayer pFeatureLayer = (IFeatureLayer)axMapControl2.get_Layer(0);
            ITable pTable = pFeatureLayer.FeatureClass as ITable;
            IFields pFields;
            pFields = pFeatureLayer.FeatureClass.Fields;
            IDataset pp = (IDataset)pFeatureLayer.FeatureClass;
            //MessageBox.Show(pp.Workspace.PathName +@"\"+ pp.Name+".shp");
            IWorkspaceFactory pWorkspaceFac = new ShapefileWorkspaceFactoryClass();
            IFeatureWorkspace pFeatureworkspace = (IFeatureWorkspace)pWorkspaceFac.OpenFromFile(pp.Workspace.PathName,0);
            IFeatureClass pFeatureClass = pFeatureworkspace.CreateFeatureClass(pp.Name + layercount.ToString() + ".shp", pFields, null, null, esriFeatureType.esriFTSimple, "shape", "");
            IFeatureCursor pFeatureCursor = pFeatureLayer.FeatureClass.Search(null,false);
            IFeature pFeature1 =pFeatureCursor.NextFeature();
            while(pFeature1!=null)
            {
            IFeature pFeature2 = pFeatureClass.CreateFeature();
            pFeature2.Shape = pFeature1.Shape;
            for (int i = 0; i < pFields.FieldCount-2; i++)
            {
               pFeature2.set_Value(i+2,pFeature1.get_Value(i+2)); 
            }
          
            pFeature2.Store();
            pFeature1 = pFeatureCursor.NextFeature();
            }
            axMapControl2.AddShapeFile(pp.Workspace.PathName, pp.Name + layercount.ToString() + ".shp");
            }
            MessageBox.Show("复制完成","系统提示",MessageBoxButtons.OK,MessageBoxIcon.Information);
            
        }
```
