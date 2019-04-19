# ArcEngine数据删除几种方法和性能比较[转] - 毛小亮 - 博客园
# [ArcEngine数据删除几种方法和性能比较[转]](https://www.cnblogs.com/xianerwonder/p/4720427.html)
**四个解决方案:**
　　1、IFeatureCursor 游标查询后，遍历删除
　　2、更新游标删除IFeatureCursor.DeleteFeature()
　　3、ITable.DeleteSearchedRows删除
　　4、 IDataset.Workspace.ExecuteSQL
**一、  几种删除方法的代码**
1.  查询结果中删除
  private void Delete1(IFeatureClass PFeatureclass)
        {
            IQueryFilter pQueryFilter = new QueryFilterClass();
            pQueryFilter.WhereClause = "objectID<=" + DeleteNum;
            IFeatureCursor pFeatureCursor = PFeatureclass.Search(pQueryFilter, false);           IFeature pFeature = pFeatureCursor.NextFeature();
            while (pFeature != null)
            {
                pFeature.Delete();
                pFeature = pFeatureCursor.NextFeature();
            }
            System.Runtime.InteropServices.Marshal.ReleaseComObject(pQueryFilter);
        }
2.  更新游标删除
private void Delete2(IFeatureClass PFeatureclass)
        {
            IQueryFilter pQueryFilter = new QueryFilterClass();
            pQueryFilter.WhereClause = "objectID<=" + DeleteNum;
            IFeatureCursor pFeatureCursor = PFeatureclass.Update(pQueryFilter, false);
            IFeature pFeature = pFeatureCursor.NextFeature();
            while (pFeature != null)
            {
                pFeatureCursor.DeleteFeature();
                pFeature = pFeatureCursor.NextFeature();
            }
            System.Runtime.InteropServices.Marshal.ReleaseComObject(pQueryFilter);
        }
3.  使用DeleteSearchedRows删除
private void Delete4(IFeatureClass PFeatureclass)
        {
            IQueryFilter pQueryFilter = new QueryFilterClass();
            pQueryFilter.WhereClause = "objectID<=" + DeleteNum;
            ITable pTable = PFeatureclass as ITable;
            pTable.DeleteSearchedRows(pQueryFilter);
            System.Runtime.InteropServices.Marshal.ReleaseComObject(pQueryFilter);
        }
4.  ExecuteSQL删除
private void Delete4(IFeatureClass PFeatureclass)
        {
            IDataset pDataset = PFeatureclass as IDataset;
            pDataset.Workspace.ExecuteSQL("delete from " + PFeatureclass.AliasName + " where objectid<=" + DeleteNum);
        }
**二、  测试性能和比较**
1、       相同的数据条件，删除2000条记录
2、       测试代码
IFeatureLayer pFeatureLayer = axMapControl1.Map.get_Layer(0) as IFeatureLayer;
            IFeatureClass PFeatureClass = pFeatureLayer.FeatureClass;
            System.Diagnostics.Stopwatch MyWatch = new System.Diagnostics.Stopwatch();
            MyWatch.Start();
            Delete1(PFeatureClass)
            //Delete2(PFeatureClass);
            //Delete3(PFeatureClass);
            //Delete4(PFeatureClass);
            //Delete5(PFeatureClass);
            MyWatch.Stop();
            MessageBox.Show("删除时间:" + MyWatch.ElapsedMilliseconds.ToString() + "毫秒");
3、       测试情况
测试方法
第一次时间（单位ms）
第一次时间（单位ms）
1
5214ms
5735ms
2
299ms
290Ms
3
59ms
28ms
4
26ms
26ms
**三、  结论**
　　1、使用ExecuteSQL删除最快，数据库的效率最高。
　　2、DeleteSearchedRows和ExecuteSQL属于批量删除，性能较优。
　　3、查询结果中删除，速度最慢，如果你使用这种方法，建立你马上修改你的程序，因为你在浪费时间。
　　4、小数据量记录数小于500000条，请使用DeleteSearchedRows或ExecuteSQL，否则使用更新游标删除(方法2)，加上进度条，这样界面很友好。

