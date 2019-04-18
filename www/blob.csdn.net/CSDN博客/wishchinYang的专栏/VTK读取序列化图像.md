# VTK读取序列化图像 - wishchinYang的专栏 - CSDN博客
2013年09月27日 12:54:55[wishchin](https://me.csdn.net/wishchin)阅读数：2432
[vtk获取内存中图像数据](http://blog.csdn.net/zmy3376365/article/details/7717721)
原文链接：[http://blog.csdn.net/zmy3376365/article/details/7717721](http://blog.csdn.net/zmy3376365/article/details/7717721)
内存中有段图片数据  ，使用VTK来读入，然后就可以通过getoutputport() /getoutput()来获取了
```cpp
vtkImageImport *importer = vtkImageImport::New();
    importer->SetWholeExtent( 0, m_image->width-1, 0, m_image->height-1, 0, 0 );
    importer->SetDataExtentToWholeExtent();
    importer->SetDataScalarTypeToUnsignedChar();
    importer->SetNumberOfScalarComponents(m_image->depth);
    importer->SetImportVoidPointer(m_image->data);
  //  importer->Update();
```
vtk读取dat 、raw等文件，不过文件后缀必须加上.1什么的
```cpp
vtkVolume16Reader *v16 = vtkVolume16Reader::New();
    v16->SetDataDimensions(640, 480); //图像大小
    v16->SetDataByteOrderToLittleEndian();
    v16->SetFilePrefix("./0.dat");
    v16->SetImageRange(1, 1);
    v16->SetDataSpacing(1.0, 1.0, 1.0);
    vtkImageResample *sample = vtkImageResample::New();
    sample->SetInput((vtkDataObject*)v16->GetOutput());
    sample->SetAxisMagnificationFactor(0, 1.2);
    sample->SetAxisMagnificationFactor(1, 1.2);
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
    vtkImageViewer *viewer = vtkImageViewer::New();
    viewer->SetupInteractor(iren);
    viewer->SetInput(sample->GetOutput());
```
**VTKRenderer显示图片进行更新切换:**
**先removeActor**
**然后addActor**
**最后Render()**
### [VTK读取序列化图像](http://blog.csdn.net/wishchin/article/details/12089587)
原文链接：[http://blog.csdn.net/www_doling_net/article/details/8896598](http://blog.csdn.net/www_doling_net/article/details/8896598)
这个更给力了！！
医学图像处理的应用程序中，经常会碰到读取一个序列图像的操作。比如CT、MR等所成的图像都是一个切面一个切面地存储的，医学图像处理程序要处理这些数据，第一步当然是把这些数据从磁盘等外部存储介质中导入内存。
        利用VTK可以读取多种格式的图像文件，支持读取单个的二维图像(比如*.BMP、*.JPEG、*.PNG等)或者三维图像文件(*.VTK、*.mhd、*.mha等)，也支持序列图像文件的导入。下面我们详细地讲解如何在VTK里实现序列图像文件的读取(我们以美国可视人的数据做为测试数据，数据可以从[***这里***](http://public.kitware.com/pub/itk/Data/VisibleWomanHead/)下载到)。
在讲解VTK序列图像读取之前，有一个问题需要注意的：就是待读取的序列图像的文件名必须是规则的，比如像下图所示的。
***Tips***：涉及到指改文件名的，如果熟悉Dos命令的，直接用rename命令就可以完成；或者是使用一个小软件Renamer来修改。
![](https://img-my.csdn.net/uploads/201305/07/1367923493_3320.png)
图1序列图像的命名示例
**方法一**：使用SetFileNames()方法读取序列图像。
```cpp
//生成图像序列的文件名数组  
    vtkSmartPointer<vtkStringArray > fileArray =  
    vtkSmartPointer<vtkStringArray >::New();  
    charfileName[128];  
    for(inti = 1; i < 20; i++) //几个图像就循环几次  
    {  
    sprintf(fileName,"../VisibleWomanHead/VisibleWomanHead_%02d.jpg", i);  
    vtkstd::stringfileStr(fileName);  
    fileArray->InsertNextValue(fileStr);     
    }  
    //读取JPG序列图像  
    vtkSmartPointer<vtkJPEGReader>reader =  
    vtkSmartPointer<vtkJPEGReader>::New();  
    reader->SetFileNames(fileArray);  
    reader->Update();
```
说明：要读取的序列文件是存放在文件夹VisibleWomanHead里的，而且每个图像的命名规则是：VisibleWomanHead_01.jpg，VisibleWomanHead_02.jpg……如图1所示。方法一我们使用了vtkStringArray先构建一个包含要读取的序列图像文件的文件名数组，然后调用vtkJPEGReader里的SetFileNames()方法。方法SetFileNames()是在类vtkImageReader2里实现的，所以大部分VTK图像的读取类都可以用这种方法来读取序列图像文件。
**方法二**：使用SetFilePrefix()/SetFilePattern()方法读取序列图像。
前面我们已经提过，在读取序列图像文件时，要求序列里的文件名有规律地命名(如图1)，既然这些文件名有规律，我们可以采用vtkImageReader2里的方法SetFilePrefix()/SetFilePattern()来读取序列图像。
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8896598#)[copy](http://blog.csdn.net/www_doling_net/article/details/8896598#)
```cpp
vtkSmartPointer<vtkJPEGReader>reader =  
    vtkSmartPointer<vtkJPEGReader>::New();  
    reader->SetFilePrefix ("D:/Data/VisibleWomanHead/VisibleWomanHead_");  
    reader->SetFilePattern("%s%02d.jpg");  
    reader ->SetDataExtent (0,511,0,511,1,20);//图像大小是512*512  
    reader->Update();
```
至于为什么这么写，你们感受一下就知道了，特别是SetFilePrefix()、SetFilePattern()、SetDataExtent()这几个函数的参数。
**方法二**：一张一张地读入，然后合并成一个三维的数据体。
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8896598#)[copy](http://blog.csdn.net/www_doling_net/article/details/8896598#)
```cpp
vtkSmartPointer<vtkImageAppend > append =  
           vtkSmartPointer<vtkImageAppend >::New();  
    append->SetAppendAxis(2);  
       
    vtkSmartPointer<vtkJPEGReader>reader =  
           vtkSmartPointer<vtkJPEGReader>::New();  
    char fileName[128];  
    for(int i = 1; i < 21; i++)  
    {  
           sprintf(fileName,"D:/Data/VisibleWomanHead/VisibleWomanHead_%02d.jpg", i);  
           reader->SetFileName(fileName);  
           append->AddInputConnection(reader->GetOutputPort());  
    }
```
我们使用类vtkImageAppend做合并的操作，其中方法SetAppendAxis(2)是指定Z轴为读入的每层图像数据的堆叠方向。其他的代码你们继续感受一下，就不再叙述了。
**==========欢迎转载，转载时请保留该声明信息==========版权归[@东灵工作室](http://weibo.com/dolingstudio)所有，更多信息请访问[东灵工作室](http://www.doling.net)教程系列导航：[http://blog.csdn.net/www_doling_net/article/details/8763686](http://blog.csdn.net/www_doling_net/article/details/8763686)**
