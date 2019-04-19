# ae工具是一种特殊的命令 - 毛小亮 - 博客园
# [ae工具是一种特殊的命令](https://www.cnblogs.com/xianerwonder/p/4255999.html)
itool继承icommand，所以itool工具的调用类似于icommand，而icommand的调用主要是oncreate和onclick方法，oncreate需要传入事件执行的的对象，onclick就相当于在Itoolbar中执行了onclick事件一样。
命令的调用方法：
ICommand replaceBaseMapCommand = new insert_Basemap();
            replaceBaseMapCommand.OnCreate(axMapControl1.Object);
            replaceBaseMapCommand.OnClick();
工具的调用方法：
ICommand pEditorContourPaint=new editor_Paint();
           pEditorContourPaint.OnCreate(axMapControl1.Object); //这句话应该在后面两句的前面，否者会出现执行先后顺序问题
           axMapControl1.CurrentTool = pEditorContourPaint as ITool;
            pEditorContourPaint.OnClick();
另外，可参见http://www.cnblogs.com/xingchen/archive/2011/02/28/1967015.html，介绍的工具和命令的引用。
1、工具号 如放大，需要在mousedown执行,设置CurrentTool，这种AE内置的命令，是需要与用户的进一步互动才能完成后即的操作的
            ICommand pCommand = new ESRI.ArcGIS.Controls.ControlsMapZoomInToolClass();
            pCommand.OnCreate(axMapControl1.Object);
            axMapControl1.CurrentTool = pCommand as ITool;
 2、直接执行 如查看整个视图,需要加OnClick();,不需要设置CurrentTool，这个不需要和用户进行互动，仅仅需要单击一次，就会完成相应的操作
            ICommand pCommand = new ESRI.ArcGIS.Controls.ControlsMapFullExtentCommand();
            pCommand.OnCreate(axMapControl1.Object);
            pCommand.OnClick();//单击菜单，完成相应的操作

