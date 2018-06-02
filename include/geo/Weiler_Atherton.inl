const double INFINITY = 1e10;
typedef struct point_ii {
  double x, y, t1, t2;
  bool isIntersection, visited;
  struct point_ii* next, * equal;
} point_ii;
int dblcmp1(double d)
{
  if (fabs(d) < 0.0000001) {
    return 0;
  }
  return (d > 0) ? 1 : -1;
}
double det(double x1, double y1, double x2, double y2)
{
  return x1 * y2 - x2 * y1;
}
double cross(point_ii* a, point_ii* b, point_ii* c) //向量ba与ca的叉乘
{
  return det(b->x - a->x, b->y - a->y, c->x - a->x, c->y - a->y);
}
double cos_a(point_ii* a, point_ii* b, point_ii* c) //向量ba与ca的夹角余弦值
{
  double x1 = b->x - a->x, y1 = b->y - a->y, x2 = c->x - a->x, y2 = c->y - a->y;
  return (x1 * x2 + y1 * y2) / (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2));
}
bool isIntersected(point_ii* a, point_ii* b, point_ii* c, point_ii* d) //判断线段是否相交
{
  int c1 = dblcmp(cross(a, c, d)), c2 = dblcmp(cross(b, c, d)), c3 = dblcmp(cross(c, a, b)), c4 = dblcmp(cross(d, a, b));
  if (((c1 ^ c2) == -2 || ((c1 & c2) == 0 && (c1 | c2) != 0)) && ((c3 ^ c4) == -2 || ((c3 & c4) == 0 && (c1 | c2) != 0))) {
    return 1;
  }
  return 0;
}
bool IsOnline(point_ii* p, point_ii* p1, point_ii* p2) //判断点是否在线段上
{
  return(!dblcmp(cross(p, p1, p2)) && ((p->x - p1->x) * (p->x - p2->x) <= 0) && ((p->y - p1->y) * (p->y - p2->y) <= 0));
}
bool segcross(point_ii* a, point_ii* b, point_ii* c, point_ii* d, point_ii* i)
{
  if (isIntersected(a, b, c, d)) { //若相交，使用参数方程求交点坐标
    double dx1 = b->x - a->x, dx2 = d->x - c->x, dx3 = c->x - a->x, dy1 = b->y - a->y, dy2 = d->y - c->y, dy3 = c->y - a->y;
    double dx = dy1 * dx2 - dx1 * dy2;
    i->t1 = (dx2 * dy3 - dx3 * dy2) / dx;
    i->t2 = (dx1 * dy3 - dx3 * dy1) / dx;
    i->x = a->x + i->t1 * dx1;
    i->y = a->y + i->t1 * dy1;
    i->isIntersection = 1;
    i->visited = 0;
    i->next = i->equal = NULL;
    return 1;
  }
  return 0;
}
point_ii* add(point_ii* p, point_ii* q, point_ii* i, int type) //将交点按顺序插入点p与点q之间
{
  point_ii* newpoint = MALLOC(point_ii, 1), * r = p;
  *(newpoint) = *i;
  if (type == 1) {
    while (r->next != q && (r->next)->t1 <= i->t1) {
      r = r->next;
    }
  }
  else {
    while (r->next != q && (r->next)->t2 <= i->t2) {
      r = r->next;
    }
  }
  newpoint->next = r->next;
  r->next = newpoint;
  return newpoint;
}
void Intersect(point_ii* p, point_ii* q, point_ii* main) //检查裁剪多边形的边pq是否与主多变形相交，若相交，将交点按顺序插入两个多边形的顶点表中
{
  point_ii* j = main, * k, * e1, * e2;
  point_ii i[1] = {0};
  while (j) {
    k = j->next;
    while (k->isIntersection) {
      k = k->next;
    }
    if (j->equal && IsOnline(j, p, q)) { //j为一个主多边形的一个起点，且点j在pq边上
      j = k;
      continue;
    }
    if (IsOnline(k, p, q)) { //若k点在pq边上
      point_ii* temp = k->next;
      if (k->equal) { //k为主多边形的一个终点
        temp = (k->equal)->next;
      }
      while (temp->isIntersection) { //寻找jk边的下一条边
        temp = temp->next;
      }
      *i = *k;
      if (q->x != p->x) {
        i->t1 = (k->x - p->x) / (q->x - p->x);
      }
      else {
        i->t1 = (k->y - p->y) / (q->y - p->y);
      }
      i->isIntersection = 1;
      i->next = i->equal = NULL;
      if (cos_a(k, j, p) < cos_a(k, temp, p)) { //调整交点插入的顺序
        i->t2 = 0;
        e1 = add(p, q, i, 1);
        if (k->equal) {
          e2 = add(k->equal, temp, i, 2);
        }
        else {
          e2 = add(k, temp, i, 2);
        }
        e1->equal = e2;
        e2->equal = e1;
        i->t2 = 1;
        e1 = add(p, q, i, 1);
        e2 = add(j, k, i, 2);
        e1->equal = e2;
        e2->equal = e1;
      }
      else {
        i->t2 = 1;
        e1 = add(p, q, i, 1);
        e2 = add(j, k, i, 2);
        e1->equal = e2;
        e2->equal = e1;
        i->t2 = 0;
        e1 = add(p, q, i, 1);
        if (k->equal) {
          e2 = add(k->equal, temp, i, 2);
        }
        else {
          e2 = add(k, temp, i, 2);
        }
        e1->equal = e2;
        e2->equal = e1;
      }
      if (k->equal) {
        j = k->next; //若k为主多边形的一个终点，将j赋为下一个起点
      }
      else {
        j = temp;
        if (j->equal) {
          j = j->next; //若j为终点，将j赋为下一个起点
        }
      }
      continue;
    }
    if (segcross(p, q, j, k, i)) { //规范相交
      e1 = add(p, q, i, 1);
      e2 = add(j, k, i, 2);
      e1->equal = e2;
      e2->equal = e1;
    }
    j = k;
    if (j->equal) {
      j = j->next; //若j为终点，将j赋为下一个起点
    }
  }
}
void getVertex(DPOINT* main, int mainlength, DPOINT* clip, int cliplength, point_ii* mainhead, point_ii* cliphead) //求主多边形与裁剪多边形的顶点表
{
  int i;
  point_ii* p, * r, * start;
  bool flag = 0;
  mainhead->x = main[0].x;
  mainhead->y = main[0].y;
  mainhead->isIntersection = mainhead->visited = false;
  mainhead->next = mainhead->equal = NULL;
  start = r = mainhead; //初始化主多边形的顶点表
  for (i = 1; i < mainlength; i++) {
    p = MALLOC(point_ii, 1);
    p->x = main[i].x;
    p->y = main[i].y;
    p->isIntersection = p->visited = false;
    p->next = p->equal = NULL;
    if (flag) {
      start = p;
      flag = 0;
    }
    if (p->x == start->x && p->y == start->y) {
      p->equal = start;
      start->equal = p;
      flag = 1;
    }
    r->next = p;
    r = p;
  }
  p->equal = start;
  start->equal = p;
  flag = 1;
  cliphead->x = clip[0].x; //初始化裁剪多边形的顶点表
  cliphead->y = clip[0].y;
  cliphead->isIntersection = cliphead->visited = false;
  cliphead->next = cliphead->equal = NULL;
  r = cliphead;
  for (i = 1; i < cliplength; i++) {
    p = MALLOC(point_ii, 1);
    p->x = clip[i].x;
    p->y = clip[i].y;
    p->isIntersection = p->visited = false;
    p->next = p->equal = NULL;
    r->next = p;
    r = p;
  }
  start = r = cliphead; //依次对裁剪多边形的各条边求交
  while (r) {
    p = r->next;
    Intersect(r, p, mainhead);
    r = p;
    if (r->x == start->x && r->y == start->y) {
      r->equal = start;
      start->equal = r;
      r = r->next;
      start = r;
    }
  }
}
bool isIntheclip(point_ii* p, DPOINT* clip, int cliplength) //使用射线法判断点p是否在裁剪多边形内
{
  int count = 0, i, start = 0;
  point_ii p1[1] = {0}, p2[1] = {0}, l[1] = {0};
  l->y = p->y;
  l->x = INFINITY;
  for (i = 0; i < cliplength - 1; i++) {
    p1->x = clip[i].x;
    p1->y = clip[i].y;
    p2->x = clip[i + 1].x;
    p2->y = clip[i + 1].y;
    if (IsOnline(p, p1, p2)) {
      return 1;
    }
    if (clip[i + 1].x == clip[start].x && clip[i + 1].y == clip[start].y) {
      i++;
      start = i + 1;
    }
    if (!dblcmp(p1->y - p2->y)) {
      continue;
    }
    if (IsOnline(p1, p, l)) {
      if (p1->y > p2->y) {
        count++;
      }
    }
    else if (IsOnline(p2, p, l)) {
      if (p2->y > p1->y) {
        count++;
      }
    }
    else if (isIntersected(p, l, p1, p2)) {
      count++;
    }
  }
  if (count % 2 == 1) {
    return 1;
  }
  return 0;
}
void display(DPOINT* pointarray, int length) //输出裁剪后的多边形
{
  int i;
  for (i = 0; i < length; i++) {
    printf("(%f,%f)", pointarray[i].x, pointarray[i].y);
  }
  printf("\n");
  printf("\n");
}
void print(point_ii* head) //输出顶点列表
{
  point_ii* r = head;
  while (r) {
    printf("(%f,%f) ", r->x, r->y);
    r = r->next;
  }
  printf("\n");
  printf("\n");
}
void output(point_ii* main, point_ii* clip, DPOINT* clipArray, int cliplength, poly2d_t* po) //裁剪
{
  point_ii* r = main, * p;
  DPOINT* outArray = MALLOC(DPOINT, 3000);
  int outlength = 0;
  bool type = 0;
  while (r) {
    while (r && (!r->isIntersection || r->visited)) { //在主多边形的顶点表中寻找第一个未访问过的交点
      p = r;
      r = r->next;
    }
    if (!r) {
      break;
    }
    //若其前一个点为主多边形的顶点，且其在裁剪多边形内，则该交点为出点，将边的起点转到裁剪多边形上
    if (!p->isIntersection && isIntheclip(p, clipArray, cliplength) && ((r->x != p->x) || (r->y != p->y))) {
      r = r->equal;
      type = 1;
    }
    outArray[outlength].x = r->x;
    outArray[outlength++].y = r->y;
    r->visited = 1;
    r = r->next;
    while (r) {
      while (!r->isIntersection && !r->visited) { //若r为多边形的顶点，则直接输出到结果多边形中
        outArray[outlength].x = r->x;
        outArray[outlength++].y = r->y;
        r->visited = 1;
        if (r->equal) {
          r = r->equal; //若r为多边形的一个终点，则转到与之相同的起点处
        }
        r = r->next;
      }
      if (r->visited) {
        break;
      }
      if (r->x != outArray[outlength - 1].x || r->y != outArray[outlength - 1].y) {
        outArray[outlength].x = r->x;
        outArray[outlength++].y = r->y;
      }
      r->visited = 1;
      (r->equal)->visited = 1; //转换到另一多边形的顶点表中
      r = (r->equal)->next;
    }
    //裁剪出了一个结果多边形，将其输出
    //display(outArray, outlength);
    poly2d_addpts(po, outlength, outArray);
    outlength = 0;
    if (type) { //若此时r为裁剪多边形顶点表中的点，将其转换为对应的主多边形中的点
      r = r->equal;
      type = 0;
    }
    p = r;
    r = r->next;
  }
  r = main;
  while (r) {
    p = r;
    r = r->next;
    pfree(p);
  }
  r = clip;
  while (r) {
    p = r;
    r = r->next;
    pfree(p);
  }
  pfree(outArray);
}
void polygonClip(DPOINT* main, int mainlength, DPOINT* clip, int cliplength, poly2d_t* po) //给出封闭的主多边形和裁剪多边形的顶点数组，对多边形进行裁剪
{
  point_ii* mainhead = MALLOC(point_ii, 1), * cliphead = MALLOC(point_ii, 1);
  getVertex(main, mainlength, clip, cliplength, mainhead, cliphead); //获得主多边形和裁剪多边形的顶点表
  output(mainhead, cliphead, clip, cliplength, po); //输出裁剪结果
}
int test_Weiler_Atherton()
{
  poly2d_t po[1] = {0};
  DPOINT main[] = {{1, 3.5}, {2, 2.5}, {1, 0}, {5, 2}, {2.7, 2.7}, {3, 4}, {1, 3.5}}, clip[] = {{1, 4}, {1, 1}, {4, 1}, {4, 4}, {1, 4}, {2, 3}, {3, 3}, {3, 2}, {2, 2}, {2, 3}};
  int mainlen = countof(main), cliplen = countof(clip);
  polygonClip(main, mainlen, clip, cliplen, po);
  free_poly2d(po);
  return 0;
}

