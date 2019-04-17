# Shader山下（十六）坐标空间与转换矩阵 - 凯奥斯的注释 - CSDN博客





2016年10月17日 13:47:48[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：5178
所属专栏：[Shader山下](https://blog.csdn.net/column/details/12981.html)









模型空间（model space）也称为对象空间（object space）或局部空间（local space）是指以模型原点为原点的坐标系。

世界空间（world space）也称全局空间（global space）是以世界原点为原点的坐标系，世界坐标也就是Unity3D里的绝对坐标。

观察空间（view space）也称摄像机空间（camera space）是以摄像机为原点的坐标系。（这是各坐标空间中唯一一个右手坐标系）

裁剪空间（clip space）也称齐次裁剪空间（homogeneous clip space），这个空间对观察空间里的渲染图元进行裁剪，会保留完全在空间内部的图元，剔除完全在空间外部的图元，以及裁剪部分在空间内部的图元，而这块空间的范围是由摄像机的视锥体决定的。而使用投影矩阵将顶点转换到裁剪空间中去，来判断图元是否在裁剪空间内。

屏幕空间（screen space）表示的是一个二维空间，是从裁剪空间投影过来的。

从模型空间到世界空间称为模型变换，从世界空间到观察空间称为观察变换，从观察空间到裁剪空间称为投影变换，从裁剪空间到屏幕空间称为投影映射。




相关数学知识[土圭垚㙓数学课（四）空间变换](http://blog.csdn.net/ecidevilin/article/details/78182421)。




Unity内置变换矩阵：


|UNITY_MATRIX_MVP|模型（M）观察（V）投影（P）矩阵，从模型空间到裁剪空间|
|----|----|
|UNITY_MATRIX_MV|模型（M）观察（V）矩阵，从模型空间到观察空间|
|UNITY_MATRIX_V|观察（V）矩阵，从世界空间到观察空间|
|UNITY_MATRIX_P|投影（P）矩阵，从观察空间到裁剪空间|
|UNITY_MATRIX_VP|观察（V）投影（P）矩阵，从世界空间到裁剪空间|
|UNITY_MATRIX_T_MV|UNITY_MATRIX_MV的转置矩阵|
|UNITY_MATRIX_IT_MV|UNITY_MATRIX_MV的逆转置矩阵，用于将法线从模型空间转换到观察空间|
|_Object2World|模型矩阵，从模型空间到世界空间|
|_World2Object|_Object2World的逆矩阵，从世界空间到模型空间|

Untiy摄像机和屏幕参数：

|_WorldSpaceCameraPos(float3)|摄像机的世界坐标|
|----|----|
|_ProjectionParams(float4)|投影参数x=±1.0（-1.0表示使用翻转投影矩阵进行渲染）y=Near近裁剪平面与摄像机的距离z=Far远裁剪平面与摄像机的距离w=1.0+1.0/Far|
|_ScreenParams(float4)|屏幕参数x=width渲染目标的像素宽度y=height渲染目标的像素高度z=1.0+1.0/widthw=1.0+1.0/height|
|_ZBufferParams(float4)|线性化Z（深度）缓冲参数x=1-Far/Neary=Far/Nearz=x/Farw=y/Far|
|unity_OrthoParams(float4)|正交投影摄像机参数x=widthy=heightz未使用w=1.0（正交投影摄像机）或0.0（透视摄像机）|
|unity_CameraProjection(float4x4)|摄像机投影矩阵|
|unity_CameraInvProjection(float4x4)|unity_CameraProjection的逆矩阵|
|unity_CameraWorldClipPlanes(float4[6])|摄像机6个裁剪平面在世界空间下的方程式。顺序：左、右、下、上、近、远|



