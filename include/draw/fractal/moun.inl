
// draw_moun.cpp : Defines the entry point for the application.
//

#include <time.h>
#include <stdlib.h>
#include "macro.h"
#include "fractal.h"

#define MAX_LOADSTRING 100

typedef struct vertex {
  IPOINT point;
  float z; //对于每个点有三个坐标，z表示在y方向上的上升坐标
}
vertex;

typedef struct LISTVERTEX {
  union {
    vertex data;
    struct {
      IPOINT point;
      float z; //对于每个点有三个坐标，z表示在y方向上的上升坐标
    };
  };
  struct LISTVERTEX* next;
}
LISTVERTEX;

LISTVERTEX* lst_vertex;

//the count of iterations.
#define PLYS 7
static int g_plys = PLYS;

//funcions for drawing the triangles.
//make the middle point of point1 and point2.
#define midpoint( x, y)  ((x)+(y))/2

#define Rnd(num) ((float)rand())/RAND_MAX*num
#define Sig (sig())//return 1 or -1

LISTVERTEX* push_back_(LISTVERTEX** l, vertex data)
{
  LISTVERTEX* x = *l;

  if (*l == NULL) {
    *l = MALLOC(LISTVERTEX, 1);
    (*l) ->data = data;
    (*l) ->next = NULL;
    x = *l;
  }
  else {
    for (; x->next; x = x->next)
      ;

    x->next = MALLOC(LISTVERTEX, 1);
    x = x->next;
    x->data = data;
    x->next = NULL;
  }

  return x;
}
#define push_back(_l, data) push_back_(&_l, data)

vertex fraline(vertex vertex1, vertex vertex2)
{
  LISTVERTEX* ite;
  int mid_x = midpoint(vertex1.point.x , vertex2.point.x);
  vertex node;

  //iterate the list for vetex.and find the point if exists.
  for (ite = lst_vertex; ite != NULL; ite = ite->next) {
    if (ite->point.x == mid_x) {
      return ite->data;
    }
  }

  node.point.x = mid_x;
  node.point.y = midpoint(vertex1.point.y, vertex2.point.y);
  //need the z posision.so the parameters must be vertex type.
  node.z = midpoint(vertex1.z, vertex2.z) + Rnd(4);
  //lst_vertex, push_front(node);
  push_back(lst_vertex, node);
  return node;
}

int sig()
{
  if (Rnd(1) > 0.5) {
    return 1;
  }
  else {
    return -1;
  }
}


//vertexs are the three vertexs of the triangle.
void fratriangle(vertex A, vertex B, vertex C, int plys)
{
  vertex AB, BC, CA;

  if (plys <= 1) {
    return ;
  }

  AB = fraline(A, B);
  BC = fraline(B, C);
  CA = fraline(C, A);

  --plys;
  fratriangle(CA, BC, C, plys);
  fratriangle(AB, B, BC, plys);
  fratriangle(BC, CA, AB, plys);
  fratriangle(A, AB, CA, plys);
}


//draw the triangles.
void drawtriangle(IDC* pDC, IPOINT A, IPOINT B, IPOINT C)
{
  LISTVERTEX* ite;
  static int loop = 100;
  int a_x = A.x;
  int b_x = B.x;
  int c_x = C.x;
  int a_z;
  int b_z;
  int c_z;
  float rnd;
  IPOINT point[ 3 ];

  //i must come back to improve this algorithm.
  for (ite = lst_vertex; ite != NULL; ite = ite->next) {
    if (ite->point.x == a_x) {
      a_z = (int) ite->z;
    }
    else if (ite->point.x == b_x) {
      b_z = (int) ite->z;
    }
    else if (ite->point.x == c_x) {
      c_z = (int) ite->z;
    }
  }

  point[ 0 ].x = a_x;
  point[ 0 ].y = A.y + a_z;
  point[ 1 ].x = b_x;
  point[ 1 ].y = B.y + b_z;
  point[ 2 ].x = c_x;
  point[ 2 ].y = C.y + c_z;
  //NOTE:to improve the efficiency,we should create a brush array that is loaded on
  //the intialization of the program.

  rnd = Rnd(loop);

  //注意：在下面修改山的颜色
  pDC->pen_color = _RGB(90 + rnd, 150 + rnd, 120 + rnd);
  loop = loop - 1;

  if (loop < 1) {
    loop = 100;
  }

  // int r=80+(int)(l*(rand-0.5)*60);
  {
    int len = 3;
    IPOINT* pp = point;
    Polygon(pDC, pp, len);
  }

  return ;
}

void startdraw(IDC* pDC, IPOINT A, IPOINT B, IPOINT C, int plys)
{
  IPOINT AB, BC, CA;

  if (plys == 1) {
    drawtriangle(pDC, A, B, C);
  }
  else {
    AB.x = midpoint(A.x, B.x);
    AB.y = midpoint(A.y, B.y);
    BC.x = midpoint(B.x, C.x);
    BC.y = midpoint(B.y, C.y);
    CA.x = midpoint(C.x, A.x);
    CA.y = midpoint(C.y, A.y);
    --plys;
    startdraw(pDC, CA, BC, C, plys);
    startdraw(pDC, AB, B, BC, plys);
    startdraw(pDC, BC, CA, AB, plys);
    startdraw(pDC, A, AB, CA, plys);

  }

}

int DrawMoun(IDC* pDC)
{
  vertex v1, v2, v3;
  //x,y,z
  //注意：在下面设置山的位置
  v1.point.x = 100;
  v1.point.y = 300;
  v2.point.x = 500;
  v2.point.y = 200;
  v3.point.x = 200;
  v3.point.y = 500;
  //注意：在下面设置山的参数
  v1.z = 6;
  v2.z = 2;
  v3.z = 60;
  push_back(lst_vertex, v1);
  push_back(lst_vertex, v2);
  push_back(lst_vertex, v3);

  fratriangle(v1, v2, v3, PLYS);
  g_plys = PLYS;
  //for debugs.

  startdraw(pDC, v1.point, v2.point, v3.point, PLYS);
  return 0;
}
