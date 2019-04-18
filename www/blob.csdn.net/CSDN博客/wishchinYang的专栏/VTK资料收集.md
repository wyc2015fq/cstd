# VTK资料收集 - wishchinYang的专栏 - CSDN博客
2013年09月27日 12:45:28[wishchin](https://me.csdn.net/wishchin)阅读数：837
### [使用Qt Creator开发VTK](http://blog.csdn.net/numit/article/details/10200507)
原文链接：[http://blog.csdn.net/numit/article/details/10200507](http://blog.csdn.net/numit/article/details/10200507)
### [VTK应用系列：非常强大！非常善良](http://blog.csdn.net/numit/article/details/11619069)
### [05-VTK在图像处理中的应用(1)](http://blog.csdn.net/www_doling_net/article/details/8541534)
原文链接：http://blog.csdn.net/www_doling_net/article/details/8541534
### [VTK概述](http://blog.csdn.net/numit/article/details/11619069)
原文链接：[http://blog.csdn.net/numit/article/details/11619069](http://blog.csdn.net/numit/article/details/11619069)
#  VTK(visualization toolkit)，直译为可视化工具包，主要用于医学图像重建，带有部分图像处理功能。属于科学性数据分析三维成像的函数库。
# 1.1   vtk历史：
        VTK最早是作为Prentice Hall出版的"TheVisualization Toolkit: An Object-Oriented Approach to 3DGraphics"一书的附件释出，该书及其软件是由奇异研发部Corporate R&D的三位研究人员：WillSchroeder、Ken Martin与Bill Lorensen用其闲暇时间合作。主要用于三维计算机图形学、图像处理和可视化。
1.2   vtk简介：
        Vtk是在面向对象原理的基础上设计和实现的，它的内核是用C++构建的，包含有大约250,000行代码，2000多个类，还包含有几个转换界面，因此也可以自由的通过Java，Tcl/Tk和Python各种语言使用vtk。
       虽然vtk被NA-Mic（美国国家医学影像计算联盟）列为关键计算工具，vtk在医学核试验的研究中也起到重大作用，但是vtk也运用于其他领域，地理信息，建筑学，气象学，航空航天等领域的逼真三维效果图的实现也都能用vtk解决。
1.3  与其他三维引擎对比：
      vtk 更加灵活，封装了c++和OpenGL的代码，简化编程。当vtk需要精确实现曲面时也需要用到OpenGL及c++完善效果。和其他三维引擎绘图不同的是更加注重对数据分析处理后的可视化，而osg，ogre等都是基于场景的绘制。Vtk主要用于科学计算可视化领域，osg主要用于虚拟现实领域。
**2 vtk流程**
vtk模式是基于pipeline设计模式，流线型执行数据流。
Two segment：可视化管线和图形化管线
**可视化管线：**source->[多个filter]形象的说是数据在可视化管线中流动，然后用图显示出数据信息。数据以原始的形式变换成图元形式（vtk内部定义的数据结构，形成数据集）最后以图形形式表现。
**图形化管线：**actor（prop对象的子类）->render->renderWindow->renderWindowInteractor
可视化管线输出的是可被绘制的几何体，图形模型主要是对几何体进行绘制，需要一个接口将两个管线连接起来，就是mapper对象。
**4  vtk简单对象**
vtk的数据处理方式将数据分为数据对象和处理对象。
其中源对象、过滤器对象（可选）、映射器对象 、绘制器对象、绘制窗口为处理对象， Props对象为数据对象（vtk中数据都是由数据对象表示的，数据对象是数据的集合），映射器对象是可视化模型和图形模型的接口。
**1、源对象**
       数据生成的源头，数据来源主要包括从磁盘读取数据文件，如VTK所支持的各种格式文件生成数据源对象，这种源对象被称为读源对象，或者利用数学方法生成源对象，如利用多个四边形构建一个圆柱体，这种对象被称为程序源对象。
**2、过滤器对象**
      对源对象进行处理，生成新的数据集输出。
**3、映射器对象**
     映射器对象主要作用是将可视化模型生成的数据转换到图形模型进行绘制，或者以磁盘文件的形式进行输出。
**5  演示例子**
**[cpp]**[view plain](http://blog.csdn.net/numit/article/details/11619069#)[copy](http://blog.csdn.net/numit/article/details/11619069#)[print](http://blog.csdn.net/numit/article/details/11619069#)[?](http://blog.csdn.net/numit/article/details/11619069#)
```cpp
#include<vtkSmartPointer.h>  
    #include<vtkCylinderSource.h>  
    #include<vtkPolyDataMapper.h>  
    #include<vtkActor.h>  
    #include<vtkProperty.h>  
    #include<vtkRenderer.h>  
    #include<vtkRenderWindow.h>  
    #include<vtkRenderWindowInteractor.h>  
    int main(int, char *[])  
    {  
             vtkSmartPointer<vtkCylinderSource>source =  
                       vtkSmartPointer<vtkCylinderSource>::New();//创建一个source对象  
             source->SetResolution(16);                     //设置source属性  
             source->Update();  
       
             vtkSmartPointer<vtkPolyDataMapper>mapper =  
                       vtkSmartPointer<vtkPolyDataMapper>::New();     //创建映射对象  
             mapper->SetInputConnection(source->GetOutputPort());  //将source的流出变成mapper的流入  
       
             vtkSmartPointer<vtkActor>cylinder =  
                       vtkSmartPointer<vtkActor>::New();           //创建一个演员对象  
             cylinder->SetMapper(mapper);                   //设置演员对象的属性  
             cylinder->SetPosition(10,2,4);  
             cylinder->GetProperty()->SetColor(1,1,0);  
             cylinder->RotateWXYZ(30,1,1,1);  
       
             vtkSmartPointer<vtkRenderer>render =          //创建渲染对象  
                       vtkSmartPointer<vtkRenderer>::New();      
             vtkSmartPointer<vtkRenderWindow>renwin =        //创建渲染窗口  
                       vtkSmartPointer<vtkRenderWindow>::New();  
             renwin->AddRenderer(render);                      //在渲染窗口添加渲染场景。一个窗口可以添加多个场景  
             vtkSmartPointer<vtkRenderWindowInteractor>iren =  
                       vtkSmartPointer<vtkRenderWindowInteractor>::New();  
             render->AddActor(cylinder);                   //在渲染对象上添加actor或其他prop（比如相机，光照）  
              
             iren->SetRenderWindow(renwin);                 //将渲染窗口设置交互属性对象  
       
             renwin->Render();                              //启动渲染  
             iren->Start();                                //启动交互  
             returnEXIT_SUCCESS;  
       
    }
```
**6   vtk可视化执行管线的解释**
       首先数据流执行的原则是上游对象输出的数据类型和下游对象输入的数据类型要一致，注意输入输出数据之间的匹配问题，如果不一致，执行管线就不能正常运行。
       构建好可视化管线后，构成可视化管线的各个对象对数据的处理过程，成为可视化管线的执行，当管线中的过程对象或数据发生改变时，可视化管线都要重新执行，为了保证管线的执行效率，管线在执行过程中必须保持处理对象的同步，VTK采用隐式执行的方式保持同步。
隐式执行包含两个互逆的过程：
1.      Execute()：source->filter->mapper
当可视化管线中的对象创建时间发生改变，表示对象已经更新了，这时执行了execute()
2.      update(): mapper-->filter->source
当renderWindowàRender()发出绘制请求，actor对象将绘制信息给mapper，管线开始执行，然后mapper、filter、source对象依次调用upDate()方法，源对象调用了update()方法后，开始比较当前修改时间和最后执行时间，如果当前被修改的时间比最后执行的时间新，则执行execute()，filter对象和mapper对象比较他们的修改时间和最后执行的时间，根据比较结果看是否需要调用execute()。
