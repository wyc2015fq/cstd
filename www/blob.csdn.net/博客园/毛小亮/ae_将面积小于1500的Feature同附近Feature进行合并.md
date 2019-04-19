# ae_将面积小于1500的Feature同附近Feature进行合并 - 毛小亮 - 博客园
# [ae_将面积小于1500的Feature同附近Feature进行合并](https://www.cnblogs.com/xianerwonder/p/4238999.html)
```csharp;gutter
private void 合并1500图斑ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            /*
             *将图层中面积小于1500的图斑与之相同BSM且相邻的最大的图斑合并。 
             */
            if(axMapControl2.LayerCount<=0)
            { MessageBox.Show("请先添加图层再合并！"); }
            else
            {
            pLayer = axMapControl2.get_Layer(0);//拿到层
            pFLayer = pLayer as IFeatureLayer;//要素层
            pFC = pFLayer.FeatureClass;//要素类
            
            IFeatureCursor pFCursor = pFC.Search(null, false);//拿到该要素下的指针
            IFeature pFeature = pFCursor.NextFeature();//递进光标的位置，拿到返回在该位置的要素对象
             
            //创建表，获得要素ID，标识码，面积，用于筛选相同表示码，面积较小的要素
            DataTable pTable = new DataTable();
            DataColumn colID = new DataColumn("要素ID");//新建一个要素列，并设置列名
            colID.DataType = System.Type.GetType("System.String");//设置该列类型
            pTable.Columns.Add(colID);//塞入表
            DataColumn colIdent = new DataColumn("标识码");
            colIdent.DataType = System.Type.GetType("System.String");
            pTable.Columns.Add(colIdent);
            DataColumn colArea = new DataColumn("面积");
            colArea.DataType = System.Type.GetType("System.Double");
            pTable.Columns.Add(colArea);
            int indexOfFID_HBTC_Z = pFC.FindField("FID_HBTC_Z");//返回要素类的指定字段的索引
            int indexOfBSM = pFC.FindField("BSM");//返回要素类的指定字段的索引
            int indexOfTBMJ = pFC.FindField("TBMJ");//返回要素类的指定字段的索引
           
            while (pFeature != null)
            {
                //string FID_HBTC_Z = pFeature.get_Value(indexOfFID_HBTC_Z).ToString();
                string BSM = pFeature.get_Value(indexOfBSM).ToString();
                double TBMJ = (double)pFeature.get_Value(indexOfTBMJ);
                //挑选出小于1500的图斑
                if (TBMJ < 1500)
                {
                    //查找标识码相同的图斑
                    MergeFeature(pFeature);
                    //IQueryFilter queryFilter1 = new QueryFilterClass();
                    //queryFilter1.WhereClause = "TBMJ=" + TBMJ.ToString();
                    //IFeatureCursor pFUpdateCursor = pFC.Update(queryFilter1, false);
                    //pFUpdateCursor.DeleteFeature();
                    
                }
                
                pFeature = pFCursor.NextFeature();
            }
            MessageBox.Show("合并成功");
            }
        }
        private void MergeFeature(IFeature tarFeature)
        {
            int indexOfBSM = pFC.FindField("BSM");
            string BSM = tarFeature.get_Value(indexOfBSM).ToString();//拿到当前feature的BSM值
            IQueryFilter queryFilter = new QueryFilterClass();
            queryFilter.WhereClause = "BSM=" + BSM;
            int tarindexOfTBMJ = pFC.FindField("TBMJ");
            double tarTBMJ = (double)tarFeature.get_Value(tarindexOfTBMJ);//拿到当前feature的TBMJ值
            IFeatureCursor updateCursor = pFC.Update(queryFilter, true);//按照查询，拿到更新要素的指针
            IFeature feature1 = updateCursor.NextFeature();//拿到BSM相同的下一个要素
            //feature1.get_Value(30);
            while (feature1 != null)
            {
                //是否相邻
                ITopologicalOperator topo = (feature1.Shape as IPolygon) as ITopologicalOperator;
                IRelationalOperator rela = (feature1.Shape as IPolygon)  as IRelationalOperator;
                //bool biii = rela.Touches(tarFeature.Shape);
                if (rela.Touches(tarFeature.Shape))//如果相邻
                {
                    int indexOfTBMJ = pFC.FindField("TBMJ");//从要素类中拿到该字段索引值
                    double TBMJ = (double)feature1.get_Value(indexOfTBMJ);//拿到该要素该字段的值
                    //合并图斑
                    if (TBMJ>=1500)
                    {
                        feature1.Shape = topo.Union(tarFeature.Shape) as IPolygon;
                        feature1.Store();
                        tarFeature.Delete();//删除被合并要素
                        break;
                    }
                }
                //ESRI.ArcGIS.SpatialStatisticsTools.CalculateAreas
                //updateCursor.Flush(); //是否。。。
                
                feature1 = updateCursor.NextFeature();
            }
           
            //TokayWorkspace.ComRelease(del_featcur);
            //updateCursor = null;  
            
            axMapControl2.ActiveView.Refresh();
        }
```
