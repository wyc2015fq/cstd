# XFile网格的应用（1） （转载） - xqhrs232的专栏 - CSDN博客
2009年07月12日 15:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1016
三维物体表面的各个剖分三角形构成了一个所谓的网格(Mesh)，使用3D建模软件绘制三维图象，可取得网格的顶点坐标，顶点纹理坐标以及三角形面的材质等数据，并可将这些数据保存到相应的三维图象文件中。此时调用DirectX提供的网格接口函数，读取三维图象文件的顶点数据，就可对三维物体表面进行渲染处理。
.X文件的基本格式
使用 DirectX SDK安装目录下的MView.exe可以打开.X文件，比如我的电脑上的可执行文件路径是E:/Microsoft DirectX 9.0 SDK (April 2005)/Utilities/Bin/x86/MView.exe。 （注: 最新版的DirectX SDK已经不包含该文件了，所以你可能需要下载安装旧版的DirectX SDK来得到这个文件。）
下面是使用MeshView打开一个.X文件的截图：
![](http://www.cppblog.com/images/cppblog_com/lovedday/4157/r_mesh_view1.jpg)
我们来看看下面这个.X文件的具体信息描述，该文件描述与上面那个截图无关。 
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->xof 0302txt 0064
template Header {
<3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}
template Vector {
<3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}
template Coords2d {
<F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}
template Matrix4x4 {
<F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}
template ColorRGBA {
<35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}
template ColorRGB {
<D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}
template IndexedColor {
<1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}
template Boolean {
<4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}
template Boolean2d {
<4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}
template MaterialWrap {
<4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}
template TextureFilename {
<A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}
template Material {
<3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}
template MeshFace {
<3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}
template MeshFaceWraps {
<4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}
template MeshTextureCoords {
<F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}
template MeshMaterialList {
<F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material <3D82AB4D-62DA-11cf-AB39-0020AF71E433>]
}
template MeshNormals {
<F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}
template MeshVertexColors {
<1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}
template Mesh {
<3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}
template FrameTransformMatrix {
<F6F23F41-7686-11cf-8F52-0040333594A3>
 Matrix4x4 frameMatrix;
}
template Frame {
<3D82AB46-62DA-11cf-AB39-0020AF71E433>
 [...]
}
Frame Scene_Root {    // 场景主框架
    FrameTransformMatrix {    // 主框架的变换矩阵
    0.000000, 0.000000, 0.000000, 0.000000, 
    0.000000, 1.000000, 0.000000, 0.000000, 
    0.000000, 0.000000, 1.000000, 0.000000, 
    0.000000, 0.000000, 0.000000, 1.000000;;
    }
    Frame Quad {    // 主框架下的Quad框架
    Mesh {    // 网格数据
        4;    // 顶点数
       -0.500000; -0.500000; 0.000000;,    // 顶点1坐标
        0.500000;  -0.500000; 0.000000;,    // 顶点2坐标
        -0.500000;  0.500000; 0.000000;,    // 顶点3坐标
        0.500000;   0.500000; 0.000000;;    // 顶点4坐标
        2;    // 三角形面数
        3; 0,1,2;,    // 第一个三角形面的顶点索引数和三个顶点索引值
        3; 0,2,1;;    // 第二个三角形面的顶点索引数和三个顶点索引值                    
         MeshNormals {
        4;    // 顶点法向量的个数
        0.000000; 0.000000; 1.000000;,    // 顶点1的法向量坐标
        0.000000; 0.000000; 1.000000;,    // 顶点2的法向量坐标
        0.000000; 0.000000; 1.000000;,    // 顶点3的法向量坐标
        0.000000; 0.000000; 1.000000;;    // 顶点4的法向量坐标
        2;    // 三角形面的法向量个数
        3; 0, 1, 2;,    // 法向量的索引数和3个法向量的索引值（计算面的法向量）
        3; 3, 2, 1;;    // 法向量的索引数和3个法向量的索引值（计算面的法向量）
        }
        MeshTextureCoords {    // 纹理坐标模版
        4;    // 纹理坐标个数
        0.000000; 1.000000;,    // 纹理坐标1
        1.000000; 1.000000;,    // 纹理坐标2
        0.000000; 0.000000;,    // 纹理坐标3
        1.000000; 0.000000;;    // 纹理坐标4                        
        }
        MeshMaterialList {    // 材质列表模版
        1;    // 材质个数
        2;    // 三角形面的材质索引数（实际是三角形面的个数）
        0,    // 第一个三角形使用0号材质索引
        0,    // 第二个三角形使用0号材质索引    
        Material{           // 材质模版
            1.000000; 1.000000; 1.000000; 1.000000;;       // 三角形面的颜色          
            0.000000;               // 镜面光的高光强度
            1.000000; 1.000000; 1.000000;; // 镜面光颜色
            0.000000; 0.000000; 0.000000;; // 自发射光颜色
            TextureFilename { // 纹理贴图文件
            "bb.jpg";
            };
        }        
        }
    }
    }
}
第一行的“xof 0302txt 0064”表明这个文件是一个.X 文件(由"xof"子串解析)，文件的版本为3.2(由“0302”子串解析)，数据以文本格式进行存放(由"txt"子串解析)，所使用的浮点数是64 位的浮点数据(由"0064"子串进行解析)。如果第一行的内容为“xof 0303bin 0032”，则表示该.X文件是一个二进制文件，此时将不能直接查看文件数据的含义。
接下来是 Scene_Root对象的定义，该对象为一个Frame模版数据，用Frame Scene_Root引出，在随后的配对大括号"{"和"}"内，定义Scene_Root对象的具体数据。这里，作为数据类型的Frame模版同样提供了数据正确性的检测方法和它的对象数据的说明方法。
Frame模版用来定义游戏场景的各个框架部分，通过在框架中定义子框架，就可形成具有层次关系的一系列框架。Frame模版在DirectX中定义如下：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template Frame {
<3D82AB46-62DA-11cf-AB39-0020AF71E433>
 [...]
}
关键字"template"表明下面是一个模版的定义，Frame为该模版的名字，每个模版都有一个唯一的全局标志符，随后的 [...]说明Frame模版是一个开放模版(Open Template)，开放模版的对象可以定义其他模版类型的数据。
在Scene_Root对象中，定义了一个省略了对象名的FrameTransformMatrix模版对象如下： 
FrameTransformMatrix {    // 主框架的变换矩阵
    0.000000, 0.000000, 0.000000, 0.000000, 
    0.000000, 1.000000, 0.000000, 0.000000, 
    0.000000, 0.000000, 1.000000, 0.000000, 
    0.000000, 0.000000, 0.000000, 1.000000;;
}
FrameTransformMatrix模版用来指定框架的变换矩阵，它的定义如下所示：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template FrameTransformMatrix {
<F6F23F41-7686-11cf-8F52-0040333594A3>
 Matrix4x4 frameMatrix;
}
template Matrix4x4 {
<F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}
在Scene_Root框架对象内定义了另一个Quad框架对象如下： 
Mesh {    // 网格数据
        4;    // 顶点数
       -0.500000; -0.500000; 0.000000;,    // 顶点1坐标
        0.500000;  -0.500000; 0.000000;,    // 顶点2坐标
        -0.500000;  0.500000; 0.000000;,    // 顶点3坐标
        0.500000;   0.500000; 0.000000;;    // 顶点4坐标
        2;    // 三角形面数
        3; 0,1,2;,    // 第一个三角形面的顶点索引数和三个顶点索引值
        3; 0,2,1;;    // 第二个三角形面的顶点索引数和三个顶点索引值                    
        MeshNormals {
        4;    // 顶点法向量的个数
        0.000000; 0.000000; 1.000000;,    // 顶点1的法向量坐标
        0.000000; 0.000000; 1.000000;,    // 顶点2的法向量坐标
        0.000000; 0.000000; 1.000000;,    // 顶点3的法向量坐标
        0.000000; 0.000000; 1.000000;;    // 顶点4的法向量坐标
        2;    // 三角形面的法向量个数
        3; 0, 1, 2;,    // 法向量的索引数和3个法向量的索引值（计算面的法向量）
        3; 3, 2, 1;;    // 法向量的索引数和3个法向量的索引值（计算面的法向量）
        }
        MeshTextureCoords {    // 纹理坐标模版
        4;    // 纹理坐标个数
        0.000000; 1.000000;,    // 纹理坐标1
        1.000000; 1.000000;,    // 纹理坐标2
        0.000000; 0.000000;,    // 纹理坐标3
        1.000000; 0.000000;;    // 纹理坐标4                        
        }
        MeshMaterialList {    // 材质列表模版
        1;    // 材质个数
        2;    // 三角形面的材质索引数（实际是三角形面的个数）
        0,    // 第一个三角形使用0号材质索引
        0,    // 第二个三角形使用0号材质索引    
        Material{           // 材质模版
            1.000000; 1.000000; 1.000000; 1.000000;;       // 三角形面的颜色          
            0.000000;               // 镜面光的高光强度
            1.000000; 1.000000; 1.000000;; // 镜面光颜色
            0.000000; 0.000000; 0.000000;; // 自发射光颜色
            TextureFilename { // 纹理贴图文件
            "bb.jpg";
            };
        }        
        }
    }
该对象提供了正方形网格(Mesh)的数据说明，如下所示： 
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template Mesh {   // 网格模版定义
<3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;                       // 顶点数
 array Vector vertices [nVertices];   // 顶点数组
 DWORD nFaces;                          // 三角形面数
 array MeshFace faces[nFaces];     // 三角形面数组
 [...]   // 开放模版定义
}
template MeshFace {
<3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;                                          // 三角形面的顶点索引数，一般为3。
 array DWORD faceVertexIndices[nFaceVertexIndices];    // 三角形面内各个顶点的索引值
}
而在Mesh数据里又定义了一个模板数据MeshNormals如下：
MeshNormals {
        4;    // 顶点法向量的个数
        0.000000; 0.000000; 1.000000;,    // 顶点1的法向量坐标
        0.000000; 0.000000; 1.000000;,    // 顶点2的法向量坐标
        0.000000; 0.000000; 1.000000;,    // 顶点3的法向量坐标
        0.000000; 0.000000; 1.000000;;    // 顶点4的法向量坐标
        2;    // 三角形面的法向量个数
        3; 0, 1, 2;,    // 法向量的索引数和3个法向量的索引值（计算面的法向量）
        3; 3, 2, 1;;    // 法向量的索引数和3个法向量的索引值（计算面的法向量）
}        
来看看MeshNormals对象的模版数据说明，定义了顶点和三角形面的法向量，如下所示：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template MeshNormals {
<F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;                                             // 顶点法向量个数
 array Vector normals [nNormals];                        // 顶点法向量数组
 DWORD nFaceNormals;                                      // 三角形面法向量个数
 array MeshFace  faceNormals[nFaceNormals];    // 三角形面法向量数组
}
template Vector {
<3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}
template MeshFace {
<3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}
在Mesh数据里又定义了一个正方形的纹理贴图模板数据MeshTextureCoords以指定纹理坐标如下： 
MeshTextureCoords {    // 纹理坐标模版
        4;    // 纹理坐标个数
        0.000000; 1.000000;,    // 纹理坐标1
        1.000000; 1.000000;,    // 纹理坐标2
        0.000000; 0.000000;,    // 纹理坐标3
        1.000000; 0.000000;;    // 纹理坐标4
} 
来看看MeshTextureCoords模版的定义：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template MeshTextureCoords {
<F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;                                       // 纹理坐标数
 array Coords2d textureCoords [nTextureCoords];    // 纹理坐标数组
}
template Coords2d {
<F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;   // 纹理U坐标
 FLOAT v;   // 纹理V坐标
}
在Mesh数据里还定义了一个模板数据MeshMaterialList以指示材质信息： 
MeshMaterialList {    // 材质列表模版
        1;    // 材质个数
        2;    // 三角形面的材质索引数（实际是三角形面的个数）
        0,    // 第一个三角形使用0号材质索引
        0,    // 第二个三角形使用0号材质索引    
        Material{           // 材质模版
            1.000000; 1.000000; 1.000000; 1.000000;;       // 三角形面的颜色          
            0.000000;               // 镜面光的高光强度
            1.000000; 1.000000; 1.000000;; // 镜面光颜色
            0.000000; 0.000000; 0.000000;; // 自发射光颜色
            TextureFilename { // 纹理贴图文件
            "bb.jpg";
            };
        }        
}
MeshMaterialList模版说明该Mesh网格所使用的材质数目，每个三角形面对应的材质索引和Material材质对象。在MeshMaterialList模版中，用[Material
<3D82AB4D-62DA-11cf-AB39-0020AF71E433>]来指明MeshMaterialList模版的对象可包含全局标志符为"3D82AB4D-62DA-11cf-AB39-0020AF71E433"的Material模版对象（可以不止一个对象）。
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template MeshMaterialList {   // 材质列表模版
<F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;         // 材质个数
 DWORD nFaceIndexes;     // 三角形面的材质索引数（实际是三角形面的个数）
 array DWORD  faceIndexes[nFaceIndexes];                               // 三角形面的材质索引值
 [Material <3D82AB4D-62DA-11cf-AB39-0020AF71E433>]    // 材质
}
template Material {                 // 材质模版
<3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;          // 三角形面的颜色
 FLOAT power;                      // 镜面光的高光强度
 ColorRGB specularColor;       // 镜面光的颜色
 ColorRGB emissiveColor;       // 自发射光的颜色
 [...]
}
template ColorRGBA {
<35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}
template ColorRGB {
<D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}
模版中通过"[模版]"的形式引入其他模版的对象，这种模版称为严格模版(Restricted Template)。MeshMaterialList模版就是一个严格模版。
通常在Material模版对象中加入纹理贴图文件的说明，此时需要使用TextureFilename模版对象来指定文件名。
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template TextureFilename {
<A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;      // 纹理贴图的文件名
}
最后，对模版的定义和使用进行简单的归纳。如下所示是模版的一般定义格式：
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template <template-name> {   // 模版定义
    <GUID>                   // 全局标志符
    <mmeber 1>           // 成员1
    ....
    <mmeber 2>           // 成员2
    [restrictions]             // 其他模版对象
}
其中，各个成员对象的类型可取为WORD, DWORD, FLOAT, DOUBLE, CHAR, UCHAR, BYTE, STRING等。此外，还可以使用如下的数组形式来说明模版中的数据对象。
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->array <data-type> <name>[<dimension-size>];
array为数组的关键字，data-type为数组元素的类型，name为数组的名字，dimension-size为数组的维数。
模版的全局标志符可利用VS提供的guidgen.exe程序自动生成，该工具位于“$ vs_setup_dir/Common7/Tools”下，如下所示：
![](http://www.cppblog.com/images/cppblog_com/lovedday/4157/r_guid.jpg)
如果一个模版，如ColorRGB模版，不包含其他模版的数据，那么该模版称为封闭模版(Closed Template)，是最简单的一种说明数据格式的模版。
在模版对象中，用来分隔数据的逗号和分号的用法需要补充说明一下。例如下面的变换矩阵的定义，中间全部用逗号分隔，最后用两个分号结束。
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->FrameTransformMatrix {    // 主框架的变换矩阵
    1.000000, 0.000000, 0.000000, 0.000000, 
    0.000000, 1.000000, 0.000000, 0.000000, 
    0.000000, 0.000000, 1.000000, 0.000000, 
    0.000000, 0.000000, 0.000000, 1.000000;;
 }
在上面的矩阵数据中，中间的逗号是数组元素的分隔，最后的第一个分号来自于数组定义的结束，第二个分号来自于矩阵数据定义的结束。观察如下的模版定义，可以清楚的看出。
<!--
Code highlighting produced by Actipro CodeHighlighter (freeware)
http://www.CodeHighlighter.com/
-->template FrameTransformMatrix {
<F6F23F41-7686-11cf-8F52-0040333594A3>
 Matrix4x4 frameMatrix;       // 产生第2个分号
}
template Matrix4x4 {
<F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];       // 产生第1个分号，数组元素之间规定用逗号分隔
}
由此可见，定义模版对象数据时，应采用代入的观点进行数据的构造，即保持模版定义中的逗号和分号不变，只是将具体的数值代入相应的变量处，就可简单地构造出合法的模版对象数据。
阅读下篇：[XFile网格的应用（2）](http://www.cppblog.com/lovedday/archive/2007/05/18/24345.html)
