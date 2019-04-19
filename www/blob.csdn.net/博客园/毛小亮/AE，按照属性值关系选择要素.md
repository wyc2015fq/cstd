# AE，按照属性值关系选择要素 - 毛小亮 - 博客园
# [AE，按照属性值关系选择要素](https://www.cnblogs.com/xianerwonder/p/4230405.html)
if(axMapControl2.LayerCount<=0)
            {
                MessageBox.Show("请加载图层后使用该功能","系统提示",MessageBoxButtons.OK,MessageBoxIcon.Warning);
            }
            else
            {
            ILayer pLayer = axMapControl2.get_Layer(0);
            IFeatureLayer pFeatureLayer = pLayer as IFeatureLayer;
            IFeatureSelection pFeatureSelection = (IFeatureSelection)pFeatureLayer;//要素层转为要素选择
            IQueryFilter pQueryFilter = new QueryFilter();//（新建查询过滤器）
            pQueryFilter.WhereClause = "TBMJ <1500";
            pFeatureSelection.SelectFeatures(pQueryFilter, esriSelectionResultEnum.esriSelectionResultNew, false);
            this.axMapControl2.ActiveView.PartialRefresh(esriViewDrawPhase.esriViewGeoSelection, null, null);
            }

