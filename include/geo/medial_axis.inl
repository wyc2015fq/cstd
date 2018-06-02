//以下为我的添加
#include<math.h>
struct Point_Link {
  unsigned char num; //序号
  double k; //斜率
  unsigned char flag; //当该点的斜率为无穷大的时候，flag取1否则为0
  double x; //点的X坐标
  double y; //点的Y 坐标
  struct Point_Link* p_pre; //前导指针
  struct Point_Link* p_succ; //后导指针
};
struct Point_Link* p_head, *p_temp;
struct Edge_Link {
  unsigned char n0; //边的前一个顶点
  unsigned char n1; //边的后一个顶点
  double d; //距离
  struct Edge_Link* e_pre; //前导指针
  struct Edge_Link* e_succ; //后导指针
};
struct Draw_Point {
  double x;
  double y;
  int num;
  unsigned char d_flag; //表示通过该点的斜率是否为无穷大
  double d_k; //表示通过该点的斜率;
  struct Draw_Point* next;
};
struct Draw_Point* d_Point_one;
bool m_pLMouseDown;
int vertex = 0;
//计算斜率（参数三个顶点）
double Caculate_X_L(struct Point_Link* point_A, struct Point_Link* point_B,
    struct Point_Link* point_C)
{
  double k_BC, k_BA;
  double k = 0, k1, k2;
  if (point_B->x == point_C->x) {
    if (point_B->y == point_A->y) {
      return (k = 1.0);
    }
    else {
      k_BC = 0.0;
      k_BA = (point_A->x - point_B->x) / (point_A->y - point_B->y);
      if (k_BA > 0.0) {
        k = (double)((sqrt(1 + k_BA * k_BA) - 1) / k_BA);
      }
      else {
        k = (double)((- sqrt(1 + k_BA * k_BA) - 1) / k_BA);
      }
      return (1 / k);
    }
  }
  if (point_A->x == point_B->x) {
    if (point_C->y == point_B->y) {
      return (k = -1.0);
    }
    else {
      k_BA = 0.0; //此时k_BA，k_BC的斜率为对y轴的斜率
      k_BC = (point_C->x - point_B->x) / (point_C->y - point_B->y);
      if (k_BC > 0) {
        k = (double)((-1 - sqrt(1 + k_BC * k_BC)) / k_BC);
      }
      else {
        k = (double)((sqrt(1 + k_BC * k_BC) - 1) / k_BC);
      }
      return (1 / k); //最后要取倒数！
    }
  }
  if ((point_A->x != point_B->x) && (point_B->x != point_C->x)) {
    k_BA = (point_A->y - point_B->y) / (point_A->x - point_B->x);
    k_BC = (point_C->y - point_B->y) / (point_C->x - point_B->x);
    if (k_BA + k_BC == 0) { //此时所求斜率可能为0，也可能为无穷大
      if (((point_B->x > point_A->x) && (point_B->x > point_C->x))
          || ((point_B->x < point_A->x) && (point_B->x < point_C->x))) {
        k = 0.0;
        return k;
      }
      else {
        point_B->flag = 1; //无穷大的时候能返回什么呢？
        //return ('s');
      }
    }
    else {
      k1 = (double)((k_BA * k_BC - 1 + sqrt(1 + k_BA * k_BA + k_BC * k_BC + k_BA * k_BA * k_BC * k_BC))
          / (k_BA + k_BC));
      k2 = (double)((k_BA * k_BC - 1 - sqrt(1 + k_BA * k_BA + k_BC * k_BC + k_BA * k_BA * k_BC * k_BC))
          / (k_BA + k_BC));
      if (k_BA >= 0 && k_BC >= 0) {
        if ((point_B->x < point_C->x && point_B->x < point_A->x)
            || (point_B->x > point_C->x && point_B->x > point_A->x)) {
          k = (k1 > k2) ? k1 : k2;
          return k;
        }
        else {
          k = (k1 > k2) ? k2 : k1;
          return k;
        }
      }
      if (k_BA <= 0 && k_BC <= 0) {
        if ((point_B->x > point_C->x && point_B->x > point_A->x)
            || (point_B->x < point_C->x && point_B->x < point_A->x)) {
          k = (k1 > k2) ? k2 : k1;
          return k;
        }
        else {
          k = (k1 > k2) ? k1 : k2;
          return k;
        }
      }
      if (k_BA >= 0 && k_BC <= 0) {
        if ((k1 > k_BA && k1 < k_BC) || (k1 < k_BA && k1 > k_BC)) {
          k = k2;
          return k;
        }
        else {
          k = k1;
          return k;
        }
      }
      if (k_BA <= 0 && k_BC >= 0) {
        if ((k1 > k_BA && k1 < k_BC) || (k1 < k_BA && k1 > k_BC)) {
          k = k1;
          return k;
        }
        else {
          k = k2;
          return k;
        }
      }
    }
  }
  return k;
}
//计算距离
double Caculate_D(struct Point_Link* point_D, struct Point_Link* point_E)
{
  double d = 0;
  double x, y, x1, y1;
  double k1, k2;
  if (point_D->flag == 1) { //斜率为无穷大
    x = point_D->x;
    y = point_E->y + point_E->k * (x - point_E->x);
    k1 = (point_E->y - point_D->y) / (point_E->x - point_D->x);
    k2 = -1 / k1;
    x1 = (point_D->y - y + k2 * x - k1 * point_D->x) / (k2 - k1);
    y1 = (x - point_D->x + k1 * y - k2 * point_D->y) / (k1 - k2);
    d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
    return d;
  }
  if (point_E->flag == 1) {
    x = point_E->x;
    y = point_D->y + point_D->k * (x - point_D->x);
    k1 = (point_E->y - point_D->y) / (point_E->x - point_D->x);
    k2 = -1 / k1;
    x1 = (point_D->y - y + k2 * x - k1 * point_D->x) / (k2 - k1);
    y1 = (x - point_D->x + k1 * y - k2 * point_D->y) / (k1 - k2);
    d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
    return d;
  }
  if (point_D->flag != 1 && point_E->flag != 1) {
    x = (point_E->y - point_D->y + point_D->k * point_D->x
        - point_E->k * point_E->x) / (point_D->k - point_E->k);
    y = (point_E->k * point_D->k * (point_E->x - point_D->x)
        + point_D->y * point_E->k - point_E->y * point_D->k)
        / (point_E->k - point_D->k);
    if (point_D->x == point_E->x) {
      y1 = y;
      x1 = point_D->x;
      d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
      return d;
    }
    if (point_D->y == point_E->y) {
      x1 = x;
      y1 = point_D->y;
      d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
      return d;
    }
    if ((point_D->x != point_E->x) && (point_D->y != point_E->y)) {
      k1 = (point_E->y - point_D->y) / (point_E->x - point_D->x);
      k2 = -1 / k1;
      x1 = (point_D->y - y + k2 * x - k1 * point_D->x) / (k2 - k1);
      y1 = (x - point_D->x + k1 * y - k2 * point_D->y) / (k1 - k2);
      d = (double)sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
      return d;
    }
  }
  return d;
}
//计算斜率（参数四个顶点）
int Caculate_Fourpoints_X_L(struct Point_Link* point_F, struct Point_Link* point_G,
    struct Point_Link* point_H, struct Point_Link* point_I)
{
  double u, v, k;
  double k1, k2, k3, k4;
  if ((point_F->x == point_G->x) && (point_H->x == point_I->x)) {
    d_Point_one->d_flag = 1;
  }
  if ((point_F->x == point_G->x) && (point_H->x != point_I->x)) {
    u = point_F->x;
    k = (point_I->y - point_H->y) / (point_I->x - point_H->x);
    v = point_H->y + k * (u - point_H->x);
    d_Point_one->d_k = (d_Point_one->y - v) / (d_Point_one->x - u);
  }
  if ((point_F->x != point_G->x) && (point_H->x == point_I->x)) {
    v = point_H->x;
    k = (point_F->y - point_G->y) / (point_F->x - point_G->x);
    v = point_F->y + k * (u - point_F->x);
    d_Point_one->d_k = (d_Point_one->y - v) / (d_Point_one->x - u);
  }
  if ((point_F->x != point_G->x) && (point_H->x != point_I->x)) {
    k1 = (point_F->y - point_G->y) / (point_F->x - point_G->x);
    k2 = (point_I->y - point_H->y) / (point_I->x - point_H->x);
    if ((k1 + k2 == 0) && (k1 != 0)) { //相反但不为0
      if ((point_F->x > point_G->x && point_I->x < point_H->x)
          || (point_F->x < point_G->x && point_I->x > point_H->x)) {
        d_Point_one->d_flag = 1;
      }
      else {
        d_Point_one->d_k = 0.0;
      }
    }
    else {
      if (k1 == k2) { //相等
        d_Point_one->d_k = k1;
      }
      else {
        k3 = (double)((k1 * k2 - 1 + sqrt(1 + k1 * k1 + k2 * k2 + k1 * k1 * k2 * k2))
            / (k1 + k2));
        k4 = (double)((k1 * k2 - 1 - sqrt(1 + k1 * k1 + k2 * k2 + k1 * k1 * k2 * k2))
            / (k1 + k2));
        if (k1 >= 0 && k2 >= 0) { //
          if ((point_F->x > point_G->x && point_I->x > point_H->x)
              || (point_F->x < point_G->x && point_I->x < point_H->x)) {
            d_Point_one->d_k = (k3 > k4) ? k3 : k4;
          }
          else {
            d_Point_one->d_k = (k3 > k4) ? k4 : k3;
          }
        }
        if (k1 <= 0 && k2 <= 0) { //
          if ((point_F->x > point_G->x && point_I->x < point_H->x)
              || (point_F->x < point_G->x && point_I->x > point_H->x)) {
            d_Point_one->d_k = (k3 > k4) ? k3 : k4;
          }
          else {
            d_Point_one->d_k = (k3 > k4) ? k4 : k3;
          }
        }
        if (k1 >= 0 && k2 <= 0) {
          if ((point_F->x > point_G->x && point_I->x < point_H->x)
              || (point_F->x < point_G->x && point_I->x > point_H->x)) {
            if ((k3 > k1 && k3 < k2) || (k3 < k1 && k3 > k2)) { //取两端!
              d_Point_one->d_k = k4;
            }
            else {
              d_Point_one->d_k = k3;
            }
          }
          else { //取中间
            if ((k3 > k1 && k3 < k2) || (k3 < k1 && k3 > k2)) {
              d_Point_one->d_k = k3;
            }
            else {
              d_Point_one->d_k = k4;
            }
          }
        }
        if (k1 <= 0 && k2 >= 0) {
          if ((point_F->x > point_G->x && point_I->x < point_H->x)
              || (point_F->x < point_G->x && point_I->x > point_H->x)) {
            if ((k3 > k1 && k3 < k2) || (k3 < k1 && k3 > k2)) { //取两端!
              d_Point_one->d_k = k4;
            }
            else {
              d_Point_one->d_k = k3;
            }
          }
          else {
            if ((k3 > k1 && k3 < k2) || (k3 < k1 && k3 > k2)) { //取中间
              d_Point_one->d_k = k3;
            }
            else {
              d_Point_one->d_k = k4;
            }
          }
        }
      }
    }
  }
  return 0;
}
//计算第一个点
int Caculate_First_Point(struct Draw_Point* d_Point_temp, struct Point_Link* point_P,
    struct Point_Link* point_Q)
{
  if (point_P->flag != 1 && point_Q->flag != 1) {
    d_Point_temp->x = (point_Q->y - point_P->y + point_P->k * point_P->x
        - point_Q->k * point_Q->x)
        / (point_P->k - point_Q->k);
    d_Point_temp->y = (point_P->k * point_Q->k * (point_Q->x - point_P->x)
        + point_Q->k * point_P->y - point_P->k * point_Q->y)
        / (point_Q->k - point_P->k);
  }
  if (point_P->flag == 1) { //如果该点的斜率为无穷大
    d_Point_temp->x = point_P->x;
    d_Point_temp->y = point_Q->k * (d_Point_temp->x - point_Q->x) + point_Q->y;
  }
  if (point_Q->flag == 1) {
    d_Point_temp->x = point_Q->x;
    d_Point_temp->y = point_P->k * (d_Point_temp->x - point_P->x) + point_P->y;
  }
  return 0;
}
// 比较远近
void Compare_Distance(struct Draw_Point* d_Point_tail, struct Point_Link* point_J,
    struct Point_Link* point_K)
{
  struct Draw_Point* d_Point_temp;
  double x1, y1, x2, y2; //保存相交的两个点
  double d1, d2;
  d_Point_temp = (struct Draw_Point*)pmalloc(sizeof(struct Draw_Point));
  d_Point_one->next = d_Point_temp;
  d_Point_temp->d_flag = 0;
  d_Point_temp->next = NULL;
  if (d_Point_tail->d_flag != 1 && point_J->flag != 1) {
    x1 = (d_Point_tail->y - point_J->y + point_J->k * point_J->x
        - d_Point_tail->d_k * d_Point_tail->x) / (point_J->k - d_Point_tail->d_k);
    y1 = (point_J->k * d_Point_tail->d_k * (d_Point_tail->x - point_J->x)
        + d_Point_tail->d_k * point_J->y - point_J->k * d_Point_tail->y)
        / (d_Point_tail->d_k - point_J->k);
  }
  if (d_Point_tail->d_flag == 1 && point_J->flag != 1) {
    x1 = d_Point_tail->x;
    y1 = point_J->k * (x1 - point_J->x) + point_J->y;
  }
  if (d_Point_tail->d_flag != 1 && point_J->flag == 1) {
    x1 = point_J->x;
    y1 = d_Point_tail->d_k * (x1 - d_Point_tail->x) + d_Point_tail->y;
  }
  if (d_Point_tail->d_flag != 1 && point_K->flag != 1) {
    x2 = (d_Point_tail->y - point_K->y + point_K->k * point_K->x
        - d_Point_tail->d_k * d_Point_tail->x) / (point_K->k - d_Point_tail->d_k);
    y2 = (point_K->k * d_Point_tail->d_k * (d_Point_tail->x - point_K->x)
        + d_Point_tail->d_k * point_K->y - point_K->k * d_Point_tail->y)
        / (d_Point_tail->d_k - point_K->k);
  }
  if (d_Point_tail->d_flag == 1 && point_K->flag != 1) {
    x2 = d_Point_tail->x;
    y2 = point_K->k * (x2 - point_K->x) + point_K->y;
  }
  if (d_Point_tail->d_flag != 1 && point_K->flag == 1) {
    x2 = point_K->x;
    y2 = d_Point_tail->d_k * (x2 - d_Point_tail->x) + d_Point_tail->y;
  }
  //分别计算point_J 和 point_K 到d_Point_temp 的距离，取其小者，保存
  d1 = (double)sqrt((d_Point_tail->x - x1) * (d_Point_tail->x - x1) +
      (d_Point_tail->y - y1) * (d_Point_tail->y - y1));
  d2 = (double)sqrt((d_Point_tail->x - x2) * (d_Point_tail->x - x2) +
      (d_Point_tail->y - y2) * (d_Point_tail->y - y2));
  if (d1 <= d2) {
    d_Point_one->num = point_J->num;
    d_Point_temp->x = x1;
    d_Point_temp->y = y1;
  }
  else {
    d_Point_one->num = point_K->num;
    d_Point_temp->x = x2;
    d_Point_temp->y = y2;
  }
}
//判断非临边是否相交
bool Cross_Or_Not(struct Point_Link* point_L)
{
  struct Point_Link* first_Pointer, *second_Pointer, *third_Pointer, *forth_Pointer;
  first_Pointer = point_L;
  second_Pointer = first_Pointer->p_succ;
  third_Pointer = second_Pointer->p_succ;
  forth_Pointer = third_Pointer->p_succ;
  while (second_Pointer != point_L->p_pre) {
    while (forth_Pointer != first_Pointer) {
      double Delta = (second_Pointer->x - first_Pointer->x) * (third_Pointer->y - forth_Pointer->y)
          + (second_Pointer->y - first_Pointer->y) * (forth_Pointer->x - third_Pointer->x);
      if (Delta == 0) {
      }
      else {
        double s = ((third_Pointer->x - first_Pointer->x) * (third_Pointer->y - forth_Pointer->y)
            + (third_Pointer->y - first_Pointer->y) * (forth_Pointer->x - third_Pointer->x))
            / Delta;
        double t = ((second_Pointer->x - first_Pointer->x) * (third_Pointer->y - first_Pointer->y)
            - (second_Pointer->y - first_Pointer->y) * (third_Pointer->x - first_Pointer->x))
            / Delta;
        if ((s >= 0 && s <= 1) && (t >= 0 && t <= 1)) {
          return 1; //有交点立即返回
        }
        else {
        }
      }
      third_Pointer = third_Pointer->p_succ;
      forth_Pointer = third_Pointer->p_succ;
    }
    first_Pointer = first_Pointer->p_succ;
    second_Pointer = first_Pointer->p_succ;
    third_Pointer = second_Pointer->p_succ;
    forth_Pointer = third_Pointer->p_succ;
  }
  return 0;
}
// 凸凹点判断
char Tu_Ao_Caculate(struct Point_Link* point_L, struct Point_Link* point_M,
    struct Point_Link* point_N)
{
  double s;
  s = -(point_L->x * (point_M->y - point_N->y)
      - point_M->x * (point_L->y - point_N->y)
      + point_N->x * (point_L->y - point_M->y));
  if (s > 0) {
    return('t');
  }
  else {
    return('a');
  }
}
//逆时针排列
BOOL Clockwise(struct Point_Link* point_M)
{
  struct Point_Link* first_Pointer, *second_Pointer, *third_Pointer;
  first_Pointer = point_M;
  second_Pointer = first_Pointer->p_succ;
  third_Pointer = second_Pointer->p_succ;
  if (second_Pointer->x == first_Pointer->x) {
    if ((second_Pointer->y > first_Pointer->y && third_Pointer->x < first_Pointer->x)
        || (second_Pointer->y < first_Pointer->y && third_Pointer->x > first_Pointer->x)) { //顺时针
      return 1;
    }
  }
  else {
    if (second_Pointer->x > first_Pointer->x &&
        (second_Pointer->y - first_Pointer->y) * (third_Pointer->x - first_Pointer->x)
        / (second_Pointer->x - first_Pointer->x) + first_Pointer->y - third_Pointer->y < 0) {
      return 1;
    }
    else {
      if (second_Pointer->x < first_Pointer->x &&
          (second_Pointer->y - first_Pointer->y) * (third_Pointer->x - first_Pointer->x)
          / (second_Pointer->x - first_Pointer->x) + first_Pointer->y - third_Pointer->y > 0) {
        return 1;
      }
      else {
        return 0;
      }
    }
  }
  return 0;
}
int OnExecute(int n, POINT* pt)
{
  int i;
  for (i = 0; i < n; ++i) {
    POINT point = pt[i];
    struct Point_Link* p_tail;
    vertex++;
    p_tail = (struct Point_Link*)pmalloc(sizeof(struct Point_Link));
    p_tail->x = point.x;
    p_tail->y = point.y;
    p_tail->num = vertex - 1;
    p_tail->flag = 0;
    p_temp->p_succ = p_tail;
    p_tail->p_pre = p_temp;
    p_temp = p_tail;
    p_tail = NULL;
  }
  // TODO: Add your command handler code here
  p_temp->p_succ = p_head;
  p_head->p_pre = p_temp;
  //判别非临边是否相交
  if (Cross_Or_Not(p_head)) {
    printf("非临边有交点!");
    return 0;
    //程序重新执行
  }
  //给出逆时针序列
  if (Clockwise(p_head)) {
    printf("请逆时针输入！");
    return 0;
  }
  //判别是否有凸点
  p_temp = p_head;
  while (p_temp != p_head->p_pre) {
    if (Tu_Ao_Caculate(p_temp->p_pre, p_temp, p_temp->p_succ) == 'a') {
      printf("此多边行有凹点！");
      return 0;
      //程序重新执行
    }
    else {
      p_temp = p_temp->p_succ;
    }
  }
  if (Tu_Ao_Caculate(p_temp->p_pre, p_temp, p_temp->p_succ) == 'a') {
    printf("此多边行有凹点！");
    return 0;
    //程序重新执行
  }
  //定义边表头指针
  int a = 1;
  struct Point_Link* pointer;
  pointer = p_head;
  p_temp = p_head->p_succ;
  //边表的创建
  struct Edge_Link* e_head, *e_tail, *e_temp;
  e_temp = (struct Edge_Link*)pmalloc(sizeof(struct Edge_Link));
  e_temp->n0 = 0;
  e_temp->n1 = 1;
  e_head = e_temp;
  e_tail = e_temp;
  e_temp = NULL;
  while (p_temp != p_head->p_pre) {
    pointer = p_temp;
    p_temp = p_temp->p_succ;
    e_temp = (struct Edge_Link*)pmalloc(sizeof(struct Edge_Link));
    e_temp->n0 = a;
    e_temp->n1 = a + 1;
    e_tail->e_succ = e_temp;
    e_temp->e_pre = e_tail;
    e_tail = e_temp;
    e_temp = NULL;
    a++;
  }
  e_temp = (struct Edge_Link*)pmalloc(sizeof(struct Edge_Link));
  e_temp->n0 = a;
  e_temp->n1 = 0;
  e_tail->e_succ = e_temp;
  e_temp->e_pre = e_tail;
  e_tail = e_temp;
  e_temp = NULL;
  e_tail->e_succ = e_head;
  e_head->e_pre = e_tail;
  //计算每个点的角平分线的斜率
  p_temp = p_head;
  while (p_temp != p_head->p_pre) {
    p_temp->k = Caculate_X_L(p_temp->p_pre, p_temp, p_temp->p_succ);
    p_temp = p_temp->p_succ;
  }
  p_temp->k = Caculate_X_L(p_temp->p_pre, p_temp, p_temp->p_succ);
  p_temp = NULL;
  //计算相临顶点角平分线的交点及交点到两顶点的距离
  e_temp = e_head;
  p_temp = p_head;
  pointer = p_temp->p_succ;
  while (e_temp != e_head->e_pre) {
    e_temp->d = Caculate_D(p_temp, pointer);
    e_temp = e_temp->e_succ;
    p_temp = pointer;
    pointer = p_temp->p_succ;
  }
  e_temp->d = Caculate_D(p_temp, pointer);
  p_temp = NULL;
  pointer = NULL;
  e_temp = e_head;
  //比较d的值，求出最小的d对应的边
  double b = e_temp->d;
  int c = e_temp->n0;
  e_temp = e_head->e_succ;
  while (e_temp != e_head) {
    if (e_temp->d < b) {
      b = e_temp->d;
      c = e_temp->n0;
    }
    e_temp = e_temp->e_succ;
  }
  //移动点头指针p_head 和 e_head 到c处
  while (p_head->num != c) {
    p_head = p_head->p_succ;
  }
  while (e_head->n0 != c) {
    e_head = e_head->e_succ;
  }
  //输出中轴
  struct Edge_Link* e_First_Pointer, *e_Second_Pointer;
  e_First_Pointer = e_head;
  p_temp = p_head;
  pointer = p_head->p_succ;
  //计算第一个非页结点 保存在d_Point_one中
  d_Point_one = (struct Draw_Point*)pmalloc(sizeof(struct Draw_Point));
  d_Point_one->num = 0;
  d_Point_one->d_flag = 0;
  d_Point_one->next = NULL;
  Caculate_First_Point(d_Point_one, p_temp, pointer);
  //设置两个边指针，进行移动
  e_First_Pointer = e_head->e_pre;
  e_Second_Pointer = e_head->e_succ;
  struct Point_Link* first_pointer, *second_pointer, *third_pointer, *forth_pointer;
  first_pointer = p_head;
  second_pointer = p_head->p_succ;
  third_pointer = first_pointer->p_pre;
  forth_pointer = second_pointer->p_succ;
  struct Draw_Point* d_point_temp; //在画线的时候用到了两个结构Draw_Point 指针：d_Point_one 和 d_point_temp
  while (third_pointer->num != forth_pointer->num) {
    //double i,j;
    Caculate_Fourpoints_X_L(third_pointer, first_pointer,
        second_pointer, forth_pointer);
    Compare_Distance(d_Point_one, third_pointer, forth_pointer);
    if (third_pointer->num == d_Point_one->num) {
      first_pointer = first_pointer->p_pre;
      third_pointer = first_pointer->p_pre;
    }
    else {
      second_pointer = second_pointer->p_succ;
      forth_pointer = second_pointer->p_succ;
    }
    d_point_temp = d_Point_one->next;
    d_Point_one->next = NULL;
    d_Point_one = d_point_temp;
  }
  return 0;
}

