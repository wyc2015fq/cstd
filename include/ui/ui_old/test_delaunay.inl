
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // for time
#include "cstd.h"

//#include "geometry.inl"
//#include "geo/subdiv.h"
//#include "geo/subdiv.inl"
#include "ui/window.inl"




#include "cv/geometry.inl"
#include "cv/subdivision2d.inl"


//typedef myCvSubdiv2D CvSubdiv2D;

//adapted by the example of leanring opencv by crazy_007
//adapted by the OpenCV2.0\samples\c\delaunay.c
//2010-4-22

int draw_subdiv_edge(img_t* img, CvSubdiv2DEdge edge, COLOR color);   //为了查看代码方便，不然需调整调用函数顺序

/* the script demostrates iterative construction of
   delaunay triangulation and voronoi tesselation */


//draw subdiv point
int draw_subdiv_point(img_t* img, CvPoint2D32f fp, COLOR color)
{
  DrawCircle(IMARG(img), cvRound(fp.x), cvRound(fp.y), 5, color, 0, 0);  //画fp为圆心，5为半径的实心圆表示delaunay顶点
  return 0;
}
//use an external point to locate an edge or vertex or step around the edges of a delaunay tirangle
//画出delaunay 顶点
int locate_point(CvSubdiv2D* subdiv, CvPoint2D32f fp, img_t* img,
    COLOR active_color)
{
  CvSubdiv2DEdge e;
  CvSubdiv2DEdge e0 = 0;
  CvSubdiv2DPoint* p = 0;

  cvSubdiv2DLocate(subdiv, fp, &e0, &p);  //使用一个外部的点定位边缘或顶点

  //该函数填充三角形的边缘和顶点或者填充该点所处在的Voronoi面
  if (e0) {
    e = e0;

    do {
      draw_subdiv_edge(img, e, active_color);  //调用下面函数：画出红色直线
      e = cvSubdiv2DGetEdge(e, CV_NEXT_AROUND_LEFT); //遍历Delaunay图：返回左区域的下一条的边缘
    }
    while (e != e0);
  }

  draw_subdiv_point(img, fp, active_color);  //调用上面的函数：实现在该点处画半径为5的圆
  return 0;
}




/*************                 分割线                ************************************/
//draw subdiv edge
int draw_subdiv_edge(img_t* img, CvSubdiv2DEdge edge, COLOR color)
{
  CvSubdiv2DPoint* org_pt;
  CvSubdiv2DPoint* dst_pt;
  CvPoint2D32f org;
  CvPoint2D32f dst;
  IPOINT iorg, idst;

  org_pt = cvSubdiv2DEdgeOrg(edge);//Delaunay或者Voronoi边缘的原始点
  dst_pt = cvSubdiv2DEdgeDst(edge);//Delaunay或者Voronoi边缘的终点

  if (org_pt && dst_pt) {
    org = org_pt->pt;
    dst = dst_pt->pt;

    iorg = iPOINT(cvRound(org.x), cvRound(org.y));
    idst = iPOINT(cvRound(dst.x), cvRound(dst.y));

    DrawLine(IMARG(img), iorg, idst, color, 1);  //画红色直线
  }

  return 0;
}

//draw subdiv:遍历所有的Delaunay边
int draw_subdiv(img_t* img, CvSubdiv2D* subdiv,
    COLOR delaunay_color, COLOR voronoi_color)
{
  int i, total = subdiv->edge_num;

  for (i = 0; i < total; i++) { //total是边数目
    CvQuadEdge2D* edge = subdiv->edges + i;
    //CvQuadEdge2D平面划分中的Quad-edge(四方边缘结构):四个边缘 (e, eRot(红色) 以及它们的逆（绿色）

    {
      draw_subdiv_edge(img, (CvSubdiv2DEdge)edge + 1, voronoi_color);   //不知如何理解(CvSubdiv2DEdge)edge + 1
      //书中P346：voronoi_edge=(CvSubdiv2DEdge)edge + 1
      //直接采用数组位移法进行各种边的对应的(即edge+1),
      //cvSubdiv2DRotateEdge((CvSubdiv2DEdge)edge,1)=(CvSubdiv2DEdge)edge+1
      //参考网址为：http://tech.ddvip.com/2007-12/119897724239781.html
      draw_subdiv_edge(img, (CvSubdiv2DEdge)edge, delaunay_color);   //调用上面的子函数
    }

  }

  return 0;
}


//draw the voronoi facet:遍历Voronoi面
int draw_subdiv_facet(img_t* img, CvSubdiv2DEdge edge)
{
  CvSubdiv2DEdge t = edge;
  int i, count = 0;
  IPOINT* buf = 0;

  // count number of edges in facet
  do {
    count++;
    t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);  //返回左区域的下一条的边缘
  }
  while (t != edge);

  buf = (IPOINT*)malloc(count * sizeof(buf[0]));

  // gather points
  t = edge;

  for (i = 0; i < count; i++) {
    CvSubdiv2DPoint* pt = cvSubdiv2DEdgeOrg(t);  //获得边缘的起点

    if (!pt) {
      break;
    }

    buf[i] = iPOINT(cvRound(pt->pt.x), cvRound(pt->pt.y)); //点记录在buf中
    t = cvSubdiv2DGetEdge(t, CV_NEXT_AROUND_LEFT);
  }

  if (i == count) {
    CvSubdiv2DPoint* pt = cvSubdiv2DEdgeDst(cvSubdiv2DRotateEdge(edge, 1));   //获得边缘的终点
    FillConvexPoly(IMARG(img), buf, count, _RGB(rand() & 255, rand() & 255, rand() & 255)); //一次只能画一个多边形，而且只能画凸多边形
    PolyLine(IMARG(img), buf, &count, 1, 1, _RGB(0, 0, 0), 1); //一次调用中绘制多个多边形
    draw_subdiv_point(img, pt->pt, CV_RGB(0, 0, 0)); //画圆
  }

  free(buf);
  return 0;
}
//draw & paint voronoi graph
int paint_voronoi(CvSubdiv2D* subdiv, img_t* img)
{
  int i, total = subdiv->edge_num;

  cvCalcSubdivVoronoi2D(subdiv);  //计算Voronoi图表的细胞结构

  for (i = 0; i < total; i++) {
    CvQuadEdge2D* edge = subdiv->edges + i;

    {
      CvSubdiv2DEdge e = (CvSubdiv2DEdge)edge;
      // left
      draw_subdiv_facet(img, cvSubdiv2DRotateEdge(e, 1));   //调用上面的子函数

      // right
      draw_subdiv_facet(img, cvSubdiv2DRotateEdge(e, 3));
    }

  }

  return 0;
}


int test_delaunay()
{
  char win[] = "source";
  int i;
  CvRect rect = { 0, 0, 600, 600 };//外界边界矩形大小
  CvSubdiv2D subdiv[1] = {0};//细分
  img_t img[1] = {0};
  COLOR active_facet_color, delaunay_color, voronoi_color, bkgnd_color;

  active_facet_color = CV_RGB(255, 0, 0);
  delaunay_color = CV_RGB(0, 0, 0);
  voronoi_color = CV_RGB(0, 180, 0);
  bkgnd_color = CV_RGB(255, 255, 255);

  imsetsize(img, rect.h, rect.w, 3, 1);  //创建白色背景的图像
  imdraw_fill(IMARG(img), bkgnd_color);

  cvNamedWindow(win, 1);  //窗口名字为"source"

  //initialization convenience function for delaunay subdivision：调用子函数确定矩形边界
  cvInitSubdivDelaunay2D(subdiv, 100, 100, rect);  //矩形确定的边界

  for (i = 0; i < 20; i++) {
    CvPoint2D32f fp = cvPoint2D32f((float)(rand() % (rect.w - 10) + 5),
        (float)(rand() % (rect.h - 10) + 5)); //This is our point holder

    locate_point(subdiv, fp, img, active_facet_color);  //调用函数
    //cvShowImage( win, (img)->h, (img)->w, (img)->tt.data, (img)->s, (img)->c );

    //等待600ms
    //if( cvWaitKey( 100 ) >= 0 )  break;

    cvSubdivDelaunay2DInsert(subdiv, fp);  //向Delaunay三角测量中插入一个点
    //cvCalcSubdivVoronoi2D( subdiv );//计算Voronoi图表的细胞结构
    //imdraw_fill(IMARG(img), bkgnd_color);
    // 给一个对象全部元素赋值:int cvSet( CvArr* arr, COLOR value, const CvArr* mask=NULL );
    //draw_subdiv( img, subdiv, delaunay_color, voronoi_color );//调用子函数：cvSeqReader逐步遍历边来获得细分结构
    //cvShowImage( win, (img)->h, (img)->w, (img)->tt.data, (img)->s, (img)->c );

    //if( cvWaitKey(100) >= 0 ) break;
  }

  cvCalcSubdivVoronoi2D(subdiv);  //计算Voronoi图表的细胞结构

  imdraw_fill(IMARG(img), bkgnd_color);
  paint_voronoi(subdiv, img);  //调用子函数
  cvShowImage(win, (img)->h, (img)->w, (img)->tt.data, (img)->s, (img)->c);

  cvWaitKey(0);

  cvDestroyWindow(win);
  return 0;
}

