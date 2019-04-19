# Direct3D中实现图元的鼠标拾取 - 逍遥剑客 - CSDN博客
2007年04月23日 23:30:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2926
**Direct3D中实现图元的鼠标拾取**
BY 重剑，2004.5.28 [重剑空间](http://www.heavysword.com/)
**索引：**
[1、什么是拾取，拾取能做什么？](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#1%E3%80%81%E4%BB%80%E4%B9%88%E6%98%AF%E6%8B%BE%E5%8F%96%EF%BC%8C%E6%8B%BE%E5%8F%96%E8%83%BD%E5%81%9A%E4%BB%80%E4%B9%88%EF%BC%9F)
[2、拾取操作的步骤和实现](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2%E3%80%81%E6%8B%BE%E5%8F%96%E6%93%8D%E4%BD%9C%E7%9A%84%E6%AD%A5%E9%AA%A4%E5%92%8C%E5%AE%9E%E7%8E%B0)
[2.1．  变换并获得通过视点和屏幕上点击点的射线矢量（Dir）](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2.1%EF%BC%8E%C2%A0%20%E5%8F%98%E6%8D%A2%E5%B9%B6%E8%8E%B7%E5%BE%97%E9%80%9A%E8%BF%87%E8%A7%86%E7%82%B9%E5%92%8C%E5%B1%8F%E5%B9%95%E4%B8%8A%E7%82%B9%E5%87%BB%E7%82%B9%E7%9A%84%E5%B0%84%E7%BA%BF%E7%9F%A2%E9%87%8F%EF%BC%88Dir%EF%BC%89)
> 
> 
[2.1.1 确定鼠标选取点的屏幕坐标](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2.1.1%20%E7%A1%AE%E5%AE%9A%E9%BC%A0%E6%A0%87%E9%80%89%E5%8F%96%E7%82%B9%E7%9A%84%E5%B1%8F%E5%B9%95%E5%9D%90%E6%A0%87)
[2.1.2 得到Dir在观察坐标空间内的表示](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2.1.2%20%E5%BE%97%E5%88%B0Dir%E5%9C%A8%E8%A7%82%E5%AF%9F%E5%9D%90%E6%A0%87%E7%A9%BA%E9%97%B4%E5%86%85%E7%9A%84%E8%A1%A8%E7%A4%BA)
[2.1.3 转换Dir到世界坐标空间，并得到观察点在世界坐标系中的坐标](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2.1.3%20%E8%BD%AC%E6%8D%A2Dir%E5%88%B0%E4%B8%96%E7%95%8C%E5%9D%90%E6%A0%87%E7%A9%BA%E9%97%B4%EF%BC%8C%E5%B9%B6%E5%BE%97%E5%88%B0%E8%A7%82%E5%AF%9F%E7%82%B9%E5%9C%A8%E4%B8%96%E7%95%8C%E5%9D%90%E6%A0%87%E7%B3%BB%E4%B8%AD%E7%9A%84%E5%9D%90%E6%A0%87)
[2.2   使用射线矢量对场景中的所有三角形图元求交，获得三角形索引值和重心坐标。](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2.2%20%C2%A0%20%E4%BD%BF%E7%94%A8%E5%B0%84%E7%BA%BF%E7%9F%A2%E9%87%8F%E5%AF%B9%E5%9C%BA%E6%99%AF%E4%B8%AD%E7%9A%84%E6%89%80%E6%9C%89%E4%B8%89%E8%A7%92%E5%BD%A2%E5%9B%BE%E5%85%83%E6%B1%82%E4%BA%A4%EF%BC%8C%E8%8E%B7%E5%BE%97%E4%B8%89%E8%A7%92%E5%BD%A2%E7%B4%A2%E5%BC%95%E5%80%BC%E5%92%8C%E9%87%8D%E5%BF%83%E5%9D%90%E6%A0%87%E3%80%82)
> 
> 
[2.2.1 D3D扩展函数实现求交](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2.2.1%20D3D%E6%89%A9%E5%B1%95%E5%87%BD%E6%95%B0%E5%AE%9E%E7%8E%B0%E6%B1%82%E4%BA%A4)
[2.2.2射线三角面相交的数学算法](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2..2.2%E5%B0%84%E7%BA%BF%E4%B8%89%E8%A7%92%E9%9D%A2%E7%9B%B8%E4%BA%A4%E7%9A%84%E6%95%B0%E5%AD%A6%E7%AE%97%E6%B3%95)
[2.2.3  拾取完成根据获得的中心坐标计算我们关心的常见量](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#2.2.3%C2%A0%20%E6%8B%BE%E5%8F%96%E5%AE%8C%E6%88%90%E6%A0%B9%E6%8D%AE%E8%8E%B7%E5%BE%97%E7%9A%84%E4%B8%AD%E5%BF%83%E5%9D%90%E6%A0%87%E8%AE%A1%E7%AE%97%E6%88%91%E4%BB%AC%E5%85%B3%E5%BF%83%E7%9A%84%E5%B8%B8%E8%A7%81%E9%87%8F%EF%BC%8C%E3%80%82)
[3、结束及声明](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#3%E3%80%81%E7%BB%93%E6%9D%9F%E5%8F%8A%E5%A3%B0%E6%98%8E)
[4、参考文献](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#4%E3%80%81%E5%8F%82%E8%80%83%E6%96%87%E7%8C%AE)
[补充：重心坐标的概念](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.htm#%E8%A1%A5%E5%85%85%EF%BC%9A%E9%87%8D%E5%BF%83%E5%9D%90%E6%A0%87%E7%9A%84%E6%A6%82%E5%BF%B5)
3D交互图形应用程序中，常常要用鼠标去选择图形，其实现的机制基于鼠标拾取算法。本文主要讲述如何在D3D中实现图元的鼠标拾取。为了讨论简单，本文假定读者理解D3D 坐标变换流程和基本的图形学知识，如果阅读有困难请参考相关资料。
**1、什么是拾取，拾取能做什么？**
首先，拾取操作指当我们在屏幕上用鼠标点击某个图元应用程序能返回该图元的一个标志和某些相关信息。有图形程序设计经验的人都知道，有这些信息就表示我们有了对该图元的控制权，我们可以删除，可以编辑，可以任意对待该图元，至于你到底想干什么，就是阁下自己的事了^_^。
**2、拾取操作的步骤和实现**
拾取算法的思想很简单：得到鼠标点击处的屏幕坐标，通过投影矩阵和观察矩阵把该坐标转换为通过视点和鼠标点击点的一条射入场景的光线，该光线如果与场景模型的三角形相交（本文只处理三角形图元），则获取该相交三角形的信息。本文讲述的方法除可以得到三角形的一个索引号以外还可以得到相交点的重心坐标。
从数学角度来看，我们只要得到射线的方向矢量和射线的出射点，我们就具备了判断射线与空间一个三角面是否相交的条件，本文主要讨论如何获得这些条件，并描述了射线三角面相交判断算法和D3D的通常实现方法。
根据拾取操作的处理顺序，大概可以依次分为以下几个步骤
**2.1****．****变换并获得通过视点和屏幕上点击点的射线矢量（Dir）**
详细介绍之前，为了大家方便理解，我们要先简单说一下d3d坐标转换的大概流程，如下图:
所以我们要通过一系列的反变换，得到我们关心的值在世界坐标中的表示。
**2.1.1 确定鼠标选取点的屏幕坐标**
这一步是非常简单的Windows给我们提供了API来完成屏幕坐标的获取，使用GetCursorPos获得鼠标指针位置，然后再利用ScreenToClient转换坐标到客户区坐标系(以窗口视区左上角为坐标原点，单位为像素)，设该坐标为（POINT screenPt）。
**2.1.2 得到Dir在观察坐标空间内的表示**
在观察坐标系中，Dir是一条从观察坐标原点出发的射线，所以我们只需要再确定一个该射线经过的点，就可以得到它在观察坐标系中的表示。假设我们要求的射线上的另外一点为该射线与透视投影平截头体近剪切面的交点，针对最普遍的透视投影而言，透视投影平截头体经投影变换后，变成一个1/2立方体（请允许我这么叫^_^，因为它的大小为一个正方体的一半，x,y方向边长为2，z方向为1）如图：
投影坐标系以近剪切面中心为坐标原点，该立方体从z轴负向看过去与图形程序视区相对应，最终近剪切面（前剪切面）上一点与屏幕坐标之间的对应关系如下图所示：
**根据比例关系，screenPt与投影空间上的点projPt之间的关系为**
假设图形程序窗口的宽为screenWidth,高为screenHeight,
projPt.x = (screenPt.x-screenWidth/2)/screenWidth*2; （公式1）
projPt.y = (screenPt.y-screenHeight/2)/screenHeight*2; （公式2）
projPt.z =0;（实际该值可任意取，不影响最终结果。为了处理简单，我们取改值为0，表示该点取在近剪切面上）
**得到projPt后，我们需要做的是把该点坐标从投影空间转换到观察空间(view space),**
根据透视投影的定义，可假设点(projPt.x，projPt.y，projPt.z)
对应的其次坐标为
(projPt.x*projPt.w，projPt.y*projPt.w，projPt.z*projPt.w，projPt.w)
我们可以通过 GetTransform(      D3DTS_PROJECTION,    &ProjMatrix)函数获得投影矩阵ProjMatrix,则根据观察空间到投影空间的变换关系则
(projPt.x*projPt.w，projPt.y*projPt.w，projPt.z*projPt.w，projPt.w)
 = (viewPt.x，viewPt.y，viewPt.z, 1)*pProjMatrx;
根据定义和图形学原理
ProjMatrix = =
所以,
(projPt.x*projPt.w，projPt.y*projPt.w，projPt.z*projPt.w，projPt.w)
= ( viewPt.x*ProjMatrix._m11,
viewPt.y*ProjMatrix._m22,
viewPt.z*Q-QZn,
viewPt.z)
所以
projPt.x*projPt.w = viewPt.x*ProjMatrix._m11
projPt.y*projPt.w = viewPt.y*ProjMatrix._m22
projPt.z*projPt.w = viewPt.z*Q-QZn （注意projPt.z = 0）
projPt.w = viewPt.z;
解得
viewPt.x = projPt.x*Zn/ ProjMatrix._m11;
viewPt.y = projPt.y*Zn/ ProjMatrix._m22;
viewPt.z = Zn;
好了，到这里为止我们终于求出了射线与近剪切面交点在观察坐标系中的坐标，现在我们拥有了射线的出发点(0,0,0)和射线方向上另外一点(viewPt.x,viewPt.y,viewPt.z),则该射线的方向矢量在观察空间中的表示可确定为（viewPt.x-0,viewPt.y-0,viewPt.z-0）,化简一下三个分量同除近剪切面z坐标Zn，该方向矢量可写作
DIRview = (projPt.x/projMatrix._m11,projPt.y/projMatrix._m22,1)
代入公式1，公式2
DIRview.x = (2*screenPt.x/screenWidth-1)/projMatrix._m11;
DIRview.y = (2*screenPt.y/screenHeight-1)/projMatrix._m22;
DIRview.z = 1;
其中screenWidth和screenHeight可以通过图像显示的backBuffer的目标表面（D3DSURFACE_DESC）来获得，该表面在程序初始化时由用户创建。
**2.1.3 转换Dir到世界坐标空间，并得到观察点在世界坐标系中的坐标**
**由于最终的运算要在世界坐标空间中进行，所以我们还需要把矢量DIRview从观察空间转换为世界坐标空间中的矢量DIRworld。**
因为
DIRview = DIRworld*ViewMatrix;
其中ViewMatrix为观察矩阵，在D3D中可以用函数GetTransform( D3DTS_VIEW, &ViewMatrix )得到。
所以DIRworld = DIRview * inverse_ViewMatrix,其中inverse_ViewMatrix为
ViewMatrix的逆矩阵。
     观察点在观察坐标系中坐标为OriginView（0，0，0，1），所以其在世界坐标系中的坐标同样可以利用ViewMatrix矩阵，反变换至世界坐标系中，事实上我们可以很简单的判断出,其在世界坐标系中的表示为:
OriginWorld = (inverse_ViewMatrix._41,
inverse_ViewMatrix._42,
inverse_ViewMatrix._43,
1);
到这里为止，判断射线与三角面是否相交的条件就完全具备了。
**2.2   使用射线矢量对场景中的所有三角形图元求交，获得三角形索引值和重心坐标。**
这一步骤地实现由两种途径:
第一种方法非常简单，利用D3D提供的扩展函数D3DXIntersect可以轻松搞定一切。见2.1
第二种方法就是我们根据空间解析几何的知识，自己来完成射线三角形的求交算法。一般来讲，应用上用第一种方法就足够了，但是我们如果要深入的话，必须理解相交检测的数学算法，这样才能自由的扩展，面对不同的需求，内容见2.2
下面分别讲解两种实现途径：
**2.2.1 D3D扩展函数实现求交**
这种方法很简单也很好用，对于应用来说应尽力是用这种方式来实现，毕竟效率比自己写得要高得多。
实际上其实没什么好讲的，大概讲一下函数D3DXIntersect吧
D3D SDK该函数声明如下
HRESULT D3DXIntersect(      
    LPD3DXBASEMESH *pMesh*,    CONST D3DXVECTOR3 **pRayPos*,    CONST D3DXVECTOR3 **pRayDir*,    BOOL **pHit*,    DWORD **pFaceIndex*,    FLOAT **pU*,    FLOAT **pV*,    FLOAT **pDist*,    LPD3DXBUFFER **ppAllHits*,    DWORD **pCountOfHits*);lpMesh指向一个ID3DXBaseMesh的对象，最简单的方式是从.x文件获得，描述了要进行相交检测的三角面元集合的信息，具体规范参阅direct9 SDKlpRayPos 指向射线发出点lpRayDir 指向前面我们辛辛苦苦求出的射线方向的向量lpHit 当检测到相交图元时，指向一个true,不与任何图元相交则为假lpU 用于返回重心坐标U分量lpV返回重心坐标V分量lpDist 返回射线发出点到相交点的长度注意：以上红色字体部分均指最近的一个返回结果（即*pDist最小）lppAllHits用于如果存在多个相交三角面返回相交的所有结果lpCountOfHits 返回共有多少个三角形与该射线相交
补充：重心坐标的概念
其中pU和pV用到了重心坐标的概念，下面稍作描述
一个三角形有三个顶点，在迪卡尔坐标系中假设表示为V1(x1,y1,z1),V2(x2,y2,z2),V3(x3,y3,z3),则三角形内任意一点的坐标可以表示为 pV = V1 + U(V2-V1) + V(V3-V1),所以已知三个顶点坐标的情况下，任意一点可用坐标(U,V)来表示，其中 参数U控制V2在结果中占多大的权值，参数V控制V3占多大权值，最终1－U－V控制V1占多大权值，这种坐标定义方式就叫重心坐标。
**2..2.2射线三角面相交的数学算法**
使用d3d扩展函数，毕竟有时不能满足具体需求，掌握了该方法，我们才能够获得最大的控制自由度，任意修改算法。
**已知条件**: 射线源点orginPoint,三角形三个顶点 v1,v2,v3,射线方向 Dir
（均以三维坐标向量形式表示）
**算法目的**: 判断射线与三角形是否相交，如果相交求出交点的重心坐标(U,V)和射线原点到交点的距离T。
我们可先假设射线与三角形相交则交点(注以下均为向量运算，*数乘，dot(X,Y) X，Y 点乘，cross（X，Y）X，Y叉乘；U，V，T为标量) 
则：
IntersectPoint = V1 + U*(V2-V1) + V*(V3-V1) ;
IntersectPoint = originPoint + T*Dir；
所以
orginPoint + T*Dir = V1 + U*(V2-V1) + V*(V3-V1);
整理得：
这是一个简单的线性方程组，若有解则行列式不为0。
根据T,U,V的含义当T>0, 0<U<1,0<V<1,0<U+V<1时该交点在三角形内部，
解此方程组即可获得我们关心的值,具体解法不再赘述，克莱姆法则就够了（详细见线性代数）:射线原点到相交点的距离T,和交点的中心坐标(U,V)。
下面给出Direct 9 SDK示例程序中的实现代码
IntersectTriangle( const D3DXVECTOR3& orig,
const D3DXVECTOR3& dir, D3DXVECTOR3& v0,
                   D3DXVECTOR3& v1, D3DXVECTOR3& v2,
                   FLOAT* t, FLOAT* u, FLOAT* v )
{
// 算出两个边的向量
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );
// 如果det为0，或接近于零则射线与三角面共面或平行，不相交
//此处det就相当于上面的，
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );
    D3DXVECTOR3 tvec;
if( det > 0 )
    {
        tvec = orig - v0;
    }
else
    {
        tvec = v0 - orig;
        det = -det;
    }
if( det < 0.0001f )
return FALSE;
// 计算u并测试是否合法（在三角形内）
    *u = D3DXVec3Dot( &tvec, &pvec );
if( *u < 0.0f || *u > det )
return FALSE;
// Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );
//计算u并测试是否合法（在三角形内）
    *v = D3DXVec3Dot( &dir, &qvec );
if( *v < 0.0f || *u + *v > det )
return FALSE;
/*计算t,并把t,u,v放缩为合法值（注意前面的t,v,u不同于算法描述中的相应量，乘了一个系数det）,注意：由于该步运算需要使用除法，所以放到最后来进行，避免不必要的运算，提高算法效率*/
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;
return TRUE;
}
**2.2.3  拾取完成根据获得的中心坐标计算我们关心的常见量，。**
根据重心坐标（U,V）,我们可以很容易的算出各种相关量比如纹理坐标和交点的差值颜色，假设以纹理坐标为例设V1,V2,V3的纹理坐标分别为T1(tu1,tv1),T2(tu2,tv2),T3(tu3,tv3)则交点的坐标为
IntersectPointTexture = T1 + U(T2-T1) + V(T3-T1)
**3、结束及声明**
Ok, 到这里为止关于拾取的相关知识就介绍完了，小弟第一次写这种文章，不知道有没有把问题说清楚，希望对大家有所帮助，有任何问题可以给我发email: [jzhang1@mail.xidian.edu.cn](mailto:jzhang1@mail.xidian.edu.cn)
或者到我的网站留言：[www.heavysword.com](http://www.heavysword.com/)
声明：
本文写作的目的是为了广大D3D学习者方便学习服务，文中算法为作者参考相关文献总结，作者无意把这些据为自己的成果，所有权原算法提出者所有（参阅参考文献），文中代码为D3d SDK的示例内容，由笔者进行了必要的解释，代码版权归microsoft所有。
**4、参考文献**
【1】Microsoft DirectX 9.0 SDK,microsoft
【2】fast,Minimun Storage Ray/Triangle Intersection,Tomas Moler,Ben Trumbore
 BY![](http://dev.gameres.com/Program/Visual/3D/pick_2004_529.files/linklogo.gif)
(转载请注明出处）
![51.la 专业、免费、强健的访问统计](http://icon.ajiang.net/icon_0.gif)![](http://31.db.51.la/s.asp?id=486352&tpages=5&ttimes=1&tzone=8&tcolor=32&sSize=1280,800&referrer=&vpage=http%3A//dev.gameres.com/Program/Visual/3D/pick_2004_529.htm)
