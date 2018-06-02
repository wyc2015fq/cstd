
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "macro.h"
#include "fractal.h"

typedef struct CKNOT {
  double x, y;
  double direction;
  double zlen;
}
CKNOT;

typedef struct L_system {
  char* unit[ 3 ];
  char* a[ 3 ];
  char* tree , *formtree;
  int len;
  double angle;
  double degree; //迭代次数
  int ox, oy;  //原点坐标
  int stackpushpos;
  int direction; // 初始方向
  int is_rand; // 是否是随机L系统
  CKNOT stack[ 1024 ];
}
L_system;

#define GetLength(_str)  strlen(_str)
#define IsEmpty(_str)  ((_str) ? (0==strlen(_str)) : (1))

//"L系统（分形频道：fractal.cn）2004"
void L_systemDraw(IDC* pDC, L_system* ls)
{
  if (IsEmpty(ls->tree)) {
    return ;
  }
  else {
    int i, treelength;
    CKNOT mNextKnot , mCurKnot;
    mCurKnot.x = ls->ox;
    mCurKnot.y = ls->oy;
    mCurKnot.direction = ls->direction;

    treelength = GetLength(ls->tree);
    i = 0;
    MoveTo(pDC, ls->ox, ls->oy);

    while (i < treelength) {
      switch (ls->tree[ i ]) {
      case 'F':
        mNextKnot.x = mCurKnot.x + ls->len * cos(mCurKnot.direction * PI / 180);
        mNextKnot.y = mCurKnot.y - ls->len * sin(mCurKnot.direction * PI / 180);
        mNextKnot.direction = mCurKnot.direction;
        LineTo(pDC, (int) mNextKnot.x , (int) mNextKnot.y);
        mCurKnot = mNextKnot;
        break;

      case '[':
        ls->stack[ ls->stackpushpos ] = mCurKnot;
        ls->stackpushpos ++;
        break;

      case ']':
        mCurKnot = ls->stack[ ls->stackpushpos - 1 ];
        ls->stackpushpos --;
        MoveTo(pDC, (int) mCurKnot.x, (int) mCurKnot.y);
        break;

      case '+':
        mCurKnot.direction = mCurKnot.direction + ls->angle;
        break;

      case '-':
        mCurKnot.direction = mCurKnot.direction - ls->angle;
        break;

      default:
        ;
      }

      i++;
    }
  }
}

#define Empty(_str) SAFEFREE(_str)
/////////////////////////////////////////////////////////////////////////////
// CMyTreeView message handlers
#define SAFESTRLEN(_str) ((_str) ? strlen(_str) : (0))
#define SAFESTRCPY(_str1, _str2) ((_str1 && _str2) ? strcpy(_str1, _str2) : (0))
char* my_strcpy(char** str1, char* str2)
{
  char* str = MALLOC(char, SAFESTRLEN(str2) + 1);
  *str = 0;
  SAFESTRCPY(str, str2);
  SAFEFREE(*str1);
  *str1 = str;
  return str;
}
char* my_strcat(char** str1, char* str2)
{
  char* str = MALLOC(char, SAFESTRLEN(*str1) + SAFESTRLEN(str2) + 1);
  *str = 0;
  SAFESTRCPY(str, *str1);
  strcat(str, str2);
  SAFEFREE(*str1);
  *str1 = str;
  return str;
}
char* my_strcatc(char** str1, char ch)
{
  char* str = MALLOC(char, SAFESTRLEN(*str1) + 2);
  *str = 0;
  SAFESTRCPY(str, *str1);
  str[ SAFESTRLEN(*str1) ] = ch;
  str[ SAFESTRLEN(*str1) + 1 ] = '\0';
  SAFEFREE(*str1);
  *str1 = str;
  return str;
}
#define MY_STRCPY(str1, str2) my_strcpy(&(str1), str2)
#define MY_STRCAT(str1, str2) my_strcat(&(str1), str2)
#define MY_STRCATC(str1, ch) my_strcatc(&(str1), ch)


void L_systemInit(L_system* ls, char* unit0, char* unit1, char* unit2,
    char* a0, char* a1, char* a2, int len, double angle,
    double degree, int ox, int oy, int direction, int is_rand)
{
  int i;

  if (is_rand) {
    assert(NULL != unit0);

    if (NULL == unit1) {
      unit1 = unit0;
    }

    if (NULL == unit2) {
      unit2 = unit1;
    }
  }

  ls->unit[ 0 ] = unit0;
  ls->unit[ 1 ] = unit1;
  ls->unit[ 2 ] = unit2;
  ls->a[ 0 ] = a0;
  ls->a[ 1 ] = a1;
  ls->a[ 2 ] = a2;
  ls->len = len;  //线段长度
  ls->angle = angle;  //转角
  ls->degree = degree; //迭代次数
  ls->ox = ox;
  ls->oy = oy;
  ls->direction = direction;

  // 初始化
  for (i = 0; i < 1024; i++) {
    ls->stack[ i ].x = 0;
    ls->stack[ i ].direction = 0;
  }

  ls->stackpushpos = 0;
  MY_STRCPY(ls->formtree, ls->unit[ is_rand ? (rand() % 3) : 0 ]);     //公理
  Empty(ls->tree);
  //////////////////////////////////////////////////////////////

  for (i = 1; i <= ls->degree; i++) {
    int curlen = GetLength(ls->formtree);
    int pos = 0 , j = 0;

    while (j < curlen) {
      if (ls->formtree[ j ] == ls->a[ 1 ][ 0 ]) {
        //ls->tree += ls->unit[1];
        MY_STRCAT(ls->tree, ls->unit[ is_rand ? (rand() % 3) : 1 ]);
        j++;
        pos = GetLength(ls->tree) - 1;
      }
      else {
        //ls->tree += ls->formtree[j];
        MY_STRCATC(ls->tree, ls->formtree[ j ]);
        pos ++;
        j++;
      }
    }

    SAFEFREE(ls->formtree);
    ls->formtree = ls->tree;
    ls->tree = 0;
  }

  SAFEFREE(ls->tree);
  ls->tree = ls->formtree;
  ls->formtree = 0;
}

void L_systemDraw2(IDC* pDC, int type)
{
  L_system ls = {0};

  if (0 == type) {
    L_systemInit(&ls, "F[+F]F[-F]F", "F[+F]F[-F[+F]]", "FF-[-F+F+F]+[+F-F-F]", 0, "F", 0, 4, 30, 4, 350, 550, 90, 1);
  }
  else if (1 == type) {
    L_systemInit(&ls, "F", "F-F++F-F", 0, 0, "F", 0, 4, 60, 5, 0, 300, 0, 1);
  }
  else if (2 == type) {
    L_systemInit(&ls, "FF+[+F-F-F]-[-F+F+F]", "FF+[+F-F-F]-[-F+F+F]", 0, 0, "F", 0, 4, 60, 5, 0, 300, 0, 1);
  }

  L_systemDraw(pDC, &ls);
  SAFEFREE(ls.tree);
  SAFEFREE(ls.formtree);
}


void DrawL(IDC* pDC)
{
  //画背景  山， 路
  IPOINT shan1[ 5 ];
  IPOINT shan2[ 7 ];
  CKNOT stack[ 1024 ];
  int y_juli;
  int total;
  int degree;
  int stackpushpos;
  int len;
  int oy;
  int ox;
  double tidu;
  double juli;
  double rate;        //压缩因子
  double angle;
  char* unit;
  char* tree;
  L_system ls = {0};

  shan1[ 0 ].x = 0;
  shan1[ 0 ].y = 300;
  shan1[ 1 ].x = 120;
  shan1[ 1 ].y = 180;
  shan1[ 2 ].x = 170;
  shan1[ 2 ].y = 200;
  shan1[ 3 ].x = 200;
  shan1[ 3 ].y = 190;
  shan1[ 4 ].x = 300;
  shan1[ 4 ].y = 250;
  shan2[ 0 ].x = 250;
  shan2[ 0 ].y = 280;
  shan2[ 1 ].x = 300;
  shan2[ 1 ].y = 250;
  shan2[ 2 ].x = 350;
  shan2[ 2 ].y = 200;
  shan2[ 3 ].x = 450;
  shan2[ 3 ].y = 175;
  shan2[ 4 ].x = 570;
  shan2[ 4 ].y = 215;
  shan2[ 5 ].x = 700;
  shan2[ 5 ].y = 300;
  shan2[ 6 ].x = 800;
  shan2[ 6 ].y = 350;

  {
    IPOINT* pp;
    pp = shan1;
    len = 5;
    Polyline(pDC, pp, len);
    len = 5;
    pp = shan2;
    Polyline(pDC, pp, len);
  }

  angle = -16 ;
  unit = "FF+[+F-F-F]-[-F+F+F]";  //分形元
  degree = 3;
  len = 10;
  ox = 150;
  oy = 450;
  rate = 0.7;
  tidu = 0.80;
  total = 5;
  juli = 200;
  y_juli = 30;
  stackpushpos = 0 ;    // 初始化栈


  L_systemInit(&ls, "FF+[+F-F-F]-[-F+F+F]", "FF+[+F-F-F]-[-F+F+F]", 0, 0, "F", 0, len, angle, degree, ox, oy, 0, 0);
  tree = ls.tree;
  {
    //画树
    int y_julit = y_juli;
    int oxt, oyt;
    int t, k, i, nextlent, treelength;
    double julit = juli;
    double lent = len;
    oxt = ox;
    oyt = oy;

    for (t = 0; t < total; t++) {
      for (k = 2; k < 12; k++) {
        if (IsEmpty(tree)) {
          return ;
        }
        else {
          CKNOT mNextKnot , mCurKnot;
          mCurKnot.x = oxt ;
          mCurKnot.y = oyt ;
          mCurKnot.zlen = lent;
          mCurKnot.direction = 90 ;

          treelength = GetLength(tree);
          i = 0;
          nextlent = (int) lent;
          MoveTo(pDC, oxt, oyt);

          while (i < treelength) {
            switch (tree[ i ]) {
            case 'F':
              mNextKnot.x = mCurKnot.x + (int)(mCurKnot.zlen * cos(mCurKnot.direction * PI / 180));
              mNextKnot.y = mCurKnot.y - (int)(mCurKnot.zlen * sin(mCurKnot.direction * PI / 180));
              mNextKnot.direction = mCurKnot.direction ;
              mNextKnot.zlen = mCurKnot.zlen;
              LineTo(pDC, (int) mNextKnot.x , (int) mNextKnot.y);
              mCurKnot = mNextKnot ;
              break ;

            case '[':
              // lent=lent*rate;
              stack[ stackpushpos ] = mCurKnot ;
              mCurKnot.zlen *= rate; //压缩
              stackpushpos ++;
              break;

            case ']':
              // lent=nextlent;
              mCurKnot = stack[ stackpushpos - 1 ];
              // mCurKnot.zlen /= rate; //还原
              stackpushpos -- ;
              MoveTo(pDC, (int) mCurKnot.x, (int) mCurKnot.y);
              break;

            case '+':
              mCurKnot.direction = mCurKnot.direction + angle;
              break;

            case '-':
              mCurKnot.direction = mCurKnot.direction - angle;
              break;

            default:
              ;
            }

            i++;
          }

        }
      }

      oxt = oxt + (int)(julit * tidu);
      oyt = oyt - (int)(y_julit * tidu);
      julit = julit * tidu;
      y_julit = (int)(y_julit * tidu);
      lent = lent * tidu;
    }
  }

  SAFEFREE(ls.tree);
  SAFEFREE(ls.formtree);
}

