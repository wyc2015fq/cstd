# PCL： Delaunay三角剖分（关于三维实体分块思想总结） - wishchinYang的专栏 - CSDN博客
2016年05月18日 19:24:57[wishchin](https://me.csdn.net/wishchin)阅读数：1227
       引言：实体造型 里面讲述的都是相对全局的方法介绍，主要应用于特定领域，在特定领域能达到相当好的结果；
**一：关于模型细分化 及其运算**
（转自于百科）
目前常用的[实体](http://baike.baidu.com/view/21996.htm)表示方法主要有：边界表示法(BRep)、构造实体几何法(CSG)和扫描法。
(1) 三维形体在计算机内的常用表示法 
对于三维几何元素或简称三维形体，最常用的表示法有两种：CSG和BRep。 
① CSG表示法：先定义一些形状比较简单的常用[体素](http://baike.baidu.com/view/1237346.htm)，如方块、[圆柱](http://baike.baidu.com/view/449736.htm)、圆锥、球、[棱柱](http://baike.baidu.com/view/781432.htm)等。然后用集合运算并、交、差把[体素](http://baike.baidu.com/view/1237346.htm)修改成复杂形状的形体。早期的CSG模型仅使用代数方程及半空间的概念，[体素](http://baike.baidu.com/view/1237346.htm)只支持[多面体](http://baike.baidu.com/view/227150.htm)与[二次曲面](http://baike.baidu.com/view/107720.htm)体，而不支持表面含有自由曲面的[实体](http://baike.baidu.com/view/21996.htm)。整个模型是棵[树结构](http://baike.baidu.com/view/1661473.htm)，最终形体的表面交线与有效区域没有显式给出，不能直接用于[NC](http://baike.baidu.com/view/114011.htm)加工与[有限元分析](http://baike.baidu.com/view/103834.htm)等后继处理。
② BRep表示法：用点、边、面、环以及它们之间相互的邻接关系定义三维[实体](http://baike.baidu.com/view/21996.htm)，形体表面、边界线、交线等都显式给出。但是生成个别形体的过程相当复杂、不直观，不可能由用户直接操作。它的优点是能支持所有类型的[曲面](http://baike.baidu.com/view/324917.htm)作为形体表面。能直接支持[NC](http://baike.baidu.com/view/114011.htm)加工与[有限元分析](http://baike.baidu.com/view/103834.htm)等，故其优缺点恰与CSG模型相反。后来，人们转向使用CSG与BRep的[混合模型](http://baike.baidu.com/view/1867767.htm)。
③ CSG与BRep的[混合模型](http://baike.baidu.com/view/1867767.htm)表示法：用CSG作为高层次抽象的数据模型，用BRep作为低层次的具体表示形式。CSG树的[叶子结点](http://baike.baidu.com/view/2335663.htm)除了存放传统的[体素](http://baike.baidu.com/view/1237346.htm)的参数定义，还存放该体素的BRep表示。CSG树的中间结点表示它的各子树的运算结果。用这样的[混合模型](http://baike.baidu.com/view/1867767.htm)对用户来说十分直观明了，可以直接支持基于特征的[参数化](http://baike.baidu.com/view/3814792.htm)造型功能，而对于形体加工，分析所需要的边界、[交线](http://baike.baidu.com/view/2078705.htm)、表面不仅可显式表示，且能够由低层的BRep直接提供。
(2) 三维形体的集合运算 
通常一个形体是由两个或两个以上较简单的形体(称之为[体素](http://baike.baidu.com/view/1237346.htm))经过集合运算得到的集合运算子包括并、交、差。设A和B是两个用BRep表示描述的维数一致的[多面体](http://baike.baidu.com/view/227150.htm)，集合运算结果形体C=AB的步骤可简介如下：
① 确定集合运算两形体之间的关系：形体边界表示BRep结构中的面、边、点之间的基本分类关系分别是"点在面上"、"点在边上"、"两点重合"、"边在面上"、"两边共线"、"两个多边形共面"等六种关系。先用数值计算确定"点在面上"的关系，其余五种关系可以根据"点在面上"关系推导出来。当这些关系发生冲突时，就用[推理](http://baike.baidu.com/view/105591.htm)的方法解决冲突。
② 进行边、体分类：对A形体上的每一条边，确定对B形体的分类关系(A在B形体内、外、上面、相交等)；同样对B形体上的每一条边，确定对A形体的分类关系。
③ 计算多边形的[交线](http://baike.baidu.com/view/2078705.htm)：对于A形体上的多边形PA和B形体上的每一个多边形PB，计算它们的交线。
④ 构造新形体C表面上的边：对于A形体上和B形体上的每一个多边形PA、PB，根据集合运算的[算子](http://baike.baidu.com/view/53313.htm)收集多边形PA的边与另一个[多面体](http://baike.baidu.com/view/227150.htm)表面多边形PB的交线以生成新形体C表面的边，如果多边形PA上[有边](http://baike.baidu.com/view/8254305.htm)被收集到新形体C的表面，则PA所有的平面将成为新形体C表面上的一个平面，多边形PA的一部分或全部则成为新形体C的一个或多个多边型。如果定义了两个形体A和B的完整边界，那么形体C的完整边界就是A和B边界各部分的总和。
⑤ 构造多边形的面：对新形体C上的每一个面，将其边排序构成多边形面环。 
⑥ 合法性检查：检查形体C的BRep表示的合法性。
**二：模型的表面细分化**：
1.1. 曲面的超像素表示方法：
       利用模型表面即曲面的表层起伏特征，把三维曲面转化为二维灰度图像；再利用分割的方法，把图像进行分割成小块；再映射到三维曲面，完成三维曲面分割，形成体元颗粒模型；
1.2. 模型体元的链接表示：  
**三：点云表面的Delaunay三角剖分**
 强烈推荐原文，我只摘抄代码![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)
 文章链接：[OpenCascade中的Delaunay三角剖分](http://www.cppblog.com/eryar/archive/2013/05/26/OpenCascade_Delaunay_Triangulation.html)
摘要：本文简要介绍了Delaunay三角剖分的基础理论，并使用OpenCascade的三角剖分算法将边界BRep表示的几何体进行三角离散化后在OpenSceneGraph中显示。
关键字：**Delaunay Triangulation、OpenCascade、OpenSceneGraph**
....................................................................................................................................
## Delaunay三角剖分在OpenCascade的应用 
OpenCascade中网格剖分的包主要有BRepMesh、MeshAlgo、MeshVS，其中，类MeshAlgo_Delaunay使用算法Watson来进行Delaunay三角剖分。从类StlTransfer中的注释The triangulation is computed with the Delaunay algorithm implemented in package BRepMesh.可以看出包BRepMesh就是Delaunay三角剖分的具体实现。使用方法如下：
BRepMesh::Mesh (aShape, Deflection); 
这个函数主要是用来对拓扑形状进行三角剖分。以下通过将一个圆柱三角剖分为例说明如何将一个拓扑形状进行三角剖分并将结果进行可视化。
```cpp
/** 
*    Copyright (c) 2013 eryar All Rights Reserved. 
* 
*        File    : Main.cpp 
*        Author  : eryar@163.com 
*        Date    : 2013-05-26 
*        Version : 0.1 
* 
*    Description : Use BRepMesh_Delaun class to learn  
*                  Delaunay's triangulation algorithm. 
* 
*/ 
// Open Cascade library. 
#include <gp_Pnt.hxx> 
#include <gp_Pln.hxx> 
#include <BRep_Tool.hxx> 
#include <TopoDS.hxx> 
#include <TopoDS_Edge.hxx> 
#include <TopoDS_Wire.hxx> 
#include <TopoDS_Face.hxx> 
#include <BRepBuilderAPI_MakeEdge.hxx> 
#include <BRepBuilderAPI_MakeWire.hxx> 
#include <BRepBuilderAPI_MakeFace.hxx> 
#include <BRepPrimAPI_MakeBox.hxx> 
#include <BRepPrimAPI_MakeCone.hxx> 
#include <BRepPrimAPI_MakeCylinder.hxx> 
#include <BRepPrimApI_MakeSphere.hxx> 
#include <BRepMesh.hxx> 
#include <TopExp_Explorer.hxx> 
#include <Poly_Triangulation.hxx> 
#include <TShort_Array1OfShortReal.hxx> 
#pragma comment(lib, "TKernel.lib") 
#pragma comment(lib, "TKMath.lib") 
#pragma comment(lib, "TKBRep.lib") 
#pragma comment(lib, "TKPrim.lib") 
#pragma comment(lib, "TKMesh.lib") 
#pragma comment(lib, "TKTopAlgo.lib") 
// OpenSceneGraph library. 
#include <osgDB/ReadFile> 
#include <osgViewer/Viewer> 
#include <osgViewer/ViewerEventHandlers> 
#include <osgGA/StateSetManipulator> 
#pragma comment(lib, "osgd.lib") 
#pragma comment(lib, "osgDbd.lib") 
#pragma comment(lib, "osgGAd.lib") 
#pragma comment(lib, "osgViewerd.lib") 
osg::Node* BuildShapeMesh(const TopoDS_Shape& aShape) 
{ 
    osg::ref_ptr<osg::Group> root = new osg::Group(); 
    osg::ref_ptr<osg::Geode> geode = new osg::Geode(); 
    osg::ref_ptr<osg::Geometry> triGeom = new osg::Geometry(); 
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(); 
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(); 
    BRepMesh::Mesh(aShape, 1); 
    TopExp_Explorer faceExplorer; 
for (faceExplorer.Init(aShape, TopAbs_FACE); faceExplorer.More(); faceExplorer.Next()) 
{ 
        TopLoc_Location loc; 
        TopoDS_Face aFace = TopoDS::Face(faceExplorer.Current()); 
        Handle_Poly_Triangulation triFace = BRep_Tool::Triangulation(aFace, loc); 
        Standard_Integer nTriangles = triFace->NbTriangles(); 
        gp_Pnt vertex1; 
        gp_Pnt vertex2; 
        gp_Pnt vertex3; 
        Standard_Integer nVertexIndex1 = 0; 
        Standard_Integer nVertexIndex2 = 0; 
        Standard_Integer nVertexIndex3 = 0; 
        TColgp_Array1OfPnt nodes(1, triFace->NbNodes()); 
        Poly_Array1OfTriangle triangles(1, triFace->NbTriangles()); 
        nodes = triFace->Nodes(); 
        triangles = triFace->Triangles(); 
for (Standard_Integer i = 1; i <= nTriangles; i++) 
{ 
            Poly_Triangle aTriangle = triangles.Value(i); 
            aTriangle.Get(nVertexIndex1, nVertexIndex2, nVertexIndex3); 
            vertex1 = nodes.Value(nVertexIndex1); 
            vertex2 = nodes.Value(nVertexIndex2); 
            vertex3 = nodes.Value(nVertexIndex3); 
            gp_XYZ vector12(vertex2.XYZ() - vertex1.XYZ()); 
            gp_XYZ vector13(vertex3.XYZ() - vertex1.XYZ()); 
            gp_XYZ normal = vector12.Crossed(vector13); 
            Standard_Real rModulus = normal.Modulus(); 
if (rModulus > gp::Resolution()) 
{ 
                normal.Normalize(); 
} 
else 
{ 
                normal.SetCoord(0., 0., 0.); 
} 
            vertices->push_back(osg::Vec3(vertex1.X(), vertex1.Y(), vertex1.Z())); 
            vertices->push_back(osg::Vec3(vertex2.X(), vertex2.Y(), vertex2.Z())); 
            vertices->push_back(osg::Vec3(vertex3.X(), vertex3.Y(), vertex3.Z())); 
            normals->push_back(osg::Vec3(normal.X(), normal.Y(), normal.Z())); 
} 
} 
    triGeom->setVertexArray(vertices.get()); 
    triGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, vertices->size())); 
    triGeom->setNormalArray(normals); 
    triGeom->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE); 
    geode->addDrawable(triGeom); 
    root->addChild(geode); 
return root.release(); 
} 
int main(int argc, char* argv[]) 
{ 
    osgViewer::Viewer myViewer; 
    osg::ref_ptr<osg::Group> root = new osg::Group(); 
    root->addChild(BuildShapeMesh(BRepPrimAPI_MakeCylinder(.6, 1))); 
    myViewer.setSceneData(root); 
    myViewer.addEventHandler(new osgGA::StateSetManipulator(myViewer.getCamera()->getOrCreateStateSet())); 
    myViewer.addEventHandler(new osgViewer::StatsHandler); 
    myViewer.addEventHandler(new osgViewer::WindowSizeHandler); 
return myViewer.run(); 
}
```
运行结果如下：
![](https://img-blog.csdn.net/20170819164551545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Figure 4.1 Cylinder mesh generated by BRepMesh::Mesh
