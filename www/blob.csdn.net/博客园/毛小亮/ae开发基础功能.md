# ae开发基础功能 - 毛小亮 - 博客园
# [ae开发基础功能](https://www.cnblogs.com/xianerwonder/p/ae.html)
放大、缩小、pan、框选要素（新建命令，建立命令HOOK，赋给当前地图控件）：
ICommand cmd = new ControlsSelectFeaturesTool();
            cmd.OnCreate(axMapControl1.Object);
            axMapControl1.CurrentTool = cmd as ITool;
取消要素选择（层——要素选择——清空）：
IFeatureSelection pfs = axMapControl1.get_Layer(0) as IFeatureSelection;
            pfs.Clear();
            axMapControl1.Refresh();
            axMapControl1.CurrentTool = null;

