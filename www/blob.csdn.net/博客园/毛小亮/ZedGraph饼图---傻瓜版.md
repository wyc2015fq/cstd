# ZedGraph饼图---傻瓜版 - 毛小亮 - 博客园
# [ZedGraph饼图---傻瓜版](https://www.cnblogs.com/xianerwonder/p/4079177.html)
GraphPane pGraphPane=this.zedGraphControl1.GraphPane;//调用饼图类
            pGraphPane.Title.Text = "重金属含量分析图";//饼图标题
//下面是饼图各个饼块的设置
            PieItem segment1 = pGraphPane.AddPieSlice(20, Color.Navy, Color.White, 45f, 0, "North");
            PieItem segment3 = pGraphPane.AddPieSlice(30, Color.Purple, Color.White, 45f, .0, "East");
            PieItem segment4 = pGraphPane.AddPieSlice(10.21, Color.LimeGreen, Color.White, 45f, 0, "West");
            PieItem segment2 = pGraphPane.AddPieSlice(40, Color.SandyBrown, Color.White, 45f, 0.2, "South");
            PieItem segment6 = pGraphPane.AddPieSlice(250, Color.Red, Color.White, 45f, 0, "Europe");
            PieItem segment7 = pGraphPane.AddPieSlice(50, Color.Blue, Color.White, 45f, 0.2, "Pac Rim");
            PieItem segment8 = pGraphPane.AddPieSlice(400, Color.Green, Color.White, 45f, 0, "South America");
            PieItem segment9 = pGraphPane.AddPieSlice(50, Color.Yellow, Color.White, 45f, 0.2, "Africa");
this.zedGraphControl1.AxisChange();//用于终断刷新
*            this.zedGraphControl1.Refresh();*
//PieItem segment6 = myPane.AddPieSlice( 250, Color.Red, Color.White, 45f, 0, "Europe" );
//Pie重载了五个构造函数，上面是参数最多的一个构造函数，共有六个，意思分别是：在整个饼形图中占的比重，渐变颜色1，渐变颜色2，渐变颜色角度，远离中心点的距离，饼形图的文字注释。

