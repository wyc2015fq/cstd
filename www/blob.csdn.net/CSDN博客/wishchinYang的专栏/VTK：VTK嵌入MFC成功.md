# VTK：VTK嵌入MFC成功 - wishchinYang的专栏 - CSDN博客
2015年05月21日 09:55:54[wishchin](https://me.csdn.net/wishchin)阅读数：2636
          VTK作为医学显示库，得到较多使用。作为较为上层的设计，对OpenGL进行了封装，并且有Windows、Linux、安卓等开发版本，可移植性较强。
          不过VES暂时没有编译成功。
          以下是嵌入MFC-IDC代码，有个缺陷，暂时没能修改。
代码：
```cpp
showPointsInVtk(short* &depthImage, int HeightDepth, int WidthDepth,int IDCX)
{
	
	//double arr[3];
	vtkPoints * points = vtkPoints::New();
	int n = 0; int idx = 0; int Length = 3 * HeightDepth*WidthDepth;
	//首先读取点云数据到点表points同时指定点对应的id:
	for (int i = 0; i < HeightDepth; ++i){
		int LastPixel = i* WidthDepth;
		for (int j = 0; j < WidthDepth; ++j){
			int y = depthImage[3 * (j + LastPixel)];
			int x = depthImage[3 * (j + LastPixel) +1];
			int z = depthImage[3 * (j + LastPixel) +2];
			points->InsertPoint(n, x, y, z);
			n++;
		}
	}
	printf("%d\n", n);
	vtkPolyVertex * polyvertex = vtkPolyVertex::New();
	polyvertex->GetPointIds()->SetNumberOfIds(n);
        int i = 0; //建立拓扑关系
        for (i = 0; i < n; i++){
		polyvertex->GetPointIds()->SetId(i, i);
	}
	vtkUnstructuredGrid * grid = vtkUnstructuredGrid::New();
	grid->SetPoints(points);
	grid->InsertNextCell(polyvertex->GetCellType(),
		polyvertex->GetPointIds());
	vtkDataSetMapper *map1 = vtkDataSetMapper::New();
	map1->SetInputData(grid);
	vtkActor *actor1 = vtkActor::New();
	actor1->SetMapper(map1);
	actor1->GetProperty()->SetColor(0.194, 0.562, 0.75);
	vtkRenderer *ren = vtkRenderer::New();
	ren->AddActor(actor1);
	ren->SetBackground(0, 0, 0);
	//renWin和iren分别是vtkWin32OpenGLRenderWindow, vtkWin32RenderWindowInteractor
	vtkWin32RenderWindowInteractor        *iren = vtkWin32RenderWindowInteractor::New();
	vtkWin32OpenGLRenderWindow             *win = vtkWin32OpenGLRenderWindow::New();
	vtkInteractorStyleTrackballCamera    *style = vtkInteractorStyleTrackballCamera::New();
	win->Clean();
	win->SetSize(480, 360);
	win->BordersOn();
	win->AddRenderer(ren);
	iren->SetRenderWindow(win);
	iren->SetInteractorStyle(style);
	//搭上一个显示用的MFC窗口框架。
         CWnd *viewer_Wnd;
        viewer_Wnd = this->GetDlgItem(IDCX);   
         HWND hWnd = (HWND)viewer_Wnd->m_hWnd;
         HWND hParent = ::GetParent(hWnd);
        //::ShowWindow(hParent, SW_HIDE);
        ::ShowWindow(hParent, 1);
        win->SetParentId(this->m_hWnd);
        iren->Start();
        map1->Delete();
        grid->Delete();
       actor1->Delete();
        polyvertex->Delete();
        ren->Delete();
        win->Delete();
        iren->Delete();
        return 1;
}
```
显示效果：
![](https://img-blog.csdn.net/20150521150620864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
** 注意事项：**
使用
```cpp
iren->Start();
```
导致renter不能更行，修改为以下代码可以完成动态更新：
把 render和win放在初始对话框初始化，其他代码修改为：
```cpp
this->win->Render();
```
```cpp
//在 对话框初始化时初始化Render
inline int Ciisu2vrMfcsDlg::showPointsInVtkG(short* &depthImage, int HeightDepth, int WidthDepth, int IDCX)
{
	//double arr[3];
	vtkPoints * points = vtkPoints::New();
	int n = 0; int idx = 0; int Length = 3 * HeightDepth*WidthDepth;
	//首先读取点云数据到点表points同时指定点对应的id:
	for (int i = 0; i < HeightDepth; ++i){
		int LastPixel = i* WidthDepth;
		for (int j = 0; j < WidthDepth; ++j){
			int y = depthImage[3 * (j + LastPixel)];
			int x = depthImage[3 * (j + LastPixel) + 1];
			int z = depthImage[3 * (j + LastPixel) + 2];//这里压缩四倍是比较好的方法//int z = (depthImage[(j + LastPixel)]) ;
			points->InsertPoint(n, x, y, z);
			n++;
		}
	}
	vtkPolyVertex * polyvertex = vtkPolyVertex::New();
	polyvertex->GetPointIds()->SetNumberOfIds(n);
	int i = 0;
	//建立拓扑关系
	for (i = 0; i < n; i++){
		polyvertex->GetPointIds()->SetId(i, i);
	}
	vtkUnstructuredGrid * grid = vtkUnstructuredGrid::New();
	grid->SetPoints(points);
	grid->InsertNextCell(polyvertex->GetCellType(),
		polyvertex->GetPointIds() );
	vtkDataSetMapper *map = vtkDataSetMapper::New();
	map->SetInputData(grid);
	vtkActor *actorG = vtkActor::New();
	actorG->SetMapper(map);
	actorG->GetProperty()->SetColor(0.194, 0.562, 0.75);
	vtkRenderer *ren = vtkRenderer::New();
	ren->AddActor(actorG);
	ren->SetBackground(0.13, 0.40, 0.15);
	this->win->AddRenderer(ren);
	this->win->Render();//自己写一个重载-更新函数！
	map->Delete();
	grid->Delete();
	actorG->Delete();
	polyvertex->Delete();
	return 1;
}
```
后记：
        效果还不错，就是和OpenCV结合时有点麻烦，命名空间冲突。
