# DEV柱状图----傻瓜版 - 毛小亮 - 博客园
# [DEV柱状图----傻瓜版](https://www.cnblogs.com/xianerwonder/p/4081565.html)
this.chartControl1.Series.Clear();//清空
 //获得数据
            sqldb sqldb = new sqldb();
            string sql = "select T_Cd,T_Hg,T_As,T_Pb,T_Cr,T_Cu,T_Ni,T_Zn from jcd  where OBJECTID_12 = 20";
            DataTable dt = sqldb.GetTable(sql);
            //设置标题和图表类型
            Series series= new Series("重金属元素",ViewType.Bar);
            //数据导入到series
            for (int i = 0; i < dt.Columns.Count; i++)
                {
                SeriesPoint point = new SeriesPoint(dt.Columns[i].ToString(), Convert.ToDouble( Convert.ToDouble(dt.Rows[0][i].ToString()).ToString("0.00")));
                series.Points.Add(point);
                }
            this.chartControl1.Series.Add(series);
            this.chartControl1.Refresh();//刷新
            //this.chartControl1.Show();

